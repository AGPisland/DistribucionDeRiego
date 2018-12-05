#include <SoftwareSerial.h>
volatile long NumPulsos;
int SensorFlujo=2;
long dt = 0;                    //variación de tiempo por cada bucle
long t0 = 0;
float factor=6.827;   
float volumen=0;
SoftwareSerial ESP32(10, 11);
String RpsEsp32HE="";
void interrupt(){
    interrupts();
    delay(100);          //muestra de 1 segundo
    noInterrupts();   
    return;
}
void ContarPulsos(){
    NumPulsos++;
  }
int ObtenerFrecuecia()
{
    int frecuencia;
    NumPulsos = 0;          //Ponemos a 0 el número de pulsos
    interrupt();
    delay(1000);
    noInterrupts();   
    frecuencia = NumPulsos; //Hz(pulsos por segundo)
    return frecuencia;
}
void GetHall()
{
    float frecuencia = ObtenerFrecuecia();
    dt = millis() - t0;
    t0 = millis();
    RpsEsp32HE=String(NumPulsos);
    Serial.println(NumPulsos);
    Serial.println(RpsEsp32HE);
  
}
void setup(){
  Serial.begin(9600);
  interrupts();ESP32.begin(115200);
 Serial.println("ESP32 CARGADO");
    pinMode(SensorFlujo, INPUT);
   attachInterrupt(0, ContarPulsos, RISING); //(Interrupción 0(Pin2),función,Flanco de subida)
   t0 = millis();
   Serial.println("FLUJO CARGADO");
}
void EnviarMensaje(String msg)
{
    if (true)
    {
        Serial.println("SE PROCEDE A ENVIAR RESPUESTA AL ESP32");
        Serial.println(msg);
        msg += '\n';
        for(int i =0; i<msg.length();i++){
            ESP32.write((char)msg[i]);
          }
    }
}
int Voltage=1;
int Voltagee=2;
void loop(){
  Serial.print("Numero de Pulsos= ");
  Serial.println(NumPulsos);
  volumen=0;
  float frecuencia=ObtenerFrecuecia();
  float caudal_L_m=frecuencia/factor;
  dt=millis()-t0;
  t0=millis();
  volumen=volumen+(caudal_L_m/60)*(dt/1000);
  String Caudal_Vol="";
  String Vol=String (volumen);
  String Caudal=String (caudal_L_m);
  Caudal_Vol=Vol+"/" +Caudal+"/"+String(Voltage)+"/"+String(Voltagee);
  delay (1000);
  EnviarMensaje(Caudal_Vol);
}
