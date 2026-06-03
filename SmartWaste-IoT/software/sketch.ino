#include <WiFi.h>
#include <PubSubClient.h>

// Wi-Fi padrão do Wokwi
const char* ssid = "Wokwi-GUEST";
const char* password = "";

// Broker MQTT
const char* mqttServer = "broker.hivemq.com";
const int mqttPort = 1883;

// Tópico MQTT
const char* topic = "MACKENZIE/SMARTWASTE/VOLUMETRIA";

// Pinos
#define PIN_TRIG 5
#define PIN_ECHO 18
#define PIN_LED 19

// Escala completa da lixeira
// 0 cm = totalmente cheia
// 400 cm = vazia
#define DISTANCIA_MINIMA 0
#define DISTANCIA_MAXIMA 400

WiFiClient espClient;
PubSubClient client(espClient);

void conectarWiFi() {
  Serial.print("Conectando ao WiFi");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi conectado!");
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void conectarMQTT() {
  while (!client.connected()) {
    Serial.print("Conectando ao MQTT... ");

    String clientId = "smartwaste-esp32-";
    clientId += String(random(0xffff), HEX);

    if (client.connect(clientId.c_str())) {
      Serial.println("conectado!");
    } else {
      Serial.print("falhou, rc=");
      Serial.print(client.state());
      Serial.println(" tentando novamente em 2 segundos");
      delay(2000);
    }
  }
}

long medirDistancia() {
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);

  digitalWrite(PIN_TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);

  long duracao = pulseIn(PIN_ECHO, HIGH, 30000);

  if (duracao == 0) {
    return DISTANCIA_MAXIMA;
  }

  long distancia = duracao * 0.034 / 2;
  return distancia;
}

void setup() {
  Serial.begin(115200);

  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_LED, OUTPUT);

  conectarWiFi();

  client.setServer(mqttServer, mqttPort);
}

void loop() {
  if (!client.connected()) {
    conectarMQTT();
  }

  client.loop();

  long distancia = medirDistancia();

  // Limita a leitura entre 0 e 400 cm
  if (distancia < DISTANCIA_MINIMA) {
    distancia = DISTANCIA_MINIMA;
  }

  if (distancia > DISTANCIA_MAXIMA) {
    distancia = DISTANCIA_MAXIMA;
  }

  // 0 cm = 100% cheia
  // 400 cm = 0% cheia
  int percentualCheio = map(distancia, DISTANCIA_MINIMA, DISTANCIA_MAXIMA, 100, 0);

  if (percentualCheio < 0) percentualCheio = 0;
  if (percentualCheio > 100) percentualCheio = 100;

  // Brilho proporcional ao nível de preenchimento
  int brilho = map(percentualCheio, 0, 100, 0, 255);

  if (brilho < 0) brilho = 0;
  if (brilho > 255) brilho = 255;

  analogWrite(PIN_LED, brilho);

  String status = "VAZIA";

  if (percentualCheio >= 90) {
    status = "CRITICA";
  } else if (percentualCheio >= 70) {
    status = "ALTA";
  } else if (percentualCheio >= 40) {
    status = "NORMAL";
  } else if (percentualCheio >= 15) {
    status = "BAIXA";
  } else {
    status = "VAZIA";
  }

  String payload = "{";
  payload += "\"projeto\":\"SmartWaste\",";
  payload += "\"distancia_cm\":" + String(distancia) + ",";
  payload += "\"distancia_minima_cm\":" + String(DISTANCIA_MINIMA) + ",";
  payload += "\"distancia_maxima_cm\":" + String(DISTANCIA_MAXIMA) + ",";
  payload += "\"nivel_percentual\":" + String(percentualCheio) + ",";
  payload += "\"brilho_led\":" + String(brilho) + ",";
  payload += "\"status\":\"" + status + "\"";
  payload += "}";

  client.publish(topic, payload.c_str());

  Serial.print("Mensagem enviada: ");
  Serial.println(payload);

  delay(1000);
}