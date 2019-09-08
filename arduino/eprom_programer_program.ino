#define SHIFT_DATA 13
#define SHIFT_CLK 12
#define SHIFT_LATCH 11
#define Q7 10
#define Q6 9
#define Q5 8
#define Q4 7
#define Q3 6
#define Q2 5
#define Q1 4
#define Q0 3
#define E 2

unsigned long size_eprom = 1048576; //number of adress memory of 27c801 eprom
unsigned long chunk = 0; //counter
unsigned long address = 0;
int buffer_size = 64; //arduino buffer size communication protocol
byte buffer_size_bin = B111111; //arduino buffer size in binary
int incomingByte = 0;
byte byteDataFile; // data received from the script in python

void setAddress(unsigned long address) {

  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 16));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void programAddress(unsigned long address, byte value){

  digitalWrite(E,HIGH);
  
  setAddress(address);
  digitalWrite(Q0,bitRead(value, 0));
  digitalWrite(Q1,bitRead(value, 1));
  digitalWrite(Q2,bitRead(value, 2));
  digitalWrite(Q3,bitRead(value, 3));
  digitalWrite(Q4,bitRead(value, 4));
  digitalWrite(Q5,bitRead(value, 5));
  digitalWrite(Q6,bitRead(value, 6));
  digitalWrite(Q7,bitRead(value, 7));
  delayMicroseconds(4);   
  digitalWrite(E,LOW);
  delayMicroseconds(60);
  digitalWrite(E,HIGH);
}

void setup() {
  // initial setups
  digitalWrite(E, HIGH);
  pinMode(E,OUTPUT);
  Serial.begin(115200);
   
  pinMode(Q0, OUTPUT);
  pinMode(Q1, OUTPUT);
  pinMode(Q2, OUTPUT);
  pinMode(Q3, OUTPUT);  
  pinMode(Q4, OUTPUT);
  pinMode(Q5, OUTPUT);  
  pinMode(Q6, OUTPUT);
  pinMode(Q7, OUTPUT); 
  
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  
  //a SR clear before the routine, just in case
  setAddress(0);
  delay(1000);

}

void loop() {

        setAddress(2); //this is because i connected a led to know if is in writing or reading mode
        delay(500);
        setAddress(0);
        delay(500);

        
        if (Serial.available() > 0) {
          // read the incoming byte:
          incomingByte = Serial.read();
          if(incomingByte == 119){  //ASCII 'w' for write and do not make errors when execute python script
            
            Serial.write(B01010100); //for the first execution, in other case the loop in python never execute
            while(chunk < size_eprom/buffer_size){
              if(Serial.available() == buffer_size_bin){ //wait for the 64 bytes from the python script
                for(int i = 0;i<buffer_size;i++){
                  byteDataFile = Serial.read();
                  address = chunk*buffer_size + i;
                  programAddress(address,byteDataFile);
                }
                chunk += 1; 
                Serial.write(B01010100); //send a T (TRUE) when the chunk was written
              }
            }
            
          }
        }          
}
