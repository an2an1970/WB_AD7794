/*
  Start_Read_Split.ino - Example of split start/read on a shared SPI bus.

  This file is part of the WB_AD7794 library.
*/

#include "WB_AD7794.h"
#include <SPI.h>

// Pin defines, change for your setup
#define AD7794_CS     10
#define OTHER_SPI_CS  8

AD7794 adc(AD7794_CS, 1000000, 2.50);

void setup() {
  Serial.begin(115200);

  pinMode(OTHER_SPI_CS, OUTPUT);
  digitalWrite(OTHER_SPI_CS, HIGH);

  adc.begin();
  adc.setUpdateRate(470); // Fastest setting, no filtering
  adc.setBipolar(0, false);
  adc.setGain(0, 1);
  adc.setEnabled(0, true);
}

void loop() {
  // Start a conversion and release the SPI bus.
  adc.startConversion(0);

  // Do something with another SPI device while AD7794 is converting.
  SPI.beginTransaction(SPISettings(1000000, MSBFIRST, SPI_MODE0));
  digitalWrite(OTHER_SPI_CS, LOW);
  SPI.transfer(0xAA);
  digitalWrite(OTHER_SPI_CS, HIGH);
  SPI.endTransaction();

  // Read the conversion result when ready.
  uint32_t raw = adc.awaitConversionAndReadRaw();
  float volts = (raw * 2.5f) / (AD7794_ADC_MAX_UP * 1.0f);
  Serial.println(volts, 6);

  delay(10);
}
