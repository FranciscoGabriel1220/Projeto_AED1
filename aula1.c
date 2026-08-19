#include <stdio.h>
int main(){
 int vida = 100;
 int *pv = &vida;
 *pv = vida;
int tesouro = 0;
int *pt = &tesouro;
 
int dano = 14;
*pv-=dano;

*pt=1;

printf("vida:%d\n", vida);
printf("dano: %d\n", dano);
printf("tesouro: %d\n",tesouro);

return 0;
}