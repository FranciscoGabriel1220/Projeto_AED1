#include <stdio.h>

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

int main(){
 int vida = 100;
 int *pvida = &vida;
 *pvida = vida;
int tesouro = 0;
int *ptesouro = &tesouro;
int dano = 14;
int cura=14;

*ptesouro=4;


aplicardano(pvida, dano);
AplicarPontuacaoDupla(ptesouro);

restaurarvida(pvida, cura);

return 0;
}