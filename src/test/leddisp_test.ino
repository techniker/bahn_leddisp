/*

  LEDDISPLAY DB Bahn regio

 Bjoern Heller <tec@hellercom.de>

 GNU/GPL - tec - 2012 (c)

 */

//Pin def

int oA0 = 2;
int oA1 = 3;
int oA2 = 4;
int E12 = 5;
int STCP = 6;
int SHCP = 7;
int DS   = 8;
int MR   = 9;
int debugLED = 13;


int d = 0; // delay

void setup()
{
  pinMode(oA0, OUTPUT);
  pinMode(oA1, OUTPUT);
  pinMode(oA2, OUTPUT);
  pinMode(E12, OUTPUT); //OUTPUT ENABLE
  
  pinMode(STCP, OUTPUT); //Storage Register Clock Input
  pinMode(SHCP, OUTPUT); //Shift Register Clock Input
  pinMode(DS, OUTPUT);   //SERIAL DATA OUT
  pinMode(MR, OUTPUT);   //MASTER RESET (actice LOW)
  
  pinMode(debugLED, OUTPUT); //debug LED
  
  digitalWrite(MR, LOW);
  digitalWrite(MR, HIGH);
}

void allLow()
{
  // sets all outputs to OFF
  digitalWrite(E12, HIGH);
}

void strobeLine(int outputPin)
// selects line
{
  digitalWrite(oA0, (outputPin & 1) ? HIGH : LOW);
  digitalWrite(oA1, (outputPin & 2) ? HIGH : LOW);
  digitalWrite(oA2, (outputPin & 4) ? HIGH : LOW);
  delayMicroseconds(1);
  
  digitalWrite(E12, LOW);
  delay(1);
  digitalWrite(E12, HIGH);
}

char blank[8] = {
  0,0,0,0,0,0,0,0
};


char romanI[8] = {
  0,
  14,
  4,
  4,
  4,
  4,
  14,
  0
};


char romanII[8] = {
  0,
  31,
  10,
  10,
  10,
  10,
  31,
  0
};

char romanIII[8] = {
  31,
  31,
  31,
  31,
  31,
  31,
  31,
  31
};


/*long content[8] = {
  0x00000000,
  0x00060000,
  0x00060000,
  0x000F0000,
  0x00060000,
  0x000F0000,
  0x00000000,
  0x00000000
};*/

char* chars[6] = {
  romanI,
  romanII,
  romanII,
  blank,
  romanI,
  romanIII
};

void displayWrite()
{
  for (int i=0; i<8; i++)
  {
    long line = 0;
    for(int j = 0; j < 6; j++) {
      char pxoff = 5 * j;
      char chrline = chars[j][i];
      long bits = chrline;
      for(int x = 0; x < j; x++) {
        bits <<= 5;
      }
      line |= bits;
    }
    line |= 0x00000000;
    writeData(line);
    strobeLine(i);
  }
}

void writeData(long data){
  
  for(int i = 0; i < 32; i++) {
    digitalWrite(DS, ((data >> i) & 1) ? HIGH : LOW);
    delayMicroseconds(1);
    digitalWrite(SHCP, HIGH);
    delayMicroseconds(1);
    digitalWrite(SHCP, LOW);
    delayMicroseconds(1);
  }
  
  digitalWrite(STCP, HIGH);
  delayMicroseconds(1);
  digitalWrite(STCP, LOW);
}

void loop()
{
  displayWrite();
}
