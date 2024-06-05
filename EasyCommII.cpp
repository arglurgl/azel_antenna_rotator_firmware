#include "EasyCommII.h"

#define AZ_DEG_PER_STEP 43 // (deg*100)
#define EL_DEG_PER_STEP 6  // (deg*100)

EasyCommII::EasyCommII(AccelStepper& azStepper, AccelStepper& elStepper, BluetoothSerial& serialBT, int azEnablePin, int elEnablePin)
    : azStepper(azStepper), elStepper(elStepper), serialBT(serialBT), azEnablePin(azEnablePin), elEnablePin(elEnablePin) {}

void EasyCommII::parseCommand(const String& command) {
    Serial.print("Parsing command: ");
    Serial.println(command);

    // Trim command to remove any leading/trailing whitespace
    String trimmedCommand = command;
    trimmedCommand.trim();

    if (trimmedCommand.startsWith("P")) {
        // Format: P AZ EL
        int firstSpace = trimmedCommand.indexOf(' ');
        int secondSpace = trimmedCommand.indexOf(' ', firstSpace + 1);
        
        if (firstSpace == -1 || secondSpace == -1) {
            serialBT.println("ERR Invalid Command Format");
            return;
        }

        float az = trimmedCommand.substring(firstSpace + 1, secondSpace).toFloat();
        float el = trimmedCommand.substring(secondSpace + 1).toFloat();
        Serial.print("Set position command received. AZ: ");
        Serial.print(az);
        Serial.print(", EL: ");
        Serial.println(el);
        setPos(az, el);
    } else if (trimmedCommand == "p" || trimmedCommand == "AZ EL") {
        // Format: p or AZ EL
        Serial.println("Get position command received.");
        getPos();
    } else if (trimmedCommand == "S" || trimmedCommand.startsWith("SA SE")) {
        // Format: S or SA SE
        Serial.println("Stop command received.");
        stop();
    } else if (trimmedCommand.startsWith("AZ")) {
        // Format: AZ0.0 EL0.0
        int elIndex = trimmedCommand.indexOf(" EL");
        if (elIndex == -1) {
            serialBT.println("ERR Invalid Command Format");
            return;
        }

        float az = trimmedCommand.substring(2, elIndex).toFloat();
        float el = trimmedCommand.substring(elIndex + 3).toFloat();
        Serial.print("AZ/EL command received. AZ: ");
        Serial.print(az);
        Serial.print(", EL: ");
        Serial.println(el);
        setPos(az, el);
    } else {
        Serial.println("Unknown command received.");
        serialBT.println("ERR Unknown Command");
    }
}

void EasyCommII::setPos(float az, float el) {
    enableMotors();

    azStepper.setMaxSpeed(1000); // Set maximum speed for azStepper
    azStepper.setAcceleration(500); // Set acceleration for azStepper
    elStepper.setMaxSpeed(1000); // Set maximum speed for elStepper
    elStepper.setAcceleration(500); // Set acceleration for elStepper

    long azSteps = az * 100 / AZ_DEG_PER_STEP;
    long elSteps = el * 100 / EL_DEG_PER_STEP;
    azStepper.moveTo(azSteps);
    elStepper.moveTo(elSteps);

    Serial.print("Moving to AZ steps: ");
    Serial.print(azSteps);
    Serial.print(", EL steps: ");
    Serial.println(elSteps);

    serialBT.println("OK");
}

void EasyCommII::getPos() {
    float az = azStepper.currentPosition() * AZ_DEG_PER_STEP / 100.0;
    float el = elStepper.currentPosition() * EL_DEG_PER_STEP / 100.0;

    Serial.print("Current position AZ: ");
    Serial.print(az);
    Serial.print(", EL: ");
    Serial.println(el);

    serialBT.print("AZ ");
    serialBT.print(az, 2);
    serialBT.print(" EL ");
    serialBT.println(el, 2);
}

void EasyCommII::stop() {
    azStepper.stop();
    elStepper.stop();
    disableMotors();

    Serial.println("Motors stopped.");
    serialBT.println("OK");
}

void EasyCommII::enableMotors() {
    digitalWrite(azEnablePin, LOW); // Assuming LOW enables the motor drivers
    digitalWrite(elEnablePin, LOW);
    Serial.println("Motors enabled.");
}

void EasyCommII::disableMotors() {
    digitalWrite(azEnablePin, HIGH); // Assuming HIGH disables the motor drivers
    digitalWrite(elEnablePin, HIGH);
    Serial.println("Motors disabled.");
}
