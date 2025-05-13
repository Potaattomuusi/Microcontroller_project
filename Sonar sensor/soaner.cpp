/*#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#define HISTORY_SIZE 2 
uint32_t range_history[HISTORY_SIZE] = {0};
#define SCL_CLOCK 100000L
#define LCD_ADDR 0x3E
#define ANGLE_MIN 10
#define ANGLE_MAX 170
#define ANGLE_STEP 1
#define NUM_ANGLES ((ANGLE_MAX - ANGLE_MIN) / ANGLE_STEP + 1)

uint32_t last_pass_ranges[NUM_ANGLES] = {0};


// --- LCD I2C Functions ---
void TWI_init(void) {
	TWSR0 = 0x00;
	TWBR0 = ((F_CPU / SCL_CLOCK) - 16) / 2;
}

void TWI_start(void) {
	TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR0 & (1 << TWINT)));
}

void TWI_stop(void) {
	TWCR0 = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
	_delay_ms(1);
}

void TWI_write(uint8_t data) {
	TWDR0 = data;
	TWCR0 = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR0 & (1 << TWINT)));
}

void LCD_sendCommand(uint8_t cmd) {
	TWI_start();
	TWI_write(LCD_ADDR << 1);
	TWI_write(0x80);
	TWI_write(cmd);
	TWI_stop();
}

void LCD_sendData(uint8_t data) {
	TWI_start();
	TWI_write(LCD_ADDR << 1);
	TWI_write(0x40);
	TWI_write(data);
	TWI_stop();
}

void LCD_init(void) {
	_delay_ms(50);
	LCD_sendCommand(0x38);
	LCD_sendCommand(0x39);
	LCD_sendCommand(0x14);
	LCD_sendCommand(0x70);
	LCD_sendCommand(0x56);
	LCD_sendCommand(0x6C);
	_delay_ms(200);
	LCD_sendCommand(0x38);
	LCD_sendCommand(0x0C);
	LCD_sendCommand(0x01);
	_delay_ms(2);
}

void LCD_setCursor(uint8_t col, uint8_t row) {
	uint8_t address = (row == 0) ? col : (0x40 + col);
	LCD_sendCommand(0x80 | address);
}

void LCD_print(const char *str) {
	while (*str) {
		LCD_sendData(*str++);
	}
}


void timer1_init() {
	TCCR1B |= (1 << WGM12);    // CTC mode
	OCR1A = 15;                // 1 microsecond tick (16MHz / 8 / 2 = 1us)
	TCCR1B |= (1 << CS11);     // Prescaler 8
	TIMSK1 |= (1 << OCIE1A);   // Enable compare interrupt
	sei();                     // Enable global interrupts
}
// ------------------- TIMER1 FOR micros() -------------------
volatile uint32_t microseconds = 0;

void timer3_init() {
	TCCR3B |= (1 << WGM32);    // CTC mode
	OCR3A = 15;                // 1 microsecond tick (16MHz / 8 / 2 = 1us)
	TCCR3B |= (1 << CS31);     // Prescaler 8
	TIMSK3 |= (1 << OCIE3A);   // Enable compare interrupt
	sei();                     // Enable global interrupts
}

ISR(TIMER3_COMPA_vect) {
	microseconds++;
}

uint32_t micros() {
	uint32_t us;
	cli();
	us = microseconds;
	sei();
	return us;
}




// ------------------- GPIO Helpers -------------------
#define TRIG_ECHO_PIN PB0

void pinModeOutput() {
	DDRB |= (1 << TRIG_ECHO_PIN);
}

void pinModeInput() {
	DDRB &= ~(1 << TRIG_ECHO_PIN);
}

void digitalWriteHigh() {
	PORTB |= (1 << TRIG_ECHO_PIN);
}

void digitalWriteLow() {
	PORTB &= ~(1 << TRIG_ECHO_PIN);
}

uint8_t digitalReadPin() {
	return (PINB & (1 << TRIG_ECHO_PIN)) != 0;
}

// ------------------- Pulse and Measurement -------------------
uint32_t MicrosDiff(uint32_t begin, uint32_t end) {
	return end - begin;
}

uint32_t mypulseIn(uint32_t timeout) {
	uint32_t begin = micros();

	// Wait for previous pulse to end
	while (digitalReadPin()) {
		if (MicrosDiff(begin, micros()) >= timeout) return 0;
	}

	// Wait for pulse to start
	while (!digitalReadPin()) {
		if (MicrosDiff(begin, micros()) >= timeout) return 0;
	}
	uint32_t pulseBegin = micros();

	// Wait for pulse to end
	while (digitalReadPin()) {
		if (MicrosDiff(begin, micros()) >= timeout) return 0;
	}
	uint32_t pulseEnd = micros();

	return MicrosDiff(pulseBegin, pulseEnd);
}

uint32_t duration(uint32_t timeout) {
	pinModeOutput();
	digitalWriteLow();
	_delay_us(2);
	digitalWriteHigh();
	_delay_us(5);
	digitalWriteLow();
	pinModeInput();
	return mypulseIn(timeout);
}

uint32_t MeasureInCentimeters(uint32_t timeout) {
	uint32_t dur = duration(timeout);
	return dur*0.29/2 ; // speed of sound approximation (us to cm)
}

// --- ADC for Potentiometer ---
void ADC_init() {
	ADMUX = (1 << REFS0); // AVcc reference, ADC0
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1); // Enable ADC, prescaler 64
}

uint16_t ADC_read() {
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

// --- Servo PWM on PB1 (OC1A) ---
void servo_init() {
	DDRB |= (1 << PB1); // Set PB1 as output
	TCCR1A = (1 << COM1A1) | (1 << WGM11); // Fast PWM, non-inverting
	TCCR1B = (1 << WGM13) | (1 << WGM32) | (1 << CS31); // Prescaler 8
	ICR1 = 39999; // 20ms period (50Hz)
}

void servo_set_angle(uint8_t angle) {
	uint16_t pulse = 500 + ((uint32_t)angle * 1900) / 180; // 500–2400 µs
	OCR1A = pulse * 2; // Timer runs at 0.5 µs per tick
}

// --- Main Program ---
int main(void) {
	uint8_t angle = 10;
	int8_t direction = 1;

	timer3_init();       // Timer3 for micros()
	TWI_init();          // I2C
	LCD_init();          // LCD
	ADC_init();          // ADC for potentiometer
	servo_init();        // Timer1 for servo PWM
	int laskuri;
	int muisti1;
	char buffer[16];
	
	

	while (1) {
		
		uint16_t adc_val = ADC_read();
		uint16_t delay_us = 1000 + (adc_val * 19000UL) / 1023;

		servo_set_angle(angle);
		laskuri++;
		for (uint16_t i = 0; i < delay_us / 100; i++) {
			_delay_us(100);
		}

		angle += direction;
		if (angle >= 170 || angle <= 10) {
			direction = -direction;
		}
		
		
			if (laskuri == 40)
			{
				uint8_t index = (angle - ANGLE_MIN) / ANGLE_STEP;

				uint32_t range_cm = MeasureInCentimeters(1000000);  // 1s timeout
				int32_t diff = (int32_t)range_cm - (int32_t)last_pass_ranges[index];
				last_pass_ranges[index] = range_cm;  // Update for next sweep

				snprintf(buffer, sizeof(buffer), "R:%3ucm D:%+3ld", (unsigned int)range_cm, diff);
				LCD_setCursor(0, 0);
				LCD_print("                ");  // Clear
				LCD_setCursor(0, 0);
				LCD_print(buffer);
				if (diff >= 10 || diff <= -10) {
					LCD_setCursor(0, 1);
					LCD_print("TUHOA");
					} else {
					LCD_setCursor(0, 1);
					LCD_print("NULL ");
				}

				laskuri = 0;
			

		}
	}
}*/

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define SCL_CLOCK 100000L
#define LCD_ADDR 0x3E
#define ANGLE_MIN 10
#define ANGLE_MAX 170
#define ANGLE_STEP 1
#define NUM_ANGLES ((ANGLE_MAX - ANGLE_MIN) / ANGLE_STEP + 1)
#define BUZZER_PIN PD7

