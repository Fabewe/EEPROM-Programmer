#define datap 11
#define shcp 13
#define stcp 12
#define butt A2
#define DIRECCION 0
#define EEPROM_D0 2
#define EEPROM_D7 9
#define ADDRPINS 16
/*
Set the address given to the A0-A15 pins on eeprom

@param: Memory address to set (int)

*/
void Address(int dir);

/*
Shows on terminal the all the data from the EEPROm
@param:
    -none:Shows all data.
    -int:Shows all the data starting from the address given
    -int,int:Shows all the data starting from the first position to the second one.

@return:Nones
*/

void printContent();
void printContent(int start);
void printContent(int starting,int ending);
/*
Read EEprom byte on given direction

@param: Memory address to read (int)

@return: Byte stored on given address (Byte)
*/
byte readEEPROM(int i);


int i=DIRECCION;


void setup() {

pinMode(datap,OUTPUT);
pinMode(shcp,OUTPUT);
pinMode(stcp,OUTPUT);
pinMode(butt,INPUT);
digitalWrite(stcp,HIGH);
Serial.begin(57600);

printContents();


}

void loop() {


}





byte readEEPROM(int i) {
  // I/O EEPROM pins configured as INPUT
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin++) pinMode(pin,INPUT);                              

  //Set the address on A0-A16 pins on eeprom
  Address(i);

  byte data = 0;   
  
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) data=(data<<1)+digitalRead(pin);
  
  return data;
}





void printContents() {
  for (int base = 0; base <= 255; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


void printContent(int start){
  if(start<pow(2,ADDRPINS)){
  Serial.println("---  0  1  2  3  4  5  6  7  8 ||  9  A  B  C  D  E  F ---");
    for (int base = start; base <= pow(2,ADDRPINS)-16; base += 16) {
      byte data[16];
      for (int offset = 0; offset <= 15; offset++) data[offset] = readEEPROM(base + offset);

    char buf[80];
    sprintf(buf, "%03X:  %02X %02X %02X %02X %02X %02X %02X %02X || %02X %02X %02X %02X %02X %02X %02X %02X",
            base, 
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }}else{
    Serial.println("Error");
    }
}

void printContent(int start,int ending){
  if(start > ending){int aux=start;start=ending; ending=aux;}
  Serial.println("---  0  1  2  3  4  5  6  7  8 ||  9  A  B  C  D  E  F ---");
 for (int base = start; base <= pow(2,ADDRPINS)-16; base += 16) {
      byte data[16];
      for (int offset = 0; offset <= 15; offset++) data[offset] = readEEPROM(base + offset);

    char buf[80];
    sprintf(buf, "%03X:  %02X %02X %02X %02X %02X %02X %02X %02X || %02X %02X %02X %02X %02X %02X %02X %02X",
            base, 
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }


  
  
  
  
  
  }
  
  



void Address(int dir){

  shiftOut(datap, shcp, MSBFIRST, dir >> 8);
  shiftOut(datap, shcp, MSBFIRST, dir);


  digitalWrite(stcp,HIGH);
  digitalWrite(stcp,LOW);
  digitalWrite(stcp,HIGH);
  }
