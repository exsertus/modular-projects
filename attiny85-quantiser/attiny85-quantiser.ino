/*============================================================
  Name : Quantiser
  Author : exsertus.com (Steve Bowerman)
 
  Summary
  1v/octave 0-10v CV quantizer and scaler

  1. Scale 0-10v CV input to 0-5v (half) with voltage divider for 5v logic proceesing
  2. Output held on clock trigger, trigger pin normalled
  3. Output 0.5v/oct scale to DAC
  4. Scale 0-5v output to 0-10v output (double) via opamp

============================================================*/

#define TRIG PB4
#define NOTE_CV_IN A3

#define DAC_SDI PB0
#define DAC_CS PB1
#define DAC_CLK PB2

#define MAX_NOTES 120
#define MAX_ADC 1023
#define MAX_DAC 4095

void setup() {
  pinMode(DAC_CS, OUTPUT);
  pinMode(DAC_CLK, OUTPUT);
  pinMode(DAC_SDI, OUTPUT);
  pinMode(TRIG, INPUT_PULLUP); // Normal trigger to continuous

  digitalWrite(DAC_CS, HIGH);
}

void loop() {
  if (digitalRead(TRIG) == HIGH) {
    int note_cv_in = analogRead(NOTE_CV_IN);
    int note = map(note_cv_in, 0, MAX_ADC, 0, MAX_NOTES);
    unsigned int dac_val = (unsigned int)map(note, 0, MAX_NOTES, 0, MAX_DAC);

    digitalWrite(DAC_CS, LOW);
    shiftOut(DAC_SDI, DAC_CLK, MSBFIRST, 0 << 7 | 0 << 6 | 1 << 5 | 1 << 4 | (dac_val & 0xF00) >> 8);
    shiftOut(DAC_SDI, DAC_CLK, MSBFIRST, dac_val & 0xFF);
    digitalWrite(DAC_CS, HIGH);
  }
}
