#include <stdio.h>
#include "queue.h"

//Alunos
//Gustavo Henrique Zeni
//Ianca Polizelo

//------------------------------------------------------------------------------
// Conta o numero de elementos na fila
// Retorno: numero de elementos na fila
int queue_size(queue_t *queue)
{

    if (queue == NULL)
    {
        printf("queue null!\n");
        return (0);
    }

    queue_t *aux = queue;
    int i = 1;

    while (aux->next != queue)
    {
        aux = aux->next;
        i++;
    }

    return (i);
}

//------------------------------------------------------------------------------
// Percorre a fila e imprime na tela seu conteúdo. A impressão de cada
// elemento é feita por uma função externa, definida pelo programa que
// usa a biblioteca. Essa função deve ter o seguinte protótipo:
//
// void print_elem (void *ptr) ; // ptr aponta para o elemento a imprimir

void queue_print(char *name, queue_t *queue, void print_elem(void *))
{
    queue_t *aux = queue->next;

    for (int i = 0; i < sizeof(name); i++)
    {
        printf("%c", name[i]);
    }

    printf("[");

    print_elem(queue);

    printf(" ");

    while (aux != queue)
    {
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
int queue_append(queue_t **queue, queue_t *elem)
{
    /**
     seja A o primeiro elemento (**queue)
     seja B o segundo e último elemento (**queue->prev)
     seja C um novo elemento (*elem)
    **/

    // verificar se a fila existe
    if (queue == NULL)
    {
        printf("Erro: a fila não existe");
        return -1;
    }

    // verificar se o elemento existe
    if (elem == NULL)
    {
        printf("Erro: o elemento não existe");
        return (-1);
    }

    // verificar se o elemento pertence a outra fila
    if ((elem)->prev != NULL || (elem)->next != NULL)
    {
        printf("Erro: o elemento pertence a outra fila");
        return (-1);
    }

    // verificar se queue é vazio
    if ((*queue) == NULL)
    {
        (*queue) = elem;
        (elem)->next = (elem);
        (elem)->prev = (elem);
        return (0);
    }
    else
    {
        // C->next = A
        elem->next = (*queue);
        // C->prev = B
        elem->prev = (*queue)->prev;
        // A->prev = C
        (*queue)->prev->next = elem;
        // B->next = C
        (*queue)->prev = elem;
    }

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
int queue_remove(queue_t **queue, queue_t *elem)
{

    //**queue -> ponteiro para um ponteiro (vetor)
    // recebe: (queue_t**) &fila0,
    //*elem -> ponteiro
    // recebe: (queue_t*) aux (mesma coisa que queue_t->aux)



    // verificar se a queue é vazia
    if (queue == NULL)
    {
        printf("Erro: a fila não existe");
        return (-1);
    }

    // verificar se a fila existe
    if ((*queue) == NULL)
    {
        printf("Erro: a fila está vazia");
        return (-1);
    }

    // verificar se o elemento existe
    if (elem == NULL)
    {
        printf("Erro: o elemento não existe");
        return (-1);
    }

    // verificar se o elemento pertence a uma fila
    if ((elem)->prev == NULL && (elem)->next == NULL)
    {
        printf("Erro: o elemento não pertence a uma fila");
        return (-1);
    }

    // uma cópia da cabeça da fila
    queue_t *queue_check = (*queue);

    // uma cópia do ponteiro do elemento a ser removido
    queue_t *aux = elem;


    // verificar se o elemento pertence a outra fila
    /*
     enquanto queue_check (auxiliar de ponteiro
     que aponta pra cabeça da fila) for diferente do
     elemento a ser removido e o próximo for diferente
     dele mesmo (ter dado uma volta completa ou ser o único
     elemento da fila)
    */
    while (queue_check != elem && queue_check->next != (*queue)) {
        queue_check = queue_check->next;
    }

    // se for diferente de elem então deu uma volta inteira e
    // não encontrou o elemento a ser removido
    if (queue_check != elem) {
        printf("Erro: elemento nao pertence a lista");
        return (-1);
    }

    // se é ele mesmo, remove ele mesmo da fila e deixa
    // ela vazia
    if (queue_check == queue_check->next) {
        //(*queue) == NULL;
    }
    // se não é ele mesmo, cria-se um auxiliar para a remoção
    else {

        // A->next aponta pro C
        aux->prev->next = elem->next;

        // C->prev aponta pro A
        aux->next->prev = elem->prev;

        if (aux==(*queue)) {
            (*queue) = aux->next;
        }
    }

    aux->next = NULL;
    aux->prev = NULL;

    return (0);
}
