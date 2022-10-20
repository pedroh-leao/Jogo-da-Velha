//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{ //struct usada para o rankeamento
    char nome[100];
    int vitorias;
    int empates;
    int derrotas;
}Jogadores;

char menu(); //exibe o menu e retorna a entrada da opção escolhida pelo usuário
void leNomeJogadores(char *jogador1, char *jogador2, char numJogadores);
void iniciaPartidaVazia(char **partida);
void exibeJogo(char **partida);
void leComando(char *comando, char *acao, char *jogador);
int charToInt(char c);
int verificaDigito(char posicao); //verifica se o usuario realmente digitou números de 1 a 3 posição, retorna 1 caso for isso, e 0 caso contrário
int verificaPosicao(int linha, int coluna, char **partida); //verifica se a posição está disponível, retorna 1 caso esteja, e 0 caso contrário
int verificaPartida(char **partida, char *jogador, int jogadorDaVez); //retorno 0: ainda não acabou, 1: vitória do jogador da vez, 2: empate
int validaNomeArquivo(char *nomeArquivo);
int validaNumJogadores(char *numJogadores); //retorna 0 caso seja um numero de jogadores invalido
int marcar(char **partida, int jogadorDaVez, char *comando); //retorna 1 caso a marcação dê certo, e 0 caso falhe nas validações
void salvar(FILE *arquivo, char **partida, char numJogadores, int jogadorDaVez, char *jogador1, char *jogador2, char *nomeArquivo);
int proximoComando(char **partida, char *jogador, int jogadorDaVez, char *outroJogador, int *partidaEmAndamento, char numJogadores, Jogadores **rankeamento, int *size); //retorna 1 se a partida acabou, 0 caso contrário, e 2 caso o jogador tenha apenas salvado ela ou digitado um comando errado
void executarPartida(char numJogadores, char **partida, char *jogador1, char *jogador2, int jogadorDaVez, int *partidaEmAndamento, Jogadores **rankeamento, int *size);
void getJogoSalvo(FILE *arquivo, char **partida, char *jogador1,  char *jogador2, int *partidaEmAndamento, char *numJogadores, Jogadores **rankeamento, int *size); //pega o jogo salvo no arquivo, e continua a partida
int verificaQualJogadorDaVez(char **partida); //usado para continuar uma partida atual (case '3')
void limpaBuffer(); //limpar os caracteres do buffer até \n
char **alocaMatriz(int n, int m);
void liberaMatriz(char **matriz, int n);
int jogadaDoComputador(char **partida);
void leArquivoRanking(Jogadores **rankeamento, int *numJogadores);
void atualizaArquivoRanking(Jogadores *rankeamento, int size);
void atualizaVED(Jogadores **rankeamento, int *size, char *jogador, int resultado);
int existeNoRanking(Jogadores *rankeamento, int size, char *jogador); //verifica se o jogador já está no ranking, caso exista, retorna o indice dele, caso contrário, retorna -1
void ordenaRanking(Jogadores *rankeamento, int size);
void realocaVetor(Jogadores **rankeamento, int size); //realoca vetor usando apenas malloc
void exibePosicaoJogadores(Jogadores *rankeamento, char *jogador1, char *jogador2);
void exibeRanking(Jogadores *rankeamento, int size);

