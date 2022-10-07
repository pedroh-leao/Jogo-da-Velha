LEMBRETES PARA IMPLEMENTAR: linhas 199, 219, 220, 221

//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int menu(); //exibe o menu e retorna a entrada da opção escolhida pelo usuário
void leNomeJogadores(char *jogador1, char *jogador2, int numJogadores);
void iniciaPartidaVazia(char **partida);
void exibeJogo(char **partida);
void leComando(char *comando, char *acao, char *jogador);
int charToInt(char c);
int verificaDigito(char posicao); //verifica se o usuario realmente digitou números de 1 a 3 posição, retorna 1 caso for isso, e 0 caso contrário
int verificaPosicao(int linha, int coluna, char **partida); //verifica se a posição está disponível, retorna 1 caso esteja, e 0 caso contrário
int executaPartidaDeDois(char **partida, char *jogador, int jogadorDaVez); //retorna 1 se a partida acabou, e 0 caso contrário
                                                                        //tentar quebrar a função em mais funções ou diluir e colocar alguns comandos dela na main
char **alocaMatriz(int n, int m);
void liberaMatriz(char **matriz, int n);

int main(){
    printf("Bem vindo ao Jogo da Velha\n\n");
    
    int opMenu, numJogadores;
    char jogador1[100], jogador2[100], **partida;
    partida = alocaMatriz(3, 3);
    do{
        opMenu = menu();

        switch (opMenu)
        {
            case 0: //encerrar jogo                
            break;

            case 1: //iniciar novo jogo
                do{
                    printf("\nDigite o número de jogadores (1 ou 2): ");
                    scanf("%d", &numJogadores);

                    if(numJogadores != 1 && numJogadores != 2)
                        printf("\nNúmero de jogadores inválido!\n");

                } while (numJogadores != 1 && numJogadores != 2);   

                iniciaPartidaVazia(partida);
                leNomeJogadores(jogador1, jogador2, numJogadores);
                exibeJogo(partida);

                if(numJogadores == 1){
                    //programar função para jogar contra o bot do computador
                }   
                else{ //2 jogadores                    
                    int jogadorDaVez = 1; //ímpar: vez do jogador 1   par: vez do jogador 2
                    int partidaFinalizada = executaPartidaDeDois(partida, jogador1, 1);
                    while(!partidaFinalizada){
                        jogadorDaVez++;

                        if(jogadorDaVez % 2 == 0)
                            partidaFinalizada = executaPartidaDeDois(partida, jogador2, 2);
                        else
                            partidaFinalizada = executaPartidaDeDois(partida, jogador1, 1);
                    }
                }       

            break;

            case 2: //continuar jogo salvo

            break;

            case 3: //continuar jogo atual

            break;

            default: //exibir ranking (opção 4 do menu)

            break;
        }

    } while (opMenu != 0);

    liberaMatriz(partida, 3);
        
    return 0;
}

int menu(){
    printf("0. Sair do Jogo\n");
    printf("1. Começar um novo jogo\n");
    printf("2. Continuar um jogo salvo\n");
    printf("3. Continuar o jogo atual\n");
    printf("4. Exibir o ranking\n");
    printf("Durante o jogo digite “voltar” para retornar ao menu.\n");

    int op;
    do{
        printf("\nEscolha a opção: ");    
        scanf("%d", &op);

        if(op < 0 || op > 4)
            printf("Opção inválida!");
    }while(op<0 || op>4);

    return op;
}

void leNomeJogadores(char *jogador1, char *jogador2, int numJogadores){
    getchar();

    printf("Digite o nome do jogador 1: ");
    fgets(jogador1, 100, stdin);
    jogador1[strlen(jogador1)-1] = '\0';

    if(numJogadores == 2){
        printf("Digite o nome do jogador 2: ");
        fgets(jogador2, 100, stdin);
        jogador2[strlen(jogador2)-1] = '\0';
    }
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
    printf("-----------------\n");
    printf("|   | 1 | 2 | 3 |\n");
    printf("-----------------\n");
    printf("| 1 | %c | %c | %c |\n", partida[0][0], partida[0][1], partida[0][2]);
    printf("-----------------\n");
    printf("| 2 | %c | %c | %c |\n", partida[1][0], partida[1][1], partida[1][2]);
    printf("-----------------\n");
    printf("| 3 | %c | %c | %c |\n", partida[2][0], partida[2][1], partida[2][2]);    
    printf("-----------------\n");
}

void leComando(char *comando, char *acao, char *jogador){
    do{
        printf("\n%s, digite o comando: ", jogador);
        fgets(comando, 100, stdin);
        comando[strlen(comando)-1] = '\0';
        printf("\n");
        
        for (int i = 0; i < 6; i++){
            acao[i] = comando[i];
        }
        acao[6] = '\0';

        if(strcmp(acao, "marcar") && strcmp(acao, "salvar") && strcmp(acao, "voltar")){
            printf("Comando inválido!\n");
        }
    } while (strcmp(acao, "marcar") && strcmp(acao, "salvar") && strcmp(acao, "voltar")); //enquanto o comando não for nenhum desses
}

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

int executaPartidaDeDois(char **partida, char *jogador, int jogadorDaVez){ 
    char comando[100];
    char acao[7]; //marcar, salvar ou voltar

    leComando(comando, acao, jogador);
    
    if(!strcmp(acao, "marcar")){
        char posicaoX, posicaoY;
        posicaoY = comando[7]; //numero da linha
        posicaoX = comando[8]; //numero da coluna
        int linha, coluna;

        //conversão da posição de char para um inteiro        
        linha = charToInt(posicaoY) -1;
        coluna = charToInt(posicaoX) -1;

        //implementar para conferir se ele vai digitar o mesmo comando novamente, tendo em vista que já estamos dentro do if do marcar

        //verifica se o usuario realmente digitou números de 1 a 3 para posição e se essa posicao está livre ou já foi marcada
        while (!verificaDigito(posicaoX) || !verificaDigito(posicaoY) || !verificaPosicao(linha, coluna, partida)){
            printf("Posições inválidas ou já marcadas! Digite apenas números de 1 até 3.\n");
            leComando(comando, acao, jogador);
            posicaoY = comando[7]; //numero da linha
            posicaoX = comando[8]; //numero da coluna
            linha = charToInt(posicaoY) -1;
            coluna = charToInt(posicaoX) -1;
        }
        
        //se jogadorDaVez = 1, marcar com X, se jogadorDaVez = 2, marcar com O
        if(jogadorDaVez == 1)
            partida[linha][coluna] = 'X';
        else
            partida[linha][coluna] = 'O';

        exibeJogo(partida);
        
        //implementar para retornar 1 caso a partida tenha acabado, e 0 caso contrário
        //implementar para exibir mensagem de vitória com o nome do ganhador (não necessariamente será aqui dentro da função, tlvz será na main)
        //implementar para depois da msg de vitoria, aparecer "Digite qualquer tecla para continuar!" e esperar o usuario digitar uma tecla se possível

    }
    else if (!strcmp(acao, "salvar"))
    {
        
    }
    else{ //voltar

    }
    
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