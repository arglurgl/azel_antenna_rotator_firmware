#include <BluetoothSerial.h>
#include <AccelStepper.h>
#include "EasyCommII.h"

#define AZ_STEP_PIN 33
#define AZ_DIR_PIN 32
#define AZ_ENABLE_PIN 12
#define EL_STEP_PIN 26
#define EL_DIR_PIN 25
#define EL_ENABLE_PIN 14
#define EL_HOME_PIN_ANL 18 // Update as needed

BluetoothSerial SerialBT;
AccelStepper azStepper(AccelStepper::DRIVER, AZ_STEP_PIN, AZ_DIR_PIN);
AccelStepper elStepper(AccelStepper::DRIVER, EL_STEP_PIN, EL_DIR_PIN);
EasyCommII easycomm(azStepper, elStepper, SerialBT, AZ_ENABLE_PIN, EL_ENABLE_PIN);

void setup() {
    Serial.begin(115200);
    if (!SerialBT.begin("ESP32_Rotor")) {
        Serial.println("An error occurred initializing Bluetooth");
    } else {
        Serial.println("Bluetooth initialized");
    }
    
    azStepper.setPinsInverted(true, true, true);
    elStepper.setPinsInverted(true, true, true);
    azStepper.setEnablePin(AZ_ENABLE_PIN);
    elStepper.setEnablePin(EL_ENABLE_PIN);
    pinMode(AZ_ENABLE_PIN, OUTPUT);
    pinMode(EL_ENABLE_PIN, OUTPUT);
    azStepper.disableOutputs();
    elStepper.disableOutputs();
    
    azStepper.setMaxSpeed(1000); // Set maximum speed for azStepper
    azStepper.setAcceleration(500); // Set acceleration for azStepper
    elStepper.setMaxSpeed(1000); // Set maximum speed for elStepper
    elStepper.setAcceleration(500); // Set acceleration for elStepper
    
    Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
    if (SerialBT.available()) {
        String command = SerialBT.readStringUntil('\n');
        Serial.print("Received command: ");
        Serial.println(command);
        easycomm.parseCommand(command);
    }

    azStepper.run();
    elStepper.run();
}