int main(){
    printf("Bem vindo ao Jogo da Velha\n\n");
    
    char opMenu, **partida;
    char jogador1[100], jogador2[100], numJogadores[3];
    partida = alocaMatriz(3, 3);
    
    FILE *arq;
    char nomeArq[94];
    //variavel para dizer se tem alguma partida em andamento, quando o usuario tentar entrar no case 3
    int partidaEmAndamento = 0; //1 se existe uma em andamento, e 0 caso contrário

    Jogadores *rankeamento = NULL; //ranking dos jogadores
    int size; //representa o numero de jogadores ranqueados
    leArquivoRanking(&rankeamento, &size);

    do{
        opMenu = menu();

        switch (opMenu)
        {
            case '0': //encerrar jogo  
                atualizaArquivoRanking(rankeamento, size);
                printf("Jogo Encerrado!\n");
            break;

            case '1': //iniciar novo jogo
                do{
                    printf("\nDigite o número de jogadores (1 ou 2): ");

                    /* O numJogadores pode ter apenas 1 caracter, já que é 1 ou 2 jogadores, entretanto o limite do fgets é 3 para fazer a validação,
                    dessa forma, se o numJogadores[1] != '\n', sinal que o usuario digitou mais de um caracter */
                    fgets(numJogadores, 3, stdin); 

                } while (!validaNumJogadores(numJogadores));   

                iniciaPartidaVazia(partida); //limpando o tabuleiro das possíveis partidas anteriores
                leNomeJogadores(jogador1, jogador2, numJogadores[0]);

                executarPartida(numJogadores[0], partida, jogador1, jogador2, 1, &partidaEmAndamento, &rankeamento, &size);
            break;

            case '2': //continuar jogo salvo
                do{
                    printf("\nDigite o nome do arquivo que o jogo está salvo: ");
                    scanf("%s", nomeArq);
                    limpaBuffer();

                    arq = fopen(nomeArq, "r");

                    if(arq == NULL){
                        printf("\nArquivo não encontrado!\n");
                    }
                } while (!validaNomeArquivo(nomeArq) || arq == NULL);
                
                getJogoSalvo(arq, partida, jogador1, jogador2, &partidaEmAndamento, &numJogadores[0], &rankeamento, &size); //pega o jogo salvo no arquivo, e continua a partida

            break;

            case '3': //continuar jogo atual
                if(!partidaEmAndamento)
                    printf("\nNão existe nenhuma partida atual!\n\n");
                else{
                    //preciso continuar o jogo, tanto se ele tiver começado do zero (case 1), quanto se a partida tiver pegada de um arquivo salvo(case 2)
                    if(verificaQualJogadorDaVez(partida) == 1)
                        executarPartida(numJogadores[0], partida, jogador1, jogador2, 1, &partidaEmAndamento, &rankeamento, &size);
                    else
                        executarPartida(numJogadores[0], partida, jogador1, jogador2, 2, &partidaEmAndamento, &rankeamento, &size);
                }
            break;

            default: //exibir ranking (opção 4 do menu)
                exibeRanking(rankeamento, size);
                printf("Pressione qualquer tecla para voltar!\n");
                limpaBuffer();
            break;
        }

    } while (opMenu != '0');

    liberaMatriz(partida, 3);
    free(rankeamento);
        
    return 0;
}

char menu(){
    printf("0. Sair do Jogo\n");
    printf("1. Começar um novo jogo\n");
    printf("2. Continuar um jogo salvo\n");
    printf("3. Continuar o jogo atual\n");
    printf("4. Exibir o ranking\n");
    printf("Durante o jogo digite “voltar” para retornar ao menu.\n");

    char op[3];
    do{
        printf("\nEscolha a opção: ");
        //o op pode ter apenas 1 caracter, já que o menu é de 1 a 4, mas o limite do fgets é 3 apenas para fazer a validação
        //se o op[1] != '\n', sinal que o usuario digitou mais de um caracter
        fgets(op, 3, stdin); 
        if(op[1] != '\n'){
            limpaBuffer(); //limpando o buffer dos caracteres lidos além 3°
            printf("Opção inválida!");
        }
        else if(op[0] < '0' || op[0] > '4')
            printf("Opção inválida!");
    }while(op[1] != '\n' || op[0]<'0' || op[0]>'4');
    
    return op[0];
}

