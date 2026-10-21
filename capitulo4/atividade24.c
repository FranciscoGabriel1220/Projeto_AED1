#include <stdio.h>
#include <string.h>

#define TAMANHO_JOGADOR 50
#define ARQUIVO_PARTIDAS "partidas.bin"
#define QUANTIDADE_PARTIDAS 3

typedef struct {
    int identificador;
    char jogador[TAMANHO_JOGADOR];
    int pontuacao;
} Partida;

enum ResultadoLeitura {
    ERRO_LEITURA = -1,
    REGISTRO_INCOMPLETO = -2,
    FIM_ARQUIVO = 0,
    REGISTRO_LIDO = 1
};

int gravarPartidasBinarias(const char *nomeArquivo,
                           const Partida partidas[], size_t quantidade)
{
    FILE *arquivo = fopen(nomeArquivo, "wb");
    if (arquivo == NULL) {
        perror("Falha ao abrir o arquivo binario para escrita");
        return 0;
    }

    int escritaValida = 1;
    for (size_t i = 0; i < quantidade; i++) {
        if (fwrite(&partidas[i].identificador,
                   sizeof partidas[i].identificador, 1, arquivo) != 1
            || fwrite(partidas[i].jogador,
                      sizeof partidas[i].jogador, 1, arquivo) != 1
            || fwrite(&partidas[i].pontuacao,
                      sizeof partidas[i].pontuacao, 1, arquivo) != 1) {
            perror("Erro ao gravar partida no arquivo binario");
            escritaValida = 0;
            break;
        }
    }

    if (fclose(arquivo) == EOF) {
        perror("Falha ao fechar o arquivo binario de escrita");
        escritaValida = 0;
    }

    return escritaValida;
}

int lerPartidaBinaria(FILE *arquivo, Partida *partida)
{
    size_t lidos = fread(&partida->identificador, 1,
                         sizeof partida->identificador, arquivo);
    if (lidos == 0) {
        if (ferror(arquivo)) {
            return ERRO_LEITURA;
        }
        return feof(arquivo) ? FIM_ARQUIVO : ERRO_LEITURA;
    }
    if (lidos != sizeof partida->identificador) {
        return ferror(arquivo) ? ERRO_LEITURA : REGISTRO_INCOMPLETO;
    }

    lidos = fread(partida->jogador, 1, sizeof partida->jogador, arquivo);
    if (lidos != sizeof partida->jogador) {
        return ferror(arquivo) ? ERRO_LEITURA : REGISTRO_INCOMPLETO;
    }

    lidos = fread(&partida->pontuacao, 1, sizeof partida->pontuacao, arquivo);
    if (lidos != sizeof partida->pontuacao) {
        return ferror(arquivo) ? ERRO_LEITURA : REGISTRO_INCOMPLETO;
    }

    if (memchr(partida->jogador, '\0', sizeof partida->jogador) == NULL) {
        return REGISTRO_INCOMPLETO;
    }

    return REGISTRO_LIDO;
}

int lerECompararPartidas(const char *nomeArquivo,
                         const Partida originais[], size_t quantidade)
{
    FILE *arquivo = fopen(nomeArquivo, "rb");
    if (arquivo == NULL) {
        perror("Falha ao abrir o arquivo binario para leitura");
        return 0;
    }

    Partida restaurada;
    size_t quantidadeLida = 0;
    int leituraValida = 1;

    printf("Partidas restauradas do arquivo binario:\n");
    while (1) {
        int resultado = lerPartidaBinaria(arquivo, &restaurada);
        if (resultado == FIM_ARQUIVO) {
            break;
        }
        if (resultado == REGISTRO_INCOMPLETO) {
            fprintf(stderr, "Erro: arquivo vazio ou ultimo registro incompleto.\n");
            leituraValida = 0;
            break;
        }
        if (resultado == ERRO_LEITURA) {
            perror("Erro ao ler o arquivo binario");
            leituraValida = 0;
            break;
        }

        if (quantidadeLida >= quantidade) {
            fprintf(stderr, "Erro: arquivo contem partidas inesperadas.\n");
            leituraValida = 0;
            break;
        }

        printf("ID: %d | Jogador: %s | Pontuacao: %d\n",
               restaurada.identificador, restaurada.jogador,
               restaurada.pontuacao);

        if (restaurada.identificador != originais[quantidadeLida].identificador
            || strcmp(restaurada.jogador,
                      originais[quantidadeLida].jogador) != 0
            || restaurada.pontuacao != originais[quantidadeLida].pontuacao) {
            fprintf(stderr, "Erro: a partida restaurada difere da original.\n");
            leituraValida = 0;
            break;
        }

        quantidadeLida++;
    }

    if (fclose(arquivo) == EOF) {
        perror("Falha ao fechar o arquivo binario de leitura");
        leituraValida = 0;
    }

    if (!leituraValida) {
        return 0;
    }
    if (quantidadeLida == 0) {
        fprintf(stderr, "Erro: o arquivo binario esta vazio.\n");
        return 0;
    }
    if (quantidadeLida != quantidade) {
        fprintf(stderr, "Erro: quantidade de registros restaurados incorreta.\n");
        return 0;
    }

    printf("Comparacao concluida: %zu partidas restauradas corretamente.\n",
           quantidadeLida);
    return 1;
}

int main(void)
{
    const Partida partidas[QUANTIDADE_PARTIDAS] = {
        {1, "Ana Souza", 120},
        {2, "Bruno Lima", 95},
        {3, "Carla Mendes", 150}
    };

    if (!gravarPartidasBinarias(ARQUIVO_PARTIDAS, partidas,
                                QUANTIDADE_PARTIDAS)) {
        return 1;
    }

    if (!lerECompararPartidas(ARQUIVO_PARTIDAS, partidas,
                              QUANTIDADE_PARTIDAS)) {
        return 1;
    }

    return 0;
}
