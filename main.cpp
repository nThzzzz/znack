#include "funcTerminal.hpp"
#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <termios.h>
#include <unistd.h>
#include <utility>
#include <vector>

using namespace std;

bool verificaPosicaCobrinha(vector<pair<int, int>> &posicoesCobra,
                            const pair<int, int> &posicao);
void printMapa(int &tamanho, vector<pair<int, int>> &posicoesCobrinha,
               pair<int, int> &posicaoFruta);
void verificaCrescimento(vector<pair<int, int>> &posicoesCobrinha,
                         pair<int, int> &cabecaNova,
                         pair<int, int> &posicaoFruta, int &tamanho);
pair<int, int> criarFruta(int &tamanho);
bool verificaMorte(vector<pair<int, int>> &posicoesCobrinha,
                   const pair<int, int> cabecaNova, int &tamanho);

int main() {

  int tamanho;
  cout << "Digite o tamanho do campo NxN: " << endl;
  cin >> tamanho;
  cin.ignore();

  setupTerminal();

  vector<pair<int, int>> posicoesCobrinha;
  posicoesCobrinha.push_back(make_pair(tamanho / 2, tamanho / 2));

  pair<int, int> posicaoFruta = criarFruta(tamanho);

  pair<int, int> dirXY = make_pair(0, 1);
  char tecla;
  bool vivo = true;

  while (vivo) {
    srand(time(0));

    printMapa(tamanho, posicoesCobrinha, posicaoFruta);

    if (read(STDIN_FILENO, &tecla, 1) > 0) {
      if (tecla == 'w' && dirXY.first != 1) {
        dirXY = make_pair(-1, 0);
      }
      if (tecla == 's' && dirXY.first != -1) {
        dirXY = make_pair(1, 0);
      }
      if (tecla == 'a' && dirXY.second != 1) {
        dirXY = make_pair(0, -1);
      }
      if (tecla == 'd' && dirXY.second != -1) {
        dirXY = make_pair(0, 1);
      }
      if (tecla == 'q')
        break;
    }

    pair<int, int> cabeca = posicoesCobrinha[0];
    pair<int, int> cabecaNova =
        make_pair(cabeca.first + dirXY.first, cabeca.second + dirXY.second);

    vivo = verificaMorte(posicoesCobrinha, cabecaNova, tamanho);
    verificaCrescimento(posicoesCobrinha, cabecaNova, posicaoFruta, tamanho);

    usleep(125000);
  }

  return 0;
}

bool verificaPosicaCobrinha(vector<pair<int, int>> &posicoesCobra,
                            const pair<int, int> &posicao) {
  for (int i = 0; i < posicoesCobra.size(); i++) {
    if ((posicoesCobra[i].first == posicao.first) &&
        (posicoesCobra[i].second == posicao.second)) {
      return true;
    }
  }
  return false;
}

void printMapa(int &tamanho, vector<pair<int, int>> &posicoesCobrinha,
               pair<int, int> &posicaoFruta) {
  cout << "\033[H";

  string buffer = "";
  for (int i = 0; i < tamanho; i++) {
    for (int j = 0; j < tamanho; j++) {
      if (verificaPosicaCobrinha(posicoesCobrinha, make_pair(i, j))) {
        buffer += "O ";
      } else if (i == posicaoFruta.first && j == posicaoFruta.second) {
        buffer += "@ ";
      } else {
        buffer += ". ";
      }
    }
    buffer += "\n";
  }
  cout << buffer << flush;
}

void verificaCrescimento(vector<pair<int, int>> &posicoesCobrinha,
                         pair<int, int> &cabecaNova,
                         pair<int, int> &posicaoFruta, int &tamanho) {
  posicoesCobrinha.insert(posicoesCobrinha.begin(), cabecaNova);
  if (cabecaNova.first == posicaoFruta.first &&
      cabecaNova.second == posicaoFruta.second) {
    posicaoFruta = criarFruta(tamanho);
  } else {
    posicoesCobrinha.pop_back();
  }
}

pair<int, int> criarFruta(int &tamanho) {
  return make_pair(rand() % tamanho, rand() % tamanho);
}

bool verificaMorte(vector<pair<int, int>> &posicoesCobrinha,
                   const pair<int, int> cabecaNova, int &tamanho) {

  if (cabecaNova.first > tamanho - 1 || cabecaNova.first < 0 ||
      cabecaNova.second > tamanho - 1 || cabecaNova.second < 0) {
    return false;
  }

  if (verificaPosicaCobrinha(posicoesCobrinha, cabecaNova)) {
    return false;
  }

  return true;
}
