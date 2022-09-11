
#include <Wire.h>		// incluye libreria de bus I2C
#include <Adafruit_Sensor.h>	// incluye librerias para sensor BMP280
#include <Adafruit_BMP280.h>
#include <SPI.h>		// incluye libreria interfaz SPI
#include <SD.h>			// incluye libreria para tarjetas SD
#include <Servo.h>

Adafruit_BMP280 bmp;

float TEMPERATURA;		// variable para almacenar valor de temperatura
float HUMEDAD;			// variable para almacenar valor de humedad
float PresionReferencia;
int ALTITUD = 0;
int max = 0;
int counter = 0;

#define SSpin 10		// Slave Select en pin digital 10

File archivo;			// objeto archivo del tipo File
Servo myservo;    // objeto del tipo Servo

void setup() {
  bool var = true;
  bool exec = false;
  pinMode(3, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(4, OUTPUT);
  pinMode(5, INPUT);
  myservo.attach(6);
  digitalWrite(3, LOW);
  digitalWrite(2, LOW);
  digitalWrite(5, LOW);
  Serial.begin(9600);		// inicializa monitor serie a 9600 bps
  bmp.begin();			// inicializacion de sensor

  myservo.write(0);
  delay(200);
  //Altimetro
  Serial.println("Inicializando altímetro...");
  if ( !bmp.begin() ) {				// si falla la comunicacion con el sensor mostrar
    Serial.println("BMP280 no encontrado !");	// texto y detener flujo del programa
    error_message();
    while (1);					// mediante bucle infinito
  }
  PresionReferencia = bmp.readPressure()/100;

  //Tarjeta
  Serial.println("Inicializando tarjeta ...");	// texto en ventana de monitor
  if (!SD.begin(SSpin)) {			// inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    error_message(); 
    return;					// se sale del setup() para finalizar el programa
  }
  Serial.println("inicializacion correcta");	// texto de inicializacion correcta
  archivo = SD.open("datos.txt", FILE_WRITE);	// apertura para lectura/escritura de archivo datos.txt
  connected_message();

  if (archivo) {  
    //archivo.println("COMIENZO DE LA PRUEBA")
    for (int i = 0; i < 3000; i++){			// bucle repite 300 veces
        TEMPERATURA = bmp.readTemperature();	// almacena en variable valor leido de temperatura
        ALTITUD = bmp.readAltitude(PresionReferencia);
        if (max < ALTITUD)
        {
          max = ALTITUD;
        }
        if (max > ALTITUD)
        {
           myservo.write(180);        
        }
        if (ALTITUD > 0 && max > ALTITUD)
        {
          counter++;
        }
        //archivo.print(i);				// escribe en tarjeta el valor del indice
        //archivo.print(",");			// escribe en tarjeta una coma
        archivo.print(ALTITUD);		// escribe en tarjeta el valor de temperatura
        archivo.println(" m");			// escribe en tarjeta una coma       
      
        //Serial.print(i);				// escribe en monitor el valor del indice
        //Serial.print(",");			// escribe en monitor una coma
        Serial.print(ALTITUD);		// escribe en monitor el valor de temperatura
        Serial.println(" m");			// escribe en monitor una coma
        //Serial.println(HUMEDAD);			// escribe en monitor el valor de humedad y salto de linea
        exec = true;
        delay(10);				// espera de 0,1 segundo
      }
      archivo.print(max);
      archivo.println(" max"); 
  
      archivo.print((max  / (counter * 0,01)) * 3,6);
      archivo.println("km/h");
      archivo.println("-----------");
      archivo.close();				// cierre de archivo
      written();
      Serial.println("escritura correcta");	// texto de escritura correcta en monitor serie
  } else {
    Serial.println("error en apertura de datos.txt");	// texto de falla en apertura de archivo
  }
}
//Apagamos todos los leds
void ft_off()
{
  for (int i = 0; i < 14; i++)
  {
    digitalWrite(i, LOW);
  }
}

void connected_message()
{
  ft_off();
  digitalWrite(4, HIGH);
  tone(7, 4000, 200);
  delay(300);
  tone(7, 4000, 200);
  delay(300);
}

void written()
{
  ft_off();
  digitalWrite(3, HIGH);
  tone(7, 4000, 200);
  delay(200);
  tone(7, 3500, 200);
  delay(200);
  tone(7, 4000, 200);
  delay(200);
  tone(7, 5000, 500);
  delay(100);
}

void error_message()
{
  ft_off();
  digitalWrite(8, HIGH);
  tone(7, 1500, 700);
  delay(500);
  tone(7, 1500, 700);
  delay(500);
}

void loop() {			// funcion loop() obligatoria de declarar pero no utilizada
  // nada por aqui
}
