#include <BluetoothSerial.h>
#include <AccelStepper.h>
#include "EasyCommII.h"

#define AZ_STEP_PIN 33
#define AZ_DIR_PIN 32
#define AZ_ENABLE_PIN 12
#define EL_STEP_PIN 26
#define EL_DIR_PIN 25
#define EL_ENABLE_PIN 14
#define EL_HOME_PIN 23 // Update as needed

#define EL_HOMING_BACKOFF_STEPS 1000//how many steps to move away from homing switch
#define EL_HOMING_FORWARD_MAXMOVE 100000 //how many steps to move towards homing switch before giving up
#define SPEED 1000
#define HOMING_SPEED 500
#define ACCELERATION 500

#define BT_SERIAL_TIMEOUT 100

BluetoothSerial SerialBT;
AccelStepper azStepper(AccelStepper::DRIVER, AZ_STEP_PIN, AZ_DIR_PIN);
AccelStepper elStepper(AccelStepper::DRIVER, EL_STEP_PIN, EL_DIR_PIN);
EasyCommII easycomm(azStepper, elStepper, SerialBT, AZ_ENABLE_PIN, EL_ENABLE_PIN);

bool home_elevation();

void setup() {
    Serial.begin(115200);
    if (!SerialBT.begin("ESP32_Rotor")) {
        Serial.println("An error occurred initializing Bluetooth");
    } else {
        Serial.println("Bluetooth initialized");
    }
    
    azStepper.setPinsInverted(true, true, true);
    elStepper.setPinsInverted(false, true, true);
    azStepper.setEnablePin(AZ_ENABLE_PIN);
    elStepper.setEnablePin(EL_ENABLE_PIN);
    pinMode(AZ_ENABLE_PIN, OUTPUT);
    pinMode(EL_ENABLE_PIN, OUTPUT);
    azStepper.disableOutputs();
    elStepper.disableOutputs();
    pinMode(EL_HOME_PIN, INPUT_PULLUP);
    
    azStepper.setMaxSpeed(SPEED); // Set maximum speed for azStepper
    azStepper.setAcceleration(ACCELERATION); // Set acceleration for azStepper
    elStepper.setMaxSpeed(SPEED); // Set maximum speed for elStepper
    elStepper.setAcceleration(ACCELERATION); // Set acceleration for elStepper
    
    Serial.println("Homing elevation");
    if (!home_elevation()){
        Serial.println("Error homing elevation, will not continue");
        while(1){};
    };
    Serial.println("Homing done");

    SerialBT.setTimeout(BT_SERIAL_TIMEOUT);

    Serial.println("The device started, now you can pair it with bluetooth!");
}

bool home_elevation(){
    elStepper.enableOutputs();
    elStepper.move(EL_HOMING_BACKOFF_STEPS);
    elStepper.runToPosition();   
    if (digitalRead(EL_HOME_PIN)){
        Serial.println("Error: EL home switch did not release. Check wiring.");
        return false;
    }
    elStepper.setMaxSpeed(HOMING_SPEED);
    elStepper.move(EL_HOMING_FORWARD_MAXMOVE*-1);
    while (!digitalRead(EL_HOME_PIN)){
        elStepper.run();
    }
    elStepper.setCurrentPosition(0); //sets position and immediately stops motor
    elStepper.setMaxSpeed(SPEED);
    return true;
}

void loop() {
    if (SerialBT.available()) {
        String command = SerialBT.readStringUntil('\n');  //TODO: use non-blocking approach
        Serial.print("Received command: ");
        Serial.println(command);
        easycomm.parseCommand(command);
    }

    azStepper.run();
    elStepper.run();
}
