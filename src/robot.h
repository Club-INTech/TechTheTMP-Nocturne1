#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include "XL430.h"
#include "SyncWrite.h"
#include "constantes.h"
#include <vector>

#ifndef _ROBOT_H
#define _ROBOT_H

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
#endif //_ROBOT_H
