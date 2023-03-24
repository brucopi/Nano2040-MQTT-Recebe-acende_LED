//#############MQTT RECEBE Mensagem ACENDE LED#################
#include <WiFiNINA.h>
#include <MQTT.h>
#include <MQTTClient.h>



char ssid[] = "";        // your network SSID (name)
char pass[] = "";
int status = WL_IDLE_STATUS;     // the Wifi radio's status

WiFiClient wifiClient;
MQTTClient mqttClient;  

#define BROKER_IP    "mqtt.eclipseprojects.io"
#define DEV_NAME     "mqttdevice3"
#define MQTT_USER    "mqtt_user3"
#define MQTT_PW      "mqtt_password3"
#define TOPIC_SUBSCRIBE "bruno"   //Informe um Tópico único. Caso sejam usados tópicos em duplicidade, o último irá eliminar o anterior.

#define pinLED1 2  //D2 na placa

void setup() {
  // put your setup code here, to run once:
 pinMode(pinLED1, OUTPUT);  //porta D2 é de saida

//RGB
pinMode(LEDR, OUTPUT);
pinMode(LEDG, OUTPUT);
pinMode(LEDB, OUTPUT);


  Serial.begin(9600);
  conectawifi();

  // MQTT brokers usually use port 8883 for secure connections.
  mqttClient.begin(BROKER_IP, 1883, wifiClient);
  conectamqtt();
mqttClient.onMessage(messageReceived);
}

void loop() {
  // put your main code here, to run repeatedly:
 mqttClient.loop();
if (!mqttClient.connected()) 
  {
    conectawifi();
    conectamqtt();
  }

}


void conectawifi()
{
   // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to network: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }

  // you're connected now, so print out the data:
  Serial.println("Você está conectado ao wifi");
  
  Serial.println("----------------------------------------");
  printData();
  Serial.println("----------------------------------------");
}


void printData() {
  Serial.println("Board Information:");
  // print your board's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  Serial.println();
  Serial.println("Network Information:");
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption, HEX);
  Serial.println();
}

void conectamqtt()
{
  Serial.print("\nconectando MQTT...");
  while (!mqttClient.connect(DEV_NAME));//, MQTT_USER, MQTT_PW)) 
  {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("\nMQTT conectado!");
  Serial.print("BROKER_IP=");
  Serial.println(BROKER_IP);
  Serial.print("DEV_NAM=");
  Serial.println(DEV_NAME);
  Serial.print("MQTT_USER=");
  Serial.println(MQTT_USER);
  Serial.print("MQTT_PW=");
  Serial.println(MQTT_PW);
  Serial.print("TOPIC_SUBSCRIBE=");
  Serial.println(TOPIC_SUBSCRIBE);
  mqttClient.subscribe(TOPIC_SUBSCRIBE); //SUBSCRIBE TO TOPIC /hello
}



void messageReceived(String &topic, String &payload) {
  Serial.println("Mensagem: " + topic + " - " + payload);
  //Serial.println(payload);
  payload.trim(); //limpa a mensagem
  if (payload == "0") {
     digitalWrite(LED_BUILTIN, LOW); // turn off the LED to indicate the connection
       digitalWrite(pinLED1, LOW);
       Serial.println("LED Apagado");
       digitalWrite(LEDR, LOW); //RED
digitalWrite(LEDG, LOW); //GREEN
digitalWrite(LEDB, LOW); //BLUE
    }

    if (payload == "1") {
       digitalWrite(LED_BUILTIN, HIGH); // turn on the LED to indicate the connection
       digitalWrite(pinLED1, HIGH);
       Serial.println("Led Acesso");
       digitalWrite(LEDR, HIGH); //RED
digitalWrite(LEDG, HIGH); //GREEN
digitalWrite(LEDB, HIGH); //BLUE
    }
}
