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
    DebugSerial.println("Starting movement");

    const unsigned int servoInArmCount = 3;

    for(int i = 0; i < servoInArmCount; ++i) {
        syncWriteData->setMotorID(i, ((side-1)*3)+i+1);
    }
    for (unsigned int i = 0; i < servoInArmCount; ++i) {
        prepareAngleData(i, positions[i]);
        syncWriteData->setData(i, &syncAngles[i*XL430::xl430GoalAngle.length]);
    }
    syncWriteData->send();
    delay(tempo);
    DebugSerial.println("Ending movement");
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
    DebugSerial.printf("Envoi de la commande sur asc: %i sur (%i, %i) (dir, step)... ", nbPas, DIR_PIN, STEP_PIN);
    for (int i = 0; i<abs(nbPas); ++i)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(ELEVATOR_TEMPO);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(ELEVATOR_TEMPO);
    }
    DebugSerial.println("Fin");
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

void makeArmFollowSteps(float* positions[], unsigned int count, int side = 1) {
    for (int i = 0; i < count; ++i) {
        setpos(positions[i], side);
    }
}

void moveFromStorageToDispenser(int side = 1) {
    makeArmFollowSteps(stepsFromStorageToDistrib, 4, side);
}

void moveFromDispenserToStorage(int side = 1) {
    makeArmFollowSteps(stepsFromDistribToStorage, 4, side);
}