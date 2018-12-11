#include "Arduino.h"
#ifndef _POSITIONS_H
#define _POSITIONS_H

static HardwareSerial& DebugSerial = Serial2;

// Définition des différentes positions
static float positionDroit[3] = {180.0f,180.0f,180.0f};
static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionDistributeur[3] = {251.0f, 125.0f, 142.0f};
static float positionPreDistributeur[3] = {223.0f, 56.0f, 100.0f};
static float positionStockage[3] = {190.0f, 271.0f, 84.0f};
static float positionSol[3] = { /*Valeur de test:*/ 278.0f /*272.0f*/, 97.0f, 189.0f};
static float positionIntermediaire[3] = {223.0f, 270.0f, 100.0f};
static float* stepsFromStorageToDistrib[] = {
        positionStockage, positionIntermediaire, positionPreDistributeur, positionDistributeur
};
static float* stepsFromDistribToStorage[] = {
        positionDistributeur, positionPreDistributeur, positionIntermediaire, positionStockage
};

// Pins pour l'ascenseur
const uint8_t STEP_PIN_1 = 2; // Vitesse
const uint8_t RST_PIN = 14; // Reset
const uint8_t DIR_PIN_1 = 4; // Direction
const uint8_t STEP_PIN_2 = 8;
const uint8_t DIR_PIN_2 = 3;
const unsigned int ELEVATOR_TEMPO = 800; //gris

// Pompe
const uint8_t VALVE_PIN_1 = 5;
const uint8_t VALVE_PIN_2 = 14;

#endif //_POSITIONS_H
