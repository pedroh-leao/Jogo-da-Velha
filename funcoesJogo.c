//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcoesJogo.h"
#include "funcoesValidacao.h"
#include "funcoesRanking.h"
#include "formatacao.h"

char menu(){
    printf(CYAN(BOLD("0. Sair do Jogo\n")));
    printf(CYAN(BOLD("1. Começar um novo jogo\n")));
    printf(CYAN(BOLD("2. Continuar um jogo salvo\n")));
    printf(CYAN(BOLD("3. Continuar o jogo atual\n")));
    printf(CYAN(BOLD("4. Exibir o ranking\n")));
    printf(WHITE(BOLD("Durante o jogo digite “voltar” para retornar ao menu.\n")));

    char op[3];
    do{
        printf(WHITE(BOLD("\nEscolha a opção: ")));
        //o op pode ter apenas 1 caracter, já que o menu é de 1 a 4, mas o limite do fgets é 3 apenas para fazer a validação
        //se o op[1] != '\n', sinal que o usuario digitou mais de um caracter
        fgets(op, 3, stdin); 
        if(op[1] != '\n'){
            limpaBuffer(); //limpando o buffer dos caracteres lidos além 3°
            printf(RED(BOLD("Opção inválida!")));
        }
        else if(op[0] < '0' || op[0] > '4')
            printf(RED(BOLD("Opção inválida!")));
    }while(op[1] != '\n' || op[0]<'0' || op[0]>'4');
    
    return op[0];
}

void leNomeJogadores(char *jogador1, char *jogador2, char numJogadores){
    int invalido, tam1, tam2;
    do{
        printf(WHITE(BOLD("Digite o nome do jogador 1: ")));
        fgets(jogador1, 100, stdin);
        jogador1[strlen(jogador1)-1] = '\0';
        tam1 = strlen(jogador1);

        while (jogador1[0] == ' '){
            for(int i = 0; i < tam1; i++){
                jogador1[i] = jogador1[i+1];
            }            
            tam1 = strlen(jogador1);
        }   

        if(numJogadores == '2'){
            printf(WHITE(BOLD("Digite o nome do jogador 2: ")));
            fgets(jogador2, 100, stdin);
            jogador2[strlen(jogador2)-1] = '\0';
            tam2 = strlen(jogador2);

            while (jogador2[0] == ' '){
                for(int i = 0; i < tam2; i++){
                    jogador2[i] = jogador2[i+1];
                }            
                tam2 = strlen(jogador2);
            }
        }
        else
            strcpy(jogador2, "Computador");

        invalido = 0;

        if(!strcmp(jogador1, "") || !strcmp(jogador2, "")){
            printf(RED(BOLD("Os nomes não podem ser nulos!\n\n")));
            invalido = 1;
        }
        else if(!strcmp(jogador1, "Computador") || (!strcmp(jogador2, "Computador") && numJogadores=='2')){
            printf(RED(BOLD("Os nomes não podem receber \"Computador\"!\n\n")));
            invalido = 1;
        }
        else if(!strcmp(jogador1, jogador2)){
            printf(RED(BOLD("Os nomes não podem ser iguais!\n\n")));
            invalido = 1;
        }
    }while(invalido);
}

void iniciaPartidaVazia(char **partida){
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            partida[i][j] = ' ';
        }        
    }    
}

