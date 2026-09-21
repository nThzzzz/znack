#ifndef FUNC_RENDER_HPP
#define FUNC_RENDER_HPP

#include <string>
#include <utility>
#include <vector>

using namespace std;

int telaInicial();
void desenharQuadro(int tamanho, const vector<pair<int, int>> &posicoesCobrinha,
                    const pair<int, int> &posicaoFruta, int pontos,
                    const string &mensagem);
void telaFinal(int pontos, bool venceu);

#endif
