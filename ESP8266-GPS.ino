#include <ESP8266WiFi.h>      //ESP8266WiFi.h .- ESP8266 Core WiFi Library
#include <ESP8266WebServer.h> //ESP8266WebServer.h .- Servidor web local utilizado para servir el portal de configuración
#include <DNSServer.h>        //DNSServer.h .- Local WebServer usado para servir el portal de configuración (https://github.com/zhouhan0126/DNSServer---esp32)
#include <WiFiManager.h>      //WiFiManager.h .- WiFi Configuration Magic (https://github.com/zhouhan0126/DNSServer---esp32) >> https://github.com/zhouhan0126/DNSServer---esp32 (ORIGINAL)
#include <ESP8266HTTPUpdateServer.h>
#include <TinyGPS++.h> //Libreria para mostrar data de GPS
#include <SoftwareSerial.h>

SoftwareSerial ss(4, 5);              // The serial connection to the GPS device
const int PIN_AP = 2;                 // pulsador para volver al modo AP
const char *AP_SSID = "ESP_AP";       // nombre del access point para configurar la conexión WiFi
const char *AP_PASSWORD = "12345678"; // password del access point

// variables para la muestra de datos del GPS
TinyGPSPlus gps;
float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str;
int pm;

//declaración de objeto wifiManager
WiFiManager wifiManager;

ESP8266WebServer webServer(80);
ESP8266HTTPUpdateServer httpUpdateServer;

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

void saveConfigCallback()
{
  Serial.println("Debería guardar la configuración");
  shouldSaveConfig = true;
}

void setup()
{
  Serial.begin(115200);
  pinMode(PIN_AP, INPUT);

  //modificamos el portal
  wifiManager.setCustomHeadElement("<style>html { color: #c5c6c7; background-color: #1f2833; } a:visited, a { color: #66fcf1; transition: 200ms; } a:hover, button:hover { color: #ff256c; } button { background-color: transparent; transition: 200ms; border: 5px solid #66fcf1; color: #c5c6c7; height: 15vh; }</style>");

  // utilizando ese comando, como las configuraciones se apagarán en la memoria
  // en caso de que la redacción se conecte automáticamente, ella é apagada.
  // wifiManager.resetSettings();

  //devolución de llamada para cuando entra en el modo de configuración AP
  wifiManager.setAPCallback(configModeCallback);
  //devolución de llamada cuando se conecta a una red, es decir, cuando pasa a trabajar en modo EST
  wifiManager.setSaveConfigCallback(saveConfigCallback);

  //crea una red de nombre ESP_AP con pass 12345678
  wifiManager.autoConnect(AP_SSID, AP_PASSWORD);

  httpUpdateServer.setup(&webServer);

  webServer.on("/", handleRoot);

  webServer.on("/gps", HTTP_GET, []() {
    webServer.sendHeader("Access-Control-Allow-Origin", "*");
    webServer.sendHeader("Access-Control-Allow-Methods", "GET,OPTIONS");
    webServer.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
    
    gps.encode(ss.read()); // leemos la posicion GPS
    Serial.print("LAT=");   Serial.println(gps.location.lat(), 6);
    Serial.print("LONG=");  Serial.println(gps.location.lng(), 6);
    Serial.print("ALT=");   Serial.println(gps.altitude.meters());
    Serial.print("DATE=");   Serial.println(gps.date.value());
    Serial.print("TIME=");   Serial.println(gps.time.value());

    String response = "";
    response.concat(gps.location.lat());
    response.concat(",");
    response.concat(gps.location.lng());
    webServer.send(200, "text/plain", response);
  });

  webServer.begin(); // iniciamos el servidor web
}

void loop()
{
  //si el botón se ha presionado
  /* if (digitalRead(PIN_AP) == HIGH)
  {
    Serial.println("reajustar"); //resetear intenta abrir el portal
    if (!wifiManager.startConfigPortal(AP_SSID, AP_PASSWORD))
    {
      Serial.println("No se pudo conectar");
      delay(2000);
      ESP.restart();
      delay(1000);
    }
    Serial.println("conectado ESP_AP!!!");
  } */
  webServer.handleClient();
}

void handleRoot()
{
  webServer.sendHeader("Access-Control-Allow-Origin", "*");
  webServer.sendHeader("Access-Control-Allow-Methods", "GET,OPTIONS");
  webServer.sendHeader("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  webServer.send(200, "text/plain", "Chico te paseo!!!");
}
