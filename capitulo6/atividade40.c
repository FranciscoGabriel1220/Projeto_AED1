#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <time.h>

#define TAM_NOME 50

typedef struct {
    long comparacoes;
    long deslocamentos;
    long trocas;
    long copias;
} Contadores;

typedef struct {
    int id;
    char nome[TAM_NOME];
    int pontuacao;
} Jogador;

/* ========================================================= */
/*  UTILITÁRIOS E VALIDAÇÃO                                  */
/* ========================================================= */

int esta_ordenado(const int v[], int n) {
    for (int i = 1; i < n; i++)
        if (v[i - 1] > v[i]) return 0;
    return 1;
}

int cmp_int(const void *a, const void *b) {
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

void imprimir_inteiros(const char *rotulo, const int v[], int n) {
    printf("%s: ", rotulo);
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

/* ========================================================= */
/*  PARTE 1: Selection Sort & Insertion Sort (Inteiros)       */
/* ========================================================= */

/* Selection Sort: busca o menor elemento no sufixo e realiza no máximo 1 troca por passada */
void selection_sort(int v[], int n, Contadores *c) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            c->comparacoes++;
            if (v[j] < v[min_idx]) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int tmp = v[i];
            v[i] = v[min_idx];
            v[min_idx] = tmp;
            c->trocas++;
        }
    }
}

void demo_selection_inteiros(void) {
    int entrada[] = {7, 3, 9, 1, 5, 8, 2};
    int n = sizeof(entrada) / sizeof(entrada[0]);

    int v[n], referencia[n];
    memcpy(v, entrada, sizeof(entrada));
    memcpy(referencia, entrada, sizeof(entrada));

    Contadores c = {0, 0, 0, 0};
    printf("\n=== Selection Sort: inteiros ===\n");
    imprimir_inteiros("Antes ", v, n);
    selection_sort(v, n, &c);
    imprimir_inteiros("Depois", v, n);

    qsort(referencia, n, sizeof(int), cmp_int);
    int mesmos_elementos = memcmp(v, referencia, sizeof(int) * n) == 0;

    printf("Comparacoes: %ld\n", c.comparacoes);
    printf("Trocas: %ld\n", c.trocas);
    printf("Ordenado: %s\n", esta_ordenado(v, n) ? "sim" : "nao");
    printf("Mesmos elementos: %s\n", mesmos_elementos ? "sim" : "nao");
}

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

void demo_inteiros(void) {
    int entrada[] = {7, 3, 9, 1, 5, 8, 2};
    int n = sizeof(entrada) / sizeof(entrada[0]);

    int v[n], referencia[n];
    memcpy(v, entrada, sizeof(entrada));
    memcpy(referencia, entrada, sizeof(entrada));

    Contadores c = {0, 0, 0, 0};
    printf("\n=== Insertion Sort: inteiros ===\n");
    imprimir_inteiros("Antes ", v, n);
    insertion_sort(v, n, &c);
    imprimir_inteiros("Depois", v, n);

    qsort(referencia, n, sizeof(int), cmp_int);
    int mesmos_elementos = memcmp(v, referencia, sizeof(int) * n) == 0;

    printf("Comparacoes: %ld\n", c.comparacoes);
    printf("Deslocamentos: %ld\n", c.deslocamentos);
    printf("Ordenado: %s\n", esta_ordenado(v, n) ? "sim" : "nao");
    printf("Mesmos elementos: %s\n", mesmos_elementos ? "sim" : "nao");
}

/* ========================================================= */
/*  PARTE 2: Estruturas Jogador & Algoritmos Diretos          */
/* ========================================================= */

int comparar_jogadores(const Jogador *a, const Jogador *b) {
    int r = strcmp(a->nome, b->nome);
    if (r != 0) return r;
    return (a->id > b->id) - (a->id < b->id);
}

void imprimir_jogadores(const char *titulo, const Jogador v[], int n) {
    printf("\n%s\n", titulo);
    printf("%-4s %-15s %s\n", "ID", "Nome", "Pontos");
    for (int i = 0; i < n; i++)
        printf("%-4d %-15s %d\n", v[i].id, v[i].nome, v[i].pontuacao);
}

int jogadores_ordenados(const Jogador v[], int n) {
    for (int i = 1; i < n; i++)
        if (comparar_jogadores(&v[i - 1], &v[i]) > 0) return 0;
    return 1;
}

int associacoes_preservadas(const Jogador original[], const Jogador ordenado[], int n) {
    for (int i = 0; i < n; i++) {
        int achou = 0;
        for (int k = 0; k < n; k++) {
            if (original[k].id == ordenado[i].id) {
                if (strcmp(original[k].nome, ordenado[i].nome) != 0 ||
                    original[k].pontuacao != ordenado[i].pontuacao)
                    return 0;
                achou = 1;
                break;
            }
        }
        if (!achou) return 0;
    }
    return 1;
}

void selection_sort_jogadores(Jogador v[], int n, Contadores *c) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            c->comparacoes++;
            if (comparar_jogadores(&v[j], &v[min_idx]) < 0) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            Jogador tmp = v[i];
            v[i] = v[min_idx];
            v[min_idx] = tmp;
            c->trocas++;
        }
    }
}

