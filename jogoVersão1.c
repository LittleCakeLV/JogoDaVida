#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <Windows.h>
#define LIMPA "cls"
#else
#include <unistd.h>
#define LIMPA "clear"
#endif

#define ORG 'X'
#define VAZ '.'
//////////////////////////////////////////////////////////////////////////////////////////////
void limpaMatriz(char **m, int nL, int nC)
{
    int i, j;
    for (i = 0; i < nL; i++)
    {
        for (j = 0; j < nC; j++)
        {
            m[i][j] = VAZ;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void inicBlinker(char **m, int nL, int nC)
{
    char padrao[1][3] = {{ORG, ORG, ORG}};
    int i, j, xInic = nL / 2, yInic = nC / 2;
    limpaMatriz(m, nL, nC);
    for (i = 0; i < 1; i++)
        for (j = 0; j < 3; j++)
            m[xInic + i][yInic + j] = padrao[i][j];
}
//////////////////////////////////////////////////////////////////////////////////////////////
void inicBloco(char **m, int nL, int nC)
{
    char padrao[2][2] = {{ORG, ORG}, {ORG, ORG}};
    int i, j, xInic = nL / 2, yInic = nC / 2;
    limpaMatriz(m, nL, nC);
    for (i = 0; i < 2; i++)
        for (j = 0; j < 2; j++)
            m[xInic + i][yInic + j] = padrao[i][j];
}
//////////////////////////////////////////////////////////////////////////////////////////////
void inicSapo(char **m, int nL, int nC)
{
    char padrao[2][4] = {{VAZ, ORG, ORG, ORG}, {ORG, ORG, ORG, VAZ}};
    int i, j, xInic = nL / 2, yInic = nC / 2;
    limpaMatriz(m, nL, nC);
    for (i = 0; i < 2; i++)
        for (j = 0; j < 4; j++)
            m[xInic + i][yInic + j] = padrao[i][j];
}
//////////////////////////////////////////////////////////////////////////////////////////////
void inicGlider(char **m, int nL, int nC)
{
    char padrao[3][3] = {{ORG, ORG, ORG}, {ORG, VAZ, VAZ}, {VAZ, ORG, VAZ}};
    int i, j, xInic, yInic;
    limpaMatriz(m, nL, nC);
    xInic = nL - 4;
    yInic = nC - 4;
    for (i = 0; i < 3; i++)
        for (j = 0; j < 3; j++)
            m[xInic + i][yInic + j] = padrao[i][j];
}
//////////////////////////////////////////////////////////////////////////////////////////////
void inicLWSS(char **m, int nL, int nC)
{
    char padrao[4][5] = {{VAZ, ORG, VAZ, VAZ, ORG}, {ORG, VAZ, VAZ, VAZ, VAZ}, {ORG, VAZ, VAZ, VAZ, ORG}, {ORG, ORG, ORG, ORG, VAZ}};
    int i, j, xInic, yInic;
    limpaMatriz(m, nL, nC);
    xInic = nL - 5;
    yInic = nC - 6;
    for (i = 0; i < 4; i++)
        for (j = 0; j < 5; j++)
            m[xInic + i][yInic + j] = padrao[i][j];
}
//////////////////////////////////////////////////////////////////////////////////////////////
void copiaMatriz(char **mAnt, char **mAtual, int nL, int nC)
{
    int i, j;
    for (i = 0; i < nL; i++)
    {
        for (j = 0; j < nC; j++)
        {
            mAnt[i][j] = mAtual[i][j];
        }
    }
    printf("----------------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////
void atualizaMat(char **mAtual, char **mAnt, int NumeroLinha, int NumeroColuna)
{
    int i, j, soma = 0;
    for (i = 0; i < NumeroLinha; i++)
    {
        for (j = 0; j < NumeroColuna; j++)
        {
            if (i - 1 >= 0 && j - 1 >= 0)      // Verifica se essa posição é valida
                if (mAnt[i - 1][j - 1] == ORG) // Cima-esquerda
                    soma++;

            if (i - 1 >= 0)                // Verifica se essa posição é valida
                if (mAnt[i - 1][j] == ORG) // Cima
                    soma++;

            if (i - 1 >= 0 && j + 1 < NumeroColuna) // Verifica se essa posição é valida
                if (mAnt[i - 1][j + 1] == ORG)      // Cima-Direita
                    soma++;

            if (j - 1 >= 0)                // Verifica se essa posição é valida
                if (mAnt[i][j - 1] == ORG) //  Esquerda
                    soma++;

            if (j + 1 < NumeroColuna)      // Verifica se essa posição é valida
                if (mAnt[i][j + 1] == ORG) // Direita
                    soma++;

            if (i + 1 < NumeroLinha && j - 1 >= 0) // Verifica se essa posição é valida
                if (mAnt[i + 1][j - 1] == ORG)     // Baixo-Esquerda
                    soma++;

            if (i + 1 < NumeroLinha)       // Verifica se essa posição é valida
                if (mAnt[i + 1][j] == ORG) // Baixo
                    soma++;

            if (i + 1 < NumeroLinha && j + 1 < NumeroColuna) // Verifica caso essa posição seja valida
                if (mAnt[i + 1][j + 1] == ORG)               // Baixo-Direita
                    soma++;

            if (mAnt[i][j] == VAZ) // Quando a Célula ta Morta, Verifica se ela vai viver no proximo ciclo
                if (soma == 3)
                {
                    mAtual[i][j] = ORG;
                    soma = 0;
                }

            if (mAnt[i][j] == ORG) // Quando a Célula ta Viva, Verifica se ela vai Morrer no proximo ciclo
                if (soma > 3 || soma < 2)
                {
                    mAtual[i][j] = VAZ;
                    soma = 0;
                }
            soma = 0;
        }
    }
}
//////////////////////////////////////////////////////////////////////////////////////////////
void imprimeMatriz(char **mat, int nL, int nC)
{
    int i, j;
    printf("----------------------------\n");
    for (i = 0; i < nL; i++)
    {
        for (j = 0; j < nC; j++)
        {
            printf("%c ", mat[i][j]);
        }
        printf("\n");
    }
    printf("----------------------------\n");
}
//////////////////////////////////////////////////////////////////////////////////////////////
char **alocaMatriz(int nL, int nC)
{
    char **mat;
    int i = 0;
    mat = (char **)malloc(nL * sizeof(char *));
    if (mat == NULL)
    {
        printf("Memoria insuficiente.\n");
        exit(1);
    }
    else
    {
        for (i = 0; i < nC; i++)
        {
            mat[i] = (char *)malloc(nC * sizeof(char));
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
void desalocaMatriz(char **mat, int nL)
{
    for (int i = 0; i < nL; i++)
    {
        free(mat[i]);
    }
    free(mat);
}
//////////////////////////////////////////////////////////////////////////////////////////////
void menuInicJogo(char **mat, int nL, int nC)
{
    int opcao;

    printf("(1)Bloco\n(2)Blinker\n(3)Sapo\n(4)Glider\n(5)LWSS\nEntre com a opcao: ");
    scanf("%d", &opcao);
    switch (opcao)
    {
    case 1:
        inicBloco(mat, nL, nC);
        break;
    case 2:
        inicBlinker(mat, nL, nC);
        break;
    case 3:
        inicSapo(mat, nL, nC);
        break;
    case 4:
        inicGlider(mat, nL, nC);
        break;
    case 5:
        inicLWSS(mat, nL, nC);
        break;
    }
    imprimeMatriz(mat, nL, nC);
    printf("Se inicializacao correta digite qualquer tecla para iniciar o jogo...");
    while (getchar() != '\n')
        ;
    getchar();
}
//////////////////////////////////////////////////////////////////////////////////////////////
void jogaJogoVida(char **mAtual, int nL, int nC, int nCiclos)
{
    char **mAnt;
    int c;
    system(LIMPA);
    imprimeMatriz(mAtual, nL, nC); // imprimindo na tela a matriz inicial
    Sleep(100);
    mAnt = alocaMatriz(nL, nC); // TO DO
    for (c = 1; c <= nCiclos; c++)
    {
        copiaMatriz(mAnt, mAtual, nL, nC);
        atualizaMat(mAtual, mAnt, nL, nC);
        system(LIMPA);
        imprimeMatriz(mAtual, nL, nC);
        Sleep(100);
    }
    desalocaMatriz(mAnt, nL);
}
//////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    char **mat;
    int i = 0, nL = 10, nC = 10, nCiclos;
    mat = alocaMatriz(nL, nC);
    while (i == 0)
    {
        printf("Defina quantos ciclos deseja: ");
        scanf("%d", &nCiclos);
        menuInicJogo(mat, nL, nC);
        jogaJogoVida(mat, nL, nC, nCiclos);
        printf("\nDeseja jogar novamente?\n(0) Sim (1) Nao\n");
        scanf("%d", &i);
    }
    desalocaMatriz(mat, nL);
}