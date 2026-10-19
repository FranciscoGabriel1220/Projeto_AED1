#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDADE_INICIAL 2
#define VIDA_MAXIMA 100
#define PONTUACAO_MAXIMA 9999
#define POSICAO_MAXIMA 100

typedef struct {
	int identificador;
	char nome[30];
	int vida;
	int pontuacao;
	int x;
	int y;
} Personagem;

int adicionar_personagem(Personagem **equipe, size_t *quantidade,
			 size_t *capacidade, Personagem personagem)
{
	if (*quantidade == *capacidade) {
		if (*capacidade > SIZE_MAX / 2 / sizeof **equipe) {
			return 0;
		}

		size_t nova_capacidade = *capacidade * 2;
		Personagem *temporario = realloc(*equipe,
						 nova_capacidade * sizeof **equipe);
		if (temporario == NULL) {
			return 0;
		}
		*equipe = temporario;
		*capacidade = nova_capacidade;
	}

	(*equipe)[*quantidade] = personagem;
	(*quantidade)++;
	return 1;
}

void mostrar_equipe(const Personagem equipe[], size_t quantidade)
{
	for (size_t i = 0; i < quantidade; i++) {
		printf("ID: %d | Nome: %s | Vida: %d | Pontuação: %d | Posição: (%d, %d)\n",
		       equipe[i].identificador, equipe[i].nome, equipe[i].vida,
		       equipe[i].pontuacao, equipe[i].x, equipe[i].y);
	}
}

void intercalar_vetores_ordenados(const int esquerda[], size_t quantidade_esquerda,
				  const int direita[], size_t quantidade_direita,
				  int resultado[], size_t *comparacoes)
{
	size_t i = 0;
	size_t j = 0;
	size_t k = 0;

	while (i < quantidade_esquerda && j < quantidade_direita) {
		(*comparacoes)++;
		if (esquerda[i] <= direita[j]) {
			resultado[k++] = esquerda[i++];
		} else {
			resultado[k++] = direita[j++];
		}
	}

	while (i < quantidade_esquerda) {
		resultado[k++] = esquerda[i++];
	}
	while (j < quantidade_direita) {
		resultado[k++] = direita[j++];
	}
}

void intercalar(int valores[], int auxiliar[], size_t inicio,
		size_t meio, size_t fim, size_t *comparacoes)
{
	size_t indice_esquerda = inicio;
	size_t indice_direita = meio;
	size_t indice_saida = inicio;

	while (indice_esquerda < meio && indice_direita < fim) {
		(*comparacoes)++;
		if (valores[indice_esquerda] <= valores[indice_direita]) {
			auxiliar[indice_saida++] = valores[indice_esquerda++];
		} else {
			auxiliar[indice_saida++] = valores[indice_direita++];
		}
	}

	while (indice_esquerda < meio) {
		auxiliar[indice_saida++] = valores[indice_esquerda++];
	}
	while (indice_direita < fim) {
		auxiliar[indice_saida++] = valores[indice_direita++];
	}

	for (size_t i = inicio; i < fim; i++) {
		valores[i] = auxiliar[i];
	}
}

void ordenar_recursivamente(int valores[], int auxiliar[], size_t inicio,
			    size_t fim, size_t *comparacoes)
{
	if (fim - inicio < 2) {
		return;
	}

	/* DIVIDIR: separa o intervalo em duas metades sem risco de overflow. */
	size_t meio = inicio + (fim - inicio) / 2;

	/* RESOLVER: ordena recursivamente cada metade. */
	ordenar_recursivamente(valores, auxiliar, inicio, meio, comparacoes);
	ordenar_recursivamente(valores, auxiliar, meio, fim, comparacoes);

	/* COMBINAR: intercala as duas metades já ordenadas. */
	intercalar(valores, auxiliar, inicio, meio, fim, comparacoes);
}

int main(void)
{
	size_t capacidade = CAPACIDADE_INICIAL;
	size_t quantidade = 0;
	Personagem *equipe = calloc(capacidade, sizeof *equipe);
	if (equipe == NULL) {
		fprintf(stderr, "Erro ao reservar memória para a equipe.\n");
		return EXIT_FAILURE;
	}

	Personagem exemplos[] = {
		{1, "Luna", 80, 250, 40, 30},
		{2, "Ravi", 90, 120, 20, 15},
		{3, "Mika", 70, 480, 10, 25},
		{4, "Noa", 100, 310, 50, 45}
	};
	size_t quantidade_exemplos = sizeof exemplos / sizeof exemplos[0];

	for (size_t i = 0; i < quantidade_exemplos; i++) {
		if (!adicionar_personagem(&equipe, &quantidade, &capacidade, exemplos[i])) {
			fprintf(stderr, "Erro ao aumentar a equipe.\n");
			free(equipe);
			return EXIT_FAILURE;
		}
	}

	printf("Equipe: %zu personagem(ns), capacidade: %zu\n", quantidade, capacidade);
	mostrar_equipe(equipe, quantidade);

	/* Intercala dois vetores já ordenados em um terceiro vetor. */
	const int pontuacoes_a[] = {120, 250};
	const int pontuacoes_b[] = {310, 480};
	const size_t tamanho_a = sizeof pontuacoes_a / sizeof pontuacoes_a[0];
	const size_t tamanho_b = sizeof pontuacoes_b / sizeof pontuacoes_b[0];
	int pontuacoes_intercaladas[sizeof pontuacoes_a / sizeof pontuacoes_a[0] +
				   sizeof pontuacoes_b / sizeof pontuacoes_b[0]];
	size_t comparacoes_intercalacao = 0;
	intercalar_vetores_ordenados(pontuacoes_a, tamanho_a, pontuacoes_b, tamanho_b,
				     pontuacoes_intercaladas, &comparacoes_intercalacao);

	printf("\nResultado da intercalação dos dois vetores:\n");
	for (size_t i = 0; i < tamanho_a + tamanho_b; i++) {
		printf("%d%s", pontuacoes_intercaladas[i],
		       i + 1 == tamanho_a + tamanho_b ? "\n" : " ");
	}
	printf("Comparações na intercalação: %zu\n", comparacoes_intercalacao);

	int *pontuacoes = malloc(quantidade * sizeof *pontuacoes);
	int *auxiliar = malloc(quantidade * sizeof *auxiliar);
	if (pontuacoes == NULL || auxiliar == NULL) {
		fprintf(stderr, "Erro ao reservar memória para ordenar pontuações.\n");
		free(auxiliar);
		free(pontuacoes);
		free(equipe);
		return EXIT_FAILURE;
	}

	for (size_t i = 0; i < quantidade; i++) {
		pontuacoes[i] = equipe[i].pontuacao;
	}

	size_t comparacoes = 0;
	ordenar_recursivamente(pontuacoes, auxiliar, 0, quantidade, &comparacoes);

	printf("\nPontuações em ordem crescente:\n");
	for (size_t i = 0; i < quantidade; i++) {
		printf("%d%s", pontuacoes[i], i + 1 == quantidade ? "\n" : " ");
	}
	printf("Comparações feitas durante a intercalação: %zu\n", comparacoes);

	free(auxiliar);
	free(pontuacoes);
	free(equipe);
	return EXIT_SUCCESS;
}
