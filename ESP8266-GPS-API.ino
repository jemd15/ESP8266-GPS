#if defined(ESP8266)
#include <ESP8266WiFi.h> //ESP8266WiFi.h .- ESP8266 Core WiFi Library
#else
#include <WiFi.h> //WiFi.h .- ESP32 Core WiFi Library
#endif

#if defined(ESP8266)

#include <ESP8266WebServer.h> //ESP8266WebServer.h .- Servidor web local utilizado para servir el portal de configuración

#else

#include <WebServer.h> //WebServer.h .- Servidor DNS local utilizado para redireccionar todas las solicitudes al portal de configuración (https://github.com/zhouhan0126/DNSServer---esp32)
#endif

#include <DNSServer.h>   //DNSServer.h .- Local WebServer usado para servir el portal de configuración (https://github.com/zhouhan0126/DNSServer---esp32)
#include <WiFiManager.h> //WiFiManager.h .- WiFi Configuration Magic (https://github.com/zhouhan0126/DNSServer---esp32) >> https://github.com/zhouhan0126/DNSServer---esp32 (ORIGINAL)
#include <SoftwareSerial.h>
#include <TinyGPS.h> //librería para mostrar los datos del GPS más ordenados

TinyGPS gps;
SoftwareSerial ss(4, 3);

char dato = ' ';
const int PIN_AP = 2; // pulsador para volver al modo AP

void configModeCallback(WiFiManager *myWiFiManager)
{
  Serial.println("Modo de configuración ingresado");
  Serial.println(WiFi.softAPIP());

  Serial.println(myWiFiManager->getConfigPortalSSID());
}

//flag for saving data
bool shouldSaveConfig = false;

// En https://github.com/tzapu/WiFiManager
//callback notifying us of the need to save config

//declaración de objeto wifiManager
WiFiManager wifiManager;

void saveConfigCallback()
{
  Serial.println("Debería guardar la configuración");
  shouldSaveConfig = true;
}

void setup()
{
  Serial.begin(115200);
  ss.begin(9600); //comunicación serial con el módulo GPS
  pinMode(PIN_AP, INPUT);

  //modificamos el portal
  wifiManager.setCustomHeadElement("<style>html { color: #c5c6c7; background-color: #1f2833; } a:visited, a { color: #66fcf1; transition: 200ms; } a:hover, button:hover { color: #ff256c; } button { background-color: transparent; transition: 200ms; border: 5px solid #66fcf1; color: #c5c6c7; height: 15vh; }</style>");

  // utilizando ese comando, eliminamos los datos guardados en memoria
  wifiManager.resetSettings();

  //devolución de llamada para cuando entra en el modo de configuración AP
  wifiManager.setAPCallback(configModeCallback);
  //devolución de llamada cuando se conecta a una red, es decir, cuando pasa a trabajar en modo EST
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //crea una red de nombre ESP_AP con pass 12345678
  wifiManager.autoConnect("ESP_AP", "12345678");
}

void loop()
{

  /* WiFiManager wifiManager;
  Serial.println("poner el código del GPS aquí  1");

  //si el botón se ha presionado
  if (digitalRead(PIN_AP) == HIGH)
  {
    Serial.println("reajustar"); //resetear intenta abrir el portal
    if (!wifiManager.startConfigPortal("ESP_AP", "12345678"))
    {
      Serial.println("No se pudo conectar");
      delay(2000);
      ESP.restart();
      delay(1000);
      Serial.println("poner el código del GPS aquí  2");
    }
    Serial.println("conectado ESP_AP!!!");
    Serial.println("poner el código del GPS aquí  3");
  }
  Serial.println("poner el código del GPS aquí  4"); */
}

/*   // código para leer el GPS
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      // Serial.write(c); // uncomment this line if you want to see the GPS data flowing
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    Serial.print("LAT=");
    Serial.print(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    Serial.print(" LON=");
    Serial.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    Serial.print(" SAT=");
    Serial.print(gps.satellites() == TinyGPS::GPS_INVALID_SATELLITES ? 0 : gps.satellites());
    Serial.print(" PREC=");
    Serial.print(gps.hdop() == TinyGPS::GPS_INVALID_HDOP ? 0 : gps.hdop());
  }

  gps.stats(&chars, &sentences, &failed);
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  if (chars == 0)
  {
    Serial.println("** No characters received from GPS: check wiring **");
  }
  // fin código para leer el GPS */