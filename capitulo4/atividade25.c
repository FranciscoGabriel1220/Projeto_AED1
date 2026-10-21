#include <ctype.h>
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_JOGADOR 50
#define TAMANHO_ENTRADA 128
#define ARQUIVO_PARTIDAS "temporada.bin"
#define MAX_PARTIDAS 10000

typedef struct {
    int identificador;
    char jogador[TAMANHO_JOGADOR];
    int pontuacao;
} Partida;

int lerLinha(const char *mensagem, char *destino, size_t tamanho)
{
    printf("%s", mensagem);
    if (fgets(destino, (int)tamanho, stdin) == NULL) {
        return 0;
    }

    size_t comprimento = strcspn(destino, "\n");
    if (destino[comprimento] == '\n') {
        destino[comprimento] = '\0';
        return 1;
    }

    int caractere;
    while ((caractere = getchar()) != '\n' && caractere != EOF) {
    }
    fprintf(stderr, "Entrada muito longa. Tente novamente.\n");
    return -1;
}

int lerInteiro(const char *mensagem, int *valor)
{
    char entrada[TAMANHO_ENTRADA];
    int resultado = lerLinha(mensagem, entrada, sizeof entrada);
    if (resultado <= 0) {
        return resultado;
    }

    errno = 0;
    char *fim;
    long convertido = strtol(entrada, &fim, 10);
    while (isspace((unsigned char)*fim)) {
        fim++;
    }
    if (entrada == fim || *fim != '\0' || errno == ERANGE
        || convertido < INT32_MIN || convertido > INT32_MAX) {
        fprintf(stderr, "Valor invalido. Digite um numero inteiro.\n");
        return -1;
    }

    *valor = (int)convertido;
    return 1;
}

void listarPartidas(const Partida partidas[], size_t quantidade)
{
    if (quantidade == 0) {
        printf("Nenhuma partida cadastrada.\n");
        return;
    }

    printf("\nPartidas da temporada:\n");
    for (size_t i = 0; i < quantidade; i++) {
        printf("ID: %d | Jogador: %s | Pontuacao: %d\n",
               partidas[i].identificador, partidas[i].jogador,
               partidas[i].pontuacao);
    }
    printf("Total: %zu partida(s).\n", quantidade);
}

int cadastrarPartida(Partida **partidas, size_t *quantidade)
{
    if (*quantidade >= MAX_PARTIDAS) {
        fprintf(stderr, "Limite de %d partidas atingido.\n", MAX_PARTIDAS);
        return 0;
    }

    Partida nova = {0};
    int resultado;

    do {
        resultado = lerInteiro("Identificador: ", &nova.identificador);
        if (resultado == 0) return -1;
    } while (resultado < 0);

    if (nova.identificador <= 0) {
        fprintf(stderr, "O identificador deve ser positivo.\n");
        return 0;
    }

    do {
        resultado = lerLinha("Jogador: ", nova.jogador, sizeof nova.jogador);
        if (resultado == 0) return -1;
    } while (resultado < 0);

    if (nova.jogador[0] == '\0') {
        fprintf(stderr, "O nome do jogador nao pode ficar vazio.\n");
        return 0;
    }

    do {
        resultado = lerInteiro("Pontuacao: ", &nova.pontuacao);
        if (resultado == 0) return -1;
    } while (resultado < 0);

    if (nova.pontuacao < 0) {
        fprintf(stderr, "A pontuacao nao pode ser negativa.\n");
        return 0;
    }

    Partida *novoVetor = realloc(*partidas,
                                  (*quantidade + 1) * sizeof **partidas);
    if (novoVetor == NULL) {
        perror("Falha ao alocar memoria para a partida");
        return 0;
    }

    *partidas = novoVetor;
    (*partidas)[*quantidade] = nova;
    (*quantidade)++;
    printf("Partida cadastrada.\n");
    return 1;
}

int salvarTemporada(const char *nomeArquivo, const Partida partidas[],
                    size_t quantidade)
{
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        perror("Falha ao abrir o arquivo para salvar");
        return 0;
    }

    uint32_t quantidadeGravada = (uint32_t)quantidade;
    int sucesso = 1;
    if (fwrite(&quantidadeGravada, sizeof quantidadeGravada, 1, arquivo) != 1) {
        perror("Erro ao gravar a quantidade de partidas");
        sucesso = 0;
    } else if (quantidade > 0
               && fwrite(partidas, sizeof *partidas, quantidade, arquivo)
                      != quantidade) {
        perror("Erro ao gravar as partidas");
        sucesso = 0;
    }

    if (fclose(arquivo) == EOF) {
        perror("Falha ao fechar o arquivo salvo");
        sucesso = 0;
    }
    if (sucesso) {
        printf("%zu partida(s) salva(s) em %s.\n", quantidade, nomeArquivo);
    }
    return sucesso;
}

