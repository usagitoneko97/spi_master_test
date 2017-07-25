#include <SPI.h>
uint8_t buffer1;
uint8_t buffer2;
int  serIn;
byte bufferSerial[3] = {0xff};
const uint8_t DUMMY = 0xff;
const uint8_t NOP = 0xff;
const uint8_t NEED_WIFI = 0x2;
const uint8_t DONE_SENDING = 0x3;
uint8_t* WifiSSID;
uint8_t* WifiPW;
uint8_t PWSSIDlength;

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
   if(Serial.available()) {
     while (Serial.available()>0){
        buffer = SPI.transfer(DUMMY);
        
        if(buffer == NEED_WIFI){
          //getWifiSsid(WifiSSID, &PWSSIDlength);
          SpiTransferArray(WifiSSID);
          SPI.transfer(PWSSIDlength);
          //getWifiPW(WifiPW, , &PWSSIDlength);
          SpiTransferrArray(WifiPw);
          SPI.transfer(PWSSIDlength);
        }
        else if (buffer == NOP){  //else?
          
        }
        
      }
     }
     
    //the serial buffer is over just go to the line (or pass your favorite stop char)               
    //Serial.println();
  }
  SpiTransferArray(uint8_t* data){
    //TODO: code:
  }
  
  //SPI.transfer(e);
  //delay(20);
  //digitalWrite(15, HIGH);