uint32_t last_pass_ranges[NUM_ANGLES] = {0};
volatile bool scanning = false;
uint32_t microseconds = 0;

// --- LCD I2C Definitions and Functions (as in your existing code) ---
void TWI_init(void) {
	TWSR0 = 0x00;
	TWBR0 = ((F_CPU / SCL_CLOCK) - 16) / 2;
}

void TWI_start(void) {
	TWCR0 = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);
	while (!(TWCR0 & (1 << TWINT)));
}

void TWI_stop(void) {
	TWCR0 = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT);
	_delay_ms(1);
}

void TWI_write(uint8_t data) {
	TWDR0 = data;
	TWCR0 = (1 << TWEN) | (1 << TWINT);
	while (!(TWCR0 & (1 << TWINT)));
}

void LCD_sendCommand(uint8_t cmd) {
	TWI_start();
	TWI_write(LCD_ADDR << 1);
	TWI_write(0x80);
	TWI_write(cmd);
	TWI_stop();
}

void LCD_sendData(uint8_t data) {
	TWI_start();
	TWI_write(LCD_ADDR << 1);
	TWI_write(0x40);
	TWI_write(data);
	TWI_stop();
}

void LCD_init(void) {
	_delay_ms(50);
	LCD_sendCommand(0x38);
	LCD_sendCommand(0x39);
	LCD_sendCommand(0x14);
	LCD_sendCommand(0x70);
	LCD_sendCommand(0x56);
	LCD_sendCommand(0x6C);
	_delay_ms(200);
	LCD_sendCommand(0x38);
	LCD_sendCommand(0x0C);
	LCD_sendCommand(0x01);
	_delay_ms(2);
}

