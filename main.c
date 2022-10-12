//LEMBRETES PARA IMPLEMENTAR: linhas 18, 285, 350(func executarPartida)

//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char menu(); //exibe o menu e retorna a entrada da opção escolhida pelo usuário
void leNomeJogadores(char *jogador1, char *jogador2, char numJogadores);
void iniciaPartidaVazia(char **partida);
void exibeJogo(char **partida);
void leComando(char *comando, char *acao, char *jogador);
int charToInt(char c);
int verificaDigito(char posicao); //verifica se o usuario realmente digitou números de 1 a 3 posição, retorna 1 caso for isso, e 0 caso contrário
int verificaPosicao(int linha, int coluna, char **partida); //verifica se a posição está disponível, retorna 1 caso esteja, e 0 caso contrário
int verificaPartida(char **partida, char *jogador, int jogadorDaVez); //retorna 1 se a partida acabou e 0 caso contrário
int validaNomeArquivo(char *nomeArquivo);
int partidaDeDois(char **partida, char *jogador, int jogadorDaVez, char *outroJogador); //retorna 1 se a partida acabou, 0 caso contrário, e 2 caso o jogador tenha apenas salvado ela ou digitado um comando errado
                                            //tentar quebrar a função em mais funções ou diluir e colocar alguns comandos dela na main, tlvz criar tbm uma função para cada opção de ação
void executarPartida(char numJogadores, char **partida, char *jogador1, char *jogador2, int jogadorDaVez);
void getJogoSalvo(FILE *arquivo);
void limpaBuffer(); //limpar os caracteres do buffer até \n
char **alocaMatriz(int n, int m);
void liberaMatriz(char **matriz, int n);

