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
 * -------------------- Definição de constantes -------------------------------
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
static volatile uint32_t msTicks = 0;

// ----------------------------------------------------------------------------

/*
 * ---------------- Protótipos das funções auxiliares ------------------------
 */
void GPIO_Config();


// ----------------------------------------------------------------------------

/*
 * -------------------------- Função Main -------------------------------------
 */

int
main(int argc, char* argv[])
{
  HAL_Init();

  GPIO_Config();

  GPIOD->ODR |= LED_GREEN | LED_RED;

  while(1);

  return 0;
}

// ----------------------------------------------------------------------------

/*
 * ----------------------- Definição das Funções Auxiliares -------------------
 */

void
GPIO_Config()
{
  RCC->AHB1ENR |= GPIO_PORTD_CLKEN; // habilita o clock no barramento

  GPIOD->MODER |= LED_GREEN_MODER  | // habilita as portas dos leds como saídas
                  LED_ORANGE_MODER |
                  LED_RED_MODER    |
                  LED_BLUE_MODER;
}


// ----------------------------------------------------------------------------

#pragma GCC diagnostic pop

// ----------------------------------------------------------------------------
