#include "./utils/funcRender.hpp"
#include "./utils/funcTerminal.hpp"
#include "./utils/funcUtilitarias.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace std;

enum Resultado { SAIU, MORREU, VENCEU };

static const int VELOCIDADE_INICIAL = 130000;
static const int VELOCIDADE_MAXIMA = 60000;
static const int GANHO_POR_FRUTA = 2500;

// Um frame anda mais rapido conforme a cobra cresce.
static int intervalo(int pontos) {
  int espera = VELOCIDADE_INICIAL - pontos * GANHO_POR_FRUTA;
  return (espera < VELOCIDADE_MAXIMA) ? VELOCIDADE_MAXIMA : espera;
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
  bool pausado = false;
  pontos = 0;

  contagemRegressiva(tamanho, posicoesCobrinha, posicaoFruta);

  while (true) {
    desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, pontos,
                   pausado ? "\033[1m\033[38;5;220m|| PAUSADO \033[0m"
                             "\033[38;5;245m— [P] continuar\033[0m"
                           : "");

    char teclas[32];
    int lidos = lerTeclas(teclas, sizeof(teclas));
    bool jaVirou = false;

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
        return SAIU;
      }
      if (tecla == 'p' || tecla == 'P') {
        pausado = !pausado;
        continue;
      }

      // So a primeira virada de cada frame vale, senao dois toques rapidos
      // deixariam a cobra dar meia volta em cima do proprio corpo.
      if (jaVirou || pausado) {
        continue;
      }
      if ((tecla == 'w' || tecla == 'W') && dirXY.first != 1) {
        dirXY = make_pair(-1, 0);
        jaVirou = true;
      }
      if ((tecla == 's' || tecla == 'S') && dirXY.first != -1) {
        dirXY = make_pair(1, 0);
        jaVirou = true;
      }
      if ((tecla == 'a' || tecla == 'A') && dirXY.second != 1) {
        dirXY = make_pair(0, -1);
        jaVirou = true;
      }
      if ((tecla == 'd' || tecla == 'D') && dirXY.second != -1) {
        dirXY = make_pair(0, 1);
        jaVirou = true;
      }
    }

    if (pausado) {
      usleep(80000);
      continue;
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

    if (posicaoFruta.first < 0) {
      desenharQuadro(tamanho, posicoesCobrinha, posicaoFruta, pontos, "");
      return VENCEU;
    }

    usleep(intervalo(pontos));
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
      usleep(50000);
    }

    if (sair) {
      break;
    }
    cout << "\033[2J" << flush;
  }

  cout << "\033[2J\033[H" << flush;
  return 0;
}