int main(){
    printf("Bem vindo ao Jogo da Velha\n\n");
    
    char opMenu;
    char **partida;
    char jogador1[100], jogador2[100], numJogadores[3];
    partida = alocaMatriz(3, 3);
    do{
        opMenu = menu();

        switch (opMenu)
        {
            case '0': //encerrar jogo  
                printf("Jogo Encerrado!\n");
            break;

            case '1': //iniciar novo jogo
                do{
                    printf("\nDigite o número de jogadores (1 ou 2): ");
                    //o numJogadores pode ter apenas 1 caracter, já que é 1 ou 2 jogadores, mas o limite do fgets é 3 apenas para fazer a validação
                    //se o numJogadores[1] != '\n', sinal que o usuario digitou mais de um caracter
                    fgets(numJogadores, 3, stdin); 
                    if(numJogadores[1] != '\n'){
                        limpaBuffer(); //limpando o buffer dos caracteres lidos além 3°
                        printf("\nNúmero de jogadores inválido!\n");
                    }
                    else if(numJogadores[0] != '1' && numJogadores[0] != '2')
                        printf("\nNúmero de jogadores inválido!\n");

                } while (numJogadores[1]!='\n' || (numJogadores[0] != '1' && numJogadores[0] != '2'));   

                iniciaPartidaVazia(partida); //limpando o tabuleiro das possíveis partidas anteriores
                leNomeJogadores(jogador1, jogador2, numJogadores[0]);

                executarPartida(numJogadores[0], partida, jogador1,jogador2, 1);
            break;

            case '2': //continuar jogo salvo
                FILE *arq;
                char nomeArq[94];

                do{
                    printf("\nDigite o nome do arquivo que o jogo está salvo: ");
                    scanf("%s", nomeArq);
                    limpaBuffer();

                    arq = fopen(nomeArq, "r");

                    if(arq == NULL){
                        printf("\nArquivo não encontrado!\n");
                    }
                } while (!validaNomeArquivo(nomeArq) || arq == NULL);
                
                getJogoSalvo(arq);

            break;

            case '3': //continuar jogo atual

            break;

            default: //exibir ranking (opção 4 do menu)

            break;
        }

    } while (opMenu != '0');

    liberaMatriz(partida, 3);
        
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
    printf("Digite o nome do jogador 1: ");
    fgets(jogador1, 100, stdin);
    jogador1[strlen(jogador1)-1] = '\0';

    if(numJogadores == '2'){
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
    printf("|---------------|\n");
    printf("| 1 | %c | %c | %c |\n", partida[0][0], partida[0][1], partida[0][2]);
    printf("|---------------|\n");
    printf("| 2 | %c | %c | %c |\n", partida[1][0], partida[1][1], partida[1][2]);
    printf("|---------------|\n");
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

int verificaPartida(char **partida, char *jogador, int jogadorDaVez){

    char verificador = jogadorDaVez == 1 ? 'X' : 'O';

    if(partida[0][0] == verificador){
        if(partida[0][1] == verificador && partida[0][2] == verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
        else if(partida[1][0] == verificador && partida[2][0] == verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
        else if(partida[1][1] == verificador && partida[2][2] == verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
    }
    if(partida[0][1] == verificador){
        if(partida[1][1]==verificador && partida[2][1]==verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
    }
    if (partida[1][0] == verificador){
        if (partida[1][1]==verificador && partida[1][2]==verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
    }
    if (partida[2][2] == verificador){
        if(partida[2][0]==verificador && partida[2][1]==verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
        else if(partida[0][2]==verificador && partida[1][2]==verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
            return 1;
        }
    }
    if(partida[0][2] == verificador){
        if(partida[1][1]==verificador && partida[2][0]==verificador){
            printf("Parabéns %s, você ganhou!\n", jogador);
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
        return 1;
    }

    return 0; //caso não entre em nenhuma verificacao acima, a partida continuará em andamento
}

int validaNomeArquivo(char *nomeArquivo){
    int lengthNomeArq = strlen(nomeArquivo);

    //se na posição 0 for um ., é pq a pessoa nao digitou um nome pro arquivo vaĺido
    //se nas ultimas 3 posições nao for "txt" a pessoa digitou uma extensao invalida
    if(nomeArquivo[0] == '.' || nomeArquivo[lengthNomeArq-1] != 't' || nomeArquivo[lengthNomeArq-2] != 'x' || nomeArquivo[lengthNomeArq-3] != 't'){
        printf("Nome inválido para arquivo ou extensão inválida (precisa ser .txt)!\n");

        return 0;
    }

    return 1;
}

int partidaDeDois(char **partida, char *jogador, int jogadorDaVez, char *outroJogador){ 
    /*retorno: 
    1: se a partida acabou, 
    0: caso a partida continue normalmente,
    2: caso o jogador tenha apenas salvado ela ou digitado um comando errado, nesse caso é a vez do mesmo jogador */
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

        //verifica se o usuario realmente digitou números de 1 a 3 para posição e se essa posicao está livre ou já foi marcada
        if(!verificaDigito(posicaoX) || !verificaDigito(posicaoY) || !verificaPosicao(linha, coluna, partida)){
            printf("Posições inválidas ou já marcadas! Digite apenas números de 1 até 3.\n");
            
            return 2;
        }
        
        //se jogadorDaVez = 1, marcar com X, se jogadorDaVez = 2, marcar com O
        if(jogadorDaVez == 1)
            partida[linha][coluna] = 'X';
        else
            partida[linha][coluna] = 'O';

        exibeJogo(partida);
        
        //se verificaPartida() retornar 0, a partida ainda não acabou, caso tenha acabado, retorna 1 e exibe uma msg de parabens ou de empate
        if(!verificaPartida(partida, jogador, jogadorDaVez)) 
            return 0;
        else
            return 1;

        //implementar para aos fins das partidas, atualizar o ranking com vitória, empate e derrota de cada jogador
        //problema: o jogador que não estiver no ranking vai perder seus dados de V, E e D. O que fazer?

    }
    else if (!strcmp(acao, "salvar"))
    {
        FILE *arquivo;
        char nomeArquivo[94]; //100 - 6    comando - os caracteres da acao

        int cont = 7;
        while(comando[cont] != '\0' && cont<100){
            nomeArquivo[cont-7] = comando[cont];
            cont++;
        }
        nomeArquivo[cont-7] = '\0';
        cont = 0;

        //se na posição 0 for um ., é pq a pessoa nao digitou um nome pro arquivo vaĺido
        //se nas ultimas 3 posições nao for "txt" a pessoa digitou uma extensao invalida
        if(!validaNomeArquivo(nomeArquivo))
            return 2;

        arquivo = fopen(nomeArquivo, "w");
        
        fprintf(arquivo, "2\n");

        //if para deixar ordenado no arquivo do jogo o nome do jogador 1 primeiro, e do jogador 2 embaixo
        if(jogadorDaVez == 1)
            fprintf(arquivo, "%s\n%s\n", jogador, outroJogador);
        else
            fprintf(arquivo, "%s\n%s\n", outroJogador, jogador);

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

        return 2;
    }
    else{ //voltar

    }
    
}

void executarPartida(char numJogadores, char **partida, char *jogador1, char *jogador2, int jogadorDaVez){
    exibeJogo(partida);

    if(numJogadores == '1'){
        //programar função para jogar contra o bot do computador
    }   
    else{ //2 jogadores 
        //jogadorDaVez:   ímpar: vez do jogador 1   par: vez do jogador 2
        int partidaFinalizada;
        if(jogadorDaVez == 1)
            partidaFinalizada = partidaDeDois(partida, jogador1, jogadorDaVez, jogador2);
        else
            partidaFinalizada = partidaDeDois(partida, jogador2, jogadorDaVez, jogador1);

        while(!partidaFinalizada || partidaFinalizada == 2){ //se o partidaDeDois retornar 2 o jogador apenas salvou a partida ou digitou algo errado
            if(!partidaFinalizada) //caso o partidaFinalizada tenha recebido 0, a rodada aconteceu normalmente e agr é a vez do outro jogador
                jogadorDaVez++;

            if(jogadorDaVez % 2 == 0)
                partidaFinalizada = partidaDeDois(partida, jogador2, 2, jogador1);
            else
                partidaFinalizada = partidaDeDois(partida, jogador1, 1, jogador2);
        }

        printf("Digite qualquer tecla para continuar!\n");
        limpaBuffer();
    }  
}

void getJogoSalvo(FILE *arquivo){
    int numJogadores, jogadorDaVez;
    char jogadores[2][100], **partidaSalva;

    partidaSalva = alocaMatriz(3, 3);

    fscanf(arquivo, "%d\n", &numJogadores);

    fgets(jogadores[0], 100, arquivo);
    jogadores[0][strlen(jogadores[0])-1] = '\0';

    fgets(jogadores[1], 100, arquivo);
    jogadores[1][strlen(jogadores[1])-1] = '\0';

    for (int i = 0; i < 3; i++){
        for (int j = 0; j < 3; j++){
            fscanf(arquivo, "%c ", &partidaSalva[i][j]);
            if(partidaSalva[i][j] == '-')
                partidaSalva[i][j] = ' ';
        }
        //fscanf(arquivo, "\n");        
    }

    fscanf(arquivo, "%d", &jogadorDaVez);
    
    fclose(arquivo);

    executarPartida(numJogadores, partidaSalva, jogadores[0], jogadores[1], jogadorDaVez);

    liberaMatriz(partidaSalva, 3);
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