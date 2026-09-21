#include "funcRender.hpp"

#include <cctype>
#include <iostream>
#include <limits>
#include <string>
#include <sys/ioctl.h>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace std;

static const string RESET = "\033[0m";
static const string NEGRITO = "\033[1m";
static const string BORDA = "\033[38;5;44m";
static const string CABECA = "\033[38;5;46m";
static const string CORPO = "\033[38;5;34m";
static const string FRUTA = "\033[38;5;203m";
static const string GRADE = "\033[38;5;238m";
static const string DESTAQUE = "\033[38;5;220m";
static const string APAGADO = "\033[38;5;245m";

static const int LARGURA_MIN = 26;

// Conta quantas colunas o texto ocupa na tela: ignora as sequencias ANSI e
// trata os bytes de continuacao do UTF-8 (emoji conta como duas colunas).
static int larguraVisivel(const string &texto) {
  int largura = 0;
  for (size_t i = 0; i < texto.size(); i++) {
    unsigned char byte = texto[i];
    if (byte == 0x1B) {
      while (i < texto.size() && !isalpha((unsigned char)texto[i])) {
        i++;
      }
      continue;
    }
    if ((byte & 0xC0) == 0x80) {
      continue;
    }
    largura += (byte >= 0xF0) ? 2 : 1;
  }
  return largura;
}

static string repetir(const string &pedaco, int vezes) {
  string texto = "";
  for (int i = 0; i < vezes; i++) {
    texto += pedaco;
  }
  return texto;
}

static string linhaBorda(const string &esquerda, const string &dir,
                         int largura) {
  return BORDA + esquerda + repetir("═", largura) + dir + RESET + "\n";
}

static string linhaConteudo(const string &conteudo, int largura) {
  int sobra = largura - larguraVisivel(conteudo);
  if (sobra < 0) {
    sobra = 0;
  }
  return BORDA + "║" + RESET + conteudo + string(sobra, ' ') + BORDA + "║" +
         RESET + "\n";
}

static string centralizar(const string &conteudo, int largura) {
  int sobra = largura - larguraVisivel(conteudo);
  if (sobra < 0) {
    sobra = 0;
  }
  return string(sobra / 2, ' ') + conteudo;
}

static string tresDigitos(int numero) {
  string texto = to_string(numero);
  while (texto.size() < 3) {
    texto = "0" + texto;
  }
  return texto;
}

static void desenharLogo() {
  const string logo[] = {
      "███████╗███╗   ██╗ █████╗  ██████╗██╗  ██╗",
      "╚══███╔╝████╗  ██║██╔══██╗██╔════╝██║ ██╔╝",
      "  ███╔╝ ██╔██╗ ██║███████║██║     █████╔╝ ",
      " ███╔╝  ██║╚██╗██║██╔══██║██║     ██╔═██╗ ",
      "███████╗██║ ╚████║██║  ██║╚██████╗██║  ██╗",
      "╚══════╝╚═╝  ╚═══╝╚═╝  ╚═╝ ╚═════╝╚═╝  ╚═╝"};

  cout << "\n";
  for (int i = 0; i < 6; i++) {
    cout << "  " << CABECA << logo[i] << RESET << "\n";
  }
  cout << "  " << APAGADO << "        jogo da cobrinha no terminal" << RESET
       << "\n\n";
}

// Um quadro ocupa tamanho + 6 linhas (bordas, placar e rodape) por
// 2 * tamanho + 4 colunas. Se nao couber, cada redraw rola a tela e o
// \033[H passa a mirar o topo errado, embaralhando o desenho.
static int tamanhoMaximo() {
  struct winsize janela;
  if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &janela) != 0 || janela.ws_row == 0) {
    return 40;
  }

  int porLinhas = (int)janela.ws_row - 6;
  int porColunas = ((int)janela.ws_col - 4) / 2;
  int maximo = (porLinhas < porColunas) ? porLinhas : porColunas;

  return (maximo > 40) ? 40 : maximo;
}

