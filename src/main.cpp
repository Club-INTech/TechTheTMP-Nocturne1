#include "Arduino.h"
#include "DynamixelUtils.h"
#include "DynamixelManager.h"
#include "XL430.h"
#include <vector>

#define _GLIBCXX_USE_C99 1

static DynamixelManager* manager = new DynamixelManager(&Serial1);
static XL430* motor1 = new XL430(1,*manager);
static XL430* motor2 = new XL430(2,*manager);
static XL430* motor3 = new XL430(3,*manager);

static std::vector<XL430*> motors{motor1,motor2,motor3};

void setup()
{
    pinMode(13,OUTPUT);

    //delay(10000);
    Serial.begin(115200);
    Serial.println("Init");

    digitalWrite(13,HIGH);

    Serial.println(motor1->toggleTorque(true));
    motor2->toggleTorque(true);
    motor3->toggleTorque(true);

    Serial.println("Ready :)");
    delay(1000);

    digitalWrite(13,LOW);
}

void setpos(float firstMotor, float secondMotor, float thirdMotor)
{
    Serial.println(motors.at(0)->setGoalAngle(firstMotor));
    delay(50);
    Serial.println(motors.at(1)->setGoalAngle(secondMotor));
    delay(50);
    Serial.println(motors.at(2)->setGoalAngle(thirdMotor));
}

void loop()
{
    static float angle = 0;
    static float oldAngle = 0;

    int id = 1;

    if(Serial.available())
    {

        oldAngle = angle;
        String input = Serial.readString(100).toLowerCase();
        digitalWrite(13, HIGH);

        if(input == "droit")
        {
            setpos(180, 180, 180);
            Serial.println("ok");
        }
        else if(input == "accel")
        {
            setpos(180, 225, 135);
            Serial.println("ok");
        }
        else if(input == "distributeur")
        {
            setpos(180, 315, 225);
            Serial.println("ok");
        }
        else if(input == "stockage")
        {
            setpos(225, 180, 225+90);
            Serial.println("ok");
        }
        else if(input == "sol")
        {
            setpos(225-90, 270, 225-90);
            Serial.println("ok");
        }
        else if(input == "toff")
        {
            for(unsigned int i = 0;i<3;i++) {
                motors.at(i)->toggleTorque(false);
            }
            Serial.println("ok");
        }
        else if(input == "ton")
        {
            for(unsigned int i = 0;i<3;i++) {
                motors.at(i)->toggleTorque(true);
            }
            Serial.println("ok");
        }
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
        else
        {
           Serial.printf("Commande non reconnue '%s'\n", input);
        }
    }

    delay(100);
    digitalWrite(13, LOW);
}

/*
DynamixelPacket* XL430::makeReadPacket(DynamixelAccessData accessData)
{
    uint8_t packetSize = dynamixelV2::minPacketLenght+2;
    unsigned char* packet = new unsigned char[packetSize];
    unsigned int position = 0;
    // HEADER
    for(unsigned char headerPart : v2Header) {
        packet[position++] = headerPart;
    }

    // Packet ID
    packet[position++] = motorData.motorID;
    motorID = motorData.motorID;

    // Instruction Length: 4(params) +3
    packet[position++] = 7;
    packet[position++] = 0;

    // Instruction
    packet[position++] = dynamixelV2::readInstruction;
    // Params: Start, Length
    packet[position++] = accessData.address[0];
    packet[position++] = accessData.address[1];

    packet[position++] = accessData.length & 0xFF;
    packet[position++] = (accessData.length >> 8) & 0xFF;

    unsigned short crc = crc_compute(packet, packetSize-2);
    packet[position++] = crc & 0xFF;
    packet[position++] = (crc >> 8) & 0xFF;

    return(new DynamixelPacket(packet, packetSize, 11+accessData.length));
}

 */