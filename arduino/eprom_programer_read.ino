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
int incomingByte = 0;

void setAddress(unsigned long address) {

  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 16));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

void readAddress(unsigned long address){

  digitalWrite(E,HIGH);
  
  setAddress(address);
  digitalWrite(E,LOW);
  delayMicroseconds(10);
  byte read_value = digitalRead(Q0) | digitalRead(Q1) << 1 | digitalRead(Q2) << 2 | digitalRead(Q3) << 3 |
                    digitalRead(Q4) << 4 | digitalRead(Q5) << 5 | digitalRead(Q6) << 6 | digitalRead(Q7) << 7;
  Serial.write(read_value);
  digitalWrite(E,HIGH);
}

void setup() {
  // Setups iniciales
  digitalWrite(E, HIGH);
  pinMode(E,OUTPUT);
  Serial.begin(115200);
   
  pinMode(Q0, INPUT);
  pinMode(Q1, INPUT);
  pinMode(Q2, INPUT);
  pinMode(Q3, INPUT);  
  pinMode(Q4, INPUT);
  pinMode(Q5, INPUT);  
  pinMode(Q6, INPUT);
  pinMode(Q7, INPUT); 
  
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  
  //Un SR clear antes de la rutina porsiacaso
  setAddress(0);
  delay(1000);

}

void loop() {

        setAddress(1);
        delay(500);
        setAddress(0);
        delay(500);
        
        if (Serial.available() > 0) {
                // read the incoming byte:
          incomingByte = Serial.read();

          if(incomingByte == 114){  //ASCII 'r' for read and do not make errors when execute python script
            for (unsigned long i = 0; i<size_eprom; i++) {
              readAddress(i); 
            }
          }
          
        }
  // put your main code here, to run repeatedly:

}
