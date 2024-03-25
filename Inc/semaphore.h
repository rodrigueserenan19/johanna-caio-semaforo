#ifndef ATOMIC_SEMAPHORE_H
#define ATOMIC_SEMAPHORE_H

#include <stdint.h>
#include "cmsis_gcc.h"

typedef struct {
    int32_t count; // Contador de recursos disponíveis (deve ser do tipo int32_t para suportar valores negativos)
} AtomicSemaphore;

/**
 * Inicializa o semáforo atômico com um valor inicial.
 *
 * @param sem            Ponteiro para o semáforo atômico.
 * @param initialValue   Valor inicial do semáforo.
 */
void OS_sem_init(AtomicSemaphore *sem, int32_t initialValue);

/**
 * Aguarda pelo recurso do semáforo. Bloqueia até que o recurso esteja disponível.
 *
 * @param sem   Ponteiro para o semáforo atômico.
 */
void OS_sem_wait(AtomicSemaphore *sem);

/**
 * Libera o recurso do semáforo.
 *
 * @param sem   Ponteiro para o semáforo atômico.
 */
void OS_sem_signal(AtomicSemaphore *sem);

#endif /* ATOMIC_SEMAPHORE_H */
