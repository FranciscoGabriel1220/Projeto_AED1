#include <stdio.h>
#include <raylib.h>
#include <string.h>
#include <stdlib.h>

#define nome 16
#define apelido 16
#define senha 7

int main(void){
 

 //tela de cadastro do usuario.
printf("**TELA DE CADASTRO**\n");
char nomejogador[nome];
char senhajogador[senha];
char apelidojogador[apelido];
char guarda_senha[senha];
printf("qual o seu nome ?\n");
fgets(nomejogador,nome,stdin);


int verificadorName=strlen(nomejogador);
if (verificadorName<1||verificadorName>15)
{
    printf("nome invalido...sao no minimo 1 e no maximo 15 caracteres\n");
    exit(1);
}
nomejogador[strcspn(nomejogador, "\n")]='\0';


printf("qual sua senha ?\n");
fgets(senhajogador,senha,stdin);
senhajogador[strcspn(senhajogador, "\n")]='\0';
int verificadorsenha=strlen(senhajogador);
if (verificadorsenha<1||verificadorsenha>6)
{
    printf("senha invalida...sao no minimo 1 e no maximo 7 caracteres\n");
    exit(1);
}
printf("digite um apelido\n");
fgets(apelidojogador,apelido,stdin);


int verificadorapelido=strlen(apelidojogador);
if (verificadorapelido<1||verificadorapelido>15)
{
    printf("apelido invalido...sao no minimo 1 e no maximo 15 caracteres\n");
    exit(1);
}
apelidojogador[strcspn(apelidojogador, "\n")]='\0';
printf("cadastro feito com sucesso!!!\n");
printf("\n");


//pega as infomações anteriores para verificar nesse proximo passo.
printf("**tela de login**\n");
printf("qual sua senha?\n");
fgets(guarda_senha,senha,stdin);
guarda_senha[strcspn(guarda_senha, "\n")]='\0';
if (strcmp(senhajogador,guarda_senha)!=0)
{
    printf("senha incorreta!!\n");
    exit(1);
}else{
    printf("senha correta!!");
}

return 0;
}