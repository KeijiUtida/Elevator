/******************************************************

Autor: Leandro Keiji Utida Pereira

******************************************************/

#include "ListaInt.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * Insere um valor na lista
 * @param lista Lista
 * @param valor Valor a ser inserido
 * @return Sucesso da operação
 */
int insert(ListaInt **lista, int valor, char sentido)
{
    ListaInt *novo= malloc(sizeof(ListaInt));
    novo->valor = valor;
    novo->sentido = sentido;
    novo->proximo = NULL;
    if (*lista == NULL)
        *lista = novo;
    else
    {
        ListaInt *andarilho = *lista;
        while (andarilho->proximo!= NULL)
        {
            if (andarilho->valor==novo->valor && andarilho->sentido==novo->sentido)
            return 0;
            andarilho = andarilho->proximo;
            
        }
        andarilho->proximo = novo;
    }
    return 0;
}

/**
 * Retorna o valor contido na lista na posição pedida
 * @param lista Lista
 * @param posicao Posição do elemento desejado
 * @return Valor contido na posição
 */
int getPos(ListaInt *lista, int posicao)
{
    int contador;
    ListaInt *andarilho = lista;
    if (posicao == 0)
        return lista->valor;
    for(contador = 0 ;  contador<posicao; contador++)
        andarilho = andarilho->proximo;
    return andarilho->valor;
}

int searchValueAndRemove(ListaInt **lista, int value, char sentido)
{
    int posicao;
    ListaInt *andarilho = *lista;
    while (andarilho->proximo!= NULL)
    {      
            if (andarilho->valor == value && (andarilho->sentido == sentido || sentido == 'i'))
            {
                  int i;
                  ListaInt *excluido;
                  if (posicao == 0)
                  {
                    ListaInt *excluido = *lista;
                    *lista = (*lista)->proximo;
                    free(excluido);
                  }
                  ListaInt *andarilho = *lista;
                  for (i=0; i<posicao; i++)
                  andarilho = andarilho->proximo;
                  excluido=andarilho->proximo;
                  andarilho->proximo = andarilho->proximo->proximo;
                  free(excluido);
                  return 1;
            }
            andarilho = andarilho->proximo;
            posicao++;   
    }
    return 0;
}

/**
 * Remove o elemento contido na lista na posição pedida
 * @param lista Lista
 * @param posicao Posição do elemento a ser excluído
 * @return Sucesso da operação
 */
int removePos(ListaInt **lista, int posicao)
{
    int i;
    ListaInt *excluido;
    if (posicao == 0)
    {
         ListaInt *excluido = *lista;
        *lista = (*lista)->proximo;
         free(excluido);
    }
    ListaInt *andarilho = *lista;

    for (i=0; i<posicao; i++)
        andarilho = andarilho->proximo;
    excluido=andarilho->proximo;
    andarilho->proximo = andarilho->proximo->proximo;
    free(excluido);

    return 0;
}

/**
 * Conta o número de elementos na lista e retorna este valor
 * @param lista Lista
 * @return Número de elementos na lista
 */
int tamanho(ListaInt *lista)
{
    int contador;
    ListaInt *andarilho = lista;
    while (andarilho!= NULL)
    {
        andarilho = andarilho->proximo;
        contador++;
    }
    return contador;
}