void leNomeJogadores(char *jogador1, char *jogador2, char numJogadores){
    int invalido;
    do{
        printf("Digite o nome do jogador 1: ");
        fgets(jogador1, 100, stdin);
        jogador1[strlen(jogador1)-1] = '\0';

        if(numJogadores == '2'){
            printf("Digite o nome do jogador 2: ");
            fgets(jogador2, 100, stdin);
            jogador2[strlen(jogador2)-1] = '\0';
        }
        else
            strcpy(jogador2, "Computador");

        invalido = 0;
        if(!strcmp(jogador1, "") || !strcmp(jogador2, "")){
            printf("Os nomes não podem ser nulos!\n\n");
            invalido = 1;
        }
        else if(!strcmp(jogador1, "Computador") || !strcmp(jogador2, "Computador")){
            printf("Os nomes não podem receber \"Computador\"!\n\n");
            invalido = 1;
        }
        else if(!strcmp(jogador1, jogador2)){
            printf("Os nomes não podem ser iguais!\n\n");
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
    printf("-----------------\n");
    printf("|   | 1 | 2 | 3 |\n");
    printf("|---------------|\n");
    printf("| 1 | %c | %c | %c |\n", partida[0][0], partida[0][1], partida[0][2]);
    printf("|---------------|\n");
    printf("| 2 | %c | %c | %c |\n", partida[1][0], partida[1][1], partida[1][2]);
    printf("|---------------|\n");
    printf("| 3 | %c | %c | %c |\n", partida[2][0], partida[2][1], partida[2][2]);    
    printf("-----------------\n");
}

void leComando(char *comando, char *acao, char *jogador){
    int invalido;
    do{
        printf("\n%s, digite o comando: ", jogador);
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
            printf("Comando inválido!\n");
    } while (invalido); //enquanto o comando estiver invalido
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

int verificaPartida(char **partida, char *jogador, int jogadorDaVez){
    //retorno 0: ainda não acabou, 1: vitória do jogador da vez, 2: empate
    char verificador = jogadorDaVez == 1 ? 'X' : 'O';

    if(partida[0][0] == verificador){
        if(partida[0][1] == verificador && partida[0][2] == verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
        if(partida[1][0] == verificador && partida[2][0] == verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
        if(partida[1][1] == verificador && partida[2][2] == verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
    }
    if(partida[0][1] == verificador){
        if(partida[1][1]==verificador && partida[2][1]==verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
    }
    if (partida[1][0] == verificador){
        if (partida[1][1]==verificador && partida[1][2]==verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
    }
    if (partida[2][2] == verificador){
        if(partida[2][0]==verificador && partida[2][1]==verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
        if(partida[0][2]==verificador && partida[1][2]==verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
            return 1;
        }
    }
    if(partida[0][2] == verificador){
        if(partida[1][1]==verificador && partida[2][0]==verificador){
            if(strcmp(jogador, "Computador"))
                printf("Parabéns %s, você ganhou!\n", jogador);
            else
                printf("Vitória do computador!\n");
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
        printf("Velha! O jogo terminou empatado!\n");
        return 2;
    }

    return 0; //caso não entre em nenhuma verificacao acima, a partida continuará em andamento
}

int validaNomeArquivo(char *nomeArquivo){
    int lengthNomeArq = strlen(nomeArquivo);

    //se na posição 0 for um ., é pq a pessoa nao digitou um nome pro arquivo vaĺido
    //se nas ultimas 4 posições nao for ".txt" a pessoa digitou uma extensao invalida
    if(nomeArquivo[0] == '.' || nomeArquivo[lengthNomeArq-1] != 't' || nomeArquivo[lengthNomeArq-2] != 'x' || nomeArquivo[lengthNomeArq-3] != 't' || nomeArquivo[lengthNomeArq-4] != '.'){
        printf("Nome inválido para arquivo ou extensão inválida (precisa ser .txt)!\n");

        return 0;
    }

    return 1;
}

int validaNumJogadores(char *numJogadores){
    //retorna 0 caso seja um numero de jogadores invalido
    if(numJogadores[1] != '\n' || (numJogadores[0] != '1' && numJogadores[0] != '2')){
        if(numJogadores[1] != '\n')
            limpaBuffer(); //limpando o buffer dos caracteres lidos além 3°
        
        printf("\nNúmero de jogadores inválido!\n");
        return 0;
    }

    return 1;
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
        printf("Posições inválidas ou já marcadas! Digite apenas números de 1 até 3.\n");
            
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

    printf("Arquivo “%s” salvo com sucesso!\n", nomeArquivo);
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

                printf("Digite qualquer tecla para continuar!\n");
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
        printf("Vez do computador: \n");
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

            printf("Digite qualquer tecla para continuar!\n");
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

void leArquivoRanking(Jogadores **rankeamento, int *numJogadores){
    FILE *arqRanking;
    arqRanking = fopen("velha.ini", "r");

    int emptyFile = 0, nullFile = 1;

    //verifica se o arquivo está vazio
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
        fscanf(arqRanking, "%d\n", numJogadores);
        *rankeamento = (Jogadores *) malloc(*numJogadores * sizeof(Jogadores));

        for (int i = 0; i < *numJogadores; i++)
        {
            fgets((*rankeamento)[i].nome, 100, arqRanking);
            (*rankeamento)[i].nome[strlen((*rankeamento)[i].nome) - 1] = '\0';
            fscanf(arqRanking, "%d %d %d\n", &(*rankeamento)[i].vitorias, &(*rankeamento)[i].empates, &(*rankeamento)[i].derrotas);
            //printf("%s\n%d %d %d\n", (*rankeamento)[i].nome, (*rankeamento)[i].vitorias, (*rankeamento)[i].empates, (*rankeamento)[i].derrotas);
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
    
    printf("\nPosição no ranking:\n\n");
    while(jogadores>0){
        if (!strcmp(rankeamento[i].nome, jogador1) || !strcmp(rankeamento[i].nome, jogador2)){
            printf("%d°- %s\n", i+1, rankeamento[i].nome);
            printf("V- %d   E- %d   D- %d\n\n", rankeamento[i].vitorias, rankeamento[i].empates, rankeamento[i].derrotas);

            jogadores--;
        }      
        i++;
    }
}

void exibeRanking(Jogadores *rankeamento, int size){
    printf("\n..:: Ranking ::..\n\n");
    for (int i = 0; i < size; i++){
        printf("%d°- %s\n", i+1, rankeamento[i].nome);
        printf("V- %d   E- %d   D- %d\n\n", rankeamento[i].vitorias, rankeamento[i].empates, rankeamento[i].derrotas);
    }    
}