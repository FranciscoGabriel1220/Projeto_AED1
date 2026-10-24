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

int buscaLinear(const int vetor[], size_t tamanho, int alvo,
                size_t *comparacoes)
{
    if (comparacoes == NULL) {
        return -1;
    }
    *comparacoes = 0;

    if (vetor == NULL || tamanho > (size_t)INT_MAX) {
        return -1;
    }

    for (size_t i = 0; i < tamanho; i++) {
        (*comparacoes)++;
        if (vetor[i] == alvo) {
            return (int)i;
        }
    }

    return -1;
}

int buscaBinaria(const int vetor[], size_t tamanho, int alvo,
                 size_t *comparacoes)
{
    if (comparacoes == NULL) {
        return -1;
    }
    *comparacoes = 0;

    if (vetor == NULL || tamanho > (size_t)INT_MAX
        || !estaOrdenado(vetor, tamanho)) {
        return -1;
    }

    size_t inicio = 0;
    size_t fim = tamanho;

    while (inicio < fim) {
        size_t meio = inicio + (fim - inicio) / 2;
        int valorMeio = vetor[meio];

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
    const int alvos[] = {0, 10, 19, 20};

    if (!estaOrdenado(vetorOrdenado, tamanhoOrdenado)) {
        fprintf(stderr, "O vetor da busca binaria nao esta ordenado.\n");
        return 1;
    }

    printf("\nComparacao de buscas no mesmo vetor ordenado:\n");
    printf("Busca linear: qualquer sequencia, O(n). Busca binaria: "
           "exige vetor ordenado, O(log n).\n");
    printf("Alvo | Linear (posicao, comparacoes) | "
           "Binaria (posicao, comparacoes)\n");
    for (size_t i = 0; i < sizeof alvos / sizeof alvos[0]; i++) {
        size_t comparacoesLineares = 0;
        size_t comparacoesBinarias = 0;
        int posicaoLinear = buscaLinear(vetorOrdenado, tamanhoOrdenado,
                                        alvos[i], &comparacoesLineares);

        printf("  %d: ", alvos[i]);
        int posicaoBinaria = buscaBinaria(vetorOrdenado, tamanhoOrdenado,
                                          alvos[i], &comparacoesBinarias);
        printf("(%d, %zu) | (%d, %zu)\n", posicaoLinear,
               comparacoesLineares, posicaoBinaria, comparacoesBinarias);
    }

    const int vetorDesordenado[] = {8, 3, 12, 1, 6};
    const size_t tamanhoDesordenado =
        sizeof vetorDesordenado / sizeof vetorDesordenado[0];
    size_t comparacoesDesordenadas = 0;
    int posicaoDesordenada = buscaLinear(vetorDesordenado,
                                         tamanhoDesordenado, 1,
                                         &comparacoesDesordenadas);

    printf("\nVetor desordenado: busca linear encontra 1 na posicao %d "
           "com %zu comparacoes.\n",
           posicaoDesordenada, comparacoesDesordenadas);
    printf("Busca binaria nao e executada neste vetor: ele nao satisfaz "
           "a pre-condicao de ordenacao.\n");

    return 0;
}