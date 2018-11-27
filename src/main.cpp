#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include "XL430.h"
#include "SyncWrite.h"
#include <vector>

#define _GLIBCXX_USE_C99 1


// Liste des moteurs du bras 1
static DynamixelManager* manager = new DynamixelManager(&Serial1);
static XL430* motor1 = new XL430(1,*manager);
static XL430* motor2 = new XL430(2,*manager);
static XL430* motor3 = new XL430(3,*manager);
// Liste des moteurs du bras 2
static XL430* motor4 = new XL430(4,*manager);
static XL430* motor5 = new XL430(5,*manager);
static XL430* motor6 = new XL430(6,*manager);

static std::vector<XL430*> motors{motor1,motor2,motor3,motor4,motor5,motor6};

static char* syncAngles = new char[XL430::xl430GoalAngle.length*6]; // TODO: changer pour 6 moteurs quand ce sera appliquable
static SyncWrite* syncWriteData = new SyncWrite(*manager, 3, (uint16_t ) (XL430::xl430GoalAngle.address[0] | (XL430::xl430GoalAngle.address[1] << 8)), XL430::xl430GoalAngle.length); // TODO: changer pour 3 moteurs quand appliquable


// Définition des différentes positions
static float positionDroit[3] = {180.0f,180.0f,180.0f};
static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionDistributeur[3] = {234.0f, 56.0f, 93.0f};
static float positionStockage[3] = {/*Valeur de test:*/184.0f/*187.0f*/, 268.0f, 83.0f};
static float positionSol[3] = { /*Valeur de test:*/ 278.0f /*272.0f*/, 97.0f, 189.0f};
static float positionIntermediaire[3] = {208.0f, 265.0f, 88.0f};

// Pins pour l'ascenseur
const uint8_t STEP_PIN_1 = 2; // Vitesse
const uint8_t RST_PIN = 3; // Reset
const uint8_t DIR_PIN_1 = 4; // Direction
const uint8_t STEP_PIN_2 = 6;
const uint8_t DIR_PIN_2 = 7;
const unsigned int ELEVATOR_TEMPO = 800; //gris

// Pompe
const uint8_t VALVE_PIN_1 = 5;
const uint8_t VALVE_PIN_2 = 8;

int aParcourrir[2];


void setup()
{
    pinMode(13, OUTPUT);

    Serial2.begin(115200);
    Serial2.println("Init");

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
    // les moteurs fournissent du couple
    for(auto mot:motors){
        mot->toggleTorque(true);
    }

    for(int i(0); i<6; ++i){
        syncWriteData->setMotorID(i, i+1);
    }


    Serial2.println("Ready :)");
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

void setpos(float* positions, bool reverse, int bras=1) {
    unsigned int tempo = 500;
    Serial2.println("Starting movement");

#ifdef TEST_MOTOR
    prepareAngleData(0, positions[0]);
    prepareAngleData(1, positions[1]);
    prepareAngleData(2, positions[2]);
    syncWriteData->setData(0, &syncAngles[0]);
    syncWriteData->setData(1, &syncAngles[1*XL430::xl430GoalAngle.length]);
    syncWriteData->setData(2, &syncAngles[2*XL430::xl430GoalAngle.length]);
    syncWriteData->send();
#else
    for(unsigned int i = 0;i<3;i++) {
        unsigned int motorIndex = i;
        if(reverse) {
            motorIndex = 2-motorIndex;
        }
        motorIndex+=(bras-1)*3;
        float angle = positions[motorIndex];
        Serial2.printf("%i -> %f\n", motorIndex, angle);
        Serial2.println(motors.at(motorIndex)->setGoalAngle(angle));
        delay(tempo);
    }
#endif
    delay(tempo);
    Serial2.println("Ending movement");
}

// Permet de positionner le bras dans une position donnée

void setpos(float* positions, int bras=1) {
    setpos(positions, false, bras);
}


// Commande de l'ascenseur
void cmdAscenseur(int nbPas, int cote=1)
{
    aParcourrir[cote-1]+=nbPas;
}

void executeFromSerial() {
    if(Serial2.available())
    {
        String input = Serial2.readString(100).toLowerCase(); // lit une commande
        digitalWrite(13, HIGH);


        // Commandes du bras
        if(input == "droit")
        {
            setpos(positionDroit);
            Serial2.println("ok");
        }
        else if(input == "accel")
        {
            setpos(positionAccelerateur);
            Serial2.println("ok");
        }
        else if(input == "distributeur")
        {
            setpos(positionDistributeur);
            Serial2.println("ok");
        }
        else if(input == "stockage")
        {
            setpos(positionStockage);
            Serial2.println("ok");
        }
        else if(input == "sol")
        {
            setpos(positionSol);
            Serial2.println("ok");
        }
        else if(input == "posinter")
        {
            setpos(positionIntermediaire);
            Serial2.println("ok");
        }

            // Toggle du couple fourni par les moteurs
        else if(input == "toff")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i)->toggleTorque(false);
            }
            Serial2.println("ok");
        }
        else if(input == "ton")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i)->toggleTorque(true);
            }
            Serial2.println("ok");
        }

            // Donne les angles des moteurs à un moment donné
        else if(input == "output")
        {
            Serial2.println("--------------------------------");
            Serial2.printf("|  Motor 1 | Motor 2 | Motor 3 |\n");
            float angle1 = 0.f;
            float angle2 = 0.f;
            float angle3 = 0.f;
            motor1->getCurrentAngle(angle1);
            motor2->getCurrentAngle(angle2);
            motor3->getCurrentAngle(angle3);
            Serial2.printf("|  %f3 | %f3 | %f3 |", angle1, angle2, angle3);
            Serial2.println("--------------------------------");
            Serial2.println("ok");
        }

            // Commande de l'ascenseur
        else if(input.startsWith("asc"))
        {
            int nbPas = input.substring(3).toInt();
            cmdAscenseur(nbPas);
        }

            // Commande de la pompe
        else if(input == "pompeon")
        {
            digitalWrite(VALVE_PIN_1, LOW);
        }
        else if(input == "pompeoff")
        {
            digitalWrite(VALVE_PIN_1, HIGH);
        }
        else
        {
            Serial2.println("Commande non reconnue " + input);
        }
    }
}

