#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

Personagem criar_personagem(int identificador, const char *nome, int vida,
			    int pontuacao, int x, int y)
{
	Personagem personagem = {
		.identificador = identificador,
		.nome = "",
		.vida = vida,
		.pontuacao = pontuacao,
		.x = x,
		.y = y
	};
	snprintf(personagem.nome, sizeof personagem.nome, "%s", nome);
	return personagem;
}

int alterar_nome(Personagem *personagem)
{
	char novo_nome[sizeof personagem->nome];
	printf("Digite o novo nome (máximo %zu caracteres): ", sizeof novo_nome - 1);
	if (fgets(novo_nome, sizeof novo_nome, stdin) == NULL) {
		return 0;
	}

	size_t tamanho = strcspn(novo_nome, "\n");
	if (novo_nome[tamanho] == '\n') {
		novo_nome[tamanho] = '\0';
	} else {
		int caractere = getchar();
		if (caractere != '\n' && caractere != EOF) {
			while ((caractere = getchar()) != '\n' && caractere != EOF) {
			}
			printf("Nome muito longo; o nome não foi alterado.\n");
			return 0;
		}
	}

	if (novo_nome[0] == '\0') {
		printf("O nome não pode ficar vazio.\n");
		return 0;
	}

	snprintf(personagem->nome, sizeof personagem->nome, "%s", novo_nome);
	return 1;
}

/*
 * Passar por valor copia a estrutura inteira, inclusive nome[30]. Isso tem
 * um pequeno custo de cópia, mas as consultas não alteram o registro original.
 */
void mostrar_personagem(Personagem personagem)
{
	printf("ID: %d\nNome: %s\nVida: %d/%d\nPontuação: %d\nPosição: (%d, %d)\n",
	       personagem.identificador, personagem.nome, personagem.vida,
	       VIDA_MAXIMA, personagem.pontuacao, personagem.x, personagem.y);
}

int calcular_pontuacao_total(Personagem personagem, int bonus)
{
	/* A cópia também é feita aqui; só calculamos e devolvemos um resultado. */
	return personagem.pontuacao + bonus;
}

void demonstrar_copia(Personagem personagem)
{
	/* Esta alteração afeta apenas a cópia local, não o registro de main. */
	personagem.vida = 0;
	printf("Vida dentro da função após alterar a cópia: %d\n", personagem.vida);
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

	personagens[0] = criar_personagem(1, "Luna", 80, 250, 40, 30);

	printf("Estado antes das alterações:\n");
	mostrar_personagem(personagens[0]);
	printf("Pontuação com bônus de 50 (consulta): %d\n",
	       calcular_pontuacao_total(personagens[0], 50));
	demonstrar_copia(personagens[0]);
	printf("Vida no registro original após a função: %d\n\n", personagens[0].vida);

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

	alterar_nome(&personagens[0]);
	printf("\nRegistro completo depois das alterações:\n");
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
