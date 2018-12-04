#include <SoftwareSerial.h>
#include <Wire.h>
//#include <Adafruit_INA219.h>



int NumPulsos;         //variable para la cantidad de pulsos recibidos
int SensorFlujo = 2;            //Sensor conectado en el pin 2
float factor_conversion = 7.11; //para convertir de frecuencia a caudal
long dt = 0;                    //variación de tiempo por cada bucle
long t0 = 0;                    //millis() del bucle anterior
float FLUJO[2];


bool EstadoValvula = false;
int Valvula = 5;

//Adafruit_INA219 ina219;
//float ENERGIA[5];


SoftwareSerial ESP32(10, 11);
String CmdEsp32 = "";
String RpsEsp32 = "";
String RpsEsp32PW = "";

bool FlagsMsg = false;

void ContarPulsos()
{
    NumPulsos++; //incrementamos la variable de pulsos
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
void interrupt(){
    interrupts();
    delay(1000);          //muestra de 1 segundo
    noInterrupts();   
    return;
}

void setup()
{
    Serial.begin(9600);
    Serial.println("INICIO DE CONFIGURACION INICIAL");
    delay(500);

    ESP32.begin(115200);
    Serial.println("ESP32 CARGADO");
    delay(500);

    pinMode(Valvula, OUTPUT);
    pinMode(Valvula, LOW);
    Serial.println("VALVULA CARGADA");
    delay(500);

    pinMode(SensorFlujo, INPUT);
    attachInterrupt(0, ContarPulsos, RISING); //(Interrupción 0(Pin2),función,Flanco de subida)
    t0 = millis();
    Serial.println("FLUJO CARGADO");

    //uint32_t currentFrequency;
    //ina219.begin();
    //Serial.println("INA219 CARGADO");

  //  Serial.println("PRUEBA DE PROGRAMA");
  //  delay(3000);
  //  Serial.println("PRUEBA SENSOR DE ENERGIA");
  //  GetPower();
  //  delay(1000);





//    Serial.println("FIN PRUEBAS....");
    //delay(5000);

}

void loop()
{
    //GetPower();
    //delay(100);
    //GetSensorFlow();
    //noInterrupts();
    //delay(500);
    if(ESP32.available()){
    for(int i=0 ; i<4;i++){
        if(ESP32.available()){
            Serial.println("nuevo mensajes: ");
            ReciboDeMensaje();
            delay(10);
            }
    }}
    
}

void GetSensorFlow()
{
    delay(500);
    float frecuencia = ObtenerFrecuecia();
    FLUJO[0] = frecuencia / factor_conversion;
    dt = millis() - t0;
    t0 = millis();
    FLUJO[1] = FLUJO[1] + (FLUJO[0] / 60) * (dt / 1000);
    RpsEsp32=String(FLUJO[0])+" L/min " + String(FLUJO[1]) + " L";
    Serial.println(RpsEsp32);
    delay(500);
  
}
/*
void GetPower() 
{
  ENERGIA[0] = ina219.getShuntVoltage_mV();
  ENERGIA[1] = ina219.getBusVoltage_V();
  ENERGIA[2] = ina219.getCurrent_mA();
  ENERGIA[3] = ina219.getPower_mW();
  ENERGIA[4] = ENERGIA[1] + (ENERGIA[0] / 1000);
  
  RpsEsp32PW=String(ENERGIA[4])+ " V " + String(ENERGIA[2])+ " mA " + String(ENERGIA[3]) + " mW" ;
  Serial.println(RpsEsp32PW);
  delay(500);
}
*/

void ReciboDeMensaje()
{
    CmdEsp32 = "";
    while (ESP32.available())
    {
        char x;
        x = ESP32.read();
        CmdEsp32 += x;
        if (x == '\n')
        {
            Serial.print(">>msg: ");
            Serial.println(CmdEsp32);
            FlagsMsg = true;
            break;
        }
    }
    DetectadorDeEventos();
}

void EnviarMensaje(String msg)
{
    if (FlagsMsg)
    {
        Serial.println("SE PROCEDE A ENVIAR RESPUESTA AL ESP32");
        Serial.println(msg);
        msg += '\n';
        for(int i =0; i<msg.length();i++){
            ESP32.write((char)msg[i]);
          }
        FlagsMsg = false;
    }
}

void DetectadorDeEventos()
{
  Serial.println("detectando evento");
    if (CmdEsp32 == "OK\n")
    {
        EnviarMensaje("OK\n");
        return;
    }
    if (CmdEsp32 == "GFL\n")
    {   
        EnviarMensaje(RpsEsp32);
        return;
    }
    if (CmdEsp32 == "GPW\n")
    {   
        EnviarMensaje(RpsEsp32PW);
        return;
    }
    Serial.println("EVENTO NO ENCONTRADO");
    return;
    

 
}
