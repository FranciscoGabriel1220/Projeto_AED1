#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDADE_INICIAL 5

static void mostrar_vetor(const int *vetor, size_t capacidade)
{
	for (size_t i = 0; i < capacidade; i++) {
		printf("Posição %zu: %d\n", i, vetor[i]);
	}
}

int main(void)
{
	size_t capacidade = CAPACIDADE_INICIAL;
	int *registros = calloc(capacidade, sizeof *registros);

	if (registros == NULL) {
		fprintf(stderr, "Erro: não foi possível reservar a memória inicial.\n");
		return EXIT_FAILURE;
	}

	printf("Vetor criado com capacidade %zu. Valores iniciais:\n", capacidade);
	mostrar_vetor(registros, capacidade);

	printf("\nInforme a nova capacidade (maior para aumentar, menor para reduzir): ");
	size_t nova_capacidade;
	if (scanf("%zu", &nova_capacidade) != 1 ||
		nova_capacidade == 0 ||
		nova_capacidade > SIZE_MAX / sizeof *registros) {
		fprintf(stderr, "Capacidade inválida. Ela deve ser positiva e possível de alocar.\n");
		free(registros);
		return EXIT_FAILURE;
	}

	size_t capacidade_anterior = capacidade;
	int *temporario = realloc(registros, nova_capacidade * sizeof *registros);
	if (temporario == NULL) {
		fprintf(stderr, "Erro: não foi possível redimensionar o vetor.\n");
		/* O bloco original continua válido quando realloc falha. */
		free(registros);
		return EXIT_FAILURE;
	}

	registros = temporario;
	if (nova_capacidade > capacidade_anterior) {
		for (size_t i = capacidade_anterior; i < nova_capacidade; i++) {
			registros[i] = 0;
		}
	}
	capacidade = nova_capacidade;

	printf("\nCapacidade anterior: %zu\nNova capacidade: %zu\n",
		   capacidade_anterior, capacidade);
	printf("Valores após o redimensionamento:\n");
	mostrar_vetor(registros, capacidade);

	free(registros);
	return EXIT_SUCCESS;
}
