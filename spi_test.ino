#include <SPI.h>
uint8_t buffer1;
uint8_t buffer2;
void setup (void)
{
  Serial.begin(9600);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH); 
  
  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.beginTransaction (SPISettings (2000000, MSBFIRST, SPI_MODE0));  
  SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV64);
  Serial.print("begin!!");
  
}  // end of setup


void loop (void)
{
  byte c = 0x73;
  byte d = 0xfd;
  digitalWrite(15, LOW);    // SS is pin 10

  do{
  buffer1 = SPI.transfer (c); 
  }while(buffer1 != 0x85);
  
  SPI.transfer(d);
  delay (500); 
  digitalWrite(15, HIGH);
  
}  
