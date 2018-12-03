#include <mq3.h>

const int sensorIn = A0;
const int SpeakerOut = A1;
const int ledGreen = 12;
const int ledRed = 11;
const int ledBlue = 10;

//                   So   La              Do  Mi
const int notes[] = {392,440,392,440,392,523,659,523,392,440,392,440,392,523,659,523
                    ,698,659,587,587,659,587,523,523,494,494,440,494,523,587,659,659
                    ,698,659,587,587,659,587,523,523,494,494,440,494,523,523,523,523};
const int dur[] =   {2,  2,  2,  2,  3,  1,  2,  2,  2,  2,  2,  2,  3,  1,  2,  2
                    ,3,  1,  2,  2,  3,  1,  2,  2,  3,  1,  2,  2,  3,  1,  2,  2
                    ,3,  1,  2,  2,  3,  1,  2,  2,  3,  1,  2,  2,  3,  1,  2,  2};

float getInputVoltage() {
    float sensor_volt;
    float sensorValue;
    int x = 0;

    /*--- Get a average data by testing 100 times ---*/
    for(x = 0; x < 100 ; ++x) {
        sensorValue = sensorValue + analogRead(sensorIn);
    }
    sensorValue = sensorValue / 100.0;
    /*-----------------------------------------------*/

    sensor_volt = sensorValue / 1024.0 * 5.0;

    return sensor_volt;
}

void(*resetFunc)(void) = 0;

float getInputVoltage1() {
  static float i = 0;
  i = i + 0.1;
  if (i > 5.0) {
    i = 0.0;
  }
  return i;
}

void OkSound() {
  tone(SpeakerOut, 440);
  delay(500);
  noTone(SpeakerOut);
  delay(50);
  tone(SpeakerOut, 523);
  delay(200);
  noTone(SpeakerOut);
  delay(50);
  tone(SpeakerOut, 659);
  delay(500);
  noTone(SpeakerOut);
}

void Kolenda() {
  int arrSize = sizeof(notes) / 2;
  //Serial.println(arrSize);
  for (int i = 0; i < arrSize; ++i) {
    tone(SpeakerOut, notes[i]);
    delay(200 * dur[i]);
    noTone(SpeakerOut);
  }
  noTone(SpeakerOut);
}

void SetRed() {
  digitalWrite(ledRed, HIGH);
  digitalWrite(ledGreen, LOW);  
}

void SetGreen() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, HIGH);  
}

void SetOff() {
  digitalWrite(ledRed, LOW);
  digitalWrite(ledGreen, LOW);
}


void HighLevel() {
  SetRed();
  Kolenda();
}

MQ3 mq3;

void setup() {
  // put your setup code here, to run once:
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  pinMode(ledBlue, OUTPUT);
  pinMode(sensorIn, INPUT);
  pinMode(SpeakerOut, OUTPUT);
  digitalWrite(ledBlue, HIGH);
  //mq3.WarmUp();
  mq3.Calibrate(getInputVoltage());
  Serial.begin(9600);
  SetOff();
  digitalWrite(ledBlue, LOW);
}

float prevVolt = 0;

void loop() {
  // put your main code here, to run repeatedly:
  float volt = getInputVoltage();
  float alc = mq3.GetAlcohol(volt);
  if (abs(volt - prevVolt) > 0.05) {
    if (alc >= 0.2) {
      HighLevel();
    } else {
      SetGreen();
      OkSound();
    }
  } else {
    SetOff();
  }
  prevVolt = volt;
  
  Serial.println(mq3.getR0());
  Serial.print("voltage: ");
  Serial.print(volt);
  Serial.print(", alcohol: ");
  Serial.println(alc, 4);
  delay(1000);
}
