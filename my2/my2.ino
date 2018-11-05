/* *** (c)2018 Pantelis  ***

1ST CHECK WITH A MULTIMETER IF DRAM CHIP IS BROKEN FOR NOT FRYING YOUR ARDUINO 
-------------------------------------------------------------------------------
 1- Check for shorted Vcc
 2- Inputs Ax, /CAS, /RAS, /WE, DIN must be high-Z. If shorted to 0v/5v,    it's broken.
 3- Output DOUT must be high-Z in the absence of RAS/CAS activity.
 4- Output DOUT is push-pull AFTER the RAS/CAS read sequence (see datasheet). If it's high-Z, it's broken.  
 

 CHIP	ARDUINO		                                    ARDUINO	 CHIP
-----------+--------------                           -------------+-------------
IO1	1	  A1	15				PD2	2	  17	CAS
IO2	2	  A2	16				PD3	3	  4	  RAS
WE	3	  A5	19				PD4	4	  --	--
RAS	4	  3	  PD3				PD5	5	  6	  A0
NC	5	  --	--				PD6	6	  7	  A1
A0	6	  5	  PD5				PD7	7	  8	  A2
A1	7	  6	  PD6				PB0	8	  9	  A3
A2	8	  7	  PD7				PB1	9	  11	A4
A3	9	  8	  PB0				PB2	10	12	A5
VCC	10	+5V	+5V				PB3	11	13	A6
A4	11	9	  PB1				PB4	12	14	A7
A5	12	10	PB2				PB5	13	15	A8
A6	13	11	PB3				--	--	5	  NC
A7	14	12	PB4				+5V	+5V	10	VCC
A8	15	13	PB5				14	A0	16	OE
OE	16	A0	14				15	A1	1	  IO1
CAS	17	2	  PD2				16	A2	2	  IO2
IO3	18	A3	17				17	A3	18	IO3
IO4	19	A4	18				18	A4	19	IO4
VSS	20	GND	GND				19	A5	3	  WE
--	--	4	  PD4				GND	GND	20	VSS


For Extra INFO LED add: 
------------------------
From Arduino PD4 4 
PD4---wiring--220K----(+)LED(-)--wiring--GND 


*/  

#define ADDR_BUS_SIZE 9

// volatile int bus_size;
char buffer[30]; 
String inputString = "";


// name = arduino PIN 
const int adr_pins[9] = {5,6,7,8,9,10,11,12,13}; // ARDUINO PINS 
const int data_pins[4] = {15, 16, 17, 18};  // ARDUINO PINS 
const int RAS = 3; // Row Array Strobe -- ACTIVE = LOW 
const int CAS = 2; // Column Array Strobe -- ACTIVE = LOW 
const int OE = A0; // Output Enable -- ACTIVE = LOW  
const int WE = A5; // Write Enable -- ACTIVE = LOW 
const int Time = A6;  
const int dellay = 0;// was 10 
const int aMax=16; // was 512 total addresses  
const int loops=1; // iterate loop()  
int rv[4]={0,0,0,0}; // used to store returned data 
bool done = false; 
bool err=false;  


// *** setup ***
void setup() {
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT);
  for(int n = 0; n <9; n++)  
    pinMode(adr_pins[n], OUTPUT); 
  pinMode(RAS, OUTPUT); 
  pinMode(CAS,OUTPUT);
  pinMode(OE, OUTPUT); 
  pinMode(WE, OUTPUT); 
  
  digitalWrite(RAS, HIGH);  // disable
  digitalWrite(CAS,HIGH);    // disable 
  digitalWrite(WE,HIGH);    // disable 
  digitalWrite(OE,HIGH);    // disable 
  Serial.println("DRAM 256k x 4 tester."); 
  
  noInterrupts();
  for (int i = 0; i < (1 << ADDR_BUS_SIZE); i++) {
    digitalWrite(RAS, LOW);
    digitalWrite(RAS, HIGH);
  }
} //**** setup ****




//**** loop ****
void loop() { 
  int k=1;
  int n;
  interrupts(); Serial.print("1st Pass writting 0:"); Serial.flush(); noInterrupts(); fill(0);
  interrupts(); Serial.print("2nd Pass writting 1:"); Serial.flush(); noInterrupts(); fill(15);
  if (++k>loops) {
    Serial.print("End."); Serial.flush(); noInterrupts(); 
    if (err)digitalWrite(LED_BUILTIN,LOW);else digitalWrite(LED_BUILTIN,HIGH);
    exit(0);}
}; //**** loop ****




//**** fill **** 
void fill( int v) {
  bool err=false;
  int n,r,c,g = 0;
  long adr;
  int Valuta;
  bool dbg=0;
  // v %= 1;
  //for (r=0 ; r< (1<<ADDR_BUS_SIZE ); r++) {
  for (r=0 ; r<2 ; r++) {
  //  for (c=0; c < (1<<ADDR_BUS_SIZE ); c++) {
    for (c=0; c < 2; c++) {
      writeValue(r, c, v);
      Valuta=readValue(r, c);
      if(dbg){Serial.print("->");Serial.print(v);
      Serial.print("->");Serial.print(Valuta);}
      if (Valuta != v) {
        adr = (c << ADDR_BUS_SIZE) + r;
        Serial.print(" adr=");Serial.println(adr);
        err=true;  
      }
      if ((Valuta != v) | dbg) Serial.println(" "); 
    }
  }
  if (!err)Serial.println("OK");
} //**** fill **** 