void demo_selection_jogadores(void) {
    Jogador catalogo[] = {
        {7, "Ana",    850},
        {5, "Bruno",  620},
        {9, "Carla",  910},
        {3, "Ana",    430},
        {2, "Bruno",  700},
        {8, "Diego",  560},
        {1, "Bia",    990},
    };
    int n = sizeof(catalogo) / sizeof(catalogo[0]);

    Jogador trabalho[n], original[n];
    memcpy(trabalho, catalogo, sizeof(catalogo));
    memcpy(original, catalogo, sizeof(catalogo));

    Contadores c = {0, 0, 0, 0};
    printf("\n=== Selection Sort: jogadores por nome ===\n");
    imprimir_jogadores("Antes:", trabalho, n);
    selection_sort_jogadores(trabalho, n, &c);
    imprimir_jogadores("Depois:", trabalho, n);

    printf("\nComparacoes: %ld\n", c.comparacoes);
    printf("Trocas: %ld\n", c.trocas);
    printf("Ordenado (nome, depois id): %s\n",
           jogadores_ordenados(trabalho, n) ? "sim" : "nao");
    printf("Id, nome e pontuacao continuam associados: %s\n",
           associacoes_preservadas(original, trabalho, n) ? "sim" : "nao");
}

void insertion_sort_jogadores(Jogador v[], int n, Contadores *c) {
    for (int i = 1; i < n; i++) {
        Jogador chave = v[i];
        int j = i - 1;

        while (j >= 0) {
            c->comparacoes++;
            if (comparar_jogadores(&v[j], &chave) > 0) {
                v[j + 1] = v[j];
                c->deslocamentos++;
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }
}

void demo_jogadores(void) {
    Jogador catalogo[] = {
        {7, "Ana",    850},
        {5, "Bruno",  620},
        {9, "Carla",  910},
        {3, "Ana",    430},
        {2, "Bruno",  700},
        {8, "Diego",  560},
        {1, "Bia",    990},
    };
    int n = sizeof(catalogo) / sizeof(catalogo[0]);

    Jogador trabalho[n], original[n];
    memcpy(trabalho, catalogo, sizeof(catalogo));
    memcpy(original, catalogo, sizeof(catalogo));

    Contadores c = {0, 0, 0, 0};
    printf("\n=== Insertion Sort: jogadores por nome ===");
    imprimir_jogadores("Antes:", trabalho, n);
    insertion_sort_jogadores(trabalho, n, &c);
    imprimir_jogadores("Depois:", trabalho, n);

    printf("\nComparacoes: %ld\n", c.comparacoes);
    printf("Deslocamentos: %ld\n", c.deslocamentos);
    printf("Ordenado (nome, depois id): %s\n",
           jogadores_ordenados(trabalho, n) ? "sim" : "nao");
    printf("Id, nome e pontuacao continuam associados: %s\n",
           associacoes_preservadas(original, trabalho, n) ? "sim" : "nao");
}

/* ========================================================= */
/*  PARTE 3: Bubble Sort com parada antecipada               */
/* ========================================================= */

typedef struct {
    long comparacoes;
    long trocas;
    long passagens;
} MetricasBubble;

void bubble_sort(int v[], int n, MetricasBubble *m) {
    int limite = n - 1;
    int trocou = 1;

    while (trocou && limite > 0) {
        trocou = 0;
        m->passagens++;

        for (int j = 0; j < limite; j++) {
            m->comparacoes++;
            if (v[j] > v[j + 1]) {
                int tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
                m->trocas++;
                trocou = 1;
            }
        }
        limite--;
    }
}

void testar_bubble(const char *rotulo, const int entrada[], int n) {
    int v[n], referencia[n];
    memcpy(v, entrada, sizeof(int) * n);
    memcpy(referencia, entrada, sizeof(int) * n);

    MetricasBubble m = {0, 0, 0};
    printf("\n--- %s ---\n", rotulo);
    imprimir_inteiros("Antes ", v, n);
    bubble_sort(v, n, &m);
    imprimir_inteiros("Depois", v, n);

    qsort(referencia, n, sizeof(int), cmp_int);
    int mesmos_elementos = memcmp(v, referencia, sizeof(int) * n) == 0;

    printf("Passagens: %ld\n", m.passagens);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Ordenado: %s\n", esta_ordenado(v, n) ? "sim" : "nao");
    printf("Mesmos elementos: %s\n", mesmos_elementos ? "sim" : "nao");
}

void demo_bubble(void) {
    int ordenado[]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[]   = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int aleatorio[] = {6, 2, 9, 4, 10, 1, 8, 3, 7, 5};
    int n = sizeof(ordenado) / sizeof(ordenado[0]);

    printf("\n=== Bubble Sort com parada antecipada ===");
    testar_bubble("Vetor ja ordenado", ordenado, n);
    testar_bubble("Vetor em ordem inversa", inverso, n);
    testar_bubble("Vetor aleatorio", aleatorio, n);
}

typedef enum { CAMPO_ID, CAMPO_PONTUACAO } Campo;
typedef enum { CRESCENTE, DECRESCENTE } Direcao;

typedef struct {
    Campo campo;
    Direcao direcao;
} Criterio;

static const Criterio CRITERIOS[] = {
    {CAMPO_ID,        CRESCENTE},
    {CAMPO_ID,        DECRESCENTE},
    {CAMPO_PONTUACAO, CRESCENTE},
    {CAMPO_PONTUACAO, DECRESCENTE},
};
#define NUM_CRITERIOS ((int)(sizeof(CRITERIOS) / sizeof(CRITERIOS[0])))

const char *nome_campo(Campo c) {
    return (c == CAMPO_ID) ? "ID" : "Pontuacao";
}

const char *nome_direcao(Direcao d) {
    return (d == CRESCENTE) ? "crescente" : "decrescente";
}

int comparar_jogadores_por(const Jogador *a, const Jogador *b, Criterio crit) {
    int r;
    switch (crit.campo) {
        case CAMPO_ID:
            r = (a->id > b->id) - (a->id < b->id);
            break;
        case CAMPO_PONTUACAO:
            r = (a->pontuacao > b->pontuacao) - (a->pontuacao < b->pontuacao);
            break;
        default:
            r = 0;
    }
    return (crit.direcao == DECRESCENTE) ? -r : r;
}

void bubble_sort_jogadores(Jogador v[], int n, Criterio crit, MetricasBubble *m) {
    int limite = n - 1;
    int trocou = 1;

    while (trocou && limite > 0) {
        trocou = 0;
        m->passagens++;

        for (int j = 0; j < limite; j++) {
            m->comparacoes++;
            if (comparar_jogadores_por(&v[j], &v[j + 1], crit) > 0) {
                Jogador tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
                m->trocas++;
                trocou = 1;
            }
        }
        limite--;
    }
}

int jogadores_ordenados_por(const Jogador v[], int n, Criterio crit) {
    for (int i = 1; i < n; i++)
        if (comparar_jogadores_por(&v[i - 1], &v[i], crit) > 0) return 0;
    return 1;
}

void demo_bubble_jogadores(Criterio crit) {
    Jogador catalogo[] = {
        {7, "Ana",    850},
        {5, "Bruno",  700},
        {9, "Carla",  910},
        {3, "Ana",    430},
        {2, "Bruno",  700},
        {8, "Diego",  560},
        {1, "Bia",    990},
    };
    int n = sizeof(catalogo) / sizeof(catalogo[0]);

    Jogador trabalho[n], original[n];
    memcpy(trabalho, catalogo, sizeof(catalogo));
    memcpy(original, catalogo, sizeof(catalogo));

    MetricasBubble m = {0, 0, 0};
    printf("\n=== Bubble Sort em jogadores ===\n");
    printf("Criterio: %s (%s)\n", nome_campo(crit.campo), nome_direcao(crit.direcao));
    imprimir_jogadores("Antes:", trabalho, n);
    bubble_sort_jogadores(trabalho, n, crit, &m);
    imprimir_jogadores("Depois:", trabalho, n);

    printf("\nPassagens: %ld\n", m.passagens);
    printf("Comparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Ordenado por %s (%s): %s\n",
           nome_campo(crit.campo), nome_direcao(crit.direcao),
           jogadores_ordenados_por(trabalho, n, crit) ? "sim" : "nao");
    printf("Id, nome e pontuacao continuam associados: %s\n",
           associacoes_preservadas(original, trabalho, n) ? "sim" : "nao");
}

/* ========================================================= */
/*  PARTE 4: Intercalação e Merge Sort                       */
/* ========================================================= */

#define TAM(v) ((int)(sizeof(v) / sizeof((v)[0])))

int intercalar(const int a[], int na, const int b[], int nb, int saida[], long *comparacoes) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        (*comparacoes)++;
        if (a[i] <= b[j])
            saida[k++] = a[i++];
        else
            saida[k++] = b[j++];
    }
    while (i < na) saida[k++] = a[i++];
    while (j < nb) saida[k++] = b[j++];
    return k;
}

