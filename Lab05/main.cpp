#include <pthread.h>
#include <cstdio>
#include <iostream>

#define NTHREADS 5

typedef struct{
  int id; //identificador do elemento que a thread ira processar
} tArgs;

/* Variaveis globais */
int primeiro_contador = 0;
int ultimo_contador = 0;
pthread_mutex_t primeiro_contador_mutex;
pthread_mutex_t ultimo_contador_mutex;
pthread_cond_t meio_cond;
pthread_cond_t ultimo_cond;


/* Thread 5 */
void * inicio() {
  std::cout << "Seja bem-vindo!" << std::endl;

  pthread_mutex_lock(&primeiro_contador_mutex);
  primeiro_contador++;
  pthread_mutex_unlock(&primeiro_contador_mutex);

  pthread_mutex_lock(&ultimo_contador_mutex);
  pthread_cond_broadcast(&meio_cond);
  pthread_mutex_unlock(&ultimo_contador_mutex);

  pthread_exit(nullptr);
  return nullptr;
}

/* Thread 2, 3 e 4 */
void * meio(void *arg) {
  tArgs *args = (tArgs*) arg;
  pthread_mutex_lock(&primeiro_contador_mutex);
  if (primeiro_contador == 0){
      pthread_cond_wait(&meio_cond, &primeiro_contador_mutex);
  }
  pthread_mutex_unlock(&primeiro_contador_mutex);

  if(args->id == 1)
    std::cout <<"Fique a vontade." << std::endl;
  else if(args->id == 2)
      std::cout << "Sente-se por favor." << std::endl;
  else
      std::cout << "Aceita um copo de agua?." << std::endl;


  pthread_mutex_lock(&ultimo_contador_mutex);
  ultimo_contador++;
  if (ultimo_contador == 3) {
      pthread_cond_signal(&ultimo_cond);
  }
  pthread_mutex_unlock(&ultimo_contador_mutex);

  pthread_exit(nullptr);
  return nullptr;
}

/* Thread 1 */
void * fim() {
  pthread_mutex_lock(&ultimo_contador_mutex);
  if (ultimo_contador < 3) {
     pthread_cond_wait(&ultimo_cond, &ultimo_contador_mutex);
  }
  pthread_mutex_unlock(&ultimo_contador_mutex);
  printf("Volte sempre!\n");
  pthread_exit(nullptr);

  return nullptr;
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&primeiro_contador_mutex, nullptr);
  pthread_mutex_init(&ultimo_contador_mutex, nullptr);
  pthread_cond_init (&meio_cond, nullptr);
  pthread_cond_init (&ultimo_cond, nullptr);

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
  for (int i = 0; i < NTHREADS; i++) {
  pthread_join(threads[i], nullptr);
  }

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&primeiro_contador_mutex);
  pthread_mutex_destroy(&ultimo_contador_mutex);
  pthread_cond_destroy(&meio_cond);
  pthread_cond_destroy(&ultimo_cond);
}