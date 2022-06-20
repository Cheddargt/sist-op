#include "queue.h"
#include "ppos_data.h"
#include "ppos.h"

#include<stdlib.h>
#include<stdio.h>
#include <signal.h>
#include <sys/time.h>

//---------- Alunos
//Gustavo Henrique Zeni
//Ianca Polizelo

#define STACKSIZE 32768
#define QUANTUM 20
//Variáveis globais
task_t ContextMain, dispatcher, *tarefaAtual;
int relogio, contador;
//Fila de prontas e concluidas
task_t *prontas, *concluidas;
// estrutura que define um tratador de sinal (deve ser global ou estatica)
struct sigaction action ;
// estrutura de inicialização to timer
struct itimerval timer;

void tratador ()
{
	relogio += 1;
	if(tarefaAtual->tid != 1) {
			tarefaAtual->cputime += 1;
			tarefaAtual->quantum -= 1;
		if(tarefaAtual->quantum < 1)
			task_yield();
	}
}

task_t * scheduler() {
	return prontas;
}

void BodyDispatcher(void *arg) {	
	task_t *next;

	while(prontas) {
		next = scheduler(); // scheduler é uma funçao

		if(next) {
			// açoes antes de lançar a tarefa "next", se houverem 
			next = (task_t *)queue_remove((queue_t **)&prontas, (queue_t *)next);
			task_switch(next);
			// açoes após retornar da tarefa "next", se houverem
		}
	}

	task_exit(0);
}

void pingpong_init() {
	setvbuf(stdout, 0, _IONBF, 0);
	prontas    			 = NULL;
	concluidas 			 = NULL;
	relogio				 = 0;
	contador 			 = 1;

	ContextMain.next		= NULL;
	ContextMain.prev		= NULL;
	ContextMain.tid 		= 0;
	ContextMain.initTime	= 0;
	ContextMain.estatica	= 0;
	ContextMain.dinamica	= 0;
	ContextMain.quantum		= QUANTUM;
	ContextMain.cputime		= 0;
	ContextMain.activations	= 0;

	getcontext(&(ContextMain.context));

	char *stack = malloc (STACKSIZE) ;
   	if (stack)
   	{
		ContextMain.context.uc_stack.ss_sp = stack ;	
		ContextMain.context.uc_stack.ss_size = STACKSIZE;
		ContextMain.context.uc_stack.ss_flags = 0;
		ContextMain.context.uc_link = 0;
   	}
   	else
   	{
		return -1; 
   	}	

	queue_append((queue_t **) &prontas, (queue_t*) &ContextMain);

	task_create(&dispatcher, (void*)(*BodyDispatcher), NULL);

	tarefaAtual = &ContextMain;

	// registra a ação para o sinal de timer SIGALRM
	action.sa_handler = tratador ;
	sigemptyset (&action.sa_mask) ;
	action.sa_flags = 0 ;
	if (sigaction (SIGALRM, &action, 0) < 0)
	{
		perror ("Erro em sigaction: ") ;
		exit (1) ;
	}

	// ajusta valores do temporizador
	timer.it_value.tv_usec 		= 1000; // primeiro disparo, em micro-segundos
	timer.it_value.tv_sec  		= 0;    // primeiro disparo, em segundos
	timer.it_interval.tv_usec 	= 1000; // disparos subsequentes, em micro-segundos
	timer.it_interval.tv_sec  	= 0;    // disparos subsequentes, em segundos
	// arma o temporizador ITIMER_REAL (vide man setitimer)
	if (setitimer (ITIMER_REAL, &timer, 0) < 0)
	{
		perror ("Erro em setitimer: ") ;
		exit (1) ;
	}

	task_switch(&dispatcher);
}

