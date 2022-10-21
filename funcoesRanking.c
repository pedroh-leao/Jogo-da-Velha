//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesRanking.h"
#include "formatacao.h"

void leArquivoRanking(Jogadores **rankeamento, int *numJogadores){
    FILE *arqRanking;
    arqRanking = fopen("velha.ini", "r");

    int emptyFile = 0, nullFile = 1;

    //verifica se o arquivo não tem nada escrito
    if(arqRanking != NULL){
        nullFile = 0;
        fseek(arqRanking, 0, SEEK_END);

        if(ftell(arqRanking) == 0)
            emptyFile = 1;
    }
    fseek(arqRanking, 0, SEEK_SET);

    if(nullFile || emptyFile){
        *rankeamento = (Jogadores *) malloc(sizeof(Jogadores));
        strcpy((*rankeamento)[0].nome, "Computador"); //o computador sempre estará no ranking
        (*rankeamento)[0].vitorias = 0;
        (*rankeamento)[0].empates = 0;
        (*rankeamento)[0].derrotas = 0;
        *numJogadores = 1;
    }
    else{
        int computadorPresente = 0;
        fscanf(arqRanking, "%d\n", numJogadores);
        *rankeamento = (Jogadores *) malloc(*numJogadores * sizeof(Jogadores));

        for (int i = 0; i < *numJogadores; i++)
        {
            fgets((*rankeamento)[i].nome, 100, arqRanking);
            if(!strcmp((*rankeamento)[i].nome, "Computador"))
                computadorPresente = 1;
            (*rankeamento)[i].nome[strlen((*rankeamento)[i].nome) - 1] = '\0';
            fscanf(arqRanking, "%d %d %d\n", &(*rankeamento)[i].vitorias, &(*rankeamento)[i].empates, &(*rankeamento)[i].derrotas);
        }      
        
        if(!computadorPresente){ //se o computador não estiver no vetor
            *numJogadores = *numJogadores + 1;
            realocaVetor(rankeamento, *numJogadores);
            strcpy((*rankeamento)[*numJogadores - 1].nome, "Computador");
            (*rankeamento)[*numJogadores - 1].derrotas = 0;
            (*rankeamento)[*numJogadores - 1].empates = 0;
            (*rankeamento)[*numJogadores - 1].vitorias = 0;
        }

        fclose(arqRanking);
    }    
}

void atualizaArquivoRanking(Jogadores *rankeamento, int size){
    FILE *arqRanking;
    int sizeAux;
    int computadorTop10 = 0; //variavel para fazer o controle se o Computador está no top 10
    
    arqRanking = fopen("velha.ini", "w");

    sizeAux = size <= 10 ? size : 10;
    
    fprintf(arqRanking, "%d\n", sizeAux);
    for (int i = 0; i < sizeAux-1; i++){
        fprintf(arqRanking, "%s\n", rankeamento[i].nome);
        fprintf(arqRanking, "%d %d %d\n", rankeamento[i].vitorias, rankeamento[i].empates, rankeamento[i].derrotas);
        if(!strcmp(rankeamento[i].nome, "Computador"))
            computadorTop10 = 1;
    }
    if(computadorTop10){ //significa que o computador já está entre até os 9 primeiros do ranking
        fprintf(arqRanking, "%s\n", rankeamento[sizeAux-1].nome);
        fprintf(arqRanking, "%d %d %d\n", rankeamento[sizeAux-1].vitorias, rankeamento[sizeAux-1].empates, rankeamento[sizeAux-1].derrotas);
    }
    else{
        int indicePC = existeNoRanking(rankeamento, size, "Computador"); //função que retorna o indice do vetor de acordo com o nome dado como parametro
        fprintf(arqRanking, "%s\n", rankeamento[indicePC].nome);
        fprintf(arqRanking, "%d %d %d\n", rankeamento[indicePC].vitorias, rankeamento[indicePC].empates, rankeamento[indicePC].derrotas);
    }

    fclose(arqRanking);
}

void atualizaVED(Jogadores **rankeamento, int *size, char *jogador, int resultado){
    /* resultado = -1 : derrota
    resultado = 0 : empate
    resultado = 1 : vitória */

    int indiceJogador = existeNoRanking(*rankeamento, *size, jogador);

    if(indiceJogador == -1){ //caso o jogador ainda não exista no ranking
        //realocar vetor, iniciar v, e e d do jogador como 0
        *size = *size + 1;
        realocaVetor(rankeamento, *size);
        strcpy((*rankeamento)[*size - 1].nome, jogador);
        (*rankeamento)[*size - 1].derrotas = 0;
        (*rankeamento)[*size - 1].empates = 0;
        (*rankeamento)[*size - 1].vitorias = 0;

        indiceJogador = *size - 1;
    }

    if(resultado == -1)
        (*rankeamento)[indiceJogador].derrotas++;
    else if(resultado == 0)
        (*rankeamento)[indiceJogador].empates++;
    else
        (*rankeamento)[indiceJogador].vitorias++;
}

int existeNoRanking(Jogadores *rankeamento, int size, char *jogador){ 
    //verifica se o jogador já está no ranking, caso exista, retorna o indice dele, caso contrário, retorna -1
    for (int i = 0; i < size; i++){
        if(!strcmp(jogador, rankeamento[i].nome))
            return i;
    }
    return -1;    
}

