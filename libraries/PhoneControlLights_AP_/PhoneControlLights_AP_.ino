#include <WiFi.h>

char ssid[] = "CREATOR_AP"; //  your network SSID (name)
char pass[] = "00000000";    // your network password (use for WPA, or use as key for WEP)
char channel[] = "1"; 
//int keyIndex = 0;            // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;
WiFiServer server(25000);  // numeric IP for Google (no DNS)
//char server[] = "www.google.com";    // name address for Google (using DNS)

//WiFiClient client;
void setup() {
  //Initialize serial and wait for port to open:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(9600);
  while (!Serial) {
    ;
  }
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    // don't continue:
    while (true);
  }
  String fv = WiFi.firmwareVersion();
  if (fv != "1.1.0") {
    Serial.println("Please upgrade the firmware");
  }
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Start AP to SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.apbegin(ssid,pass,channel);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.println("Success to wifi");
  server.begin();
  printWifiStatus();

//  Serial.println("\nStarting connection to server...");
//  // if you get a connection, report back via serial:
//  if (client.connect(server, 25000)) {
//    Serial.println("connected to server");
//    // Make a HTTP request:
//    //client.println("GET /search?q=ameba HTTP/1.1");
//    //client.println("Host: www.google.com");
//    //client.println("Connection: close");
//    client.println("Hello");
//  }
}

void loop() {
  // if there are incoming bytes available
  // from the server, read them and print them:
  WiFiClient client = server.available();
  if (client){
  Serial.println("new client");
  client.print("hello");
  while (client.connected()) {
  if (client.available()) {
    String c = client.readString();
    Serial.println(c);
    
    if (c == "Open")
    {
      digitalWrite(LED_BUILTIN, HIGH);
      client.print("OK");
    }
    else if (c == "Close")
    {
      digitalWrite(LED_BUILTIN, LOW);
      client.print("OK");
    }
    else
    {
      client.print("ERROR");
    }
  }
 }
  // if the server's disconnected, stop the client:
  if (!client.connected()) {
   // Serial.println();
    //Serial.println("disconnecting from server.");
    client.stop();
    Serial.println("client disonnected");
    // do nothing forevermore:
    //while (true);
  }
 }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