void exibeJogo(char **partida){
    printf(TAB_TL TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TJ TAB_HOR TAB_HOR TAB_HOR TAB_TR"\n");
    printf(TAB_VER"   "TAB_VER" 1 "TAB_VER" 2 "TAB_VER" 3 "TAB_VER"\n");
    printf(TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR"\n");
    printf(TAB_VER" 1 "TAB_VER BOLD(" %c ") TAB_VER BOLD(" %c ") TAB_VER BOLD(" %c ") TAB_VER"\n", partida[0][0], partida[0][1], partida[0][2]);
    printf(TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR"\n");
    printf(TAB_VER" 2 "TAB_VER BOLD(" %c ") TAB_VER BOLD(" %c ") TAB_VER BOLD(" %c ") TAB_VER"\n", partida[1][0], partida[1][1], partida[1][2]);
    printf(TAB_ML TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MJ TAB_HOR TAB_HOR TAB_HOR TAB_MR"\n");
    printf(TAB_VER" 3 "TAB_VER BOLD(" %c ") TAB_VER BOLD(" %c ") TAB_VER BOLD(" %c ") TAB_VER"\n", partida[2][0], partida[2][1], partida[2][2]);    
    printf(TAB_BL TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BJ TAB_HOR TAB_HOR TAB_HOR TAB_BR"\n");
}

void leComando(char *comando, char *acao, char *jogador){
    int invalido;
    do{
        printf(WHITE(BOLD("\n%s, digite o comando: ")), jogador);
        fgets(comando, 100, stdin);

        if(strlen(comando) == 100 && comando[99] != '\n') //caso o usuario tenha digitado um comando maior do que o tamanho maximo da variavel
            limpaBuffer();

        comando[strlen(comando)-1] = '\0';
        printf("\n");
        
        for (int i = 0; i < 6; i++){
            acao[i] = comando[i];
        }
        acao[6] = '\0';

        if(strcmp(acao, "marcar") && strcmp(acao, "salvar") && strcmp(acao, "voltar"))
            invalido = 1;
        else if((!strcmp(acao, "marcar") || !strcmp(acao, "salvar")) && comando[6] != ' ')
            invalido = 1;
        else if(!strcmp(acao, "marcar") && comando[9] != '\0')
            invalido = 1;
        else if(!strcmp(acao, "voltar") && comando[6] != '\0')
            invalido = 1;
        else
            invalido = 0;

        if(invalido)
            printf(RED(BOLD("Comando inválido!\n")));
    } while (invalido); //enquanto o comando estiver invalido
}

int marcar(char **partida, int jogadorDaVez, char *comando){
    //retorna 1 caso a marcação dê certo, e 0 caso falhe nas validações

    char posicaoX, posicaoY;
    posicaoY = comando[7]; //numero da linha
    posicaoX = comando[8]; //numero da coluna
    int linha, coluna;

    //conversão da posição de char para um inteiro        
    linha = charToInt(posicaoY) -1;
    coluna = charToInt(posicaoX) -1;

    //verifica se o usuario realmente digitou números de 1 a 3 para posição e se essa posicao está livre ou já foi marcada
    if(!verificaDigito(posicaoX) || !verificaDigito(posicaoY) || !verificaPosicao(linha, coluna, partida)){
        printf(RED(BOLD("Posições inválidas ou já marcadas! Digite apenas números de 1 até 3.\n")));
            
        return 0;
    }
        
    //se jogadorDaVez = 1, marcar com X, se jogadorDaVez = 2, marcar com O
    if(jogadorDaVez == 1)
        partida[linha][coluna] = 'X';
    else
        partida[linha][coluna] = 'O';

    return 1;
}

void salvar(FILE *arquivo, char **partida, char numJogadores, int jogadorDaVez, char *jogador1, char *jogador2, char *nomeArquivo){
    fprintf(arquivo, "%c\n", numJogadores);

    fprintf(arquivo, "%s\n", jogador1);

    if(numJogadores == '2')
        fprintf(arquivo, "%s\n", jogador2);

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if (partida[i][j] != ' ')
                fprintf(arquivo, "%c ", partida[i][j]);
            else
                fprintf(arquivo, "- ");
        }
        fprintf(arquivo, "\n");
    }

    fprintf(arquivo, "%d", jogadorDaVez);

    fclose(arquivo);

    printf(GREEN(BOLD("Arquivo “%s” salvo com sucesso!\n")), nomeArquivo);
}

