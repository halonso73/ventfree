#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

#define PIN_SERVO 10 // Servo
#define PIN_FREQ A0 //Potenciometro de frecuencia
#define PIN_PEEP A1 //Potenciometro PEEP
#define PIN_LED 6
#define EXPIRANDO  0
#define INSPIRANDO  1
#define MIN_ANGLE 92 // Ángulo de apertura de inspiración
#define MAX_ANGLE 139 // Ángulo de apertura de expiración

int frecuencia = 0;
int valInspira = 0;//Tiempo de inspiración
int valExpira = 0;//Tiempo de expiración
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
		if ( estado == EXPIRANDO )
		{
			// Pasar a inspirando
			digitalWrite(PIN_LED,HIGH);
			myServo.write(MIN_ANGLE);
			time = millis() + (valInspira * 100);
			estado = INSPIRANDO;
		}
		else if ( estado == INSPIRANDO )
		{
			// Pasar a expirando
			digitalWrite(PIN_LED,LOW);
			myServo.write(MAX_ANGLE - valPeep);
			time = millis() + (valExpira * 100);
			estado = EXPIRANDO;
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
	estado=EXPIRANDO;
  
}

void loop() 
{
	// Leer los valores de los potenciometros en cada pasada
	// Inspira
	aux = analogRead(PIN_FREQ); // Leer el valor del potenciometro
	frecuencia = map(aux,0,1023, 10, 60); // Mapeo del potiencimetro a operaciones por minuto
	// Convertimos esas operaciones por minuto a tiempo de espera de cada operación en décimas de segundo.
	valInspira = ((1.0 / frecuencia )* 600.0 ) * (1.0/3.0);
	valExpira = ((1.0 / frecuencia ) * 600.0 ) * (2.0/3.0);
	
	//expira
	aux = analogRead(PIN_PEEP); // Leer el valor del potenciometro
	valPeep = map(aux,0,1023, 0, 10); // Mapeo del potiencimetro a valores en grados de apertura para el peep.
	
	// escribir valores solo si hay algún cambio
  if ( (frecuencia_ant != frecuencia) || (valPeep_ant != valPeep))
	{
    
		Serial.print("Frecuencia: ");
		Serial.println(frecuencia);
		Serial.print("Inspira: ");
		Serial.println(valInspira);
		Serial.print("Expira: ");
		Serial.println(valExpira);
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


