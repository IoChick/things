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

#define RAIN_PIN             A3
#define SENSOR_RAIN_ID       1
#define SENSOR_RAIN_NAME     "Chuva"

#define LUM_PIN              A2
#define SENSOR_LUM_ID        2
#define SENSOR_LUM_NAME       "Luminosidade"

#define MOTOR_PIN             5
#define MOTOR_ID              3
#define MOTOR_NAME            "Motor"

KNoTThing thing;

int rain = 0;
int lum = 0;
static uint8_t motor = 0; 

static int rain_read(int32_t *val, int32_t *multiplier)
{
    *val = analogRead(RAIN_PIN);
    Serial.print(F("RAIN: "));
    Serial.println(*val);
    *multiplier = 1;
    return 0;
}

static int rain_write(int32_t *val, int32_t *multiplier)
{
    *val = *val%256;
    rain = *val;

    analogWrite(RAIN_PIN, *val);
    Serial.print(F("RAIN: "));
    Serial.println(*val);
      /* TODO: Save light status in EEMPROM in to handle when reboot */
    return 0;
}

static int lum_read(int32_t *val, int32_t *multiplier)
{
    
    *val = analogRead(LUM_PIN);
    Serial.print(F("LUMINOSIDADE: "));
    Serial.println(*val);
    *multiplier = 1;
    return 0;
}

static int lum_write(int32_t *val, int32_t *multiplier)
{
    *val = *val%256;
    lum = *val;

    analogWrite(LUM_PIN, *val);
    Serial.print(F("LUMINOSIDADE: "));
    Serial.println(*val);
      /* TODO: Save light status in EEMPROM in to handle when reboot */
    return 0;
}

static int motor_read(uint8_t *val)
{
    *val = digitalRead(MOTOR_PIN);
    Serial.print(F("MOTOR: "));
    Serial.println(*val);
    return 0;
}

static int motor_write(uint8_t *val)
{
    motor = *val;

    digitalWrite(MOTOR_PIN, *val);
    Serial.print(F("MOTOR: "));
    Serial.println(*val);
      /* TODO: Save light status in EEMPROM in to handle when reboot */
    return 0;
}

void setup()
{
    Serial.begin(115200);

    pinMode(MOTOR_PIN, OUTPUT);
    
    /* TODO: Read lamp status from eeprom for reboot cases */
    thing.init("KTRLM");

    thing.registerIntData(SENSOR_RAIN_NAME, SENSOR_RAIN_ID, KNOT_TYPE_ID_LUMINOSITY,
        KNOT_UNIT_LUMINOSITY_LM, rain_read, rain_write);

    thing.registerIntData(SENSOR_LUM_NAME, SENSOR_LUM_ID, KNOT_TYPE_ID_LUMINOSITY,
        KNOT_UNIT_LUMINOSITY_LM, lum_read, lum_write);

    thing.registerBoolData(MOTOR_NAME, MOTOR_ID, KNOT_TYPE_ID_SWITCH,
        KNOT_UNIT_NOT_APPLICABLE, motor_read, motor_write);
    Serial.println(F("RAIN_LUM_MOTOR DEMO"));
    thing.registerDefaultConfig(SENSOR_RAIN_ID, KNOT_EVT_FLAG_TIME, 10, 0, 0, 0, 0);
    thing.registerDefaultConfig(SENSOR_LUM_ID, KNOT_EVT_FLAG_TIME, 10, 0, 0, 0, 0);
    thing.registerDefaultConfig(MOTOR_ID, KNOT_EVT_FLAG_TIME, 10, 0, 0, 0, 0);

}


void loop()
{
    thing.run();
}
