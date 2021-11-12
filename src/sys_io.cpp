/*
 * sys_io.cpp
 *
 * Handles the low level details of system I/O
 *
Copyright (c) 2013 Collin Kidder, Michael Neuweiler, Charles Galpin

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

some portions based on code credited as:
Arduino Due ADC->DMA->USB 1MSPS
by stimmer

*/

#include "sys_io.h"
#include "devices/io/CANIODevice.h"
#include "i2c_driver_wire.h"

#undef HID_ENABLED

SystemIO::SystemIO()
{
    for (int i = 0; i < NUM_EXT_IO; i++)
    {
        extendedDigitalOut[i].device = NULL;
        extendedDigitalIn[i].device = NULL;
        extendedAnalogOut[i].device = NULL;
        extendedAnalogIn[i].device = NULL;
    }
    
    numDigIn = NUM_DIGITAL;
    numDigOut = NUM_OUTPUT;
    numAnaIn = NUM_ANALOG;
    numAnaOut = 0;
    pcaDigitalOutputCache = 0; //all outputs off by default
    adcMuxSelect = 0;
}

void SystemIO::setup_ADC_params()
{
    int i;
    
    sysPrefs->read("Adc0Gain", &adc_comp[0].gain, 1024);
    sysPrefs->read("Adc0Offset", &adc_comp[0].offset, 0);
    sysPrefs->read("Adc1Gain", &adc_comp[1].gain, 1024);
    sysPrefs->read("Adc1Offset", &adc_comp[1].offset, 0);
    sysPrefs->read("Adc2Gain", &adc_comp[2].gain, 1024);
    sysPrefs->read("Adc2Offset", &adc_comp[2].offset, 0);
    sysPrefs->read("Adc3Gain", &adc_comp[3].gain, 1024);
    sysPrefs->read("Adc3Offset", &adc_comp[3].offset, 0);
    sysPrefs->read("Adc4Gain", &adc_comp[4].gain, 1024);
    sysPrefs->read("Adc4Offset", &adc_comp[4].offset, 0);
    sysPrefs->read("Adc5Gain", &adc_comp[5].gain, 1024);
    sysPrefs->read("Adc5Offset", &adc_comp[5].offset, 0);
    sysPrefs->read("Adc6Gain", &adc_comp[6].gain, 1024);
    sysPrefs->read("Adc6Offset", &adc_comp[6].offset, 0);
    sysPrefs->read("Adc7Gain", &adc_comp[7].gain, 1024);
    sysPrefs->read("Adc7Offset", &adc_comp[7].offset, 0);

    for (int i = 0; i < 8; i++) 
        Logger::debug("ADC:%d GAIN: %d Offset: %d", i, adc_comp[i].gain, adc_comp[i].offset);
}

void SystemIO::setSystemType(SystemType systemType) {
    if (systemType >= GEVCU1 && systemType <= GEVCU6)
    {
        sysType = systemType;
        sysPrefs->write("SysType", (uint8_t)sysType);
    }
}

SystemType SystemIO::getSystemType() {
    return sysType;
}

void SystemIO::setup() {
    int i;

    sysPrefs->read("SysType", (uint8_t *) &sysType, 7);

    analogReadRes(12);

    setup_ADC_params();

    pinMode(9, INPUT); //these 4 are digital inputs not
    pinMode(40, INPUT); //connected to PCA chip. They're direct
    pinMode(41, INPUT);
    pinMode(42, INPUT);

    pinMode(2, OUTPUT); //PWM1 = ADC Select B
    pinMode(3, OUTPUT); //PWM0 = ADC Select A
    digitalWrite(2, LOW); //both off by default to select mux 0
    digitalWrite(3, LOW); 

    initDigitalMultiplexor(); //set I/O direction for all pins, polarity, etc.
}

