#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include "XL430.h"
#include "SyncWrite.h"
#include "constantes.h"
#include <vector>

#ifndef _ROBOT_H
#define _ROBOT_H

// Liste des moteurs du bras 1
static DynamixelManager* manager = new DynamixelManager(&Serial1, &DebugSerial);
static XL430* motor1 = new XL430(1,*manager);
static XL430* motor2 = new XL430(2,*manager);
static XL430* motor3 = new XL430(3,*manager);
// Liste des moteurs du bras 2
static XL430* motor4 = new XL430(4,*manager);
static XL430* motor5 = new XL430(5,*manager);
static XL430* motor6 = new XL430(6,*manager);

static std::vector<XL430*> motors{motor1,motor2,motor3,motor4,motor5,motor6};


// Les deux champs ci-dessous ne considèrent que 3 servo moteurs car un seul bras est utilisé à la fois
static char* syncAngles = new char[XL430::xl430GoalAngle.length*3];
static SyncWrite* syncWriteData = new SyncWrite(*manager, 3, (uint16_t ) (XL430::xl430GoalAngle.address[0] | (XL430::xl430GoalAngle.address[1] << 8)), XL430::xl430GoalAngle.length);

// Mets à jour la position du bras (positions, côté: 1 ou 2)
void setpos(float*, int);

// Actives l'électrovanne: coupe le flux d'air (côté: 1 ou 2)
void unsuck(int);

// Désactives l'électrovanne: laisse passer le flux d'air (côté: 1 ou 2)
void suck(int);

// Montes l'ascenseur (quantité, côté)
void raiseElevator(unsigned int, int);

// Baisses l'ascenseur (quantité, côté)
void lowerElevator(unsigned int, int);

// Fais en sorte que le bras suive un mouvement (positions, nombre de positions, côté)
void makeArmFollowSteps(float*[], unsigned int, int);

// Déplaces le bras dans la position de stockage vers distributeur (côté)
void moveFromStorageToDispenser(int);

// Déplaces le bras dans la position de distributeur vers stockage (côté)
void moveFromDispenserToStorage(int);

#endif //_ROBOT_H
