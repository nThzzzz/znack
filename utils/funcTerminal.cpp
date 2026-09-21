#include "funcTerminal.hpp"

#include <cstdlib>
#include <fcntl.h>
#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

static struct termios orig_termios;
static int orig_flags = 0;

void resetTerminal() {
  tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
  fcntl(STDIN_FILENO, F_SETFL, orig_flags);
  cout << "\033[?25h" << "\033[0m" << flush;
}

void setupTerminal() {
  tcgetattr(STDIN_FILENO, &orig_termios);
  orig_flags = fcntl(STDIN_FILENO, F_GETFL, 0);
  atexit(resetTerminal);

  struct termios new_termios = orig_termios;
  new_termios.c_lflag &= ~(ICANON | ECHO);
  tcsetattr(STDIN_FILENO, TCSANOW, &new_termios);

  fcntl(STDIN_FILENO, F_SETFL, orig_flags | O_NONBLOCK);

  cout << "\033[?25l" << "\033[2J" << flush;
}

// Esvazia tudo que o usuario digitou desde o ultimo frame de uma vez so,
// senao as setas (3 bytes) levariam tres frames para chegar.
int lerTeclas(char *buffer, int tamanhoBuffer) {
  int lidos = read(STDIN_FILENO, buffer, tamanhoBuffer);
  return (lidos > 0) ? lidos : 0;
}
