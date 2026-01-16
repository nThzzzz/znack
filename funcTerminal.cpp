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

