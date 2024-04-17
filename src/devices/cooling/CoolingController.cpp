/*
 * CoolingController.h - implements a simple unified way to control fans, coolant pumps, heaters, etc that
 might be used in a car. 
 
 Copyright (c) 2021 Collin Kidder

 Permission is hereby granted, free of charge, to any person obtaining
 a copy of this software and associated documentation files (the
 "Software"), to deal in the Software without restriction, including
 without limitation the rights to use, copy, modify, merge, publish,
 distribute, sublicense, and/or sell copies of the Software, and to
 permit persons to whom the Software is furnished to do so, subject to
 the following conditions:

 The above copyright notice and this permission notice shall be included
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "CoolingController.h"
#include "../../DeviceManager.h"

/*
 * Constructor
 */
CoolingController::CoolingController() : Device() {    
    commonName = "Cooling Controller";
    shortName = "CoolingController"; 

    isAccumulatorPumpOn = false;
    isMotorPumpOn = false;
    isAccumulatorFanOn = false;
    isMotorFanOn = false;

}

void CoolingController::earlyInit()
{
    prefsHandler = new PrefHandler(COOLCONTROL);
}

/*
 * Setup the device.
 */
void CoolingController::setup() {
    crashHandler.addBreadcrumb(ENCODE_BREAD("COOLING") + 0);
    tickHandler.detach(this); // unregister from TickHandler first

    Logger::info("add device: CoolingController (id: %X, %X)", COOLCONTROL, this);

    loadConfiguration();

    Device::setup(); //call base class

    CoolingControllerConfiguration *config = (CoolingControllerConfiguration *)getConfiguration();

    StatusEntry stat;// Values that should not be changed to the outside world


    stat = {"COOLING_AccumulatorFanOn", &isAccumulatorFanOn, CFG_ENTRY_VAR_TYPE::BYTE, 0, this};
    deviceManager.addStatusEntry(stat);
    stat = {"COOLING_AccumulatorPumpOn", &isAccumulatorPumpOn, CFG_ENTRY_VAR_TYPE::BYTE, 0, this};
    deviceManager.addStatusEntry(stat);
    stat = {"COOLING_MotorPummpOn", &isMotorPumpOn, CFG_ENTRY_VAR_TYPE::BYTE, 0, this};
    deviceManager.addStatusEntry(stat);    
    stat = {"COOLING_MotorFanOn", &isMotorFanOn, CFG_ENTRY_VAR_TYPE::BYTE, 0, this};
    deviceManager.addStatusEntry(stat);    

    tickHandler.attach(this, CFG_TICK_INTERVAL_COOLCONTROL);
}

/*
 * Process a timer event. This is where you should be doing checks and updates. 
 */
void CoolingController::handleTick() {
    // setDigitalOutputPWM
    crashHandler.addBreadcrumb(ENCODE_BREAD("COOLING") + 1);
    Device::handleTick(); // Call parent which controls the workflow
    Logger::avalanche("Cooling Controller Tick Handler");

    CoolingControllerConfiguration *config = (CoolingControllerConfiguration *) getConfiguration();

    // Retrieve the temperature of the motor and the accumulator
    uint16_t motorTemperatureAnalogReading = systemIO.getAnalogIn(config->motorTemperatureSensorPin);
    uint16_t accumulatorTemperatureAnalogReading = systemIO.getAnalogIn(config->accumulatorTemperatureSensorPin);
    
    Logger::info(COOLCONTROL, "Motor Temperature Anlog Reading: %d", motorTemperatureAnalogReading);
    Logger::info(COOLCONTROL, "Accumulator Temperature Anlog Reading: %d", accumulatorTemperatureAnalogReading);

    // Convert the analog input to temperature values using an equation from the manufacturer
    int RESISTORVALUE = 100000; // Change this to the resistor value on the temperature sensor

    double motorTemperature = (motorTemperatureAnalogReading * 5) / 1023;
    motorTemperature = (motorTemperature * RESISTORVALUE / 5) / ( 1 - motorTemperature / 5 );
    motorTemperature = -0.00388 * (motorTemperature) + 65.6;

    double accumulatorTemperature = (accumulatorTemperatureAnalogReading * 5) / 1023;
    accumulatorTemperature = (accumulatorTemperature * RESISTORVALUE / 5) / ( 1 - accumulatorTemperature / 5 );
    accumulatorTemperature = -0.00388 * (accumulatorTemperature) + 65.6;

    Logger::info(COOLCONTROL, "Motor Temperature: %d", motorTemperature);
    Logger::info(COOLCONTROL, "Accumulator Temperature: %d", accumulatorTemperature);    
}
 
