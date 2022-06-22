#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NTHREADS  2 //total de threads a serem criadas
int vetorReferencia[10000];
int vetorAoQuadrado[10000];


//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
    int idThread, nThreads;
} t_Args;

//funcao executada pelas threads
void *elevarAoQuadrado(void *threadId)
{
    t_Args id;
    id = *(t_Args *) threadId;

    if (id.idThread == 1)
    {
        for (int i = 0; i < 5000; i++)
        {
            vetorAoQuadrado[i] = vetorReferencia[i] * vetorReferencia[i];
        }
    } else
    {
        for (int i = 5000; i < 10000; i++)
        {
            vetorAoQuadrado[i] = vetorReferencia[i] * vetorReferencia[i];
        }
    }
    pthread_exit(NULL);
    return NULL;
}

//funcao principal do programa
int main()
{
    int t = 0;
    int igual = 1;
    while (t < 10000){
        vetorReferencia[t] = t;
        t++;
    }

    pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
    int thread; //variavel auxiliar
    t_Args arg[NTHREADS]; //receberá os argumentos para a thread

    for (thread = 0; thread < NTHREADS; thread++)
    {
        arg[thread].idThread = thread;
        arg[thread].nThreads = NTHREADS;
        if (pthread_create(&tid_sistema[thread], NULL, elevarAoQuadrado,
                           (void *) &(arg[thread]))) {
            printf("ERRO: pthread_create()\n");
            exit(-1);
        }
    }

    //--espera todas as threads terminarem
    for (thread = 0; thread < NTHREADS; thread++)
    {
        if (pthread_join(tid_sistema[thread], NULL)) {
            printf("ERRO: pthread_join() \n");
            exit(-1);
        }
    }

    for (int i = 0; i < 10000; i++)
    {
        if (vetorAoQuadrado[i] != (vetorReferencia[i] * vetorReferencia[i])) {
            igual = 0;
            break;
        }
    }

    if (igual) {
        printf("Os valores finais estao corretos\n");
    } else {
        printf("Os valores finais não estao corretos\n");
    }

    return 0;
}