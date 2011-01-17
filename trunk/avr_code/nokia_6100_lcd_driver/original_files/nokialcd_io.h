void LCDSend9Bit(int data);

#define LCDCommand(data) LCDSend9Bit((data) & 0xff)
#define LCDData(data)    LCDSend9Bit(0x100 + ((data) & 0xff))
