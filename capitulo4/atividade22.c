#include <stdio.h>
#include <string.h>

#define QUANTIDADE_PARTIDAS 3
#define TAMANHO_JOGADOR 50
#define ARQUIVO_RELATORIO "relatorio_partidas.txt"

typedef struct {
    int identificador;
    char jogador[TAMANHO_JOGADOR];
    int pontuacao;
} Partida;

int gravarPartidas(const char *nomeArquivo, const Partida partidas[],
                   size_t quantidade)
{
    FILE *arquivo = fopen(nomeArquivo, "a");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo para escrita");
        return 0;
    }

    for (size_t i = 0; i < quantidade; i++) {
        if (fprintf(arquivo, "%d|%s|%d\n", partidas[i].identificador,
                    partidas[i].jogador, partidas[i].pontuacao) < 0) {
            perror("Erro ao gravar o relatorio");
            fclose(arquivo);
            return 0;
        }
    }

    if (fclose(arquivo) == EOF) {
        perror("Erro ao fechar o arquivo de escrita");
        return 0;
    }

    return 1;
}

int lerPartidas(const char *nomeArquivo)
{
    FILE *arquivo = fopen(nomeArquivo, "r");
    if (arquivo == NULL) {
        perror("Nao foi possivel abrir o arquivo para leitura");
        return 0;
    }

    Partida partida;
    char linha[256];
    size_t quantidadeLida = 0;
    int leituraValida = 1;

    printf("Historico de partidas:\n");
    while (fgets(linha, sizeof linha, arquivo) != NULL) {
        int consumidos = -1;

        if (strchr(linha, '\n') == NULL && !feof(arquivo)) {
            fprintf(stderr, "Erro: registro muito longo no relatorio.\n");
            leituraValida = 0;
            break;
        }

        if (sscanf(linha, "%d|%49[^|]|%d %n", &partida.identificador,
                   partida.jogador, &partida.pontuacao, &consumidos) != 3
            || consumidos < 0 || linha[consumidos] != '\0') {
            fprintf(stderr, "Erro: registro malformado no relatorio: %s",
                    linha);
            leituraValida = 0;
            break;
        }

        printf("ID: %d | Jogador: %s | Pontuacao: %d\n",
               partida.identificador, partida.jogador, partida.pontuacao);
        quantidadeLida++;
    }

    if (ferror(arquivo)) {
        perror("Erro ao ler o relatorio");
        leituraValida = 0;
    }

    if (fclose(arquivo) == EOF) {
        perror("Erro ao fechar o arquivo de leitura");
        return 0;
    }

    if (!leituraValida) {
        return 0;
    }

    if (quantidadeLida == 0) {
        printf("O historico esta vazio.\n");
    } else {
        printf("Total de partidas lidas: %zu\n", quantidadeLida);
    }

    return 1;
}

int main(void)
{
    const Partida partidas[QUANTIDADE_PARTIDAS] = {
        {1, "Ana Souza", 120},
        {2, "Bruno Lima", 95},
        {3, "Carla Mendes", 150}
    };

    if (!gravarPartidas(ARQUIVO_RELATORIO, partidas, QUANTIDADE_PARTIDAS)) {
        return 1;
    }

    if (!lerPartidas(ARQUIVO_RELATORIO)) {
        return 1;
    }

    return 0;
}