int task_create(task_t *task, void (*start_func)(void *), void *arg) {

	// task          : 	estrutura que referencia a tarefa criada
	// start_routine : 	função que será executada pela tarefa
	// arg           : 	parâmetro a passar para a tarefa que está sendo criada

	char *stack;

	getcontext(&(task->context));

	stack = malloc (STACKSIZE) ;
   	if (stack)
   	{
		//Inicializa atributos da struct ContextPing
		//stack_t uc_stack: Pilha utilizada pelo contexto.

		//ss_sp: Aponta para a base da pilha.
		task->context.uc_stack.ss_sp = stack ;
		//ss_size: Tamanho (em bytes) da pilha apontada por ss_sp.
		task->context.uc_stack.ss_size = STACKSIZE;
		//ss_flags: inteiro bitwise
		task->context.uc_stack.ss_flags = 0;
		//Pontero para o contexto que será resumido quando
		//esse contexto retornar.
		task->context.uc_link = 0;
   	}
   	else
   	{
		return -1; 
   	}

	makecontext(&(task->context), (void*)(start_func), 1, arg);

	// Precisa criar uma ID para a task
	task->prev 			= NULL;
	task->next 			= NULL;
	task->tid 			= contador;
	task->initTime		= 0;
	task->estatica 		= 0;
	task->dinamica  	= 0;
	task->quantum 		= QUANTUM;
	task->cputime		= 0;
	task->activations 	= 0;
	task->initTime		= systime();

	contador += 1;

	if(arg)
    	queue_append((queue_t **) &prontas, (queue_t*) task);

	return task->tid; //ID da task ou valor negativo, se houver erro
}

int task_switch(task_t *task) {
	// task: tarefa que irá assumir o processador
	if(task) {
		task->activations += 1;
		task_t *aux = tarefaAtual;
		tarefaAtual = task;
		swapcontext (&(aux->context), &(task->context));
		return 0;
	}

	return -1; //valor negativo se houver erro
}

void task_exit(int exit_code) {
	// exit_code: código de término devolvido pela tarefa corrente
	printf("Task %d exit: %d ms run time, %d ms CPU time, %d activations\n", 
		tarefaAtual->tid, relogio - tarefaAtual->initTime, tarefaAtual->cputime, tarefaAtual->activations);
	
	if(tarefaAtual == &dispatcher) {
		//Libera a fila de prontas concluídas.
		while(concluidas)
			queue_remove((queue_t **)&concluidas, (queue_t *)concluidas);

		task_switch(&ContextMain);
	}
	else {
		queue_append((queue_t **)&concluidas, (queue_t *)tarefaAtual);
		task_switch(&dispatcher);
	}
}

int task_id() {
	return tarefaAtual->tid; //Identificador numérico (ID)
}

void task_yield() {
	// Libera o processador para a próxima tarefa, retornando à fila de prontas prontas
	if(tarefaAtual->tid != 1) {
		queue_append((queue_t **) &prontas, (queue_t *) tarefaAtual);
		tarefaAtual->dinamica	= tarefaAtual->estatica;	//Reinicia a prioridade dinamica
		tarefaAtual->quantum	= QUANTUM;					//Reinicia o quantum
	}

	task_switch(&dispatcher);
}

void task_setprio(task_t *task, int prio) {
	// Ajusta a prioridade estática da tarefa task para o valor prio 
	// (que deve estar entre -20 e +20). Caso task seja nulo, ajusta a prioridade 
	// da tarefa atual.

	// Define a prioridade estática de uma tarefa (ou da tarefa atual)
	if(task) {
		task->estatica = prio;
		task->dinamica  = prio;
	}

	tarefaAtual->estatica = prio;
	tarefaAtual->dinamica  = prio;
}

int task_getprio(task_t *task) {
	//Obtém o valor da prioridade estática da tarefa task (ou da tarefa corrente, se task for nulo)
	if(task)
		return task->estatica; // retorna a prioridade estática de uma tarefa.
	//Caso task seja NULL.
	return tarefaAtual->estatica;
}

// retorna o relógio atual (em milisegundos)
unsigned int systime () {	
	return relogio;
}
