#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_NOME 50

typedef struct {
    long comparacoes;
    long deslocamentos;
} Contadores;

typedef struct {
    int id;
    char nome[TAM_NOME];
    int pontuacao;
} Jogador;

/* ========================================================= */
/*  PARTE 1: Insertion Sort em vetor de inteiros              */
/* ========================================================= */

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

void imprimir_inteiros(const char *rotulo, const int v[], int n) {
    printf("%s: ", rotulo);
    for (int i = 0; i < n; i++) printf("%d ", v[i]);
    printf("\n");
}

void demo_inteiros(void) {
    int entrada[] = {7, 3, 9, 1, 5, 8, 2};   /* substitua pela sua entrada */
    int n = sizeof(entrada) / sizeof(entrada[0]);

    int v[n], referencia[n];
    memcpy(v, entrada, sizeof(entrada));           /* cópia de trabalho */
    memcpy(referencia, entrada, sizeof(entrada));  /* cópia para validar */

    Contadores c = {0, 0};
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
/*  PARTE 2: Insertion Sort em jogadores, por nome            */
/* ========================================================= */

/*
 * Critério de ordenação:
 *   1) nome, com strcmp (ordem alfabética);
 *   2) se os nomes forem iguais, menor id primeiro.
 * Como o id é único, a ordem é total e o resultado é sempre o mesmo.
 */
int comparar_jogadores(const Jogador *a, const Jogador *b) {
    int r = strcmp(a->nome, b->nome);
    if (r != 0) return r;
    return (a->id > b->id) - (a->id < b->id);
}

void insertion_sort_jogadores(Jogador v[], int n, Contadores *c) {
    for (int i = 1; i < n; i++) {
        Jogador chave = v[i];      /* estrutura temporária completa */
        int j = i - 1;

        while (j >= 0) {
            c->comparacoes++;
            if (comparar_jogadores(&v[j], &chave) > 0) {
                v[j + 1] = v[j];   /* move todos os campos juntos */
                c->deslocamentos++;
                j--;
            } else {
                break;
            }
        }
        v[j + 1] = chave;
    }
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

/* Cada id do resultado deve manter o mesmo nome e a mesma pontuação do original. */
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

void demo_jogadores(void) {
    /* Nomes repetidos de propósito: Ana (ids 7 e 3) e Bruno (ids 5 e 2) */
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
    memcpy(trabalho, catalogo, sizeof(catalogo));   /* cópia de trabalho */
    memcpy(original, catalogo, sizeof(catalogo));   /* cópia para validar */

    Contadores c = {0, 0};
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
/*  PARTE 3: Bubble Sort com parada antecipada                */
/* ========================================================= */

typedef struct {
    long comparacoes;
    long trocas;
    long passagens;
} MetricasBubble;

void bubble_sort(int v[], int n, MetricasBubble *m) {
    int limite = n - 1;   /* faixa útil: compara os pares (j, j+1) com j < limite */
    int trocou = 1;       /* indicador de troca da passagem */

    while (trocou && limite > 0) {
        trocou = 0;       /* zera o indicador a cada passagem */
        m->passagens++;

        for (int j = 0; j < limite; j++) {
            m->comparacoes++;                 /* compara vizinhos */
            if (v[j] > v[j + 1]) {
                int tmp = v[j];
                v[j] = v[j + 1];
                v[j + 1] = tmp;
                m->trocas++;
                trocou = 1;
            }
        }
        limite--;         /* o maior da faixa já foi para o fim: reduz a faixa útil */
    }
}

void testar_bubble(const char *rotulo, const int entrada[], int n) {
    int v[n], referencia[n];
    memcpy(v, entrada, sizeof(int) * n);          /* cópia de trabalho */
    memcpy(referencia, entrada, sizeof(int) * n); /* cópia para validar */

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
    int aleatorio[] = {6, 2, 9, 4, 10, 1, 8, 3, 7, 5};   /* substitua se quiser */
    int n = sizeof(ordenado) / sizeof(ordenado[0]);

    printf("\n=== Bubble Sort com parada antecipada ===");
    testar_bubble("Vetor ja ordenado", ordenado, n);
    testar_bubble("Vetor em ordem inversa", inverso, n);
    testar_bubble("Vetor aleatorio", aleatorio, n);
}

/* ========================================================= */
/*  PARTE 4: Bubble Sort em jogadores (campo + direcao)       */
/* ========================================================= */

typedef enum { CAMPO_ID, CAMPO_PONTUACAO } Campo;
typedef enum { CRESCENTE, DECRESCENTE } Direcao;

typedef struct {
    Campo campo;
    Direcao direcao;
} Criterio;

/* Criterios oferecidos na central (opcoes 4, 5, 6 e 7 do menu) */
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

/*
 * Regra de comparacao (separada da logica de ordenacao).
 * Retorna < 0, 0 ou > 0. Na direcao decrescente, o sinal e invertido.
 */
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

/* Logica de ordenacao: so pergunta "a deve vir depois de b?", sem conhecer o campo. */
void bubble_sort_jogadores(Jogador v[], int n, Criterio crit, MetricasBubble *m) {
    int limite = n - 1;
    int trocou = 1;

    while (trocou && limite > 0) {
        trocou = 0;
        m->passagens++;

        for (int j = 0; j < limite; j++) {
            m->comparacoes++;
            if (comparar_jogadores_por(&v[j], &v[j + 1], crit) > 0) {
                Jogador tmp = v[j];      /* troca o registro inteiro: */
                v[j] = v[j + 1];         /* id, nome e pontuacao vao juntos */
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
    /* Bruno (ids 5 e 2) empata em 700 pontos de proposito */
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
    memcpy(trabalho, catalogo, sizeof(catalogo));   /* cópia de trabalho */
    memcpy(original, catalogo, sizeof(catalogo));   /* cópia para validar */

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
/*  CENTRAL                                                   */
/* ========================================================= */

int main(void) {
    int opcao;
    do {
        printf("\n===== CENTRAL =====\n");
        printf("1 - Insertion Sort em inteiros\n");
        printf("2 - Ordenar jogadores por nome\n");
        printf("3 - Bubble Sort com parada antecipada\n");
        for (int i = 0; i < NUM_CRITERIOS; i++)
            printf("%d - Bubble Sort de jogadores: %s (%s)\n", 4 + i,
                   nome_campo(CRITERIOS[i].campo),
                   nome_direcao(CRITERIOS[i].direcao));
        printf("0 - Sair\n");
        printf("Opcao: ");
        if (scanf("%d", &opcao) != 1) break;

        switch (opcao) {
            case 1: demo_inteiros();  break;
            case 2: demo_jogadores(); break;
            case 3: demo_bubble();    break;
            case 0: printf("Ate mais!\n"); break;
            default:
                if (opcao >= 4 && opcao < 4 + NUM_CRITERIOS)
                    demo_bubble_jogadores(CRITERIOS[opcao - 4]);
                else
                    printf("Opcao invalida.\n");
        }
    } while (opcao != 0);
    return 0;
}