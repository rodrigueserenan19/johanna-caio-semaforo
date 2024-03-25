#include <stdint.h>
#include "miros.h"
#include "cmsis_gcc.h"

typedef struct {
    int32_t count; // Contador de recursos disponíveis (deve ser do tipo int32_t para suportar valores negativos)
} AtomicSemaphore;

void OS_sem_init(AtomicSemaphore *sem, int32_t initialValue) {
    sem->count = initialValue;
}

void OS_sem_wait(AtomicSemaphore *sem) {
    int32_t expected;
    do {
        // Desabilita as interrupções
        __disable_irq();

        // Carrega o valor atual de forma atômica
        expected = __LDREXW((uint32_t *)&(sem->count));

        // Verifica se há recursos disponíveis (expected > 0)
        if (expected > 0) {
            // Tenta decrementar o contador de forma atômica
            int storeResult = __STREXW(expected - 1, (uint32_t *)&(sem->count));
            // Se a operação de armazenamento foi bem-sucedida (storeResult == 0),
            // então saímos do loop, pois conseguimos adquirir o recurso
            if (storeResult == 0) {
                // Habilita as interrupções novamente
                __enable_irq();
                break;
            }
            // Se a operação de armazenamento falhou, o loop continuará,
            // tentando novamente até que seja bem-sucedido.
        }
        // Se não houver recursos disponíveis (expected <= 0),
        // o loop continuará, tentando novamente.

        // Habilita as interrupções novamente antes de repetir o loop
        __enable_irq();
    } while (1);
}

void OS_sem_signal(AtomicSemaphore *sem) {
    int32_t expected;
    do {
        // Desabilita as interrupções
        __disable_irq();

        expected = __LDREXW((uint32_t *)&(sem->count));
        // Tenta incrementar e armazenar atomicamente
        if (__STREXW(expected + 1, (uint32_t *)&(sem->count)) == 0) {
            // Se bem-sucedido, quebra o loop
            // Habilita as interrupções novamente
            __enable_irq();
            break;
        }
        // Se não for bem-sucedido, repete

        // Habilita as interrupções novamente antes de repetir o loop
        __enable_irq();
    } while (1);
}
