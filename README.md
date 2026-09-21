# 🐍 Znack

> "Znack é só um passatempo que uso quando não tenho nada pra fazer. Inicialmente fiz ele para aprender a mexer no nvim, vou colocando o que vier na cabeça."

**Znack** é uma implementação simples e direta do clássico "Jogo da Cobrinha" (Snake), rodando inteiramente no terminal e escrito em C++.

```
╔══════════════════════════════════╗
║ 🐍 ZNACK               SCORE 007 ║
╠══════════════════════════════════╣
║ · · · · · · · · · · · · · · · ·  ║
║ · · · · ● ▪ ▪ ▪ ▪ · · · · · · ·  ║
║ · · · · · · · · ▪ · · · · · · ·  ║
║ · · · · · · · · ▪ ▪ ▪ · · ◆ · ·  ║
║ · · · · · · · · · · · · · · · ·  ║
╚══════════════════════════════════╝
  [WASD] mover   [P] pausar   [Q] sair
```

## 💻 Sobre o Projeto

Este projeto nasceu da vontade de praticar a edição de código e movimentação usando o **Neovim** e acabou virando um playground para testar lógica em C++. O foco não é ser uma engine complexa, mas sim um código divertido para passar o tempo.

O jogo utiliza manipulação de terminal em nível baixo (`termios.h`, `unistd.h`) para permitir leitura de teclas em tempo real (sem precisar apertar Enter) e renderização frame a frame.

## 🚀 Funcionalidades

- **Interface colorida:** Bordas em box-drawing, placar no topo, logo ASCII na abertura e tela de fim de jogo.
- **Controle Fluido:** Movimentação via **WASD** ou **setas**, sem bloqueio de input.
- **Tamanho Customizável:** Você define o tamanho do grid (NxN) ao iniciar o jogo.
- **Pausa:** Tecla `P` congela a partida a qualquer momento.
- **Dificuldade progressiva:** A cobra acelera conforme vai comendo.
- **Lógica de Colisão:** Game Over ao bater nas paredes ou no próprio corpo.
- **Replay:** `R` na tela final começa outra partida sem sair do jogo.

## 🎮 Controles

| Tecla           | Ação             |
| --------------- | ---------------- |
| `W` `A` `S` `D` | Mover a cobra    |
| `↑` `←` `↓` `→` | Mover a cobra    |
| `P`             | Pausar / retomar |
| `Q`             | Sair             |
| `R`             | Jogar de novo    |

## ⚙️ Pré-requisitos

O código faz uso de bibliotecas de sistema Unix (`<termios.h>`, `<unistd.h>`). Por isso, ele roda nativamente em:
- **macOS**
- **Linux**
- *(Windows apenas via WSL)*

Você precisará de um compilador C++ instalado (como `g++` ou `clang++`) e de um terminal com suporte a **UTF-8** e **cores 256** — praticamente qualquer terminal moderno.

## ▶️ Como rodar

```bash
make run     # compila e já abre o jogo
make         # só compila (gera o binário ./znack)
make clean   # limpa os .o e o binário
```

## 📂 Estrutura

```
main.cpp                    # loop do jogo, input e estados (menu, pausa, fim)
utils/funcTerminal.cpp      # modo raw do terminal e leitura não-bloqueante
utils/funcUtilitarias.cpp   # regras: movimento, fruta, colisão, crescimento
utils/funcRender.cpp        # desenho: caixa, cores, logo e telas
```
