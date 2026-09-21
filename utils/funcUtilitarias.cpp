#include "funcUtilitarias.hpp"

#include <cstdlib>
#include <utility>
#include <vector>

using namespace std;

bool verificaPosicaCobrinha(const vector<pair<int, int>> &posicoesCobra,
                            const pair<int, int> &posicao) {
  for (size_t i = 0; i < posicoesCobra.size(); i++) {
    if ((posicoesCobra[i].first == posicao.first) &&
        (posicoesCobra[i].second == posicao.second)) {
      return true;
    }
  }
  return false;
}

// Sorteia entre as casas livres para a fruta nunca nascer em cima da cobra.
// Devolve (-1, -1) quando a cobra ja ocupa o campo inteiro.
pair<int, int> criarFruta(const vector<pair<int, int>> &posicoesCobrinha,
                          int tamanho) {
  vector<bool> ocupado(tamanho * tamanho, false);
  for (size_t i = 0; i < posicoesCobrinha.size(); i++) {
    ocupado[posicoesCobrinha[i].first * tamanho +
            posicoesCobrinha[i].second] = true;
  }

  vector<pair<int, int>> livres;
  for (int i = 0; i < tamanho; i++) {
    for (int j = 0; j < tamanho; j++) {
      if (!ocupado[i * tamanho + j]) {
        livres.push_back(make_pair(i, j));
      }
    }
  }

  if (livres.empty()) {
    return make_pair(-1, -1);
  }
  return livres[rand() % livres.size()];
}

// Anda com a cobra e devolve true se ela comeu a fruta nesse passo.
bool verificaCrescimento(vector<pair<int, int>> &posicoesCobrinha,
                         const pair<int, int> &cabecaNova,
                         pair<int, int> &posicaoFruta, int tamanho) {
  posicoesCobrinha.insert(posicoesCobrinha.begin(), cabecaNova);

  if (cabecaNova.first == posicaoFruta.first &&
      cabecaNova.second == posicaoFruta.second) {
    posicaoFruta = criarFruta(posicoesCobrinha, tamanho);
    return true;
  }

  posicoesCobrinha.pop_back();
  return false;
}

bool verificaMorte(const vector<pair<int, int>> &posicoesCobrinha,
                   const pair<int, int> &cabecaNova, int tamanho) {
  if (cabecaNova.first > tamanho - 1 || cabecaNova.first < 0 ||
      cabecaNova.second > tamanho - 1 || cabecaNova.second < 0) {
    return true;
  }

  // A ultima casa do rabo vai sair no mesmo passo, entao nao mata.
  for (size_t i = 0; i + 1 < posicoesCobrinha.size(); i++) {
    if (posicoesCobrinha[i] == cabecaNova) {
      return true;
    }
  }

  return false;
}
