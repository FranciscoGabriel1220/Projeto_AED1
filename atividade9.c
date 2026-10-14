#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define NOME_CAPACIDADE 16
#define APELIDO_CAPACIDADE 16
#define EQUIPE_CAPACIDADE 16
#define SENHA_CAPACIDADE 8
#define QUANTIDADE_JOGADORES 5
#define LINHAS_MAPA 5
#define COLUNAS_MAPA 5
#define POSICAO_LIVRE -1
#define SEPARADOR " - "
#define EXIBICAO_CAPACIDADE (APELIDO_CAPACIDADE + EQUIPE_CAPACIDADE + sizeof(SEPARADOR))

/* Lê uma linha, remove o '\n' e rejeita entradas maiores que o vetor. */
int lerTexto(const char *prompt, char texto[], size_t capacidade)
{
    printf("%s", prompt);

    if (fgets(texto, capacidade, stdin) == NULL) {
        return 0;
    }

    size_t tamanho = strcspn(texto, "\n");
    if (texto[tamanho] == '\n') {
        texto[tamanho] = '\0';
    } else {
        int caractere = getchar();
        if (caractere != '\n' && caractere != EOF) {
            while ((caractere = getchar()) != '\n' && caractere != EOF) {
                /* Descarta os caracteres restantes da linha. */
            }
            return 0;
        }
    }

    return 1;
}

/* Copia apelido e concatena separador e equipe somente após validar os espaços. */
int montarNomeExibicao(char destino[], size_t capacidade,
                       const char apelido[], const char equipe[])
{
    size_t tamanhoApelido = strlen(apelido);
    size_t tamanhoSeparador = strlen(SEPARADOR);
    size_t tamanhoEquipe = strlen(equipe);

    if (tamanhoApelido + 1 > capacidade) {
        return 0;
    }
    strcpy(destino, apelido);

    size_t tamanhoAtual = strlen(destino);
    if (tamanhoAtual + tamanhoSeparador + 1 > capacidade) {
        return 0;
    }
    strcat(destino, SEPARADOR);

    tamanhoAtual = strlen(destino);
    if (tamanhoAtual + tamanhoEquipe + 1 > capacidade) {
        return 0;
    }
    strcat(destino, equipe);
    return 1;
}

int coordenadaValida(int linha, int coluna)
{
    return linha >= 0 && linha < LINHAS_MAPA &&
           coluna >= 0 && coluna < COLUNAS_MAPA;
}

void inicializarMapa(int mapa[LINHAS_MAPA][COLUNAS_MAPA])
{
    for (int linha = 0; linha < LINHAS_MAPA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_MAPA; coluna++) {
            mapa[linha][coluna] = POSICAO_LIVRE;
        }
    }
}

int posicionarJogador(int mapa[LINHAS_MAPA][COLUNAS_MAPA],
                      int jogador, size_t quantidadeJogadores,
                      int linha, int coluna)
{
    if (!coordenadaValida(linha, coluna) || jogador < 0 ||
        (size_t)jogador >= quantidadeJogadores ||
        mapa[linha][coluna] != POSICAO_LIVRE) {
        return 0;
    }

    for (int l = 0; l < LINHAS_MAPA; l++) {
        for (int c = 0; c < COLUNAS_MAPA; c++) {
            if (mapa[l][c] == jogador) {
                return 0;
            }
        }
    }

    mapa[linha][coluna] = jogador;
    return 1;
}

int removerPosicao(int mapa[LINHAS_MAPA][COLUNAS_MAPA],
                   int linha, int coluna, int *jogadorRemovido)
{
    if (!coordenadaValida(linha, coluna) ||
        mapa[linha][coluna] == POSICAO_LIVRE) {
        return 0;
    }

    if (jogadorRemovido != NULL) {
        *jogadorRemovido = mapa[linha][coluna];
    }
    mapa[linha][coluna] = POSICAO_LIVRE;
    return 1;
}

int reposicionarJogador(int mapa[LINHAS_MAPA][COLUNAS_MAPA],
                        int jogador, size_t quantidadeJogadores,
                        int novaLinha, int novaColuna)
{
    if (!coordenadaValida(novaLinha, novaColuna) || jogador < 0 ||
        (size_t)jogador >= quantidadeJogadores ||
        (mapa[novaLinha][novaColuna] != POSICAO_LIVRE &&
         mapa[novaLinha][novaColuna] != jogador)) {
        return 0;
    }

    for (int linha = 0; linha < LINHAS_MAPA; linha++) {
        for (int coluna = 0; coluna < COLUNAS_MAPA; coluna++) {
            if (mapa[linha][coluna] == jogador) {
                mapa[linha][coluna] = POSICAO_LIVRE;
            }
        }
    }

    mapa[novaLinha][novaColuna] = jogador;
    return 1;
}