void testar_intercalacao(const char *rotulo, const int a[], int na, const int b[], int nb) {
    int total = na + nb;
    int saida[total + 1], referencia[total + 1];
    long comparacoes = 0;

    int escritos = intercalar(a, na, b, nb, saida, &comparacoes);

    for (int i = 0; i < na; i++) referencia[i] = a[i];
    for (int j = 0; j < nb; j++) referencia[na + j] = b[j];
    qsort(referencia, total, sizeof(int), cmp_int);

    int minimo = (na < nb) ? na : nb;
    int maximo = (total > 0) ? total - 1 : 0;

    printf("\n--- %s ---\n", rotulo);
    imprimir_inteiros("A     ", a, na);
    imprimir_inteiros("B     ", b, nb);
    imprimir_inteiros("Saida ", saida, escritos);
    printf("Comparacoes: %ld (entre %d e %d possiveis)\n", comparacoes, minimo, maximo);
    printf("Tamanho correto (%d): %s\n", total, escritos == total ? "sim" : "nao");
    printf("Ordenada: %s\n", esta_ordenado(saida, escritos) ? "sim" : "nao");
    printf("Todos os valores presentes: %s\n",
           (escritos == total && memcmp(saida, referencia, sizeof(int) * total) == 0)
               ? "sim" : "nao");
}

void demo_intercalacao(void) {
    int vazio[1] = {0};
    int a1[] = {2, 5, 8, 11, 14}, b1[] = {1, 3, 6, 9};
    int a2[] = {1, 2, 3},         b2[] = {7, 8, 9, 10};
    int a3[] = {7, 8, 9},         b3[] = {1, 2, 3, 4};
    int a4[] = {1, 3, 3, 5},      b4[] = {3, 3, 4, 5};
    int b5[] = {4, 6, 9};

    printf("\n=== Intercalacao de dois vetores ordenados ===\n");
    testar_intercalacao("Intercalado",              a1, TAM(a1), b1, TAM(b1));
    testar_intercalacao("Todos de A menores que B", a2, TAM(a2), b2, TAM(b2));
    testar_intercalacao("Todos de B menores que A", a3, TAM(a3), b3, TAM(b3));
    testar_intercalacao("Valores repetidos",        a4, TAM(a4), b4, TAM(b4));
    testar_intercalacao("Uma entrada vazia",        vazio, 0,    b5, TAM(b5));
}

int ponto_medio(int ini, int fim) {
    return ini + (fim - ini) / 2;
}

