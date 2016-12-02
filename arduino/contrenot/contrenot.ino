#include <Adafruit_ADS1015.h>
#include <TimerOne.h>

#define FSR_PIN 0
//#define ROTPINA 7
#define ROTPINA 2

const int SENSOR_PIN = 0;
int fsr_prev = 0;
int16_t sp_prev = 0;
int rot_prev = -1;
    
int fsr_val = 0;
int16_t sp_val = 0;
int rot_val = 0;
int count = 0;
int outcount = 0;
int tog = 0;
unsigned long t1 = 0;
unsigned long t2 = 0;
int mode = 0;

Adafruit_ADS1115 ads1115; 

void do_encoder()
{
    rot_val = digitalRead(ROTPINA);
    if(rot_val != rot_prev) {
        outcount = count & 127;
        count = 0;
    }
    rot_prev = rot_val;
    //do_stuff = 1;
}

void send_message(int16_t sp, int fsr, int rot, int vel)
{
    uint8_t b[5];
    
    b[0] = 128 + (sp >> 9);
    b[1] = (sp >> 2) & 127;
    b[2] = ((sp << 5) & 127) + (fsr >> 5);
    b[3] = ((fsr << 2) & 127)  + rot;
    b[4] = (vel & 127);
    Serial.write(b, 5);
    //Serial.print(b[0]);
    //Serial.print(b[1]);
    //Serial.print(b[2]);
    //Serial.print(b[3]);
}

void setup()
{
    Serial.begin(115200);
    //Serial.begin(230400);
    //Serial.begin(9600);
    ads1115.begin();
    pinMode(ROTPINA, INPUT);
    /* turn on pull-up resistor */


    digitalWrite(ROTPINA, HIGH);
    attachInterrupt(0, do_encoder, CHANGE);


    //Timer1.initialize(150000); //0.15
    Timer1.initialize(200); //0.0002
    Timer1.attachInterrupt(run_counter); 
}

void loop()
{
    unsigned long diff;
    fsr_val = analogRead(SENSOR_PIN);
    sp_val = ads1115.readADC_SingleEnded(0);
    //rot_val = digitalRead(ROTPINA);

    if(rot_val == HIGH) rot_val = 1;
    else rot_val = 0;

    //if(rot_val != rot_prev) {
    //    outcount = count;
    //    count = 0;
    //}
   
    //if(abs(sp_val - sp_prev) > 9000) sp_val = sp_prev; 

    if(sp_val < 0) sp_val = sp_prev;

    //if((fsr_val != fsr_prev)
    //    || (sp_val != sp_prev)
    //    || (rot_val != rot_prev))
    if(1)
    {
        //Serial.print(fsr_val);
        //Serial.print(" ");
        //Serial.print(sp_val);
        //Serial.print(" ");
        //Serial.println(rot_val);
        send_message(sp_val, fsr_val, rot_val, outcount);
    }

    fsr_prev = fsr_val;
    sp_prev = sp_val;
    //rot_prev = rot_val;
    delay(1);
}

void run_counter()
{
    count++;
}
