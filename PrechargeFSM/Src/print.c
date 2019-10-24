/*
 * print.c
 *
 *  Created on: Oct 23, 2019
 *      Author: Minya
 */

#include "stdio.h"
#include "print.h"
#include "usart.h"

int __io_putchar(int ch) {
	uint8_t c[1];
	c[0] = ch & 0x00FF;
	HAL_UART_Transmit(&huart2, &*c, 1, 10);
	return ch;
}

void print(char* formatString) {
	printf(formatString);
}


