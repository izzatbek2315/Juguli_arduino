#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>
#include <ArduinoOTA.h>

// Motorlarni boshqarish uchun pinlar
const int in1 = D2;
const int in2 = D3;
const int in3 = D4;
const int in4 = D5;

const int buzPin = D7;  // Buzzer
const int ledPin = D8;  // Old chiroq
const int BackwardledPin = D1;  // Orqa chiroq
const int leftTurnPin = D6;  // Chap burilish chirog‘i
const int rightTurnPin = D0;  // O‘ng burilish chirog‘i
const int wifiLedPin = D10;  // Wi-Fi status LED

ESP8266WebServer server(80);
WiFiManager wifiManager;

String command;
int SPEED = 1023; // boshlang‘ich tezlik

bool hazardLights = false; // Avariyaviy chiroqlar holati
bool apModeActive = false;
unsigned long apStartMillis = 0;

void setup() {
  Serial.begin(115200);
  Serial.println("*WiFi Robot Remote Control Mode - L298N 2A*");

  // Pinlarni sozlash
  pinMode(buzPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(BackwardledPin, OUTPUT);
  pinMode(leftTurnPin, OUTPUT);
  pinMode(rightTurnPin, OUTPUT);
  pinMode(wifiLedPin, OUTPUT);

  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  // Dastlab barcha chiqishlarni o‘chirib qo‘yish
  digitalWrite(buzPin, LOW);
  digitalWrite(ledPin, LOW);
  digitalWrite(BackwardledPin, LOW);
  digitalWrite(leftTurnPin, LOW);
  digitalWrite(rightTurnPin, LOW);
  digitalWrite(wifiLedPin, HIGH);

  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // Wi-Fi ulanishi
  if (!wifiManager.autoConnect("Car_setting", "12341234")) {
    Serial.println("WiFi ulanishi topilmadi, faqat AP rejimida ishlaydi.");
  } else {
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    digitalWrite(wifiLedPin, LOW);

    // **AP rejimini Wi-Fi IP bilan yoqish**
    startAPWithIP();
  }

  // Har doim AP rejimi yoqilgan bo‘lishi kerak
  startPultAP();

  // Web server funksiyalari
  server.on("/", HTTP_handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();

  ArduinoOTA.begin();
}

void loop() {
  ArduinoOTA.handle();
  server.handleClient();
  blinkHazardLights();

  // **2 daqiqa o'tgandan keyin AP rejimini o‘chirish**
  if (apModeActive && millis() - apStartMillis >= 120000) {
    stopAPMode();
  }

  command = server.arg("State");
  if (command == "F") Forward();
  else if (command == "B") Backward();
  else if (command == "R") TurnRight();
  else if (command == "L") TurnLeft();
  else if (command == "G") ForwardLeft();
  else if (command == "H") BackwardLeft();
  else if (command == "I") ForwardRight();
  else if (command == "J") BackwardRight();
  else if (command == "S") Stop();
  else if (command == "V") BeepHorn();
  else if (command == "W") TurnLightOn();
  else if (command == "w") TurnLightOff();
  else if (command == "0") TurnLightOn_O();
  else if (command == "1") TurnLightOff_O();
  else if (command == "9") hazardLights = true;
  else if (command == "q") hazardLights = false;
  else if (command == "8") {
    hazardLights = false;
    digitalWrite(leftTurnPin, HIGH);
    digitalWrite(rightTurnPin, HIGH);
  }
}

// **AP rejimini "Car_IP:[IP]" SSID bilan 2 daqiqa yoqish**
void startAPWithIP() {
  String apSSID = "Car_IP:" + WiFi.localIP().toString();
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(apSSID.c_str(), "12341234");

  Serial.println("AP Mode Started: " + apSSID);
  apModeActive = true;
  apStartMillis = millis();
}

// **AP rejimini o‘chirish**
void stopAPMode() {
  WiFi.softAPdisconnect(true);
  Serial.println("AP Mode Stopped.");
  apModeActive = false;
}

// **Har doim ishlaydigan Pult AP rejimi**
void startPultAP() {
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP("Pult", "12341234");
  IPAddress myIP = WiFi.softAPIP();
  
  Serial.println("Pult AP doim ishlaydi!");
  Serial.print("AP IP address: ");
  Serial.println(myIP);
}

void HTTP_handleRoot() {
  server.send(200, "text/html", "");
  if (server.hasArg("State")) {
    Serial.println(server.arg("State"));
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "404: Not found");
}



// Harakat funksiyalari
void Forward() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
}

void Backward() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
}

void TurnRight() {
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  signalTurn(rightTurnPin);
}

void TurnLeft() {
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  signalTurn(leftTurnPin);
}

void ForwardLeft() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void BackwardLeft() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
}

void ForwardRight() {
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void BackwardRight() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
}

void Stop() {
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void BeepHorn() {
  int speed = 150;  // Ovozni chiqish tezligi

  tone(buzPin, 600, speed); delay(speed); // "I"
  tone(buzPin, 700, speed); delay(speed); // "Z"
  tone(buzPin, 750, speed); delay(speed); // "Z"
  tone(buzPin, 800, speed); delay(speed); // "A"
  tone(buzPin, 900, speed); delay(speed); // "T"
  tone(buzPin, 950, speed); delay(speed); // "B"
  tone(buzPin, 1000, speed); delay(speed); // "E"
  tone(buzPin, 1100, speed); delay(speed); // "K"

  noTone(buzPin); // Ovoz o'chiriladi
}

void TurnLightOn() {
  digitalWrite(ledPin, HIGH);
}

void TurnLightOff() {
  digitalWrite(ledPin, LOW);
}

void TurnLightOn_O() {
  digitalWrite(BackwardledPin, HIGH);
}

void TurnLightOff_O() {
  digitalWrite(BackwardledPin, LOW);
}

void ToggleHazardLights() {
  hazardLights = !hazardLights;
}

void blinkHazardLights() {
  static unsigned long prevMillis = 0;
  static bool state = false;
  unsigned long currentMillis = millis();

  if (hazardLights && currentMillis - prevMillis >= 500) {
    prevMillis = currentMillis;
    state = !state;
    digitalWrite(leftTurnPin, state);
    digitalWrite(rightTurnPin, state);
  }
}

void signalTurn(int turnPin) {
  digitalWrite(turnPin, HIGH);
  delay(100);
  digitalWrite(turnPin, LOW);
}
