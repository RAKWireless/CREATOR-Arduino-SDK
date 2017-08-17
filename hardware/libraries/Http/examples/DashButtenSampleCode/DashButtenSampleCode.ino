#include <HttpClient.h>
#include <WiFi.h>
#include <WiFiClient.h>

#define LED1  0
#define LED2  1
#define LED3  2
#define LED4  3
#define RED   0
#define GREEN 1
#define BLUE  2
#define OFF   3

void printWifiStatus();
void led_off();
void http_get(char* str);
void led_ctrl(uint8_t led_num, uint8_t rgb);

char ssid[] = "RAK_2.4GHz_1"; //  your network SSID (name) 
char pass[] = "rakwireless205";    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;            // your network key Index number (needed only for WEP)

// Name of the server we want to connect to
const char kHostname[] = "dash.multimac.pt";
const char kPath[] = "/press.php/?id=";
const int kHttpPort = 80;
// Number of milliseconds to wait without receiving any data before we give up
const int kNetworkTimeout = 30*1000;
// Number of milliseconds to wait if no data is available before trying again
const int kNetworkDelay = 1000;
int status = WL_IDLE_STATUS;
/* power enable */
int pwr_en = 15;
/* leds */
int led1_r = 25;
int led1_g = 24;
int led1_b = 19;
int led2_r = 0;
int led2_g = 2;
int led2_b = 6;
int led4_r = 12;
int led4_g = 11;
int led4_b = 13;
int led3_r = 22;
int led3_g = 21;
int led3_b = 1;
/* keys */
int key1 = 23;
int key2 = 14;
int key3 = 10;
int key4 = 20;

void setup()
{
  Serial.begin(9600); 

  pinMode(pwr_en, OUTPUT);
  digitalWrite(pwr_en, 1);

  pinMode(led2_r, OUTPUT);
  pinMode(led2_g, OUTPUT);
  pinMode(led2_b, OUTPUT);

  pinMode(led1_b, OUTPUT);
  pinMode(led3_r, OUTPUT);
  pinMode(led3_g, OUTPUT);
  pinMode(led3_b, OUTPUT);
  pinMode(led4_b, OUTPUT);

  pinMode(key2, INPUT_PULLUP);
  pinMode(key3, INPUT_PULLUP);
  pinMode(key4, INPUT_PULLUP);

#if 1
/*  
 *   Pin D21-D25 can not be used as digital IO ,when in debug mode(Enable JTAG). 
 *   D21-D25 can be used as digital IO when in factory mode.(Disable JTAG)  
*/
  //D21-D25
  pinMode(led4_g, OUTPUT);
  pinMode(led4_r, OUTPUT);
  pinMode(led1_g, OUTPUT);
  pinMode(led1_r, OUTPUT);
  pinMode(key1, INPUT_PULLUP);
#endif

  led_off();
 
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
    // wait 10 seconds for connection:
    delay(10000);
  } 
  Serial.println("Connected to wifi");
  printWifiStatus();
}

void loop()
{
  led_off();
  if (digitalRead(key1) == 0) {
    delay(50);
    if (digitalRead(key1) == 0) {
    led_ctrl(LED1,BLUE); 
    delay(500); 
    Serial.print("key1");
    http_get("1");
    }
  }
  if (digitalRead(key2) == 0) {
    delay(50);
    if (digitalRead(key2) == 0) {
    led_ctrl(LED2,BLUE);  
    delay(500);
    Serial.print("key2");
    http_get("2");
    }
  }
  if (digitalRead(key3) == 0) {
    delay(50);
    if (digitalRead(key3) == 0) {
    led_ctrl(LED3,BLUE);  
    delay(500);
    Serial.print("key3");
    http_get("3");
    }
  }
  if (digitalRead(key4) == 0) {
    delay(50);
    if (digitalRead(key4) == 0) {
    led_ctrl(LED4,BLUE);  
    delay(500);
    Serial.print("key4");
    http_get("4");
    }
  }  
delay(100);
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
  led_ctrl(LED1,RED);
  led_ctrl(LED2,RED);
  led_ctrl(LED3,RED);
  led_ctrl(LED4,RED);
  delay(500);
}

void led_off() {
   led_ctrl(LED1,OFF);
   led_ctrl(LED2,OFF);
   led_ctrl(LED3,OFF);
   led_ctrl(LED4,OFF);
}

