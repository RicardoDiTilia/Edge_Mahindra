# Edge_Mahindra
# Projeto IoT com ESP32

Este repositório contém dois projetos de sistemas IoT usando ESP32 que combinam sensores, displays e uma interface com Wi-Fi e MQTT para criar um sistema interativo. Esses projetos demonstram o uso de uma variedade de componentes eletrônicos, como joysticks, potenciômetros, LEDs, buzzer e displays (LCD e TM1637), para realizar a leitura de dados e transmiti-los em tempo real utilizando o protocolo MQTT.

## Detalhes dos Projetos

### Projeto 1: Sistema de Controle de Velocidade e Direção
Simula um sistema de controle de velocidade e direção de um veículo utilizando:
- **Joystick** para controle da direção (Frente/Trás e Esquerda/Direita).
- **Potenciômetro** para ajuste da velocidade.
- **Três LEDs** (verde, amarelo e vermelho) indicam o estado de aceleração e recarga da "bateria".
- **Display LCD** para exibir a velocidade, nível de bateria e a direção atual.
- **Wi-Fi** para conexão com a rede.
- **MQTT** para envio de dados de velocidade, bateria e direção para um broker remoto.

### Projeto 2: Temporizador com Notificações e Drop de Itens
Simula um temporizador avançado com um sistema de notificação baseado em horários que emite alertas sonoros e visuais quando determinados "drops" de itens estão disponíveis:
- **Display TM1637** para exibir horas e minutos.
- **Display LCD** para exibir informações de notificação sobre o drop de itens.
- **Buzzer** para emitir sons dependendo do tipo de drop.
- **Wi-Fi** para conexão com a rede.
- **MQTT** para notificação de novos drops e publicação de eventos.

## Instruções de Uso

### Projeto 1: Sistema de Controle de Velocidade e Direção

1. **Configuração do Hardware**:
   - Conecte o **joystick** aos pinos 33 e 34 do ESP32 para controle de direção.
   - Conecte o **potenciômetro** ao pino 35 para ajustar a velocidade.
   - Conecte os **LEDs** nos pinos 12, 13 e 14 para indicação de aceleração e recarga.
   - Conecte o **display LCD** usando I2C (endereço 0x27).
   - O ESP32 deverá estar conectado à rede Wi-Fi e a um broker MQTT para transmitir dados.

2. **Configuração do Software**:
   - Defina as credenciais do Wi-Fi no código (`ssid` e `password`).
   - Modifique o endereço do **broker MQTT** se necessário.
   - Faça o upload do código para o ESP32 e abra o monitor serial para visualizar as mensagens de depuração.

3. **Uso**:
   - O joystick controla a direção (Frente, Trás, Esquerda e Direita).
   - O potenciômetro ajusta a velocidade do carro.
   - O display LCD mostra as informações de velocidade, nível de bateria e direção.
   - O sistema publica os dados no tópico MQTT configurado.

### Projeto 2: Temporizador com Notificações

1. **Configuração do Hardware**:
   - Conecte o **display TM1637** aos pinos 12 (DIO) e 13 (CLK).
   - Conecte o **buzzer** ao pino 11 para emitir alertas sonoros.
   - Conecte o **display LCD** usando I2C (endereço 0x27).

2. **Configuração do Software**:
   - Defina as credenciais do Wi-Fi no código (`ssid` e `password`).
   - Modifique o endereço do **broker MQTT** se necessário.
   - Faça o upload do código para o ESP32 e abra o monitor serial para visualizar as mensagens de depuração.

3. **Uso**:
   - O display TM1637 exibe o tempo avançando rapidamente (1 segundo no sistema equivale a 1 hora).
   - A cada horário específico (3h, 6h e 9h), o sistema notifica via MQTT e emite um alerta visual e sonoro sobre um "drop" de item.
   - O display LCD mostra os detalhes do item disponível.

## Requisitos

- **Microcontrolador**: ESP32
- **Wi-Fi**: Conectividade à internet é necessária.
- **MQTT Broker**: O código utiliza o broker público **broker.hivemq.com**, mas pode ser substituído por outro broker.
- **Displays**: LCD com comunicação I2C e TM1637 de 4 dígitos.
- **Componentes adicionais**: Joystick, potenciômetro, LEDs, buzzer.

### Dependências do Projeto

Ambos os projetos utilizam várias bibliotecas que devem ser instaladas na IDE Arduino:

1. **LiquidCrystal_I2C**: Para o controle do display LCD via I2C.
   ```cpp
   #include <LiquidCrystal_I2C.h>
WiFi: Para a conexão Wi-Fi.

## Bibliotecas
#include <WiFi.h>
PubSubClient: Para o protocolo MQTT.

#include <PubSubClient.h>
TM1637: Para controlar o display TM1637 de 4 dígitos.

#include <TM1637.h>
Tone: Para controle do buzzer (biblioteca nativa do Arduino).
Outras Informações Relevantes
Broker MQTT: O broker público broker.hivemq.com foi utilizado para o envio e recebimento de mensagens MQTT. No entanto, você pode configurar um broker privado se desejar mais segurança.
Otimizando os projetos:
Para um ambiente de produção, considere adicionar segurança à conexão MQTT com autenticação e criptografia.
A alimentação dos sensores e LEDs deve ser garantida por uma fonte de 5V para evitar problemas de energia no ESP32. 
