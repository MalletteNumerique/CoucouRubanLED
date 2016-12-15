
////////////////
//COUCOU-rubanLED//
////////////////


/* D'après le programme Petit BOT conçu par Julien Rat sous licence CC-By-Sa. 
Ce programme sert à piloter le "Petit Bot". C'est un robot pédagogique, très peu cher, utilisé par les petits débrouillards*/


/*

Programme COUCOU-rubanLED par Guilaume Apremont/ Les Petits débrouillards Aquitaine/ sous licence CC-By-Sa
= Lampe Informative RGB WIFI

Les outils :
- Wemos Mini D1mini  
- Ruban de LED WS2812B 
La Wemos a la bonne idée de fournir une sortie 5V : juste ce qu'il faut pour alimenter le ruban de led.

Branchement :
- Le fil rouge du ruban raccordé a la PIN 5V de la Wemos
- Le fil noir du ruban raccodé a la PIN GND de la Wemos
- Le fil DATA du ruban raccordé a la PIn D4 (GPIO2) de la Wemos

Pilotage :
Connectez-vous au réseau wifi COUCOU
Vous pourrez piloter votre ruban de LED de plusieurs façons :
      1- Dans un  navigateur, tapez 192.168.4.1/ puis l'instruction ( rouge, vert, bleu, .....)
      ou
      2- Dans un  navigateur, tapez coucou.local/ puis l'instruction ( rouge, vert, bleu, .....)
      ou
      3- Installez l'apk android 
*/

//See more at: http://www.esp8266.com/viewtopic.php?f=29&t=6419#sthash.gd1tJhwU.dpuf

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h> // pour faire la redirection sur coucou.local

//////////////////////
// Définition du Wifi //
//////////////////////

const char WiFiAPPSK[] = "1234567890"; //mot de passe si besoin lorsque vous vous connectez au réseau COUCOU

#include <Adafruit_NeoPixel.h> // Télécharger la librairie ici https://github.com/adafruit/Adafruit_NeoPixel/archive/master.zip
#define PIN D4 // On définit le pin où est connecté le DATA du bandeau
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, PIN, NEO_GRB + NEO_KHZ800); // 30 -- Modifiez ce paramètre pour qu'il corresponde au nombre de pixels que vous utilisez.


////////////////////////////
// Définition des broches //
///////////////////////////
const int led = 30; // nombre de LED sur votre ruban
const int LED_PIN = 4;

WiFiServer server(80);

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}


void setup()
{
  initHardware();
  setupWiFi();
  server.begin();
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);

  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop()
{
// Regarder si un client s'est connecté
WiFiClient client = server.available();
if (!client) {
return;
}

// Lire la première ligne de la requête
String req = client.readStringUntil('\r');

client.flush();

// Tester la requête pour identifier la consigne
int val = -1; 
 
  if (req.indexOf("/rouge") != -1)
  {
   colorWipe(strip.Color(255, 0, 0), 50); // Rouge fixe
  }
  else if (req.indexOf("/vert") != -1)
  {
   colorWipe(strip.Color(0, 255, 0), 50); // Vert fixe
  }
  else if (req.indexOf("/bleu") != -1)
    {
   colorWipe(strip.Color(0, 0, 255), 50); // Bleu fixe
  }
  else if (req.indexOf("/color1") != -1)
    {
      theaterChase(strip.Color(127, 127, 127), 50); // Blanc clignotant
    }
    else if (req.indexOf("/color2") != -1)
    {
      theaterChase(strip.Color(127, 0, 0), 50); // Rouge clignotant
    }
    else if (req.indexOf("/color3") != -1)
    {
      theaterChase(strip.Color(0, 0, 127), 50); // Bleu clignotant
    }
  else if (req.indexOf("/rainbow1") != -1)
  {
      rainbowCycle(20);
 
  }
  else if (req.indexOf("/rainbow2") != -1)
  {
      rainbow(20);
 
  }
  else if (req.indexOf("/rainbow3") != -1)
  {
      theaterChaseRainbow(50);

  }
  
  else if (req.indexOf("/off") != -1) // Merci à Julien Rat pour le OFF
  {
      colorWipe(strip.Color(0, 0, 0), 0);
 
  }

// Prépare la page web de réponse. Débute par le l’en-tête (header) commun :
    
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  if (req.indexOf("/commande") != -1 ) {
    s += "<input type=\"button\" onclick=\"location.href='192.168.4.1/led/1';\" value=\" OFF \" />";
    s += "<input type=\"button\" onclick=\"location.href='192.168.4.1/led/0';\" value=\" ON \" />";

  }

// Si on a reçu l’instruction, il faut l’appliquer
  
    s += " ok ";  

    s += "</html>\n";

// Envoie la réponse au client
  
  client.print(s);
  delay(1);
  Serial.println("Client deconnecté ");
  client.flush();

}

void setupWiFi()
{
WiFi.mode(WIFI_AP);

// Do a little work to get a unique-ish name. Append the
// last two bytes of the MAC (HEX'd) to "Thing-":
uint8_t mac[WL_MAC_ADDR_LENGTH];
WiFi.softAPmacAddress(mac);
String macID = String(mac[WL_MAC_ADDR_LENGTH - 2], HEX) +
String(mac[WL_MAC_ADDR_LENGTH - 1], HEX);
macID.toUpperCase();
String AP_NameString = "COUCOU" + macID;

char AP_NameChar[AP_NameString.length() + 1];
memset(AP_NameChar, 0, AP_NameString.length() + 1);//zero out AP_NameChar

for (int i=0; i<AP_NameString.length(); i++)
AP_NameChar[i] = AP_NameString.charAt(i);

WiFi.softAP(AP_NameChar);
MDNS.begin("coucou");
MDNS.addService("http", "tcp", 80);

}

void initHardware()
{
Serial.begin(115200);
pinMode(LED_PIN, OUTPUT);
digitalWrite(LED_PIN, LOW);

} 
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}
