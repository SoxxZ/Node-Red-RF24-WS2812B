#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <FastLED.h>

RF24 radio(9, 10); // CE, CSN

char text[32] = "";
String textStr = "";

#define LED_PIN     4
#define NUM_LEDS    30
CRGB leds[NUM_LEDS];

void colorchange(String textcc);
void rainbow(String textrb);
void interruptFunc();


void setup() {
    Serial.begin(115200);
    radio.begin();
    radio.maskIRQ(1,1,0);
    radio.openReadingPipe(0, 0x0000000001);
    radio.setPALevel(RF24_PA_MIN);
    Serial.println("Radio ready");
    radio.startListening();
    Serial.println("Listening");
    FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);
    attachInterrupt(0, interruptFunc, FALLING);
}

void loop() {
    if(textStr.startsWith("*")){    
        colorchange(textStr);
    }
    if(textStr.startsWith("/")){
        rainbow(textStr);
    }
}

void colorchange(String textcc = ""){
    
    textcc.remove(0,1);
            
    int commaIndex = textcc.indexOf(',');
    int secondCommaIndex = textcc.indexOf(',', commaIndex + 1);

    String firstValue = textcc.substring(0, commaIndex);
    String secondValue = textcc.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = textcc.substring(secondCommaIndex + 1);

    int r = firstValue.toInt();
    int g = secondValue.toInt();
    int b = thirdValue.toInt();

    Serial.println("Changing color!");
        
    int i = 0;
    while (i < 30){
        leds[i] = CRGB(r, g, b);
        FastLED.show();
        i++;
    }
    Serial.print("Color Changed to: ");
    Serial.print(textStr);
    Serial.println();
    textStr = "";
}

void rainbow(String textrb = ""){

    textrb.remove(0,1);
            
    int commaIndex = textrb.indexOf(',');
    int secondCommaIndex = textrb.indexOf(',', commaIndex + 1);

    String firstValue = textrb.substring(0, commaIndex);
    String secondValue = textrb.substring(commaIndex + 1, secondCommaIndex);
    String thirdValue = textrb.substring(secondCommaIndex + 1);

    int speed = firstValue.toInt();
    int brightness = secondValue.toInt();
    int saturation = thirdValue.toInt();

    Serial.println("Starting Rainbow animation!");


    String k = textStr;
    for (int j = 0; j < 255 && textStr.startsWith("/") == true && textStr == k ; j++) {
        for (int i = 0; i < NUM_LEDS && textStr.startsWith("/") == true && textStr == k; i++) {
            leds[i] = CHSV(i - (j * 2), saturation, brightness);
        }
    FastLED.show();
    delay(speed); 
    }
    Serial.println("End of rainbow animation!");
}

void interruptFunc(){
    Serial.println("Interrupt triggered!");
    
    if (radio.available()) {
        radio.read(&text, sizeof(text));
        Serial.print("Received: ");
        Serial.println(text);
        textStr = String(text);
    }else{
        Serial.println("No data");
    }

}