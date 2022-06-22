#include<pthread.h>
#include <cstdlib>
//#include<unistd.h> // Biblioteca Sleep para o Unix/Linux
#include <Windows.h> // Biblioteca Sleep para o Windows
#include<iostream>
#include <unistd.h>

#define L 4
#define E 8

//variaveis do problema
int leit=0;
int escr=0;
int escr_espera=0;

//variaveis para sincronizacao
pthread_mutex_t mutex;
pthread_cond_t cond_leit, cond_escr;

//entrada leitura
void InicLeit (int id) {
    pthread_mutex_lock(&mutex);
    std::cout << "L[" << id << "] quer ler" << std::endl;
    while(escr > 0 || escr_espera > 0 ) {
        //checa se tem alguma thread de escrita escrevendo ou esperando
        std::cout << "L[" << id << "] Bloqueou" << std::endl;
        pthread_cond_wait(&cond_leit, &mutex);
        std::cout << "L[" << id << "] Desbloqueou" << std::endl;
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}

//saida leitura
void FimLeit (int id) {
    pthread_mutex_lock(&mutex);
    std::cout << "L[" << id << "] terminou de ler" << std::endl;
    leit--;
    if(leit==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}

//entrada escrita
void InicEscr (int id) {
    pthread_mutex_lock(&mutex);
    std::cout << "E[" << id << "] Quer escrever" << std::endl;
    escr_espera++; //aumenta o contador de threads de escrita em espera
    while((leit>0) || (escr>0)) {
        std::cout << "E[" << id << "] Bloqueou" << std::endl;
        pthread_cond_wait(&cond_escr, &mutex);
        std::cout << "E[" << id << "] Desbloqueou" << std::endl;
    }
    escr_espera--; //diminui o contador de threads de escrita em espera
    escr++;
    pthread_mutex_unlock(&mutex);
}

//saida escrita
void FimEscr (int id) {
    pthread_mutex_lock(&mutex);
    std::cout << "E[" << id << "] terminou de escrever" << std::endl;
    escr--;
    pthread_cond_signal(&cond_escr);
    if(escr_espera <= 0){
        //checa se ha threads de escrita em espera para nao dar broadcast atoa
        pthread_cond_broadcast(&cond_leit);
    }
    pthread_mutex_unlock(&mutex);
}

//thread leitora
void * leitor (void * arg) {
    int *id = (int *) arg;
    while(true) {
        InicLeit(*id);
        std::cout << "Leitor " << *id << " esta lendo" << std::endl;
        FimLeit(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(nullptr);
}

//thread leitora
void * escritor (void * arg) {
    int *id = (int *) arg;
    while(true) {
        InicEscr(*id);
        std::cout << "Escritor " << *id << " esta escrevendo" << std::endl;
        FimEscr(*id);
        sleep(1);
    }
    free(arg);
    pthread_exit(nullptr);
}

//funcao principal
int main() {
    //identificadores das threads
    pthread_t tid[L+E];
    int id[L+E];

    //inicializa as variaveis de sincronizacao
    pthread_mutex_init(&mutex, nullptr);
    pthread_cond_init(&cond_leit, nullptr);
    pthread_cond_init(&cond_escr, nullptr);

    //cria as threads leitoras
    for(int i=0; i<L; i++) {
        id[i] = i+1;
        if(pthread_create(&tid[i], nullptr, leitor, (void *) &id[i])) exit(-1);
    }

    //cria as threads escritoras
    for(int i=0; i<E; i++) {
        id[i+L] = i+1;
        if(pthread_create(&tid[i+L], nullptr, escritor, (void *) &id[i+L])) exit(-1);
    }


    pthread_exit(nullptr);
    return 0;
}
