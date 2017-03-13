#define leftUp 5
#define leftDown 6
#define rightUp 11
#define rightDown 4

#define  BUFSIZE 10
#define MOTORNUM 4

byte motors[MOTORNUM] = {leftUp, leftDown, rightUp, rightDown};

byte buffer[MOTORNUM][BUFSIZE];
byte bufferIndex[MOTORNUM];

unsigned int bufferSum[MOTORNUM];
byte averageSpeed[MOTORNUM];

void clearBuffer()
{
  for(byte j=0; j<MOTORNUM; j++)
    {
      for(byte i=0; i<BUFSIZE; i++)
        buffer[j][i] = 0;
      bufferIndex[j] = 0;
    }
}

void addElementToBuffer(byte motor, byte element)
{
  bufferIndex[motor]++;
  if(bufferIndex[motor] >= BUFSIZE)
    bufferIndex[motor] = 0;
  buffer[motor][bufferIndex[motor]] = element; // Add element from serial to buffer
}

void calculateAverages()
{
  for(byte i=0; i<MOTORNUM; i++) // Zerowanie sum elementów buforów
    bufferSum[i] = 0;
  for(byte i=0; i<MOTORNUM; i++)
  {
    for(byte j=0; j<BUFSIZE; j++)
      bufferSum[i] += buffer[i][j];  // Dodawanie kolejnych elementów buforów
    averageSpeed[i] = bufferSum[i] / BUFSIZE;  // Obliczenie średniej wartości buforów
  }
}

void setOutputs()
{
   for(byte i=0; i<MOTORNUM; i++)
     analogWrite(motors[i], averageSpeed[i]); // Write calculated values to PWM outputs
}

void setup()
{
  Serial.begin(9600);
  while(!Serial){}
  clearBuffer();
}

void loop()
{
  if(Serial.available())
  {
    for(byte i=0; i<MOTORNUM; i++)
      addElementToBuffer(i, Serial.read());
  }
  
  calculateAverages();
  setOutputs();
  
}
