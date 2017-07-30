#include <SPI.h>
uint8_t buffer1;
const uint8_t DUMMY = 0xff;
const uint8_t NOP = 0xff;
const uint8_t NEED_WIFI = 0x02;
const uint8_t WIFI_DATA = 0x3;
const uint8_t DONE_SENDING = 0x4;
const uint8_t NEED = 1;
const uint8_t XNEED = 0;
uint8_t testBuffer[2] = {0x89, 0x23};
/*Mock wifiSSID*/ 
uint8_t WifiSSID[8] = {0x2d, 0x2d, 0x89, 0x78, 0x98, 0x54, 0x84, 0x28};
uint8_t WifiPW[8] = {0xba, 0x18, 0x1d, 0xad, 0x03, 0x82, 0x92, 0x12};

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
  
    /*while(1){
      SPI.transfer(NEED_WIFI);
      delay(10);
    }*/
     SPI.transfer(NEED_WIFI);
     //delay(1);
     buffer1 = SPI.transfer(DUMMY);
        
     if(buffer1 == NEED){
       //getWifiSsid(WifiSSID, &PWSSIDlength);
        
      /* SpiTransferArray(WifiSSID);
          //getWifiPW(WifiPW, , &PWSSIDlength);
       SpiTransferArray(WifiPW);*/
       //delay(1);
       //SPI.transfer(WIFI_DATA);
       delay(1);
       SpiTransferArray(WifiSSID);
       SpiTransferArray(WifiPW);
       Serial.println("done");
       delay(1000);
        }
     else if (buffer1 == XNEED){  //else?
         Serial.println("NOP");
       }
     Serial.println(buffer1);
     delay(1);
    //the serial buffer is over just go to the line (or pass your favorite stop char)               
    //Serial.println();
}


void SpiTransferArray(uint8_t* data){
    //TODO: code:
    int i;
    for(i = 0;i < 8;i++){
      SPI.transfer(data[i]);
      delay(1);
    }
    delay(1);
  }
  
  //SPI.transfer(e);
  //delay(20);
  //digitalWrite(15, HIGH);