void dividir_intervalo(const int v[], int ini, int fim, int nivel, long *chamadas) {
    (*chamadas)++;

    printf("%*s[%d..%d] {", nivel * 4, "", ini, fim);
    for (int i = ini; i <= fim; i++) printf("%s%d", (i > ini) ? " " : "", v[i]);
    printf("}");

    if (ini >= fim) {
        printf("  <- caso base\n");
        return;
    }

    int meio = ponto_medio(ini, fim);
    printf("  meio=%d\n", meio);

    dividir_intervalo(v, ini, meio, nivel + 1, chamadas);
    dividir_intervalo(v, meio + 1, fim, nivel + 1, chamadas);

    printf("%*s<- combinar [%d..%d] + [%d..%d]\n",
           nivel * 4, "", ini, meio, meio + 1, fim);
}

int validar_ponto_medio(long *testados) {
    *testados = 0;
    for (int ini = 0; ini <= 20; ini++) {
        for (int fim = ini + 1; fim <= 20; fim++) {
            int meio = ponto_medio(ini, fim);
            int esq = meio - ini + 1, dir = fim - meio;
            (*testados)++;
            if (meio < ini || meio >= fim) return 0;
            if (esq < dir || esq - dir > 1) return 0;
        }
    }
    int grande_ini = INT_MAX - 10, grande_fim = INT_MAX;
    int meio = ponto_medio(grande_ini, grande_fim);
    (*testados)++;
    return meio >= grande_ini && meio < grande_fim;
}

void demo_divisao(void) {
    int v[] = {7, 3, 9, 1, 5, 8, 2};
    int n = TAM(v);
    long chamadas = 0, testados = 0;

    printf("\n=== Divisao recursiva de intervalo (estrutura do Merge Sort) ===\n");
    dividir_intervalo(v, 0, n - 1, 0, &chamadas);

    printf("\nChamadas: %ld (esperado 2n-1 = %d)\n", chamadas, 2 * n - 1);
    int ok = validar_ponto_medio(&testados);
    printf("Ponto medio valido em %ld intervalos testados: %s\n", testados, ok ? "sim" : "nao");
    printf("Nenhum elemento foi movido: esta opcao so mostra a divisao.\n");
}

typedef int (*Comparador)(const void *a, const void *b, void *ctx);

static long g_alocacoes = 0, g_liberacoes = 0;

static void *aux_alocar(size_t bytes) {
    void *p = malloc(bytes);
    if (p != NULL) g_alocacoes++;
    return p;
}

static void aux_liberar(void *p) {
    if (p != NULL) {
        free(p);
        g_liberacoes++;
    }
}

static unsigned char *elem(void *base, int i, size_t tam) {
    return (unsigned char *)base + (size_t)i * tam;
}

static void combinar_faixas(void *base, void *aux, int ini, int meio, int fim,
                            size_t tam, Comparador cmp, void *ctx, long *comparacoes) {
    int i = ini, j = meio + 1, k = ini;

    while (i <= meio && j <= fim) {
        (*comparacoes)++;
        if (cmp(elem(base, i, tam), elem(base, j, tam), ctx) <= 0) {
            memcpy(elem(aux, k, tam), elem(base, i, tam), tam);
            i++;
        } else {
            memcpy(elem(aux, k, tam), elem(base, j, tam), tam);
            j++;
        }
        k++;
    }

    while (i <= meio) {
        memcpy(elem(aux, k, tam), elem(base, i, tam), tam);
        i++; k++;
    }
    while (j <= fim) {
        memcpy(elem(aux, k, tam), elem(base, j, tam), tam);
        j++; k++;
    }

    memcpy(elem(base, ini, tam), elem(aux, ini, tam), (size_t)(fim - ini + 1) * tam);
}

static void merge_sort_rec(void *base, void *aux, int ini, int fim,
                           size_t tam, Comparador cmp, void *ctx, long *comparacoes) {
    if (ini >= fim) return;
    int meio = ponto_medio(ini, fim);
    merge_sort_rec(base, aux, ini, meio, tam, cmp, ctx, comparacoes);
    merge_sort_rec(base, aux, meio + 1, fim, tam, cmp, ctx, comparacoes);
    combinar_faixas(base, aux, ini, meio, fim, tam, cmp, ctx, comparacoes);
}

int merge_sort_generico(void *base, int n, size_t tam, Comparador cmp, void *ctx,
                        long *comparacoes) {
    if (n <= 1) return 0;
    void *aux = aux_alocar((size_t)n * tam);
    if (aux == NULL) return -1;
    merge_sort_rec(base, aux, 0, n - 1, tam, cmp, ctx, comparacoes);
    aux_liberar(aux);
    return 0;
}

int cmp_int_ctx(const void *a, const void *b, void *ctx) {
    (void)ctx;
    int x = *(const int *)a, y = *(const int *)b;
    return (x > y) - (x < y);
}

int cmp_jogador_ctx(const void *a, const void *b, void *ctx) {
    const Criterio *crit = ctx;
    return comparar_jogadores_por((const Jogador *)a, (const Jogador *)b, *crit);
}

void testar_merge_inteiros(const char *rotulo, const int entrada[], int n) {
    int v[n + 1], referencia[n + 1];
    for (int i = 0; i < n; i++) {
        v[i] = entrada[i];
        referencia[i] = entrada[i];
    }

    long comparacoes = 0;
    long aloc_antes = g_alocacoes, lib_antes = g_liberacoes;

    printf("\n--- %s ---\n", rotulo);
    imprimir_inteiros("Antes ", v, n);
    if (merge_sort_generico(v, n, sizeof(int), cmp_int_ctx, NULL, &comparacoes) != 0) {
        printf("Falha: sem memoria para o vetor auxiliar.\n");
        return;
    }
    imprimir_inteiros("Depois", v, n);

    qsort(referencia, n, sizeof(int), cmp_int);
    int mesmos = memcmp(v, referencia, sizeof(int) * n) == 0;
    long aloc = g_alocacoes - aloc_antes, lib = g_liberacoes - lib_antes;

    printf("Comparacoes: %ld\n", comparacoes);
    printf("Ordenado: %s\n", esta_ordenado(v, n) ? "sim" : "nao");
    printf("Mesmos elementos (repetidos incluidos): %s\n", mesmos ? "sim" : "nao");
    printf("Memoria auxiliar: %ld alocacao, %ld liberacao -> %s\n",
           aloc, lib, (aloc == lib) ? "liberada" : "VAZAMENTO");
}

