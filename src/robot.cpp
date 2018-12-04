#include "robot.h"

#define _GLIBCXX_USE_C99 1

void prepareAngleData(unsigned int motorIndex, float angle)
{
    uint32_t targetAngleValue = (uint32_t)(angle/0.088);
    char* parameter = &syncAngles[motorIndex*4];

    for(int i = 0;i<XL430::xl430GoalAngle.length;i++)
    {
        parameter[i] = targetAngleValue & 0xFF;
        targetAngleValue = targetAngleValue >> 8;
    }
}

void setpos(float* positions, int side=1) {
    unsigned int tempo = 500;
    Serial2.println("Starting movement");

    prepareAngleData(0, positions[0]);
    prepareAngleData(1, positions[1]);
    prepareAngleData(2, positions[2]);
    syncWriteData->setData(0, &syncAngles[0]);
    syncWriteData->setData(1, &syncAngles[1*XL430::xl430GoalAngle.length]);
    syncWriteData->setData(2, &syncAngles[2*XL430::xl430GoalAngle.length]);
    syncWriteData->send();
    delay(tempo);
    Serial2.println("Ending movement");
}

// Commande de l'ascenseur
void cmdAscenseur(int nbPas, int cote=1)
{
    uint8_t DIR_PIN, STEP_PIN;
    if (cote==1) {
        DIR_PIN = DIR_PIN_1;
        STEP_PIN = STEP_PIN_1;
    } else {
        DIR_PIN = DIR_PIN_2;
        STEP_PIN = STEP_PIN_2;
    }

    if (nbPas < 0)
    {
        digitalWrite(DIR_PIN,LOW);
    }
    else
    {
        digitalWrite(DIR_PIN, HIGH);
    }
    Serial2.printf("Envoi de la commande sur asc: %i sur (%i, %i) (dir, step)... ", nbPas, DIR_PIN, STEP_PIN);
    for (int i = 0; i<abs(nbPas); ++i)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(ELEVATOR_TEMPO);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(ELEVATOR_TEMPO);
    }
    Serial2.println("Fin");
}

void unsuck(int side=1) {
    if(side == 1) {
        digitalWrite(VALVE_PIN_1, HIGH);
    } else {
        digitalWrite(VALVE_PIN_2, HIGH);
    }
    delay(500);
}

void suck(int side=1){
    if(side == 1){
        digitalWrite(VALVE_PIN_1, LOW);
    } else {
        digitalWrite(VALVE_PIN_2, LOW);
    }
    delay(500);
}

void raiseElevator(unsigned int amount, int side=1) {
    cmdAscenseur(700*amount, side);
}

void lowerElevator(unsigned int amount, int side=1) {
    cmdAscenseur(-700*amount, side);
}
