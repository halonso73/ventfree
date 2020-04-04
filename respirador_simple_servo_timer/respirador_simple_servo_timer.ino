/* Programa para control de válvula del respirador VentFree. 
 * Dispone de dos potenciómetros en las entradas A0 y A1 para controlar la frecuencia de las maniobras de 10 a 60 veces por minuto,
 * Dicha frecuencia se traducirá en el tiempo en el que el servo debe permanecer en la posición de 'espirando' o 'inspirando' en una relación 2 a 1 
 * Es decir, 2 partes del tiempo estará en 'espirando' y 1 en inspirando.
 * Además, se añade el potenciómero del peep, que indicará cuantos grados de apertura debe tener en la posición de 'espirando' para que siempre tenga una
 * pequeña entrada de aire.
 *
 * @Author: Hugo Alonso Sobrino
 * @e-mail: halonso73@gmail.com
 * @Version: 1.0
 * 
*/


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

#define PIN_SERVO 10 // Servo
#define PIN_FREQ A0 //Potenciometro de frecuencia
#define PIN_PEEP A1 //Potenciometro PEEP
#define PIN_LED 6
#define ESPIRANDO  0
#define INSPIRANDO  1
#define MIN_ANGLE 92 // Ángulo de apertura de inspiración
#define MAX_ANGLE 139 // Ángulo de apertura de espiración

int frecuencia = 0;
int valInspira = 0;//Tiempo de inspiración
int valEspira = 0;//Tiempo de espiración
int valPeep = 0; // Grados de peep
int aux; // Variable auxiliar 
unsigned long time;
Servo myServo;
int frecuencia_ant;
int valPeep_ant;

int estado;

void setServo()
{
	if ( millis() > time )
	{
		if ( estado == ESPIRANDO )
		{
			// Pasar a inspirando
			digitalWrite(PIN_LED,HIGH);
			myServo.write(MIN_ANGLE);
			time = millis() + (valInspira * 100);
			estado = INSPIRANDO;
		}
		else if ( estado == INSPIRANDO )
		{
			// Pasar a espirando
			digitalWrite(PIN_LED,LOW);
			myServo.write(MAX_ANGLE - valPeep);
			time = millis() + (valEspira * 100);
			estado = ESPIRANDO;
		}
	}
}

void setup() 
{
  myServo.attach(PIN_SERVO);
  myServo.write(120);
  digitalWrite(PIN_LED,LOW);
	pinMode(PIN_LED,OUTPUT);
	Serial.begin(9600);//set the serial communication baudrate as 9600
	time = millis();
  frecuencia_ant=-1;
  valPeep_ant=-1;
	lcd.init();
  lcd.backlight();
	delay(3000);
	estado=ESPIRANDO;
  
}

void loop() 
{
	// Leer los valores de los potenciometros en cada pasada
	// Inspira
	aux = analogRead(PIN_FREQ); // Leer el valor del potenciometro
	frecuencia = map(aux,0,1023, 10, 60); // Mapeo del potiencimetro a operaciones por minuto
	// Convertimos esas operaciones por minuto a tiempo de espera de cada operación en décimas de segundo.
	valInspira = ((1.0 / frecuencia )* 600.0 ) * (1.0/3.0);
	valEspira = ((1.0 / frecuencia ) * 600.0 ) * (2.0/3.0);
	
	//espira
	aux = analogRead(PIN_PEEP); // Leer el valor del potenciometro
	valPeep = map(aux,0,1023, 0, 10); // Mapeo del potiencimetro a valores en grados de apertura para el peep.
	
	// escribir valores solo si hay algún cambio
  if ( (frecuencia_ant != frecuencia) || (valPeep_ant != valPeep))
	{
    
		Serial.print("Frecuencia: ");
		Serial.println(frecuencia);
		Serial.print("Inspira: ");
		Serial.println(valInspira);
		Serial.print("Espira: ");
		Serial.println(valEspira);
		Serial.print("Peep: ");
		Serial.println(valPeep);
		// Se escribe el lcd
		lcd.setCursor(0, 0);
		lcd.print("FRECUENCIA: ");
		lcd.setCursor(13, 0);
		lcd.print(frecuencia);
		lcd.setCursor(0, 1);
		lcd.print("PEEP:  ");
		lcd.setCursor(13, 1);
		lcd.print(valPeep);
    frecuencia_ant = frecuencia;
    valPeep_ant = valPeep;
	}
	
	setServo();
	
	delay(10);
}


