#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define CAPACIDADE_INICIAL 2
#define VIDA_MAXIMA 100
#define PONTUACAO_MAXIMA 9999
#define POSICAO_MAXIMA 100
#define TAMANHO_NOME 30

typedef struct {
	int x;
	int y;
} Posicao;

typedef enum {
	GUERREIRO = 1,
	MAGO,
	ARQUEIRO
} Classe;

typedef struct {
	int id;
	char nome[TAMANHO_NOME];
	int vida;
	int pontuacao;
	Posicao posicao;
	Classe classe;
} Personagem;

typedef struct {
	Personagem *personagens;
	size_t quantidade;
	size_t capacidade;
} Equipe;

const char *nome_classe(Classe classe)
{
	switch (classe) {
	case GUERREIRO: return "Guerreiro";
	case MAGO: return "Mago";
	case ARQUEIRO: return "Arqueiro";
	default: return "Desconhecida";
	}
}

int ler_inteiro(const char *mensagem, int minimo, int maximo, int *valor)
{
	int entrada;
	for (;;) {
		printf("%s", mensagem);
		if (scanf("%d", &entrada) != 1) {
			if (feof(stdin)) return 0;
			printf("Entrada inválida. Digite um número.\n");
			int caractere;
			while ((caractere = getchar()) != '\n' && caractere != EOF) {
			}
			continue;
		}

		int caractere;
		while ((caractere = getchar()) != '\n' && caractere != EOF) {
			if (caractere != ' ' && caractere != '\t' && caractere != '\r') {
				while (caractere != '\n' && caractere != EOF) {
					caractere = getchar();
				}
				printf("Digite somente um número.\n");
				entrada = minimo - 1;
				break;
			}
		}
		if (entrada >= minimo && entrada <= maximo) {
			*valor = entrada;
			return 1;
		}
		printf("Valor deve estar entre %d e %d.\n", minimo, maximo);
	}
}

int ler_nome(char nome[TAMANHO_NOME])
{
	for (;;) {
		printf("Nome (até %d caracteres): ", TAMANHO_NOME - 1);
		if (fgets(nome, TAMANHO_NOME, stdin) == NULL) return 0;

		size_t i = 0;
		while (nome[i] != '\0' && nome[i] != '\n') i++;
		if (nome[i] == '\n') {
			nome[i] = '\0';
		} else if (i == TAMANHO_NOME - 1) {
			int caractere = getchar();
			if (caractere != '\n' && caractere != EOF) {
				while ((caractere = getchar()) != '\n' && caractere != EOF) {
				}
				printf("Nome muito longo; tente novamente.\n");
				continue;
			}
		}
		if (nome[0] == '\0') {
			printf("O nome não pode ficar vazio.\n");
			continue;
		}
		return 1;
	}
}

size_t buscar_personagem(const Equipe *equipe, int id)
{
	for (size_t i = 0; i < equipe->quantidade; i++) {
		if (equipe->personagens[i].id == id) return i;
	}
	return SIZE_MAX;
}

int adicionar_personagem(Equipe *equipe, Personagem personagem)
{
	if (equipe->quantidade == equipe->capacidade) {
		if (equipe->capacidade > SIZE_MAX / 2 / sizeof *equipe->personagens) return 0;
		size_t nova_capacidade = equipe->capacidade * 2;
		Personagem *temporario = realloc(equipe->personagens,
						 nova_capacidade * sizeof *equipe->personagens);
		if (temporario == NULL) return 0;
		for (size_t i = equipe->capacidade; i < nova_capacidade; i++) {
			temporario[i] = (Personagem){0};
		}
		equipe->personagens = temporario;
		equipe->capacidade = nova_capacidade;
	}
	equipe->personagens[equipe->quantidade++] = personagem;
	return 1;
}

void mostrar_personagem(Personagem personagem)
{
	printf("ID: %d | Nome: %s | Classe: %s | Vida: %d/%d | Pontuação: %d | Posição: (%d, %d)\n",
	       personagem.id, personagem.nome, nome_classe(personagem.classe),
	       personagem.vida, VIDA_MAXIMA, personagem.pontuacao,
	       personagem.posicao.x, personagem.posicao.y);
}

void listar_equipe(const Equipe *equipe)
{
	if (equipe->quantidade == 0) {
		printf("A equipe está vazia.\n");
		return;
	}
	for (size_t i = 0; i < equipe->quantidade; i++) {
		mostrar_personagem(equipe->personagens[i]);
	}
}

int cadastrar(Equipe *equipe)
{
	int id, vida, pontos, x, y, classe;
	char nome[TAMANHO_NOME];
	if (!ler_inteiro("ID: ", 1, 1000000, &id)) return 0;
	if (buscar_personagem(equipe, id) != SIZE_MAX) {
		printf("Esse ID já está cadastrado.\n");
		return 1;
	}
	if (!ler_nome(nome) ||
	    !ler_inteiro("Vida: ", 0, VIDA_MAXIMA, &vida) ||
	    !ler_inteiro("Pontuação: ", 0, PONTUACAO_MAXIMA, &pontos) ||
	    !ler_inteiro("Posição X: ", 0, POSICAO_MAXIMA, &x) ||
	    !ler_inteiro("Posição Y: ", 0, POSICAO_MAXIMA, &y) ||
	    !ler_inteiro("Classe (1-Guerreiro, 2-Mago, 3-Arqueiro): ", 1, 3, &classe)) return 0;

	Personagem novo = {
		.id = id,
		.nome = "",
		.vida = vida,
		.pontuacao = pontos,
		.posicao = {x, y},
		.classe = (Classe)classe
	};
	size_t i = 0;
	while (nome[i] != '\0') {
		novo.nome[i] = nome[i];
		i++;
	}
	novo.nome[i] = '\0';
	if (!adicionar_personagem(equipe, novo)) {
		printf("Não foi possível aumentar a equipe.\n");
	}
	return 1;
}