/*
 * Return the device ID
 */
DeviceId CoolingController::getId() {
    return (COOLCONTROL);
}

DeviceType CoolingController::getType()
{
    return (DeviceType::DEVICE_CHARGER);
}

/*
 * Load the device configuration.
 * If possible values are read from EEPROM. If not, reasonable default values
 * are chosen and the configuration is overwritten in the EEPROM.
 */
void CoolingController::loadConfiguration() {
    CoolingControllerConfiguration *config = (CoolingControllerConfiguration *) getConfiguration();

    if (!config) { // as lowest sub-class make sure we have a config object
        config = new CoolingControllerConfiguration();
        Logger::debug("loading configuration in CoolingController");
        setConfiguration(config);
    }
    
    Device::loadConfiguration(); // call parent

    //TODO Change pin number to pin for input
    prefsHandler->read("motorTempeartureSensorPin", &config->motorTemperatureSensorPin, 0); // ANALOG0 PIN
    prefsHandler->read("accumulatorTempeartureSensorPin", &config->accumulatorTemperatureSensorPin, 1); // ANALOG1 PIN
    prefsHandler->read("fanAccumulatorPin", &config->fanAccumulatorPin, 255);
    prefsHandler->read("fanMotorPin", &config->fanMotorPin, 255);
    prefsHandler->read("waterAccumulatorPin", &config->waterAccumulatorPin, 255);
    prefsHandler->read("waterMotorPin", &config->waterMotorPin, 255);

    prefsHandler->read("motorPumpOnTemperature", &config->motorPumpOnTemperature, 0);
    prefsHandler->read("motorPumpOffTempearture", &config->motorPumpOffTempearture, 0);
    prefsHandler->read("accumulatorPumpOnTemperature", &config->accumulatorPumpOnTemperature, 0);
    prefsHandler->read("accumulatorPumpOffTemperature", &config->accumulatorPumpOffTemperature, 0);
    
    prefsHandler->read("motorFanOnTemperature", &config->motorFanOnTemperature, 0);
    prefsHandler->read("motorFanOffTemperature", &config->motorFanOffTemperature, 0);
    prefsHandler->read("accumulatorFanOnTemperature", &config->accumulatorFanOnTemperature, 0);
    prefsHandler->read("accumulatorFanOffTemperature", &config->accumulatorFanOffTemperature, 0);
}
/*
 * Store the current configuration to EEPROM
 */
void CoolingController::saveConfiguration() {
    CoolingControllerConfiguration *config = (CoolingControllerConfiguration *) getConfiguration();

    Device::saveConfiguration(); // call parent

    //TODO Change pin number to pin for input
    prefsHandler->write("motorTempeartureSensorPin", config->motorTemperatureSensorPin);
    prefsHandler->write("accumulatorTempeartureSensorPin", config->accumulatorTemperatureSensorPin);
    prefsHandler->write("fanAccumulatorPin", config->fanAccumulatorPin);
    prefsHandler->write("fanMotorPin", config->fanMotorPin);
    prefsHandler->write("waterAccumulatorPin", config->waterAccumulatorPin);
    prefsHandler->write("waterMotorPin", config->waterMotorPin);

    prefsHandler->write("motorPumpOnTemperature", config->motorPumpOnTemperature);
    prefsHandler->write("motorPumpOffTempearture", config->motorPumpOffTempearture);
    prefsHandler->write("accumulatorPumpOnTemperature", config->accumulatorPumpOnTemperature);
    prefsHandler->write("accumulatorPumpOffTemperature", config->accumulatorPumpOffTemperature);
    
    prefsHandler->write("motorFanOnTemperature", config->motorFanOnTemperature);
    prefsHandler->write("motorFanOffTemperature", config->motorFanOffTemperature);
    prefsHandler->write("accumulatorFanOnTemperature", config->accumulatorFanOnTemperature);
    prefsHandler->write("accumulatorFanOffTemperature", config->accumulatorFanOffTemperature);

    prefsHandler->saveChecksum();
    prefsHandler->forceCacheWrite();
}

CoolingController coolingController;
