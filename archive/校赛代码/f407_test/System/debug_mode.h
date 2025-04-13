#ifndef __DEBUG_MODE_H
#define __DEBUG_MODE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "usart.h"

#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */
PUTCHAR_PROTOTYPE
{

    HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}

#ifdef __cplusplus
}

#endif


#endif
