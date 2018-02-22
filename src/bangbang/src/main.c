/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "pt.h"
#include <string.h>
#include <stdio.h>

static PT_THREAD( PT_Setup(struct pt *pt) )
{
	struct pt_timer tm;
	PT_BEGIN(pt);
	
	
	//TIMER_OPEN(&cygGlobalTimer, &tm, 10);

	while(1)
	{
		PT_EXIT(pt);
	}
	
	//TIMER_CLOSE(&cygGlobalTimer, &tm);
	
	PT_END(pt);
}

static PT_THREAD( PT_WiFiHandle(struct pt *pt) )
{
	static int nData;
	PT_BEGIN(pt);
	
	while(1)
	{
		usart_serial_write_packet(USART_SERIAL_DEBUG, "Test String", strlen("Test String"));
		usart_serial_write_packet(USART_SERIAL_WIFI, "Test String", strlen("Test String"));
	}
	
	PT_END(pt);
}

static PT_THREAD( PT_AdcHandle(struct pt *pt) )
{
	PT_BEGIN(pt);

	uint16_t result;
	while(1)
	{
		adc_enable(&MY_ADC);
		adc_start_conversion(&MY_ADC, MY_ADC_CH);
		//adc_wait_for_interrupt_flag(&MY_ADC, MY_ADC_CH);
		PT_WAIT_UNTIL(pt, adc_get_interrupt_flag(&MY_ADC, MY_ADC_CH) != MY_ADC_CH );
		adc_clear_interrupt_flag(&MY_ADC, MY_ADC_CH);
		
		result = adc_get_result(&MY_ADC, MY_ADC_CH);	
		
		PT_EXIT(pt);
	}
	
	PT_END(pt);
}

#if 0
int main (void)
{
	
	static struct pt pt_setup, pt_wifi, pt_adc;
	
	/* Initialize the cpu clock to config conf_clock.h file */
	sysclk_init();
	
	//cpu_irq_disable();		
	
	//wdt_disable();
	
	/* Initialize the board to config conf_board.h file */
	board_init();
	

	
	
	//cpu_irq_enable();	

	/* Insert application code here, after the board has been initialized. */
	
	PT_INIT(&pt_setup);
	PT_INIT(&pt_wifi);
	PT_INIT(&pt_adc);
	
	while(1)
	{
		//PT_Setup(&pt_setup);
		//PT_WiFiHandle(&pt_wifi);
		//PT_AdcHandle(&pt_adc);
		
		usart_serial_putchar(USART_SERIAL_0, "C");
		delay_s(1);
	}

	return 0;

}

#endif

#define POWER_EN	IOPORT_CREATE_PIN(PORTB,3)
#define KEY_1		IOPORT_CREATE_PIN(PORTC,0)
#define KEY_2		IOPORT_CREATE_PIN(PORTC,4)

int main(void)
{
	uint8_t tx_buf[] = "\n\rHello AVR world ! : ";
	uint8_t tx_length = 22;
	uint8_t received_byte;
	uint8_t i;

	//ioport_init(); 
	//ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTD,3), IOPORT_DIR_OUTPUT);
	//ioport_set_pin_dir(IOPORT_CREATE_PIN(PORTD,2), IOPORT_DIR_INPUT);
	

	/* Initialize the cpu clock to config conf_clock.h file */
	sysclk_init();
	
	/* Initialize the board to config conf_board.h file */
	board_init();
	
	pmic_init();
	//pmic_enable_level(PMIC_LVL_LOW | PMIC_LVL_MEDIUM | PMIC_LVL_HIGH);
	ioport_set_pin_dir(POWER_EN, IOPORT_DIR_OUTPUT);

	ioport_set_pin_level(POWER_EN, true);
	
	ioport_set_pin_dir(KEY_1, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(KEY_1, IOPORT_MODE_WIREDANDPULL);
	ioport_set_pin_sense_mode(KEY_1, IOPORT_SENSE_FALLING);
	
	ioport_set_pin_dir(KEY_2, IOPORT_DIR_INPUT);
	ioport_set_pin_mode(KEY_2, IOPORT_MODE_WIREDANDPULL);
	ioport_set_pin_sense_mode(KEY_2, IOPORT_SENSE_RISING);
	
	PORTC.INT0MASK = 0x01 | (0x01 << 4); //PC0 INT 
	PORTC.INTCTRL = 0x02;  //Medium-level interrupt
	ioport_enable_pin(KEY_1);
	ioport_enable_pin(KEY_2);

	//pmic_set_scheduling(PMIC_SCH_ROUND_ROBIN);
	cpu_irq_enable();
	
	
	while (true) {
		received_byte = usart_getchar(USART_SERIAL_DEBUG);
		if (received_byte == '\r') {
			printf("hello,world, again!\r");
		} else
			usart_putchar(USART_SERIAL_DEBUG, received_byte);
	}
}


static void atomic_operation(void)
{
	irqflags_t flags;
	flags = cpu_irq_save();
	// Uninterruptible block of code
	
	cpu_irq_restore(flags);
}

ISR(PORTC_INT0_vect){
	//Interrupt Service Routine
	//atomic_operation();
	//printf("h\n");
	uint8_t data = PORTC.IN;
	
	if(data & 0x01 == 0x01)
		printf("B\r");
		//usart_putchar(USART_SERIAL_0, data);
}

//ISR_ALIAS(INT1_vect, INT0_vect);