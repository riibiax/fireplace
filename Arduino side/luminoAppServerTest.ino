//neoMatrix
#include <Adafruit_GFX.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
//wifi
// Include required libraries
#include <Adafruit_CC3000.h>
#include <SPI.h>
#define PIN 6
// Define CC3000 chip pins
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// WiFi network
#define WLAN_SSID       "William"        
#define WLAN_PASS       "head01/07/1983"
#define WLAN_SECURITY   WLAN_SEC_WPA2
// What TCP port to listen on for connections.
#define LISTEN_PORT           8888 
// Create CC3000 instances
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                                         SPI_CLOCK_DIVIDER);
// Create server
Adafruit_CC3000_Server luminoServer(LISTEN_PORT);

Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(8, 8, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_RIGHT +
  NEO_MATRIX_COLUMNS + NEO_MATRIX_PROGRESSIVE,
  NEO_GRB            + NEO_KHZ800);

//wifi
//char prevResult= '0';
char result='0'; //à mettre 0
int wire1=2;
int wire2=4;
int wire3=7;

void setup(){
  Serial.begin(115200);
//Matrix LED part
  setupLED();
//wifi part
  setupModem();
//wire part
  setupWire();
}

void setupLED(){
  matrix.begin();
  matrix.setBrightness(255);
}

void setupWire(){
  pinMode(wire1, OUTPUT);
  pinMode(wire2, OUTPUT);
  pinMode(wire3, OUTPUT);
}

void setupModem(){
  /* Initialise the module */
  Serial.println(F("\nInitializing..."));
  if (!cc3000.begin())
  {
   // Serial.println(F("Couldn't begin()! Check your wiring?"));
    while(1);
  }
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY)) {
    Serial.println(F("Failed!"));
    while(1);
  }
  Serial.println(F("Connected!"));
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100); // ToDo: Insert a DHCP timeout!
  }  
  /* Display the IP address DNS, Gateway, etc. */  
  while (! displayConnectionDetails()) {
    delay(100);
  }
  // Start listening for connections
  luminoServer.begin();
  Serial.println(F("\nListening for connections..."));
}


void loop(){
//programs part
  loopProgram();
//wifi part
  loopModem();
//wire part
  loopWire();
}

void loopModem(){
  Adafruit_CC3000_ClientRef client = luminoServer.available();
  // Try to get a client which is connected.
  if (client) {
     // Check if there is data available to read.
     if (client.available() > 0) {
       char c = client.read();
       if(c=='1' or c=='2' or c=='3' or c=='4' or c=='5'){
         result=c;
         Serial.println(result);
       }
     }
  }
}

void loopWire(){
  switch(result){
    case '1':
      //light
      digitalWrite(wire1,1);
      digitalWrite(wire2,1);
      digitalWrite(wire3,1);
      break;
    case '2':
      //moderate
      digitalWrite(wire1,1);
      digitalWrite(wire2,1);
      digitalWrite(wire3,0);
      break;
    case '3':
      //busy
      digitalWrite(wire1,1);
      digitalWrite(wire2,0);
      digitalWrite(wire3,1);
      break;
    case '4':
      //sportive
      digitalWrite(wire1,0);
      digitalWrite(wire2,1);
      digitalWrite(wire3,1);
      break;
    default:
      digitalWrite(wire1,0);
      digitalWrite(wire2,0);
      digitalWrite(wire3,0);
      break;
  }
}

void loopProgram(){
  switch (result){
    case '1':
      //light, alpha
      matrix.setBrightness(50);
      program(90);
      break;
    case '2':
      //moderate
      matrix.setBrightness(45);
      program(133);
      break;
    case '3':
      //busy
      matrix.setBrightness(40);
      program(166);
      break;
    case '4':
      //sportive, beta
      matrix.setBrightness(35);
      program(250);
      break;
    default:
      break; 
  }
}

void program(int frequence){
  sequence();
  delay(frequence);
  breakSequence();
  delay(frequence);
}

void sequence(){
  matrix.fillScreen(matrix.Color(255, 255, 255));
  matrix.show();
}

void breakSequence(){
  matrix.fillScreen(matrix.Color(0, 0, 0));
  matrix.show();
}

bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;
  
  if(cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv)){
    Serial.println(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    return true;
  }
}
