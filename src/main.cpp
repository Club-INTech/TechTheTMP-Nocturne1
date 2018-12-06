#include "Arduino.h"
#include "robot.h"
#include "autoscript.h"
#include "commande_serial.h"

#define _GLIBCXX_USE_C99 1

void initPins() {
    digitalWrite(13, HIGH); // led de débug

    // Préparation de l'ascenseur
    pinMode(DIR_PIN_1, OUTPUT);
    pinMode(STEP_PIN_1, OUTPUT);
    pinMode(RST_PIN, OUTPUT);
    pinMode(DIR_PIN_2, OUTPUT);
    pinMode(STEP_PIN_2, OUTPUT);

    // Préparation du MOSFET pour la pompe
    pinMode(VALVE_PIN_1, OUTPUT);
    pinMode(VALVE_PIN_2, OUTPUT);
    digitalWrite(RST_PIN, HIGH);
}

void setup()
{
    pinMode(13, OUTPUT);

    DebugSerial.begin(115200);
    DebugSerial.println("Init");

    initPins();
    // les moteurs fournissent du couple
    for(auto mot:motors) {
        mot->toggleTorque(true);
    }

    for(int i = 0; i < 6; ++i) {
        syncWriteData->setMotorID(i, i+1);
    }

    DebugSerial.println("Ready :)");
    delay(1000);

    digitalWrite(13, LOW);
}

void loop() {
#ifndef COMMANDE_SERIAL
    autoExecuteStep();
#else
    executeFromSerial();
#endif

    delay(100);
    digitalWrite(13, LOW);
}
