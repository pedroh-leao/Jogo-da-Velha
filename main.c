//Pedro Henrique Rabelo Leão de Oliveira - 22.1.4022
#include "funcoesJogo.h"
#include "funcoesRanking.h"
#include "funcoesValidacao.h"
#include "formatacao.h"

int main(){
    printf(GREEN(BOLD("\nBem vindo ao Jogo da Velha\n\n")));
    
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
                printf(GREEN(BOLD("Jogo Encerrado!\n\n")));
            break;

            case '1': //iniciar novo jogo
                do{
                    printf(WHITE(BOLD("\nDigite o número de jogadores (1 ou 2): ")));

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
                    printf(WHITE(BOLD("\nDigite o nome do arquivo que o jogo está salvo: ")));
                    scanf("%s", nomeArq);
                    limpaBuffer();

                    arq = fopen(nomeArq, "r");

                    if(arq == NULL){
                        printf(RED(BOLD("\nArquivo não encontrado!\n")));
                    }
                } while (!validaNomeArquivo(nomeArq) || arq == NULL);
                
                getJogoSalvo(arq, partida, jogador1, jogador2, &partidaEmAndamento, &numJogadores[0], &rankeamento, &size); //pega o jogo salvo no arquivo, e continua a partida

            break;

            case '3': //continuar jogo atual
                if(!partidaEmAndamento)
                    printf(RED(BOLD("\nNão existe nenhuma partida atual!\n\n")));
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
                printf(WHITE(BOLD("Pressione qualquer tecla para voltar!\n")));
                limpaBuffer();
            break;
        }

    } while (opMenu != '0');

    liberaMatriz(partida, 3);
    free(rankeamento);
        
    return 0;
}