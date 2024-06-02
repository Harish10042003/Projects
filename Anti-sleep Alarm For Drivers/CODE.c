#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Define pin connections
#define RS 13
#define EN 12
#define D4 11
#define D5 10
#define D6 9
#define D7 8
#define B1 A0
#define B2 6
#define B3 5
#define BZ 4

int a = 0, b = 0;

// Define functions to interact with hardware (placeholders)
void Serial_begin(int baud_rate);
void Serial_print(const char *message);
int Serial_available();
void Serial_readString(char *buffer, int length);
void pinMode(int pin, int mode);
void digitalWrite(int pin, int value);
int digitalRead(int pin);
void delay(int milliseconds);

// LCD functions (placeholders)
void lcd_begin(int columns, int rows);
void lcd_setCursor(int col, int row);
void lcd_print(const char *message);

// EEPROM functions (placeholders)
void EEPROM_write(int address, uint8_t value);
uint8_t EEPROM_read(int address);

void writeString(int address, const char *data);
void readString(int address, char *buffer, int length);

void setup() {
    // Start serial communication
    Serial_begin(9600);
    
    // Set pin modes
    pinMode(B1, INPUT);
    pinMode(B2, OUTPUT);
    pinMode(B3, OUTPUT);
    pinMode(BZ, OUTPUT);
    
    // Initialize the LCD
    lcd_begin(16, 2);
    lcd_setCursor(0, 0);
    lcd_print(" WELCOME ");
    lcd_setCursor(0, 1);
    lcd_print("DROWSINESS D SYS ");
    delay(500);
    
    // Set initial states for the buzzer and LEDs
    digitalWrite(BZ, LOW);
    digitalWrite(B3, LOW);
    digitalWrite(B2, LOW);
}

void loop() {
    char buffer[100];
    
    // Display initial messages on the LCD
    lcd_setCursor(0, 0);
    lcd_print(" WELCOME ");
    lcd_setCursor(0, 1);
    lcd_print("DROWSINESS D SYS ");
    delay(500);
    
    lcd_setCursor(0, 0);
    lcd_print(" WELCOME ");
    lcd_setCursor(0, 1);
    lcd_print("VEHICLE MONITOR ");
    delay(500);
    
    // Check for serial input
    if (Serial_available()) {
        Serial_readString(buffer, 100);
        writeString(11, buffer);
        delay(10);
        
        readString(11, buffer, 100);
        if (buffer[0] == 'A') {
            digitalWrite(B2, HIGH);
            digitalWrite(B3, HIGH);
            lcd_setCursor(0, 0);
            lcd_print("*****");
            digitalWrite(BZ, HIGH);
            lcd_setCursor(0, 1);
            lcd_print("Drowsiness Detect ");
            delay(5000);
            digitalWrite(BZ, LOW);
        }
    }
    
    // Read the state of pin 7
    int dd = digitalRead(7);
    if (dd == 0) {
        digitalWrite(B2, LOW);
        digitalWrite(B3, HIGH);
        lcd_setCursor(0, 0);
        lcd_print("*****");
        digitalWrite(BZ, HIGH);
        lcd_setCursor(0, 1);
        lcd_print("GAS Detect ");
        delay(5000);
        digitalWrite(BZ, LOW);
    }
}

void writeString(int address, const char *data) {
    int _size = strlen(data);
    int i;
    for (i = 0; i < _size + 1; i++) {
        EEPROM_write(address + i, data[i]);
    }
    EEPROM_write(address + _size, '\0'); // Add termination null character for String Data
}

void readString(int address, char *buffer, int length) {
    int len = 0;
    uint8_t k;
    k = EEPROM_read(address);
    while (k != '\0' && len <= length) { // Read until null character
        k = EEPROM_read(address + len);
        buffer[len] = k;
        len++;
    }
    buffer[len] = '\0';
}