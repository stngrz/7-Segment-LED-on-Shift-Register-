/*
  Shift Register Example
  Turning on the outputs of a 74HC595 using an array

 Hardware:
 * 74HC595 shift register 
 * LEDs attached to each of the outputs of the shift register

 */
//Pin connected to ST_CP of 74HC595
int latchPin = 8;
//Pin connected to SH_CP of 74HC595
int clockPin = 12;
////Pin connected to DS of 74HC595
int dataPin = 11;

//holders for infromation you're going to pass to shifting function
byte data;
byte dataArray[10];

void setup() {
  //set pins to output because they are addressed in the main loop
  pinMode(latchPin, OUTPUT);
  Serial.begin(9600);

  //Binary notation as comment
  dataArray[0] = 0x3F;//0
  dataArray[1] = 0x06;//1 
  dataArray[2] = 0x5B;//2 
  dataArray[3] = 0x4F;//3 
  dataArray[4] = 0x66;//4 
  dataArray[5] = 0x6D;//5 
  dataArray[6] = 0x7D;//6 
  dataArray[7] = 0x07;//7 
  dataArray[8] = 0x7F;//8 
  dataArray[9] = 0x67;//9
//  dataArray[10] = 0x77;//A
//  dataArray[11] = 0x7C;//b
//  dataArray[12] = 0x39;//c 
//  dataArray[13] = 0x5B;//C 
//  dataArray[14] = 0x5E;//d 
//  dataArray[15] = 0x79;//E 
//  dataArray[16] = 0x71;//F 
//  dataArray[17] = 0x76;//H 
//  dataArray[18] = 0x04;//i
//  dataArray[19] = 0x1E;//J
//  //dataArray[20] = 0x;
//  dataArray[21] = 0x38;//L
//  //dataArray[22] = 0x; 
//  dataArray[23] = 0x54;//n
//  dataArray[24] = 0x5C;//o
//  dataArray[25] = 0x3F;//O 
//  dataArray[26] = 0x73;//P
//  //dataArray[27] = 0x; 
//  dataArray[28] = 0x50;//r
//  dataArray[29] = 0x6D;//S
//  //dataArray[30] = 0x;
//  dataArray[31] = 0x1C;//u
//  dataArray[32] = 0x3E;//U
//  //dataArray[33] = 0x;//V
//  //dataArray[34] = 0x;//W 
//  //dataArray[35] = 0x;//X 
//  //dataArray[36] = 0x;//Y 
//  //dataArray[37] = 0x;//Z 
   
  //function that blinks all the LEDs
  //gets passed the number of blinks and the pause time
  blinkAll_2Bytes(2,500); 
}

void loop() {

  for (int j = 0; j < 10; j++) {
    //load the light sequence you want from array
    data = dataArray[j];
    //ground latchPin and hold low for as long as you are transmitting
    digitalWrite(latchPin, 0);
    //move 'em out
    shiftOut(dataPin, clockPin, data);
    //return the latch pin high to signal chip that it 
    //no longer needs to listen for information
    digitalWrite(latchPin, 1);
    delay(1300);
  }
}



// the heart of the program
void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  // This shifts 8 bits out MSB first, 
  //on the rising edge of the clock,
  //clock idles low

  //internal function setup
  int i=0;
  int pinState;
  pinMode(myClockPin, OUTPUT);
  pinMode(myDataPin, OUTPUT);

  //clear everything out just in case to
  //prepare shift register for bit shifting
  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  //for each bit in the byte myDataOut�
  //NOTICE THAT WE ARE COUNTING DOWN in our for loop
  //This means that %00000001 or "1" will go through such
  //that it will be pin Q0 that lights. 
  for (i=7; i>=0; i--)  {
    digitalWrite(myClockPin, 0);

    //if the value passed to myDataOut and a bitmask result 
    // true then... so if we are at i=6 and our value is
    // %11010100 it would the code compares it to %01000000 
    // and proceeds to set pinState to 1.
    if ( myDataOut & (1<<i) ) {
      pinState= 1;
    }
    else {  
      pinState= 0;
    }

    //Sets the pin to HIGH or LOW depending on pinState
    digitalWrite(myDataPin, pinState);
    //register shifts bits on upstroke of clock pin  
    digitalWrite(myClockPin, 1);
    //zero the data pin after shift to prevent bleed through
    digitalWrite(myDataPin, 0);
  }

  //stop shifting
  digitalWrite(myClockPin, 0);
}


//blinks the whole register based on the number of times you want to 
//blink "n" and the pause between them "d"
//starts with a moment of darkness to make sure the first blink
//has its full visual effect.
void blinkAll_2Bytes(int n, int d) {
  digitalWrite(latchPin, 0);
  shiftOut(dataPin, clockPin, 0);
  shiftOut(dataPin, clockPin, 0);
  digitalWrite(latchPin, 1);
  delay(200);
  for (int x = 0; x < n; x++) {
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 255);
    shiftOut(dataPin, clockPin, 255);
    digitalWrite(latchPin, 1);
    delay(d);
    digitalWrite(latchPin, 0);
    shiftOut(dataPin, clockPin, 0);
    shiftOut(dataPin, clockPin, 0);
    digitalWrite(latchPin, 1);
    delay(d);
  }
}
