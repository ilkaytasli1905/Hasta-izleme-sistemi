#include <SoftwareSerial.h>
#include<dht.h>
#include <OneWire.h>
#include <DallasTemperature.h>

dht DHT;
#define ONE_WIRE_BUS 3
#define DHT11_PIN 2
#define MQ_PIN (1) //define which analog input channel you are going to use
#define RL_VALUE (5) //define the load resistance on the board, in kilo ohms
#define RO_CLEAN_AIR_FACTOR (9.83) //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
#define CALIBARAION_SAMPLE_TIMES (50) //define how many samples you are going to take in the calibration phase
#define CALIBRATION_SAMPLE_INTERVAL (500) //define the time interal(in milisecond) between each samples in the
#define READ_SAMPLE_INTERVAL (50) //define how many samples you are going to take in normal operation
#define READ_SAMPLE_TIMES (5) //defi
#define GAS_LPG (0)
#define GAS_CO (1)
#define GAS_SMOKE (2)
#define TIM2_EN() (TIMSK2 = 0x02)
#define TIM2_DIS() (TIMSK2 = 0x00)


float LPGCurve[3] = {2.3,0.21,-0.47}; 
float COCurve[3] = {2.3,0.72,-0.34}; 
float SmokeCurve[3] ={2.3,0.53,-0.44}; 
float Ro = 10; 

int pulsePin = 0;                 // Pulse Sensor purple wire connected to analog pin 0
int blinkPin = 13;                // pin to blink led at each beat
int fadePin = 5;                  // pin to do fancy classy fading blink at each beat
int fadeRate = 0;                 // used to fade LED on with PWM on fadePin
volatile int rate[10];                    // used to hold last ten IBI values
volatile unsigned long sampleCounter = 0;          // used to determine pulse timing
volatile unsigned long lastBeatTime = 0;           // used to find the inter beat interval
volatile int P =512;                      // used to find peak in pulse wave
volatile int T = 512;                     // used to find trough in pulse wave
volatile int thresh = 512;                // used to find instant moment of heart beat
volatile int amp = 100;                   // used to hold amplitude of pulse waveform
volatile boolean firstBeat = true;        // used to seed rate array so we startup with reasonable BPM
volatile boolean secondBeat = true;       // used to seed rate array so we startup with reasonable BPM

// these variables are volatile because they are used during the interrupt service routine!
volatile int BPM;                   // used to hold the pulse rate
volatile int Signal;                // holds the incoming raw data
volatile int IBI = 600;             // holds the time between beats, the Inter-Beat Interval
volatile boolean Pulse = false;     // true when pulse wave is high, false when it's low
volatile boolean QS = false;        // becomes true when Arduoino finds a beat.




OneWire oneWire(ONE_WIRE_BUS);
float tempMin = 999;
float tempMax = 0; 
DallasTemperature sensors(&oneWire);
DeviceAddress sensor1;
SoftwareSerial swSer(0, 1);


void interruptSetup(){     
  // Initializes Timer2 to throw an interrupt every 2mS.
  TCCR2A = 0x02;     // DISABLE PWM ON DIGITAL PINS 3 AND 11, AND GO INTO CTC MODE
  TCCR2B = 0x06;     // DON'T FORCE COMPARE, 256 PRESCALER 
  OCR2A = 0X7C;      // SET THE TOP OF THE COUNT TO 124 FOR 500Hz SAMPLE RATE
  TIMSK2 = 0x02;     // ENABLE INTERRUPT ON MATCH BETWEEN TIMER2 AND OCR2A
  sei();             // MAKE SURE GLOBAL INTERRUPTS ARE ENABLED      
} 

