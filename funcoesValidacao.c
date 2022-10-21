//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesValidacao.h"
#include "formatacao.h"

int charToInt(char c){
    int inteiro;
    inteiro = c - '0';

    return inteiro;
}

int verificaDigito(char posicao){
    //1 a 3 na tabela ASCII: 49 a 51
    if(posicao < 49 || posicao > 51){
        return 0;
    }
    return 1;
}

int verificaPosicao(int linha, int coluna, char **partida){
    if(partida[linha][coluna] == ' '){ //sinal que a posição está disponível
        return 1;
    }
    return 0;
}

int verificaPartida(char **partida, char *jogador, int jogadorDaVez){
    //retorno 0: ainda não acabou, 1: vitória do jogador da vez, 2: empate
    char verificador = jogadorDaVez == 1 ? 'X' : 'O';

    if(partida[0][0] == verificador){
        if(partida[0][1] == verificador && partida[0][2] == verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
        if(partida[1][0] == verificador && partida[2][0] == verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
        if(partida[1][1] == verificador && partida[2][2] == verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
    }
    if(partida[0][1] == verificador){
        if(partida[1][1]==verificador && partida[2][1]==verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
    }
    if (partida[1][0] == verificador){
        if (partida[1][1]==verificador && partida[1][2]==verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
    }
    if (partida[2][2] == verificador){
        if(partida[2][0]==verificador && partida[2][1]==verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
        if(partida[0][2]==verificador && partida[1][2]==verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
    }
    if(partida[0][2] == verificador){
        if(partida[1][1]==verificador && partida[2][0]==verificador){
            if(strcmp(jogador, "Computador"))
                printf(GREEN(BOLD("Parabéns %s, você ganhou!\n")), jogador);
            else
                printf(RED(BOLD("Vitória do computador!\n")));
            return 1;
        }
    }
    
    //caso de empate:
    int contParaEmpate = 0;
    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if(partida[i][j]!= ' ')
                contParaEmpate++;
        }        
    }
    //se contParaEmpate = 9, o tabuleiro está todo preenchido, e não entrou em nenhum dos casos de vitoria acima, o jogo terminará empatado
    if(contParaEmpate==9){ 
        printf(YELLOW(BOLD("Velha! O jogo terminou empatado!\n")));
        return 2;
    }

    return 0; //caso não entre em nenhuma verificacao acima, a partida continuará em andamento
}

int validaNomeArquivo(char *nomeArquivo){
    int lengthNomeArq = strlen(nomeArquivo);

    //se na posição 0 for um ., é pq a pessoa nao digitou um nome pro arquivo vaĺido
    //se nas ultimas 4 posições nao for ".txt" a pessoa digitou uma extensao invalida
    if(nomeArquivo[0] == '.' || nomeArquivo[lengthNomeArq-1] != 't' || nomeArquivo[lengthNomeArq-2] != 'x' || nomeArquivo[lengthNomeArq-3] != 't' || nomeArquivo[lengthNomeArq-4] != '.'){
        printf(RED(BOLD("Nome inválido para arquivo ou extensão inválida (precisa ser .txt)!\n")));

        return 0;
    }

    return 1;
}

int validaNumJogadores(char *numJogadores){
    //retorna 0 caso seja um numero de jogadores invalido
    if(numJogadores[1] != '\n' || (numJogadores[0] != '1' && numJogadores[0] != '2')){
        if(numJogadores[1] != '\n')
            limpaBuffer(); //limpando o buffer dos caracteres lidos além 3°
        
        printf(RED(BOLD("\nNúmero de jogadores inválido!\n")));
        return 0;
    }

    return 1;
}

void limpaBuffer(){
    while(getchar() != '\n');
}

char **alocaMatriz(int n, int m){
    char **matriz;

    matriz = malloc(n * sizeof(char*));
    for (int i = 0; i < n; i++)
    {
        matriz[i] = malloc(m * sizeof(char));
    }

    return matriz;    
}

void liberaMatriz(char **matriz, int n){
    for (int i = 0; i < n; i++)
    {
        free(matriz[i]);
    }
    free(matriz);    
}