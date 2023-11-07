int tmpPWM_Out = 64;
int PWM_Out;
int Direction = 1;

int InHigh = 255;
int InLow = 0;

int OutputHigh;
int OutputLow;

// in - outputs
int StepPin = 7;
int DirPin = 8;
int Mode = 3;
int OnOff = 2;

int ScaleOn = 5;
int ZeroOff = 6;

int AnalogLow = A0;           // potentiometer connected to analog pin 0  laser min 
int AnalogHigh = A1;          // potentiometer connected to analog pin 1  laser max

int Laser = 9;

void setup()
{
  pinMode(DirPin, INPUT_PULLUP);
  pinMode(StepPin, INPUT_PULLUP);
  pinMode(Mode, INPUT_PULLUP);
  pinMode(OnOff, INPUT_PULLUP);

  pinMode(ScaleOn, INPUT_PULLUP);

  analogWriteResolution(8);
  pinMode(Laser, OUTPUT);

  // interrupts on de Step an Dir inputs
  // try to make it as fast as posible, because we don't want to miss steps
  // maybe the Dir input on a normal input is Better?
  attachInterrupt(digitalPinToInterrupt(StepPin), Count, RISING);
  attachInterrupt(digitalPinToInterrupt(DirPin), ChangeDir, CHANGE);

  if (digitalRead(DirPin))  // Dir input
  {
    Direction = -1;
  }

}

void loop() {
  if (digitalRead(Mode) == HIGH)                                                                // on/off mode on

    // if the On/Off mode is active, we are not sending step and Dir pulses
    // so Count and ChangeDir are never called
  {
    if (digitalRead(OnOff) == HIGH)                                                             // on off input
    {
      tmpPWM_Out = 255;
    }
    else
    {
      tmpPWM_Out = 0;
    }
  }

  if (digitalRead(ScaleOn) == HIGH)                                                             // Scale fubction on
  {
    OutputLow = analogRead(AnalogLow) / 4;
    OutputHigh = analogRead(AnalogHigh) / 4;
    PWM_Out = map(tmpPWM_Out, 0, 255, OutputLow, OutputHigh);                                   // scale wanted value between 2 potmeter setpoints..

    if (digitalRead(ZeroOff) == HIGH)
    {
      if ( tmpPWM_Out == 0)
      {
        PWM_Out = 0;
      }
    }
  }
  else                                                                                          // Scale fubction off
  {
    PWM_Out = tmpPWM_Out;
  }
  analogWrite(Laser, PWM_Out);                                                                  // real output to laser
}

void Count()
{
  tmpPWM_Out = tmpPWM_Out + Direction;                                                                // step pulse input on the rising edges Interrupt
  if (tmpPWM_Out > 255)
  {
    tmpPWM_Out = 255;
  }
  if (tmpPWM_Out < 0)
  {
    tmpPWM_Out = 0;
  }
}

void ChangeDir()
{
  Direction = 1;                                                                              // Dir input on a change interrupt
  if (digitalRead(DirPin))
  {
    Direction = -1;
  }
}
