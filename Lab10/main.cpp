#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

//threads leitoras
#define L 3
#define E 5 \
//threads escritoras
#define NTHREADS L+E

using namespace std;

/* Variaveis globais */
int varContadorLeitor=0;
int varContadorEscritor=0;

/* Semáforos */
sem_t em_e, em_l, escr, leit;

/* Thread Leitora */
void * leitor (void * arg) {
    int *id = (int *) arg;
    while(true) {
        sem_wait(&leit);
        sem_wait(&em_l);
        cout<< "L["<< *id << "] quer ler" << endl;
        varContadorLeitor++; if(varContadorLeitor == 1) sem_wait(&escr);
        sem_post(&em_l);
        sem_post(&leit);
        cout<< "L["<< *id << "] esta lendo" << endl;
        //Le Algo!
        sem_wait(&em_l);
        cout<< "L["<< *id << "] terminou de ler" << endl;
        varContadorLeitor--; if(varContadorLeitor == 0) sem_post(&escr);
        sem_post(&em_l);
        sleep(1);
    }
    delete arg;
    pthread_exit(nullptr);
}

/* Thread Escritora */
void * escritor (void * arg) {
    int *id = (int *) arg;
    while(true) {
        sem_wait(&em_e);
        varContadorEscritor++; if(varContadorEscritor == 1) sem_wait(&leit);
        cout<< "E["<< *id << "] quer escrever" << endl;
        cout<< "E["<< *id << "] esta esperando" << endl;
        sem_post(&em_e);
        sem_wait(&escr);
        cout<< "E["<< *id << "] esta escrevendo" << endl;
        //Escreve Algo!
        cout<< "E["<< *id << "] terminou de escrever" << endl;
        sem_post(&escr);
        sem_wait(&em_e);
        varContadorEscritor--; if(varContadorEscritor == 0) sem_post(&leit);
        sem_post(&em_e);
        sleep(1);
    }
    delete arg;
    pthread_exit(nullptr);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int i;
    pthread_t threads[NTHREADS];   // Threads totais.
    int identificadorThread[NTHREADS];         //Identificador das Threads

    // Inicia os semáforos
    sem_init(&em_l, 0, 1);
    sem_init(&em_e, 0, 1);
    sem_init(&leit, 0, 1);
    sem_init(&escr, 0, 1);

    //cria as threads leitoras
    for(i=0; i<L; i++) {
        identificadorThread[i] = i + 1;
        if(pthread_create(&threads[i], nullptr, leitor, (void *) &identificadorThread[i])) exit(-1);
    }

    //cria as threads escritoras
    for(i=0; i<E; i++) {
        identificadorThread[i + L] = i + 1;
        if(pthread_create(&threads[i + L], nullptr, escritor, (void *) &identificadorThread[i + L])) exit(-1);
    }

    /* Espera todas as threads completarem */
    for (pthread_t thread : threads) {
        pthread_join(thread, nullptr);
    }

    /* Desaloca variaveis semafaros termina */
    sem_destroy(&em_l);
    sem_destroy(&em_e);
    sem_destroy(&leit);
    sem_destroy(&escr);

}