void SystemIO::installExtendedIO(CANIODevice *device)
{
    bool found = false;
    int counter;
    
    //Logger::debug("Before adding extended IO counts are DI:%i DO:%i AI:%i AO:%i", numDigIn, numDigOut, numAnaIn, numAnaOut);
    //Logger::debug("Num Analog Inputs: %i", device->getAnalogInputCount());
    //Logger::debug("Num Analog Outputs: %i", device->getAnalogOutputCount());
    //Logger::debug("Num Digital Inputs: %i", device->getDigitalInputCount());
    //Logger::debug("Num Digital Outputs: %i", device->getDigitalOutputCount());
   
    if (device->getAnalogInputCount() > 0)
    {
        for (counter = 0; counter < NUM_EXT_IO; counter++)
        {
            if (extendedAnalogIn[counter].device == NULL)
            {
                for (int i = 0; i < device->getAnalogInputCount(); i++)
                {
                    if ((counter + i) == NUM_EXT_IO) break;
                    extendedAnalogIn[counter + i].device = device;
                    extendedAnalogIn[counter + i].localOffset = i;
                }
                break;
            }
        }
    }
    
    if (device->getAnalogOutputCount() > 0)
    {
        for (counter = 0; counter < NUM_EXT_IO; counter++)
        {
            if (extendedAnalogOut[counter].device == NULL)
            {
                for (int i = 0; i < device->getAnalogOutputCount(); i++)
                {
                    if ((counter + i) == NUM_EXT_IO) break;
                    extendedAnalogOut[counter + i].device = device;
                    extendedAnalogOut[counter + i].localOffset = i;
                }
                break;
            }
        }
    }

    if (device->getDigitalOutputCount() > 0)
    {
        for (counter = 0; counter < NUM_EXT_IO; counter++)
        {
            if (extendedDigitalOut[counter].device == NULL)
            {
                for (int i = 0; i < device->getDigitalOutputCount(); i++)
                {
                    if ((counter + i) == NUM_EXT_IO) break;
                    extendedDigitalOut[counter + i].device = device;
                    extendedDigitalOut[counter + i].localOffset = i;
                }
                break;
            }
        }
    }

    if (device->getDigitalInputCount() > 0)
    {
        for (counter = 0; counter < NUM_EXT_IO; counter++)
        {
            if (extendedDigitalIn[counter].device == NULL)
            {
                for (int i = 0; i < device->getDigitalInputCount(); i++)
                {
                    if ((counter + i) == NUM_EXT_IO) break;
                    extendedDigitalIn[counter + i].device = device;
                    extendedDigitalIn[counter + i].localOffset = i;
                }
                break;
            }
        }
    }
    
    int numDI = NUM_DIGITAL; 
    for (int i = 0; i < NUM_EXT_IO; i++)
    {
        if (extendedDigitalIn[i].device != NULL) numDI++;
        else break;
    }
    numDigIn = numDI;
    
    int numDO = NUM_OUTPUT;
    for (int i = 0; i < NUM_EXT_IO; i++)
    {
        if (extendedDigitalOut[i].device != NULL) numDO++;
        else break;
    }
    numDigOut = numDO;

    int numAI = NUM_ANALOG; 
    for (int i = 0; i < NUM_EXT_IO; i++)
    {
        if (extendedAnalogIn[i].device != NULL) numAI++;
        else break;
    }
    numAnaIn = numAI;

    int numAO = 0; //GEVCU has no real analog outputs - there are PWM but they're on the digital outputs
    for (int i = 0; i < NUM_EXT_IO; i++)
    {
        if (extendedDigitalIn[i].device != NULL) numAO++;
        else break;
    }
    numAnaOut = numAO;
    Logger::debug("After added extended IO the counts are DI:%i DO:%i AI:%i AO:%i", numDigIn, numDigOut, numAnaIn, numAnaOut);
}

int SystemIO::numDigitalInputs()
{
    return numDigIn;
}

int SystemIO::numDigitalOutputs()
{
    return numDigOut;
}

int SystemIO::numAnalogInputs()
{
    return numAnaIn;
}

int SystemIO::numAnalogOutputs()
{
    return numAnaOut;
}

int16_t SystemIO::_pGetAnalogRaw(uint8_t which)
{
    int32_t valu;

    if (which >= NUM_ANALOG)
    {
        return 0;
    }
        
    int neededMux = which % 4;
    if (neededMux != adcMuxSelect) //must change mux to read this
    {
        digitalWrite(2, (neededMux & 2) ? HIGH : LOW);
        digitalWrite(3, (neededMux & 1) ? HIGH : LOW);
        adcMuxSelect = neededMux;
        //the analog multiplexor input switch pins are on direct outputs from the teensy
        //and so will change very rapidly. The multiplexor also switches inputs in less than
        //1 microsecond. The inputs are all buffered with 1uF caps and so perhaps the slowest
        //part of switching is allowing the analog input pin on the teensy to settle
        //to the new value of the analog input. Even so, this should happen very rapidly.
        //Perhaps only 1us goes by between switching the mux and the new value appearing
        //but give it at least a few microseconds for everything to switch and the new value to
        //settle at the Teensy ADC pin.
        delayMicroseconds(5);
    }
    //Analog inputs 0-3 are always on ADC0, 4-7 are on ADC1
    if (which < 4) valu = analogRead(0);
    else valu = analogRead(1);
    return valu;
}


