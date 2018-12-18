#include "robot.h"
#include "commande_serial.h"

int side = 1;

void executeFromSerial() {
    if(DebugSerial.available())
    {
        String input = DebugSerial.readString(100).toLowerCase(); // lit une commande
        digitalWrite(13, HIGH);

        delay(2000);

        // Commandes du bras
        if(input == "droit")
        {
            setpos(positionDroit, side);
            DebugSerial.println("ok");
        }
        else if(input == "accel")
        {
            setpos(positionAccelerateur, side);
            DebugSerial.println("ok");
        }
        else if(input == "distributeur")
        {
            setpos(positionDistributeur, side);
            DebugSerial.println("ok");
        }
        else if(input == "stockage")
        {
            setpos(positionStockage, side);
            DebugSerial.println("ok");
        }
        else if(input == "sol")
        {
            setpos(positionSol, side);
            DebugSerial.println("ok");
        }
        else if(input == "posinter")
        {
            setpos(positionIntermediaire, side);
            DebugSerial.println("ok");
        }

            // Toggle du couple fourni par les moteurs
        else if(input == "toff")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i+(side-1)*3)->toggleTorque(false);
            }
            DebugSerial.println("ok");
        }
        else if(input == "ton")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i+(side-1)*3)->toggleTorque(true);
            }
            DebugSerial.println("ok");
        }
        else if(input == "stock2distrib")
        {
            moveFromStorageToDispenser(side);
        }
        else if(input == "distrib2stock")
        {
            moveFromDispenserToStorage(side);
        }
        else if(input.startsWith("couple"))
        {
            uint8_t motorID = (uint8_t) input.substring(6).toInt();
            float torque;
            motors.at(motorID-1)->getCurrentTorque(torque);
            DebugSerial.printf("Couple (moteur %i) = %f\n", motorID, torque);
        }
        else if(input == "testforce")
        {
            float cb;
            motor2->getCurrentTorque(cb);
            cb *= 840.0; // N.mm
            float theta1;
            motor1->getCurrentAngle(theta1);
            float theta2;
            motor2->getCurrentAngle(theta2);
            float theta = theta1-90.0f + theta2 - 90.0f;
            double cos0 = cos(theta*3.14/180);
            double sin0 = sin(theta*3.14/180);
            double force = (cb - 50-72-1.6*82*cos0)/(-82*sin0);
            DebugSerial.printf("Theta = %f\n", theta);
            DebugSerial.printf("Force = %f\n", force);
        }

            // Donne les angles des moteurs à un moment donné
        else if(input == "output")
        {
            DebugSerial.println("--------------------------------");
            float angles[6];
            for(unsigned int i = 0;i<6;i++) {
                motors.at(i)->getCurrentAngle(angles[i]);
                DebugSerial.printf("Motor %i: %f°\n", (i+1), angles[i]);
            }
            DebugSerial.println("--------------------------------");
            DebugSerial.println("ok");
        }

            // Commande de l'ascenseur
        else if(input.startsWith("asc"))
        {
            int nbPas = input.substring(3).toInt();
            if(nbPas < 0) {
                lowerElevator((unsigned int)(-nbPas), side);
            } else {
                raiseElevator((unsigned int)nbPas, side);
            }
        }
        else if(input.startsWith("setside"))
        {
            int newSide = input.substring(7).toInt();
            if(newSide == 1 || newSide == 2) {
                side = newSide;
                DebugSerial.println("ok c:");
            } else {
                DebugSerial.println("Pas un côté correct >:c");
            }
        }

            // Commande de la pompe
        else if(input == "pompeon")
        {
            if(side == 1) {
                digitalWrite(VALVE_PIN_1, LOW);
            } else if(side == 2) {
                digitalWrite(VALVE_PIN_2, LOW);
            }
        }
        else if(input == "pompeoff")
        {
            if(side == 1) {
                digitalWrite(VALVE_PIN_1, HIGH);
            } else if(side == 2) {
                digitalWrite(VALVE_PIN_2, HIGH);
            }
        }
        else
        {
            DebugSerial.println("Commande non reconnue " + input);
        }
    }
}