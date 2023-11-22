#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h> // Para a função sleep

#define NUM_THREADS 10 // Defina o número de threads aqui

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

'''Função auxiliar da thread'''
void* funcaoHelper(void* idThread) {
    long tid = (long) idThread; // Convertendo para o tipo correto
    
    printf("Thread %ld iniciada\n", tid);
    
    // Dormir por um período aleatório entre 1 e 5 segundos
    int tempoDormindo = rand() % 5 + 1;
    sleep(tempoDormindo);

    // Acordar a thread principal
    pthread_mutex_lock(&mutex);
    printf("Thread %ld dormiu por %d segundos. Acordando a thread principal.\n", tid, tempoDormindo);
    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);

    // Finalizar com o código de retorno igual ao idThread
    pthread_exit(idThread);
}

int main(int argc, char *argv[]) {
    pthread_t vetorThreads[NUM_THREADS];
    int statusRetorno;
    void *statusFinalizacao;
    
    //Inicializar gerador de números aleatórios
    srand((unsigned int)time(NULL));

    //Criar as threads
    for (long i = 0; i < NUM_THREADS; i++) {
        statusRetorno = pthread_create(&vetorThreads[i], NULL, funcaoHelper, (void *)i);
        if (statusRetorno) {
            printf("Erro ao criar thread: %d\n", statusRetorno);
            exit(-1);
        }
    }

    '''Aguardar até que todas as threads estejam "dormindo"'''
    pthread_mutex_lock(&mutex);
    printf("Thread principal aguardando todas as threads dormirem...\n");
    pthread_cond_wait(&cond, &mutex);
    pthread_mutex_unlock(&mutex);

    '''Realizando um join com todas as threads'''
    for (int i = 0; i < NUM_THREADS; i++) {
        statusRetorno = pthread_join(vetorThreads[i], &statusFinalizacao);
        if (statusRetorno) {
            printf("Erro ao executar pthread_join:%d\n", statusRetorno);
            exit(-1);
        }
        printf("Join finalizado para a thread: %d com o código: %ld\n", i, (long)statusFinalizacao);
    }

    printf("Finalizando o método main\n");
    pthread_exit(NULL);
}
