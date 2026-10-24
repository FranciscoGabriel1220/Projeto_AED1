#include <stddef.h>
#include <stdio.h>

typedef struct {
    size_t acessos;
    size_t comparacoes;
} Contadores;

int acessarPosicao(const int vetor[], size_t tamanho, size_t indice,
                   int *valor, Contadores *contadores)
{
    if (vetor == NULL || valor == NULL || contadores == NULL
        || indice >= tamanho) {
        return 0;
    }

    *valor = vetor[indice];
    contadores->acessos++;
    return 1;
}

int encontrarMaior(const int vetor[], size_t tamanho, int *maior,
                   Contadores *contadores)
{
    if (vetor == NULL || tamanho == 0 || maior == NULL || contadores == NULL) {
        return 0;
    }

    *maior = vetor[0];
    contadores->acessos++;

    for (size_t i = 1; i < tamanho; i++) {
        int valorAtual = vetor[i];
        contadores->acessos++;
        contadores->comparacoes++;

        if (valorAtual > *maior) {
            *maior = valorAtual;
        }
    }

    return 1;
}

int encontrarParesSoma(const int vetor[], size_t tamanho, int alvo,
                       size_t *comparacoes)
{
    if ((vetor == NULL && tamanho > 0) || comparacoes == NULL) {
        return 0;
    }

    *comparacoes = 0;
    printf("  Pares cuja soma e %d:", alvo);
    int encontrouPar = 0;

    for (size_t i = 0; i < tamanho; i++) {
        for (size_t j = i + 1; j < tamanho; j++) {
            long long soma = (long long)vetor[i] + vetor[j];
            (*comparacoes)++;
            if (soma == alvo) {
                printf(" (%d, %d)", vetor[i], vetor[j]);
                encontrouPar = 1;
            }
        }
    }

    if (!encontrouPar) {
        printf(" nenhum");
    }
    printf("\n");
    return 1;
}

int main(void)
{
    const int vetor[] = {
        3, 9, 1, 7, 2, 11, 4, 6, 8, 5,
        13, 0, 12, 10, 17, 14, 15, 16, 18, 19
    };
    const size_t tamanhos[] = {5, 10, 20};
    const size_t quantidadeTamanhos = sizeof tamanhos / sizeof tamanhos[0];

    printf("Comparacao de operacoes para diferentes tamanhos:\n");
    printf("Contadores medem acessos ao vetor e comparacoes entre valores.\n\n");

    for (size_t i = 0; i < quantidadeTamanhos; i++) {
        size_t tamanho = tamanhos[i];
        size_t indice = tamanho / 2;
        int valorAcessado;
        int maior;
        size_t comparacoesPares;
        Contadores contadoresAcesso = {0, 0};
        Contadores contadoresBusca = {0, 0};

        printf("n = %zu\n", tamanho);
        if (!acessarPosicao(vetor, tamanho, indice, &valorAcessado,
                            &contadoresAcesso)
            || !encontrarMaior(vetor, tamanho, &maior, &contadoresBusca)) {
            fprintf(stderr, "Erro ao processar vetor de tamanho %zu.\n",
                    tamanho);
            return 1;
        }
        if (!encontrarParesSoma(vetor, tamanho, 19, &comparacoesPares)) {
            fprintf(stderr, "Erro ao buscar pares no vetor de tamanho %zu.\n",
                    tamanho);
            return 1;
        }

        printf("  Acesso ao indice %zu: valor = %d, %zu acesso(s), "
               "%zu comparacao(oes) [O(1)]\n",
               indice, valorAcessado, contadoresAcesso.acessos,
               contadoresAcesso.comparacoes);
        printf("  Busca do maior: valor = %d, %zu acesso(s), "
               "%zu comparacao(oes) [O(n)]\n",
               maior, contadoresBusca.acessos, contadoresBusca.comparacoes);
        printf("  Busca de pares: %zu comparacao(oes) [O(n^2)]\n\n",
               comparacoesPares);
    }

    Contadores contadoresInvalidos = {0, 0};
    int valor;
    if (!acessarPosicao(vetor, tamanhos[2], tamanhos[2], &valor,
                        &contadoresInvalidos)) {
        printf("Indice igual ao tamanho do vetor rejeitado corretamente.\n");
    } else {
        fprintf(stderr, "A validacao aceitou um indice invalido.\n");
        return 1;
    }

    return 0;
}