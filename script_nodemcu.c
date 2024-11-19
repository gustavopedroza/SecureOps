#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// Pinos
#define pinLED1 2             // LED no GP2 (ESP-01)
#define pinRELE 5             // Relé no GPIO5 (D1 no NodeMCU)
#define pinSENSOR_PIR 4       // Sensor PIR no GPIO4 (D2 no NodeMCU)
#define pinSENSOR_CORRENTE A0 // Sensor de corrente no pino analógico

// Configuração do sensor de corrente
const int numeroAmostras = 1000; // Número de amostras para cálculo RMS
const float fatorConversao = 30.0 / 1024.0; // Conversão ADC -> corrente (exemplo para sensor SCT-013)

// WiFi
const char* SSID = "Patricia";          // SSID / nome da rede WiFi
const char* PASSWORD = "@0patricia1#"; // Senha da rede WiFi
WiFiClient wifiClient;

// MQTT
const char* BROKER_MQTT = "iot.eclipse.org"; // URL do broker MQTT
int BROKER_PORT = 1883;                      // Porta do Broker MQTT
#define ID_MQTT  "SecureOps01"               // ID único para MQTT
#define TOPIC_LED "SecureOps/LED"            // Tópico para controlar o LED
#define TOPIC_RELE "SecureOps/RELE"          // Tópico para controlar o relé
#define TOPIC_CORRENTE "SecureOps/Corrente"  // Tópico para enviar dados do sensor de corrente
#define TOPIC_PIR "SecureOps/PIR"            // Tópico para eventos do sensor PIR

PubSubClient MQTT(wifiClient);

// Declaração das Funções
void mantemConexoes();
void conectaWiFi();
void conectaMQTT();
void recebePacote(char* topic, byte* payload, unsigned int length);
void verificaSensores();
float calculaCorrenteRMS();

void setup() {
  pinMode(pinLED1, OUTPUT);
  pinMode(pinRELE, OUTPUT);
  pinMode(pinSENSOR_PIR, INPUT);

  digitalWrite(pinLED1, LOW);
  digitalWrite(pinRELE, LOW);

  Serial.begin(115200);

  conectaWiFi();
  MQTT.setServer(BROKER_MQTT, BROKER_PORT);
  MQTT.setCallback(recebePacote);
}

void loop() {
  mantemConexoes();
  MQTT.loop();
  verificaSensores();
}

void mantemConexoes() {
  if (!MQTT.connected()) {
    conectaMQTT();
  }
  conectaWiFi();
}

void conectaWiFi() {
  if (WiFi.status() == WL_CONNECTED) return;

  Serial.print("Conectando-se na rede: ");
  Serial.print(SSID);
  Serial.println("  Aguarde!");

  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(100);
    Serial.print(".");
  }

  Serial.println();
  Serial.print("Conectado com sucesso na rede: ");
  Serial.print(SSID);
  Serial.print("  IP obtido: ");
  Serial.println(WiFi.localIP());
}

void conectaMQTT() {
  while (!MQTT.connected()) {
    Serial.print("Conectando ao Broker MQTT: ");
    Serial.println(BROKER_MQTT);
    if (MQTT.connect(ID_MQTT)) {
      Serial.println("Conectado ao Broker com sucesso!");
      MQTT.subscribe(TOPIC_LED);
      MQTT.subscribe(TOPIC_RELE);
    } else {
      Serial.println("Não foi possível se conectar ao broker.");
      Serial.println("Nova tentativa de conexão em 10s");
      delay(10000);
    }
  }
}

void recebePacote(char* topic, byte* payload, unsigned int length) {
  String msg;

  // Obtém a string do payload recebido
  for (int i = 0; i < length; i++) {
    char c = (char)payload[i];
    msg += c;
  }

  if (String(topic) == TOPIC_LED) {
    if (msg == "0") digitalWrite(pinLED1, LOW);
    if (msg == "1") digitalWrite(pinLED1, HIGH);
  }

  if (String(topic) == TOPIC_RELE) {
    if (msg == "0") digitalWrite(pinRELE, LOW);
    if (msg == "1") digitalWrite(pinRELE, HIGH);
  }
}

void verificaSensores() {
  // Sensor PIR
  int movimento = digitalRead(pinSENSOR_PIR);
  if (movimento == HIGH) {
    MQTT.publish(TOPIC_PIR, "Movimento detectado");
  }

  // Sensor de Corrente AC
  float correnteRMS = calculaCorrenteRMS();
  char mensagem[10];
  dtostrf(correnteRMS, 6, 2, mensagem);
  MQTT.publish(TOPIC_CORRENTE, mensagem);

  delay(2000); // Intervalo entre leituras
}

float calculaCorrenteRMS() {
  long somaQuadrados = 0;

  // Coleta de amostras
  for (int i = 0; i < numeroAmostras; i++) {
    int leituraADC = analogRead(pinSENSOR_CORRENTE);
    int valorAjustado = leituraADC - 512; // Ajusta para 0 central
    somaQuadrados += valorAjustado * valorAjustado;
  }

  // Calcula o valor RMS
  float valorMedioQuadrado = somaQuadrados / (float)numeroAmostras;
  float correnteRMS = sqrt(valorMedioQuadrado) * fatorConversao;

  return correnteRMS;
}
