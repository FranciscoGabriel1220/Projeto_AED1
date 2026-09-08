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
void verificar_inventario(int *inventario[3], int escolha ){
   
int verificar;
int i;
printf("qual o valor do item ?");

    do
    { 
        scanf("%d", inventario[escolha]);
        printf("valor do item:%d\n", *inventario[escolha]);
        

        printf("vai continuar ?\n");
        scanf("%d", &verificar);

    } while (verificar!=0);
}
void pegar_item(int *inventario[3],int escolha){

    switch (escolha)
    {
    case 1:
        printf("espada recolhida\n, valor:%d",**(inventario+escolha-1));
        break;
    case 2:
    printf("porcao recolhida, valor:%d", **(inventario+escolha-1));
    break;
    case 3:
    printf("comida recolhida, valor:%d",**(inventario+escolha-1));
    break;
    default:
    printf("invalido");
        break;
    }
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



//o deslocamento respeita o tipo do ponteiro por causa que cada tipo gasta uma quantidade de memoria diferente
int tamanho;
int exploracao;
int *pexploracao = &exploracao;
int *pontuacao;
int pontu;
pontuacao=&pontu;
pontu=15;

int *inventario[3];
int espada;
int porcao;
int flecha;
 inventario[0]=&espada;
 inventario[1]=&porcao;
 inventario[2]=&flecha;
 espada=10;
 porcao=20;
 flecha=8;



int opcao;
int escolha;
do
{
    printf("1 - Vida\n2 - Mapa\n3 - Itens\n0 - Sair\n");
    scanf("%d", &opcao);
    
    switch (opcao)
    {
        case 1:
           printf("vida\n");
           aplicardano(pvida, dano);
           restaurarvida(pvida, cura);
            break;
        case 2:{ 
        printf("qual o tamanho do mapa ?");
        scanf("%d",&tamanho);
        int mapa[tamanho];
        ler_mapa(mapa,tamanho,pexploracao);
        mostrar_mapa(mapa,tamanho,pexploracao);
        explorar_mapa(pontuacao, mapa, tamanho);
            break;
    }
        case 3:
        printf("qual o item ?");
        scanf("%d", &escolha);
        verificar_inventario(inventario,escolha);
        pegar_item(inventario, escolha);
        break;

        default:
        break;
    }
} while (opcao != 0);


return 0;
}