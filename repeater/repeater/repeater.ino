/**
 * Repeater node for the MySensors Arduino library.
 * 
 * It's basically the code from the example, with some small adaptions.
 */

#define MY_DEBUG
#define MY_RADIO_NRF24
#define MY_REPEATER_FEATURE

#define MY_DEFAULT_LED_BLINK_PERIOD 50
#define MY_DEFAULT_ERR_LED_PIN 4
#define MY_DEFAULT_TX_LED_PIN 5
#define MY_DEFAULT_RX_LED_PIN 6

#include <MySensors.h>  

void setup() {
}

void presentation()  {
  sendSketchInfo("Repeater", "1.0.0");
}

void loop() {
}
