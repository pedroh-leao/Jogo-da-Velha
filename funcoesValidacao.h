//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#ifndef FUNCOESVALIDACAO_H_INCLUDED
#define FUNCOESVALIDACAO_H_INCLUDED

int charToInt(char c);

int verificaDigito(char posicao); //verifica se o usuario realmente digitou números de 1 a 3 posição, retorna 1 caso for isso, e 0 caso contrário

int verificaPosicao(int linha, int coluna, char **partida); //verifica se a posição está disponível, retorna 1 caso esteja, e 0 caso contrário

int verificaPartida(char **partida, char *jogador, int jogadorDaVez); //retorno 0: ainda não acabou, 1: vitória do jogador da vez, 2: empate

int validaNomeArquivo(char *nomeArquivo);

int validaNumJogadores(char *numJogadores); //retorna 0 caso seja um numero de jogadores invalido

void limpaBuffer(); //limpar os caracteres do buffer até \n

char **alocaMatriz(int n, int m);

void liberaMatriz(char **matriz, int n);

#endif