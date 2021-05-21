#ifndef LISTAINT_H_INCLUDED
#define LISTAINT_H_INCLUDED

typedef struct ListaInt
{
    char sentido;
    int valor;
    struct ListaInt *proximo;
	
} ListaInt;

/*funcoes basicas*/
int insert(ListaInt **lista, int valor,  char sentido);
int searchValueAndRemove(ListaInt **lista, int value, char sentido, int listSize);
int get(ListaInt *lista, int posicao);
int tamanho(ListaInt *lista);

/*funcoes avançadas*/
int removePos(ListaInt **lista, int posicao);

/*auxiliares*/
void toString(ListaInt *lista, char saida[]);
void autor(char nome[]);



#endif // LISTAINT_H_INCLUDED
