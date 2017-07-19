#include <SPI.h>
uint8_t buffer1;
uint8_t buffer2;
int  serIn;
byte c = 0x73;
byte d = 0xda;
byte e = 0x7d;
byte bufferSerial[3] = {0xff};
void setup (void)
{
  Serial.begin(9600);
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH); 

  delay(1000);
  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.beginTransaction (SPISettings (2000000, MSBFIRST, SPI_MODE0));  
  
  SPI.begin ();  
  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV8);
  Serial.print("begin!!");
   
}  // end of setup


void loop (void)
{
  
 /* do{
  buffer1 = SPI.transfer (c); 
  Serial.println(buffer1, HEX);
  delay(100);
  }while(buffer1 != 0x85);
  */
   //if(Serial.available()) {    
    //inform that Arduino heard you saying something
    //Serial.print("Arduino heard you say: ");
    
    //keep reading and printing from serial untill there are bytes in the serial buffer
     //while (Serial.available()>0){
        //Serial.readBytes(bufferSerial, 1);  //read Serial        
        //Serial.print(bufferSerial[0]);  //prints the character just read
        digitalWrite(15, LOW);    
         while(buffer1 != 0xba){
          buffer1 = SPI.transfer(c);
          Serial.println(buffer1);
          delay(10);
         }
        //Serial.println(buffer1);
        //delay(60);
        buffer1 = SPI.transfer(e);
        //Serial.println(buffer1);
        delay(60);
        SPI.transfer(d);
        
     //}
     //}
     
    //the serial buffer is over just go to the line (or pass your favorite stop char)               
    //Serial.println();
  }
  
  //SPI.transfer(e);
  //delay(20);
  //digitalWrite(15, HIGH);
 
  