int proximoComando(char **partida, char *jogador, int jogadorDaVez, char *outroJogador, int *partidaEmAndamento, char numJogadores, Jogadores **rankeamento, int *size){ 
    /*retorno: 
    1: se a partida acabou, ou o usuario digitou para voltar
    0: caso a partida continue normalmente,
    2: caso o jogador tenha apenas salvado a partida ou digitado um comando errado, nesse caso ainda será a vez do mesmo jogador */
    
    if(numJogadores == '2' || jogadorDaVez == 1){
        char comando[100];
        char acao[7]; //marcar, salvar ou voltar

        leComando(comando, acao, jogador);
        
        if(!strcmp(acao, "marcar")){
            
            if(!marcar(partida, jogadorDaVez, comando))
                return 2; //falhou nas validações na hora de marcar (posição já preenchida ou posição inválida)

            exibeJogo(partida);
            
            //se verificaPartida() retornar 0, a partida ainda não acabou, caso tenha acabado, retorna 1 e exibe msg de parabens, ou 2 e msg de empate
            int fimDaPartida = verificaPartida(partida, jogador, jogadorDaVez);
            if(!fimDaPartida) 
                return 0;
            else{ //fim da partida
                *partidaEmAndamento = 0;

                if(fimDaPartida == 1){
                    atualizaVED(rankeamento, size, jogador, 1);
                    atualizaVED(rankeamento, size, outroJogador, -1);
                }
                else{
                    atualizaVED(rankeamento, size, jogador, 0);
                    atualizaVED(rankeamento, size, outroJogador, 0);
                }
                ordenaRanking(*rankeamento, *size);

                exibePosicaoJogadores(*rankeamento, jogador, outroJogador);

                printf(WHITE(BOLD("Digite qualquer tecla para continuar!\n")));
                limpaBuffer();
                
                return 1;
            }
        }
        else if (!strcmp(acao, "salvar")){
            FILE *arquivo;
            char nomeArquivo[94]; //100 - 6  =  comando - os caracteres da acao

            int cont = 7;
            while(comando[cont] != '\0' && cont<100){
                nomeArquivo[cont-7] = comando[cont];
                cont++;
            }
            nomeArquivo[cont-7] = '\0';
            cont = 0;

            //se na posição 0 for um ., é pq a pessoa nao digitou um nome pro arquivo vaĺido
            //se nas ultimas 4 posições nao for ".txt" a pessoa digitou uma extensao invalida
            if(!validaNomeArquivo(nomeArquivo))
                return 2;

            arquivo = fopen(nomeArquivo, "w");

            if(arquivo != NULL){
                if(numJogadores == '2'){
                    //if para deixar ordenado no arquivo do jogo o nome do jogador 1 primeiro, e do jogador 2 embaixo
                    if(jogadorDaVez == 1)
                        salvar(arquivo, partida, '2', jogadorDaVez, jogador, outroJogador, nomeArquivo);
                    else
                        salvar(arquivo, partida, '2', jogadorDaVez, outroJogador, jogador, nomeArquivo);
                }
                else
                    salvar(arquivo, partida, '1', jogadorDaVez, jogador, "", nomeArquivo);
            }        

            return 2;
        }
        else{ //voltar
            return 1;
        }
    }
    else{ //vez do computador na partida de 1 pessoa
        //executar função de marcar do computador, exibir partida, depois verificar se a partida acabou
        jogadaDoComputador(partida);
        printf(WHITE(BOLD("Vez do computador: \n")));
        exibeJogo(partida);
            
        //se verificaPartida() retornar 0, a partida ainda não acabou, caso tenha acabado, retorna 1 e exibe uma msg de parabens ou de empate
        if(!verificaPartida(partida, "Computador", jogadorDaVez)) 
            return 0;
        else{ //fim da partida
            *partidaEmAndamento = 0;

            atualizaVED(rankeamento, size, jogador, 1);
            atualizaVED(rankeamento, size, outroJogador, -1);
            ordenaRanking(*rankeamento, *size);
            exibePosicaoJogadores(*rankeamento, outroJogador, jogador);

            printf(WHITE(BOLD("Digite qualquer tecla para continuar!\n")));
            limpaBuffer();

            return 1;
        }
    }    
}

