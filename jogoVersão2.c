#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#ifdef _WIN32
#include <Windows.h>
#define LIMPA "cls"
#else
#include <unistd.h>
#define LIMPA "clear"
#endif

#define ORG 'X'
#define VAZ '.'
#define BUFFER 1024
#define TAM 100

typedef struct Jogo
{
    char NomeJogo[TAM];
    int numeroLinha, numeroColuna, numeroCiclos;
    char **MatrizAtual;
} jogo;
//////////////////////////////////////////////////////////////////////////////////////////////
void copiaMatriz(jogo *Tabuleiro, char **MatrizCopia)
{
    int i, j;
    for (i = 0; i < Tabuleiro->numeroLinha; i++)
    {
        for (j = 0; j < Tabuleiro->numeroColuna; j++)
        {
            MatrizCopia[i][j] = Tabuleiro->MatrizAtual[i][j];
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void atualizaMat(jogo *Tabuleiro, char **MatrizCopia)
{
    int i, j, soma = 0;
    for (i = 0; i < Tabuleiro->numeroLinha; i++)
    {
        for (j = 0; j < Tabuleiro->numeroColuna; j++)
        {
            if (i - 1 >= 0 && j - 1 >= 0)             // Verifica se essa posição é valida
                if (MatrizCopia[i - 1][j - 1] == ORG) // Cima-esquerda
                    soma++;

            if (i - 1 >= 0)                       // Verifica se essa posição é valida
                if (MatrizCopia[i - 1][j] == ORG) // Cima
                    soma++;

            if (i - 1 >= 0 && j + 1 < Tabuleiro->numeroColuna) // Verifica se essa posição é valida
                if (MatrizCopia[i - 1][j + 1] == ORG)          // Cima-Direita
                    soma++;

            if (j - 1 >= 0)                       // Verifica se essa posição é valida
                if (MatrizCopia[i][j - 1] == ORG) //  Esquerda
                    soma++;

            if (j + 1 < Tabuleiro->numeroColuna)  // Verifica se essa posição é valida
                if (MatrizCopia[i][j + 1] == ORG) // Direita
                    soma++;

            if (i + 1 < Tabuleiro->numeroLinha && j - 1 >= 0) // Verifica se essa posição é valida
                if (MatrizCopia[i + 1][j - 1] == ORG)         // Baixo-Esquerda
                    soma++;

            if (i + 1 < Tabuleiro->numeroLinha)   // Verifica se essa posição é valida
                if (MatrizCopia[i + 1][j] == ORG) // Baixo
                    soma++;
            if (i + 1 < Tabuleiro->numeroLinha && j + 1 < Tabuleiro->numeroColuna) // Verifica caso essa posição seja valida
                if (MatrizCopia[i + 1][j + 1] == ORG)                              // Baixo-Direita
                    soma++;

            if (MatrizCopia[i][j] == VAZ) // Quando a Célula ta Morta, Verifica se ela vai viver no proximo ciclo
                if (soma == 3)
                {
                    Tabuleiro->MatrizAtual[i][j] = ORG;
                    soma = 0;
                }

            if (MatrizCopia[i][j] == ORG) // Quando a Célula ta Viva, Verifica se ela vai Morrer no proximo ciclo
                if (soma > 3 || soma < 2)
                {
                    Tabuleiro->MatrizAtual[i][j] = VAZ;
                    soma = 0;
                }
            soma = 0;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void desalocaMatriz(int Linha, char **Matriz)
{
    for (int i = 0; i < Linha; i++)
    {
        free(Matriz[i]);
    }
    free(Matriz);
}
//////////////////////////////////////////////////////////////////////////////////////////////
char **alocaMatriz(int Linha, int Coluna)
{
    char **mat;
    int i;
    mat = (char **)malloc(Linha * sizeof(char *));
    if (mat == NULL)
    {
        printf("Memoria insuficiente.\n");
        exit(1);
    }
    else
    {
        for (i = 0; i < Coluna; i++)
        {
            mat[i] = (char *)malloc(Coluna * sizeof(char));
            if (mat[i] == NULL)
            {
                printf("Memoria insuficiente.\n");
                exit(1);
            }
        }
    }
    return mat;
}
//////////////////////////////////////////////////////////////////////////////////////////////
void limpaMatriz(int Linha, int Coluna, char **Matriz)
{
    int i, j;
    for (i = 0; i < Linha; i++)
    {
        for (j = 0; j < Coluna; j++)
        {
            Matriz[i][j] = VAZ;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void imprimeMatriz(jogo *Tabuleiro)
{
    int i, j;
    for (i = 0; i < Tabuleiro->numeroLinha; i++)
    {
        for (j = 0; j < Tabuleiro->numeroColuna; j++)
        {
            printf("%c ", Tabuleiro->MatrizAtual[i][j]);
        }
        printf("\n");
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void VerificaTamanhoMatriz(int *linha, int *Coluna, char *NomeArquivo)
{
    FILE *Arquivo;
    char LinhaArquivo[BUFFER];
    char *Caracter;
    Arquivo = fopen(NomeArquivo, "r");
    if (Arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s\n", NomeArquivo);
        return;
    }
    *linha = 0;
    *Coluna = 0;
    while (!feof(Arquivo))
    {
        fgets(LinhaArquivo, BUFFER, Arquivo);
        Caracter = strtok(LinhaArquivo, ",");
        while (Caracter != NULL)
        {
            if (*Coluna < atoi(Caracter))
            {
                *Coluna = atoi(Caracter);
            }
            Caracter = strtok(NULL, ",");
        }
        (*linha)++;
    }
    fclose(Arquivo);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void LerArquivo(jogo *Tabuleiro)
{
    FILE *Arquivo;
    char *NomeArquivo, **Padrao, *Caracter;
    char LinhaArquivo[BUFFER];
    int Linha, Coluna, i = 0, j, LinhaPadrao, ColunaPadrao;
    LinhaPadrao = Tabuleiro->numeroLinha / 2;
    ColunaPadrao = Tabuleiro->numeroColuna / 2;
    NomeArquivo = (char *)malloc(sizeof(Tabuleiro->NomeJogo) + 4 * sizeof(char *));
    if (NomeArquivo == NULL)
    {
        printf("Memoria insuficiente.\n");
        exit(1);
    }
    strcpy(NomeArquivo, Tabuleiro->NomeJogo);
    strcat(NomeArquivo, ".csv");
    Arquivo = fopen(NomeArquivo, "r");
    if (Arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s\n", NomeArquivo);
        return;
    }
    VerificaTamanhoMatriz(&Linha, &Coluna, NomeArquivo);
    Padrao = alocaMatriz(Linha, Coluna);
    limpaMatriz(Linha, Coluna, Padrao);
    limpaMatriz(Tabuleiro->numeroLinha, Tabuleiro->numeroColuna, Tabuleiro->MatrizAtual);
    while (!feof(Arquivo))
    {
        fgets(LinhaArquivo, BUFFER, Arquivo);
        Caracter = strtok(LinhaArquivo, ",");
        while (Caracter != NULL)
        {
            Padrao[i][atoi(Caracter) - 1] = ORG;
            Caracter = strtok(NULL, ",");
        }
        i++;
    }
    for (i = 0; i < Linha; i++)
    {
        for (j = 0; j < Coluna; j++)
        {
            Tabuleiro->MatrizAtual[LinhaPadrao + i][ColunaPadrao + j] = Padrao[i][j];
        }
    }
    fclose(Arquivo);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void Invasor(jogo *Tabuleiro)
{
    FILE *Arquivo;
    char *NomeArquivo, **Padrao, *Caracter;
    char LinhaArquivo[BUFFER];
    int Linha, Coluna, i = 0, j, LinhaPadrao, ColunaPadrao;
    int opcao;
    LinhaPadrao = Tabuleiro->numeroLinha / 2;
    ColunaPadrao = Tabuleiro->numeroColuna / 2;
    printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre o padrao do invasor: ");
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        NomeArquivo = (char *)malloc(TAM * sizeof(char));
        strcpy(NomeArquivo, "Bloco.csv");
        break;
    case 2:
        NomeArquivo = (char *)malloc(TAM * sizeof(char));
        strcpy(NomeArquivo, "Blinker.csv");
        break;
    case 3:
        NomeArquivo = (char *)malloc(TAM * sizeof(char));
        strcpy(NomeArquivo, "Sapo.csv");
        break;
    case 4:
        NomeArquivo = (char *)malloc(TAM * sizeof(char));
        strcpy(NomeArquivo, "Glider.csv");
        break;
    case 5:
        NomeArquivo = (char *)malloc(TAM * sizeof(char));
        strcpy(NomeArquivo, "LWSS.csv");
        break;
    }
    // printf("Digite coordenadas para insercao do invasor: ");
    // scanf("%d %d", LinhaPadrao, ColunaPadrao);
    // if (LinhaPadrao < 0 || LinhaPadrao > Tabuleiro->numeroLinha || ColunaPadrao < 0 || ColunaPadrao > Tabuleiro->numeroColuna)
    // {
    //     printf("Tamanho invalido, invasor nao sera inserido");
    //     return;
    // }

    Arquivo = fopen(NomeArquivo, "r");
    if (Arquivo == NULL)
    {
        printf("Erro ao abrir arquivo %s\n", NomeArquivo);
        return;
    }
    VerificaTamanhoMatriz(&Linha, &Coluna, NomeArquivo);
    Padrao = alocaMatriz(Linha, Coluna);
    limpaMatriz(Linha, Coluna, Padrao);
    while (!feof(Arquivo))
    {
        fgets(LinhaArquivo, BUFFER, Arquivo);
        Caracter = strtok(LinhaArquivo, ",");
        while (Caracter != NULL)
        {
            Padrao[i][atoi(Caracter) - 1] = ORG;
            Caracter = strtok(NULL, ",");
        }
        i++;
    }
    for (i = 0; i < Linha; i++)
    {
        for (j = 0; j < Coluna; j++)
        {
            Tabuleiro->MatrizAtual[LinhaPadrao + i][ColunaPadrao + j] = Padrao[i][j];
        }
    }
    fclose(Arquivo);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void menuInicJogo(jogo *Tabuleiro)
{
    int opcao;

    printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre com a opcao: ");
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        strcpy(Tabuleiro->NomeJogo, "Bloco");
        break;
    case 2:
        strcpy(Tabuleiro->NomeJogo, "Blinker");
        break;
    case 3:
        strcpy(Tabuleiro->NomeJogo, "Sapo");
        break;
    case 4:
        strcpy(Tabuleiro->NomeJogo, "Glider");
        break;
    case 5:
        strcpy(Tabuleiro->NomeJogo, "LWSS");
        break;
    }
    system(LIMPA);
    LerArquivo(Tabuleiro);
    imprimeMatriz(Tabuleiro);
    printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo...");
    while (getchar() != '\n')
        ;
    getchar();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void jogaJogoVida(jogo *Tabuleiro)
{
    int c, invasor, decisao;
    char **MatrizCopia;
    srand(time(NULL));
    invasor = rand() % (Tabuleiro->numeroCiclos) + 1;
    system(LIMPA);
    imprimeMatriz(Tabuleiro);
    Sleep(300);
    MatrizCopia = alocaMatriz(Tabuleiro->numeroLinha, Tabuleiro->numeroColuna);
    copiaMatriz(Tabuleiro, MatrizCopia);
    for (c = 0; c < Tabuleiro->numeroCiclos; c++)
    {
        if (invasor == c)
        {
            printf("Padrao invasor pronto para ser inserido, Deseja inseri-lo?\n");
            printf("(1)Sim (0)Nao\n");
            scanf("%d", &decisao);
            if (decisao == 1)
            {
                Invasor(Tabuleiro);
            }
        }
        atualizaMat(Tabuleiro, MatrizCopia);
        copiaMatriz(Tabuleiro, MatrizCopia);
        system(LIMPA);
        imprimeMatriz(Tabuleiro);
        Sleep(300);
    }
    desalocaMatriz(Tabuleiro->numeroLinha, MatrizCopia);
}
//////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    jogo Tabuleiro;
    int i = 1;
    while (i == 1)
    {
        printf("Defina quantos ciclos deseja: \n");
        scanf("%d", &Tabuleiro.numeroCiclos);
        printf("Defina o tamanho da matriz\n");
        printf("Defina o tamanho da Linha: ");
        scanf("%d", &Tabuleiro.numeroLinha);
        printf("Defina o tamanho da Coluna: ");
        scanf("%d", &Tabuleiro.numeroColuna);
        Tabuleiro.MatrizAtual = alocaMatriz(Tabuleiro.numeroLinha, Tabuleiro.numeroColuna);
        menuInicJogo(&Tabuleiro);
        jogaJogoVida(&Tabuleiro);
        desalocaMatriz(Tabuleiro.numeroLinha, Tabuleiro.MatrizAtual);
        printf("\nDeseja jogar novamente?\n(1) Sim (0) Nao\n");
        scanf("%d", &i);
    }
}