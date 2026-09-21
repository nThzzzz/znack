#include "./utils/funcRender.hpp"
#include "./utils/funcTerminal.hpp"
#include "./utils/funcUtilitarias.hpp"

#include <chrono>
#include <cstdlib>
#include <ctime>
#include <deque>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace std;

enum Resultado { SAIU, MORREU, VENCEU };

static const int VELOCIDADE_INICIAL = 180000;
static const int VELOCIDADE_MAXIMA = 90000;
static const int GANHO_POR_FRUTA = 1500;

// De quanto em quanto tempo o teclado e cutucado dentro de um mesmo passo.
static const int PASSO_LEITURA = 5000;

static const string MSG_PAUSA = "\033[1m\033[38;5;220m|| PAUSADO \033[0m"
                                "\033[38;5;245m— [P] continuar\033[0m";

// Um frame anda mais rapido conforme a cobra cresce.
static int intervalo(int pontos) {
  int espera = VELOCIDADE_INICIAL - pontos * GANHO_POR_FRUTA;
  return (espera < VELOCIDADE_MAXIMA) ? VELOCIDADE_MAXIMA : espera;
}

// Guarda ate duas viradas na fila para que dois toques rapidos (direita e
// logo depois cima) virem em passos seguidos em vez de um deles se perder.
static void enfileirarVirada(deque<pair<int, int>> &viradas,
                             const pair<int, int> &dirAtual,
                             const pair<int, int> &nova) {
  pair<int, int> ultima = viradas.empty() ? dirAtual : viradas.back();

  // Meia volta em cima do proprio corpo nao vale, e repetir a direcao atual
  // so ocuparia uma vaga na fila a toa.
  if (ultima == nova ||
      (ultima.first == -nova.first && ultima.second == -nova.second)) {
    return;
  }
  if (viradas.size() < 2) {
    viradas.push_back(nova);
  }
}

// Le tudo que foi digitado desde a ultima chamada e traduz em viradas e
// comandos. Devolve true quando o jogador pediu para sair.
static bool processarEntrada(deque<pair<int, int>> &viradas,
                             const pair<int, int> &dirAtual, bool &pausado) {
  char teclas[32];
  int lidos = lerTeclas(teclas, sizeof(teclas));

  for (int i = 0; i < lidos; i++) {
    char tecla = teclas[i];

    // Setas chegam como ESC [ A/B/C/D — converte para o WASD equivalente.
    if (tecla == '\033' && i + 2 < lidos && teclas[i + 1] == '[') {
      switch (teclas[i + 2]) {
      case 'A':
        tecla = 'w';
        break;
      case 'B':
        tecla = 's';
        break;
      case 'C':
        tecla = 'd';
        break;
      case 'D':
        tecla = 'a';
        break;
      default:
        tecla = 0;
        break;
      }
      i += 2;
    }

    if (tecla == 'q' || tecla == 'Q') {
      return true;
    }
    if (tecla == 'p' || tecla == 'P') {
      pausado = !pausado;
      continue;
    }
    if (pausado) {
      continue;
    }

    if (tecla == 'w' || tecla == 'W') {
      enfileirarVirada(viradas, dirAtual, make_pair(-1, 0));
    }
    if (tecla == 's' || tecla == 'S') {
      enfileirarVirada(viradas, dirAtual, make_pair(1, 0));
    }
    if (tecla == 'a' || tecla == 'A') {
      enfileirarVirada(viradas, dirAtual, make_pair(0, -1));
    }
    if (tecla == 'd' || tecla == 'D') {
      enfileirarVirada(viradas, dirAtual, make_pair(0, 1));
    }
  }

  return false;
}

static void contagemRegressiva(int tamanho,
                               const vector<pair<int, int>> &posicoesCobrinha,
                               const pair<int, int> &posicaoFruta) {
  const string passos[] = {"\033[38;5;220m3\033[0m", "\033[38;5;220m2\033[0m",
                           "\033[38;5;220m1\033[0m",
                           "\033[1m\033[38;5;46mJA!\033[0m"};

  for (int i = 0; i < 4; i++) {
    desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, 0, passos[i]);
    usleep(400000);
  }
}

