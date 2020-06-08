#include <DigiUSB.h> // Remove at the end of days...
#include <DHT11.h>

// Some Hardware
#define DHT_PIN 1
#define LED_PIN 0
#define VIN_PIN 2
#define VIN_APIN 1 // digital pin 2 is also analog pin 1

// Scale factor for ADC1 to volts
#define VOLTAGESCALE 0.0531

// Sensor
DHT11 dht;
// Store result
unsigned int t[10];
unsigned int h[10];
unsigned int v[10];

void setup() {                
  // initialize the IO pin
  pinMode(LED_PIN,OUTPUT);
  pinMode(VIN_PIN,INPUT);
  dht.setup(DHT_PIN);
}

void loop() {
  for (int i=0; i < 10; i++) {
    digitalWrite(LED_PIN,i%2);
    DigiUSB.delay(2000);
    t[i] = dht.getTemperature();
    h[i] = dht.getHumidity();
    v[i] = analogRead(VIN_APIN);
  }
  unsigned int tt=0;
  unsigned int tmax=0;
  unsigned int tmin=255;
  unsigned int ht=0;
  unsigned int hmax=0;
  unsigned int hmin=255;
  unsigned int vt=0;
  unsigned int vmax=0;
  unsigned int vmin=65534;
  for (int i=0; i < 10; i++) {
    tt+=t[i];
    if (t[i] < tmin) tmin=t[i];
    if (t[i] > tmax) tmax=t[i];
    ht+=h[i];
    if (h[i] < hmin) hmin=h[i];
    if (h[i] > hmax) hmax=h[i];
    vt+=v[i];
    if (v[i] < vmin) vmin=v[i];
    if (v[i] > vmax) vmax=v[i];
  }
  tt = tt-tmax-tmin;   // take off one min and one max value from 10 reading total
  unsigned int temp = round(tt/8);  // then divide by 8 to get a de-peaked average
  ht = ht-hmax-hmin;   // do the same for humidity readings
  unsigned int humi = round(ht/8);
  vt = vt-vmax-vmin;   // do the same for humidity readings
  float mv = vt * VOLTAGESCALE / 8;

  for (int i=0;i<20;i++) {
    digitalWrite(LED_PIN,HIGH);
    DigiUSB.delay(50);
    digitalWrite(LED_PIN,LOW);
    DigiUSB.delay(50);
    }
  DigiUSB.begin();
  DigiUSB.delay(1000);
  
  for (int i=0; i < 10; i++) {
    DigiUSB.print(t[i]);
    DigiUSB.print(',');
    DigiUSB.print(h[i]);
    DigiUSB.print(',');
    DigiUSB.print(v[i]);
    DigiUSB.println();
  }
  DigiUSB.println();
  DigiUSB.print(temp);
  DigiUSB.println('C');
  DigiUSB.print(humi);
  DigiUSB.println('%');
  DigiUSB.print(int(mv));
  DigiUSB.print('.');
  DigiUSB.println(int(mv*10)%10);
  DigiUSB.println();
}
