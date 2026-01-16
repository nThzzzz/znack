#include <stdlib.h>
#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <termios.h> 
#include <unistd.h>  
#include <fcntl.h>   
#include <ctime>
#include <cstdlib>

using namespace std;

struct termios orig_termios;

void resetTerminal() {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
    cout << "\033[?25h"; 
}

void setupTerminal() {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(resetTerminal);

    struct termios new_termios = orig_termios;
    new_termios.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);

    cout << "\033[?25l"; 
}

bool verificaPosicaCobrinha(vector<pair<int, int>> &posicoesCobra, const pair<int, int> &posicao);

int main(){
  
  int tamanho;
  cout << "Digite o tamanho do campo NxN: " << endl;
  cin >> tamanho;
  cin.ignore();

  setupTerminal();

  vector<pair<int, int>> posicoesCobrinha;
  posicoesCobrinha.push_back(make_pair(tamanho/2, tamanho/2));
  
  int dirX = 0;
  int dirY = 1;
  char tecla;
  bool sla = true;
  bool sla2 = false;
  int x, y;

  while(sla){
      srand(time(0));
      
      if(!sla2){
          x = rand() % tamanho;
          y = rand() % tamanho;
          sla2 = true;
      }

      cout << "\033[H"; 
        
      string buffer = "";
      
      for(int i=0; i<tamanho; i++){
          for(int j=0; j<tamanho; j++){
              if(verificaPosicaCobrinha(posicoesCobrinha, make_pair(i, j))){
                  buffer += "O "; 
              }else if(i==x && j==y){
                  buffer += "@ "; 
              }else{
                  buffer += ". "; 
              }
          }
          buffer += "\n";
      }
      cout << buffer << flush;

      if (read(STDIN_FILENO, &tecla, 1) > 0) {
          if (tecla == 'w' && dirX != 1) { dirX = -1; dirY = 0; }
          if (tecla == 's' && dirX != -1) { dirX = 1; dirY = 0; }
          if (tecla == 'a' && dirY != 1) { dirX = 0; dirY = -1; }
          if (tecla == 'd' && dirY != -1) { dirX = 0; dirY = 1; }
          if (tecla == 'q') break;
      }

      pair<int, int> cabeca = posicoesCobrinha[0];
      pair<int, int> cabecaNova = make_pair(cabeca.first + dirX, cabeca.second + dirY);
        
      if(cabecaNova.first > tamanho-1 || cabecaNova.first < 0 || cabecaNova.second > tamanho-1 || cabecaNova.second < 0){
         sla = false; 
      }

      posicoesCobrinha.insert(posicoesCobrinha.begin(), cabecaNova);
      if(cabecaNova.first == x && cabecaNova.second == y){
        sla2 = false;  
      }else{
        posicoesCobrinha.pop_back();
      }

      usleep(125000);
  }

  return 0;
}

bool verificaPosicaCobrinha(vector<pair<int, int>> &posicoesCobra, const pair<int, int> &posicao){
  for(int i=0; i<posicoesCobra.size(); i++){
    if((posicoesCobra[i].first == posicao.first) && (posicoesCobra[i].second == posicao.second)){
        return true;
    }
  }
  return false;
}
