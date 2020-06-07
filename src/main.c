/*
 * --------------------------- Exercício 01 ----------------------------------
 *
 * Egydio Tadeu Gomes Ramos
 *
 * Programa Blink.
 *
 *  - Implementa a contagem de tempo utilizando o SysTick
 *  - Uma função de delay
 *  - Funções para acender e apagar LEDs
 *  - Teste das funções com os LEDs da placa
 */

// ----------------------------------------------------------------------------


/*
 * --------------- Inclusão das bibliotecas auxiliares ------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include "diag/Trace.h"
#include "stm32f4xx.h"

// ----------------------------------------------------------------------------

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

// ----------------------------------------------------------------------------

/*
 * ------------- Definição de constantes e variáveis globais ------------------
 */

// Apelidos auxilares para configuração das GPIOS

#define GPIO_PORTD_CLKEN 1U<<3
#define LED_GREEN_MODER  1U<<24
#define LED_ORANGE_MODER 1U<<26
#define LED_RED_MODER    1U<<28
#define LED_BLUE_MODER   1U<<30

// Apelidos auxiliares para manipulação do registrador de saída ODR

#define LED_GREEN  1U<<12
#define LED_ORANGE 1U<<13
#define LED_RED    1U<<14
#define LED_BLUE   1U<<15

// Variável para contagem do tempo em ms
volatile uint32_t msTicks = 0;

// Pilhas das tarefas
uint32_t orange_stack[40];
uint32_t green_stack[40];

// Apontadores das pilhas das tarefas
uint32_t* sp_orange = &orange_stack[40];
uint32_t* sp_green = &green_stack[40];

// ----------------------------------------------------------------------------

/*
 * ---------------- Protótipos das funções auxiliares ------------------------
 */
void gpio_config();
uint32_t get_ticks();
void delay_ms(uint32_t delay);
void led_turnOn(uint32_t ledPin);
void led_turnOff(uint32_t ledPin);
void initialize_task_stack();

// ----------------------------------------------------------------------------

/*
 * ---------------- Protótipos das funções de Tasks ---------------------------
 */
void blink_orange();
void blink_green();

// ----------------------------------------------------------------------------

/*
 * -------------------------- Função Main -------------------------------------
 */

int
main(int argc, char* argv[])
{
  HAL_Init();

  initialize_task_stack();

  SysTick_Config(SystemCoreClock/16000); // habilita interrupções do systick

  gpio_config(); // configura o clock no barramento e as portas GPIO

  // Cria apontadores para as tasks para garantir que as funções sejam definidas
  // no arquivo compilado
  void (*blink_orange_task)() = &blink_orange;
  void (*blink_green_task)() = &blink_green;

  while (1)
    {
      blink_orange_task();
      blink_green_task();
    }

  return 0;
}

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Tasks --------------------------------
 */

void
blink_orange()
{
  while (1)
    {
      led_turnOn(LED_ORANGE);
      delay_ms(1000);
      led_turnOff(LED_ORANGE);
      delay_ms(1000);
    }
}

void
blink_green()
{
  while(1)
    {
      led_turnOn(LED_GREEN);
      delay_ms(1000);
      led_turnOff(LED_GREEN);
      delay_ms(1000);
    }
}

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Funções Auxiliares -------------------
 */

// Função de interrupção do SysTick
void
SysTick_Handler()
{
  msTicks++;
}

uint32_t
get_ticks()
{
  uint32_t ticks;

  __disable_irq(); // desabilita interrupções enquanto lê a variável msTicks
                   // para evitar race conditions

  ticks = msTicks;

  __enable_irq(); // habilita novamente as interrupções

  return ticks;
}

void
delay_ms(uint32_t delay)
{
  uint32_t startTime = get_ticks();

  while (get_ticks() - startTime < delay); // laço dummy para esperar o delay
}

void
gpio_config()
{
  RCC->AHB1ENR |= GPIO_PORTD_CLKEN; // habilita o clock no barramento

  GPIOD->MODER |= LED_GREEN_MODER  | // habilita as portas dos leds como saídas
                  LED_ORANGE_MODER |
                  LED_RED_MODER    |
                  LED_BLUE_MODER;
}

void
led_turnOn(uint32_t ledPin)
{
  GPIOD->ODR |= ledPin; // Atribui nível alto na saída do pino
}

void
led_turnOff(uint32_t ledPin)
{
  GPIOD->ODR &= ~(ledPin); // Atribui nível baixo na saída do pino
}

void
initialize_task_stack()
{
  // Sequência dos registradores:
  //    xPSR
  //    PC
  //    LR
  //    R12
  //    R3
  //    R2
  //    R1
  //    R0

  // Inicialização da pilha da task blink_orange
  orange_stack[39] = 1U << 24;
  orange_stack[38] = 0x08003DE4;
  orange_stack[37] = 0x0000AAAA;
  orange_stack[36] = 0x0000BBBB;
  orange_stack[35] = 0x0000CCCC;
  orange_stack[34] = 0x0000DDDD;
  orange_stack[33] = 0x0000EEEE;
  orange_stack[32] = 0x0000FFFF;

  sp_orange--;

  // Inicialização da pilha da task blink_green
  green_stack[39] = 1U << 24;
  green_stack[38] = 0x08003E0A;
  green_stack[37] = 0x0000ABCD;
  green_stack[36] = 0x0000BCDE;
  green_stack[35] = 0x0000CDEF;
  green_stack[34] = 0x0000DEFA;
  green_stack[33] = 0x0000EFAB;
  green_stack[32] = 0x0000FABC;

  sp_green--;
}


// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
