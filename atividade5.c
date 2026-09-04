#include <stdio.h>
#include <raylib.h>

void aplicardano(int *pvida, int dano)
{
    
    if (pvida == NULL)
    {
        printf("ponteiro invalido!!");
        return;
    }
    *pvida -= dano;
    printf("dano: %d\nvida atual:%d\n", dano, *pvida);  
}
void restaurarvida(int *pvida, int cura)
{
    
     if (pvida == NULL)
    {
        printf("ponteiro invalido!!");
        return;
    }
    *pvida += cura;
    if (*pvida>100)
    {
        *pvida = 100;
    }
    
    printf("vida recuperada: %d\nvida atual:%d\n",cura, *pvida);
}
void AplicarPontuacaoDupla(int *ptesouro)
{
    
     if (ptesouro == NULL)
    {
        printf("ponteiro invalido!!");
        return;
    }
    *ptesouro *= 2;
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
void explorar_mapa(int*pontucao,int*mapa,int tamanho){
    if (pontucao==NULL)
    {
        printf("ponteiro invalido!!");
        return;
    }
    

int *p=mapa; //auxilio
int posicao;//auxilio

while (p<mapa+tamanho) // para quando "passar" do ultimo elemento
{
    posicao=p-mapa; // calcula a posicao logica
    *pontucao+=*p;  // soma o conteudo atual na pontuacao
    printf("posição= %d\nvalor=%d\n", *pontucao, *p); //mostra os valores
    p++; 
    //a diferença entre incremento de ponteiro e acesso de indice é que o incremento usa p++ e o acesso por indice usa por exemplo *(mapa+i)
}
printf("valor final=%d\n", *pontucao);

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

int *pontuacao;
int pontu;
pontuacao=&pontu;
pontu=15;

explorar_mapa(pontuacao, mapa, tamanho);


return 0;
}