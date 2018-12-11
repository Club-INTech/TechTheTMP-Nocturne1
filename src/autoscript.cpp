#include "autoscript.h"
#include "robot.h"

void autoStepLoop(int side=1) {
    setpos(positionDroit, side);
    // Prendre au sol
    suck(side);
    //setpos(positionIntermediaire);
    setpos(positionSol, side);
    // Monter jusqu'à la pile et le ranger
    setpos(positionIntermediaire, side);
    setpos(positionStockage, side);
    unsuck(side);
    lowerElevator(1, side);

    // A LA MAIN: Mettre devant distrib !!
    digitalWrite(13, HIGH);
    delay(5000); // 5 secondes
    digitalWrite(13, LOW);

    // Prendre 1 palet dans distrib
    moveFromStorageToDispenser(side);
    suck(side);
    // On le stocke
    moveFromDispenserToStorage(side);
    unsuck(side);
    lowerElevator(1, side);

    // A LA MAIN: Mettre devant accelérateur !!
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);

    // Remonter et ventouser
    raiseElevator(1, side);
    suck(side);

    // Mettre le palet pour le poser dans l'accélérateur
    setpos(positionIntermediaire, side);
    setpos(positionAccelerateur, side);
    unsuck(side);

    // On remonte l'ascenseur et on prend le palet
    setpos(positionIntermediaire, side);
    setpos(positionStockage, side);
    raiseElevator(1, side);
    suck(side);
    setpos(positionIntermediaire, side);
    setpos(positionAccelerateur, side);

    // A LA MAIN: Décaler le robot sur le côté (pour pousser les palets dans l'accélérateur)
    digitalWrite(13, HIGH);
    delay(5000);
    digitalWrite(13, LOW);
    unsuck(side);
}