/* 
 *  ECE 547 Project
 *  A. Sahay, J. Folliard, S. Rao, S. Shah
 *  RNG using Arduino devboard
 */

#include <Wire.h> 

// hardware defines 
#define ADXL375 0x53    
#define DIODE A0  
#define MIC   A3

// variables
uint8_t   x, y, z;        // x, y, and z axes for accelerometer    

uint8_t   rand_bits = 0;  // temp variable, random bits
uint32_t  rand_num = 0;  // temp seed for PRNG

uint8_t   counter = 0;    // counter for superloop 

void setup() {
  Serial.begin(115200);

  // set SCL to 400KHz and begin I2C
  Wire.setClock(400000); 
  Wire.begin();

  // communicate to set up ADXL375
  Wire.beginTransmission(ADXL375);
  Wire.write(0x2C); // write to BW_RATE register  
  Wire.write(0x0F); // set bits D3:D0 to 1101 for 800 Hz
  Wire.endTransmission();

  Wire.beginTransmission(ADXL375);
  Wire.write(0x2D); // write to POWER_CTL register 
  Wire.write(0x08); // set bit D3 high for measurement 
  Wire.endTransmission();

  Wire.beginTransmission(ADXL375);
  Wire.write(0x31); // write to DATA_FORMAT register
  Wire.write(0x0B); // set bits D4:D0 to 01011 for format
  Wire.endTransmission();

  // set DIODE as input 
  pinMode(DIODE, INPUT); 
  pinMode(MIC, INPUT); 
  // minor delay for ensuring setup 
  delay(10);
}

void loop() {
  for(uint8_t i = 0; i < 16; i++) {
    // communicate to get data from ADXL375
    Wire.beginTransmission(ADXL375);
    Wire.write(0x32); 
    Wire.endTransmission(false);

    // read 6 byte over I2C, convert to normal values
    Wire.requestFrom(ADXL375, 6, true); 
    x = (int16_t)( Wire.read()| Wire.read() << 8); 
    y = (int16_t)( Wire.read()| Wire.read() << 8); 
    z = (int16_t)( Wire.read()| Wire.read() << 8); 

    // extract last 2 bits from a random sample 
    if(i % 5 == 0) rand_bits = (analogRead(DIODE) & (1 << 1 | 1 << 0));
    else if(i % 5 == 1) rand_bits = (x & (1 << 1 | 1 << 0)); 
    else if(i % 5 == 2) rand_bits = (y & (1 << 1 | 1 << 0));  
    else if(i % 5 == 3) rand_bits = (z & (1 << 1 | 1 << 0)); 
    else if(i % 5 == 4)rand_bits = (analogRead(MIC) & (1 << 1 | 1 << 0)); 

    // shift bits into random number 
    rand_num = rand_num | (rand_bits << 2*i);

    delay(1); 
  } 

  // print number to UART
  Serial.println(rand_num); 
  delay(8); 

  rand_num = 0; 
}
