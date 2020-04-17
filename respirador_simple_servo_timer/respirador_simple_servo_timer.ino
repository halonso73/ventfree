/* Programa para control de válvula del respirador VentFree. 
 * Dispone de dos potenciómetros en las entradas A0 y A1 para controlar la frecuencia de las maniobras de 10 a 60 veces por minuto,
 * Dicha frecuencia se traducirá en el tiempo en el que el servo debe permanecer en la posición de 'espirando' o 'inspirando' en una relación 2 a 1 
 * Es decir, 2 partes del tiempo estará en 'espirando' y 1 en inspirando.
 * Además, se añade el potenciómero del peep, que indicará cuantos grados de apertura debe tener en la posición de 'espirando' para que siempre tenga una
 * pequeña entrada de aire.
 *
 * Se añade un proceso de configuración para ajustar el recorrido del servo (ver fichero README.txt)
 * para saber si se ha ajustado o no un servo, se leerá la posición 3 de la EEPROM, cuyo valor debe ser 128.
 * Si esto se cumple, es que el servo ya fue ajustado con anterioridad.
 * 
 * @Author: Hugo Alonso Sobrino
 * @e-mail: halonso73@gmail.com
 * @Version: 1.0
 * 
*/


#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <EEPROM.h>

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 20 chars and 4 line display

// PInes
#define PIN_SERVO 10 // Servo
#define PIN_FREQ A0 //Potenciometro de frecuencia
#define PIN_PEEP A1 //Potenciometro PEEP
#define PIN_CONFIG 3 // Botón/interruptor de configuración
#define PIN_LED 6

//estados
#define ESPIRANDO  0
#define INSPIRANDO  1
#define MENSAJE_ESPIRACION 2
#define MENSAJE_INSPIRACION 3
#define FIN 4

//angulos por defecto del servo
#define DEFAULT_MIN_ANGLE 92 // Ángulo de apertura de inspiración
#define DEFAULT_MAX_ANGLE 139 // Ángulo de apertura de espiración

int frecuencia = 0;
int valInspira = 0;//Tiempo de inspiración
int valEspira = 0;//Tiempo de espiración
int valPeep = 0; // Grados de peep
int aux; // Variable auxiliar 
unsigned long time;
Servo myServo;
int frecuencia_ant;
int valPeep_ant;
char aux_f[3];
char aux_p[3];
int min_angle = DEFAULT_MIN_ANGLE;
int max_angle = DEFAULT_MAX_ANGLE;


int estado;
int config;

void setServo()
{
	if ( millis() > time )
	{
		if ( estado == ESPIRANDO )
		{
			// Pasar a inspirando
			digitalWrite(PIN_LED,HIGH);
			myServo.write(min_angle);
			time = millis() + (valInspira * 100);
			estado = INSPIRANDO;
		}
		else if ( estado == INSPIRANDO )
		{
			// Pasar a espirando
			digitalWrite(PIN_LED,LOW);
			myServo.write(max_angle - valPeep);
			time = millis() + (valEspira * 100);
			estado = ESPIRANDO;
		}
	}
}

void setup() 
{
  int configurado;
  pinMode(PIN_CONFIG, INPUT);
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
  config = digitalRead(PIN_CONFIG);
  Serial.print("Estado de configuración???");
  Serial.println(config);
  Serial.print("Valor de max_angle: ");
  Serial.println(EEPROM.read(0));
  Serial.print("Valor de min_angle: ");
  Serial.println(EEPROM.read(1));
  configurado = EEPROM.read(2);
  Serial.print("Valor de configurado:");
  Serial.println(EEPROM.read(2));
  if ( configurado == 128 )
  {
    max_angle = EEPROM.read(0);
    min_angle = EEPROM.read(1);
  }
  
  if ( config )
    estado = MENSAJE_ESPIRACION;
  delay(5000);
  attachInterrupt(digitalPinToInterrupt(PIN_CONFIG),buttom, FALLING);
}


