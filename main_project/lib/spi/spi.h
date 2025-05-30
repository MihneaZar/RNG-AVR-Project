#ifndef SPI_H
#define SPI_H

#include <stdint.h>

/* SPI config */
#define SPI_PORT	PORTB
#define SPI_DDR 	DDRB
#define SPI_MISO	PB1
#define SPI_MOSI	PB2
#define SPI_SCK 	PB3

void SPI_init();
uint8_t SPI_exchange(uint8_t data);

#endif // SPI_H

