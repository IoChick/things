/*
 * Copyright (c) 2016, CESAR.
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license. See the LICENSE file for details.
 *
 */

/*
 * The default behavior for a Thing is to send data every 30 seconds.
 * To change its behavior on the firmware side, use the function
 * registerDefaultConfig(). See the documentation and lib examples.
 */

#include <KNoTThing.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN            3         // Pin which is connected to the DHT sensor.

// Uncomment the type of sensor in use:
#define DHTTYPE           DHT11     // DHT 11 
//#define DHTTYPE           DHT22     // DHT 22 (AM2302)
//#define DHTTYPE           DHT21     // DHT 21 (AM2301)

#define TEMP_PIN             3
#define SENSOR_TEMP_ID       1
#define SENSOR_TEMP_NAME     "Temperatura"

#define UMI_PIN              3
#define SENSOR_UMI_ID        2
#define SENSOR_UMI_NAME       "Umidade"

#define LAMP_PIN             4
#define LAMP_ID              3
#define LAMP_NAME            "Lampada"

DHT_Unified dht(DHTPIN, DHTTYPE);
uint32_t delayMS;

KNoTThing thing;

int temp = 0;
int umi = 0;
static uint8_t lamp = 0;
const long interval = 2000;
unsigned long previousMillis = 0; 

static int temp_read(int32_t *val, int32_t *multiplier)
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= delayMS) {
      previousMillis = currentMillis;
      /*
      int chk = DHT.read11(TEMP_PIN);
      Serial.print(F("DHT TEMPERATURE: "));
      Serial.println(DHT.temperature);
      if(DHT.temperature > 0){
        temp = DHT.temperature;
      }*/

      sensors_event_t event;  
      dht.temperature().getEvent(&event);
      if (isnan(event.temperature)) {
        Serial.println("Error reading temperature!");
      }
      else {
        Serial.print("Temperature: ");
        temp = event.temperature;
        Serial.print(temp);
        Serial.println(" *C");
      }
      
    }
    
    //*val = DHT.temperature;
    *val = temp;
    Serial.print(F("TEMPERATURE: "));
    Serial.println(*val);
    *multiplier = 1;
    return 0;
}

static int temp_write(int32_t *val, int32_t *multiplier)
{
    *val = *val%256;
    temp = *val;

    analogWrite(TEMP_PIN, *val);
    Serial.print(F("TEMPERATURE: "));
    Serial.println(*val);
      /* TODO: Save light status in EEMPROM in to handle when reboot */
    return 0;
}

static int umi_read(int32_t *val, int32_t *multiplier)
{
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= delayMS) {
      previousMillis = currentMillis;
      /*
      int chk = DHT.read11(TEMP_PIN);
      Serial.print(F("DHT TEMPERATURE: "));
      Serial.println(DHT.humidity);
      if(DHT.humidity > 0){
        umi = DHT.humidity;
      }*/

      sensors_event_t event;  
      dht.humidity().getEvent(&event);
      if (isnan(event.relative_humidity)) {
        Serial.println("Error reading temperature!");
      }
      else {
        Serial.print("Temperature: ");
        umi = event.relative_humidity;
        Serial.print(umi);
        Serial.println(" *C");
      }
      
    }
    
    *val = umi;
    Serial.print(F("UMIDADE: "));
    Serial.println(*val);
    *multiplier = 1;
    return 0;
}

static int umi_write(int32_t *val, int32_t *multiplier)
{
    *val = *val%256;
    umi = *val;

    analogWrite(UMI_PIN, *val);
    Serial.print(F("UMIDADE: "));
    Serial.println(*val);
      /* TODO: Save light status in EEMPROM in to handle when reboot */
    return 0;
}

static int lamp_read(uint8_t *val)
{
    *val = digitalRead(LAMP_PIN);
    Serial.print(F("LAMPADA: "));
    Serial.println(*val);
    return 0;
}

static int lamp_write(uint8_t *val)
{
    lamp = *val;

    digitalWrite(LAMP_PIN, *val);
    Serial.print(F("LAMPADA: "));
    Serial.println(*val);
      /* TODO: Save light status in EEMPROM in to handle when reboot */
    return 0;
}

void setup()
{
    Serial.begin(115200);

    //pinMode(TEMP_PIN, OUTPUT);
    //pinMode(UMI_PIN, OUTPUT);
    pinMode(LAMP_PIN, OUTPUT);
    dht.begin();
    Serial.println("DHTxx Unified Sensor Example");
    // Print temperature sensor details.
    sensor_t sensor;
    dht.temperature().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Temperature");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println(" *C");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println(" *C");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println(" *C");  
    Serial.println("------------------------------------");
    // Print humidity sensor details.
    dht.humidity().getSensor(&sensor);
    Serial.println("------------------------------------");
    Serial.println("Humidity");
    Serial.print  ("Sensor:       "); Serial.println(sensor.name);
    Serial.print  ("Driver Ver:   "); Serial.println(sensor.version);
    Serial.print  ("Unique ID:    "); Serial.println(sensor.sensor_id);
    Serial.print  ("Max Value:    "); Serial.print(sensor.max_value); Serial.println("%");
    Serial.print  ("Min Value:    "); Serial.print(sensor.min_value); Serial.println("%");
    Serial.print  ("Resolution:   "); Serial.print(sensor.resolution); Serial.println("%");  
    Serial.println("------------------------------------");
    // Set delay between sensor readings based on sensor details.
    delayMS = sensor.min_delay / 1000;
    
    /* TODO: Read lamp status from eeprom for reboot cases */
    thing.init("KTTUL");

    thing.registerIntData(SENSOR_TEMP_NAME, SENSOR_TEMP_ID, KNOT_TYPE_ID_TEMPERATURE,
        KNOT_UNIT_TEMPERATURE_C, temp_read, temp_write);

    thing.registerIntData(SENSOR_UMI_NAME, SENSOR_UMI_ID, KNOT_TYPE_ID_RELATIVE_HUMIDITY,
        KNOT_UNIT_RELATIVE_HUMIDITY, umi_read, umi_write);

    thing.registerBoolData(LAMP_NAME, LAMP_ID, KNOT_TYPE_ID_SWITCH,
        KNOT_UNIT_NOT_APPLICABLE, lamp_read, lamp_write);
    Serial.println(F("TEMP_UMI_LAMP DEMO"));

    thing.registerDefaultConfig(SENSOR_TEMP_ID, KNOT_EVT_FLAG_TIME, 2, 0, 0, 0, 0);
    thing.registerDefaultConfig(SENSOR_UMI_ID, KNOT_EVT_FLAG_TIME, 2, 0, 0, 0, 0);
    thing.registerDefaultConfig(LAMP_ID, KNOT_EVT_FLAG_TIME, 5, 0, 0, 0, 0);

}


void loop()
{
    thing.run();
}