void main_loop() 
{
	// Leer los valores de los potenciometros en cada pasada
	// Inspira
	aux = analogRead(PIN_FREQ); // Leer el valor del potenciometro
	frecuencia = map(aux,0,1023, 5, 60); // Mapeo del potiencimetro a operaciones por minuto
	// Convertimos esas operaciones por minuto a tiempo de espera de cada operación en décimas de segundo.
	valInspira = ((1.0 / frecuencia )* 600.0 ) * (1.0/3.0);
	valEspira = ((1.0 / frecuencia ) * 600.0 ) * (2.0/3.0);
	
	//espira
	aux = analogRead(PIN_PEEP); // Leer el valor del potenciometro
	valPeep = map(aux,0,1023, 0, 10); // Mapeo del potiencimetro a valores en grados de apertura para el peep.
	
	// escribir valores solo si hay algún cambio
  if ( (frecuencia_ant != frecuencia) || (valPeep_ant != valPeep))
	{
  
    sprintf(aux_f,"%2d",frecuencia);
    sprintf(aux_p,"%2d",valPeep);
    Serial.print("Frecuencia: ");
		Serial.println(aux_f);
		Serial.print("Inspira: ");
		Serial.println(valInspira);
		Serial.print("Espira: ");
		Serial.println(valEspira);
		Serial.print("Peep: ");
		Serial.println(aux_p);
		// Se escribe el lcd
		lcd.setCursor(0, 0);
		lcd.print("FRECUENCIA: ");
		lcd.setCursor(13, 0);
		lcd.print(aux_f);
		lcd.setCursor(0, 1);
		lcd.print("PEEP:  ");
		lcd.setCursor(13, 1);
		lcd.print(aux_p);
    frecuencia_ant = frecuencia;
    valPeep_ant = valPeep;
	}
	
	setServo();
	
	delay(10);
}

void config_loop() 
{
  // Leer los valores de los potenciometros en cada pasada
  // Inspira
  if ( estado == MENSAJE_ESPIRACION )
  {
    Serial.println("Coloque el servo, regulación de la posición ESPIRANDO");
    Serial.println("Cuando tenga el ángulo, pulse el botón");
    estado = ESPIRANDO;
  }
  else if ( estado == MENSAJE_INSPIRACION)
  {
    Serial.println("Coloque el servo, regulación de la posición INSPIRANDO");
    Serial.println("Cuando tenga el ángulo, pulse el botón");
    estado = INSPIRANDO;
  }
  if ( estado != FIN )
  {
    aux = analogRead(PIN_FREQ); // Leer el valor del potenciometro
    frecuencia = map(aux,0,1023, 0, 180); // Mapeo del potiencimetro a grados
    if ( frecuencia != frecuencia_ant)
    {
      Serial.print("angulo: ");
      Serial.println(frecuencia);
      Serial.print("Estado: ");
      Serial.println(estado);
      myServo.write(frecuencia);
      frecuencia_ant = frecuencia;
    }
  }
  delay(100);
}

void buttom()
{
  // Rutina para ejecutar acciones cada vez que se pulse el botón
  if ( config )
  {
    // Sólo se harán cosas en el modo de configuración
    Serial.println("Botón pulsado");
    if ( estado == ESPIRANDO )
    {
      estado = MENSAJE_INSPIRACION;
      Serial.print("Grabando el la EEPROM el valor espirando, max_angle. Valor: ");
      Serial.println(frecuencia);
      EEPROM.write(0,frecuencia);
      
    }
    else if ( estado == INSPIRANDO )
    {
      estado = FIN;
      Serial.print("Grabando el la EEPROM el valor inspirando, min_angle. Valor: ");
      Serial.println(frecuencia);
      EEPROM.write(1,frecuencia);
      EEPROM.write(2,128);
      estado == FIN;
    }
  }
}

void loop()
{
  if ( !config )
    main_loop();
  else
    config_loop();
}