void LCD_setCursor(uint8_t col, uint8_t row) {
	uint8_t address = (row == 0) ? col : (0x40 + col);
	LCD_sendCommand(0x80 | address);
}

void LCD_print(const char *str) {
	while (*str) {
		LCD_sendData(*str++);
	}
}

// --- Timer3 for micros ---
ISR(TIMER3_COMPA_vect) {
	microseconds++;
}
void timer3_init() {
	TCCR3B |= (1 << WGM32);
	OCR3A = 15;
	TCCR3B |= (1 << CS31);
	TIMSK3 |= (1 << OCIE3A);
}

// --- micros() ---
uint32_t micros() {
	uint32_t us;
	cli();
	us = microseconds;
	sei();
	return us;
}

// --- GPIO + Ultrasonic Measurement (as in your existing code) ---
#define TRIG_ECHO_PIN PB0

void pinModeOutput() {
	DDRB |= (1 << TRIG_ECHO_PIN);
}

void pinModeInput() {
	DDRB &= ~(1 << TRIG_ECHO_PIN);
}

void digitalWriteHigh() {
	PORTB |= (1 << TRIG_ECHO_PIN);
}

void digitalWriteLow() {
	PORTB &= ~(1 << TRIG_ECHO_PIN);
}

uint8_t digitalReadPin() {
	return (PINB & (1 << TRIG_ECHO_PIN)) != 0;
}

// ------------------- Pulse and Measurement -------------------
uint32_t MicrosDiff(uint32_t begin, uint32_t end) {
	return end - begin;
}

uint32_t mypulseIn(uint32_t timeout) {
	uint32_t begin = micros();

	// Wait for previous pulse to end
	while (digitalReadPin()) {
		if (MicrosDiff(begin, micros()) >= timeout) return 0;
	}

	// Wait for pulse to start
	while (!digitalReadPin()) {
		if (MicrosDiff(begin, micros()) >= timeout) return 0;
	}
	uint32_t pulseBegin = micros();

	// Wait for pulse to end
	while (digitalReadPin()) {
		if (MicrosDiff(begin, micros()) >= timeout) return 0;
	}
	uint32_t pulseEnd = micros();

	return MicrosDiff(pulseBegin, pulseEnd);
}

uint32_t duration(uint32_t timeout) {
	pinModeOutput();
	digitalWriteLow();
	_delay_us(2);
	digitalWriteHigh();
	_delay_us(5);
	digitalWriteLow();
	pinModeInput();
	return mypulseIn(timeout);
}

uint32_t MeasureInCentimeters(uint32_t timeout) {
	uint32_t dur = duration(timeout);
	return dur*0.29/2 ; // speed of sound approximation (us to cm)
}


