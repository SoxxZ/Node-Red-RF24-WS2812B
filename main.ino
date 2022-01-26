#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <FastLED.h>


RF24 radio(7, 8);  // CE, CSN

#define LED_PIN     4
#define NUM_LEDS    30
CRGB leds[NUM_LEDS];

char text[32] = {0};

void colorchange(String texts = "");
void rainbow(String texts = "");


void setup()
{
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    while (!Serial);
    Serial.begin(9600);
    Serial.println("Serial ready");
  
    radio.begin();
    radio.openReadingPipe(0, 0x0000000001);
    radio.startListening();

    Serial.println("Listening");
}

void loop()
{
    if (radio.available())
    {
        int rainbowspeed = 0;
        int rainbowbrightness = 0;
        int rainbowsaturation = 0;
        radio.read(&text, sizeof(text));
        Serial.print("Received: ");
        Serial.print(text);
        Serial.println();

    }
    String textStr = String(text);
    Serial.println(textStr);

    if(texts.startsWith("*")){    
        colorchange(textStr);
    }
    if(texts.startsWith("/")){
        rainbow(textStr);
    }
}

void colorchange(String textcc = "")
{
    
    textcc.remove(0,1);
            
    int commaIndex = textcc.indexOf(',');
    int secondCommaIndex = textcc.indexOf(',', commaIndex + 1);

    String firstValue = textcc.substring(0, commaIndex);
    String secondValue = textcc.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = textcc.substring(secondCommaIndex + 1); // To the end of the string

    int r = firstValue.toInt();
    int g = secondValue.toInt();
    int b = thirdValue.toInt();

    Serial.println(r);
    Serial.println(g);
    Serial.println(b);
        
    int i = 0;
    while (i < 30)
    {
        leds[i] = CRGB(r, g, b);
        FastLED.show();
        i++;
    }
}

void rainbow(String textrb = "")
{
    textrb.remove(0,1);
            
    int commaIndex = textrb.indexOf(',');
    int secondCommaIndex = textrb.indexOf(',', commaIndex + 1);

    String firstValue = textrb.substring(0, commaIndex);
    String secondValue = textrb.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = textrb.substring(secondCommaIndex + 1); // To the end of the string

    int speed = firstValue.toInt();
    int brightness = secondValue.toInt();
    int saturation = thirdValue.toInt();

    Serial.println(speed);
    Serial.println(brightness);
    Serial.println(saturation);



    for (int j = 0; j < 255; j++) {
        for (int i = 0; i < NUM_LEDS; i++) {
            leds[i] = CHSV(i - (j * 2), saturation, brightness); /* The higher the value 4 the less fade there is and vice versa */ 
        }
    FastLED.show();
    delay(speed); /* Change this to your hearts desire, the lower the value the faster your colors move (and vice versa) */
    }
}