void executarPartida(char numJogadores, char **partida, char *jogador1, char *jogador2, int jogadorDaVez, int *partidaEmAndamento, Jogadores **rankeamento, int *size){
    *partidaEmAndamento = 1;
    exibeJogo(partida);
    int partidaFinalizada = 2; //nao é inicializado com 0, pois se não ja pularia a vez do jogador atual ao entrar a primeira vez no while
  
        //jogadorDaVez:   ímpar: vez do jogador 1   par: vez do jogador 2        
    while(!partidaFinalizada || partidaFinalizada == 2){ //se o proximoComando retornar 2 o jogador apenas salvou a partida ou digitou algo errado
        if(!partidaFinalizada) //caso o partidaFinalizada tenha recebido 0, a rodada aconteceu normalmente e agr é a vez do outro jogador
            jogadorDaVez++;

        if(jogadorDaVez % 2 == 0)
            partidaFinalizada = proximoComando(partida, jogador2, 2, jogador1, partidaEmAndamento, numJogadores, rankeamento, size);
        else
            partidaFinalizada = proximoComando(partida, jogador1, 1, jogador2, partidaEmAndamento, numJogadores, rankeamento, size);
    }
}

void getJogoSalvo(FILE *arquivo, char **partida, char *jogador1,  char *jogador2, int *partidaEmAndamento, char *numJogadores, Jogadores **rankeamento, int *size){
    int jogadorDaVez;

    fscanf(arquivo, "%c\n", numJogadores);

    fgets(jogador1, 100, arquivo);
    jogador1[strlen(jogador1)-1] = '\0';

    if(*numJogadores == '2'){
        fgets(jogador2, 100, arquivo);
        jogador2[strlen(jogador2)-1] = '\0';
    }

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            fscanf(arquivo, "%c ", &partida[i][j]);
            if(partida[i][j] == '-')
                partida[i][j] = ' ';
        }
        //fscanf(arquivo, "\n");        
    }

    fscanf(arquivo, "%d", &jogadorDaVez);
    
    fclose(arquivo);

    executarPartida(*numJogadores, partida, jogador1, *numJogadores=='2' ? jogador2 : "Computador", jogadorDaVez, partidaEmAndamento, rankeamento, size);
}

int verificaQualJogadorDaVez(char **partida){
    int vezDoJogador, contX = 0, contO = 0;

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            if(partida[i][j] == 'X')
                contX++;
            else if(partida[i][j] == 'O')
                contO++;
        }        
    }

    //quando o jogador 1 e o jogador 2 já jogou o mesmo número de vezes, portanto é a vez do jogador 1
    if(contX == contO)
        vezDoJogador = 1;
    else //nesse caso o jogador 1 jogou mais que o 2, então é a vez do 2° jogador
        vezDoJogador = 2;
    
    return vezDoJogador;
}

