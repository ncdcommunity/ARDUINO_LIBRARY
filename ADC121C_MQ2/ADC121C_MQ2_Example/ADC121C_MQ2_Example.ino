/**************************************************************************/
/*
        Distributed with a free-will license.
        Use it any way you want, profit or free, provided it fits in the licenses of its associated works.
        ADC121C_MQ2
        This code is designed to work with the ADC121C_I2CGAS_MQ2 I2C Mini Module available from ControlEverything.com.
        https://shop.controleverything.com/products/propane-butane-methane-alcohol-gas-sensor
        NT
*/
/**************************************************************************/

#include <Wire.h>
#include <ADC121C_MQ2.h>

ADC121C_MQ2 mq2;


void setup(void) 
{
    Serial.begin(9600);

    // The address can be changed making the option of connecting multiple devices
    mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_DEFAULT_ADDRESS);         // 0x50, 1010000
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_FLGND);        // 0x51, 1010001
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_FLVA);         // 0x52, 1010010
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_GNDFL);        // 0x54, 1010100
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_GNDGND);       // 0x55, 1010101
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_GNDVA);        // 0x56, 1010110
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_VAFL);         // 0x58, 1011000
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_VAGND);        // 0x59, 1011001
    // mq2.getAddr_ADC121C_MQ2(ADC121C_MQ2_ADDRESS_VAVA);         // 0x5A, 1011010


    // The Automatic Conversion Mode, Alert Hold, Alert Flag Enable,
    // Alert Pin Enable and ALERT pin Polarity
    // can be changed via the following functions

    mq2.setCycleTime(CYCLE_TIME_32);                    // Tconvert x 32, 27 ksps
    // mq2.setCycleTime(AUTOMATIC_MODE_DISABLED);       // Automatic Mode Disabled, 0 ksps
    // mq2.setCycleTime(CYCLE_TIME_64);                 // Tconvert x 64, 13.5 ksps
    // mq2.setCycleTime(CYCLE_TIME_128);                // Tconvert x 128, 6.7 ksps
    // mq2.setCycleTime(CYCLE_TIME_256);                // Tconvert x 256, 3.4 ksps
    // mq2.setCycleTime(CYCLE_TIME_512);                // Tconvert x 512, 1.7 ksps
    // mq2.setCycleTime(CYCLE_TIME_1024);               // Tconvert x 1024, 0.9 ksps
    // mq2.setCycleTime(CYCLE_TIME_2048);               // Tconvert x 2048, 0.4 ksps

    mq2.setAlertHold(ALERT_HOLD_CLEAR);                 // Alerts will self-clear
    // mq2.setAlertHold(ALERT_HOLD_NOCLEAR);            // Alerts will not self-clear

    mq2.setAlertFlag(ALERT_FLAG_DISABLE);               // Disables alert status bit in the Conversion Result register
    // mq2.setAlertFlag(ALERT_FLAG_ENABLE);             // Enables alert status bit in the Conversion Result register

    mq2.setAlertPin(ALERT_PIN_DISABLE);                 // Disables the ALERT output pin
    // mq2.setAlertPin(ALERT_PIN_ENABLE);               // Enables the ALERT output pin

    mq2.setPolarity(POLARITY_LOW);                      // Sets the ALERT pin to active low
    // mq2.setPolarity(POLARITY_HIGH);                  // Sets the ALERT pin to active high

    mq2.begin();

    // Calibrate your Sensor to Obtain Precise Value
    mq2.Calibration();
    // We Deduce Resistance of the Sensor in Clean Air (Ro) in the Serial Port
    // Serial.print("Ro = ");
    // Serial.println(mq2.Measure_Ro());
}

void loop(void)
{
    byte error;
    int8_t address;

    address = mq2.ADC121C_MQ2_i2cAddress;
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
    if (error == 0)
    {
        uint16_t result;

        Serial.println("Getting Gas Readings from ADC121C_MQ2");
        Serial.println(" ");
        // Serial.print("Rs/Ro Ratio: ");
        // Serial.println(mq2.Measure_RatioRsRo());
        // The Graph at fig of datasheet (sensitivity characteristics of the MQ-2)
        // seems a power function y = a*x^b
        // so ppm = a*(Rs/Ro)^b
        // Using Power Regression, you can obtain Slope (a), and Logarithmic Value of ppm (b) for LPG, Methane (CH4), and Hydrgen (H2)!
        // From the graph, we get points for LPG: (200. 1.7), (10000, 0.18)
        // From the graph, we get points for CH4: (200, 3.1), (10000, 0.71)
        // From the graph, we get points for H2: (200, 2.1), (10000, 0.33)

        Serial.println(mq2.Measure_RatioRsRo());
        Serial.print("Concentration  of LPG is: ");
        float ppmLPG = mq2.Measure_LPG(-0.57, 2.3);
        Serial.print(ppmLPG, 3);
        Serial.println(" ppm ");
        delay(500);
        Serial.print("Concentration of Methane is: ");
        float ppmCH4 = mq2.Measure_Methane(-0.37, 2.3);
        Serial.print(ppmCH4, 3);
        Serial.println(" ppm ");
        delay(500);
        Serial.print("Concentration of Hydrogen is: ");
        float ppmH2 = mq2.Measure_Hydrogen(-0.47, 2.3);
        Serial.print(ppmH2, 3);
        Serial.println(" ppm ");
        Serial.println(" ");
        Serial.println("        ***************************        ");
        Serial.println(" ");
    }
    else
    {
        Serial.println("ADC121C_MQ2 Disconnected!");
        Serial.println(" ");
        Serial.println("        ************        ");
        Serial.println(" ");
    }

    delay(1000);
}
