#include <SPI.h>
uint8_t buffer1;
const uint8_t DUMMY = 0xff;
const uint8_t NOP = 0xff;
const uint8_t NEED_WIFI = 0x02;
const uint8_t DONE_SENDING = 0x3;
const uint8_t NEED = 1;
const uint8_t XNEED = 0;
/*Mock wifiSSID*/ 
uint8_t WifiSSID[8] = {0x89, 0x2d, 0x89, 0x78, 0x98, 0x54, 0x84, 0x28};
uint8_t WifiPW[8] = {0x23, 0x18, 0x1d, 0xad, 0x03, 0x82, 0x92, 0x12};

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
     SPI.transfer(NEED_WIFI);
     buffer1 = SPI.transfer(DUMMY);
        
     if(buffer1 == NEED){
       //getWifiSsid(WifiSSID, &PWSSIDlength);
        
      /* SpiTransferArray(WifiSSID);
          //getWifiPW(WifiPW, , &PWSSIDlength);
       SpiTransferArray(WifiPW);*/
       Serial.println("done");
       delay(1000);
        }
     else if (buffer1 == XNEED){  //else?
         Serial.println("NOP");
       }
     Serial.println(buffer1);
     delay(10);
    //the serial buffer is over just go to the line (or pass your favorite stop char)               
    //Serial.println();
}


void SpiTransferArray(uint8_t* data){
    //TODO: code:
    int i;
    for(i = 0;i < 8;i++){
      SPI.transfer(data[i]);
      delay(10);
    }
  }
  
  //SPI.transfer(e);
  //delay(20);
  //digitalWrite(15, HIGH);
