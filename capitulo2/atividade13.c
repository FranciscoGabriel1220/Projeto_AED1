#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#define NOME_CAPACIDADE 16
#define APELIDO_CAPACIDADE 16
#define EQUIPE_CAPACIDADE 16
#define SENHA_CAPACIDADE 8
#define POSICAO_LIVRE -1
#define SEPARADOR " - "
#define EXIBICAO_CAPACIDADE (APELIDO_CAPACIDADE + EQUIPE_CAPACIDADE + sizeof(SEPARADOR))
#define MAX_PARTIDAS 100

#define TIPO_INTEIRO 1
#define TIPO_REAL 2

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

int dimensoesValidas(size_t linhas, size_t colunas)
{
    return linhas > 0 && colunas > 0 &&
           linhas <= SIZE_MAX / sizeof(int *) &&
           linhas <= SIZE_MAX / colunas &&
           linhas * colunas <= SIZE_MAX / sizeof(int);
}

int *criarMapaLinear(size_t linhas, size_t colunas)
{
    if (!dimensoesValidas(linhas, colunas)) {
        return NULL;
    }
    return malloc(linhas * colunas * sizeof(int));
}

int **criarMapaPorLinhas(size_t linhas, size_t colunas)
{
    if (!dimensoesValidas(linhas, colunas)) {
        return NULL;
    }

    int **mapa = malloc(linhas * sizeof(*mapa));
    if (mapa == NULL) {
        return NULL;
    }
    for (size_t linha = 0; linha < linhas; linha++) {
        mapa[linha] = malloc(colunas * sizeof(*mapa[linha]));
        if (mapa[linha] == NULL) {
            while (linha > 0) {
                free(mapa[--linha]);
            }
            free(mapa);
            return NULL;
        }
    }
    return mapa;
}

void preencherMapaLinear(int *mapa, size_t linhas, size_t colunas)
{
    for (size_t linha = 0; linha < linhas; linha++) {
        for (size_t coluna = 0; coluna < colunas; coluna++) {
            mapa[linha * colunas + coluna] = POSICAO_LIVRE;
        }
    }
}

void preencherMapaPorLinhas(int **mapa, size_t linhas, size_t colunas)
{
    for (size_t linha = 0; linha < linhas; linha++) {
        for (size_t coluna = 0; coluna < colunas; coluna++) {
            mapa[linha][coluna] = POSICAO_LIVRE;
        }
    }
}

void exibirMapaLinear(const int *mapa, size_t linhas, size_t colunas)
{
    printf("Mapa linear:\n");
    for (size_t linha = 0; linha < linhas; linha++) {
        for (size_t coluna = 0; coluna < colunas; coluna++) {
            int jogador = mapa[linha * colunas + coluna];
            if (jogador == POSICAO_LIVRE) printf(" . ");
            else printf(" %d ", jogador + 1);
        }
        printf("\n");
    }
}

void exibirMapaPorLinhas(int *const *mapa, size_t linhas, size_t colunas)
{
    printf("Mapa por linhas:\n");
    for (size_t linha = 0; linha < linhas; linha++) {
        for (size_t coluna = 0; coluna < colunas; coluna++) {
            int jogador = mapa[linha][coluna];
            if (jogador == POSICAO_LIVRE) printf(" . ");
            else printf(" %d ", jogador + 1);
        }
        printf("\n");
    }
}

void liberarMapaLinear(int *mapa)
{
    free(mapa);
}

void liberarMapaPorLinhas(int **mapa, size_t linhas)
{
    if (mapa == NULL) return;
    for (size_t linha = 0; linha < linhas; linha++) {
        free(mapa[linha]);
    }
    free(mapa);
}

int coordenadaValida(int linha, int coluna, size_t linhas, size_t colunas)
{
    return linha >= 0 && coluna >= 0 &&
           (size_t)linha < linhas && (size_t)coluna < colunas;
}

