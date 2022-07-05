#include <iostream>
#include <pthread.h>
#include <semaphore.h>

#define L 3
#define E 4
#define NTHREADS L+E

sem_t em_e, em_l, escr, leit, texto; //semaforos
int varEscritor=0, varLeitor=0; //globais

typedef struct{
    int id; //identificador do elemento que a thread ira processar
} tArgs;

[[noreturn]] void *leitor(void * arg){
    auto *args = (tArgs*) arg;
    while (1){

        sem_wait(&leit);
        sem_wait(&em_l);
        varLeitor++;
        if(varLeitor == 1) sem_wait(&escr);
        sem_post(&em_l);
        sem_post(&leit);
        sem_wait(&texto);
        std::cout << "Thread [" << args->id << "] Esta se Preparando para Ler" << std::endl;
        sem_post(&texto);
        std::cout << "Thread [" << args->id << "] Esta Lendo" << std::endl;
        for(int i = 0; i < 777777777.7; i++) int k = i+1; // Processamento Bobo
        std::cout << "Thread [" << args->id << "] Leu" << std::endl;
        sem_wait(&em_l); varLeitor--;
        if(varLeitor == 0) {
            sem_post(&escr);
        }
        sem_post(&em_l);
    }
}

[[noreturn]] void *escritor(void * arg){
    auto *args = (tArgs*) arg;

    while (1){

        sem_wait(&em_e);
        varEscritor++;
        if(varEscritor == 1) {
            sem_wait(&leit);
        }
        sem_post(&em_e);
        sem_wait(&escr); //Mutex Escritor
        std::cout << "Thread [" << args->id << "] Esta Pensando no Que vai Escrever" << std::endl;
        std::cout << "Thread [" << args->id << "] Esta Escrevendo" << std::endl;
        for(int i = 0; i < 777777777.7; i++) int k = i+1; // Processamento Bobo
        std::cout << "Thread [" << args->id << "] Escreveu" << std::endl;
        sem_post(&escr); //Fim Mutex Escritor
        sem_wait(&em_e);
        if (varEscritor > 1) for(int w = 0; w < E; w++) varEscritor--;
        else varEscritor--; // Por algum motivo estava acumulando var escritor
        if(varEscritor == 0) {
            sem_post(&leit);
        }
        sem_post(&em_e);
    }
}

int main() {
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) varEscritor a variavel de condicao */
    sem_init(&em_e, 0, 1);
    sem_init(&texto, 0, 1);
    sem_init(&em_l, 0, 1);
    sem_init(&escr, 0, 1);
    sem_init(&leit, 0, 1);

    tArgs *args = new tArgs[NTHREADS];
    if(!args) {std::cout << "Erro ao alocar Memoria"; return 2;}

    /* Cria as threads Escritoras*/
    /* Cria as threads Leitoras*/
    for(int i=0; i < L; i++){
        (args+i)->id = i+1;
        pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(leitor), (void*) (args + i));
    }

    for(int i=0; i < E; i++){
        (args+i+L)->id = i+1;
        pthread_create(&threads[i], nullptr, reinterpret_cast<void *(*)(void *)>(escritor), (void*) (args + i + L));
    }

    /* Espera todas as threads completarem */
    for (unsigned long long thread : threads) {
        pthread_join(thread, nullptr);
    }

    /* Desaloca variaveis varEscritor termina */
    delete [] args;
    sem_destroy(&em_e);
    sem_destroy(&texto);
    sem_destroy(&em_l);
    sem_destroy(&escr);
    sem_destroy(&leit);
}