ISR(TIMER2_COMPA_vect){                         // triggered when Timer2 counts to 124
    cli();                                      // disable interrupts while we do this
    Signal = analogRead(pulsePin);              // read the Pulse Sensor 
    sampleCounter += 2;                         // keep track of the time in mS with this variable
    int N = sampleCounter - lastBeatTime;       // monitor the time since the last beat to avoid noise

//  find the peak and trough of the pulse wave
    if(Signal < thresh && N > (IBI/5)*3){       // avoid dichrotic noise by waiting 3/5 of last IBI
        if (Signal < T){                        // T is the trough
            T = Signal;                         // keep track of lowest point in pulse wave 
         }
       }
      
    if(Signal > thresh && Signal > P){          // thresh condition helps avoid noise
        P = Signal;                             // P is the peak
       }                                        // keep track of highest point in pulse wave
    
  //  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
  // signal surges up in value every time there is a pulse
if (N > 250){                                   // avoid high frequency noise
  if ( (Signal > thresh) && (Pulse == false) && (N > (IBI/5)*3) ){        
    Pulse = true;                               // set the Pulse flag when we think there is a pulse
    digitalWrite(blinkPin,HIGH);                // turn on pin 13 LED
    IBI = sampleCounter - lastBeatTime;         // measure time between beats in mS
    lastBeatTime = sampleCounter;               // keep track of time for next pulse
         
         if(firstBeat){                         // if it's the first time we found a beat, if firstBeat == TRUE
             firstBeat = false;                 // clear firstBeat flag
             return;                            // IBI value is unreliable so discard it
            }   
         if(secondBeat){                        // if this is the second beat, if secondBeat == TRUE
            secondBeat = false;                 // clear secondBeat flag
               for(int i=0; i<=9; i++){         // seed the running total to get a realisitic BPM at startup
                    rate[i] = IBI;                      
                    }
            }
          
    // keep a running total of the last 10 IBI values
    word runningTotal = 0;                   // clear the runningTotal variable    

    for(int i=0; i<=8; i++){                // shift data in the rate array
          rate[i] = rate[i+1];              // and drop the oldest IBI value 
          runningTotal += rate[i];          // add up the 9 oldest IBI values
        }
        
    rate[9] = IBI;                          // add the latest IBI to the rate array
    runningTotal += rate[9];                // add the latest IBI to runningTotal
    runningTotal /= 10;                     // average the last 10 IBI values 
    BPM = 60000/runningTotal;               // how many beats can fit into a minute? that's BPM!
    QS = true;                              // set Quantified Self flag 
    // QS FLAG IS NOT CLEARED INSIDE THIS ISR
    }                       
}

  if (Signal < thresh && Pulse == true){     // when the values are going down, the beat is over
      digitalWrite(blinkPin,LOW);            // turn off pin 13 LED
      Pulse = false;                         // reset the Pulse flag so we can do it again
      amp = P - T;                           // get amplitude of the pulse wave
      thresh = amp/2 + T;                    // set thresh at 50% of the amplitude
      P = thresh;                            // reset these for next time
      T = thresh;
     }
  
  if (N > 2500){                             // if 2.5 seconds go by without a beat
      thresh = 512;                          // set thresh default
      P = 512;                               // set P default
      T = 512;                               // set T default
      lastBeatTime = sampleCounter;          // bring the lastBeatTime up to date        
      firstBeat = true;                      // set these to avoid noise
      secondBeat = true;                     // when we get the heartbeat back
     }
  
  sei();                                     // enable interrupts when youre done!
}// end isr
void ledFadeToBeat(){
    fadeRate -= 15;                         //  set LED fade value
    fadeRate = constrain(fadeRate,0,255);   //  keep LED fade value from going into negative numbers!
    analogWrite(fadePin,fadeRate);          //  fade LED
  }
