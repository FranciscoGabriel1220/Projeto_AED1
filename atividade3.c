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

int main(void){
 int vida = 100;
 int *pvida = &vida;
 *pvida = vida;
int tesouro = 0;
int *ptesouro = &tesouro;
int dano = 14;
int cura=14;

*ptesouro=4;
int mapa[10];
int i, soma;

soma=0;


aplicardano(pvida, dano);
AplicarPontuacaoDupla(ptesouro);

restaurarvida(pvida, cura);
//o deslocamento respeita o tipo do ponteiro por causa que cada tipo gasta uma quantidade de memoria diferente
printf ("qual a sua posição no mapa ?");

for ( i = 0; i < 10; i++)
{
    scanf("%d", &mapa[i]);
    soma+=*(mapa+i);

}

for ( i = 0; i < 10; i++)
{
    printf("posicao= %d\n",i);
    printf("%d\n", *(mapa+i));
    printf("%p\n", (mapa+i));
}
printf("resultado da exploracao= %d", soma);

return 0;
}