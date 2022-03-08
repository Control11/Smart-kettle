#include  <ESP8266WiFi.h> 

int Led1 = 12; //D6
int Led2 = 14; //D5
int But1 = 4;  //D2
int But2 = 5;  //D1 
int Buzz = 13; //D7
int Kettle = 15; //D8 ;
int Licznik = 0;
bool OnOff = 0;
double TempVal=0;
unsigned long TempMeas = 0; 

const char* ssid     ="ssid"; // Tu wpisz nazwę swojego wifi
const char* password = "password"; // Tu wpisz hasło do swojego wifi

WiFiServer server(80);

void setup() {
Serial.begin(115200);
pinMode(Led1, OUTPUT);
pinMode(Led2, OUTPUT);
pinMode(But1, INPUT_PULLUP);
pinMode(But2, INPUT_PULLUP);
pinMode(Buzz, OUTPUT);
pinMode(A0, INPUT); 
pinMode(Kettle, OUTPUT); 

WiFi.disconnect(1);
  
Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);
 
WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
 
Serial.println("");
Serial.println("WiFi connected");
 
server.begin();
Serial.println("Server started");
 
Serial.print("Use this URL to connect: ");
Serial.print("http://");
Serial.print(WiFi.localIP());
Serial.println("/");

}

void loop() {
  if (!digitalRead(But1) && digitalRead(But2)) { 
    digitalRead(Led1) ? digitalWrite(Led1, LOW) : digitalWrite(Led1, HIGH);
    digitalWrite(Kettle, OnOff=!OnOff); 
    tone(Buzz, 1000,100);
    delay(1000);
  }  
  if (!digitalRead(But2) && digitalRead(But1)) { 
    digitalRead(Led2) ? digitalWrite(Led2, LOW) : digitalWrite(Led2, HIGH);
    digitalWrite(Kettle, OnOff=LOW);
    tone(Buzz, 200,100);
  } 
  else {
    digitalWrite(Led1, LOW);  
    digitalWrite(Led2, LOW);
  }
  if(millis()-TempMeas >= 100){
    TempVal = analogRead(A0)/0.35; //temperatura przy czajniku wyłączonym
    TempMeas = millis();         
  }  

WiFiClient client = server.available();
if (!client) {
  return;
}
 
Serial.println("new client");
int timewate = 0;
while(!client.available()){
  delay(1);
  timewate = timewate +1;
  if(timewate>1800)
  {
    Serial.println(">>> Client Timeout !");
    client.stop();
    return;
  }
}
 
String request = client.readStringUntil('\r');
Serial.println(request);
client.flush();
  
int value = LOW;
if (request.indexOf("/on") != -1)  {
  digitalWrite(Kettle, HIGH);
  tone(Buzz, 1000,100);
  digitalWrite(Led1,HIGH);
  delay(400);
  digitalWrite(Led1,LOW);
  value = HIGH;
}
 
if (request.indexOf("/off") != -1)  {
  digitalWrite(Kettle, LOW);
  tone(Buzz, 200,100);
  digitalWrite(Led2,HIGH);
  delay(400);
  digitalWrite(Led2,LOW);
  value = LOW;
}

client.println("HTTP/1.1 200 OK");
client.println("Content-Type: text/html");
client.println("");
client.println("");
client.println("");
client.println("Steruj czajnikiem");
client.println("Wpisz w polu wyszukiwarki /ON aby wlaczyc lub /OFF aby wylaczyc czajnik");
client.println("");
client.println("");
client.println("");
client.println("");
client.println("");
client.println("");
delay(1);
Serial.println("Client disonnected");
Serial.println("");
}