void exibirMapa(int mapa[LINHAS_MAPA][COLUNAS_MAPA])
{
    printf("\nMapa da equipe ( . = livre; números = jogadores)\n");
    printf("    ");
    for (int coluna = 0; coluna < COLUNAS_MAPA; coluna++) {
        printf(" %2d ", coluna);
    }
    printf("\n");

    for (int linha = 0; linha < LINHAS_MAPA; linha++) {
        printf("%2d: ", linha);
        for (int coluna = 0; coluna < COLUNAS_MAPA; coluna++) {
            if (mapa[linha][coluna] == POSICAO_LIVRE) {
                printf("  . ");
            } else {
                printf(" %2d ", mapa[linha][coluna] + 1);
            }
        }
        printf("\n");
    }
}

int lerInteiro(const char *prompt, int *valor)
{
    char entrada[32];
    if (!lerTexto(prompt, entrada, sizeof entrada)) {
        return 0;
    }

    char *fim;
    long convertido = strtol(entrada, &fim, 10);
    if (fim == entrada) {
        return 0;
    }
    while (*fim == ' ' || *fim == '\t' || *fim == '\r') {
        fim++;
    }
    /* As opções e coordenadas deste programa ficam entre -1 e 100. */
    if (*fim != '\0' || convertido < -1 || convertido > 100) {
        return 0;
    }

    *valor = (int)convertido;
    return 1;
}

