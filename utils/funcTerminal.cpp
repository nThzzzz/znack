#include "funcTerminal.hpp"

#include <csignal>
#include <cstdlib>
#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

static struct termios orig_termios;

void resetTerminal() {
  tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
  cout << "\033[?25h" << "\033[0m" << flush;
}

// Ctrl+C mata o processo sem passar pelo atexit, entao o terminal ficaria
// sem eco e sem cursor depois do jogo.
static void aoReceberSinal(int sinal) {
  resetTerminal();
  cout << "\033[2J\033[H" << flush;
  signal(sinal, SIG_DFL);
  raise(sinal);
}

void setupTerminal() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  atexit(resetTerminal);
  signal(SIGINT, aoReceberSinal);
  signal(SIGTERM, aoReceberSinal);

  struct termios new_termios = orig_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);

  // Leitura sem bloqueio: o read volta na hora mesmo sem tecla nenhuma.
  // Usar O_NONBLOCK no stdin nao serve aqui porque fd 0, 1 e 2 compartilham a
  // mesma descricao de arquivo do terminal — o stdout ficaria nao bloqueante
  // junto e os quadros maiores que o buffer do tty seriam descartados.
  new_termios.c_cc[VMIN] = 0;
  new_termios.c_cc[VTIME] = 0;

  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  cout << "\033[?25l" << "\033[2J" << flush;
}

// Esvazia tudo que o usuario digitou desde o ultimo frame de uma vez so,
// senao as setas (3 bytes) levariam tres frames para chegar.
int lerTeclas(char *buffer, int tamanhoBuffer) {
  int lidos = read(STDIN_FILENO, buffer, tamanhoBuffer);
  return (lidos > 0) ? lidos : 0;
}
