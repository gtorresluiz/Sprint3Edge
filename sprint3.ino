#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>

HTTPClient client;
const char *ssid = "Casa Gabriel";
const char *password = "091105002ss";
const char *serverAddress = "https://admin.tago.io/devices/650772ec19643c001064c137";
const char *contentHeader = "application/json";
const char *tokenHeader = "b7dd6c40-6b63-4743-8bb9-37708a72023e";
const int sensorPin = 2;  // Pin do sensor de presença
const int motorPin = 12;  // Pin do motor (altere para o seu setup)

bool isMotionDetected = false;

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  pinMode(motorPin, OUTPUT);
  init_wifi();
}

void loop() {
  // Verifique se o sensor de presença detectou movimento
  isMotionDetected = digitalRead(sensorPin) == HIGH;

  if (isMotionDetected) {
    // Ligue o motor para abrir a tampa da privada (você pode precisar ajustar a lógica do motor)
    digitalWrite(motorPin, HIGH);
    delay(2000);  // Mantenha o motor ligado por 2 segundos (ajuste conforme necessário)
    digitalWrite(motorPin, LOW);

    // Envie um POST para o servidor com a informação de que a tampa foi aberta
    send_tampa_aberta();
  }

  delay(1000);  // Aguarde 1 segundo antes de verificar novamente
}

void init_wifi() {
  Serial.println("Conectando WiFi");
  Serial.print("Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("WiFi Conectado");
  Serial.print("Meu IP eh: ");
  Serial.println(WiFi.localIP());
}

void send_tampa_aberta() {
  int statusCode = 0;
  String postData = "{\"tampa_aberta\": true}";
  
  client.begin(serverAddress);
  client.addHeader("Content-Type", contentHeader);
  client.addHeader("Device-Token", tokenHeader);
  statusCode = client.POST(postData);
  
  // Leia o código de status e o corpo da resposta
  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.println("Tampa da privada aberta");
  Serial.println();
}
