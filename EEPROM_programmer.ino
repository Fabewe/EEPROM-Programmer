#define DATAPIN 11
#define SHIFTPIN 13
#define STORAGEPIN 12
#define BUTTONPIN A2
#define WEPIN A3
#define OEPIN A4
#define DIRECCION 0
#define EEPROM_D0 2
#define EEPROM_D7 9
#define ADDRPINS 6
#define NBYTES 4


/*
Quick check to parameters given to the program
*/
boolean check();

/*
Set the address given to the A0-A15 pins on EEPROM

@param: Memory address to set (unsignet int)

*/
void Address(unsigned int dir);

/*
Write data on the EEPROM

@param: Byte to write on EEPROM
@param: Control to turn pins to OUTPUT

*/
bool singleWrite(byte data,bool control);

/*
Read and shows on serial monitor single memory address data

@param: Memory address to check (unsignet int)

*/
void singleRead(unsigned int dir);

/*
Shows on terminal the all the data from the EEPROM
@param:
    -none:Shows all data.
    -unsignet int:Shows all the data starting from the address given
    -int,int:Shows all the data starting from the first position to the second one.

@return:None
*/
void printContent();
void printContent(unsigned int start);
void printContent(int starting, int ending);

/*
Read EEprom byte on given direction

@param: Memory address to read (int)

@return: Byte stored on given address (Byte)
△△△△ NO OUTPUT ENABLE CONTROL △△△△

*/
byte readEEPROM(unsigned int i);

int i = DIRECCION;

void setup()
{

  pinMode(DATAPIN, OUTPUT);
  pinMode(SHIFTPIN, OUTPUT);
  pinMode(STORAGEPIN, OUTPUT);
  pinMode(WEPIN,OUTPUT);
  pinMode(OEPIN,OUTPUT);
  pinMode(BUTTONPIN, INPUT);
  digitalWrite(WEPIN, HIGH);
  digitalWrite(STORAGEPIN, HIGH);
  Serial.begin(38400,SERIAL_8O1);
  if (!check())
  {
    Serial.println("Error detected, execution aborted!!");
  }
  else
  {
    
    printContent();
    
  }
}


void loop() {}
//---------FUNCTIONS--------//
bool singleWrite(byte data,bool control){
  if (control) for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++){ pinMode(pin, OUTPUT);}

  for(int i=EEPROM_D0,j=0;i<=EEPROM_D7;i++,j++){digitalWrite(i,(data>>j)&1);};

  digitalWrite(WEPIN, LOW);
  delayMicroseconds(1);
  digitalWrite(WEPIN, HIGH);
  delay(10);

  return true;
}

bool multiWrite(){}

byte readEEPROM(unsigned int i)
{
  
  // I/O EEPROM pins configured as INPUT
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++)
    pinMode(pin, INPUT);

  // Set the address on A0-A16 pins on eeprom
  Address(i);

  byte data = 0;

  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1)
    data = (data << 1) + digitalRead(pin);

  return data;
}

void singleRead(unsigned int dir)
{
  digitalWrite(OEPIN,LOW);
  byte data = readEEPROM(dir);
  char buffer[18];
  sprintf(buffer, "%04X : %02X %d%d%d%d%d%d%d%d",
          dir,
          data,
          data >> 7 & 1,
          data >> 6 & 1,
          data >> 5 & 1,
          data >> 4 & 1,
          data >> 3 & 1,
          data >> 2 & 1,
          data >> 1 & 1,
          data & 1);
  Serial.println(buffer);
  digitalWrite(OEPIN,HIGH);
}

void printContent()
{
  digitalWrite(OEPIN,LOW);
  Serial.println("----   0  1  2  3  4  5  6  7  ||  8  9  A  B  C  D  E  F ----");
  unsigned int add = 16;
  for (unsigned int base = 0; base <= pow(2, ADDRPINS) - 15; base += add)
  {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1)
    {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%04X:  %02X %02X %02X %02X %02X %02X %02X %02X || %02X %02X %02X %02X %02X %02X %02X %02X",
            base,
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
    if (base == (unsigned int)(pow(2, ADDRPINS)) - 15)
      add = 15;
  }
  digitalWrite(OEPIN,HIGH);
  Serial.println("--");

}

void printContent(unsigned int start)
{
  if (start < (unsigned int)(pow(2, ADDRPINS)))
  {
    Serial.println("----   0  1  2  3  4  5  6  7  ||  8  9  A  B  C  D  E  F ----");
    unsigned int firstoffset = start % 16;
    int add = 16;
    for (unsigned int base = (start - firstoffset); base <= pow(2, ADDRPINS) - 15; base += add)
    {
      byte data[16];
      if (firstoffset != 0)
      {
        for (int fill = 0; fill < firstoffset; fill++)
        {
          data[fill] = B00;
        }
      }
      for (int offset = 0 + firstoffset; offset <= 15; offset++)
        data[offset] = readEEPROM(base + offset);

      char buf[80];
      sprintf(buf, "%04X:  %02X %02X %02X %02X %02X %02X %02X %02X || %02X %02X %02X %02X %02X %02X %02X %02X",
              base,
              data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
              data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

      Serial.println(buf);
      firstoffset = 0;
      if (base == (unsigned int)(pow(2, ADDRPINS)) - 15)
        add = 15;
    }
    Serial.println("--");
  }
  else
  {
    Serial.println("Error: Address is not in the access range");
  }
}

void printContent(int start, int ending)
{
  if (start > ending)
  {
    int aux = start;
    start = ending;
    ending = aux;
  }
  Serial.println("---  0  1  2  3  4  5  6  7  8 ||  9  A  B  C  D  E  F ---");
  int firstoffset = start % 16;
  int endoffset = ending % 16;
  for (long int base = (start - firstoffset); base <= pow(2, ADDRPINS) - 16; base += 16)
  {
    byte data[16];
    if (firstoffset != 0)
    {
      for (int fill = 0; fill < firstoffset; fill++)
      {
        data[fill] = B00;
      }
    }
    for (int offset = 0; offset <= 15; offset++)
      data[offset] = readEEPROM((int)base + offset);

    char buf[80];
    sprintf(buf, "%03X:  %02X %02X %02X %02X %02X %02X %02X %02X || %02X %02X %02X %02X %02X %02X %02X %02X",
            base,
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
    firstoffset = 0;
  }
}

void Address(unsigned int dir)
{

  shiftOut(DATAPIN, SHIFTPIN, MSBFIRST, dir >> 8);
  shiftOut(DATAPIN, SHIFTPIN, MSBFIRST, dir);

  digitalWrite(STORAGEPIN, HIGH);
  digitalWrite(STORAGEPIN, LOW);
  digitalWrite(STORAGEPIN, HIGH);
}

boolean check()
{
  if (EEPROM_D7 != EEPROM_D0 + 7)
  {
    Serial.println("I/O pins must be in serial");
    return false;
  }
  return true;
}
