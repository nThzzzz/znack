# 🐍 Znack

> "Znack é só um passatempo que uso quando não tenho nada pra fazer. Inicialmente fiz ele para aprender a mexer no nvim, vou colocando o que vier na cabeça."

**Znack** é uma implementação simples e direta do clássico "Jogo da Cobrinha" (Snake), rodando inteiramente no terminal e escrito em C++.

## 💻 Sobre o Projeto

Este projeto nasceu da vontade de praticar a edição de código e movimentação usando o **Neovim** e acabou virando um playground para testar lógica em C++. O foco não é ser uma engine complexa, mas sim um código divertido para passar o tempo.

O jogo utiliza manipulação de terminal em nível baixo (`termios.h`, `unistd.h`) para permitir leitura de teclas em tempo real (sem precisar apertar Enter) e renderização frame a frame.

## 🚀 Funcionalidades

- **Controle Fluido:** Movimentação via teclas (WASD) sem bloqueio de input.
- **Tamanho Customizável:** Você define o tamanho do grid (NxN) ao iniciar o jogo.
- **Renderização ASCII:** Gráficos minimalistas direto no shell.
- **Lógica de Colisão:** Game Over ao bater nas paredes.

## ⚙️ Pré-requisitos

O código faz uso de bibliotecas de sistema Unix (`<termios.h>`, `<unistd.h>`, `<fcntl.h>`). Por isso, ele roda nativamente em:
- **macOS**
- **Linux**
- *(Windows apenas via WSL)*

Você precisará de um compilador C++ instalado (como `g++` ou `clang++`).

## 🛠️ Como Rodar

1. **Clone o repositório:**
   ```bash
   git clone [https://github.com/SEU_USUARIO/znack.git](https://github.com/SEU_USUARIO/znack.git)
   cd znack
