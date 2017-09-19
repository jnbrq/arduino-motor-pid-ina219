#include <Wire.h>       // for INA219
#include <INA219.h>

#define MEASUREMENT_PIN     2
#define MOTOR_PIN           9
#define BAUD_RATE           115200

class PID {
public:
    float ki, kp, kd;
    PID() {
        first_ = true;
        ki = 0.1f;
        kp = 0;
        kd = 0;
    }
    
    float Set(float kp_, float ki_, float kd_) {
        ki = ki_;
        kp = kp_;
        kd = kd_;
    }
    
    void Zero() {
        first_ = true;
    }
    
    float Calc(float e)
    {
        if (first_)
        {
            i_ = 0;
            old_ = e;
            first_ = false;
            return kp * e;
        }
        else 
        {
            float d = (e-old_);
            i_ += e;
            i_ = constrain(i_, -40000, 40000);
            old_ = e;
            return ki * i_ + kd * d + kp * e;
        }
    }
private:
    float i_;
    float old_;
    bool first_;
};

INA219 ina;
PID pid;
float desiredFreq = 300.0f;
float freq;
float error;
float PWM = 127.0f;
long t = 0;

String s1,s2,s3,s4;
int c;

void setup()
{
    Serial.begin(BAUD_RATE);
    pinMode(MEASUREMENT_PIN, INPUT);
    pinMode(MOTOR_PIN, OUTPUT);
    attachInterrupt(
        digitalPinToInterrupt(MEASUREMENT_PIN),
        freq_interrupt,
        RISING);

    /* calibrate INA219 */
    ina.begin();
    ina.configure(
        INA219_RANGE_32V,
        INA219_GAIN_320MV,
        INA219_BUS_RES_12BIT,
        INA219_SHUNT_RES_12BIT_1S);
    ina.calibrate(0.01, 0.5);
  
    analogWrite(MOTOR_PIN, PWM);
    delay(3000);
}

void loop() 
{
    if (Serial.available() > 0)
    {
        c = Serial.read();
        if (c == 'X')
        {
            s1 = Serial.readStringUntil(',');
            s2 = Serial.readStringUntil(',');
            s3 = Serial.readStringUntil(',');
            s4 = Serial.readStringUntil('\n');
            pid.Set(s1.toFloat(), s2.toFloat(), s3.toFloat());
            desiredFreq = s4.toFloat();
        }
        else if (c == 'R')
        {
            pid.Zero();
            PWM = 127.0f;
            analogWrite(MOTOR_PIN, PWM);
            delay(3000);
        }
    }

    error =    desiredFreq - freq;
    PWM = pid.Calc(error);
    PWM = constrain(PWM, 0, 255);
    
    Serial.print(micros()); Serial.print(",");  /* print current time */
    Serial.print(error); Serial.print(",");     /* print error e(t) */
    Serial.print(ina.readShuntCurrent()); Serial.print(",");
        /* print current */
    Serial.print(freq); Serial.print(",");      /* print current freq */
    Serial.print(PWM); Serial.println();        /* print PWM and end with \m */
    
    analogWrite(MOTOR_PIN, PWM);
    
    delay(25);
}

void freq_interrupt() {
    if (t == 0) {
        t = micros();
    }
    else {
        long t2 = micros();
        freq = 1.0E6/(t2-t);
        t = t2;
    }
}

