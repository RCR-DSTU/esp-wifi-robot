#include <Arduino.h>
#include <HardwareSerial.h>
#include <ESPAsyncWebServer.h>
#include "SPIFFS.h"

// objects
AsyncWebServer server(80);
HardwareSerial SerialPort(2);

// Wifi settings
const char* ssid = "ROBOTECH";
const char* password = "vaeG7nmt";
String clientIP = "";

// Hardware settings
const int ledPin = 2;
int state;
float speed;


bool isOperator = false;
String processor(const String& var){
	Serial.println("test");
  if(var == "SWITCH"){
	if(!isOperator) {
	return "<p allign=\"center\"><h4>get controll robot</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"getControlled(this)\" disabled><span class=\"slider\"></span></label></p>";
	} else {
	return "<p allign=\"center\"><h4>get controll robot</h4><label class=\"switch\"><input type=\"checkbox\" onchange=\"getControlled(this)\"><span class=\"slider\"></span></label></p>";		
	}
  }
  return String();
}

void setup() {
	// Serial connection
	pinMode(ledPin, OUTPUT);
	Serial.begin(115200);
	SerialPort.begin(115200, SERIAL_8N1, 16, 17);

	SPIFFS.begin(true);

	// Wifi connection
	WiFi.mode(WIFI_STA);
	WiFi.begin(ssid, password);
	Serial.print("Connecting to WiFi..");
	while(WiFi.status() != WL_CONNECTED)
	{
		delay(500);
		Serial.print(".");
	}
	Serial.println("\r\nWiFi connected!");
	Serial.println(WiFi.localIP());

   server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
		if(String(request->client()->remoteIP()) == clientIP || clientIP == "") isOperator = true;
		else isOperator = false;
		request->send(SPIFFS, "/index.html", String(), false, processor);
  });
  
  // URL для файла «style.css»:
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/style.css", "text/css");
  });

  // URL для файла «style.css»:
  server.on("/rcr", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send(SPIFFS, "/rcr.jpg", "image/jpg");
  });

  server.on("/update", HTTP_POST, [](AsyncWebServerRequest *request){
	String newState, sliderState;

	if(request->hasParam("operator")) {
		int isControl = request->getParam("operator")->value().toInt();
		if(isControl == 0) { 
			clientIP = ""; 
			digitalWrite(ledPin, LOW);
			Serial.print("Disconnected from operator with IP: "); 
			Serial.println(request->client()->remoteIP()); }
		else { 
			clientIP += request->client()->remoteIP();
			Serial.print("Connected control from operator with IP: ");
			Serial.println(request->client()->remoteIP()); 
			digitalWrite(ledPin, HIGH); }
	}

	if(String(request->client()->remoteIP()) == clientIP) {
		if(request->hasParam("state")) {
			newState = request->getParam("state")->value();
			Serial.println("New state - " + newState);
			state = newState.toInt();
		}

		if(request->hasParam("value")) {
			sliderState = request->getParam("value")->value();
			Serial.println("Slider state - " + sliderState);
			speed = sliderState.toFloat();
		}
	}
  });
  // Запускаем сервер:
  server.begin();
}

void loop() {}