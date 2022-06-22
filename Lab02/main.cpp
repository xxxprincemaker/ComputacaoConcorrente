#include <iostream>
#include <thread>
#include <string>
#include <algorithm>
#include <random>
#include <pthread.h>
#include "timer.h"

using namespace std;

float *MatrizA;
float *MatrizB;
float *MatrixSaida;
float *MatrixSaidaThread;
int numeroDeThreads;

typedef struct {
    int id;
    int tam;
} threadArgs;

void *tarefa(void *arg) {
    int somaProd;
    threadArgs *args = (threadArgs *) arg;

    for (int i = args->id; i < args->tam; i += numeroDeThreads) {
        for (int j = 0; j < (args->tam); j++) {
            for (int k = 0; k < (args->tam); k++) {
                MatrixSaidaThread[i * (args->tam) + j] += MatrizA[i * (args->tam) + k] * MatrizB[k * (args->tam) + j];
            }
        }
    }
    pthread_exit(nullptr);
    return nullptr;
}

double random(int min, int max) //range : [min, max]
{
    static bool first = true;
    if (first) {
        std::srand(time(nullptr)); //seeding for the first time only!
        first = false;
    }
    return (min + std::rand() % ((max + 1) - min)) * 1.0;
}

template<typename T>
void inicializa(T &matrix, int tamMatrix) {
    for (int i = 0; i < tamMatrix; ++i) {
        for (int j = 0; j < tamMatrix; ++j) {
            matrix[i * tamMatrix + j] = random(1, 10);
        }
    }
}

template<typename T>
void inicializa(T &matrix, int tamMatrix, int valor) {
    for (int i = 0; i < tamMatrix; ++i) {
        for (int j = 0; j < tamMatrix; ++j) {
            matrix[i * tamMatrix + j] = valor;
        }
    }
}


template<typename T>
int alocaMemoria(T *&matrix, int tamMatrix) {
    matrix = new T[tamMatrix * tamMatrix];
    if (!matrix) {
        cout << "Não voi possivel alocar";
        return 1;
    }
    return 0;
}

template<typename T>
void show(T *&matrix, int tamMatrix) {
    for (int i = 0; i < tamMatrix; ++i) {
        for (int j = 0; j < tamMatrix; ++j) {
            cout << matrix[i * tamMatrix + j] << " ";
        }
        std::cout << std::endl;
    }
}

template<typename T>
void liberarMemoriaMatrix(T *&matrix, int tamanhoMatrix) {
    delete[] matrix;
}

void multiplica(int tamMatrix) {
    for (int i = 0; i < tamMatrix; ++i)
        for (int j = 0; j < tamMatrix; ++j)
            for (int k = 0; k < tamMatrix; ++k) {
                MatrixSaida[i * tamMatrix + j] += MatrizA[i * tamMatrix + k] * MatrizB[k * tamMatrix + j];
            }
}

int corretude(int tamanhoMatrix){
    cout << "Iniciando Corretude" << endl;
    for (int i = 0; i < tamanhoMatrix; i++) {
        for (int j = 0; j < tamanhoMatrix; j++) {
            if (MatrixSaida[i * tamanhoMatrix + j] != MatrixSaidaThread[i * tamanhoMatrix + j]) {
                cout << "Matriz Sequencial e Concorrente sao diferentes!" << endl;
                return 1;
            }
        }
    }
    cout << "Matriz Sequencial e Concorrente sao iguais!" << endl;
    return 0;
}

int main(int argc, char *argv[]) {

    int tamanhoDaMatriz;
    double inicio, fim, delta, sequencial, concorrente;
    pthread_t *tid;
    threadArgs *args;


    tamanhoDaMatriz = std::stoi(argv[1]);
    numeroDeThreads = std::stoi(argv[2]);

    GET_TIME(inicio);
    if (argc < 3) {
        std::cout << "Digite" << argv[0] << "<tamanho da matriz> <numero de threads> \n" << std::endl;
        return 0;
    }

    if (numeroDeThreads > tamanhoDaMatriz) {
        numeroDeThreads = tamanhoDaMatriz;
    }

    //Alocando memória na matriz

    std::cout << "Iniciando com: " << numeroDeThreads << " thread(s) " <<
              "\nDimensao da matriz: " << tamanhoDaMatriz << std::endl;


    alocaMemoria(MatrizA, tamanhoDaMatriz);
    alocaMemoria(MatrizB, tamanhoDaMatriz);
    alocaMemoria(MatrixSaida, tamanhoDaMatriz);
    alocaMemoria(MatrixSaidaThread, tamanhoDaMatriz);
    inicializa(MatrizA, tamanhoDaMatriz);
    inicializa(MatrizB, tamanhoDaMatriz);
    inicializa(MatrixSaida, tamanhoDaMatriz, 0);
    inicializa(MatrixSaidaThread, tamanhoDaMatriz, 0);

    GET_TIME(fim);
    delta = fim - inicio;
    cout << "O tempo de inicializacao (segundos): " << delta << endl;

    GET_TIME(inicio);
    multiplica(tamanhoDaMatriz);
    GET_TIME(fim);
    delta = fim - inicio;
    sequencial = delta;
    cout << "O tempo de multiplicacao de maneira sequencial (segundos): " << sequencial << endl;

    GET_TIME(inicio);
    // Alocação das estruturas
    tid = new pthread_t[numeroDeThreads];
//    tid = (pthread_t *) malloc(sizeof(pthread_t) * numeroDeThreads);

    if (!tid) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    args = new threadArgs[numeroDeThreads];
//    args = (threadArgs *) malloc(sizeof(threadArgs) * numeroDeThreads);
    if (!args) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    for (int i = 0; i < numeroDeThreads; i++) {
        args[i].id = i;
        args[i].tam = tamanhoDaMatriz;
        if (pthread_create(tid + i, nullptr, tarefa, (void *) (args + i))) {
            printf("Erro ao criar as threads\n");
            break;
            return 0;
        }
    }

    for (int i = 0; i < numeroDeThreads; i++) {
        pthread_join(*(tid + i), nullptr);
    }
    GET_TIME(fim);
    delta = fim - inicio;
    concorrente = delta;
    cout << "O tempo de multiplicacao de maneira concorrente com " << numeroDeThreads << " threads (segundos): "
                                                                   << concorrente << endl;


    GET_TIME(inicio);
    corretude(tamanhoDaMatriz);
    liberarMemoriaMatrix(MatrizA, tamanhoDaMatriz);
    liberarMemoriaMatrix(MatrizB, tamanhoDaMatriz);
    liberarMemoriaMatrix(MatrixSaida, tamanhoDaMatriz);
    liberarMemoriaMatrix(MatrixSaidaThread, tamanhoDaMatriz);
    GET_TIME(fim);
    delta = fim - inicio;
    cout << "O tempo de finalizacao (segundos): " << delta << endl;

    return 0;
}