void demo_merge_inteiros(void) {
    int vazio[1] = {0};
    int um[]         = {42};
    int opcao1[]     = {7, 3, 9, 1, 5, 8, 2};
    int ordenado[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[]    = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int aleatorio[]  = {6, 2, 9, 4, 10, 1, 8, 3, 7, 5};
    int repetidos[]  = {5, 3, 5, 1, 3, 5, 1, 3};

    printf("\n=== Merge Sort em inteiros ===\n");
    testar_merge_inteiros("Entrada da opcao 1",       opcao1,    TAM(opcao1));
    testar_merge_inteiros("Ja ordenado",              ordenado,  TAM(ordenado));
    testar_merge_inteiros("Ordem inversa",            inverso,   TAM(inverso));
    testar_merge_inteiros("Aleatorio",                aleatorio, TAM(aleatorio));
    testar_merge_inteiros("Valores repetidos",        repetidos, TAM(repetidos));
    testar_merge_inteiros("Um elemento",              um,        TAM(um));
    testar_merge_inteiros("Vazio",                    vazio,     0);
}

int mesmos_jogadores(const Jogador a[], const Jogador b[], int n) {
    int usado[n + 1];
    memset(usado, 0, sizeof(int) * (n + 1));
    for (int i = 0; i < n; i++) {
        int achou = 0;
        for (int j = 0; j < n && !achou; j++) {
            if (!usado[j] && a[i].id == b[j].id && a[i].pontuacao == b[j].pontuacao &&
                strcmp(a[i].nome, b[j].nome) == 0) {
                usado[j] = 1;
                achou = 1;
            }
        }
        if (!achou) return 0;
    }
    return 1;
}

static int posicao_por_id(const Jogador v[], int n, int id) {
    for (int i = 0; i < n; i++)
        if (v[i].id == id) return i;
    return -1;
}

int ordem_estavel(const Jogador original[], const Jogador ordenado[], int n, Criterio crit) {
    for (int i = 1; i < n; i++) {
        if (comparar_jogadores_por(&ordenado[i - 1], &ordenado[i], crit) == 0 &&
            posicao_por_id(original, n, ordenado[i - 1].id) >
                posicao_por_id(original, n, ordenado[i].id))
            return 0;
    }
    return 1;
}

void demo_merge_jogadores(void) {
    Jogador catalogo[] = {
        {7, "Ana",    850},
        {5, "Bruno",  700},
        {9, "Carla",  910},
        {3, "Ana",    430},
        {2, "Bruno",  700},
        {8, "Diego",  560},
        {1, "Bia",    990},
    };
    int n = TAM(catalogo);

    printf("\n=== Merge Sort em jogadores ===");
    imprimir_jogadores("Entrada (catalogo da central):", catalogo, n);

    for (int c = 0; c < NUM_CRITERIOS; c++) {
        Criterio crit = CRITERIOS[c];
        Jogador trabalho[n];
        memcpy(trabalho, catalogo, sizeof(catalogo));

        long comparacoes = 0;
        long aloc_antes = g_alocacoes, lib_antes = g_liberacoes;

        printf("\n--- Criterio: %s (%s) ---", nome_campo(crit.campo), nome_direcao(crit.direcao));
        if (merge_sort_generico(trabalho, n, sizeof(Jogador), cmp_jogador_ctx, &crit,
                                &comparacoes) != 0) {
            printf("\nFalha: sem memoria para o vetor auxiliar.\n");
            continue;
        }
        imprimir_jogadores("Resultado:", trabalho, n);

        long aloc = g_alocacoes - aloc_antes, lib = g_liberacoes - lib_antes;
        printf("\nComparacoes: %ld\n", comparacoes);
        printf("Ordenado pelo criterio: %s\n",
               jogadores_ordenados_por(trabalho, n, crit) ? "sim" : "nao");
        printf("Mesmos registros da entrada: %s\n",
               mesmos_jogadores(catalogo, trabalho, n) ? "sim" : "nao");
        printf("Empates mantem a ordem original (estavel): %s\n",
               ordem_estavel(catalogo, trabalho, n, crit) ? "sim" : "nao");
        printf("Memoria auxiliar: %ld alocacao, %ld liberacao -> %s\n",
               aloc, lib, (aloc == lib) ? "liberada" : "VAZAMENTO");
    }
}

/* ========================================================= */
/*  PARTE 5: Quick Sort (Partições, Pivôs e Estruturas)      */
/* ========================================================= */

typedef struct {
    long comparacoes;
    long trocas;
    long copias;
    long particoes;
} MetricasQuick;

static void trocar_int(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static void trocar_jogador(Jogador *a, Jogador *b) {
    Jogador tmp = *a;
    *a = *b;
    *b = tmp;
}

int particionar_auxiliar(int v[], int lo, int hi, MetricasQuick *m) {
    int pivo = v[hi];
    int n = hi - lo + 1;

    int *aux = aux_alocar(sizeof(int) * (size_t)(2 * n));
    if (aux == NULL) return -1;
    int *menores = aux;
    int *maiores = aux + n;
    int nm = 0, nM = 0;

    m->particoes++;
    for (int i = lo; i < hi; i++) {
        m->comparacoes++;
        if (v[i] < pivo) menores[nm++] = v[i];
        else             maiores[nM++] = v[i];
        m->copias++;
    }

    int k = lo;
    for (int t = 0; t < nm; t++) { v[k++] = menores[t]; m->copias++; }
    int pos_pivo = k;
    v[k++] = pivo;                               m->copias++;
    for (int t = 0; t < nM; t++) { v[k++] = maiores[t]; m->copias++; }

    aux_liberar(aux);
    return pos_pivo;
}

int particionar_inplace(int v[], int lo, int hi, MetricasQuick *m) {
    int pivo = v[hi];
    int i = lo;

    m->particoes++;
    for (int j = lo; j < hi; j++) {
        m->comparacoes++;
        if (v[j] < pivo) {
            if (i != j) {
                trocar_int(&v[i], &v[j]);
                m->trocas++;
            }
            i++;
        }
    }
    if (i != hi) {
        trocar_int(&v[i], &v[hi]);
        m->trocas++;
    }
    return i;
}

/* Pivô Aleatório: seleciona índice randômico em [lo..hi], troca com v[hi] e particiona in-place */
int particionar_aleatorio(int v[], int lo, int hi, MetricasQuick *m) {
    int idx_pivo = lo + rand() % (hi - lo + 1);
    if (idx_pivo != hi) {
        trocar_int(&v[idx_pivo], &v[hi]);
        m->trocas++;
    }
    return particionar_inplace(v, lo, hi, m);
}

int particao_valida(const int v[], int lo, int hi, int p, int pivo) {
    if (p < lo || p > hi || v[p] != pivo) return 0;
    for (int i = lo; i < p; i++)
        if (v[i] >= pivo) return 0;
    for (int i = p + 1; i <= hi; i++)
        if (v[i] < pivo) return 0;
    return 1;
}

void testar_particao(const char *rotulo, const int entrada[], int n, int lo, int hi) {
    int original[n], aux_v[n], inpl_v[n];
    for (int i = 0; i < n; i++) original[i] = aux_v[i] = inpl_v[i] = entrada[i];

    int pivo = entrada[hi];
    MetricasQuick ma = {0, 0, 0, 0}, mi = {0, 0, 0, 0};
    long aloc_antes = g_alocacoes, lib_antes = g_liberacoes;

    int pa = particionar_auxiliar(aux_v, lo, hi, &ma);
    int pi = particionar_inplace(inpl_v, lo, hi, &mi);
    long aloc = g_alocacoes - aloc_antes, lib = g_liberacoes - lib_antes;

    printf("\n--- %s ---\n", rotulo);
    printf("Intervalo [%d..%d], pivo = %d (ultimo elemento)\n", lo, hi, pivo);
    if (pa < 0) return;

    imprimir_inteiros("Entrada ", original, n);
    imprimir_inteiros("Auxiliar", aux_v, n);
    printf("  pivo na posicao %d | comparacoes %ld | copias %ld\n", pa, ma.comparacoes, ma.copias);
    imprimir_inteiros("In-place", inpl_v, n);
    printf("  pivo na posicao %d | comparacoes %ld | trocas %ld\n", pi, mi.comparacoes, mi.trocas);

    printf("Auxiliar ok: %s | In-place ok: %s | Mesma pos: %s\n",
           particao_valida(aux_v, lo, hi, pa, pivo) ? "sim" : "nao",
           particao_valida(inpl_v, lo, hi, pi, pivo) ? "sim" : "nao",
           (pa == pi) ? "sim" : "nao");
    printf("Memoria auxiliar: %ld alocacao, %ld liberacao -> %s\n",
           aloc, lib, (aloc == lib) ? "liberada" : "VAZAMENTO");
}

void demo_particoes(void) {
    int distintos[]  = {8, 3, 10, 1, 7, 4, 9, 5};
    int repetidos[]  = {5, 3, 5, 1, 5, 7, 3, 5};
    int n = TAM(distintos);

    printf("\n=== Particao do Quick Sort: auxiliar x in-place ===\n");
    testar_particao("Valores distintos", distintos, n, 0, n - 1);
    testar_particao("Valores repetidos", repetidos, TAM(repetidos), 0, TAM(repetidos) - 1);
}

static void quick_sort_rec(int v[], int lo, int hi, int aleatorio, MetricasQuick *m) {
    if (lo >= hi) return;
    int p = aleatorio ? particionar_aleatorio(v, lo, hi, m) : particionar_inplace(v, lo, hi, m);

    if (p - 1 > lo) quick_sort_rec(v, lo, p - 1, aleatorio, m);
    if (p + 1 < hi) quick_sort_rec(v, p + 1, hi, aleatorio, m);
}

void quick_sort(int v[], int n, MetricasQuick *m, int aleatorio) {
    if (n <= 1) return;
    quick_sort_rec(v, 0, n - 1, aleatorio, m);
}

void demo_quick_sort(void) {
    int opcao1[]     = {7, 3, 9, 1, 5, 8, 2};
    int ordenado[]   = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[]    = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int aleatorio[]  = {6, 2, 9, 4, 10, 1, 8, 3, 7, 5};

    printf("\n=== Quick Sort: Pivô Fixo vs Pivô Aleatório ===\n");

    MetricasQuick mf = {0, 0, 0, 0}, ma = {0, 0, 0, 0};
    int v_fixo[TAM(opcao1)], v_rand[TAM(opcao1)];
    memcpy(v_fixo, opcao1, sizeof(opcao1));
    memcpy(v_rand, opcao1, sizeof(opcao1));

    quick_sort(v_fixo, TAM(opcao1), &mf, 0);
    quick_sort(v_rand, TAM(opcao1), &ma, 1);

    printf("Entrada Opcao 1 (7 elems):\n");
    printf("  Pivo Fixo     -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", mf.comparacoes, mf.trocas, mf.particoes);
    printf("  Pivo Aleatorio -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", ma.comparacoes, ma.trocas, ma.particoes);

    MetricasQuick m_fixo_ord = {0, 0, 0, 0}, m_rand_ord = {0, 0, 0, 0};
    int n10 = TAM(ordenado);
    int v_ord_f[n10], v_ord_r[n10];
    memcpy(v_ord_f, ordenado, sizeof(ordenado));
    memcpy(v_ord_r, ordenado, sizeof(ordenado));
    quick_sort(v_ord_f, n10, &m_fixo_ord, 0);
    quick_sort(v_ord_r, n10, &m_rand_ord, 1);

    printf("\nVetor Ja Ordenado (10 elems):\n");
    printf("  Pivo Fixo (Pior Caso) -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", m_fixo_ord.comparacoes, m_fixo_ord.trocas, m_fixo_ord.particoes);
    printf("  Pivo Aleatorio        -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", m_rand_ord.comparacoes, m_rand_ord.trocas, m_rand_ord.particoes);

    MetricasQuick m_fixo_inv = {0, 0, 0, 0}, m_rand_inv = {0, 0, 0, 0};
    int v_inv_f[n10], v_inv_r[n10];
    memcpy(v_inv_f, inverso, sizeof(inverso));
    memcpy(v_inv_r, inverso, sizeof(inverso));
    quick_sort(v_inv_f, n10, &m_fixo_inv, 0);
    quick_sort(v_inv_r, n10, &m_rand_inv, 1);

    printf("\nVetor Ordem Inversa (10 elems):\n");
    printf("  Pivo Fixo (Pior Caso) -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", m_fixo_inv.comparacoes, m_fixo_inv.trocas, m_fixo_inv.particoes);
    printf("  Pivo Aleatorio        -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", m_rand_inv.comparacoes, m_rand_inv.trocas, m_rand_inv.particoes);

    MetricasQuick m_fixo_rand = {0, 0, 0, 0}, m_rand_rand = {0, 0, 0, 0};
    int v_rnd_f[n10], v_rnd_r[n10];
    memcpy(v_rnd_f, aleatorio, sizeof(aleatorio));
    memcpy(v_rnd_r, aleatorio, sizeof(aleatorio));
    quick_sort(v_rnd_f, n10, &m_fixo_rand, 0);
    quick_sort(v_rnd_r, n10, &m_rand_rand, 1);

    printf("\nVetor Aleatorio (10 elems):\n");
    printf("  Pivo Fixo             -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", m_fixo_rand.comparacoes, m_fixo_rand.trocas, m_fixo_rand.particoes);
    printf("  Pivo Aleatorio        -> Comp: %ld, Trocas: %ld, Particoes: %ld\n", m_rand_rand.comparacoes, m_rand_rand.trocas, m_rand_rand.particoes);
}

int particionar_jogadores(Jogador v[], int lo, int hi, MetricasQuick *m) {
    Jogador pivo = v[hi];
    int i = lo;

    m->particoes++;
    for (int j = lo; j < hi; j++) {
        m->comparacoes++;
        if (comparar_jogadores(&v[j], &pivo) < 0) {
            if (i != j) {
                trocar_jogador(&v[i], &v[j]);
                m->trocas++;
            }
            i++;
        }
    }
    if (i != hi) {
        trocar_jogador(&v[i], &v[hi]);
        m->trocas++;
    }
    return i;
}

void quick_sort_jogadores_rec(Jogador v[], int lo, int hi, MetricasQuick *m) {
    if (lo >= hi) return;
    int p = particionar_jogadores(v, lo, hi, m);
    if (p - 1 > lo) quick_sort_jogadores_rec(v, lo, p - 1, m);
    if (p + 1 < hi) quick_sort_jogadores_rec(v, p + 1, hi, m);
}

void demo_quick_jogadores(void) {
    Jogador catalogo[] = {
        {7, "Ana",    850},
        {5, "Bruno",  620},
        {9, "Carla",  910},
        {3, "Ana",    430},
        {2, "Bruno",  700},
        {8, "Diego",  560},
        {1, "Bia",    990},
    };
    int n = sizeof(catalogo) / sizeof(catalogo[0]);

    Jogador trabalho[n], original[n];
    memcpy(trabalho, catalogo, sizeof(catalogo));
    memcpy(original, catalogo, sizeof(catalogo));

    MetricasQuick m = {0, 0, 0, 0};
    printf("\n=== Quick Sort: jogadores por nome ===\n");
    imprimir_jogadores("Antes:", trabalho, n);
    quick_sort_jogadores_rec(trabalho, 0, n - 1, &m);
    imprimir_jogadores("Depois:", trabalho, n);

    printf("\nComparacoes: %ld\n", m.comparacoes);
    printf("Trocas: %ld\n", m.trocas);
    printf("Ordenado (nome, depois id): %s\n",
           jogadores_ordenados(trabalho, n) ? "sim" : "nao");
    printf("Id, nome e pontuacao continuam associados: %s\n",
           associacoes_preservadas(original, trabalho, n) ? "sim" : "nao");
}

/* ========================================================= */
/*  PARTE 6: RELATÓRIO COMPARATIVO DE MÉTRICAS               */
/* ========================================================= */

void executar_relatorio_para(const char *titulo, const int entrada[], int n) {
    printf("\n========================================================\n");
    printf(" RELATÓRIO COMPARATIVO: %s (%d elementos)\n", titulo, n);
    printf("========================================================\n");
    printf("%-22s | %-12s | %-15s\n", "Algoritmo", "Comparações", "Trocas/Desloc.");
    printf("--------------------------------------------------------\n");

    /* 1. Selection Sort */
    {
        int v[n]; memcpy(v, entrada, sizeof(int)*n);
        Contadores c = {0, 0, 0, 0};
        selection_sort(v, n, &c);
        printf("%-22s | %-12ld | %-15ld (trocas)\n", "Selection Sort", c.comparacoes, c.trocas);
    }
    /* 2. Insertion Sort */
    {
        int v[n]; memcpy(v, entrada, sizeof(int)*n);
        Contadores c = {0, 0, 0, 0};
        insertion_sort(v, n, &c);
        printf("%-22s | %-12ld | %-15ld (desloc.)\n", "Insertion Sort", c.comparacoes, c.deslocamentos);
    }
    /* 3. Bubble Sort */
    {
        int v[n]; memcpy(v, entrada, sizeof(int)*n);
        MetricasBubble m = {0, 0, 0};
        bubble_sort(v, n, &m);
        printf("%-22s | %-12ld | %-15ld (trocas)\n", "Bubble Sort (Otimizado)", m.comparacoes, m.trocas);
    }
    /* 4. Merge Sort */
    {
        int v[n]; memcpy(v, entrada, sizeof(int)*n);
        long comp = 0;
        merge_sort_generico(v, n, sizeof(int), cmp_int_ctx, NULL, &comp);
        printf("%-22s | %-12ld | %-15s\n", "Merge Sort", comp, "N/A (Auxiliar)");
    }
    /* 5. Quick Sort Fixo */
    {
        int v[n]; memcpy(v, entrada, sizeof(int)*n);
        MetricasQuick m = {0, 0, 0, 0};
        quick_sort(v, n, &m, 0);
        printf("%-22s | %-12ld | %-15ld (trocas)\n", "Quick Sort (Pivô Fixo)", m.comparacoes, m.trocas);
    }
    /* 6. Quick Sort Aleatório */
    {
        int v[n]; memcpy(v, entrada, sizeof(int)*n);
        MetricasQuick m = {0, 0, 0, 0};
        quick_sort(v, n, &m, 1);
        printf("%-22s | %-12ld | %-15ld (trocas)\n", "Quick Sort (Pivô Rand)", m.comparacoes, m.trocas);
    }
    printf("--------------------------------------------------------\n");
}

void demo_relatorio_comparativo(void) {
    int opcao1[]    = {7, 3, 9, 1, 5, 8, 2};
    int ordenado[]  = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    int inverso[]   = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    int aleatorio[] = {6, 2, 9, 4, 10, 1, 8, 3, 7, 5};

    executar_relatorio_para("Entrada Principal (7 números)", opcao1, 7);
    executar_relatorio_para("Vetor Já Ordenado (10 números)", ordenado, 10);
    executar_relatorio_para("Vetor Ordem Inversa (10 números)", inverso, 10);
    executar_relatorio_para("Vetor Aleatório (10 números)", aleatorio, 10);
}

/* ========================================================= */
/*  MAIN CENTRAL                                             */
/* ========================================================= */

int main(void) {
    srand((unsigned int)time(NULL));
    int opcao;
    do {
        printf("\n===== CENTRAL DE ORDENAÇÃO (CAPÍTULO 6) =====\n");
        printf("1 - Insertion Sort em inteiros\n");
        printf("2 - Ordenar jogadores por nome (Insertion Sort)\n");
        printf("3 - Bubble Sort com parada antecipada\n");
        printf("4 - Selection Sort em inteiros\n");
        printf("5 - Selection Sort em jogadores por nome\n");
        printf("6 - Quick Sort com jogadores por nome\n");
        for (int i = 0; i < NUM_CRITERIOS; i++)
            printf("%d - Bubble Sort de jogadores: %s (%s)\n", 7 + i,
                   nome_campo(CRITERIOS[i].campo),
                   nome_direcao(CRITERIOS[i].direcao));
        printf("%d - Intercalacao de dois vetores ordenados\n", 7 + NUM_CRITERIOS);
        printf("%d - Divisao recursiva de intervalo (so divide)\n", 8 + NUM_CRITERIOS);
        printf("%d - Merge Sort de inteiros\n", 9 + NUM_CRITERIOS);
        printf("%d - Merge Sort de jogadores (todos os criterios)\n", 10 + NUM_CRITERIOS);
        printf("%d - Particao do Quick Sort (auxiliar x in-place)\n", 11 + NUM_CRITERIOS);
        printf("%d - Quick Sort: Pivô Fixo vs Pivô Aleatório\n", 12 + NUM_CRITERIOS);
        printf("%d - RELATÓRIO COMPARATIVO DE MÉTRICAS (Cópias idênticas)\n", 13 + NUM_CRITERIOS);
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1: demo_inteiros();  break;
            case 2: demo_jogadores(); break;
            case 3: demo_bubble();    break;
            case 4: demo_selection_inteiros(); break;
            case 5: demo_selection_jogadores(); break;
            case 6: demo_quick_jogadores(); break;
            case 7: demo_bubble_jogadores(CRITERIOS[0]); break;
            case 8: demo_bubble_jogadores(CRITERIOS[1]); break;
            case 9: demo_bubble_jogadores(CRITERIOS[2]); break;
            case 10: demo_bubble_jogadores(CRITERIOS[3]); break;
            case 11: demo_intercalacao(); break;
            case 12: demo_divisao(); break;
            case 13: demo_merge_inteiros(); break;
            case 14: demo_merge_jogadores(); break;
            case 15: demo_particoes(); break;
            case 16: demo_quick_sort(); break;
            case 17: demo_relatorio_comparativo(); break;
            case 0: printf("Ate mais!\n"); break;
            default: printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
    return 0;
}