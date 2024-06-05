#ifndef EASYCOMMII_H
#define EASYCOMMII_H

#include <Arduino.h>
#include <BluetoothSerial.h>
#include <AccelStepper.h>

class EasyCommII {
public:
    EasyCommII(AccelStepper& azStepper, AccelStepper& elStepper, BluetoothSerial& serialBT, int azEnablePin, int elEnablePin);
    void parseCommand(const String& command);

private:
    void setPos(float az, float el);
    void getPos();
    void stop();
    void enableMotors();
    void disableMotors();

    AccelStepper& azStepper;
    AccelStepper& elStepper;
    BluetoothSerial& serialBT;
    int azEnablePin;
    int elEnablePin;
};

#endif
