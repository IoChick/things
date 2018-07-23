/*
 * The default behavior for a Thing is to send data every 30 seconds.
 * To change its behavior on the firmware side, use the function
 * registerDefaultConfig(). See the documentation and lib examples.
 */

#include <KNoTThing.h>

#define WATER_PUMP_PIN      2
#define WATER_PUMP_ID       1
#define WATER_PUMP_NAME     "Water pump"

#define WATER_LEVEL_PIN     3
#define WATER_LEVEL_ID      2
#define WATER_LEVEL_NAME    "Water level"

#define FOOD_LEVEL_PIN     5
#define FOOD_LEVEL_ID      3
#define FOOD_LEVEL_NAME    "Food level"

KNoTThing thing;

static int water_pump_read(uint8_t *val) {
    *val = digitalRead(WATER_PUMP_PIN);
    Serial.print(F("Water Pump Status: "));
    if (*val)
      Serial.println(F("ON"));
    else
      Serial.println(F("OFF"));
    return 0;
}

static int water_pump_write(uint8_t *val) {
    digitalWrite(WATER_PUMP_PIN, *val);
    Serial.print(F("Water Pump Status: "));
    if (*val)
      Serial.println(F("ON"));
    else
      Serial.println(F("OFF"));
      /* TODO: Save water pump status in EEMPROM in to handle when reboot */
    return 0;
}

static int water_level_read(uint8_t *val) {
    *val = digitalRead(WATER_LEVEL_PIN);
    Serial.print(F("Water Level Status: "));
    if (*val)
      Serial.println(F("FULL"));
    else
      Serial.println(F("EMPTY"));
    return 0;
}

static int food_level_read(uint8_t *val) {
    *val = digitalRead(FOOD_LEVEL_PIN);
    Serial.print(F("Food Level Status: "));
    if (*val)
      Serial.println(F("FULL"));
    else
      Serial.println(F("EMPTY"));
    return 0;
}

void setup() {
    Serial.begin(9600);

    pinMode(WATER_PUMP_PIN, OUTPUT);
    pinMode(WATER_LEVEL_PIN, INPUT);
    pinMode(FOOD_LEVEL_PIN, INPUT);

    /* TODO: Read water pump status from eeprom for reboot cases */

    thing.init("KNoTThingFeed");
    thing.registerBoolData(WATER_PUMP_NAME, WATER_PUMP_ID, KNOT_TYPE_ID_SWITCH,
        KNOT_UNIT_NOT_APPLICABLE, water_pump_read, water_pump_write);

    thing.registerBoolData(WATER_LEVEL_NAME, WATER_LEVEL_ID, KNOT_TYPE_ID_SWITCH,
        KNOT_UNIT_NOT_APPLICABLE, water_level_read, NULL);

    thing.registerBoolData(FOOD_LEVEL_NAME, FOOD_LEVEL_ID, KNOT_TYPE_ID_SWITCH,
        KNOT_UNIT_NOT_APPLICABLE, food_level_read, NULL);

    /* Send data every 10 seconds*/
    thing.registerDefaultConfig(WATER_PUMP_ID, KNOT_EVT_FLAG_TIME, 10, 0, 0, 0, 0);

    /* Send data every 2 seconds*/
    thing.registerDefaultConfig(WATER_LEVEL_ID, KNOT_EVT_FLAG_TIME, 2, 0, 0, 0, 0);

    /* Send data every 2 seconds*/
    thing.registerDefaultConfig(FOOD_LEVEL_ID, KNOT_EVT_FLAG_TIME, 2, 0, 0, 0, 0);

    Serial.println(F("Remote KNoT Feed Thing"));
}

void loop() {
    thing.run();
}