// --- ADC for PC0 (speed) and PC1 (duration) ---
void ADC_init() {
	ADMUX = (1 << REFS0); // AVcc
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);
}
uint16_t ADC_read(uint8_t channel) {
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

// --- Servo PWM on PB1 ---
void servo_init() {
	DDRB |= (1 << PB1);
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
	ICR1 = 39999;
}
void servo_set_angle(uint8_t angle) {
	uint16_t pulse = 500 + ((uint32_t)angle * 1900) / 180;
	OCR1A = pulse * 2;
}

// --- Button interrupt for Start/Stop ---
void button_init() {
	DDRD &= ~(1 << PD2);    // INT0 pin as input
	PORTD |= (1 << PD2);    // Pull-up
	EIMSK |= (1 << INT0);   // Enable INT0
	EICRA |= (1 << ISC01);  // Falling edge triggers
	sei();
}
ISR(INT0_vect) {
	scanning = !scanning; // Toggle scanning state
}

// --- Buzzer ---
void buzzer_init() {
	// Set the buzzer pin as output
	DDRD |= (1 << BUZZER_PIN);

	// Configure Timer2 for Fast PWM mode to generate a frequency signal for the buzzer
	TCCR2A = (1 << COM2A0) | (1 << WGM21) | (1 << WGM20);  // Toggle OC2A on Compare Match, Fast PWM mode
	TCCR2B = (1 << CS21);  // Prescaler = 8

	// Set OCR2A to generate the desired frequency
	OCR2A = (F_CPU / (2 * 8 *8000)) - 1; // (16 MHz / 2 / 8 / 1000 Hz) - 1 = 124
}
void buzzer_on() {
	PORTD |= (1 << BUZZER_PIN);
}
void buzzer_off() {
	PORTD &= ~(1 << BUZZER_PIN);
}

int main(void) {
	uint8_t angle = ANGLE_MIN;
	int8_t direction = 1;
	char buffer[16];
	uint32_t scan_start_time = 0;

	timer3_init();
	TWI_init();
	LCD_init();
	ADC_init();
	servo_init();
	button_init();
	buzzer_init();

	while (1) {
		uint16_t speed_adc = ADC_read(0);      // PC0 controls speed
		//uint16_t duration_adc = ADC_read(1);   // PC1 controls scan duration
		//uint32_t scan_duration_ms = 1000 + ((uint32_t)duration_adc * 9000UL) / 1023; // 1–10 s
		uint16_t delay_us = 500 + ((uint32_t)speed_adc * 4500UL) / 1023;            // 0.5–5 ms per step

		if (scanning) {
			if (scan_start_time == 0) {
				scan_start_time = micros() / 1000; // convert to ms
			}

			servo_set_angle(angle);
			for (uint16_t i = 0; i < delay_us / 100; i++) _delay_us(100);

			uint8_t index = (angle - ANGLE_MIN) / ANGLE_STEP;
			uint32_t range_cm = MeasureInCentimeters(60000);  // 60 ms timeout
			int32_t diff = (int32_t)range_cm - (int32_t)last_pass_ranges[index];
			last_pass_ranges[index] = range_cm;

			snprintf(buffer, sizeof(buffer), "R:%3ucm D:%+3ld", (unsigned int)range_cm, diff);
			LCD_setCursor(0, 0);
			LCD_print("                ");  // Clear line
			LCD_setCursor(0, 0);
			LCD_print(buffer);

			if (diff >= 10 || diff <= -10) {
				LCD_setCursor(0, 1);
				LCD_print("!! ALERT !!     ");
				buzzer_on();
				} else {
				LCD_setCursor(0, 1);
				LCD_print("All Clear       ");
				buzzer_off();
			}

			// Update angle
			angle += direction;
			if (angle >= ANGLE_MAX || angle <= ANGLE_MIN) {
				direction = -direction;
			}

			// Check if scanning duration elapsed
		/*	if ((micros() / 1000) - scan_start_time >= scan_duration_ms) {
				scanning = false;
				buzzer_off();
				LCD_setCursor(0, 0);
				LCD_print("Scan Complete   ");
				LCD_setCursor(0, 1);
				LCD_print("Press to Start  ");
				scan_start_time = 0;
			} */

			} else {
			// Idle state LCD message
			LCD_setCursor(0, 0);
			LCD_print("System Idle     ");
			LCD_setCursor(0, 1);
			LCD_print("Press Button    ");
			buzzer_off();
			_delay_ms(500); // avoid LCD flicker
		}
	}
}

