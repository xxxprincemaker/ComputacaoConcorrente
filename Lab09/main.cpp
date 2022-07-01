#include <pthread.h>
#include <cstdio>
#include <iostream>
#include <semaphore.h>

#define NTHREADS 5

typedef struct{
    int id; //identificador do elemento que a thread ira processar
} tArgs;

/* Variaveis globais */
sem_t condt1, meios;
sem_t mutex;
int messages_left = 3;

/* Thread 5 */
void * inicio() {
    //Semafaro
    std::cout << "Seja bem-vindo!" << std::endl;
    sem_post(&meios); //Libera as 3 mensagens
    sem_post(&meios);
    sem_post(&meios);
    pthread_exit(nullptr);
}

/* Thread 2, 3 e 4 */
void * meio(void *arg) {
    tArgs *args = (tArgs*) arg;

    sem_wait(&meios);

    //Sessão Crítica
    sem_wait(&mutex); // Uma thread de cada vez vai executar o código abaixo.
    if(args->id == 1) {

        std::cout << "Fique a vontade." << std::endl;

    }
    else if(args->id == 2) {

        std::cout << "Sente-se por favor." << std::endl;

    }
    else {
        std::cout << "Aceita um copo de agua?" << std::endl;
    }

    messages_left--;

    if(messages_left == 0){ // Se todas as mensagens que tinham para serem executadas foram executadas, libera a condicao da thread 1
        sem_post(&condt1);
    }

    sem_post(&mutex); // Fim da sessao critica


    pthread_exit(nullptr);
}

/* Thread 1 */
void * fim() {
    //Semafaro

    sem_wait(&condt1); //Espera o semafaro da thread 1
    std::cout << "Volte Sempre!" << std::endl;

    pthread_exit(nullptr);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    pthread_t threads[NTHREADS];

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    sem_init(&condt1, 0,0);
    sem_init(&meios, 0, 0);
    sem_init(&mutex, 0, 1);

    tArgs *args = new tArgs[3];
    if(!args) {std::cout << "Erro ao alocar Memoria"; return 2;}


    /* Cria as threads */
    pthread_create(&threads[0], nullptr, reinterpret_cast<void *(*)(void *)>(fim), nullptr);
    for(int i=1; i <= 3; i++){
        (args+i)->id = i;
        pthread_create(&threads[i], nullptr, meio, (void*) (args+i));
    }
    pthread_create(&threads[4], nullptr, reinterpret_cast<void *(*)(void *)>(inicio), nullptr);

    /* Espera todas as threads completarem */
    for (unsigned long long thread : threads) {
        pthread_join(thread, nullptr);
    }

    /* Desaloca variaveis e termina */
    delete [] args;
    sem_destroy(&condt1);
    sem_destroy(&meios);
    sem_destroy(&mutex);
}