int posicionarJogador(int *mapaLinear, int **mapaLinhas,
                      int jogador, size_t quantidadeJogadores,
                      int linha, int coluna, size_t linhas, size_t colunas)
{
    if (!coordenadaValida(linha, coluna, linhas, colunas) || jogador < 0 ||
        (size_t)jogador >= quantidadeJogadores ||
        mapaLinear[(size_t)linha * colunas + (size_t)coluna] != POSICAO_LIVRE) {
        return 0;
    }
    for (size_t l = 0; l < linhas; l++) {
        for (size_t c = 0; c < colunas; c++) {
            if (mapaLinear[l * colunas + c] == jogador) return 0;
        }
    }
    mapaLinear[(size_t)linha * colunas + (size_t)coluna] = jogador;
    mapaLinhas[linha][coluna] = jogador;
    return 1;
}

int removerJogador(int *mapaLinear, int **mapaLinhas,
                   int linha, int coluna, size_t linhas, size_t colunas)
{
    if (!coordenadaValida(linha, coluna, linhas, colunas) ||
        mapaLinear[(size_t)linha * colunas + (size_t)coluna] == POSICAO_LIVRE) {
        return 0;
    }
    mapaLinear[(size_t)linha * colunas + (size_t)coluna] = POSICAO_LIVRE;
    mapaLinhas[linha][coluna] = POSICAO_LIVRE;
    return 1;
}