void ordenaRanking(Jogadores *rankeamento, int size){
    Jogadores aux;
    int trocaPosicao = 0;

    //ordenando de acordo com as vitórias, depois empates e depois derrotas
    for (int i = size - 1; i > 0; i--){
        for (int j = 0; j < i; j++){
            if(rankeamento[j].vitorias < rankeamento[j+1].vitorias)
                trocaPosicao = 1;
            else if(rankeamento[j].vitorias == rankeamento[j+1].vitorias && rankeamento[j].empates < rankeamento[j+1].empates)
                trocaPosicao = 1;
            else if(rankeamento[j].vitorias == rankeamento[j+1].vitorias && rankeamento[j].empates == rankeamento[j+1].empates)
                if(rankeamento[j].derrotas > rankeamento[j+1].derrotas)
                    trocaPosicao = 1;

            if(trocaPosicao){
                aux = rankeamento[j];
                rankeamento[j] = rankeamento[j+1];
                rankeamento[j+1] = aux;
                trocaPosicao = 0;
            }            
        }
    }   
}

void realocaVetor(Jogadores **rankeamento, int size){
    //função para realocar o vetor sem usar realloc
    Jogadores *vetorAux;
    vetorAux = (Jogadores *) malloc((size-1) * sizeof(Jogadores));

    for (int i = 0; i < size-1; i++){
        vetorAux[i] = (*rankeamento)[i];
    }    

    free(*rankeamento);
    *rankeamento = (Jogadores *) malloc(size * sizeof(Jogadores));
    for (int i = 0; i < size-1; i++){
        (*rankeamento)[i] = vetorAux[i];
    }
    
    free(vetorAux);
}

void exibePosicaoJogadores(Jogadores *rankeamento, char *jogador1, char *jogador2){
    int jogadores = 2, i = 0;
    int maiorNome = strlen(jogador1);
    int outroNome = strlen(jogador2);

    if(outroNome > maiorNome){
        int aux = outroNome;
        outroNome = maiorNome;
        maiorNome = aux;
    }
    printf("\n");
    inserirEspaço(7);
    printf(GREEN(BOLD("Posição dos jogadores no ranking\n\n")));

    printf("    "TAB_VER);
    inserirEspaço((maiorNome-4)/2);
    printf(GREEN(BOLD("Nome")));
    inserirEspaço(maiorNome%2==0 ? (maiorNome-4)/2 : ((maiorNome-4)/2)+1);
    printf(TAB_VER" "GREEN(BOLD("Vitórias"))" "TAB_VER" "GREEN(BOLD("Empates"))" "TAB_VER" "GREEN(BOLD("Derrotas\n")));
    printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_MJ);
    for (int i = 0; i < maiorNome; i++)
        printf(TAB_HOR);    
    printf(TAB_MJ);
    for (int i = 0; i < 10; i++)
        printf(TAB_HOR);
    for (int i = 0; i < 2; i++){
        printf(TAB_MJ);
        for (int j = 0; j < 9; j++){
            printf(TAB_HOR);
        }
    }    
    printf("\n");

    while(jogadores>0){
        if (!strcmp(rankeamento[i].nome, jogador1) || !strcmp(rankeamento[i].nome, jogador2)){
            printf(GREEN(BOLD("%2d° "))TAB_VER, i+1);
            outroNome = strlen(rankeamento[i].nome);
            inserirEspaço((maiorNome-outroNome)%2==0 ? (maiorNome-outroNome)/2 : ((maiorNome-outroNome)/2)+1);
            printf("%s", rankeamento[i].nome);
            inserirEspaço((maiorNome-outroNome)/2);
            printf(TAB_VER"%6d    "TAB_VER"%5d    "TAB_VER"%5d\n", rankeamento[i].vitorias, rankeamento[i].empates, rankeamento[i].derrotas);

            jogadores--;
        }      
        i++;
    }
    printf("\n");
}

void exibeRanking(Jogadores *rankeamento, int size){
    printf("\n");
    inserirEspaço(15);
    printf(GREEN(BOLD("..:: Ranking ::..\n\n")));
    
    int caracteresMaiorNome=0, tamNome;
    for (int i = 0; i < size; i++){
        tamNome = strlen(rankeamento[i].nome);
        if(tamNome > caracteresMaiorNome)
            caracteresMaiorNome = tamNome;
    }
    
    printf("    "TAB_VER);
    inserirEspaço((caracteresMaiorNome-4)/2);
    printf(GREEN(BOLD("Nome")));
    inserirEspaço(caracteresMaiorNome%2==0 ? (caracteresMaiorNome-4)/2 : ((caracteresMaiorNome-4)/2)+1);
    printf(TAB_VER" "GREEN(BOLD("Vitórias"))" "TAB_VER" "GREEN(BOLD("Empates"))" "TAB_VER" "GREEN(BOLD("Derrotas\n")));
    printf(TAB_HOR TAB_HOR TAB_HOR TAB_HOR TAB_MJ);
    for (int i = 0; i < caracteresMaiorNome; i++)
        printf(TAB_HOR);    
    printf(TAB_MJ);
    for (int i = 0; i < 10; i++)
        printf(TAB_HOR);
    for (int i = 0; i < 2; i++){
        printf(TAB_MJ);
        for (int j = 0; j < 9; j++){
            printf(TAB_HOR);
        }
    }    
    printf("\n");
        
    for (int i = 0; i < size; i++){
        printf(GREEN(BOLD("%2d° "))TAB_VER, i+1);
        tamNome = strlen(rankeamento[i].nome);
        inserirEspaço((caracteresMaiorNome-tamNome)%2==0 ? (caracteresMaiorNome-tamNome)/2 : ((caracteresMaiorNome-tamNome)/2)+1);
        printf("%s", rankeamento[i].nome);
        inserirEspaço((caracteresMaiorNome-tamNome)/2);
        printf(TAB_VER"%6d    "TAB_VER"%5d    "TAB_VER"%5d\n", rankeamento[i].vitorias, rankeamento[i].empates, rankeamento[i].derrotas);
    }    
    printf("\n");
}

void inserirEspaço(int n){
    for (int i = 0; i < n; i++){
        printf(" ");
    }    
}