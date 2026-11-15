#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <ctype.h>
#include <windows.h>

#define tamanhoMat 10
#define limite 20

// matriz do nosso jogo
char mat[tamanhoMat][tamanhoMat];

// jogador
typedef struct jogador
{
    char nome[4];
    int pontuacao;
} Jogador;

typedef struct jogo
{
    Jogador *jogadores;
    int quantidade;
    int rodadas;
    char **matriz;
    int tamanho;
} Jogo;

bool encontrar_palavra(const char *arquivo, char *palavra);
void adicionar_palavra(const char *arquivo, char *palavra);
void remover_palavra(const char *arquivo, char *palavra);
void matriz();
void iniciar(Jogo *jogo);
Jogador criarJogador();
int pesquisaHorizontalLeste(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaHorizontalOeste(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaVerticalSul(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaVerticalNorte(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaDiagonalSudeste(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaDiagonalSudoeste(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaDiagonalNordeste(Jogador jogador[], int w, char x[], int y, int z);
int pesquisaDiagonalNoroeste(Jogador jogador[], int w, char x[], int y, int z);

int main()
{
    srand(time(NULL));
    setlocale(LC_ALL, "utf-8");
    int opcao;
    char *nome_arquivo = "palavras";
    do
    {
        printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*");
        printf("0 - Sair\n 1 - Adicionar Palavra\n 2 - Remover Palavra\n 3 - Iniciar Jogo\n");
        scanf("%d", &opcao);
        printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*");
        switch (opcao)
        {
        case 0:
            printf("Saindo . . .\n");
            break;
        case 1:
            char *palavra;
            char buffer[128];
            int tamanho = 0;
            printf("Digite a palavra que deseja adicionar: ");
            while (scanf("%127s", buffer) == 1)
            {
                int buffer_tamanho = strlen(buffer);
                char *nova_palavra = realloc(palavra, tamanho + buffer_tamanho + 1);
                if (nova_palavra == NULL)
                {
                    printf("Erro ao alocar memória!\n");
                    free(palavra);
                    return 1;
                }
                palavra = nova_palavra;
                strcpy(palavra + tamanho, buffer);
                tamanho += buffer_tamanho;
                if (getchar() == '\n')
                    break;
            }
            palavra = toupper(palavra);

            printf("Palavra digitada: %s\n", palavra);

            if (encontrar_palavra(nome_arquivo, palavra) != true)
                adicionar_palavra(nome_arquivo, palavra);
            else
                printf("Palavra '%s' já existe no arquivo.\n", palavra);

            free(palavra);
            break;
        case 2:
            char *palavra;
            char buffer[128];
            int tamanho = 0;
            printf("Digite a palavra que deseja remover: ");
            while (scanf("%127s", buffer) == 1)
            {
                int buffer_tamanho = strlen(buffer);
                char *nova_palavra = realloc(palavra, tamanho + buffer_tamanho + 1);
                if (nova_palavra == NULL)
                {
                    printf("Erro ao alocar memória!\n");
                    free(palavra);
                    return 1;
                }
                palavra = nova_palavra;
                strcpy(palavra + tamanho, buffer);
                tamanho += buffer_tamanho;
                if (getchar() == '\n')
                    break;
            }

            palavra = toupper(palavra);

            printf("Palavra digitada: %s\n", palavra);

            if (encontrar_palavra(nome_arquivo, palavra) == true)
                remover_palavra(nome_arquivo, palavra);
            else
                printf("Palavra '%s' não existe no arquivo.\n", palavra);

            free(palavra);
            break;
        case 3:
            Jogo jogo;
            iniciar(&jogo);

            break;
        default:

            break;
        }
        printf("*=*=*=*=*=*=*=*=*=*=*=*=*=*=*");
    } while (opcao != 0);

    Jogador jogadores[limite];
    int total, tamanho, maior = -999999;
    char valor[tamanhoMat], parametro;
    matriz();

    do
    {
        int resultado = 0;
        for (int i = 0; i < total; i++)
        {
            printf("%s, qual a sua palavra:", jogadores[i].nome);
            fgets(valor, tamanhoMat + 1, stdin);
            tamanho = strlen(valor) - 1;
            printf("\n");
            if (i == 0)
            {
                if (strncmp(valor, ".", 1) == 0)
                {
                    parametro = '.';
                    i = total;
                }
                // As palavras digitadas pelos jogadores devem conter no mínimo dois caracteres.
                else if (tamanho > 1)
                {
                    resultado = pesquisaHorizontalLeste(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaHorizontalOeste(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaVerticalSul(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaVerticalNorte(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaDiagonalSudeste(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaDiagonalSudoeste(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaDiagonalNordeste(jogadores, resultado, valor, i, tamanho);
                    resultado = pesquisaDiagonalNoroeste(jogadores, resultado, valor, i, tamanho);
                    if (resultado >= 8)
                    {
                        printf("Palavra não encontrada!\n");
                        jogadores[i].pontuacao--;
                        resultado = 0;
                    }
                }
                else
                {
                    printf("Palavra inválida!\n");
                }
            }
            // As palavras digitadas pelos jogadores devem conter no mínimo dois caracteres.
            else if (tamanho > 1)
            {
                resultado = pesquisaHorizontalLeste(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaHorizontalOeste(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaVerticalSul(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaVerticalNorte(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaDiagonalSudeste(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaDiagonalSudoeste(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaDiagonalNordeste(jogadores, resultado, valor, i, tamanho);
                resultado = pesquisaDiagonalNoroeste(jogadores, resultado, valor, i, tamanho);
                if (resultado >= 8)
                {
                    printf("Palavra não encontrada!\n");
                    jogadores[i].pontuacao--;
                    resultado = 0;
                }
            }
            else
            {
                printf("Palavra inválida!\n");
            }
        }

    } while (parametro != '.');
    for (int i = 0; i < total; i++)
    {
        if (jogadores[i].pontuacao > maior)
        {
            maior = jogadores[i].pontuacao;
        }
    }
    printf("Vencedor(es):");
    for (int i = 0; i < total; i++)
    {
        if (jogadores[i].pontuacao == maior)
        {
            printf("%s ", jogadores[i].nome);
        }
    }
    return 0;
}

bool encontrar_palavra(const char *arquivo, char *palavra)
{
    FILE *arq = fopen(arquivo, "r");
    if (!arq)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char comparador[strlen(palavra)];
    bool encotrado = false;

    rewind(arq);
    while (fscanf(arq, "%s", comparador) == 1)
    {
        if (strcmp(comparador, palavra) == 0)
        {
            encotrado = true;
            break;
        }
    }

    fclose(arq);
    return encotrado;
}

void adicionar_palavra(const char *arquivo, char *palavra)
{
    FILE *arq = fopen(arquivo, "a");
    if (!arq)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }
    printf(arq, "%s\n", palavra);
    printf("Palavra '%s' adicionada ao arquivo.\n", palavra);
    fclose(arq);
}

void remover_palavra(const char *arquivo, char *palavra)
{
    FILE *arq = fopen(arquivo, "r");
    FILE *temp = fopen("temporario.txt", "w");
    if (!arq || !temp)
    {
        printf("Erro ao abrir o arquivo.\n");
        return;
    }

    char comparador[strlen(palavra)];

    rewind(arq);
    while (fscanf(arq, "%s", comparador) == 1)
    {
        if (strcmp(comparador, palavra) != 0)
        {
            fprintf(temp, "%s\n", palavra);
        }
    }
    fclose(arq);
    fclose(temp);
    remove(arquivo);
    rename(arquivo, temp);

    printf("Palavra '%s' removida do arquivo.\n", palavra);
}

int quantidade_palavras(const char *arquivo)
{
    FILE *arq = fopen(arquivo, "r");
    if (arq == NULL)
    {
        printf("Erro ao abrir o arquivo.\n");
        return -1;
    }

    int quantidade = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), arq) != NULL)
        quantidade++;

    fclose(arq);
    return quantidade;
}

void iniciar(Jogo *jogo)
{
    int total;
    do
    {
        printf("Digite a quantidade de jogadores: ");
        scanf("%d", &total);
    } while (total < 2);
    jogo->jogadores = (Jogador *)malloc(total * sizeof(Jogador));
    jogo->quantidade = total;
    for (int i = 1; i <= total; i++)
    {
        printf("Qual o nome do %d° jogador? ", i + 1);
        fgets(jogo->jogadores[i].nome, strlen(jogo->jogadores[i].nome), stdin);
        jogo->jogadores[i].nome[strcspn(jogo->jogadores[i].nome, "\n")] = '\0';
        strupr(jogo->jogadores[i].nome);
        getchar();
    }

    int rodadas;
    do
    {
        printf("Quantas rodadas (o mínimo é 1)? ");
        scanf("%d", &rodadas);
    } while (rodadas <= 0);
    jogo->rodadas = rodadas;

    int tamanho;
    do
    {
        printf("Qual o tamanho do caça-palavras (tamanho mínimo é 20)? ");
        scanf("%d", &tamanho);
    } while (tamanho < 20);

    jogo->tamanho = tamanho;
    jogo->matriz = (char *)malloc(tamanho * sizeof(char));

    for (int i = 0; i < tamanho; i++)
        jogo->matriz[i] = (char *)malloc(tamanho * sizeof(char));
}

void gerar_caca_palavras(Jogo *jogo, const char *arquivo)
{
    int matriz_auxiliar[jogo->tamanho][jogo->tamanho];
    int quantidade_palavras_arquivo = quantidade_palavras(arquivo);
    int verificador_palavras[quantidade_palavras_arquivo];

    for (int i = 0; i < quantidade_palavras_arquivo; i++)
        verificador_palavras[i] = 0;

    for (int i = 0; i < jogo->tamanho; i++)
        for (int j = 0; j < jogo->tamanho; j++)
            matriz_auxiliar[i][j] = 0;

    for (int i = 0; i < jogo->tamanho; i++)
    {
        for (int j = 0; j < jogo->tamanho; j++)
        {
            int aleatoria = rand() % 5;
            if (aleatoria == 4)
            {
                if (matriz_auxiliar[i][j] == 0)
                {
                    int indice_palavra = rand() % quantidade_palavras_arquivo;
                    if(verificador_palavras[indice_palavra] != 0){

                    }

                    int direcao = rand() % 6;
                    int valor = direcao;
                    do
                    {
                        if (direcao == 0)
                        {

                            if (palavra_leste() == true)
                            {
                            }
                        }
                        else if ()
                        {
                        }
                        else if ()
                        {
                        }
                        direcao = direcao + 1 % 6;
                    } while (direcao != valor);
                }
            }
            else
            {
                jogo->matriz[i][j] = letra_aleatoria();
                matriz_auxiliar[i][j] = 2;
            }
        }
    }
}

char letra_aleatoria()
{
    return "A" + (rand() % 26);
}

void matriz()
{
    int x, y;
    for (x = 0; x < tamanhoMat; x++)
    {
        for (y = 0; y < tamanhoMat; y++)
        {
            mat[x][y] = rand() % (122 - 97 + 1) + 97;
        }
    }
    printf("\nCaça palavras:\n\n");
    for (x = 0; x < tamanhoMat; x++)
    {
        for (y = 0; y < tamanhoMat; y++)
        {
            printf("%c ", mat[x][y]);
        }
        printf("\n");
    }
    printf("\n");
}

Jogador criarJogador()
{
    Jogador jogador;
    fgets(jogador.nome, 4, stdin);
    jogador.nome[strcspn(jogador.nome, "\n")] = '\0';
    jogador.pontuacao = 0;
    return jogador;
}

int pesquisaHorizontalLeste(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int cont = 0;
            while (mat[i][j + cont] == x[cont] && mat[j][j + cont] != '\0' && cont != z && (j + cont) != 10)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Horizontal (Leste --> Oeste) na linha %d coluna %d\n", x, i, j);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaHorizontalOeste(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = tamanhoMat - 1; j > -1; j--)
        {
            int cont = 0;
            while (mat[i][j - cont] == x[cont] && cont != z && j != 0)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Horizontal (Oeste --> Leste) na linha %d coluna %d\n", x, i, j);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaVerticalSul(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int cont = 0;
            while (mat[j + cont][i] == x[cont] && mat[j + cont][i] != '\0' && cont != z && j != 10)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Vertical (Norte --> Sul) na linha %d coluna %d\n", x, j, i);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaVerticalNorte(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = tamanhoMat - 1; j > -1; j--)
        {
            int cont = 0;
            while (mat[j - cont][i] == x[cont] && cont != z && j != 10)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Vertical (Sul --> Norte) na linha %d coluna %d\n", x, j, i);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaDiagonalSudeste(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = 0; j < 10; j++)
        {
            int cont = 0;
            while (mat[i + cont][j + cont] == x[cont] && mat[i + cont][j + cont] != '\0' && cont != z && j != 10)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Diagonal (Noroeste --> Sudeste) na linha %d coluna %d\n", x, i, j);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaDiagonalSudoeste(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = tamanhoMat - 1; j > -1; j--)
        {
            int cont = 0;
            while (mat[i + cont][j - cont] == x[cont] && mat[j - cont][i + cont] != '\0' && cont != z && j != 0)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Diagonal (Nordeste --> Sudoeste) na linha %d coluna %d\n", x, i, j);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaDiagonalNordeste(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = 0; i < 10; i++)
    {
        for (int j = tamanhoMat - 1; j > -1; j--)
        {
            int cont = 0;
            while (mat[j - cont][i + cont] == x[cont] && mat[j - cont][i + cont] != '\0' && cont != z && j != 10)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Diagonal (Sudoeste --> Nordeste) na linha %d coluna %d\n", x, j, i);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}

// Usem funções para verificar se a palavra existe no puzzle (uma função pra cada sentido).
int pesquisaDiagonalNoroeste(Jogador jogador[], int w, char x[], int y, int z)
{
    int naoencontrado = 0;
    for (int i = tamanhoMat - 1; i > -1; i--)
    {
        for (int j = tamanhoMat - 1; j > -1; j--)
        {
            int cont = 0;
            while (mat[i - cont][j - cont] == x[cont] && mat[j - cont][i - cont] != '\0' && cont != z && j != 0)
            {
                cont++;
                if (cont == (z))
                {
                    printf("Palavra %s encontrada na Diagonal (Sudeste --> Noroeste) na linha %d coluna %d\n", x, i, j);
                    naoencontrado++;
                    jogador[y].pontuacao++;
                    w = 0;
                }
            }
        }
    }
    if (naoencontrado == 0)
    {
        w++;
    }
    return w;
}
