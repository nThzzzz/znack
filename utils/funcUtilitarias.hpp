#ifndef FUNC_UTILS_HPP
#define FUNC_UTILS_HPP

#include <utility>
#include <vector>

using namespace std;

bool verificaPosicaCobrinha(const vector<pair<int, int>> &posicoesCobra,
                            const pair<int, int> &posicao);
pair<int, int> criarFruta(const vector<pair<int, int>> &posicoesCobrinha,
                          int tamanho);
bool verificaCrescimento(vector<pair<int, int>> &posicoesCobrinha,
                         const pair<int, int> &cabecaNova,
                         pair<int, int> &posicaoFruta, int tamanho);
bool verificaMorte(const vector<pair<int, int>> &posicoesCobrinha,
                   const pair<int, int> &cabecaNova, int tamanho);

#endif
