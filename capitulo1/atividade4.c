#include <stdio.h>
#include <raylib.h>

void aplicardano(int *pvida, int dano)
{
    *pvida -= dano;
    if (pvida == NULL)
    {
        printf("ponteiro invalido!!");
    }
    
    printf("dano: %d\nvida atual:%d\n", dano, *pvida);  
}
void restaurarvida(int *pvida, int cura)
{
    *pvida += cura;
     if (pvida == NULL)
    {
        printf("ponteiro invalido!!");
    }
    
    if (cura >100)
    {
        *pvida = 100;
    }
    
    printf("vida recuperada: %d\nvida atual:%d\n",cura, *pvida);
}
void AplicarPontuacaoDupla(int *ptesouro)
{
    *ptesouro *= 2;
     if (ptesouro == NULL)
    {
        printf("ponteiro invalido!!");
    }
    
    printf("tesouro: %d\n", *ptesouro);
}
void ler_mapa(int *mapa, int tamanho, int*exploracao){
 int i;
 *exploracao = 0;
 printf("qual o tamanho do mapa?:");

 if (tamanho<0)
 {
    printf("invalido!! valor menor dos parametros");
    return;
 }else if (tamanho>999)
 {
    printf("ultrapassou os limites do mapa!!");
    return;
 }
 
 

    for ( i = 0; i < tamanho; i++)
{   
    scanf("%d\n", (mapa+i));
    if (*(mapa +i) < 0)
{
    printf("invalido\n");
    *(mapa+i)=0;
}else{
    *exploracao+=*(mapa+i); 
}

}
 
}
void mostrar_mapa(const int *mapa, int tamanho, int*exploracao){
  int i;
  
    for ( i = 0; i < tamanho; i++)
{
    
    printf("posicao= %d\n",i);
    printf("%d\n", *(mapa+i));
    
    
}
printf("resultado da exploracao= %d", *exploracao);
}  


int main(void){
 int vida = 100;
 int *pvida = &vida;
 *pvida = vida;
int tesouro = 0;
int *ptesouro = &tesouro;
int dano = 14;
int cura=14;

*ptesouro=4;
int i, soma;



aplicardano(pvida, dano);
AplicarPontuacaoDupla(ptesouro);

restaurarvida(pvida, cura);
//o deslocamento respeita o tipo do ponteiro por causa que cada tipo gasta uma quantidade de memoria diferente
int tamanho;
int exploracao;
int *pexploracao = &exploracao;

printf("qual o tamanho do mapa ?");
scanf("%d",&tamanho);
int mapa[tamanho];
ler_mapa(mapa,tamanho,pexploracao);
mostrar_mapa(mapa,tamanho,pexploracao);




return 0;
}