void sendDataToProcessing(char symbol, int data ){
    Serial.print(symbol);                // symbol prefix tells Processing what type of data is coming
    Serial.println(data);                // the data to send culminating in a carriage return
}
void mostra_endereco_sensor(DeviceAddress deviceAddress)
{
  for (uint8_t i = 0; i < 8; i++)
  {
    // Adiciona zeros se necessário
    if (deviceAddress[i] < 16) Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}
float MQResistanceCalculation(int raw_adc)
{
return ( ((float)RL_VALUE*(1023-raw_adc)/raw_adc));
}
float MQCalibration(int mq_pin)
{
int i;
float val=0;
for (i=0;i<CALIBARAION_SAMPLE_TIMES;i++) { //take multiple samples
val += MQResistanceCalculation(analogRead(mq_pin));
delay(CALIBRATION_SAMPLE_INTERVAL);
}
val = val/CALIBARAION_SAMPLE_TIMES; //calculate the average value
val = val/RO_CLEAN_AIR_FACTOR; //divided by RO_CLEAN_AIR_FACTOR yields the Ro
//according to the chart in the datasheet
return val;
}

float MQRead(int mq_pin)
{
int i;
float rs=0;
for (i=0;i<READ_SAMPLE_TIMES;i++) {
rs += MQResistanceCalculation(analogRead(mq_pin));
delay(READ_SAMPLE_INTERVAL);
}
rs = rs/READ_SAMPLE_TIMES;
return rs;
}
int MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
return (pow(10,( ((log(rs_ro_ratio)-pcurve[1])/pcurve[2]) + pcurve[0])));
}
int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
if ( gas_id == GAS_LPG ) {
return MQGetPercentage(rs_ro_ratio,LPGCurve);
} else if ( gas_id == GAS_CO ) {
return MQGetPercentage(rs_ro_ratio,COCurve);
} else if ( gas_id == GAS_SMOKE ) {
return MQGetPercentage(rs_ro_ratio,SmokeCurve);
}
return 0;
}
void setup() {
  pinMode(blinkPin,OUTPUT);         // pin that will blink to your heartbeat!
  pinMode(fadePin,OUTPUT);          // pin that will fade to your heartbeat!
  interruptSetup();                 // sets up to read Pulse Sensor signal ever
  Serial.begin(115200);
  swSer.begin(115200);
  Ro = MQCalibration(MQ_PIN);
  sensors.begin();
  Serial.print(sensors.getDeviceCount(), DEC);
  if (!sensors.getAddress(sensor1, 0)) 
     Serial.println("Sensores nao encontrados !"); 
  // Mostra o endereco do sensor encontrado no barramento
  Serial.print("Endereco sensor: ");
  mostra_endereco_sensor(sensor1);
}

void loop() {
TIM2_DIS();
int chk = DHT.read11(DHT11_PIN);
String degerler ="";
degerler += "a";
degerler += (DHT.humidity);
degerler += "*";
Serial.println(degerler);
swSer.println(degerler);
delay(5);
degerler = "";
degerler += "b";
degerler += (DHT.temperature);
degerler += "*";
Serial.println(degerler);
swSer.println(degerler);
delay(5);
degerler = "";
degerler += "c";
sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensor1);
  // Atualiza temperaturas minima e maxima
degerler += (tempC); 
degerler += "*";
Serial.println(degerler);
swSer.println(degerler);
delay(5);
TIM2_EN();
degerler = "";
degerler += "d";
degerler += (MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_CO));
degerler += "*";
Serial.println(degerler);
swSer.println(degerler);
delay(5);
degerler = "";
degerler += "e";
degerler += (MQGetGasPercentage(MQRead(MQ_PIN)/Ro,GAS_SMOKE));
degerler += "*";
Serial.println(degerler);
swSer.println(degerler);
delay(5);
int a = 0;
degerler = "";
degerler += "f";
if (BPM > 0)
{
  a = BPM;
  }
 if (QS == true){                       // Quantified Self flag is true when arduino finds a heartbeat
        fadeRate = 255;                  // Set 'fadeRate' Variable to 255 to fade LED with pulse
        degerler += (BPM);   // send heart rate with a 'B' prefix
        QS = false;                      // reset the Quantified Self flag for next time    
     }
else {
  degerler += a;
      }
  ledFadeToBeat();
degerler += "*";
Serial.println(degerler);
swSer.println(degerler);
delay(5);
}

