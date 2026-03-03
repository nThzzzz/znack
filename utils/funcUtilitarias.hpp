
#ifndef FUNC_UTILS_HPP
#define FUNC_UTILS_HPP

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

#endif
