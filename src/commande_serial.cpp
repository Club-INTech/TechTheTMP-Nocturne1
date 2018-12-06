#include "robot.h"
#include "commande_serial.h"
void executeFromSerial() {
    if(DebugSerial.available())
    {
        String input = DebugSerial.readString(100).toLowerCase(); // lit une commande
        digitalWrite(13, HIGH);

        // Commandes du bras
        if(input == "droit")
        {
            setpos(positionDroit, 1);
            DebugSerial.println("ok");
        }
        else if(input == "accel")
        {
            setpos(positionAccelerateur, 1);
            DebugSerial.println("ok");
        }
        else if(input == "distributeur")
        {
            setpos(positionDistributeur, 1);
            DebugSerial.println("ok");
        }
        else if(input == "stockage")
        {
            setpos(positionStockage, 1);
            DebugSerial.println("ok");
        }
        else if(input == "sol")
        {
            setpos(positionSol, 1);
            DebugSerial.println("ok");
        }
        else if(input == "posinter")
        {
            setpos(positionIntermediaire, 1);
            DebugSerial.println("ok");
        }

            // Toggle du couple fourni par les moteurs
        else if(input == "toff")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i)->toggleTorque(false);
            }
            DebugSerial.println("ok");
        }
        else if(input == "ton")
        {
            for(unsigned int i = 0;i<3;i++)
            {
                motors.at(i)->toggleTorque(true);
            }
            DebugSerial.println("ok");
        }

            // Donne les angles des moteurs à un moment donné
        else if(input == "output")
        {
            DebugSerial.println("--------------------------------");
            DebugSerial.printf("|  Motor 1 | Motor 2 | Motor 3 |\n");
            float angle1 = 0.f;
            float angle2 = 0.f;
            float angle3 = 0.f;
            motor1->getCurrentAngle(angle1);
            motor2->getCurrentAngle(angle2);
            motor3->getCurrentAngle(angle3);
            DebugSerial.printf("|  %f3 | %f3 | %f3 |", angle1, angle2, angle3);
            DebugSerial.println("--------------------------------");
            DebugSerial.println("ok");
        }

            // Commande de l'ascenseur
        else if(input.startsWith("asc"))
        {
            int nbPas = input.substring(3).toInt();
            if(nbPas < 0) {
                lowerElevator((unsigned int)(-nbPas), 1);
            } else {
                raiseElevator((unsigned int)nbPas, 1);
            }
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
            DebugSerial.println("Commande non reconnue " + input);
        }
    }
}