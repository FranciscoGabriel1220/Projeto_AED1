#include <stdio.h>
#include <string.h>

#define NOME_CAPACIDADE 16
#define APELIDO_CAPACIDADE 16
#define SENHA_CAPACIDADE 8

/* Lê uma linha, remove o '\n' e rejeita entradas maiores que o vetor. */
static int lerTexto(const char *prompt, char texto[], size_t capacidade)
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

int main(void)
{
    char nomeJogador[NOME_CAPACIDADE];
    char apelidoJogador[APELIDO_CAPACIDADE];
    char senhaJogador[SENHA_CAPACIDADE];
    char confirmacaoSenha[SENHA_CAPACIDADE];

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

    printf("Cadastro realizado com sucesso, %s (%s)!\n", nomeJogador, apelidoJogador);
    return 0;
}