/*__________________________________________________________________________________________________________________________*/
//Including All Libraries

#include <ESP8266WiFi.h>
#include <SinricPro.h>
#include <SinricProSwitch.h>

/*_________________________________________________________________________________________________________________________*/
// Defining all the relays and Constants
#define RELAYPIN_1 D1
#define RELAYPIN_2 D2
#define RELAYPIN_3 D3
#define WIFI_SSID         "Amuzz"    
#define WIFI_PASS         "aman13102"
#define APP_KEY           "c9f01d18-585b-4e51-a8ac-20c6c2361744" 
#define APP_SECRET        "42c3de9e-5bd4-4c3e-b4d7-3c5c9acbfd08-5516bbd7-9abc-4eb2-980a-b25e705dcb08" 
/*_________________________________________________________________________________________________________________________*/


//Setting Up relays(getting Informaton From Sentic Pro)

struct RelayInfo {
  String deviceId;
  String name;
  int pin;
};
//Defining Device Id Of Senric Pro
std::vector<RelayInfo> relays = {
    {"644fbda7312d40edc3009973", "room light", RELAYPIN_1},
    {"644fc00b312d40edc3009d95", "kitchen light", RELAYPIN_2},
    {"644fc096312d40edc3009e20", "bed room light", RELAYPIN_3},
   };
/*_____________________________________________________________________________________________________________________*/

// getting Information of Relay and Its State
bool onPowerState(const String &deviceId, bool &state) {
  for (auto &relay : relays) {                                                            //-> for each relay configuration
    if (deviceId == relay.deviceId) {                                                       //->check if deviceId matches
      Serial.printf("Device %s turned %s\r\n", relay.name.c_str(), state ? "on" : "off");     //->print relay name and state to serial
      digitalWrite(relay.pin, state);                                                         // -> set state to digital pin / gpio
      return true;                                                                            // -> return with success true
    }
  }
  return false; // if no relay configuration was found, return false
}

void setupSinricPro() 
  {
    for (auto &relay : relays) 
    {                             // for each relay configuration
      SinricProSwitch &mySwitch = SinricPro[relay.deviceId];   // create a new device with deviceId from relay configuration
      mySwitch.onPowerState(onPowerState);                     // attach onPowerState callback to the new device
    }

    SinricPro.onConnected([]() { Serial.printf("Connected to SinricPro\r\n"); });
    SinricPro.onDisconnected([]() { Serial.printf("Disconnected from SinricPro\r\n"); });

    SinricPro.begin(APP_KEY, APP_SECRET);
  }
/*_____________________________________________________________________________________________________________________________*/
void setup() 
  {
    Serial.begin(9600);


    /*-----------------------------------------------------------------------------*/
    //Initilizing the Relays Pins
    /*-----------------------------------------------------------------------------*/
    for (auto &relay : relays) 
        {
          pinMode(relay.pin, OUTPUT);     // set pinMode to OUTPUT
        }
    /*-----------------------------------------------------------------------------*/


    // Setting Up WIFI
    /*-----------------------------------------------------------------------------*/
    Serial.printf("\r\n[Wifi]: Connecting");
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED) 
      {
        Serial.printf(".");
        delay(250);
      }
    Serial.printf("connected!\r\n[WiFi]: IP-Address is %s\r\n", WiFi.localIP().toString().c_str());
    /*-----------------------------------------------------------------------------*/
    setupSinricPro();
  }

void loop() 
  {
    SinricPro.handle();
  }