static Resultado jogar(int tamanho, int &pontos) {
  vector<pair<int, int>> posicoesCobrinha;
  posicoesCobrinha.push_back(make_pair(tamanho / 2, tamanho / 2));

  pair<int, int> posicaoFruta = criarFruta(posicoesCobrinha, tamanho);
  pair<int, int> dirXY = make_pair(0, 1);
  deque<pair<int, int>> viradas;
  bool pausado = false;
  pontos = 0;

  contagemRegressiva(tamanho, posicoesCobrinha, posicaoFruta);
  desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, pontos, "");

  chrono::steady_clock::time_point proximoPasso = chrono::steady_clock::now();

  while (true) {
    proximoPasso += chrono::microseconds(intervalo(pontos));

    // Se o desenho de um quadro demorou mais que o passo inteiro, nao vale
    // disparar varios passos seguidos para "recuperar" o atraso.
    if (proximoPasso < chrono::steady_clock::now()) {
      proximoPasso = chrono::steady_clock::now() +
                     chrono::microseconds(intervalo(pontos));
    }

    // Em vez de dormir o passo inteiro de uma vez, o teclado e lido a cada
    // poucos milissegundos: a tecla entra na fila quase na hora em que foi
    // apertada, e nao so quando o passo termina. O que falta sempre vem do
    // relogio — somar os usleep curtos esticaria o passo, porque cada um
    // deles dorme um pouco mais do que foi pedido.
    while (true) {
      long long falta = chrono::duration_cast<chrono::microseconds>(
                            proximoPasso - chrono::steady_clock::now())
                            .count();
      if (falta <= 0) {
        break;
      }
      usleep((useconds_t)((falta < PASSO_LEITURA) ? falta : PASSO_LEITURA));

      if (processarEntrada(viradas, dirXY, pausado)) {
        return SAIU;
      }

      if (pausado) {
        desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, pontos,
                       MSG_PAUSA);
        while (pausado) {
          usleep(PASSO_LEITURA);
          if (processarEntrada(viradas, dirXY, pausado)) {
            return SAIU;
          }
        }
        desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, pontos, "");
        proximoPasso = chrono::steady_clock::now() +
                       chrono::microseconds(intervalo(pontos));
      }
    }

    if (!viradas.empty()) {
      dirXY = viradas.front();
      viradas.pop_front();
    }

    pair<int, int> cabeca = posicoesCobrinha[0];
    pair<int, int> cabecaNova =
        make_pair(cabeca.first + dirXY.first, cabeca.second + dirXY.second);

    if (verificaMorte(posicoesCobrinha, cabecaNova, tamanho)) {
      return MORREU;
    }

    if (verificaCrescimento(posicoesCobrinha, cabecaNova, posicaoFruta,
                            tamanho)) {
      pontos++;
    }

    // Desenha logo apos o passo, e nao no comeco do proximo: senao o
    // movimento so apareceria um frame inteiro depois de ter acontecido.
    desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, pontos, "");

    if (posicaoFruta.first < 0) {
      return VENCEU;
    }
  }
}

int main() {
  srand(time(0));

  int tamanho = telaInicial();
  setupTerminal();

  while (true) {
    int pontos = 0;
    Resultado resultado = jogar(tamanho, pontos);

    if (resultado == SAIU) {
      break;
    }

    usleep(500000);
    telaFinal(pontos, resultado == VENCEU);

    bool decidiu = false;
    bool sair = false;
    while (!decidiu) {
      char teclas[32];
      int lidos = lerTeclas(teclas, sizeof(teclas));
      for (int i = 0; i < lidos; i++) {
        if (teclas[i] == 'r' || teclas[i] == 'R') {
          decidiu = true;
        }
        if (teclas[i] == 'q' || teclas[i] == 'Q') {
          decidiu = true;
          sair = true;
        }
      }
      usleep(30000);
    }

    if (sair) {
      break;
    }
    cout << "\033[2J" << flush;
  }

  cout << "\033[2J\033[H" << flush;
  return 0;
}
