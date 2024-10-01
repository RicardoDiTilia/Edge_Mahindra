#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include <PubSubClient.h>

// Configurações do LCD
LiquidCrystal_I2C lcd(0x27, 20, 4); // Endereço do LCD 0x27 para um display de 20 caracteres e 4 linhas

// Pinos
int pinoJoystickVert = 33;
int pinoJoystickHorz = 34;  // Ajustado para o ESP32
int pinoPot = 35;           // Ajustado para o ESP32
int pinoLEDVerde = 12;
int pinoLEDAmarelo = 13;
int pinoLEDVermelho = 14;

// Variáveis
float velocidade = 0;
float bateria = 100.0;  // Bateria inicial em 100%
int maxVelocidade = 290; // Velocidade máxima em km/h
String frenteTras = "Parado";  // Definido como Parado inicialmente
String direcao = "Centro";     // Definido como Centro inicialmente
String direcaoFinal = "Parado";
float velocidadeAnterior = 0;

// Configurações WiFi e MQTT
const char* ssid = "Wokwi-GUEST";
const char* password = "";                  // Substitua pela senha do WiFi
const char* mqtt_server = "broker.hivemq.com"; // Substitua pelo endereço do seu broker MQTT

WiFiClient espClient;
PubSubClient client(espClient);

// Função de reconexão MQTT
void reconnect() {
  while (!client.connected()) {
    Serial.print("Tentando se conectar ao MQTT...");
    if (client.connect("ESP32_Client")) {
      Serial.println("conectado!");
    } else {
      Serial.print("Falha, rc=");
      Serial.print(client.state());
      Serial.println(" Tentando novamente em 5 segundos...");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  lcd.init();
  lcd.backlight();

  pinMode(pinoLEDVerde, OUTPUT);
  pinMode(pinoLEDAmarelo, OUTPUT);
  pinMode(pinoLEDVermelho, OUTPUT);

  // Conexão WiFi
  setup_wifi();

  // Configuração do cliente MQTT
  client.setServer(mqtt_server, 1883);

  // Conectar-se ao MQTT
  reconnect();
}
void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Conectando a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectado");
  Serial.println("Endereço IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // Ler joystick e potenciômetro
  int direcaoVolante = analogRead(pinoJoystickHorz);  // Leitura joystick horizontal
  int direcaoFrenteTras = analogRead(pinoJoystickVert);  // Leitura joystick vertical
  int valorPot = analogRead(pinoPot);  // Leitura potenciômetro

  // Mapear valor do potenciômetro para velocidade
  velocidade = map(valorPot, 0, 4095, 0, maxVelocidade);  // Valores ADC do ESP32 (0-4095)

  // Determinar a direção com base no joystick para Frente/Trás
  if (direcaoFrenteTras < 1500) {
    frenteTras = "Frente";
  } else if (direcaoFrenteTras > 2500) {
    frenteTras = "Tras";
  } else {
    frenteTras = "Parado";
  }

  // Determinar a direção lateral com base no joystick
  if (direcaoVolante < 1500) {
    direcao = "Esquerda";
  } else if (direcaoVolante > 2500) {
    direcao = "Direita";
  } else {
    direcao = "Centro";
  }

  // Combinar as direções para criar a direção final
  if (frenteTras == "Frente" && direcao == "Centro") {
    direcaoFinal = "Frente";
  } else if (frenteTras == "Frente" && direcao == "Esquerda") {
    direcaoFinal = "Frente-Esquerda";
  } else if (frenteTras == "Frente" && direcao == "Direita") {
    direcaoFinal = "Frente-Direita";
  } else if (frenteTras == "Tras" && direcao == "Centro") {
    direcaoFinal = "Ré";
  } else if (frenteTras == "Tras" && direcao == "Esquerda") {
    direcaoFinal = "Ré-Esquerda";
  } else if (frenteTras == "Tras" && direcao == "Direita") {
    direcaoFinal = "Ré-Direita";
  } else if (frenteTras == "Parado" && direcao == "Centro") {
    direcaoFinal = "Parado";
  } else if (frenteTras == "Parado" && direcao == "Esquerda") {
    direcaoFinal = "Esquerda";
  } else if (frenteTras == "Parado" && direcao == "Direita") {
    direcaoFinal = "Direita";
  }

  // Atualizar estado da bateria
  if (velocidade > velocidadeAnterior) {
    // Carro acelerando
    digitalWrite(pinoLEDVerde, HIGH);
    digitalWrite(pinoLEDAmarelo, LOW);
    digitalWrite(pinoLEDVermelho, LOW);
    bateria -= 0.5;  // Diminui a bateria conforme a velocidade aumenta
  } else if (velocidade < velocidadeAnterior) {
    // Carro desacelerando
    digitalWrite(pinoLEDVerde, LOW);
    digitalWrite(pinoLEDAmarelo, HIGH);
    digitalWrite(pinoLEDVermelho, HIGH); // Acender vermelho para indicar recarga
    bateria += 0.05;  // Recarregar a bateria aos poucos quando desacelera
    if (bateria > 100.0) {
      bateria = 100.0;  // Limitar a bateria a 100%
    }
  }

  // Atualizar a velocidade anterior
  velocidadeAnterior = velocidade;

  // Publicar no MQTT
  client.publish("challenge/crowma/velocidade", String(velocidade).c_str());
  client.publish("challenge/crowma/bateria", String(bateria).c_str());
  client.publish("challenge/crowma/direcao", direcaoFinal.c_str());

  // Atualizar o LCD com informações
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Velocidade: ");
  lcd.print(velocidade);
  lcd.print(" km/h");

  lcd.setCursor(0, 1);
  lcd.print("Bateria: ");
  lcd.print(bateria);
  lcd.print("%");

  lcd.setCursor(0, 2);
  lcd.print("Direcao: ");
  lcd.print(direcaoFinal);

  delay(500);
}
