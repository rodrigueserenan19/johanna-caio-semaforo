# Implementação de Semáforo Atômico

## Introdução

Este código implementa um semáforo atômico em linguagem C para sistemas embarcados. Semáforos são comumente usados para controlar o acesso concorrente a recursos compartilhados em sistemas multitarefa. A atomicidade aqui significa que as operações sobre o semáforo são executadas de forma indivisível, garantindo consistência e prevenindo condições de corrida.

## Estrutura do Código

### Arquivos Fonte

- `cmsis_gcc.h`: Inclui instruções intrínsecas específicas do compilador GCC.


## Funcionamento do Código

O código implementa um semáforo atômico que consiste em três principais funções:

### `OS_sem_init`

```c
void OS_sem_init(AtomicSemaphore *sem, int32_t initialValue);
```
Esta função é utilizada para inicializar um semáforo. Ela recebe um ponteiro para a estrutura AtomicSemaphore e um valor inicial que determina a quantidade de recursos disponíveis.

### `OS_sem_wait`
```c
void OS_sem_wait(AtomicSemaphore *sem);
```

Esta função é usada para aguardar até que recursos estejam disponíveis. Se recursos estiverem disponíveis, ela os consome (decrementa o contador de recursos disponíveis). Se nenhum recurso estiver disponível, a função entra em um loop até que recursos estejam disponíveis.

### `OS_sem_signal`
```c
void OS_sem_signal(AtomicSemaphore *sem);
```
Esta função é usada para sinalizar (liberar) recursos. Ela incrementa o contador de recursos disponíveis, indicando que um recurso foi liberado para ser utilizado por outra parte do código.

### Funcionamento Interno

As operações sobre o semáforo são protegidas por desabilitação e reabilitação de interrupções para garantir a atomicidade.
A função OS_sem_wait realiza uma operação de leitura e escrita atômica no contador do semáforo para garantir que apenas um thread possa modificar o contador por vez.
A função OS_sem_signal opera de forma semelhante, garantindo que o incremento do contador seja atômico.
