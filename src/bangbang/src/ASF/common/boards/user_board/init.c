/**
* \file
*
* \brief User board initialization template
*
*/
/*
* Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
*/

#include <asf.h>
#include <board.h>
#include <conf_board.h>
#include <avr/io.h>
#include <adc.h>
#include <stdio.h>

static void adc_init(void)
{
	struct adc_config adc_conf;
	struct adc_channel_config adcch_conf;
	adc_read_configuration(&MY_ADC, &adc_conf);
	adcch_read_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
	adc_set_conversion_parameters(&adc_conf, ADC_SIGN_OFF, ADC_RES_12,
	ADC_REF_BANDGAP);
	adc_set_conversion_trigger(&adc_conf, ADC_TRIG_MANUAL, 1, 0);
	adc_set_clock_rate(&adc_conf, 200000UL);
	adcch_set_input(&adcch_conf, ADCCH_POS_PIN0, ADCCH_NEG_NONE, 1);
	adc_write_configuration(&MY_ADC, &adc_conf);
	adcch_write_configuration(&MY_ADC, MY_ADC_CH, &adcch_conf);
}

/************************************************************************/
/* printf support functions                                             */
/************************************************************************/
#if CONF_BOARD_ENABLE_DEBUG
static int uart_putchar(char c, FILE *stream);
static FILE mystdout = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
static int uart_putchar(char c, FILE *stream)
{
	if (c == '\n')
	uart_putchar(usart_putchar,'\r');
	usart_putchar(USART_SERIAL_DEBUG, c);
}
#endif //CONF_BOARD_ENABLE_DEBUG

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	* for, e.g., the I/O pins. The initialization can rely on application-
	* specific board configuration, found in conf_board.h.
	*/
	
	ioport_init();
	
#if CONF_BOARD_ENABLE_DEBUG	
	/* setup uart debug port */
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTD,3), IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTD,2), IOPORT_DIR_INPUT);
	
	{ 
		usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE_115200,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
		};
		usart_serial_init(USART_SERIAL_DEBUG, &usart_options);
	}
	
	/* setup printf */
	stdout = &mystdout;
#endif //CONF_BOARD_ENABLE_DEBUG
	
#if CONF_BOARD_ENABLE_WIFI	
	/* setup uart wifi port */
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTE,3), IOPORT_DIR_OUTPUT);
	ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTE,2), IOPORT_DIR_INPUT);
	
	{
		usart_serial_options_t usart_options = {
		.baudrate = USART_SERIAL_BAUDRATE_9600,
		.charlength = USART_SERIAL_CHAR_LENGTH,
		.paritytype = USART_SERIAL_PARITY,
		.stopbits = USART_SERIAL_STOP_BIT
		};
		usart_serial_init(USART_SERIAL_WIFI, &usart_options);
	}

#endif //CONF_BOARD_ENABLE_WIFI
	
	//adc_init();
	
	//delay_init();
	
}
