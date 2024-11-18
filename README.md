# **SecureOps - Sistema de Monitoramento e Controle de Equipamentos Industriais via MQTT**

## **Descrição do Funcionamento e Uso**

O **SecureOps** é um sistema de monitoramento e controle de equipamentos industriais, baseado na tecnologia de Internet das Coisas (IoT), utilizando o protocolo MQTT para comunicação. O sistema é composto por um conjunto de sensores, atuadores e uma plataforma de microcontrolador (NodeMCU) para gerenciar as interações entre eles. A principal função do **SecureOps** é detectar atividades não autorizadas em equipamentos industriais, como a ativação indevida de um dispositivo ou falha na operação, e enviar alertas para os operadores ou administradores.

### **Funcionalidade Principal:**
- **Sensores**: Detectam mudanças no ambiente (por exemplo, presença de movimento, sobrecarga de corrente).
- **Atuadores**: Realizam ações de controle (como ligar/desligar equipamentos, acionar alarmes ou outros dispositivos).
- **Protocolo MQTT**: Utilizado para comunicação em tempo real entre os dispositivos IoT e a plataforma de monitoramento. 

### **Como Reproduzir**
1. Clone o repositório em sua máquina:
2. Instale as dependências necessárias:
   - **NodeMCU/ESP8266**: Para programar o dispositivo, você precisará do **Arduino IDE** com suporte ao NodeMCU/ESP8266.
   - **Bibliotecas MQTT**: Utilize a biblioteca **PubSubClient** para MQTT no Arduino IDE.
   - **Fritzing**: Para visualizar os esquemas eletrônicos.
   
3. Conecte os sensores e atuadores aos pinos correspondentes no NodeMCU conforme o esquema eletrônico.
   
4. Carregue o código do firmware para o NodeMCU via Arduino IDE. O código gerenciará a leitura dos sensores e acionamento dos atuadores, enviando e recebendo dados via MQTT.

5. Configure o broker MQTT (por exemplo, **Mosquitto**) ou use um broker em nuvem (como **CloudMQTT**).

6. Acesse a plataforma de monitoramento para visualizar os dados em tempo real e configurar alertas.

---

## **Hardware Utilizado**

### **Plataforma de Desenvolvimento:**
- **NodeMCU (ESP8266)**: Microcontrolador de baixo custo com suporte a Wi-Fi, ideal para aplicações de IoT. Ele é utilizado para controlar os sensores, atuar sobre os dispositivos e se comunicar com o broker MQTT via rede Wi-Fi.

### **Sensores:**
1. **Sensor PIR (Infrared Passive Motion)**: Utilizado para detectar movimentos em áreas específicas. Este sensor é essencial para detectar atividades não autorizadas em áreas monitoradas.
   - **Especificação**: Pode detectar movimentos a uma distância de até 6 metros e uma área de 120º.

2. **Sensor de Corrente AC (100A)**: Monitora a corrente elétrica de equipamentos industriais, detectando sobrecargas ou falhas no funcionamento.
   - **Especificação**: Capacidade de medir correntes até 100A, com saída analógica proporcional à intensidade da corrente.

### **Atuadores:**
- **Relé 2TY 10A**: Utilizado para controlar a ativação ou desativação de equipamentos industriais de acordo com os sinais enviados pelo NodeMCU.
   - **Especificação**: Capacidade de controlar cargas de até 10A em corrente alternada (AC) ou contínua (DC), permitindo o controle de equipamentos de média potência.

### **Fontes de Alimentação:**
- **Fonte de Alimentação 5V**: Necessária para alimentar o NodeMCU e os sensores.

---

## **Softwares e Ferramentas Utilizadas**

### **1. Arduino IDE**
- Utilizado para programar o NodeMCU/ESP8266. A IDE é configurada para compilar e enviar o código para o dispositivo, que realiza o controle de sensores e atuadores, além de estabelecer a comunicação MQTT.

### **2. PubSubClient (Biblioteca MQTT)**
- Biblioteca de código aberto utilizada para comunicação MQTT entre o NodeMCU e o broker. Essa biblioteca facilita a troca de mensagens entre o microcontrolador e a plataforma de monitoramento.


