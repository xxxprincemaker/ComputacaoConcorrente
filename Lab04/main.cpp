#include <iostream>
#include "time.h"
#include <random>

using namespace std;


int *vetorEntrada;
double *vetorSaidaSequencial;
double *vetorSaidaConcorrente;

int i_global = 0;
pthread_mutex_t bastao;

int numeroDeThreads;
long long int tamanhoVetor;

//Cria estrutura para realizar Threads.
typedef struct {
    int id;
    double *vetorSaidaConcorrente;
} threadArgs;

int ehPrimo(long long int n) {
    if (n <= 1)
        return 0;
    if (n == 2)
        return 1;
    if (n % 2 == 0)
        return 0;
    for (int i = 3; i < sqrt(n) + 1; i += 2)
        if (n % i == 0)
            return 0;
    return 1;
}

template<typename T>
int alocaMemoria(T *&vetor, long long int tamVetor) {
    vetor = new T[tamVetor];
    if (!vetor) {
        cout << "Não voi possivel alocar";
        return 1;
    }
    return 0;
}

template<typename T>
void inicializa(T &vetor, long long int tamVetor, int valor) {
    for (long long int i = 0; i < tamVetor; ++i) {
        vetor[i] = valor;
    }
}

void *tarefa(void * arg) {

    int i_local;

    do{
        pthread_mutex_lock(&bastao);
        i_local = i_global; i_global++;
        pthread_mutex_unlock(&bastao);
        if (ehPrimo(vetorEntrada[i_local])) vetorSaidaConcorrente[i_local] = sqrt(vetorEntrada[i_local]);
        else vetorSaidaConcorrente[i_local] = vetorEntrada[i_local];

    }while ( i_local  < tamanhoVetor);

    pthread_exit(nullptr);
    return nullptr;
}

int random() {
    int r = (rand() % tamanhoVetor+1) + tamanhoVetor;
    return r;
}

template<typename T>
void inicializa(T &vetor, long long int tamVetor) {
    int valor;
    for (long long int i = 0; i < tamVetor; i++) {
        valor = random();
        vetor[i] = valor;
    }
}

template<typename T>
void liberarMemoriaVetor(T *&vetor, long long int tamanhoMatrix) {
    delete[] vetor;
}

int corretude(long long int tamanhoVetor) {
    cout << "Iniciando Corretude" << endl;
    for (int i = 0; i < tamanhoVetor; i++) {
        if (vetorSaidaSequencial[i] != vetorSaidaConcorrente[i]) {
            cout << "Vetor de Saida e Concorrente nao sao iguais!" << endl;
            return 1;
        }
    }
    cout << "Vetor de Saida Sequencial e Concorrente sao iguais!" << endl;
    return 0;
}

void processaPrimos(long long int tamanhoVetor) {

    for (long long int i = 0; i < tamanhoVetor; ++i) {
        if (ehPrimo(vetorEntrada[i])) {
            vetorSaidaSequencial[i] = sqrt(vetorEntrada[i]);
        } else {
            vetorSaidaSequencial[i] = vetorEntrada[i];
        }
    }
}

int main(int argc, char *argv[]) {

    double inicio, fim, delta, sequencial, concorrente;
    pthread_t *tid;
    threadArgs *args;


    tamanhoVetor = atoll(argv[1]);
    numeroDeThreads = std::stoi(argv[2]);

    GET_TIME(inicio);
    if (argc < 3) {
        std::cout << "Digite" << argv[0] << "<tamanho do vetorEntrada> <numero de threads> \n" << std::endl;
        return 0;
    }

    std::cout << "Iniciando com: " << numeroDeThreads << " thread(s) " <<
              "\nDimensao do Vetor: " << tamanhoVetor << std::endl;


    if (numeroDeThreads > tamanhoVetor) {
        numeroDeThreads = tamanhoVetor;
    }

    alocaMemoria(vetorEntrada, tamanhoVetor);
    alocaMemoria(vetorSaidaSequencial, tamanhoVetor);
    alocaMemoria(vetorSaidaConcorrente, tamanhoVetor);
    inicializa(vetorEntrada, tamanhoVetor);


    GET_TIME(fim);
    delta = fim - inicio;
    cout << "O tempo de inicializacao (segundos): " << delta << endl;

    GET_TIME(inicio);
    processaPrimos(tamanhoVetor);
    GET_TIME(fim);
    delta = fim - inicio;
    sequencial = delta;
    cout << "O tempo de executar o ProcessaPrimos de maneira sequencial (segundos): "
         << sequencial << endl;

    GET_TIME(inicio);
    // Alocação das estruturas
    tid = new pthread_t[numeroDeThreads];

    if (!tid) {
        printf("Não conseguiu alocar\n");
        return 0;
    }


    pthread_mutex_init(&bastao, nullptr);

    //criar threads
    for (int i = 0; i < numeroDeThreads; i++) {
        if (pthread_create(&tid[i], nullptr, tarefa, nullptr) ){
            printf("Erro ao criar as threads\n");
            break;
        }
    }

    for (int i = 0; i < numeroDeThreads; i++) {
        pthread_join(tid[i], nullptr);
    }


    GET_TIME(fim);
    delta = fim - inicio;
    concorrente = delta;
    cout << "O tempo de executar o ProcessaPrimos de maneira concorrente com "
         << numeroDeThreads << " threads (segundos): "
         << concorrente << endl;

    GET_TIME(inicio);
    corretude(tamanhoVetor);
    liberarMemoriaVetor(vetorEntrada, tamanhoVetor);
    liberarMemoriaVetor(vetorSaidaSequencial, tamanhoVetor);
    liberarMemoriaVetor(vetorSaidaConcorrente, tamanhoVetor);
    pthread_mutex_destroy(&bastao);
    delete[] tid;
    GET_TIME(fim);
    delta = fim - inicio;
    cout << "O tempo de finalizacao (segundos): " << delta << endl;

    cout << "Aceleracao: " << (sequencial) / (concorrente) << endl;

    return 0;
}
