/* 
 *  ECE 547 Project
 *  A. Sahay, J. Folliard, S. Rao, S. Shah
 *  RNG using Arduino devboard
 */

#define DIODE A0

uint8_t   rand_bits = 0;
uint32_t  rand_num = 0;  
float     voltage = 0; 

void setup() {
  Serial.begin(115200);
  pinMode(DIODE, INPUT); 
}

void loop() {
  for(uint8_t i = 0; i < 32; i++) {
    // extract last 2 bits from a random sample 
    rand_bits = (analogRead(DIODE) & (1 << 0)); 
    // shift bits into random number 
    rand_num = rand_num | (rand_bits << i);

    delay(1); 
  }

  // print number to UART
  Serial.println(rand_num); 
  delay(4);  

  rand_num = 0; 
}
