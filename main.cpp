#include "./utils/funcTerminal.hpp"
#include "./utils/funcUtilitarias.hpp"

#include <cstdlib>
#include <ctime>
#include <fcntl.h>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
#include <utility>

using namespace std;

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
