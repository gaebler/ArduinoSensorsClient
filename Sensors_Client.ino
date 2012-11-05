#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {  0xDE, 0xFD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192,168,1,5);

EthernetClient client;

void setup() {

  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }
  // Tempo para inicializacao
  delay(1000);
}

void loop()
{
  
  Serial.println("Connecting...");

  // Conecta ao servidor do sistema
  if (client.connect(server, 1234)) {
    Serial.println("Connected to server" + server);
    // Envia dados dos sensores para servidor
    for (int input = 0; input < 4; input++) {
            int sensor = analogRead(input);
            client.print(sensor);
            client.print(";");
          }
          client.println("");
          Serial.println("Message sent");
          client.stop();
  }
  else {
    Serial.println("connection failed");
  }
  
  Serial.println("");
  // Espera 5 segundos para enviar de novo
  delay(5000);
}

