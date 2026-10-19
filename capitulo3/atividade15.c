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

void mostrar_personagem(Personagem personagem)
{
	printf("ID: %d\nNome: %s\nVida: %d/%d\nPontuação: %d\nPosição: (%d, %d)\n",
	       personagem.identificador, personagem.nome, personagem.vida,
	       VIDA_MAXIMA, personagem.pontuacao, personagem.x, personagem.y);
}

int main(void)
{
	size_t capacidade = CAPACIDADE_INICIAL;
	Personagem *personagens = calloc(capacidade, sizeof *personagens);
	if (personagens == NULL) {
		printf("Erro ao reservar memória.\n");
		return EXIT_FAILURE;
	}

	printf("Capacidade inicial: %zu\n", capacidade);
	printf("Registro vazio (zerado pelo calloc): ID=%d, vida=%d, pontuação=%d, posição=(%d, %d)\n\n",
	       personagens[1].identificador, personagens[1].vida,
	       personagens[1].pontuacao, personagens[1].x, personagens[1].y);

	personagens[0] = (Personagem){1, "Luna", 80, 250, 40, 30};

	printf("Estado antes das alterações:\n");
	mostrar_personagem(personagens[0]);

	/* Atualiza vida, pontuação e posição sem sair dos limites definidos. */
	personagens[0].vida -= 20;
	if (personagens[0].vida < 0) {
		personagens[0].vida = 0;
	}

	personagens[0].pontuacao += 100;
	if (personagens[0].pontuacao > PONTUACAO_MAXIMA) {
		personagens[0].pontuacao = PONTUACAO_MAXIMA;
	}

	personagens[0].x += 5;
	personagens[0].y += 3;
	if (personagens[0].x > POSICAO_MAXIMA) {
		personagens[0].x = POSICAO_MAXIMA;
	}
	if (personagens[0].y > POSICAO_MAXIMA) {
		personagens[0].y = POSICAO_MAXIMA;
	}

	printf("\nEstado depois das alterações:\n");
	mostrar_personagem(personagens[0]);

	size_t capacidade_anterior = capacidade;
	size_t nova_capacidade;
	printf("\nInforme a nova capacidade: ");
	if (scanf("%zu", &nova_capacidade) != 1 || nova_capacidade == 0 ||
	    nova_capacidade > SIZE_MAX / sizeof *personagens) {
		printf("Capacidade inválida.\n");
		free(personagens);
		return EXIT_FAILURE;
	}

	Personagem *temporario = realloc(personagens, nova_capacidade * sizeof *personagens);
	if (temporario == NULL) {
		printf("Erro ao redimensionar; o vetor original continua válido.\n");
		free(personagens);
		return EXIT_FAILURE;
	}
	personagens = temporario;

	for (size_t i = capacidade_anterior; i < nova_capacidade; i++) {
		personagens[i] = (Personagem){0};
	}
	capacidade = nova_capacidade;
	printf("Capacidade anterior: %zu\nNova capacidade: %zu\n",
	       capacidade_anterior, capacidade);

	free(personagens);
	return EXIT_SUCCESS;
}
