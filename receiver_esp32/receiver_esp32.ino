#include <WiFi.h>
#include <WebServer.h>

const char* ssid     = "YOUR_WIFI_SSID";
const char* password = "YOUR_WIFI_PASSWORD";

#define AIN1 26
#define AIN2 27
#define PWMA 25
#define BIN1 32
#define BIN2 33
#define PWMB 14
#define STBY 4

const int PWM_FREQ = 5000;
const int PWM_RES  = 8;

WebServer server(80);

void motorA(int spd, bool fwd) {
  digitalWrite(AIN1, fwd ? HIGH : LOW);
  digitalWrite(AIN2, fwd ? LOW : HIGH);
  ledcWrite(PWMA, spd);
}

void motorB(int spd, bool fwd) {
  digitalWrite(BIN1, fwd ? HIGH : LOW);
  digitalWrite(BIN2, fwd ? LOW : HIGH);
  ledcWrite(PWMB, spd);
}

void stopAll() {
  digitalWrite(AIN1, HIGH); digitalWrite(AIN2, HIGH);
  digitalWrite(BIN1, HIGH); digitalWrite(BIN2, HIGH);
  ledcWrite(PWMA, 0);
  ledcWrite(PWMB, 0);
}

void processCommand(String cmd) {
  cmd.trim();
  char action = toupper(cmd.charAt(0));
  int spd = constrain(cmd.substring(1).toInt(), 0, 255);

  if (action == 'F') {
    motorA(spd, true);
    motorB(spd, true);
    Serial.print("Forward: "); Serial.println(spd);

  } else if (action == 'B') {
    motorA(spd, false);
    motorB(spd, false);
    Serial.print("Backward: "); Serial.println(spd);

  } else if (action == 'L') {
    motorA(spd, false);
    motorB(spd, true);
    Serial.print("Left: "); Serial.println(spd);

  } else if (action == 'R') {
    motorA(spd, true);
    motorB(spd, false);
    Serial.print("Right: "); Serial.println(spd);

  } else if (action == 'S') {
    stopAll();
    Serial.println("Stop");
  }
}

void setup() {
  Serial.begin(115200);

  pinMode(AIN1, OUTPUT); pinMode(AIN2, OUTPUT);
  pinMode(BIN1, OUTPUT); pinMode(BIN2, OUTPUT);
  pinMode(STBY, OUTPUT);

  digitalWrite(STBY, HIGH);

  ledcAttach(PWMA, PWM_FREQ, PWM_RES);
  ledcAttach(PWMB, PWM_FREQ, PWM_RES);

  stopAll();

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("ESP32 IP: ");
  Serial.println(WiFi.localIP());

  server.on("/cmd", []() {
    if (server.hasArg("val")) {
      processCommand(server.arg("val"));
      server.send(200, "text/plain", "OK");
    }
  });

  server.begin();
}

void loop() {
  server.handleClient();
}