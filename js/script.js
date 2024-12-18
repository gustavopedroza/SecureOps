
    // Configurações do Broker MQTT
    const BROKER = "broker.hivemq.com"; // Substitua pelo endereço do broker MQTT
    const PORT = 8000; // Porta WebSocket do broker
    const CLIENT_ID = "WebClient_" + Math.random().toString(16).substr(2, 8);

    // Tópicos MQTT
    const TOPIC_CORRENTE = "CS001";  // Tópico para corrente
    const TOPIC_RELE = "RD001";     // Tópico para relé
    const TOPIC_PIR = "MD001";      // Tópico para sensor PIR

    let client; // Cliente MQTT

    // Função para conectar ao broker MQTT
    function connectMQTT() {
      client = new Paho.MQTT.Client(BROKER, PORT, CLIENT_ID);

      // Funções de callback
      client.onConnectionLost = onConnectionLost;
      client.onMessageArrived = onMessageArrived;

      // Conexão ao broker
      client.connect({
        onSuccess: onConnect,
        onFailure: onFailure
      });
    }

    // Callback de sucesso na conexão
    function onConnect() {
      console.log("Conectado ao broker MQTT");
      // Inscrever-se nos tópicos
      client.subscribe(TOPIC_CORRENTE);
      client.subscribe(TOPIC_RELE);
      client.subscribe(TOPIC_PIR);
    }

    // Callback de falha na conexão
    function onFailure(error) {
      console.error("Falha ao conectar ao broker MQTT:", error.errorMessage);
      setTimeout(connectMQTT, 5000); // Tentar reconectar após 5 segundos
    }

    // Callback de desconexão
    function onConnectionLost(responseObject) {
      if (responseObject.errorCode !== 0) {
        console.error("Conexão perdida:", responseObject.errorMessage);
        connectMQTT(); // Tentar reconectar
      }
    }

    // Callback para mensagens recebidas
    function onMessageArrived(message) {
      const topic = message.destinationName;
      const payload = message.payloadString;

      // Exibir mensagens de acordo com o tópico
      if (topic === TOPIC_CORRENTE) {
        displayMessage({ topic, payload, timestamp: new Date().toLocaleTimeString() }, 'messageBox1');
      } else if (topic === TOPIC_RELE) {
        displayMessage({ topic, payload, timestamp: new Date().toLocaleTimeString() }, 'messageBox2');
      } else if (topic === TOPIC_PIR) {
        displayMessage({ topic, payload, timestamp: new Date().toLocaleTimeString() }, 'messageBox3');
      }
    }

    // Função para exibir mensagens no painel
    function displayMessage(message, messageBoxId) {
      const messageBox = document.getElementById(messageBoxId);

      // Cria um novo elemento de mensagem
      const newMessage = document.createElement("div");
      newMessage.textContent = `Tópico: ${message.topic} | Mensagem: ${message.payload} | Hora: ${message.timestamp}`;

      // Adiciona a nova mensagem ao painel
      messageBox.appendChild(newMessage);

      // Rola para o final da lista de mensagens
      messageBox.scrollTop = messageBox.scrollHeight;
    }

    // Função para enviar mensagem de desligamento do relé
    function desligarRele() {
      const message = new Paho.MQTT.Message("Relé desligado");
      message.destinationName = TOPIC_RELE;
      client.send(message);

      // Exibir mensagem no painel
      displayMessage({ topic: TOPIC_RELE, payload: "Relé desligado", timestamp: new Date().toLocaleTimeString() }, 'messageBox2');
    }

    // Inicialização ao carregar a página
    window.onload = function() {
      connectMQTT();
    };