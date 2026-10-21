#include <stdio.h>
#include <string.h>

#define TAMANHO_JOGADOR 50
#define TAMANHO_LINHA 256
#define ARQUIVO_ORIGEM "relatorio_partidas.txt"
#define ARQUIVO_DESTINO "relatorio_processado.txt"
#define BONUS_PONTUACAO 10

typedef struct {
    int identificador;
    char jogador[TAMANHO_JOGADOR];
    int pontuacao;
} Partida;

int processarPartidas(const char *nomeOrigem, const char *nomeDestino)
{
    FILE *origem = fopen(nomeOrigem, "r");
    if (origem == NULL) {
        perror("Falha ao abrir o arquivo de origem");
        return 0;
    }

    FILE *destino = fopen(nomeDestino, "w");
    if (destino == NULL) {
        perror("Falha ao abrir o arquivo de destino");
        if (fclose(origem) == EOF) {
            perror("Falha ao fechar o arquivo de origem");
        }
        return 0;
    }

    Partida partida;
    char linha[TAMANHO_LINHA];
    size_t quantidadeProcessada = 0;
    int processamentoValido = 1;

    while (fgets(linha, sizeof linha, origem) != NULL) {
        int consumidos = -1;

        if (strchr(linha, '\n') == NULL && !feof(origem)) {
            fprintf(stderr, "Erro: registro muito longo no arquivo de origem.\n");
            processamentoValido = 0;
            break;
        }

        if (sscanf(linha, "%d|%49[^|]|%d %n", &partida.identificador,
                   partida.jogador, &partida.pontuacao, &consumidos) != 3
            || consumidos < 0 || linha[consumidos] != '\0') {
            fprintf(stderr, "Erro: registro malformado no arquivo de origem: %s",
                    linha);
            processamentoValido = 0;
            break;
        }

        partida.pontuacao += BONUS_PONTUACAO;
        if (fprintf(destino, "%d|%s|%d\n", partida.identificador,
                    partida.jogador, partida.pontuacao) < 0) {
            perror("Erro ao escrever no arquivo de destino");
            processamentoValido = 0;
            break;
        }

        quantidadeProcessada++;
    }

    if (ferror(origem)) {
        perror("Erro ao ler o arquivo de origem");
        processamentoValido = 0;
    }

    if (fclose(origem) == EOF) {
        perror("Falha ao fechar o arquivo de origem");
        processamentoValido = 0;
    }

    if (fclose(destino) == EOF) {
        perror("Falha ao fechar o arquivo de destino");
        processamentoValido = 0;
    }

    if (!processamentoValido) {
        return 0;
    }

    printf("Registros processados: %zu\n", quantidadeProcessada);
    return 1;
}

int main(void)
{
    if (!processarPartidas(ARQUIVO_ORIGEM, ARQUIVO_DESTINO)) {
        return 1;
    }

    return 0;
}