/*
get value of one of the analog inputs
*/
int16_t SystemIO::getAnalogIn(uint8_t which) {
    int base;
    int valu;

    if (which > numAnaIn)
    {
        return 0;
    }
        
    if (which < NUM_ANALOG)
    {
        valu = _pGetAnalogRaw(which);
        valu -= adc_comp[which].offset;
        valu = (valu * adc_comp[which].gain) / 1024;
        return valu;
    }
    else //the return makes this superfluous...
    {        
        //handle an extended I/O call
        CANIODevice *dev = extendedAnalogIn[which - NUM_ANALOG].device;
        if (dev) return dev->getAnalogInput(extendedAnalogIn[which - NUM_ANALOG].localOffset);
        return 0;
    }
    return 0; //if it falls through and nothing could provide the answer then return 0
}

//there really are no directly connected analog outputs but extended I/O devices might implement some
boolean SystemIO::setAnalogOut(uint8_t which, int32_t level)
{
    if (which >= numAnaOut) return false;
    CANIODevice *dev;
    dev = extendedAnalogOut[which].device;
    if (dev) dev->setAnalogOutput(extendedAnalogOut[which].localOffset, level);
    return true;   
}

int32_t SystemIO::getAnalogOut(uint8_t which)
{
    if (which >= numAnaOut) return 0;
    CANIODevice *dev;
    dev = extendedAnalogOut[which].device;
    if (dev) return dev->getAnalogOutput(extendedAnalogOut[which].localOffset);
    return 0;    
}

//get value of one of the 12 digital inputs (or more if extended I/O added more)
boolean SystemIO::getDigitalIn(uint8_t which) {
    if (which >= numDigIn) return false;
    
    if (which < NUM_DIGITAL) 
    {
        if (which < 8) return _pGetDigitalInput(which);
        else 
        {
            switch (which)
            {
            case 8:
                return !(digitalRead(40));
                break;
            case 9:
                return !(digitalRead(41));
                break;
            case 10:
                return !(digitalRead(42));
                break;
            case 11:
                return !(digitalRead(9));
                break;

            }
        }
    }
    else
    {
        CANIODevice *dev;
        dev = extendedDigitalIn[which - NUM_DIGITAL].device;
        if (dev) return dev->getDigitalInput(extendedDigitalIn[which - NUM_DIGITAL].localOffset);
    }
    return false;
}

//set output high or not
void SystemIO::setDigitalOutput(uint8_t which, boolean active) {
    if (which >= numDigOut) return;
    
    if (which < NUM_OUTPUT)
    {
        _pSetDigitalOutput(which, active);
    }
    else
    {
        CANIODevice *dev;
        dev = extendedDigitalOut[which - NUM_OUTPUT].device;
        if (dev) return dev->setDigitalOutput(extendedDigitalOut[which - NUM_OUTPUT].localOffset, active);
    }
}

//get current value of output state (high?)
boolean SystemIO::getDigitalOutput(uint8_t which) {
    if (which >= numDigOut) return false;
    
    if (which < NUM_OUTPUT)
    {
        return _pGetDigitalOutput(which);
    }
    else
    {
        CANIODevice *dev;
        dev = extendedDigitalOut[which - NUM_OUTPUT].device;
        if (dev) return dev->getDigitalOutput(extendedDigitalOut[which - NUM_OUTPUT].localOffset);
    }
    return false;
}

/*
 * adc is the adc port to calibrate, update if true will write the new value to EEPROM automatically
 */
bool SystemIO::calibrateADCOffset(int adc, bool update)
{
    int32_t accum = 0;
    
    if (adc < 0 || adc > 7) return false;
    
    for (int j = 0; j < 500; j++)
    {
        accum += _pGetAnalogRaw(adc);
        //normally one shouldn't call watchdog reset in multiple
        //places but this is a special case.
        //watchdogReset();
        delay(2);
    }
    accum /= 500;
    char paramName[20];
    snprintf(paramName, 20, "Adc%uOffset", adc);
    if (update) sysPrefs->write((const char *)paramName, (uint16_t)(accum));    
    Logger::console("ADC %i offset is now %i", adc, accum);
    return true;
}


