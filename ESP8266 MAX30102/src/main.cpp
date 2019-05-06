#include <Arduino.h>

#include <Wire.h>
#include "MAX30105.h"

#include "heartRate.h"
#include "mynetwork.h"

MAX30105 particleSensor;

const byte RATE_SIZE = 4; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;
// timer of 1 second
unsigned long lastConnectionTime = 0;             // last time you connected to the server, in milliseconds
const unsigned long postingInterval = 100L; // delay between updates, in milliseconds

void setup()
{
        Serial.begin(115200);
        Serial.println("Initializing...");
        mynetwork_init();
        pinMode(LED_BUILTIN, OUTPUT);     // Initialize the LED_BUILTIN pin as an output
        digitalWrite(LED_BUILTIN, LOW);
// Initialize sensor
        if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
        {
                Serial.println("MAX30102 was not found. Please check wiring/power. ");
                while (1);
        }
        Serial.println("Place your index finger on the sensor with steady pressure.");

        particleSensor.setup(); //Configure sensor with default settings
        particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
        particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
        particleSensor.enableDIETEMPRDY(); //Enable the temp ready interrupt. This is required.
}

void loop()
{
        Serial.print(" R[");
        Serial.print(particleSensor.getRed());
        Serial.print("] IR[");
        Serial.print(particleSensor.getIR());
        Serial.print("] G[");
        Serial.print(particleSensor.getGreen());
        Serial.print("]");

        Serial.println();
        return;
        long irValue = particleSensor.getIR();

        if (checkForBeat(irValue) == true)
        {
//We sensed a beat!
                long delta = millis() - lastBeat;
                lastBeat = millis();

                beatsPerMinute = 60 / (delta / 1000.0);

                if (beatsPerMinute < 255 && beatsPerMinute > 20)
                {
                        rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
                        rateSpot %= RATE_SIZE; //Wrap variable

//Take average of readings
                        beatAvg = 0;
                        for (byte x = 0; x < RATE_SIZE; x++)
                                beatAvg += rates[x];
                        beatAvg /= RATE_SIZE;
                }
        }

        float temperature = particleSensor.readTemperature();

        if (millis() - lastConnectionTime > postingInterval)
        {

                sendUDP("<IR>"+String(irValue));
                sendUDP("<BPM>"+String(beatsPerMinute));
                sendUDP("<AvgBPM>"+String(beatAvg));
                sendUDP("<temperature>"+String(temperature));

                digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
                lastConnectionTime = millis();

                Serial.print("IR=");
                Serial.print(irValue);
                Serial.print(", BPM=");
                Serial.print(beatsPerMinute);
                Serial.print(", Avg BPM=");
                Serial.print(beatAvg);
                Serial.print(", temperature=");
                Serial.print(temperature, 4);


                if (irValue < 50000)
                        Serial.print(" No finger?");

                Serial.println();
        }
}
