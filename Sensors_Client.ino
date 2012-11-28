#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

#define DHTTYPE DHT11

byte mac[] = {  0xDE, 0xFD, 0xBE, 0xEF, 0xFE, 0xED };

IPAddress server(192,168,1,4);
int serverPort = 1234;

EthernetClient client;

int dhtPin = A0;
int ldrPin = A1;
int gasPin = A2;

DHT dht(dhtPin, DHTTYPE);

int hum;
int temp;
int ldr;
int gas;

int lastHum = 0;
int lastTemp = 0;
int lastLdr = 0;
int lastGas = 0;

// Posicao do Sensor
int x = 1;
int y = 1;

void setup() {

  Serial.begin(9600);

  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    for(;;)
      ;
  }
  
  dht.begin();
  // Tempo para inicializacao
  delay(1000);
  
  lastHum = dht.readHumidity();
  lastTemp = dht.readTemperature();
  lastLdr = analogRead(ldrPin);
  lastGas = analogRead(gasPin);
}

void loop()
{
  
  hum = dht.readHumidity();
  temp = dht.readTemperature();
  ldr = analogRead(ldrPin);
  gas = analogRead(gasPin);
  
  Serial.println(hum);
  Serial.println(temp);
  Serial.println(ldr);
  Serial.println(gas);
  
  if(change(lastHum, hum) || change(lastTemp, temp) || change(lastLdr, ldr) || change(lastGas, gas)) {
    
    Serial.println("Change");
    lastHum = hum;
    lastTemp = temp;
    lastLdr = ldr;
    lastGas = gas; 

    // Conecta ao servidor do sistema
    if (client.connect(server, serverPort)) {
      Serial.print("Connected to Server ");
      Serial.println(server);
      // Envia dados dos sensores para o servidor
      client.print(x);
      client.print(";");
      client.print(y);
      client.print(";");
    
      client.print(hum);
      client.print(";");
      
      client.print(temp);
      client.print(";");
      
      client.print(ldr);
      client.print(";");
      
      client.print(gas);
      client.print(";");
      
      client.println("");
      Serial.println("Message sent");
      client.stop();
    }
    else {
      Serial.println("connection failed");
    }
  }
  
  Serial.println("");
  // Espera 10 segundos para enviar de novo
  delay(1000);
}

boolean change(int x, int y) {
  if (y <= x * 0.9) {
    return true;
  }
  if (y >= x * 1.1) {
    return true;
  }
  return false;
}