void unsuck(int cote=1) {
    if(cote==1) {
        digitalWrite(VALVE_PIN_1, HIGH);
    }else{
        digitalWrite(VALVE_PIN_2, HIGH);
    }
    delay(500);
}

void suck(int cote=1){
    if(cote==1){
        digitalWrite(VALVE_PIN_1, LOW);
    }else{
        digitalWrite(VALVE_PIN_2, LOW);
    }
    delay(500);
}

void raiseElevator(unsigned int amount, int cote=1) {
    cmdAscenseur(700*amount, cote);
}

void lowerElevator(unsigned int amount, int cote=1) {
    cmdAscenseur(-700*amount, cote);
}

void autoExecute(int cote=1) {
    setpos(positionDroit, cote);
    // Prendre au sol
    suck(cote);
    //setpos(positionIntermediaire);
    setpos(positionSol, true, cote);
    // Monter jusqu'à la pile et le ranger
    setpos(positionIntermediaire, cote);
    setpos(positionStockage, cote);
    unsuck(cote);
    lowerElevator(1, cote);

    // A LA MAIN: Mettre devant distrib !!
    digitalWrite(13, HIGH);
    delay(5000); // 5 secondes
    digitalWrite(13, LOW);

    // Prendre 1 palet dans distrib
    setpos(positionIntermediaire, cote);
    setpos(positionDistributeur, true, cote);
    suck(cote);
    // On le stocke
    setpos(positionIntermediaire, cote);
    setpos(positionStockage, cote);
    unsuck(cote);
    lowerElevator(1, cote);

    // A LA MAIN: Mettre devant accelérateur !!
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);

    // Remonter et ventouser
    raiseElevator(1, cote);
    suck(cote);

    // Mettre le palet pour le poser dans l'accélérateur
    setpos(positionIntermediaire, cote);
    setpos(positionAccelerateur, cote);
    unsuck(cote);

    // On remonte l'ascenseur et on prend le palet
    setpos(positionIntermediaire, cote);
    setpos(positionStockage, cote);
    raiseElevator(1, cote);
    suck(cote);
    setpos(positionIntermediaire, cote);
    setpos(positionAccelerateur, cote);

    // A LA MAIN: Décaler le robot sur le côté (pour pousser les palets dans l'accélérateur)
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);
    unsuck(cote);
}


bool actAscenseurs(){
    uint8_t DIR_PIN, STEP_PIN;
    for(int cote(1); cote<=2; ++cote) {
        if (cote == 1) {
            DIR_PIN = DIR_PIN_1;
            STEP_PIN = STEP_PIN_1;
        } else {
            DIR_PIN = DIR_PIN_2;
            STEP_PIN = STEP_PIN_2;
        }

        if(aParcourrir[cote-1]>0){
            digitalWrite(DIR_PIN, HIGH);
            int parcourru=0;
            while(parcourru<aParcourrir[cote-1] && parcourru<10){
                digitalWrite(STEP_PIN, HIGH);
                delay(ELEVATOR_TEMPO);
                digitalWrite(STEP_PIN, LOW);
                delay(ELEVATOR_TEMPO);
                ++parcourru;
            }
        }else{
            digitalWrite(DIR_PIN, LOW);
            int parcourru=0;
            while(parcourru>aParcourrir[cote-1] && parcourru>-10){
                digitalWrite(STEP_PIN, HIGH);
                delay(ELEVATOR_TEMPO);
                digitalWrite(STEP_PIN, LOW);
                delay(ELEVATOR_TEMPO);
                --parcourru
            }
        }

    }
}

void loop() {
#ifndef COMMANDE_SERIAL
    autoExecute();
#else
    executeFromSerial();
#endif
    actAscenseurs();
    delay(100);
    digitalWrite(13, LOW);
}