int jogadaDoComputador(char **partida){
    int posicoesPreenchidas = 0;
    for(int i = 0; i<3; i++){
        for(int j=0; j<3; j++){
            if(partida[i][j] != ' ')
                posicoesPreenchidas++;
        }
    }

    if(posicoesPreenchidas == 1){//só aconteceu a primeira jogada
        if(partida[0][0] == 'X' || partida[0][2] == 'X' || partida[2][0] == 'X' || partida[2][2] == 'X'){
            partida[1][1] = 'O';
            return 1;
        }
        else if(partida[1][1] == 'X'){
            partida[0][0] = 'O';
            return 1;
        }
        else if(partida[0][1] == 'X' || partida[1][0] == 'X' || partida[1][2] == 'X' || partida[2][1] == 'X'){
            partida[1][1] = 'O';
            return 1;
        }
    }
    if(posicoesPreenchidas >= 3){ //na 4 jogada é preciso verificar possibilidade de vitoria, pois na 5 o jogador já tem possibilidade de ganhar
        //verifica se tem alguma forma do computador ganhar:
        //se tiver duas marcações de O na linha 0
        if(partida[0][0] == 'O' && partida[0][1] == 'O'){
            if(verificaPosicao(0, 2, partida)){
                partida[0][2] = 'O';
                return 1;
            }
        }
        if(partida[0][1] == 'O' && partida[0][2] == 'O'){
            if(verificaPosicao(0, 0, partida)){
                partida[0][0] = 'O';
                return 1;
            }
        }
        if(partida[0][0] == 'O' && partida[0][2] == 'O'){
            if(verificaPosicao(0, 1, partida)){
                partida[0][1] = 'O';
                return 1;
            }
        }
        //se tiver duas marcações de O na coluna 0
        if(partida[0][0] == 'O' && partida[1][0] == 'O'){
            if(verificaPosicao(2, 0, partida)){
                partida[2][0] = 'O';
                return 1;
            }
        }
        if(partida[1][0] == 'O' && partida[2][0] == 'O'){
            if(verificaPosicao(0, 0, partida)){
                partida[0][0] = 'O';
                return 1;
            }
        }
        if(partida[0][0] == 'O' && partida[2][0] == 'O'){
            if(verificaPosicao(1, 0, partida)){
                partida[1][0] = 'O';
                return 1;
            }
        }

        //se tiver duas marcações de O na linha 1
        if(partida[1][0] == 'O' && partida[1][1] == 'O'){
            if(verificaPosicao(1, 2, partida)){
                partida[1][2] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'O' && partida[1][2] == 'O'){
            if(verificaPosicao(1, 0, partida)){
                partida[1][0] = 'O';
                return 1;
            }
        }
        if(partida[1][0] == 'O' && partida[1][2] == 'O'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }
        //se tiver duas marcações de O na coluna 1
        if(partida[0][1] == 'O' && partida[1][1] == 'O'){
            if(verificaPosicao(2, 1, partida)){
                partida[2][1] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'O' && partida[2][1] == 'O'){
            if(verificaPosicao(0, 1, partida)){
                partida[0][1] = 'O';
                return 1;
            }
        }
        if(partida[0][1] == 'O' && partida[2][1] == 'O'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }

        //se tiver duas marcações de O na linha 2
        if(partida[2][0] == 'O' && partida[2][1] == 'O'){
            if(verificaPosicao(2, 2, partida)){
                partida[2][2] = 'O';
                return 1;
            }
        }
        if(partida[2][1] == 'O' && partida[2][2] == 'O'){
            if(verificaPosicao(2, 0, partida)){
                partida[2][0] = 'O';
                return 1;
            }
        }
        if(partida[2][0] == 'O' && partida[2][2] == 'O'){
            if(verificaPosicao(2, 1, partida)){
                partida[2][1] = 'O';
                return 1;
            }
        }
        //se tiver duas marcações de O na coluna 2
        if(partida[0][2] == 'O' && partida[1][2] == 'O'){
            if(verificaPosicao(2, 2, partida)){
                partida[2][2] = 'O';
                return 1;
            }
        }
        if(partida[1][2] == 'O' && partida[2][2] == 'O'){
            if(verificaPosicao(0, 2, partida)){
                partida[0][2] = 'O';
                return 1;
            }
        }
        if(partida[0][2] == 'O' && partida[2][2] == 'O'){
            if(verificaPosicao(1, 2, partida)){
                partida[1][2] = 'O';
                return 1;
            }
        }

        //caso tenha 2 marcações de O na diagonal principal:
        if(partida[0][0] == 'O' && partida[1][1] == 'O'){
            if(verificaPosicao(2, 2, partida)){
                partida[2][2] = 'O';
                return 1;
            }
        }
        if(partida[0][0] == 'O' && partida[2][2] == 'O'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'O' && partida[2][2] == 'O'){
            if(verificaPosicao(0, 0, partida)){
                partida[0][0] = 'O';
                return 1;
            }
        }

        //caso tenha 2 marcações de O na diagonal secundaria:
        if(partida[0][2] == 'O' && partida[1][1] == 'O'){
            if(verificaPosicao(2, 0, partida)){
                partida[2][0] = 'O';
                return 1;
            }
        }
        if(partida[0][2] == 'O' && partida[2][0] == 'O'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'O' && partida[2][0] == 'O'){
            if(verificaPosicao(0, 2, partida)){
                partida[0][2] = 'O';
                return 1;
            }
        }

        //verifica se tem alguma forma do jogador ganhar na proxima jogada:
        //se tiver duas marcações de X na linha 0
        if(partida[0][0] == 'X' && partida[0][1] == 'X'){
            if(verificaPosicao(0, 2, partida)){
                partida[0][2] = 'O';
                return 1;
            }
        }
        if(partida[0][1] == 'X' && partida[0][2] == 'X'){
            if(verificaPosicao(0, 0, partida)){
                partida[0][0] = 'O';
                return 1;
            }
        }
        if(partida[0][0] == 'X' && partida[0][2] == 'X'){
            if(verificaPosicao(0, 1, partida)){
                partida[0][1] = 'O';
                return 1;
            }
        }
        //se tiver duas marcações de X na coluna 0
        if(partida[0][0] == 'X' && partida[1][0] == 'X'){
            if(verificaPosicao(2, 0, partida)){
                partida[2][0] = 'O';
                return 1;
            }
        }
        if(partida[1][0] == 'X' && partida[2][0] == 'X'){
            if(verificaPosicao(0, 0, partida)){
                partida[0][0] = 'O';
                return 1;
            }
        }
        if(partida[0][0] == 'X' && partida[2][0] == 'X'){
            if(verificaPosicao(1, 0, partida)){
                partida[1][0] = 'O';
                return 1;
            }
        }

        //se tiver duas marcações de X na linha 1
        if(partida[1][0] == 'X' && partida[1][1] == 'X'){
            if(verificaPosicao(1, 2, partida)){
                partida[1][2] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'X' && partida[1][2] == 'X'){
            if(verificaPosicao(1, 0, partida)){
                partida[1][0] = 'O';
                return 1;
            }
        }
        if(partida[1][0] == 'X' && partida[1][2] == 'X'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }
        //se tiver duas marcações de X na coluna 1
        if(partida[0][1] == 'X' && partida[1][1] == 'X'){
            if(verificaPosicao(2, 1, partida)){
                partida[2][1] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'X' && partida[2][1] == 'X'){
            if(verificaPosicao(0, 1, partida)){
                partida[0][1] = 'O';
                return 1;
            }
        }
        if(partida[0][1] == 'X' && partida[2][1] == 'X'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }

        //se tiver duas marcações de X na linha 2
        if(partida[2][0] == 'X' && partida[2][1] == 'X'){
            if(verificaPosicao(2, 2, partida)){
                partida[2][2] = 'O';
                return 1;
            }
        }
        if(partida[2][1] == 'X' && partida[2][2] == 'X'){
            if(verificaPosicao(2, 0, partida)){
                partida[2][0] = 'O';
                return 1;
            }
        }
        if(partida[2][0] == 'X' && partida[2][2] == 'X'){
            if(verificaPosicao(2, 1, partida)){
                partida[2][1] = 'O';
                return 1;
            }
        }
        //se tiver duas marcações de X na coluna 2
        if(partida[0][2] == 'X' && partida[1][2] == 'X'){
            if(verificaPosicao(2, 2, partida)){
                partida[2][2] = 'O';
                return 1;
            }
        }
        if(partida[1][2] == 'X' && partida[2][2] == 'X'){
            if(verificaPosicao(0, 2, partida)){
                partida[0][2] = 'O';
                return 1;
            }
        }
        if(partida[0][2] == 'X' && partida[2][2] == 'X'){
            if(verificaPosicao(1, 2, partida)){
                partida[1][2] = 'O';
                return 1;
            }
        }

        //caso tenha 2 marcações de X na diagonal principal:
        if(partida[0][0] == 'X' && partida[1][1] == 'X'){
            if(verificaPosicao(2, 2, partida)){
                partida[2][2] = 'O';
                return 1;
            }
        }
        if(partida[0][0] == 'X' && partida[2][2] == 'X'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'X' && partida[2][2] == 'X'){
            if(verificaPosicao(0, 0, partida)){
                partida[0][0] = 'O';
                return 1;
            }
        }

        //caso tenha 2 marcações de X na diagonal secundaria:
        if(partida[0][2] == 'X' && partida[1][1] == 'X'){
            if(verificaPosicao(2, 0, partida)){
                partida[2][0] = 'O';
                return 1;
            }
        }
        if(partida[0][2] == 'X' && partida[2][0] == 'X'){
            if(verificaPosicao(1, 1, partida)){
                partida[1][1] = 'O';
                return 1;
            }
        }
        if(partida[1][1] == 'X' && partida[2][0] == 'X'){
            if(verificaPosicao(0, 2, partida)){
                partida[0][2] = 'O';
                return 1;
            }
        }
    }
    //a partir daqui não existe nenhuma forma do jogador ganhar na proxima jogada
    //verificando qual a melhor jogada pro computador
    if(partida[1][1] == 'X' && partida[2][2] == 'X'){
        if(verificaPosicao(2, 0, partida)){
            partida[2][0] = 'O';
            return 1;
        }
    }    
    if((partida[0][0] == 'X' && partida[2][2] == 'X') || (partida[0][2] == 'X' && partida[2][0] == 'X')){
        if(verificaPosicao(0, 1, partida)){
            partida[0][1] = 'O';
            return 1;
        }
    }
    if(partida[0][0] == 'X'){
        if(partida[2][1] == 'X'){
            partida[1][2] = 'O';
            return 1;
        }
        if(partida[1][2] == 'X'){
            partida[2][1] = 'O';
            return 1;
        }
    }
    if(partida[0][2] == 'X'){
        if(partida[2][1] == 'X'){
            partida[1][0] = 'O';
            return 1;
        }
        if(partida[1][0] == 'X'){
            partida[2][1] = 'O';
            return 1;
        }
    }
    if(partida[2][2] == 'X'){
        if(partida[0][1] == 'X'){
            partida[1][0] = 'O';
            return 1;
        }
        if(partida[1][0] == 'X'){
            partida[0][1] = 'O';
            return 1;
        }
    }
    if(partida[2][0] == 'X'){
        if(partida[0][1] == 'X'){
            partida[1][2] = 'O';
            return 1;
        }
        if(partida[1][2] == 'X'){
            partida[0][1] = 'O';
            return 1;
        }
    }
    if(partida[0][1] == 'X'){
        if(partida[2][0] != 'X'){
            partida[2][0] = 'O';
            return 1;
        }
    }
    if(partida[1][2] == 'X'){
        if(partida[2][0] != 'X'){
            partida[2][0] = 'O';
            return 1;
        }
    }
    if(partida[2][1] == 'X'){
        if(partida[0][0] != 'X'){
            partida[0][0] = 'O';
            return 1;
        }
    }
    if(partida[1][0] == 'X'){
        if(partida[2][2] != 'X'){
            partida[2][2] = 'O';
            return 1;
        }
    }

    return 0;
}