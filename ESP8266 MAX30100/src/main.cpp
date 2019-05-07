#include <Arduino.h>
#include "mynetwork.h"
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

#define REPORTING_PERIOD_MS     1000

// PulseOximeter is the higher level interface to the sensor
// it offers:
//  * beat detection reporting
//  * heart rate calculation
//  * SpO2 (oxidation level) calculation
PulseOximeter pox;

uint32_t tsLastReport = 0;

// Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
        Serial.println("Beat!");
        sendUDP("<Beat>"+String(pox.getHeartRate()));
}
// timer of 1 second
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 100L; // delay between updates, in milliseconds

void setup()
{
        Serial.begin(115200);
        mynetwork_init();
        pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
        digitalWrite(LED_BUILTIN, LOW);

        Serial.print("Initializing pulse oximeter..");

        // Initialize the PulseOximeter instance
        // Failures are generally due to an improper I2C wiring, missing power supply
        // or wrong target chip
        if (!pox.begin()) {
                Serial.println("FAILED");
                for(;;);
        } else {
                Serial.println("SUCCESS");
        }

        // The default current for the IR LED is 50mA and it could be changed
        //   by uncommenting the following line. Check MAX30100_Registers.h for all the
        //   available options.
        // pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);

        // Register a callback for the beat detection
        pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop()
{
        // Make sure to call update as fast as possible
        pox.update();

        // Asynchronously dump heart rate and oxidation levels to the serial
        // For both, a value of 0 means "invalid"
        if (millis() - tsLastReport > REPORTING_PERIOD_MS) {
                Serial.print("Heart rate:");
                Serial.print(pox.getHeartRate());
                Serial.print("bpm / SpO2:");
                Serial.print(pox.getSpO2());
                Serial.println("%");

                digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
                sendUDP("<BPM>"+String(pox.getHeartRate()));
                sendUDP("<SpO2>"+String(pox.getSpO2()));


                tsLastReport = millis();
        }

}
