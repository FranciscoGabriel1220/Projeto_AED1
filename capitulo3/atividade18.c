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

int definir_vida(Personagem *personagem, int vida)
{
	if (personagem == NULL || vida < 0 || vida > VIDA_MAXIMA) {
		return 0;
	}

	/* (*personagem).vida equivale a personagem->vida. */
	(*personagem).vida = vida;
	return 1;
}

int mover_personagem(Personagem *personagem, int deslocamento_x, int deslocamento_y)
{
	if (personagem == NULL) {
		return 0;
	}

	long long novo_x = (long long)personagem->x + deslocamento_x;
	long long novo_y = (long long)personagem->y + deslocamento_y;
	if (novo_x < 0 || novo_x > POSICAO_MAXIMA ||
	    novo_y < 0 || novo_y > POSICAO_MAXIMA) {
		return 0;
	}

	personagem->x = (int)novo_x;
	personagem->y = (int)novo_y;
	return 1;
}

int adicionar_pontuacao(Personagem *personagem, int pontos)
{
	if (personagem == NULL || pontos < 0 ||
	    personagem->pontuacao < 0 || personagem->pontuacao > PONTUACAO_MAXIMA ||
	    pontos > PONTUACAO_MAXIMA - personagem->pontuacao) {
		return 0;
	}

	personagem->pontuacao += pontos;
	return 1;
}

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

	/* As funções recebem o endereço para modificar o registro armazenado. */
	if (!definir_vida(&personagens[0], 60) ||
	    !mover_personagem(&personagens[0], 5, 3) ||
	    !adicionar_pontuacao(&personagens[0], 100)) {
		printf("Uma alteração foi rejeitada por estar fora dos limites.\n");
		free(personagens);
		return EXIT_FAILURE;
	}

	/* NULL é rejeitado sem tentar acessar memória inválida. */
	if (!definir_vida(NULL, 60)) {
		printf("Ponteiro nulo rejeitado com segurança.\n");
	}

	alterar_nome(&personagens[0]);
	printf("\nRegistro completo depois do retorno das funções:\n");
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
