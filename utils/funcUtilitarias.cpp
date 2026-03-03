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

pair<int, int> criarFruta(int &tamanho) {
  return make_pair(rand() % tamanho, rand() % tamanho);
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