void http_get(char* str) {
  int err = 0;
  WiFiClient c;
  HttpClient http(c);
  char path[100];
  sprintf(path,"%s%s", kPath, str);
  err = http.get(kHostname, kHttpPort, path);
  Serial.println(path);
  if (err == 0)
  {
    Serial.println("startedRequest ok");
    err = http.responseStatusCode();
    if (err >=0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
}

/*
void http_get(char* str) {
  int err = 0;
  WiFiClient c;
  HttpClient http(c);
  char path[100];
  sprintf(path,"%s%s", kPath, str);
  err = http.get(kHostname, kHttpPort, path);
  Serial.println(path);
  if (err == 0)
  {
    Serial.println("startedRequest ok");

    err = http.responseStatusCode();
    if (err >= 0)
    {
      Serial.print("Got status code: ");
      Serial.println(err);

      // Usually you'd check that the response code is 200 or a similar "success" code (200-299) before carrying on, but we'll print out whatever response we get

      err = http.skipResponseHeaders();
      if (err >= 0)
      {
        int bodyLen = http.contentLength();
        Serial.print("Content length is: ");
        Serial.println(bodyLen);
        Serial.println();
        Serial.println("Body returned follows:");
      
        // Now we've got to the body, so we can print it out
        unsigned long timeoutStart = millis();
        char c;
        // Whilst we haven't timed out & haven't reached the end of the body
        while ( (http.connected() || http.available()) &&
               ((millis() - timeoutStart) < kNetworkTimeout) )
        {
            if (http.available())
            {
                c = http.read();
                // Print out this character
                Serial.print(c);
               
                bodyLen--;
                // We read something, reset the timeout counter
                timeoutStart = millis();
            }
            else
            {
                // We haven't got any data, so let's pause to allow some to arrive
                delay(kNetworkDelay);
            }
        }
      }
      else
      {
        Serial.print("Failed to skip response headers: ");
        Serial.println(err);
      }
    }
    else
    {    
      Serial.print("Getting response failed: ");
      Serial.println(err);
    }
  }
  else
  {
    Serial.print("Connect failed: ");
    Serial.println(err);
  }
  http.stop();
}
*/

void led_ctrl(uint8_t led_num, uint8_t rgb)
{
  switch (led_num) {
    case LED1:
      if (rgb == RED) {
        digitalWrite(led1_r, 0);
        digitalWrite(led1_g, 1);
        digitalWrite(led1_b, 1);
      }
      else if (rgb == GREEN) {
        digitalWrite(led1_r, 1);
        digitalWrite(led1_g, 0);
        digitalWrite(led1_b, 1);
      }
      else if (rgb == BLUE) {
        digitalWrite(led1_r, 1);
        digitalWrite(led1_g, 1);
        digitalWrite(led1_b, 0);
      }
      else if (rgb == OFF) {
        digitalWrite(led1_r, 1);
        digitalWrite(led1_g, 1);
        digitalWrite(led1_b, 1);
      }
      break;

    case LED2:
      if (rgb == RED) {
        digitalWrite(led2_r, 0);
        digitalWrite(led2_g, 1);
        digitalWrite(led2_b, 1);
      }
      else if (rgb == GREEN) {
        digitalWrite(led2_r, 1);
        digitalWrite(led2_g, 0);
        digitalWrite(led2_b, 1);
      }
      else if (rgb == BLUE) {
        digitalWrite(led2_r, 1);
        digitalWrite(led2_g, 1);
        digitalWrite(led2_b, 0);
      }
      else if (rgb == OFF) {
        digitalWrite(led2_r, 1);
        digitalWrite(led2_g, 1);
        digitalWrite(led2_b, 1);
      }
      break;

    case LED3:
      if (rgb == RED) {
        digitalWrite(led3_r, 0);
        digitalWrite(led3_g, 1);
        digitalWrite(led3_b, 1);
      }
      else if (rgb == GREEN) {
        digitalWrite(led3_r, 1);
        digitalWrite(led3_g, 0);
        digitalWrite(led3_b, 1);
      }
      else if (rgb == BLUE) {
        digitalWrite(led3_r, 1);
        digitalWrite(led3_g, 1);
        digitalWrite(led3_b, 0);
      }
      else if (rgb == OFF) {
        digitalWrite(led3_r, 1);
        digitalWrite(led3_g, 1);
        digitalWrite(led3_b, 1);
      }
      break;

    case LED4:
      if (rgb == RED) {
        digitalWrite(led4_r, 0);
        digitalWrite(led4_g, 1);
        digitalWrite(led4_b, 1);
      }
      else if (rgb == GREEN) {
        digitalWrite(led4_r, 1);
        digitalWrite(led4_g, 0);
        digitalWrite(led4_b, 1);
      }
      else if (rgb == BLUE) {
        digitalWrite(led4_r, 1);
        digitalWrite(led4_g, 1);
        digitalWrite(led4_b, 0);
      }
      else if (rgb == OFF) {
        digitalWrite(led4_r, 1);
        digitalWrite(led4_g, 1);
        digitalWrite(led4_b, 1);
      }
      break;
    default:
      break;
  }
}
