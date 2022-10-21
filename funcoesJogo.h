//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#ifndef FUNCOESJOGO_H_INCLUDED
#define FUNCOESJOGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include "funcoesRanking.h"

char menu(); //exibe o menu e retorna a entrada da opção escolhida pelo usuário

void leNomeJogadores(char *jogador1, char *jogador2, char numJogadores);

void iniciaPartidaVazia(char **partida);

void exibeJogo(char **partida);

void leComando(char *comando, char *acao, char *jogador);

int marcar(char **partida, int jogadorDaVez, char *comando); //retorna 1 caso a marcação dê certo, e 0 caso falhe nas validações

void salvar(FILE *arquivo, char **partida, char numJogadores, int jogadorDaVez, char *jogador1, char *jogador2, char *nomeArquivo);

//retorna 1 se a partida acabou, 0 caso contrário, e 2 caso o jogador tenha apenas salvado ela ou digitado um comando errado
int proximoComando(char **partida, char *jogador, int jogadorDaVez, char *outroJogador, int *partidaEmAndamento, char numJogadores, Jogadores **rankeamento, int *size);

void executarPartida(char numJogadores, char **partida, char *jogador1, char *jogador2, int jogadorDaVez, int *partidaEmAndamento, Jogadores **rankeamento, int *size);

//pega o jogo salvo no arquivo, e continua a partida
void getJogoSalvo(FILE *arquivo, char **partida, char *jogador1,  char *jogador2, int *partidaEmAndamento, char *numJogadores, Jogadores **rankeamento, int *size);

int verificaQualJogadorDaVez(char **partida); //usado para continuar uma partida atual (case '3')

int jogadaDoComputador(char **partida);

#endif