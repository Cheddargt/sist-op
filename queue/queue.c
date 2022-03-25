#include <stdio.h>
#include "queue.h"


//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size (queue_t *queue) {

    if (queue == NULL) { return (0); }

    queue_t *aux = queue->next;
    int i = 1;

    while (aux->next != queue){ i++; }

    return (i);
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print (char *name, queue_t *queue, void print_elem (void*)) {
    queue_t *aux = queue->next;

    for (int i = 0; i < sizeof(name); i++) {
        printf("%c", name[i]);
    }

    printf("[");

    print_elem(queue);

    printf(" ");

    while (aux != queue) {
        print_elem(aux);
        printf(" ");
    }

    printf("]");
}

//------------------------------------------------------------------------------
// Insere um elemento no final da fila.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - o elemento deve existir
// - o elemento nao deve estar em outra fila
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_append (queue_t **queue, queue_t *elem) {
    /**
     seja A o primeiro elemento (**queue)
     seja B o segundo e último elemento (**queue->prev)
     seja C um novo elemento (*elem)
    **/

    // verificar se a fila existe
        // return -1

    // verificar se o elemento existe
    if (elem == NULL) {
        printf ("Erro: o elemento não existe");
        return (-1);
    }

    // verificar se o elemento pertence a outra fila
    if ((elem)->prev != NULL || (elem)->next != NULL) {
        printf ("Erro: o elemento pertence a outra fila");
        return (-1);
    }

    // verificar se queue é vazio
    if ((*queue) == NULL) {
        (elem)->next = (elem);
        (elem)->prev = (elem);
        return (0);
    }

    // TODO: resolver problema
    queue_t *aux = (*queue)->prev;

    // A->prev = C
    (*queue)->prev = elem;
    // B->next = C
    aux->next = elem;
    // C->prev = B
    elem->prev = aux;
    // C->next = A
    elem->next = (*queue);
    return (0);
}

//------------------------------------------------------------------------------
// Remove o elemento indicado da fila, sem o destruir.
// Condicoes a verificar, gerando msgs de erro:
// - a fila deve existir
// - a fila nao deve estar vazia
// - o elemento deve existir
// - o elemento deve pertencer a fila indicada
// Retorno: 0 se sucesso, <0 se ocorreu algum erro
int queue_remove (queue_t **queue, queue_t *elem) {

    // verificar se a queue é vazia
    if (queue == NULL) {
        return (-1);
    }

    // verificar se a fila existe
        // return -1

    // verificar se o elemento existe
    if (elem == NULL) {
        printf ("Erro: o elemento não existe");
        return (-1);
    }

    // verificar se o elemento pertence a outra fila
    if ((elem)->prev == NULL && (elem)->next == NULL) {
        printf ("Erro: o elemento não pertence a uma fila");
        return (-1);
    }

    queue_t **queue_check = queue;

    while ((*queue_check) != elem) {
        if ((*queue_check)->next == *queue) {
            printf("Erro: elemento nao pertence a lista");
            return (-1);
        }

        *queue_check=(*queue_check)->next;
    }

    queue_t *aux_prev = (elem)->prev;
    queue_t *aux_next = (elem)->next;

    // C->prev aponta pro A
    aux_next->prev = aux_prev;
    // A->next aponta pro C
    aux_prev->next = aux_next;

    return (0);
}

