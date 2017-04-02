/*
 * DC Power Meter - Basic Code
 * - Use one INA219, an i2c current monitor from TI
 * with 12 bit ADC, capable to monitor Voltage and
 * Current
 * -------------------------------------------------
 * Medidor de Potência DC - Código Básico
 * - Utiliza um INA219, um monitor de corrente i2c
 * com um ADC de 12 bit, capaz de monitorar Tensão
 * e Corrente
 * 
 * developed by: Haroldo Amaral
 * 2017/03/26 - v 1.0
 */

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>      // lcd i2c lib
#include <Adafruit_INA219.h>        // ina219 lib

// Define some constants used in code
const int LED1 =  2;        // the number of the LED pin
const int LED2 =  3;        // the number of the LED pin
const int ON = 1;           // on state
const int OFF = 0;          // off state
const int LCD_addr = 0x3F;  // LCD i2c address
const int LCD_chars = 16;   // number of characters
const int LCD_lines = 2;    // number of lines
const int INA_addr = 0x40;  // INA219 address


// set the LCD address to 0x3F for a 16 chars and 2 line display
// indica o endereço do LCD  - 0x3F, com 16 caracteres e 2 linhas
LiquidCrystal_I2C lcd(LCD_addr, LCD_chars, LCD_lines);

// set the INA219 to address 0x40
// indica o endereço do INA219 - 0x40
Adafruit_INA219 ina219(INA_addr);

// global variables
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;

// Prototype of functions
void Read_INA219_Values(void);
void LCD_Update(void);
void Serial_Send(void);

void setup()
{
  // configure LED pins
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);

  // configure the serial to 9600bps
  Serial.begin(9600);

  Serial.println("Hello!");
  Serial.println("Measuring voltage and current with INA219 ...");
  
  // initialize the INA219
  ina219.begin();

  // initialize the lcd 
  lcd.init();
  lcd.clear();

  // Print a message to the LCD.
  lcd.backlight();

  // set the cursor and write some text
  lcd.setCursor(0,0);
  lcd.print("Tensao Corrente"); 
}

/*
 * MAIN LOOP
 * - change the led state, measure the values and update LCD
 */
void loop()
{
  // read values from INA219
  // lê os valores do INA219
  Read_INA219_Values();

  // update the LCD
  // atualiza o LCD
  LCD_Update();

  // send data over the serial
  // envia os dados pela serial
  Serial_Send();

  // change the value to increase/decrease the update rate
  // mude o valor para alterar a taxa de atualização
  delay(250);
}

/*
 * Functions
 */

// Read the values from INA219
// lê os valores convertidos pelo INA219
void Read_INA219_Values(void)
{
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
}

// update the LCD with values
// atualiza o LCD com os valores
void LCD_Update(void)
{
  lcd.setCursor(0,1);
  lcd.print(busvoltage);  lcd.print("V   ");

  lcd.setCursor(7,1);
  lcd.print(current_mA); lcd.print("mA   ");
}

// Send data over the serial
// envia os valores pela serial
void Serial_Send(void)
{
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.println("");
}