int reposicionarJogador(int *mapaLinear, int **mapaLinhas,
                        int jogador, size_t quantidadeJogadores,
                        int novaLinha, int novaColuna,
                        size_t linhas, size_t colunas)
{
    if (!coordenadaValida(novaLinha, novaColuna, linhas, colunas) ||
        jogador < 0 || (size_t)jogador >= quantidadeJogadores) {
        return 0;
    }
    size_t novoIndice = (size_t)novaLinha * colunas + (size_t)novaColuna;
    if (mapaLinear[novoIndice] != POSICAO_LIVRE &&
        mapaLinear[novoIndice] != jogador) {
        return 0;
    }
    for (size_t l = 0; l < linhas; l++) {
        for (size_t c = 0; c < colunas; c++) {
            if (mapaLinear[l * colunas + c] == jogador) {
                mapaLinear[l * colunas + c] = POSICAO_LIVRE;
                mapaLinhas[l][c] = POSICAO_LIVRE;
            }
        }
    }
    mapaLinear[novoIndice] = jogador;
    mapaLinhas[novaLinha][novaColuna] = jogador;
    return 1;
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

int *criarHistorico(size_t quantidade)
{
    if (quantidade == 0 || quantidade > MAX_PARTIDAS) {
        return NULL;
    }
    return malloc(quantidade * sizeof(int));
}

int preencherHistorico(int pontuacoes[], size_t quantidade)
{
    for (size_t i = 0; i < quantidade; i++) {
        printf("Pontuação da partida %zu (0 a 100): ", i + 1);
        if (!lerInteiro("", &pontuacoes[i]) ||
            pontuacoes[i] < 0 || pontuacoes[i] > 100) {
            return 0;
        }
    }
    return 1;
}

void exibirHistorico(const int pontuacoes[], size_t quantidade)
{
    printf("\nHistórico de pontuações:\n");
    for (size_t i = 0; i < quantidade; i++) {
        printf("Partida %zu: %d\n", i + 1, pontuacoes[i]);
    }
}

double calcularMedia(const int pontuacoes[], size_t quantidade)
{
    int soma = 0;
    for (size_t i = 0; i < quantidade; i++) {
        soma += pontuacoes[i];
    }
    return (double)soma / quantidade;
}

int localizarMaior(const int pontuacoes[], size_t quantidade,
                   size_t *posicaoMaior)
{
    int maior = pontuacoes[0];
    *posicaoMaior = 0;
    for (size_t i = 1; i < quantidade; i++) {
        if (pontuacoes[i] > maior) {
            maior = pontuacoes[i];
            *posicaoMaior = i;
        }
    }
    return maior;
}

void exibirValor(void *valor, int tipo)
{
    if (tipo == TIPO_INTEIRO) {
        printf("%d\n", *(int *)valor);
    } else if (tipo == TIPO_REAL) {
        printf("%g\n", *(float *)valor);
    } else {
        printf("Tipo desconhecido.\n");
    }
}

int main(void)
{
    int *pontuacoes = NULL;
    int *mapaLinear = NULL;
    int **mapaLinhas = NULL;
    size_t linhasMapa = 0;
    size_t colunasMapa = 0;
    int quantidadeInformada;
    printf("** TELA DE CADASTRO **\n");
    if (!lerInteiro("Quantos jogadores deseja cadastrar? (1 a 100): ",
                    &quantidadeInformada) ||
        quantidadeInformada < 1 || quantidadeInformada > 100) {
        printf("Quantidade inválida: informe um valor entre 1 e 100.\n");
        return 1;
    }

    size_t quantidadeJogadores = (size_t)quantidadeInformada;
    /* sizeof(*jogadores) acompanha automaticamente o tamanho de cada registro. */
    char (*jogadores)[EXIBICAO_CAPACIDADE] =
        malloc(quantidadeJogadores * sizeof(*jogadores));
    if (jogadores == NULL) {
        printf("Não foi possível reservar memória para os jogadores.\n");
        return 1;
    }

    char nomeJogador[NOME_CAPACIDADE];
    char apelidoJogador[APELIDO_CAPACIDADE];
    char equipeJogador[EQUIPE_CAPACIDADE];
    char senhaJogador[SENHA_CAPACIDADE];
    char confirmacaoSenha[SENHA_CAPACIDADE];
    char apelidoOutro[APELIDO_CAPACIDADE];
    char equipeOutra[EQUIPE_CAPACIDADE];
    char busca[EXIBICAO_CAPACIDADE];

    int diagnosticoInteiro = 42;
    float diagnosticoReal = 3.5f;
    printf("Teste de valores genéricos: ");
    exibirValor(&diagnosticoInteiro, TIPO_INTEIRO);
    printf("Teste de valores genéricos: ");
    exibirValor(&diagnosticoReal, TIPO_REAL);

    if (!lerTexto("Qual é o seu nome? ", nomeJogador, sizeof nomeJogador)) {
        printf("Nome inválido: informe no máximo 15 caracteres.\n");
        goto falha;
    }
    if (strlen(nomeJogador) == 0) {
        printf("Nome inválido: o campo não pode ficar vazio.\n");
        goto falha;
    }

    if (!lerTexto("Escolha um apelido: ", apelidoJogador, sizeof apelidoJogador)) {
        printf("Apelido inválido: informe de 1 a 15 caracteres.\n");
        goto falha;
    }
    size_t tamanhoApelido = strlen(apelidoJogador);
    if (tamanhoApelido < 1 || tamanhoApelido > 15) {
        printf("Apelido inválido: informe de 1 a 15 caracteres.\n");
        goto falha;
    }

    if (!lerTexto("Crie uma senha (1 a 7 caracteres): ", senhaJogador, sizeof senhaJogador)) {
        printf("Senha inválida: informe de 1 a 7 caracteres.\n");
        goto falha;
    }
    if (strlen(senhaJogador) == 0) {
        printf("Senha inválida: informe pelo menos 1 caractere.\n");
        goto falha;
    }

    if (!lerTexto("Confirme a senha: ", confirmacaoSenha, sizeof confirmacaoSenha)) {
        printf("Confirmação inválida: a senha pode ter no máximo 7 caracteres.\n");
        goto falha;
    }
    if (strcmp(senhaJogador, confirmacaoSenha) != 0) {
        printf("As senhas não coincidem. Corrija a confirmação da senha.\n");
        goto falha;
    }

    if (!lerTexto("Nome da equipe (1 a 15 caracteres): ", equipeJogador,
                  sizeof equipeJogador) || strlen(equipeJogador) == 0) {
        printf("Equipe inválida: informe de 1 a 15 caracteres.\n");
        goto falha;
    }

    if (!montarNomeExibicao(jogadores[0],
                            sizeof(*jogadores),
                            apelidoJogador, equipeJogador)) {
        printf("Não foi possível montar o nome de exibição: espaço insuficiente.\n");
        goto falha;
    }
    printf("Cadastro realizado com sucesso, %s (%s)!\n", nomeJogador,
           jogadores[0]);

    for (size_t i = 1; i < quantidadeJogadores; i++) {
        printf("\nCadastro do jogador %zu de %zu\n", i + 1,
               quantidadeJogadores);
        if (!lerTexto("Apelido (1 a 15 caracteres): ", apelidoOutro,
                      sizeof apelidoOutro) || strlen(apelidoOutro) == 0) {
            printf("Apelido inválido: informe de 1 a 15 caracteres.\n");
            goto falha;
        }
        if (!lerTexto("Equipe (1 a 15 caracteres): ", equipeOutra,
                      sizeof equipeOutra) || strlen(equipeOutra) == 0) {
            printf("Equipe inválida: informe de 1 a 15 caracteres.\n");
            goto falha;
        }
        if (!montarNomeExibicao(jogadores[i], sizeof(*jogadores),
                                apelidoOutro, equipeOutra)) {
            printf("Não foi possível montar o nome de exibição: espaço insuficiente.\n");
            goto falha;
        }
    }

    printf("\n** JOGADORES CADASTRADOS **\n");
    for (size_t i = 0; i < quantidadeJogadores; i++) {
        printf("%zu: %s\n", i + 1, jogadores[i]);
    }

    int quantidadePartidas;
    if (!lerInteiro("\nQuantas partidas deseja registrar? (1 a 100): ",
                    &quantidadePartidas) ||
        quantidadePartidas < 1 || quantidadePartidas > MAX_PARTIDAS) {
        printf("Quantidade inválida: informe de 1 a %d partidas.\n",
               MAX_PARTIDAS);
        goto falha;
    }

    size_t totalPartidas = (size_t)quantidadePartidas;
    pontuacoes = criarHistorico(totalPartidas);
    if (pontuacoes == NULL) {
        printf("Não foi possível criar o histórico.\n");
        goto falha;
    }
    if (!preencherHistorico(pontuacoes, totalPartidas)) {
        printf("Pontuação inválida. Informe valores entre 0 e 100.\n");
        goto falha;
    }

    exibirHistorico(pontuacoes, totalPartidas);
    size_t posicaoMaior;
    int maior = localizarMaior(pontuacoes, totalPartidas, &posicaoMaior);
    printf("Média: %.2f\n", calcularMedia(pontuacoes, totalPartidas));
    printf("Maior pontuação: %d (partida %zu)\n", maior, posicaoMaior + 1);

    if (!lerTexto("\nNome de exibição exato para buscar: ", busca,
                  sizeof busca)) {
        printf("Busca inválida: informe no máximo %zu caracteres.\n",
               sizeof busca - 1);
         goto falha;
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

    int linhasInformadas;
    int colunasInformadas;
    if (!lerInteiro("\nQuantidade de linhas do mapa (1 a 100): ",
                    &linhasInformadas) || linhasInformadas < 1 ||
        linhasInformadas > 100 ||
        !lerInteiro("Quantidade de colunas do mapa (1 a 100): ",
                    &colunasInformadas) || colunasInformadas < 1 ||
        colunasInformadas > 100) {
        printf("Dimensões inválidas.\n");
        goto falha;
    }
    linhasMapa = (size_t)linhasInformadas;
    colunasMapa = (size_t)colunasInformadas;

    mapaLinear = criarMapaLinear(linhasMapa, colunasMapa);
    mapaLinhas = criarMapaPorLinhas(linhasMapa, colunasMapa);
    if (mapaLinear == NULL || mapaLinhas == NULL) {
        printf("Não foi possível alocar as duas matrizes.\n");
        goto falha;
    }
    /*
     * Linear: todos os elementos ficam em um bloco contíguo e usam
     * linha * colunas + coluna. Por linhas: cada linha é alocada separada.
     */
    preencherMapaLinear(mapaLinear, linhasMapa, colunasMapa);
    preencherMapaPorLinhas(mapaLinhas, linhasMapa, colunasMapa);

    int opcao;
    while (1) {
        printf("\n1 - Exibir as duas matrizes\n");
        printf("2 - Posicionar jogador | 3 - Remover posição\n");
        printf("4 - Reposicionar jogador | 5 - Limpar mapas | 0 - Encerrar\n");
        if (!lerInteiro("Escolha uma opção: ", &opcao)) {
            printf("Opção inválida.\n");
            continue;
        }
        if (opcao == 0) {
            break;
        }
        if (opcao == 1) {
            exibirMapaLinear(mapaLinear, linhasMapa, colunasMapa);
            exibirMapaPorLinhas(mapaLinhas, linhasMapa, colunasMapa);
            continue;
        }
        if (opcao == 5) {
            preencherMapaLinear(mapaLinear, linhasMapa, colunasMapa);
            preencherMapaPorLinhas(mapaLinhas, linhasMapa, colunasMapa);
            printf("Os dois mapas foram limpos.\n");
            continue;
        }

        int jogador;
        int linha;
        int coluna;
        if (opcao == 2 || opcao == 4) {
            if (!lerInteiro("Número do jogador: ", &jogador) ||
                jogador < 1 || jogador > (int)quantidadeJogadores) {
                printf("Jogador inválido.\n");
                continue;
            }
            if (!lerInteiro("Linha: ", &linha) ||
                !lerInteiro("Coluna: ", &coluna) ||
                !coordenadaValida(linha, coluna, linhasMapa, colunasMapa)) {
                printf("Coordenadas fora do mapa.\n");
                continue;
            }

            int sucesso;
            if (opcao == 2) {
                sucesso = posicionarJogador(mapaLinear, mapaLinhas,
                                             jogador - 1, quantidadeJogadores,
                                             linha, coluna, linhasMapa, colunasMapa);
            } else {
                sucesso = reposicionarJogador(mapaLinear, mapaLinhas,
                                               jogador - 1, quantidadeJogadores,
                                               linha, coluna, linhasMapa, colunasMapa);
            }
            if (sucesso) {
                printf("Posição atualizada.\n");
            } else {
                printf("Não foi possível usar essa posição. Verifique as coordenadas e a ocupação.\n");
            }
        } else if (opcao == 3) {
            if (!lerInteiro("Linha: ", &linha) ||
                !lerInteiro("Coluna: ", &coluna) ||
                !coordenadaValida(linha, coluna, linhasMapa, colunasMapa)) {
                printf("Coordenadas fora do mapa.\n");
                continue;
            }
            if (removerJogador(mapaLinear, mapaLinhas, linha, coluna,
                               linhasMapa, colunasMapa)) {
                printf("Posição removida nos dois mapas.\n");
            } else {
                printf("Coordenada inválida ou posição já livre.\n");
            }
        } else {
            printf("Opção inválida.\n");
        }
    }

    free(jogadores);
    jogadores = NULL;
    free(pontuacoes);
    pontuacoes = NULL;
    liberarMapaLinear(mapaLinear);
    mapaLinear = NULL;
    liberarMapaPorLinhas(mapaLinhas, linhasMapa);
    mapaLinhas = NULL;
    return 0;

falha:
    free(jogadores);
    jogadores = NULL;
    free(pontuacoes);
    pontuacoes = NULL;
    liberarMapaLinear(mapaLinear);
    mapaLinear = NULL;
    liberarMapaPorLinhas(mapaLinhas, linhasMapa);
    mapaLinhas = NULL;
    return 1;
}