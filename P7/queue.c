#include <stdio.h>
#include "queue.h"

//Alunos
//Gustavo Henrique Zeni
//Ianca Polizelo


//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila

void queue_append(queue_t **queue, queue_t *elem) {

    //Verifica se a fila existe.
    if(!queue) {
        printf("ERRO: A fila nao existe.\n");
        return;
    }

    //Verifica se o elemento existe.
    if(!elem) {
        printf("ERRO: O elemento nao existe.\n");
        return;
    }

    //Verifica se o elemento já não pertence a outra fila.
    if(elem->next != NULL || elem->prev != NULL) {
        printf("ERRO: O elemento já pertence a outra fila.\n");
        return;
    }

    //Caso a fila seja vazia, acrescenta o primeiro elemento
    //e faz ele apontar para si mesmo.
    if((*queue) == NULL) {
        (*queue) = elem;
        (*queue)->next = (*queue);
        (*queue)->prev = (*queue);
    }
    //Caso contrário, adicionar no final da fila.
    else {
        queue_t *aux = (*queue)->prev;
        (*queue)->prev = elem;
        aux->next = elem;
        elem->prev = aux;
        elem->next = (*queue);
    }
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: apontador para o elemento removido, ou NULL se erro

queue_t *queue_remove (queue_t **queue, queue_t *elem) {

    //Verifica se a fila existe.
    if(!queue) {
        printf("ERRO: A fila nao existe.\n");
        return NULL;
    }

    //Verifica se a fila não está vazia.
    if((*queue) == NULL) {
        printf("ERRO: A fila esta vazia.\n");
        return NULL;
    }

    //Verifica se o elemento existe.
    if(!elem) {
        printf("ERRO: O elemento nao existe.\n");
        return NULL;
    }

    queue_t *aux = (*queue);
    int flag = 0;
    //Percorre por toda a fila procurando o elemento a ser removido.
    do {
        if(aux == elem) {           
           flag = 1;
           break;
        }
        aux = aux->next;
    } while(aux != (*queue));
    
    //Caso não pertença a fila, retorna NULL.
    if(!flag) {
        printf("ERRO: O elemento nao pertence a fila indicada.\n");
        return NULL;
    }

    //Corrige os ponteiros.
    elem->next->prev = elem->prev;
    elem->prev->next = elem->next;

    //Se a fila possui apenas 1 elemento, ela fica vazia.
    if(elem->next == elem)
        (*queue) = NULL;

    //Se o elemento é o primeiro da fila, corrige a posição.
    else if(elem == (*queue))
        (*queue) = elem->next;         
    
    //Apaga antigas relações do elemento com a fila.
    elem->next = NULL;
    elem->prev = NULL;

    return elem;
}

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila

int queue_size (queue_t *queue) {
    
    //Verifica se a fila existe.
    if(!queue)
        return 0;

    //Verifica se a fila é vazia.
    if(queue->next == NULL || queue->prev == NULL)
        return 0;
    
    queue_t *aux = queue;
    int contador = 0;
    //Conta todos os elementos.
    do {                
        contador += 1;
        aux = aux->next;
    } while(aux != queue);

    return contador;
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print(char *name, queue_t *queue, void print_elem(void*)) {

    printf("%s: [", name);

    if(queue) {
        queue_t *aux = queue;
        do {
            print_elem(aux);
            if(aux != queue->prev)
                printf(" ");   
            aux = aux->next;
        } while(aux != queue);
    }

    printf("]\n");
}