int telaInicial() {
  cout << "\033[2J\033[H";
  desenharLogo();

  int maximo = tamanhoMaximo();
  if (maximo < 8) {
    cout << "  " << FRUTA << "a janela do terminal e pequena demais" << RESET
         << APAGADO << " (precisa de pelo menos 20 colunas x 14 linhas)" << RESET
         << "\n\n";
    maximo = 8;
  }

  int tamanho = 0;
  while (true) {
    cout << "  " << DESTAQUE << "Tamanho do campo NxN" << RESET << APAGADO
         << " (8 a " << maximo << "): " << RESET << flush;

    if (cin >> tamanho && tamanho >= 8 && tamanho <= maximo) {
      break;
    }

    if (cin.eof()) {
      tamanho = 8;
      break;
    }

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "  " << FRUTA << "valor invalido, tenta de novo" << RESET << "\n";
  }
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  cout << "\033[2J";
  return tamanho;
}

void desenharQuadro(int tamanho, const vector<pair<int, int>> &posicoesCobrinha,
                    const pair<int, int> &posicaoFruta, int pontos,
                    const string &mensagem) {
  int largura = 2 * tamanho + 2;
  if (largura < LARGURA_MIN) {
    largura = LARGURA_MIN;
  }

  // 0 = vazio, 1 = cabeca, 2 = corpo, 3 = fruta
  vector<int> grade(tamanho * tamanho, 0);
  for (size_t i = 0; i < posicoesCobrinha.size(); i++) {
    grade[posicoesCobrinha[i].first * tamanho + posicoesCobrinha[i].second] =
        (i == 0) ? 1 : 2;
  }
  if (posicaoFruta.first >= 0) {
    grade[posicaoFruta.first * tamanho + posicaoFruta.second] = 3;
  }

  string titulo = " " + NEGRITO + CABECA + "🐍 ZNACK" + RESET;
  string placar = DESTAQUE + "SCORE " + tresDigitos(pontos) + RESET + " ";
  int vao = largura - larguraVisivel(titulo) - larguraVisivel(placar);
  if (vao < 1) {
    vao = 1;
  }

  string buffer = "\033[H";
  buffer += linhaBorda("╔", "╗", largura);
  buffer += linhaConteudo(titulo + string(vao, ' ') + placar, largura);
  buffer += linhaBorda("╠", "╣", largura);

  // Campos pequenos ficam mais estreitos que a caixa: centraliza a grade.
  int recuo = (largura - (2 * tamanho - 1)) / 2;
  if (recuo < 1) {
    recuo = 1;
  }

  for (int i = 0; i < tamanho; i++) {
    string linha = string(recuo, ' ');
    for (int j = 0; j < tamanho; j++) {
      switch (grade[i * tamanho + j]) {
      case 1:
        linha += NEGRITO + CABECA + "●" + RESET + " ";
        break;
      case 2:
        linha += CORPO + "▪" + RESET + " ";
        break;
      case 3:
        linha += NEGRITO + FRUTA + "◆" + RESET + " ";
        break;
      default:
        linha += GRADE + "·" + RESET + " ";
        break;
      }
    }
    buffer += linhaConteudo(linha, largura);
  }

  buffer += linhaBorda("╚", "╝", largura);

  string rodape = mensagem.empty()
                      ? APAGADO + "[WASD] mover   [P] pausar   [Q] sair" + RESET
                      : mensagem;
  buffer += " " + centralizar(rodape, largura - 2) + "\033[K\n";

  cout << buffer << "\033[J" << flush;
}

void telaFinal(int pontos, bool venceu) {
  const int largura = 34;

  string titulo = venceu ? DESTAQUE + "V I T O R I A" + RESET
                         : FRUTA + "G A M E   O V E R" + RESET;

  string buffer = "\033[2J\033[H\n";
  buffer += "  " + linhaBorda("╔", "╗", largura);
  buffer += "  " + linhaConteudo(centralizar(NEGRITO + titulo, largura),
                                 largura);
  buffer += "  " + linhaBorda("╠", "╣", largura);
  buffer += "  " + linhaConteudo("  Frutas comidas:   " + DESTAQUE +
                                     tresDigitos(pontos) + RESET,
                                 largura);
  buffer += "  " + linhaConteudo("  Tamanho da cobra: " + CORPO +
                                     tresDigitos(pontos + 1) + RESET,
                                 largura);
  buffer += "  " + linhaBorda("╚", "╝", largura);
  buffer += "\n  " +
            centralizar(APAGADO + "[R] jogar de novo   [Q] sair" + RESET,
                        largura) +
            "\n";

  cout << buffer << "\033[J" << flush;
}
