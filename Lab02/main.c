#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "time.h"

float *m1;
float *saida;
int numeroDeThreads;

typedef struct {
    int id;
    int tam;
} threadArgs;

void *tarefa(void *arg) {
    int somaprod;
    threadArgs *args = (threadArgs *) arg;

    for (int i = args->id; i < args->tam; i += numeroDeThreads) {
        for (int j = 0; j < (args->tam); j++) {
            somaprod = 0;
            for (int k = 0; k < (args->tam); k++) {
                somaprod += m1[i * (args->tam) + k] * m1[k * (args->tam) + j];
                saida[i * (args->tam) + j] = somaprod;
            }
        }
    }
    pthread_exit(NULL);
}

int corretude(float *matrizSaida, int tam) {
    int tamanho = tam;
    int soma;
    float *matrizParametro;
    float *saidaParametro;
    float *saida = matrizSaida;

    matrizParametro = (float *) malloc(sizeof(float) * tam * tam);
    if (matrizParametro == NULL) {
        printf("Não conseguiu alocar\n");
        return 2;
    }
    saidaParametro = (float *) malloc(sizeof(float) * tam * tam);
    if (saidaParametro == NULL) {
        printf("Não conseguiu alocar\n");
        return 2;
    }

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            matrizParametro[i * tamanho + j] = 1;
            saidaParametro[i * tamanho + j] = 0;
        }
    }

    for (int i = 0; i < tamanho; i++)
        for (int j = 0; j < tamanho; j++) {
            soma = 0;
            for (int k = 0; k < tamanho; k++) {
                soma += matrizParametro[i * tamanho + k] * matrizParametro[k * tamanho + j];
                saidaParametro[i * tamanho + j] = soma;
            }
        }

    for (int i = 0; i < tamanho; i++) {
        for (int j = 0; j < tamanho; j++) {
            if (saidaParametro[i * tamanho + j] != saida[i * tamanho + j]) {
                printf("Não é igual\n");
                return 0;
            }
        }
    }
    printf("Está correto\n");
    return 1;
}

int main(int argc, char *argv[]) {
    int tamanhoDaMatriz;
    double inicio, fim, delta;
    pthread_t *tid;
    threadArgs *args;


    GET_TIME(inicio);
    if (argc < 3) {
        printf("Digite %s <tamanho da matriz> <numero de threads> \n", argv[0]);
        return 0;
    }

    tamanhoDaMatriz = atoi(argv[1]);
    numeroDeThreads = atoi(argv[2]);

    if (numeroDeThreads > tamanhoDaMatriz){
        numeroDeThreads = tamanhoDaMatriz;
    }

    printf("Iniciando com: %d thread(s) \nDimensao da matriz: %d\n", numeroDeThreads, tamanhoDaMatriz);

    m1 = (float *) malloc(sizeof(float) * tamanhoDaMatriz * tamanhoDaMatriz);
    if (m1 == NULL) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    saida = (float *) malloc(sizeof(float) * tamanhoDaMatriz * tamanhoDaMatriz);

    if (saida == NULL) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    //Inicializando o vetor
    for (int i = 0; i < tamanhoDaMatriz; i++) {
        for (int j = 0; j < tamanhoDaMatriz; j++) {
            m1[i * tamanhoDaMatriz + j] = 1;
            saida[i * tamanhoDaMatriz + j] = 0;
        }
    }

    GET_TIME(fim);
    delta = fim - inicio;
    printf("\nO tempo de inicializacao: %lf\n", delta);

    GET_TIME(inicio);
    // Alocação das estruturas
    tid = (pthread_t *) malloc(sizeof(pthread_t) * numeroDeThreads);

    if (tid == NULL) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    args = (threadArgs *) malloc(sizeof(threadArgs) * numeroDeThreads);
    if (args == NULL) {
        printf("Não conseguiu alocar\n");
        return 0;
    }

    for (int i = 0; i < numeroDeThreads; i++) {
        (args + i)->id = i;
        (args + i)->tam = tamanhoDaMatriz;
        if (pthread_create(tid + i, NULL, tarefa, (void *) (args + i))) {
            printf("Erro ao criar as threads\n");
            return 0;
        }
    }
    for (int i = 0; i < numeroDeThreads; i++) {
        pthread_join(*(tid + i), NULL);
    }
    GET_TIME(fim);
    delta = fim - inicio;
    printf("O tempo de multiplicacao: %lf\n", delta);


    //corretude(saida, tamanhoDaMatriz);
    GET_TIME(inicio);
    free(m1);
    free(saida);
    free(tid);
    free(args);
    GET_TIME(fim);
    delta = fim - inicio;
    printf("O tempo de finalizacao: %lf\n", delta);
    return 0;

}