//much like the above function but now we use the calculated offset and take readings, average them
//and figure out how to set the gain such that the average reading turns up to be the target value
bool SystemIO::calibrateADCGain(int adc, int32_t target, bool update)
{
    int32_t accum = 0;
    
    if (adc < 0 || adc > 7) return false;
    
    for (int j = 0; j < 500; j++)
    {
        accum += _pGetAnalogRaw(adc);

        //normally one shouldn't call watchdog reset in multiple
        //places but this is a special case.
        //watchdogReset();
        delay(2);
    }
    accum /= 500;
    Logger::console("Unprocessed accum: %i", accum);
    
    //now apply the proper offset we've got set.
    if (adc < 4) {
        accum /= 2048;
        accum -= adc_comp[adc].offset;
    }
    else {
        accum -= adc_comp[adc].offset * 32;
        accum >>= 3;
    }
    
    if ((target / accum) > 20) {
        Logger::console("Calibration not possible. Check your target value.");
        return false;
    }
    
    if (accum < 1000 && accum > -1000) {
        Logger::console("Readings are too low. Try applying more voltage/current");
        return false;
    }
    
    //1024 is one to one so all gains are multiplied by that much to bring them into fixed point math.
    //we've got a reading accum and a target. The rational gain is target/accum    
    adc_comp[adc].gain = (int16_t)((16384ull * target) / accum);
    char paramName[20];
    snprintf(paramName, 20, "Adc%uGain", adc);
    if (update) sysPrefs->write((const char *)paramName, adc_comp[adc].gain);
    Logger::console("Accum: %i    Target: %i", accum, target);
    Logger::console("ADC %i gain is now %i", adc, adc_comp[adc].gain);
    return true;
}

void SystemIO::initDigitalMultiplexor()
{
    //all of port 0 are outputs, all of port 1 are inputs
    //1 in a config bit means input, 0 = output
    Wire.begin();
    Wire.beginTransmission(PCA_ADDR);  // setup to write to PCA chip
    Wire.write(PCA_CFG_0);
    Wire.write(0); //all zeros means all outputs
    Wire.endTransmission();

    Wire.begin();
    Wire.beginTransmission(PCA_ADDR);  // setup to write to PCA chip
    Wire.write(PCA_CFG_1);
    Wire.write(0xFF); //all 1's means all inputs
    Wire.endTransmission();

    Wire.beginTransmission(PCA_ADDR);  // setup to write to PCA chip
    Wire.write(PCA_POLARITY_1);
    Wire.write(0xFF); //all inputs are active low so invert all those
    Wire.endTransmission();
    
}

int SystemIO::_pGetDigitalInput(int pin) //all inputs are on port 1
{
    if ( (pin < 0) || (pin > 7) ) return 0;
    Wire.beginTransmission(PCA_ADDR);
    Wire.write(PCA_READ_IN1);
    Wire.endTransmission();

    Wire.requestFrom(PCA_ADDR, 1); //get one byte
    if (Wire.available())
    {
        uint8_t c = Wire.read();
        c = (c >> pin) & 1;
        return c;
    }
    return 0; //fallback in case something messes up
}

void SystemIO::_pSetDigitalOutput(int pin, int state)
{
    if ( (pin < 0) || (pin > 7) ) return;

    uint8_t outputMask = ~(1<<pin);
    pcaDigitalOutputCache &= outputMask;
    if (state != 0) pcaDigitalOutputCache |= (1<<pin);

    Wire.beginTransmission(PCA_ADDR);
    Wire.write(PCA_WRITE_OUT0);
    Wire.write(pcaDigitalOutputCache);
    Wire.endTransmission();
}

int SystemIO::_pGetDigitalOutput(int pin)
{
    if ( (pin < 0) || (pin > 7) ) return 0;
    Wire.beginTransmission(PCA_ADDR);
    Wire.write(PCA_READ_IN0);
    Wire.endTransmission();

    Wire.requestFrom(PCA_ADDR, 1); //get one byte
    if (Wire.available())
    {
        uint8_t c = Wire.read();
        c = (c >> pin) & 1;
        return c;
    }
    return 0; //fallback in case something messes up
}

SystemIO systemIO;


