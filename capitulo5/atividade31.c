#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

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
                 size_t *comparacoes, size_t *iteracoes)
{
    if (comparacoes == NULL || iteracoes == NULL) {
        return -1;
    }
    *comparacoes = 0;
    *iteracoes = 0;

    if (vetor == NULL || tamanho > (size_t)INT_MAX
        || !estaOrdenado(vetor, tamanho)) {
        return -1;
    }

    size_t inicio = 0;
    size_t fim = tamanho;

    while (inicio < fim) {
        (*iteracoes)++;
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

unsigned long long fatorialRecursivo(unsigned int n, size_t *chamadas)
{
    (*chamadas)++;
    if (n <= 1) {
        return 1;
    }
    return n * fatorialRecursivo(n - 1, chamadas);
}

unsigned long long fatorialIterativo(unsigned int n, size_t *iteracoes)
{
    unsigned long long resultado = 1;
    *iteracoes = 0;
    for (unsigned int i = 2; i <= n; i++) {
        resultado *= i;
        (*iteracoes)++;
    }
    return resultado;
}

long long somarVetorRecursivo(const int vetor[], size_t tamanho,
                              size_t *chamadas)
{
    (*chamadas)++;
    if (tamanho == 0) {
        return 0;
    }
    return vetor[0] + somarVetorRecursivo(vetor + 1, tamanho - 1,
                                          chamadas);
}

long long somarVetorIterativo(const int vetor[], size_t tamanho,
                              size_t *iteracoes)
{
    long long soma = 0;
    *iteracoes = 0;
    for (size_t i = 0; i < tamanho; i++) {
        soma += vetor[i];
        (*iteracoes)++;
    }
    return soma;
}

unsigned long long fibonacciRecursivo(unsigned int n, size_t *chamadas)
{
    (*chamadas)++;
    if (n <= 1) {
        return n;
    }
    return fibonacciRecursivo(n - 1, chamadas)
         + fibonacciRecursivo(n - 2, chamadas);
}

unsigned long long fibonacciIterativo(unsigned int n)
{
    unsigned long long anterior = 0;
    unsigned long long atual = 1;
    for (unsigned int i = 0; i < n; i++) {
        unsigned long long proximo = anterior + atual;
        anterior = atual;
        atual = proximo;
    }
    return anterior;
}

int buscaBinariaRecursiva(const int vetor[], size_t inicio, size_t fim,
                          int alvo, size_t *chamadas)
{
    (*chamadas)++;
    if (inicio >= fim) {
        return -1;
    }

    size_t meio = inicio + (fim - inicio) / 2;
    if (vetor[meio] == alvo) {
        return (int)meio;
    }
    if (alvo < vetor[meio]) {
        return buscaBinariaRecursiva(vetor, inicio, meio, alvo, chamadas);
    }
    return buscaBinariaRecursiva(vetor, meio + 1, fim, alvo, chamadas);
}

int lerInteiro(const char *mensagem, int *valor)
{
    char entrada[64];
    printf("%s", mensagem);
    if (fgets(entrada, sizeof entrada, stdin) == NULL) {
        return 0;
    }

    errno = 0;
    char *fim;
    long convertido = strtol(entrada, &fim, 10);
    while (isspace((unsigned char)*fim)) {
        fim++;
    }
    if (entrada == fim || *fim != '\0' || errno == ERANGE
        || convertido < INT_MIN || convertido > INT_MAX) {
        return -1;
    }
    *valor = (int)convertido;
    return 1;
}

void demonstrarOperacoes(void)
{
    const int vetor[] = {
        3, 9, 1, 7, 2, 11, 4, 6, 8, 5,
        13, 0, 12, 10, 17, 14, 15, 16, 18, 19
    };
    const size_t tamanhos[] = {5, 10, 20};

    printf("\nAcesso O(1), busca do maior O(n) e pares O(n^2):\n");
    for (size_t i = 0; i < sizeof tamanhos / sizeof tamanhos[0]; i++) {
        size_t tamanho = tamanhos[i];
        size_t indice = tamanho / 2;
        int valor;
        int maior;
        size_t comparacoesPares;
        Contadores acesso = {0, 0};
        Contadores buscaMaior = {0, 0};

        if (!acessarPosicao(vetor, tamanho, indice, &valor, &acesso)
            || !encontrarMaior(vetor, tamanho, &maior, &buscaMaior)
            || !encontrarParesSoma(vetor, tamanho, 19, &comparacoesPares)) {
            fprintf(stderr, "Falha ao executar as operacoes do vetor.\n");
            return;
        }

        printf("n = %zu\n", tamanho);
        printf("  Acesso: valor %d, %zu acesso(s) [O(1)]\n",
               valor, acesso.acessos);
        printf("  Maior: %d, %zu comparacao(oes) [O(n)]\n",
               maior, buscaMaior.comparacoes);
        printf("  Pares: %zu comparacao(oes) [O(n^2)]\n\n",
               comparacoesPares);
    }

    int valor;
    Contadores contadorIndiceInvalido = {0, 0};
    if (!acessarPosicao(vetor, 20, 20, &valor, &contadorIndiceInvalido)) {
        printf("Indice igual ao tamanho do vetor rejeitado corretamente.\n");
    } else {
        fprintf(stderr, "A validacao aceitou um indice invalido.\n");
    }
}

void demonstrarBuscas(void)
{
    const int ordenado[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    };
    const int alvos[] = {0, 10, 19, 20};
    const size_t tamanho = sizeof ordenado / sizeof ordenado[0];

    if (!estaOrdenado(ordenado, tamanho)) {
        fprintf(stderr, "O vetor de busca binaria nao esta ordenado.\n");
        return;
    }

    printf("\nBusca linear O(n), qualquer sequencia; "
           "busca binaria O(log n), exige ordenacao:\n");
    printf("Alvo | Linear (posicao, comparacoes) | "
           "Binaria (posicao, comparacoes, iteracoes)\n");
    for (size_t i = 0; i < sizeof alvos / sizeof alvos[0]; i++) {
        size_t comparacoesLineares;
        size_t comparacoesBinarias;
        size_t iteracoesBinarias;
        int linear = buscaLinear(ordenado, tamanho, alvos[i],
                                 &comparacoesLineares);
        int binaria = buscaBinaria(ordenado, tamanho, alvos[i],
                                   &comparacoesBinarias, &iteracoesBinarias);
        printf("%d | (%d, %zu) | (%d, %zu, %zu)\n", alvos[i], linear,
               comparacoesLineares, binaria, comparacoesBinarias,
               iteracoesBinarias);
    }

    const int desordenado[] = {8, 3, 12, 1, 6};
    size_t comparacoes;
    int posicao = buscaLinear(desordenado,
                              sizeof desordenado / sizeof desordenado[0],
                              1, &comparacoes);
    printf("Vetor desordenado: linear encontra 1 na posicao %d "
           "com %zu comparacoes; busca binaria nao se aplica.\n",
           posicao, comparacoes);
}

void demonstrarRecursaoLinear(void)
{
    const int vetor[] = {
        3, 9, 1, 7, 2, 11, 4, 6, 8, 5,
        13, 0, 12, 10, 17, 14, 15, 16, 18, 19
    };
    const unsigned int numero = 8;
    size_t chamadasFatorial = 0;
    size_t iteracoesFatorial;
    size_t chamadasSoma = 0;
    size_t iteracoesSoma;
    unsigned long long fatorialRec = fatorialRecursivo(numero,
                                                       &chamadasFatorial);
    unsigned long long fatorialIter = fatorialIterativo(numero,
                                                         &iteracoesFatorial);
    long long somaRec = somarVetorRecursivo(vetor, 20, &chamadasSoma);
    long long somaIter = somarVetorIterativo(vetor, 20, &iteracoesSoma);

    printf("\nRecursao linear: fatorial(8) recursivo = %llu (%zu chamadas), "
           "iterativo = %llu (%zu iteracoes).\n",
           fatorialRec, chamadasFatorial, fatorialIter, iteracoesFatorial);
    printf("Soma recursiva = %lld (%zu chamadas incluindo caso-base), "
           "iterativa = %lld (%zu iteracoes).\n",
           somaRec, chamadasSoma, somaIter, iteracoesSoma);
    if (fatorialRec != fatorialIter || somaRec != somaIter) {
        fprintf(stderr, "Resultados recursivos e iterativos divergiram.\n");
    }
}

void demonstrarRecursaoRamificada(void)
{
    enum { FIBONACCI_MAX = 30 };
    int entrada;
    int leitura = lerInteiro("Informe n para Fibonacci (0 a 30): ", &entrada);
    if (leitura == 0) {
        return;
    }
    if (leitura < 0 || entrada < 0 || entrada > FIBONACCI_MAX) {
        fprintf(stderr, "Entrada invalida: use um inteiro entre 0 e %d.\n",
                FIBONACCI_MAX);
        return;
    }

    size_t chamadasFibonacci = 0;
    unsigned long long fibonacciRec =
        fibonacciRecursivo((unsigned int)entrada, &chamadasFibonacci);
    unsigned long long fibonacciIter =
        fibonacciIterativo((unsigned int)entrada);

    printf("\nFibonacci recursivo: F(0)=0 e F(1)=1 sao casos-base; "
           "cada chamada restante gera duas subchamadas.\n");
    printf("  F(%d) = %llu, %zu chamadas; iterativo = %llu.\n",
           entrada, fibonacciRec, chamadasFibonacci, fibonacciIter);

    const int ordenado[] = {
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19
    };
    const size_t tamanho = sizeof ordenado / sizeof ordenado[0];
    const int alvos[] = {11, 20};
    if (!estaOrdenado(ordenado, tamanho)) {
        fprintf(stderr, "O vetor de busca binaria nao esta ordenado.\n");
        return;
    }

    printf("Busca binaria recursiva: caso-base intervalo vazio; "
           "cada chamada restante escolhe uma metade.\n");
    printf("A contagem de chamadas inclui a chamada do caso-base; "
           "iteracoes contam apenas passagens pelo laco.\n");
    printf("Alvo | Posicao | Chamadas recursivas | "
           "Iteracoes iterativas | Comparacoes iterativas\n");
    for (size_t i = 0; i < sizeof alvos / sizeof alvos[0]; i++) {
        size_t chamadas = 0;
        size_t comparacoesIterativas;
        size_t iteracoesIterativas;
        int posicao = buscaBinariaRecursiva(ordenado, 0, tamanho, alvos[i],
                                            &chamadas);
        int posicaoIterativa = buscaBinaria(ordenado, tamanho, alvos[i],
                                            &comparacoesIterativas,
                                            &iteracoesIterativas);
        printf("%d | %d | %zu | %zu | %zu\n", alvos[i], posicao, chamadas,
               iteracoesIterativas, comparacoesIterativas);
        if (posicao != posicaoIterativa) {
            fprintf(stderr, "As buscas binarias divergiram para alvo %d.\n",
                    alvos[i]);
        }
    }
    printf("Fibonacci ingenuo cresce exponencialmente; busca binaria "
           "recursiva descarta metade e cresce em O(log n).\n");
}

int main(void)
{
    for (;;) {
        int opcao;
        printf("\n=== Laboratorio de algoritmos ===\n"
               "1 - Acesso, maior e busca de pares\n"
               "2 - Busca linear e busca binaria iterativa\n"
               "3 - Recursao linear: fatorial e soma\n"
               "4 - Fibonacci ramificado e busca binaria recursiva\n"
               "0 - Sair\n");
        int leitura = lerInteiro("Opcao: ", &opcao);
        if (leitura == 0 || (leitura > 0 && opcao == 0)) {
            break;
        }
        if (leitura < 0) {
            fprintf(stderr, "Opcao invalida. Digite um numero do menu.\n");
            continue;
        }

        switch (opcao) {
        case 1:
            demonstrarOperacoes();
            break;
        case 2:
            demonstrarBuscas();
            break;
        case 3:
            demonstrarRecursaoLinear();
            break;
        case 4:
            demonstrarRecursaoRamificada();
            break;
        default:
            fprintf(stderr, "Opcao inexistente.\n");
            break;
        }
    }
    return 0;
}