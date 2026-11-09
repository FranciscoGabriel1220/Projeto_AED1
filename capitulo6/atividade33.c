#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long comparacoes;
    long deslocamentos;
} Contadores;

/* Insere v[i] no prefixo ordenado v[0..i-1] */
void inserir_em_prefixo_ordenado(int v[], int i, Contadores *c) {
    int chave = v[i];          /* guarda o valor para não perdê-lo */
    int j = i - 1;

    while (j >= 0) {           /* limite inferior: nunca lê v[-1] */
        c->comparacoes++;
        if (v[j] > chave) {
            v[j + 1] = v[j];   /* desloca para a direita */
            c->deslocamentos++;
            j--;
        } else {
            break;             /* achou a posição */
        }
    }
    v[j + 1] = chave;          /* escreve a chave no espaço aberto */
}

void insertion_sort(int v[], int n, Contadores *c) {
    for (int i = 1; i < n; i++)
        inserir_em_prefixo_ordenado(v, i, c);
}

int esta_ordenado(const int v[], int n) {
    for (int i = 1; i < n; i++)
        if (v[i - 1] > v[i]) return 0;
    return 1;
}

int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

void imprimir(const char *rotulo, const int v[], int n) {
    printf("%s: ", rotulo);
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

int main(void) {
    int entrada[] = {7, 3, 9, 1, 5, 8, 2};   /* substitua pela sua entrada */
    int n = sizeof(entrada) / sizeof(entrada[0]);

    int v[n], referencia[n];
    memcpy(v, entrada, sizeof(entrada));           /* cópia de trabalho */
    memcpy(referencia, entrada, sizeof(entrada));  /* cópia para validar */

    Contadores c = {0, 0};
    imprimir("Antes ", v, n);
    insertion_sort(v, n, &c);
    imprimir("Depois", v, n);

    qsort(referencia, n, sizeof(int), cmp_int);
    int mesmos_elementos = memcmp(v, referencia, sizeof(int) * n) == 0;

    printf("Comparacoes: %ld\n", c.comparacoes);
    printf("Deslocamentos: %ld\n", c.deslocamentos);
    printf("Ordenado: %s\n", esta_ordenado(v, n) ? "sim" : "nao");
    printf("Mesmos elementos: %s\n", mesmos_elementos ? "sim" : "nao");
    return 0;
}