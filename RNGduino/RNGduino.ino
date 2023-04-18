/* 
 *  ECE 547 Project
 *  A. Sahay, J. Folliard, S. Rao, S. Shah
 *  RNG using Arduino devboard
 */

#define DIODE A0

uint8_t rand_bits = 0;
uint8_t rand_num = 0;  

void setup() {
  Serial.begin(115200);
  pinMode(DIODE, INPUT); 
}

void loop() {
  // extract last 2 bits from a random sample 
  rand_bits = (analogRead(DIODE) & (1 << 0 | 1 << 1)); 
  // populate last 2 bits of random number 
  rand_num = rand_bits;

  delay(1); 

  // extract last 2 bits from a random sample 
  rand_bits = (analogRead(DIODE) & (1 << 0 | 1 << 1)); 
  // populate next 2 bits of random number 
  rand_num = rand_num | (rand_bits << 2);

  delay(1); 

  // extract last 2 bits from a random sample 
  rand_bits = (analogRead(DIODE) & (1 << 0 | 1 << 1)); 
  // populate next 2 bits of random number 
  rand_num = rand_num | (rand_bits << 4);

  delay(1); 

  // extract last 2 bits from a random sample 
  rand_bits = (analogRead(DIODE) & (1 << 0 | 1 << 1)); 
  // populate next 2 bits of random number 
  rand_num = rand_num | (rand_bits << 6);

  Serial.println(rand_num);
  delay(10);  
}
