# SmartWaste-IoT

Sistema IoT para Otimização da Gestão de Resíduos UrbanosO SmartWaste é uma solução baseada em Internet das Coisas (IoT) alinhada ao Objetivo de Desenvolvimento Sustentável 11 (ODS 11) da ONU. O projeto monitora em tempo real o nível de preenchimento volumétrico de lixeiras urbanas utilizando um sensor ultrassônico e envia os dados via Wi-Fi para um Dashboard na nuvem através do protocolo MQTT.O sistema também atua como um Ambient Display, alterando o brilho de um LED local de forma inversamente proporcional à distância dos resíduos (lixeira cheia = LED em brilho máximo).

🚀 Como Reproduzir o ProjetoAbra o arquivo sketch.ino e copie o código fonte.No simulador Wokwi, monte o circuito utilizando os pinos indicados na seção de hardware abaixo ou importe o arquivo diagram.json.Certifique-se de que a biblioteca PubSubClient está instalada no ambiente.Execute a simulação. O ESP32 se conectará à rede virtual e passará a publicar os dados.Abra o arquivo index.html em qualquer navegador web para acompanhar o Dashboard em tempo real.

🛠️ Descrição do Hardware UtilizadoMicrocontrolador: SoC ESP32 (NodeMCU DevelKit v1) com Wi-Fi nativo.Sensor: Sensor Ultrassônico HC-SR04 (Pino Trigger no D5 e Echo no D18).Atuador: LED Amarelo de Alto Brilho (Conectado na porta digital D19).Resistência: Resistor de 220 $\Omega$ para limitação de corrente do LED.

💻 Software e Documentação do CódigoLinguagem: C++ / Arduino SDK (para o firmware do ESP32) e HTML5/JavaScript nativo (para o Dashboard).Lógica Base: O microcontrolador dispara um pulso ultrassônico, calcula a distância atual, converte em percentual de preenchimento (0% a 100%) e mapeia esse valor no ciclo de trabalho do PWM (0 a 255) para controlar a intensidade do LED. Os dados são empacotados em formato JSON e publicados a cada 1,5 segundos.

🌐 Interfaces e Protocolos de ComunicaçãoRede: Conexão sem fios via pilha nativa TCP/IP do ESP32.Protocolo de Aplicação: MQTT (Message Queuing Telemetry Transport) sob o modelo de publicação/assinatura.Broker Utilizado: HiveMQ Público (broker.hivemq.com:1883).Tópico de Publicação: MACKENZIE/SMARTWASTE/VOLUMETRIA.

🔗 Links do ProjetoVídeo de Apresentação no YouTube: Assista aqui ao vídeo com a demonstração prática

Desenvolvido por: José Fellipe (RA: 10433493) Instituição: Universidade Presbiteriana Mackenzie
