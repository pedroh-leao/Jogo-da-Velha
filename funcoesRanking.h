//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#ifndef FUNCOESRANKING_H_INCLUDED
#define FUNCOESRANKING_H_INCLUDED

typedef struct{ //struct usada para o rankeamento
    char nome[100];
    int vitorias;
    int empates;
    int derrotas;
}Jogadores;

void leArquivoRanking(Jogadores **rankeamento, int *numJogadores);

void atualizaArquivoRanking(Jogadores *rankeamento, int size);

void atualizaVED(Jogadores **rankeamento, int *size, char *jogador, int resultado);

//verifica se o jogador já está no ranking, caso exista, retorna o indice dele, caso contrário, retorna -1
int existeNoRanking(Jogadores *rankeamento, int size, char *jogador); 

void ordenaRanking(Jogadores *rankeamento, int size);

void realocaVetor(Jogadores **rankeamento, int size); //realoca vetor usando apenas malloc

void exibePosicaoJogadores(Jogadores *rankeamento, char *jogador1, char *jogador2);

void exibeRanking(Jogadores *rankeamento, int size);

void inserirEspaço(int n);

#endif