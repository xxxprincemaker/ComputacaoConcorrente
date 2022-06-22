#include <iostream>
#include "time.h"
#include <random>
#include <ctime>

using namespace std;

long long int tamanhoVetor;
double *vetor;
double *vetorSaidaSequencial;
double *vetorSaidaConcorrente;

int numeroDeThreads;

//Cria estrutura para realizar Threads.
typedef struct {
    int id;
} threadArgs;

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

void *tarefa(void *arg) {
    double maior = vetorSaidaConcorrente[0];
    double menor = vetorSaidaConcorrente[1];
    threadArgs *args = (threadArgs *) arg;

    //Logica de blocos.
    long long int id = (long long int) args->id; //indentificador da thread;
    long long tamBloco = tamanhoVetor/numeroDeThreads; //tamanho de cada thread;
    long long ini = id * tamBloco; // elemento inicial do bloco da thread;
    long long fim = ini + tamBloco; // elemento final nao processado
    if (args->id == numeroDeThreads-1) fim = tamanhoVetor;
    else fim = ini + tamBloco; //trata resto se houver

    // calcular o menor valor da thread
    for (long long int i = ini; i < fim; i++ ) {
        if(vetor[i] > maior){
            maior = vetor[i];
        }
        if (vetor[i] < menor){
            menor = vetor[i];
        }
    }

    vetorSaidaConcorrente[0] = maior;
    vetorSaidaConcorrente[1] = menor;

    pthread_exit(NULL);
    return NULL;
}

float random() {
    float r = 1.0 + static_cast <float> (rand()) / ( static_cast <float> (RAND_MAX/(10.0-1.0)));
    return r;
}



template<typename T>
void inicializa(T &vetor, long long int tamVetor) {
    float valor;
    for (long long int i = 0; i < tamVetor; i++) {
        valor = random();
        vetor[i] = valor;
    }
}

template<typename T>
void liberarMemoriaVetor(T *&vetor, long long int tamanhoMatrix) {
    delete[] vetor;
}

int corretude(long long int tamanhoVetor){
    cout << "Iniciando Corretude" << endl;
    for (int i = 0; i < tamanhoVetor; i++) {
        if(vetorSaidaSequencial[i] != vetorSaidaConcorrente[i]){
            cout << "Matriz Sequencial e Concorrente nao sao iguais!" << endl;
            return 1;
        }
    }
    cout << "Matriz Sequencial e Concorrente sao iguais!" << endl;
    return 0;
}

void encontraMaiorMenor(long long int tamanhoVetor) {
    float maior = vetor[0];
    float menor = vetor[0];

    for (long long int i = 0; i < tamanhoVetor; ++i) {
        if (vetor[i] > maior) {
            maior = vetor[i];
        }
        if (vetor[i] < menor) {
            menor = vetor[i];
        }
    }

    vetorSaidaSequencial[0] = maior;
    vetorSaidaSequencial[1] = menor;
}

int main(int argc, char *argv[]) {

    double inicio, fim, delta, sequencial, concorrente;
    pthread_t *tid;
    threadArgs *args;

    float *retorno;

    tamanhoVetor = atoll(argv[1]);
    numeroDeThreads = std::stoi(argv[2]);

    GET_TIME(inicio);
    if (argc < 3) {
        std::cout << "Digite" << argv[0] << "<tamanho do vetor> <numero de threads> \n" << std::endl;
        return 0;
    }

    std::cout << "Iniciando com: " << numeroDeThreads << " thread(s) " <<
              "\nDimensao do Vetor: " << tamanhoVetor << std::endl;


    if (numeroDeThreads > tamanhoVetor) {
        numeroDeThreads = tamanhoVetor;
    }

    alocaMemoria(vetor, tamanhoVetor);
    alocaMemoria(vetorSaidaSequencial, 2);
    alocaMemoria(vetorSaidaConcorrente, 2);
    alocaMemoria(retorno, 2);
    inicializa(vetor, tamanhoVetor);
    inicializa(vetorSaidaSequencial, 2,0);
    inicializa(vetorSaidaConcorrente, 2,0);

    GET_TIME(fim);
    delta = fim - inicio;
    cout << "O tempo de inicializacao (segundos): " << delta << endl;

    GET_TIME(inicio);
    encontraMaiorMenor(tamanhoVetor);
    GET_TIME(fim);
    delta = fim - inicio;
    sequencial = delta;
    cout << "O tempo de encontrar o maior e o menor valor do vetor de maneira sequencial (segundos): " << sequencial << endl;

    GET_TIME(inicio);
    // Alocação das estruturas
    tid = new pthread_t[numeroDeThreads];

    if (!tid) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    args = new threadArgs[numeroDeThreads];
    if (!args) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    //criar threads
    vetorSaidaConcorrente[0] = vetor[0];
    vetorSaidaConcorrente[1] = vetor[0];
    for (int i = 0; i < numeroDeThreads; i++) {
        args[i].id = i;
        if (pthread_create(tid + i, nullptr, tarefa, (void *) (args + i))) {
            printf("Erro ao criar as threads\n");
            break;
        }
    }

    for (int i = 0; i < numeroDeThreads; i++) {
        pthread_join(*(tid + i), nullptr);
//        vetorSaidaConcorrente[1] = retorno[1];
//        vetorSaidaConcorrente[0] = retorno[0];
    }
    GET_TIME(fim);
    delta = fim - inicio;
    concorrente = delta;
    cout << "O tempo de encontrar o maior e o menor valor do vetor de maneira concorrente com " << numeroDeThreads << " threads (segundos): "
         << concorrente << endl;

    GET_TIME(inicio);
    corretude(2);
    liberarMemoriaVetor(vetor, tamanhoVetor);
    liberarMemoriaVetor(vetorSaidaSequencial, tamanhoVetor);
    liberarMemoriaVetor(vetorSaidaConcorrente, tamanhoVetor);
    liberarMemoriaVetor(retorno, 2);
    delete [] args;
    delete [] tid;
    GET_TIME(fim);
    delta = fim - inicio;
    cout << "O tempo de finalizacao (segundos): " << delta << endl;

    cout << "Aceleracao: " << sequencial/concorrente << endl;

    return 0;
}
