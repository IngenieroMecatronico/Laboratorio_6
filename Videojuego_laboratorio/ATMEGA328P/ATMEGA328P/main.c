/*
 * ATMEGA328P.c
 *
 * Created: 24/03/2025 23:23:14
 * Author : ferma
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdlib.h>
#include <stdio.h>
/*
const uint8_t arriba = 1;
const uint8_t abajo = 2;
const uint8_t derecha = 3;
const uint8_t izquierda = 4;
const uint8_t arriba = 5;
const uint8_t arriba = 6;
*/
volatile uint8_t numero;
char buffer[1];  // Buffer para almacenar el número convertido en string
volatile uint8_t BufferTX;		  //Variable doonde se guarda el valor le?do.

void initports(void);
void boton(const uint8_t botonop);
void initUart115200(void);   //Inicializo mi TX y RX pero lo usar? a 115200
void WriteTextUART(char * Texto);
void writeUART(char Caracter);


int main(void)
{
	initports();
	initUart115200();   //Inicializo mi TX y RX pero lo usar? a 115200
	numero = 7;
	sei();
    /* Replace with your application code */
    while (1) 
    {
		switch(numero){
			case 0:
				//WriteTextUART("Arriba:");
				sprintf(buffer, "%d", numero);  // Convierte el número a string
				WriteTextUART(buffer);  // Enviar el string por UART
				//writeUART(numero+48);
				//WriteTextUART("\n\r");
				numero = 7;
				_delay_ms(500);
			break;
			case 1:
				//WriteTextUART("Abajo:");
				//writeUART(numero+48);
				sprintf(buffer, "%d", numero);  // Convierte el número a string
				WriteTextUART(buffer);  // Enviar el string por UART
				//WriteTextUART("\n\r");
				numero = 7;
				_delay_ms(500);
			break;
			case 2:
				//WriteTextUART("Derecha:");
				//writeUART(numero+48);
				sprintf(buffer, "%d", numero);  // Convierte el número a string
				WriteTextUART(buffer);  // Enviar el string por UART
				//WriteTextUART("\n\r");
				numero = 7;
				_delay_ms(500);
			break;
			case 3:
				//WriteTextUART("Izquierda:");
				//writeUART(numero+48);
				sprintf(buffer, "%d", numero);  // Convierte el número a string
				WriteTextUART(buffer);  // Enviar el string por UART
				//WriteTextUART("\n\r");
				numero = 7;
				_delay_ms(500);
			break;
			case 4:
				//WriteTextUART("Botón A:");
				//writeUART(numero+48);
				sprintf(buffer, "%d", numero);  // Convierte el número a string
				WriteTextUART(buffer);  // Enviar el string por UART
				//WriteTextUART("\n\r");
				numero = 7;
				_delay_ms(500);
			break;
			case 5:
				//WriteTextUART("Botón B:");
				//writeUART(numero+48);
				sprintf(buffer, "%d", numero);  // Convierte el número a string
				WriteTextUART(buffer);  // Enviar el string por UART
				//WriteTextUART("\n\r");
				numero = 7;
				_delay_ms(500);
			break;
			default:
			break;
		}
    }
}

void initports(void){
	//Inicializamos los puertos de interés
	//Apago todos los leds
	//PB0 es salida, lo de más es entrada.
	DDRB = 0;
	DDRD &= ~(1<<PORTD7);
	//Configurar los botones como downs, pero se conecta con pullup.
	PORTB |= (1<<PORTB0)|(1<<PORTB1)|(1<<PORTB2)|(1<<PORTB3)|(1<<PORTB4)|(1<<PORTB5);
	PORTD |= (1<<PORTD7);
	//PORTB |= (1<<PORTC0)|(1<<PORTC1)|~(1<<PORTC2)|~(1<<PORTC3)|~(1<<PORTC4)|~(1<<PORTC5);
	//Vamos a habilitar la interrupción de los puertos B.
	PCICR |= (1<<PCIE0)|(1<<PCIE2);
	//Habilitamos la máscara.
	PCMSK0 |= (1<<PCINT0)|(1<<PCINT1)|(1<<PCINT2)|(1<<PCINT3)|(1<<PCINT4)|(1<<PCINT5);
	PCMSK2 |= (1<<PCINT23);
}

ISR(PCINT0_vect){
	if (!(PINB & (1<<PORTB0)))   //Si se detecta un cambio en PINB y fué en el puerto PB1, encender la bandera para ejecutar el display.
	{
		numero = 1;
	}
	if (!(PINB & (1<<PORTB1))) //Si se detecta cambio en PINB y fué Puerto PB2, preguntar si ya se ejecutó el ciclo del display.
	{
		numero = 2;
	}
	if (!(PINB & (1<<PORTB2))) //Si se detecta cambio en PINB y fué Puerto PB2¿3, preguntar si ya se ejecutó el ciclo del display.
	{
		numero = 3;
	}
	if (!(PINB & (1<<PORTB3)))   //Si se detecta un cambio en PINB y fué en el puerto PB1, encender la bandera para ejecutar el display.
	{
		numero = 4;
	}
	if (!(PINB & (1<<PORTB4))) //Si se detecta cambio en PINB y fué Puerto PB2, preguntar si ya se ejecutó el ciclo del display.
	{
		numero = 5;
	}
	
}
ISR(PCINT2_vect){
	if (!(PIND & (1<<PORTD7))) //Si se detecta cambio en PINB y fué Puerto PB2¿3, preguntar si ya se ejecutó el ciclo del display.
	{
		numero = 0;
	}
}


void WriteTextUART(char * Texto){
	uint8_t i;
	for (i=0; Texto[i]!= '\0'; i++){
		while (!(UCSR0A & (1<<UDRE0)) );
		UDR0 = Texto[i];
	}
}

void writeUART(char Caracter){
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = Caracter;
	//PORTD = Caracter;
}

ISR(USART_RX_vect){  //uint8_t bit2; //Interrupci?n para obtener los valores de UDR0 en BUFFERTX.
	BufferTX = UDR0;
	while(!(UCSR0A & (1<<UDRE0)));
	UDR0 = BufferTX;
}

void initUart115200(void){
	//Paso 1: RX como entrada y TX como salida.
	DDRD |= (1<<DDD1);
	DDRD &=	~(1<<DDD0);
	//Paso 2: Configurar UCSR0A
	UCSR0A = 0;
	//Paso 3: Configurar UCSR0B: habilitamos ISR de recepci?n y RX Y TX.
	UCSR0B = 0;
	UCSR0B |= (1<<RXCIE0) | (1<<RXEN0) | (1<<TXEN0); //Estoy activando la bandera que me indica que se recibi? el mensaje.
	//Paso 4: COnfigurar UCSR0C: Primero as?ncrono, sin pariedad, bit 1 de stop, Data bits/ 8 bits,
	UCSR0C = 0;
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);
	//Paso 5: Configurar velocidad de Baudrate: 115200
	UBRR0 = 8;
}