int consultar(const Equipe *equipe)
{
	int id;
	if (!ler_inteiro("ID para buscar: ", 1, 1000000, &id)) return 0;
	size_t indice = buscar_personagem(equipe, id);
	if (indice == SIZE_MAX) printf("Personagem não encontrado.\n");
	else mostrar_personagem(equipe->personagens[indice]);
	return 1;
}

int alterar(Equipe *equipe)
{
	int id, vida, pontos, x, y, classe;
	char nome[TAMANHO_NOME];
	if (!ler_inteiro("ID para alterar: ", 1, 1000000, &id)) return 0;
	size_t indice = buscar_personagem(equipe, id);
	if (indice == SIZE_MAX) {
		printf("Personagem não encontrado.\n");
		return 1;
	}
	if (!ler_nome(nome) ||
	    !ler_inteiro("Nova vida: ", 0, VIDA_MAXIMA, &vida) ||
	    !ler_inteiro("Nova pontuação: ", 0, PONTUACAO_MAXIMA, &pontos) ||
	    !ler_inteiro("Nova posição X: ", 0, POSICAO_MAXIMA, &x) ||
	    !ler_inteiro("Nova posição Y: ", 0, POSICAO_MAXIMA, &y) ||
	    !ler_inteiro("Nova classe (1-Guerreiro, 2-Mago, 3-Arqueiro): ", 1, 3, &classe)) return 0;

	Personagem *personagem = &equipe->personagens[indice];
	personagem->vida = vida;
	personagem->pontuacao = pontos;
	personagem->posicao = (Posicao){x, y};
	personagem->classe = (Classe)classe;
	size_t i = 0;
	while (nome[i] != '\0') {
		personagem->nome[i] = nome[i];
		i++;
	}
	personagem->nome[i] = '\0';
	printf("Personagem alterado.\n");
	return 1;
}

void intercalar(Personagem valores[], Personagem auxiliar[], size_t inicio,
		size_t meio, size_t fim, size_t *comparacoes)
{
	size_t i = inicio, j = meio, k = inicio;
	while (i < meio && j < fim) {
		(*comparacoes)++;
		if (valores[i].pontuacao <= valores[j].pontuacao) auxiliar[k++] = valores[i++];
		else auxiliar[k++] = valores[j++];
	}
	while (i < meio) auxiliar[k++] = valores[i++];
	while (j < fim) auxiliar[k++] = valores[j++];
	for (i = inicio; i < fim; i++) valores[i] = auxiliar[i];
}

void ordenar(Personagem valores[], Personagem auxiliar[], size_t inicio,
	      size_t fim, size_t *comparacoes)
{
	if (fim - inicio < 2) return;
	size_t meio = inicio + (fim - inicio) / 2;
	ordenar(valores, auxiliar, inicio, meio, comparacoes);
	ordenar(valores, auxiliar, meio, fim, comparacoes);
	intercalar(valores, auxiliar, inicio, meio, fim, comparacoes);
}

int ordenar_equipe(Equipe *equipe, size_t *comparacoes)
{
	*comparacoes = 0;
	if (equipe->quantidade < 2) return 1;
	Personagem *auxiliar = malloc(equipe->quantidade * sizeof *auxiliar);
	if (auxiliar == NULL) return 0;
	ordenar(equipe->personagens, auxiliar, 0, equipe->quantidade, comparacoes);
	free(auxiliar);
	return 1;
}

int main(void)
{
	Equipe equipe = {0};
	equipe.capacidade = CAPACIDADE_INICIAL;
	equipe.personagens = calloc(equipe.capacidade, sizeof *equipe.personagens);
	if (equipe.personagens == NULL) {
		printf("Erro ao reservar memória para a equipe.\n");
		return EXIT_FAILURE;
	}

	int opcao;
	int continuar = 1;
	while (continuar) {
		printf("\n=== CATÁLOGO DE PERSONAGENS ===\n"
		       "1. Cadastrar\n2. Buscar por ID\n3. Alterar\n"
		       "4. Listar equipe\n5. Ordenar por pontuação\n0. Sair\n");
		if (!ler_inteiro("Opção: ", 0, 5, &opcao)) break;
		switch (opcao) {
		case 1: continuar = cadastrar(&equipe); break;
		case 2: continuar = consultar(&equipe); break;
		case 3: continuar = alterar(&equipe); break;
		case 4:
			printf("Membros: %zu | Capacidade: %zu\n", equipe.quantidade, equipe.capacidade);
			listar_equipe(&equipe);
			break;
		case 5: {
			size_t comparacoes;
			if (ordenar_equipe(&equipe, &comparacoes))
				printf("Equipe ordenada. Comparações: %zu\n", comparacoes);
			else
				printf("Falha ao reservar memória para ordenação.\n");
			break;
		}
		case 0: continuar = 0; break;
		}
	}

	free(equipe.personagens);
	return EXIT_SUCCESS;
}
