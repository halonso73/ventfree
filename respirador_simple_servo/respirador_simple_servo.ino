#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

#define PIN_SERVO 10 // Servo
#define PIN_FREQ A0 //Potenciometro de frecuencia
#define PIN_PEEP A1 //Potenciometro PEEP
#define PIN_LED 6

int frecuencia = 0;
int valInspira = 0;//Tiempo de inspiración
int valExpira = 0;//Tiempo de expiración
int valPeep = 0; // Grados de peep
int aux; // Variable auxiliar 
Servo myServo;

void inspira()
{
	digitalWrite(PIN_LED,HIGH);
	myServo.write(92);
	
}

void expira()
{
	digitalWrite(PIN_LED,LOW);
	myServo.write(139 - valPeep);
}


void setup() 
{
	pinMode(PIN_LED,OUTPUT);
	Serial.begin(9600);//set the serial communication baudrate as 9600
	myServo.attach(PIN_SERVO);
	digitalWrite(PIN_LED,LOW);
	myServo.write(120);
  lcd.init();
  delay(3000);
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
	
	//lcd.setCursor(0,3);
	//lcd.print("INSPIRACION....");
	Serial.println("INSPIRACION...");
	inspira();
	delay(valInspira * 100);
	
	//lcd.setCursor(0,3);
	//lcd.print("EXPIRACION.....");
	Serial.println("EXPIRACION...");
	expira();
	delay(valExpira * 100);
}


