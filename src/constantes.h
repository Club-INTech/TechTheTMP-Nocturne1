#include "Arduino.h"
#ifndef _POSITIONS_H
#define _POSITIONS_H
// Définition des différentes positions
static float positionDroit[3] = {180.0f,180.0f,180.0f};
static float positionAccelerateur[3] = {186.0f, 55.0f, 141.0f};
static float positionDistributeur[3] = {234.0f, 56.0f, 93.0f};
static float positionStockage[3] = {/*Valeur de test:*/184.0f/*187.0f*/, 268.0f, 83.0f};
static float positionSol[3] = { /*Valeur de test:*/ 278.0f /*272.0f*/, 97.0f, 189.0f};
static float positionIntermediaire[3] = {208.0f, 265.0f, 88.0f};

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
