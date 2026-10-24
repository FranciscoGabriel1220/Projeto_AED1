#include <limits.h>
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

int estaOrdenado(const int vetor[], size_t tamanho)
{
    if (vetor == NULL && tamanho > 0) {
        return 0;
    }

    for (size_t i = 1; i < tamanho; i++) {
        if (vetor[i - 1] > vetor[i]) {
            return 0;
        }
    }
    return 1;
}

int buscaBinaria(const int vetor[], size_t tamanho, int alvo,
                 size_t *comparacoes)
{
    if (vetor == NULL || comparacoes == NULL || tamanho > (size_t)INT_MAX) {
        return -1;
    }

    *comparacoes = 0;
    size_t inicio = 0;
    size_t fim = tamanho;

    while (inicio < fim) {
        size_t meio = inicio + (fim - inicio) / 2;
        int valorMeio = vetor[meio];

        printf("    inicio = %zu, meio = %zu, fim = %zu "
               "(intervalo [inicio, fim))\n",
               inicio, meio, fim);

        (*comparacoes)++;
        if (valorMeio == alvo) {
            return (int)meio;
        }

        (*comparacoes)++;
        if (alvo < valorMeio) {
            fim = meio;
        } else {
            inicio = meio + 1;
        }
    }

    return -1;
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

    const int vetorOrdenado[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    };
    const size_t tamanhoOrdenado =
        sizeof vetorOrdenado / sizeof vetorOrdenado[0];
    const int alvos[] = {11, 20};

    if (!estaOrdenado(vetorOrdenado, tamanhoOrdenado)) {
        fprintf(stderr, "O vetor da busca binaria nao esta ordenado.\n");
        return 1;
    }

    printf("\nBusca binaria iterativa: vetor verificado como ordenado; "
           "complexidade O(log n).\n");
    printf("Os limites impressos representam o intervalo [inicio, fim).\n");
    for (size_t i = 0; i < sizeof alvos / sizeof alvos[0]; i++) {
        size_t comparacoes = 0;
        printf("  Procurando %d:\n", alvos[i]);
        int posicao = buscaBinaria(vetorOrdenado, tamanhoOrdenado, alvos[i],
                                   &comparacoes);
        printf("    Resultado: posicao = %d, %zu comparacao(oes)\n",
               posicao, comparacoes);
    }

    return 0;
}