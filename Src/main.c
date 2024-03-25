#include <stdint.h>
#include "miros.h"
#include "stm32f1xx_hal.h"
#include "stm32f1xx_hal_gpio.h"
#include "semaphore.h"

/* Definições para o LED */
#define LED_PIN GPIO_PIN_13
#define LED_PORT GPIOC

/* Definições para o semáforo */
typedef struct {
    AtomicSemaphore sem; // Semáforo atômico para controlar o acesso ao LED
} LedSemaphore;

LedSemaphore ledSem; // Instância do semáforo

uint32_t stack_blinky1[40];
OSThread blinky1;

uint32_t stack_blinky2[40];
OSThread blinky2;

void main_blinky1() {
    while (1) {
        OS_sem_wait(&ledSem.sem); // Aguarda pelo semáforo para acessar o LED

        /* Inverte o estado do LED */
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

        OS_sem_signal(&ledSem.sem); // Libera o semáforo após acessar o LED

        /* Aguarda 0.5 segundo */
        OS_delay(TICKS_PER_SEC);
        OS_yield(); // Chama a função de yield para permitir que outras tarefas sejam executadas
    }
}

void main_blinky2() {
    while (1) {
        OS_sem_wait(&ledSem.sem); // Aguarda pelo semáforo para acessar o LED

        /* Inverte o estado do LED */
        HAL_GPIO_TogglePin(LED_PORT, LED_PIN);

        OS_sem_signal(&ledSem.sem); // Libera o semáforo após acessar o LED

        /* Aguarda 1 segundo */
        OS_delay(5*TICKS_PER_SEC);
        OS_yield(); // Chama a função de yield para permitir que outras tarefas sejam executadas
    }
}

uint32_t stack_idleThread[40];

int main() {
    /* Inicializa o MIROS */
    OS_init(stack_idleThread, sizeof(stack_idleThread));

    /* Inicializa o HAL */
    HAL_Init();

    /* Inicializa o pino do LED */
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    __HAL_RCC_GPIOC_CLK_ENABLE(); // Habilita o clock para o GPIOC
    GPIO_InitStruct.Pin = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);

    /* Inicializa o semáforo */
    OS_sem_init(&ledSem.sem, 1); // Inicializa o semáforo com 1 recurso disponível (inicialmente liberado)

    /* Inicia a thread para piscar o LED 1 */
    OSThread_start(&blinky1,
                   3U, /* Prioridade */
                   &main_blinky1,
                   stack_blinky1, sizeof(stack_blinky1));

    /* Inicia a thread para piscar o LED 2 */
    OSThread_start(&blinky2,
                   2U, /* Prioridade */
                   &main_blinky2,
                   stack_blinky2, sizeof(stack_blinky2));

    /* Transfere o controle para o RTOS para executar as threads */
    OS_run();

    return 0;
}