void setBus(unsigned int a) {
  bool dbg=0; // debug 
  for (int i = 0; i < ADDR_BUS_SIZE; i++) {
    digitalWrite(adr_pins[i],((a >> i) & 1));
    if(dbg) Serial.print(((a >> i) & 1));
  };
  if (dbg)Serial.println(" setbus: a=" + String(a)+" ");
}




void writeValue(int r, int c, int v) { 
  char buffer[30];   
  bool dbg=0;
  int n; 
/*   
  0 RAS CAS HIGH DISABLE 
  1 row address 
  2 RAS low ENABLE
  3 OE low ENABLE
  4 col address input 
  5 CAS low ENABLE
  6 OE HIGH DISABLE
  7 WE low ENABLE
  8 Write Data 
  9*OE low ENABLE * maybe not needed
 10 CAS RAS high DISABLE  
 11 WE high DISABLE  * maybe not needed
 */
 
  //0 RAS CAS HIGH DISABLE 
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  
  //1 SET ROW ADDRESS 
  setBus(r);
  delay(dellay); //tASR

  //2 RAS low ENABLE
  digitalWrite(RAS, LOW);

  //3 OE low ENABLE
  delay(dellay); //tRAH
  digitalWrite(OE, LOW);

  //4 SET col address input  
  delay(dellay);
  setBus(c);
   
  //5 CAS low ENABLE
  digitalWrite(CAS, LOW);
  
  //6 OE HIGH DISABLE
  digitalWrite(OE, HIGH);
   
  //7 WE low DISABLE WRITE ENABLE
  digitalWrite(WE,LOW);

  if (dbg) Serial.print(" ["+String(r)+","+String(c)+"]:");
  //8 Write Data 
  if (dbg)Serial.print("(");
  for(n = 0; n <4; n++)  {
    pinMode(data_pins[n],OUTPUT);
    // all 4 digits the same v :0 or 1 
    //digitalWrite(data_pins[n], (v & 1)? HIGH : LOW);
    //if (dbg){Serial.print((v & 1)? HIGH : LOW );if(n<3)Serial.print(",");}
    // whole number v in 4 bits: 
    digitalWrite(data_pins[n],((v >> n) & 1)? HIGH : LOW);
    if (dbg){Serial.print(((v >> n) & 1)? HIGH : LOW );if(n<3)Serial.print(",");} 
  } 
  if (dbg)Serial.print(")");

 //9 OE low ENABLE * maybe not needed
  digitalWrite(OE, LOW);
  
 //10 CAS RAS high DISABLE  
  digitalWrite(CAS, HIGH);   
  digitalWrite(RAS, HIGH); 

 //11 WE high DISABLE  * maybe not needed
  digitalWrite(WE,HIGH);
 //   OE HIGH DISABLE * maybe not needed
  digitalWrite(OE, HIGH);
} // writeValue



int readValue(int r, int c)  { 
  bool addressBit;
  char buffer[30]; 
  int n,Valuta=0; 
  int rv[3];
  bool dbg=0;

/*   
  0 RAS CAS HIGH DISABLE 
  1 row address 
  2 RAS low ENABLE
  3 col address input 
  4 WE high DISABLE
  5 CAS low ENABLE
  6 OE low ENABLE
  7 READ data 
  8 RAS  CAS WE OE high DISABLE  */
  
  //0 RAS CAS HIGH DISABLE 
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  digitalWrite(WE,HIGH);
  digitalWrite(OE, HIGH);
  
  //1 SET ROW ADDRESS 
  setBus(r);
 
  //2 RAS low ENABLE
  delay(dellay); //tASR
  digitalWrite(RAS, LOW);

  //4 WE high DISABLE WRITE ENABLE
  digitalWrite(WE,HIGH);
  
  //3 SET col address input  
  delay(dellay); //tASC
  setBus(c);

  //5 CAS low ENABLE
  delay(dellay); //tRCS
  digitalWrite(CAS, LOW);
  
  //6 OE LOW ENABLE Output Enable
  digitalWrite(OE, LOW);
  
  //7 READ DATA 
  if(dbg)Serial.print("<");
  for(n = 0; n <4; n++) {
      pinMode(data_pins[n],INPUT);
    //Valuta = Valuta | ( (digitalRead(data_pins[n]) << n));     
    rv[n]=0;
    rv[n]=digitalRead(data_pins[n]);
    if(dbg){Serial.print(rv[n]);if(n<3)Serial.print(",");}
  }
   Valuta=rv[0]+rv[1]*2+rv[2]*4+rv[3]*8;
   if(dbg){Serial.print(">=");Serial.println(Valuta);}
   return Valuta;
 
  //8 RAS  CAS  high DISABLE 
  digitalWrite(RAS, HIGH);
  digitalWrite(CAS, HIGH);
  digitalWrite(WE,HIGH);
  digitalWrite(OE, HIGH);
 
} // readValue 