int carregarTemporada(const char *nomeArquivo, Partida **partidas,
                      size_t *quantidade)
{
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha ao abrir o arquivo para carregar");
        return 0;
    }

    uint32_t quantidadeLida;
    Partida *temporarias = NULL;
    int sucesso = 1;

    if (fread(&quantidadeLida, sizeof quantidadeLida, 1, arquivo) != 1) {
        if (ferror(arquivo)) {
            perror("Erro ao ler a quantidade de partidas");
        } else {
            fprintf(stderr, "Arquivo vazio ou sem metadados validos.\n");
        }
        sucesso = 0;
    } else if (quantidadeLida > MAX_PARTIDAS) {
        fprintf(stderr, "Quantidade invalida no arquivo: %u.\n",
                quantidadeLida);
        sucesso = 0;
    }

    if (sucesso && quantidadeLida > 0) {
        temporarias = malloc((size_t)quantidadeLida * sizeof *temporarias);
        if (temporarias == NULL) {
            perror("Falha ao alocar memoria para carregar as partidas");
            sucesso = 0;
        } else if (fread(temporarias, sizeof *temporarias, quantidadeLida,
                         arquivo) != quantidadeLida) {
            if (ferror(arquivo)) {
                perror("Erro ao ler as partidas");
            } else {
                fprintf(stderr, "Arquivo incompleto: faltam registros.\n");
            }
            sucesso = 0;
        }
    }

    if (sucesso) {
        for (uint32_t i = 0; i < quantidadeLida; i++) {
            if (memchr(temporarias[i].jogador, '\0',
                       sizeof temporarias[i].jogador) == NULL
                || temporarias[i].identificador <= 0
                || temporarias[i].pontuacao < 0
                || temporarias[i].jogador[0] == '\0') {
                fprintf(stderr, "Registro invalido no arquivo (posicao %u).\n",
                        i + 1);
                sucesso = 0;
                break;
            }
        }
    }

    if (sucesso) {
        int extra = fgetc(arquivo);
        if (extra != EOF) {
            fprintf(stderr, "Arquivo invalido: existem dados apos os registros.\n");
            sucesso = 0;
        } else if (ferror(arquivo)) {
            perror("Erro ao verificar o fim do arquivo");
            sucesso = 0;
        }
    }

    if (fclose(arquivo) == EOF) {
        perror("Falha ao fechar o arquivo carregado");
        sucesso = 0;
    }

    if (!sucesso) {
        free(temporarias);
        return 0;
    }

    free(*partidas);
    *partidas = temporarias;
    *quantidade = quantidadeLida;
    printf("%zu partida(s) carregada(s) de %s.\n",
           *quantidade, nomeArquivo);
    return 1;
}

int main(void)
{
    Partida *partidas = NULL;
    size_t quantidade = 0;
    int continuar = 1;

    while (continuar) {
        int opcao;
        printf("\n=== Gerenciador de temporada ===\n"
               "1. Cadastrar partida\n"
               "2. Listar partidas\n"
               "3. Salvar temporada\n"
               "4. Carregar temporada\n"
               "0. Sair\n");

        int resultado;
        do {
            resultado = lerInteiro("Opcao: ", &opcao);
            if (resultado == 0) {
                continuar = 0;
                break;
            }
        } while (resultado < 0);
        if (!continuar) break;

        switch (opcao) {
        case 1: {
            int cadastrada = cadastrarPartida(&partidas, &quantidade);
            if (cadastrada < 0) continuar = 0;
            break;
        }
        case 2:
            listarPartidas(partidas, quantidade);
            break;
        case 3:
            salvarTemporada(ARQUIVO_PARTIDAS, partidas, quantidade);
            break;
        case 4:
            carregarTemporada(ARQUIVO_PARTIDAS, &partidas, &quantidade);
            break;
        case 0:
            continuar = 0;
            break;
        default:
            fprintf(stderr, "Opcao invalida.\n");
            break;
        }
    }

    free(partidas);
    printf("Programa encerrado.\n");
    return 0;
}
