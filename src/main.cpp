#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include "XL430.h"
#include "SyncWrite.h"
#include <vector>

#define _GLIBCXX_USE_C99 1


// Liste des moteurs du bras
static DynamixelManager* manager = new DynamixelManager(&Serial1);
static XL430* motor1 = new XL430(1,*manager);
static XL430* motor2 = new XL430(2,*manager);
static XL430* motor3 = new XL430(3,*manager);

static std::vector<XL430*> motors{motor1,motor2,motor3};

static char* syncAngles = new char[XL430::xl430GoalAngle.length*3];
static SyncWrite* syncWriteData = new SyncWrite(*manager, 3, (uint16_t ) (XL430::xl430GoalAngle.address[0] | (XL430::xl430GoalAngle.address[1] << 8)), XL430::xl430GoalAngle.length);


#ifndef TEST_MOTOR
// Pins pour l'ascenseur
const uint8_t STEP_PIN = 2; // Vitesse
const uint8_t RST_PIN = 3; // Reset
const uint8_t DIR_PIN = 4; // Direction
const unsigned int ELEVATOR_TEMPO = 800; //gris

// Pompe
const uint8_t PUMP_PIN = 5;
#endif


void setup()
{
    pinMode(13, OUTPUT);

    Serial.begin(115200);
    Serial.println("Init");

    digitalWrite(13, HIGH); // led de débug

#ifndef TEST_MOTOR
    // Préparation de l'ascenseur
    pinMode(DIR_PIN, OUTPUT);
    pinMode(STEP_PIN, OUTPUT);
    pinMode(RST_PIN, OUTPUT);

    // Préparation du MOSFET pour la pompe
    pinMode(PUMP_PIN, OUTPUT);
    digitalWrite(RST_PIN, HIGH);
#endif
    // les moteurs fournissent du couple
    motor1->toggleTorque(true);
    motor2->toggleTorque(true);
    motor3->toggleTorque(true);

    syncWriteData->setMotorID(0, 1);
    syncWriteData->setMotorID(1, 2);
    syncWriteData->setMotorID(2, 3);

    Serial.println("Ready :)");
    delay(1000);

    digitalWrite(13, LOW);
}

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

// Permet de positionner le bras dans une position donnée
void setpos(float firstMotor, float secondMotor, float thirdMotor)
{
    prepareAngleData(0, firstMotor);
    prepareAngleData(1, secondMotor);
    prepareAngleData(2, thirdMotor);
    syncWriteData->setData(0, &syncAngles[0]);
    syncWriteData->setData(1, &syncAngles[1*4]);
    syncWriteData->setData(2, &syncAngles[2*4]);
    syncWriteData->send();
    /*Serial.println(motors.at(0)->setGoalAngle(firstMotor));
    delay(100);
    Serial.println(motors.at(1)->setGoalAngle(secondMotor));
    delay(100);
    Serial.println(motors.at(2)->setGoalAngle(thirdMotor));*/
}

#ifndef TEST_MOTOR
// Commande de l'ascenseur
void cmdAscenseur(int nbPas)
{
    if (nbPas < 0)
    {
        digitalWrite(DIR_PIN,LOW);
    }
    else
    {
        digitalWrite(DIR_PIN, HIGH);
    }
    for (int i = 0; i<abs(nbPas); ++i)
    {
        digitalWrite(STEP_PIN, HIGH);
        delayMicroseconds(ELEVATOR_TEMPO);
        digitalWrite(STEP_PIN, LOW);
        delayMicroseconds(ELEVATOR_TEMPO);
    }
}

#endif

void loop()
{
    if(Serial.available())
    {
        String input = Serial.readString(100).toLowerCase(); // lit une commande
        digitalWrite(13, HIGH);


        // Commandes du bras
        if(input == "droit")
        {
            setpos(180, 180, 180);
            Serial.println("ok");
        }
        else if(input == "accel")
        {
            setpos(186, 55, 141);
            Serial.println("ok");
        }
        else if(input == "distributeur")
        {
            setpos(234, 56, 93);
            Serial.println("ok");
        }
        else if(input == "stockage")
        {
            setpos(187, 268, 83);
            Serial.println("ok");
        }
        else if(input == "sol")
        {
            setpos(272, 97, 189);
            Serial.println("ok");
        }
        else if(input == "posinter")
        {
            setpos(208, 265, 88);
            Serial.println("ok");
        }

        // Toggle du couple fourni par les moteurs
        else if(input == "toff")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i)->toggleTorque(false);
            }
            Serial.println("ok");
        }
        else if(input == "ton")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i)->toggleTorque(true);
            }
            Serial.println("ok");
        }

        // Donne les angles des moteurs à un moment donné
        else if(input == "output")
        {
            Serial.println("--------------------------------");
            Serial.printf("|  Motor 1 | Motor 2 | Motor 3 |\n");
            float angle1 = 0.f;
            float angle2 = 0.f;
            float angle3 = 0.f;
            motor1->getCurrentAngle(angle1);
            motor2->getCurrentAngle(angle2);
            motor3->getCurrentAngle(angle3);
            Serial.printf("|  %f3 | %f3 | %f3 |", angle1, angle2, angle3);
            Serial.println("--------------------------------");
            Serial.println("ok");
        }

#ifndef TEST_MOTOR
        // Commande de l'ascenseur
        else if(input.startsWith("asc"))
        {
            int nbPas = input.substring(3).toInt();
            cmdAscenseur(nbPas);
        }

        // Commande de la pompe
        else if(input == "pompeon")
        {
            digitalWrite(PUMP_PIN, HIGH);
        }
        else if(input == "pompeoff")
        {
            digitalWrite(PUMP_PIN, LOW);
        }
#endif
        else
        {
           Serial.printf("Commande non reconnue '%s'\n", input);
        }
    }

    delay(100);
    digitalWrite(13, LOW);
}