int main(void)
{
    char nomeJogador[NOME_CAPACIDADE];
    char apelidoJogador[APELIDO_CAPACIDADE];
    char equipeJogador[EQUIPE_CAPACIDADE];
    char senhaJogador[SENHA_CAPACIDADE];
    char confirmacaoSenha[SENHA_CAPACIDADE];
    char jogadores[QUANTIDADE_JOGADORES][EXIBICAO_CAPACIDADE];
    char apelidoOutro[APELIDO_CAPACIDADE];
    char equipeOutra[EQUIPE_CAPACIDADE];
    char busca[EXIBICAO_CAPACIDADE];
    int mapa[LINHAS_MAPA][COLUNAS_MAPA];
    size_t quantidadeJogadores = 0;

    inicializarMapa(mapa);

    printf("** TELA DE CADASTRO **\n");

    if (!lerTexto("Qual é o seu nome? ", nomeJogador, sizeof nomeJogador)) {
        printf("Nome inválido: informe no máximo 15 caracteres.\n");
        return 1;
    }
    if (strlen(nomeJogador) == 0) {
        printf("Nome inválido: o campo não pode ficar vazio.\n");
        return 1;
    }

    if (!lerTexto("Escolha um apelido: ", apelidoJogador, sizeof apelidoJogador)) {
        printf("Apelido inválido: informe de 1 a 15 caracteres.\n");
        return 1;
    }
    size_t tamanhoApelido = strlen(apelidoJogador);
    if (tamanhoApelido < 1 || tamanhoApelido > 15) {
        printf("Apelido inválido: informe de 1 a 15 caracteres.\n");
        return 1;
    }

    if (!lerTexto("Crie uma senha (1 a 7 caracteres): ", senhaJogador, sizeof senhaJogador)) {
        printf("Senha inválida: informe de 1 a 7 caracteres.\n");
        return 1;
    }
    if (strlen(senhaJogador) == 0) {
        printf("Senha inválida: informe pelo menos 1 caractere.\n");
        return 1;
    }

    if (!lerTexto("Confirme a senha: ", confirmacaoSenha, sizeof confirmacaoSenha)) {
        printf("Confirmação inválida: a senha pode ter no máximo 7 caracteres.\n");
        return 1;
    }
    if (strcmp(senhaJogador, confirmacaoSenha) != 0) {
        printf("As senhas não coincidem. Corrija a confirmação da senha.\n");
        return 1;
    }

    if (!lerTexto("Nome da equipe (1 a 15 caracteres): ", equipeJogador,
                  sizeof equipeJogador) || strlen(equipeJogador) == 0) {
        printf("Equipe inválida: informe de 1 a 15 caracteres.\n");
        return 1;
    }

    if (!montarNomeExibicao(jogadores[quantidadeJogadores],
                            sizeof jogadores[quantidadeJogadores],
                            apelidoJogador, equipeJogador)) {
        printf("Não foi possível montar o nome de exibição: espaço insuficiente.\n");
        return 1;
    }
    quantidadeJogadores++;
    printf("Cadastro realizado com sucesso, %s (%s)!\n", nomeJogador,
           jogadores[0]);

    for (size_t i = quantidadeJogadores; i < QUANTIDADE_JOGADORES; i++) {
        printf("\nCadastro do jogador %zu de %d\n", i + 1,
               QUANTIDADE_JOGADORES);
        if (!lerTexto("Apelido (1 a 15 caracteres): ", apelidoOutro,
                      sizeof apelidoOutro) || strlen(apelidoOutro) == 0) {
            printf("Apelido inválido: informe de 1 a 15 caracteres.\n");
            return 1;
        }
        if (!lerTexto("Equipe (1 a 15 caracteres): ", equipeOutra,
                      sizeof equipeOutra) || strlen(equipeOutra) == 0) {
            printf("Equipe inválida: informe de 1 a 15 caracteres.\n");
            return 1;
        }
        if (!montarNomeExibicao(jogadores[i], sizeof jogadores[i],
                                apelidoOutro, equipeOutra)) {
            printf("Não foi possível montar o nome de exibição: espaço insuficiente.\n");
            return 1;
        }
        quantidadeJogadores++;
    }

    printf("\n** JOGADORES CADASTRADOS **\n");
    for (size_t i = 0; i < quantidadeJogadores; i++) {
        printf("%zu: %s\n", i + 1, jogadores[i]);
    }

    if (!lerTexto("\nNome de exibição exato para buscar: ", busca,
                  sizeof busca)) {
        printf("Busca inválida: informe no máximo %zu caracteres.\n",
               sizeof busca - 1);
        return 1;
    }

    int encontrado = 0;
    for (size_t i = 0; i < quantidadeJogadores; i++) {
        if (strcmp(jogadores[i], busca) == 0) {
            printf("Jogador encontrado na posição %zu: %s\n", i + 1,
                   jogadores[i]);
            encontrado = 1;
            break;
        }
    }
    if (!encontrado) {
        printf("Jogador não encontrado.\n");
    }

    int opcao;
    while (1) {
        exibirMapa(mapa);
        printf("\n1 - Posicionar jogador | 2 - Remover posição\n");
        printf("3 - Reposicionar jogador | 0 - Encerrar\n");
        if (!lerInteiro("Escolha uma opção: ", &opcao)) {
            printf("Opção inválida.\n");
            continue;
        }
        if (opcao == 0) {
            break;
        }

        int jogador;
        int linha;
        int coluna;
        if (opcao == 1 || opcao == 3) {
            if (!lerInteiro("Número do jogador (1 a 5): ", &jogador) ||
                jogador < 1 || jogador > (int)quantidadeJogadores) {
                printf("Jogador inválido.\n");
                continue;
            }
            if (!lerInteiro("Linha (0 a 4): ", &linha) ||
                !lerInteiro("Coluna (0 a 4): ", &coluna)) {
                printf("Coordenadas inválidas.\n");
                continue;
            }

            int sucesso;
            if (opcao == 1) {
                sucesso = posicionarJogador(mapa, jogador - 1,
                                             quantidadeJogadores, linha, coluna);
            } else {
                sucesso = reposicionarJogador(mapa, jogador - 1,
                                               quantidadeJogadores, linha, coluna);
            }
            if (sucesso) {
                printf("Posição atualizada.\n");
            } else {
                printf("Não foi possível usar essa posição. Verifique as coordenadas e a ocupação.\n");
            }
        } else if (opcao == 2) {
            int jogadorRemovido;
            if (!lerInteiro("Linha (0 a 4): ", &linha) ||
                !lerInteiro("Coluna (0 a 4): ", &coluna)) {
                printf("Coordenadas inválidas.\n");
                continue;
            }
            if (removerPosicao(mapa, linha, coluna, &jogadorRemovido)) {
                printf("Jogador %d removido do mapa.\n", jogadorRemovido + 1);
            } else {
                printf("Coordenada inválida ou posição já livre.\n");
            }
        } else {
            printf("Opção inválida.\n");
        }
    }

    return 0;
}