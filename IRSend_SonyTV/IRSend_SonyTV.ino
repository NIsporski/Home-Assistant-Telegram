#include <IRac.h>
#include <IRmacros.h>
#include <IRrecv.h>
#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtext.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <i18n.h>
#include <ir_Airton.h>
#include <ir_Airwell.h>
#include <ir_Amcor.h>
#include <ir_Argo.h>
#include <ir_Bosch.h>
#include <ir_Carrier.h>
#include <ir_Coolix.h>
#include <ir_Corona.h>
#include <ir_Daikin.h>
#include <ir_Delonghi.h>
#include <ir_Ecoclim.h>
#include <ir_Electra.h>
#include <ir_Fujitsu.h>
#include <ir_Goodweather.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Hitachi.h>
#include <ir_Kelon.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Magiquest.h>
#include <ir_Midea.h>
#include <ir_Mirage.h>
#include <ir_Mitsubishi.h>
#include <ir_MitsubishiHeavy.h>
#include <ir_NEC.h>
#include <ir_Neoclima.h>
#include <ir_Panasonic.h>
#include <ir_Rhoss.h>
#include <ir_Samsung.h>
#include <ir_Sanyo.h>
#include <ir_Sharp.h>
#include <ir_Tcl.h>
#include <ir_Technibel.h>
#include <ir_Teco.h>
#include <ir_Toshiba.h>
#include <ir_Transcold.h>
#include <ir_Trotec.h>
#include <ir_Truma.h>
#include <ir_Vestel.h>
#include <ir_Voltas.h>
#include <ir_Whirlpool.h>

#include <IRremoteESP8266.h>
#include <IRsend.h>
#include <IRtext.h>
#include <IRtimer.h>
#include <IRutils.h>
#include <i18n.h>
#include <ir_Airton.h>
#include <ir_Airwell.h>
#include <ir_Amcor.h>
#include <ir_Argo.h>
#include <ir_Bosch.h>
#include <ir_Carrier.h>
#include <ir_Coolix.h>
#include <ir_Corona.h>
#include <ir_Daikin.h>
#include <ir_Delonghi.h>
#include <ir_Ecoclim.h>
#include <ir_Electra.h>
#include <ir_Fujitsu.h>
#include <ir_Goodweather.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Hitachi.h>
#include <ir_Kelon.h>
#include <ir_Kelvinator.h>
#include <ir_LG.h>
#include <ir_Magiquest.h>
#include <ir_Midea.h>
#include <ir_Mirage.h>
#include <ir_Mitsubishi.h>
#include <ir_MitsubishiHeavy.h>
#include <ir_NEC.h>
#include <ir_Neoclima.h>
#include <ir_Panasonic.h>
#include <ir_Rhoss.h>
#include <ir_Samsung.h>
#include <ir_Sanyo.h>
#include <ir_Sharp.h>
#include <ir_Tcl.h>
#include <ir_Technibel.h>
#include <ir_Teco.h>
#include <ir_Toshiba.h>
#include <ir_Transcold.h>
#include <ir_Trotec.h>
#include <ir_Truma.h>
#include <ir_Vestel.h>
#include <ir_Voltas.h>
#include <ir_Whirlpool.h>

#define POWER  (0xE0E040BF)
#define POWER_OFF (0xE0E019E6)
#define POWER_ON ( 0xE0E09966)
#define LEFT (0xE0E0A659)
#define RIGHT (0xE0E046B9)
#define UP (0xE0E006F9)
#define DOWN (0xE0E08679)
#define ENTER (0xE0E016E9)
#define MENU (0xe0e058a7)
#define TOOLS (0xE0E0D22D)
#define EXIT (0xE0E0B44B)
#define RETURN (0xE0E01AE5)
#define SOURCE (0xE0E0807F)
#define EXIT (0xE0E0B44B)
#define VOLUME_UP (0xE0E0E01F)
#define VOLUME_DOWN (0xE0E0D02F)
#define MUTE (0xE0E0F00F)

//#define SAMSUNG_BITS  32 

IRsend irsend(D2);

void Samsung_Send_Code(uint32_t code, uint32_t ms)
{
  irsend.sendSAMSUNG(code, 32);
  delay(ms);
}

void Samsung_Right(uint steps)
{
  for(uint i = 0; i < steps; ++i)
  {
    irsend.sendSAMSUNG(RIGHT, 32);
    delay(1000);
  }
}

void Samsung_Power(uint32_t code)
{
    irsend.sendSAMSUNG(code, SAMSUNG_BITS); 
    delay(4000); // wait 5 seconds
}


void Samsung_Enter()
{
      irsend.sendSAMSUNG(ENTER, SAMSUNG_BITS); 
      delay(2000); // wait 5 seconds
}

void Samsung_Menu()
{
      irsend.sendSAMSUNG(MENU, SAMSUNG_BITS); 
      delay(2000); // wait 5 seconds
}

void Samsung_App_Play(uint steps)
{
    for(uint i = 0; i < steps; ++i)
    Samsung_Send_Code(RIGHT, 1000);

  Samsung_Send_Code(ENTER, 5000);
}

void SamsungYouTube()
{
  Samsung_App_Play(3);
}

void SamsungNetflix()
{
  Samsung_App_Play(2);
}

void SamsungDisney()
{
  Samsung_App_Play(1);
}

void Samsung_VolumeUp(uint vol)
{
  for(uint i = 0; i < vol; ++i)
    Samsung_Send_Code(VOLUME_UP, 100);
}

void Samsung_VolumeDown(uint vol)
{
  for(uint i = 0; i < vol; ++i)
    Samsung_Send_Code(VOLUME_DOWN, 100);
}


void setup()
{
  Serial.begin(115200); // Initialize serial interface
  irsend.begin();
}

void loop() 
{
  //   for (int i = 0; i < 10; i++) { 
  //      //irsend.sendSony(0xa90, 12); // code for Sony TV power command
  //      //irsend.sendNEC(0x20DF8877, 32); // TV power code
  //      //irsend.sendNEC(0x20DF10EF, 32); // TV power code LG
  //      irsend.sendSAMSUNG(POWER, SAMSUNG_BITS); 
  //      //irsend.sendPronto(lgProntoCode, sizeof(lgProntoCode)/sizeof(uint16_t));
  //      //irsend.sendNEC(0x20df10ef, SAMSUNG_BITS);
  //      delay(5000); // wait 5 seconds
  //  }

  //SamsungYouTube();

  // Samsung_Power(POWER_ON);
  
  // delay(5000);

  // Samsung_Menu();

  // delay(5000);

  // Samsung_Power(POWER_OFF);



  //Samsung_Send_Code(POWER_OFF, 4000);

//   Samsung_Send_Code(POWER_ON, 5000);

//   SamsungYouTube();

//   Samsung_Send_Code(EXIT, 3000);

//   Samsung_VolumeUp(10);

// delay(5000);  

//   Samsung_VolumeDown(10);

//   Samsung_Send_Code(MUTE, 2000);

//   //Samsung_Send_Code(ENTER, 3000);
  
//   //SamsungNetflix();

//   Samsung_Send_Code(POWER_OFF, 5000);

}