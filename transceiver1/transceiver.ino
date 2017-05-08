#define SERIAL_BAUD   115200

#define RFM23_FIFO_LENGTH 64

#define BIT_CHECK(a,b) ((a) & (1<<(b)))

const char* EP = "endPacket";
const byte EPLength = 9;

const byte blockSizeMax = RFM23_FIFO_LENGTH;

byte lostBlocks[255]; // only 255 blocks can be lost :)
byte lostBlocksLength = 0;

word packetID = 0;
byte blockLength = 0;
byte blockID = 0;
byte prevBlockID = 0;
int epIndex;
char buffer[100];
//String command = "";
char buffer2[100] = "test-hello\n\r";

#include "transmitter.h"
#include "rfm23bp.h"

char received = 0;
uint8_t STATUS_1;
//uint8_t STATUS_2;

void check_rfm(void)
{
  STATUS_1 = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_1);
  //STATUS_2 = rfm23bp_read_register(RFM23BP_INTERRUPT_STATUS_2);
  
  if (BIT_CHECK(STATUS_1,1))
  {
    received = 1;
  }
  if (BIT_CHECK(STATUS_1,2))
  {
    sent = 1;
  }
  
  //Serial.println(STATUS_1,BIN);
}


void setup()
{
  Serial.begin(SERIAL_BAUD);
  delay(10);
  //Serial.readString();
  //Serial.println("Init started...");

  rfm23bp_init(&params);
  rfm23bp_mode_spi(RFM23_SLAVE_SELECT_PIN);
  rfm23bp_ready();
  rfm23bp_set_default_radio_settings();
  rfm23bp_set_power_level(level_4);

  rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_1,0x06);//0x02
  rfm23bp_set_register(RFM23BP_INTERRUPT_ENABLE_2,0x00);

  rfm23bp_set_register(RFM23BP_GPIO0_CONFIGURATION, 0x12) ; // TX state
  rfm23bp_set_register(RFM23BP_GPIO1_CONFIGURATION, 0x15) ; // RX state

  rfm23bp_enable_rx();
  
  Setup_Transmitter();
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), check_rfm, HIGH);
}

void loop()
{
  // Check for terminal commands
  if (Serial.available() > 0)
  {
    unsigned int commandSize = Serial.readBytes(buffer, 100);
    SendCommand(buffer, commandSize);
  }

  // Check for data received from the Sat
  if (received==1)
  {
    received = 0;
    int absRSSI = rfm23bp_get_rx_fifo();
    Serial.println();
    byte packetIDPtr[] = {RFM_DATA[0], RFM_DATA[1]};
    byte blockIDPtr[] = {RFM_DATA[2], RFM_DATA[3]};
    packetID = (packetIDPtr[1] << 8) + packetIDPtr[0];
    blockID = (blockIDPtr[1] << 8) + blockIDPtr[0];

    if (blockID == 0)
    {
      lostBlocksLength = 0;

      Serial.print((char)RFM_DATA[0]);
      Serial.print((char)RFM_DATA[1]);
    }

    epIndex = strSearch((const char *)RFM_DATA, RFM23_FIFO_LENGTH, EP, EPLength);
    blockLength = epIndex < 0 ? RFM23_FIFO_LENGTH : epIndex;

    for (byte i = 4; i < blockLength; i++)
    {
      if(i<RFM23_FIFO_LENGTH) Serial.print(RFM_DATA[i],HEX);
    }
    
    // Check for lost blocks
    if (blockID != 0 && blockID != prevBlockID + 1)
    {
      byte nLostBlocks = blockID - prevBlockID + 1;
      for(byte i=1; i<=nLostBlocks; i++)
        lostBlocks[lostBlocksLength + i] = prevBlockID + i;

      lostBlocksLength += nLostBlocks;
    }

    prevBlockID = blockID;

    //Send RSSI value and lost blocks information
    if (epIndex >= 0)
    {
      //Serial.println("EP label found! ");
      byte* rssi = (byte*) & (absRSSI);

      Serial.print("RSSI");
      Serial.print((char)rssi[0]);
      Serial.print((char)rssi[1]);
      
      Serial.print(EP);
    }
  }
}

int strSearch(const char* str, byte strSize, const char* pattern, byte patternSize)
{
  bool coincidence = false;
  byte firstIndex = 0;

  for (byte i = 0; i < strSize; i++)
    if (str[i] == pattern[0])
    {
      coincidence = true;
      firstIndex = i;
      for (byte j = 1; j < patternSize; j++)
        if (str[i + j] != pattern[j])
        {
          coincidence = false;
          break;
        }

      if (coincidence)
        return firstIndex;
    }
  return -1;
}

String ToString(char* block, byte size)
{
  String str = "";
  for (byte i = 0; i < size; i++)
    str += block[i];

  return str;
}

