#include "FastLED.h"
//Pins setup

//Arms
#define numNeoPixelsArm 8
//Left Arm
#define PINLEFTARM 3
//Right Arm
#define PINRIGHTARM 4

//Back
//For the left and side stripe
#define numNeoPixelsSideBack 36

//Left side
#define PINLEFTBACK 5

//Right side
#define PINRIGHTBACK 6

//Middle: the main one
#define numNeoPixelsMainBack 51
#define PINMAINBACK 7

//Front
//Up
#define PINUPFRONT 2
#define numNeoPixelsUpFront 9
//Middle
#define PINMIDDLEFRONT 10
#define numNeoPixelsMiddleFront 5
//Down
#define PINDOWNFRONT 9
#define numNeoPixelsDownFront 5


//Define the array of leds for each stripe
//Left Arm
CRGB stripLeftArm[numNeoPixelsArm];
int hueLeftArm = 140; //HSV
int satLeftArm = 75;
int valLeftArm = 255;

//Right arm
CRGB stripRightArm[numNeoPixelsArm];
int hueRightArm = hueLeftArm; //HSV
int satRightArm = satLeftArm;
int valRightArm = valLeftArm;

//Back
//Left side
CRGB stripLeftBack[numNeoPixelsSideBack];
int hueLeftBack = 40; //HSV
int satLeftBack = 220;
int valLeftBack = 200;

//Right side
CRGB stripRightBack[numNeoPixelsSideBack];
int hueRightBack = hueLeftBack; //HSV
int satRightBack = satLeftBack;
int valRightBack = valLeftBack;

//Middle
CRGB stripMainBack[numNeoPixelsMainBack];
int hueMainBack = hueLeftBack; //HSV
int satMainBack = satLeftBack;
int valMainBack = valLeftBack;

//Front
//Up
CRGB stripUpFront[numNeoPixelsUpFront];
int hueUpFront = 40; //HSV
int satUpFront = 220;
int valUpFront = 200;

//Middle
CRGB stripMiddleFront[numNeoPixelsMiddleFront];
int hueMiddleFront = hueUpFront; //HSV
int satMiddleFront = satUpFront;
int valMiddleFront = valUpFront;

//Down
CRGB stripDownFront[numNeoPixelsDownFront];
int hueDownFront = hueUpFront; //HSV
int satDownFront = satUpFront;
int valDownFront = valUpFront;

//Variables for all the accellerometers
int num = 20; //Iterations to smooForBackth the raw values from the acc

int startTimeCount = 0; //Tracks when a new cycle starts
unsigned long  timeCount = 50;  //Tracks how many milliseconds have passed in total.
unsigned long  timePlus = 50; //Defines how many milliseconds lasts a cycle
unsigned long timeCycle = 0;

//Arms
int diffAccArm;
//Left
int diffAccArmLeft;
//Right
int diffAccArmRight;

//Arms
int accSmooForArm = 0;
float smooForArm = 0;
float counterForArm = 0.0;
float counterAdderForArm = 1.0;

//Back
int accSmooForBack = 0;
float smooForBack = 0;
float counterForBack = 0.0; //aux value to to show the chasing lighting efecct.
float counterAdderForBack = 1.0;


//Variables that both accelerometers on the arms use
//For both X Axis on the arms

int minReadingXArm = 220; //290 //218
int maxReadingXArm = 255; //405 //255

int minAccArm = 0; //The minimum value that abs(endCycleValue - initCycleValue) may give
int maxAccArm = 200; //The maximum value that abs(endCycleValue - initCycleValue) may give

int mapMinAccArm = 0; //The minimum value that accBack may have at the end
int mapMaxAccArm = 300; //The maximum value that accBack may have at the end

float animationSpeedArm = 0.8;

//For both Y Axis on the arms

int minReadingYArm = 180; //273;
int maxReadingYArm = 220; //420

int mapMinHeightArm = 0;
int mapMaxHeightArm = 30;

// Controlling the Hue

int hueRange = 40;
float hueChange, helpHueChange = 0;

//For the X on the back

int minReadingXBack = 180;
int maxReadingXBack = 220; //220

int minAccBack = 0; //The minimum value that abs(endCycleValue - initCycleValue) may give
int maxAccBack = 100; //The maximum value that abs(endCycleValue - initCycleValue) may give

int mapMinAccBack = 0; //The minimum value that accBack may have at the end
int mapMaxAccBack = 10; //The maximum value that accBack may have at the end

//For the accelerometer on the LEFT Arm. All the variables end with the word "Left"

//X Axis
const int xpinLeft = A0;  //x axis of

int analogValueXLeft = 0; //has the average value of the axis after a "num" number of iterations

int valueXStartLeft = 0; //Tracks the value in the start of every new cycle

int accLeft = 0; //The final value that it is passed to the NeoPixel Function.

//Y Axis
const int ypinLeft = A1;  //y axis of the acc

int analogValueYLeft = 0;

//For the accelerometer on the RIGHT Arm. All the variables end with the word "Right"

//X Axis
const int xpinRight = A2;  //x axis of

int analogValueXRight = 0; //has the average value of the axis after a "num" number of iterations

int valueXStartRight = 0; //Tracks the value in the start of every new cycle

int accRight = 0; //The final value that it is passed to the NeoPixel Function.
//Y Axis
const int ypinRight = A3;  //y axis of the acc

int analogValueYRight = 0;

//For the accelerometer on the Back; All the variables end with the word "Back"
const int xpinBack = A4;  //x axis of the acc located on the back

int analogValueXBack = 0; //has the average value of the axis after a "num" number of iterations

int valueXStartBack = 0; //Tracks the value in the start of every new cycle

int accBack = 0; //The final value that it is passed to the NeoPixel Function.

//Variables for Effects

boolean flickering = true;
float flickerSize = 30; // min 30, max 255

//Variables for press button
float overallBrightness = 255;
float brightnessChangeSpeed = 4;
int maxBrightness = 255;
int minBrightness = 30;

int buttonPin = 11;
int buttonValue;
boolean buttonPressed = false;
boolean onOff = true;

//Variables for the presentation

int mode = 0;
int numModes = 1;
float hue1, hue2, helpHue1, helpHue2;

void setup() {
  // initialize the serial communications:
  Serial.begin(9600);
  switchColors();

  //Button
  pinMode(buttonPin, INPUT_PULLUP);

  //Arms
  FastLED.addLeds<NEOPIXEL, PINLEFTARM>(stripLeftArm, numNeoPixelsArm);
  FastLED.addLeds<NEOPIXEL, PINRIGHTARM>(stripRightArm, numNeoPixelsArm);

  //Back
  FastLED.addLeds<NEOPIXEL, PINLEFTBACK>(stripLeftBack, numNeoPixelsSideBack);
  FastLED.addLeds<NEOPIXEL, PINRIGHTBACK>(stripRightBack, numNeoPixelsSideBack);
  FastLED.addLeds<NEOPIXEL, PINMAINBACK>(stripMainBack, numNeoPixelsMainBack);

  //Front
  FastLED.addLeds<NEOPIXEL, PINUPFRONT>(stripUpFront, numNeoPixelsUpFront);
  FastLED.addLeds<NEOPIXEL, PINMIDDLEFRONT>(stripMiddleFront, numNeoPixelsMiddleFront);
  FastLED.addLeds<NEOPIXEL, PINDOWNFRONT>(stripDownFront, numNeoPixelsDownFront);


}

void loop() {

  //Press Button
  // make use of the button


  buttonValue = digitalRead(buttonPin);

  //Serial.println(buttonValue);

  if (buttonValue == 0 && buttonPressed == false) {
    buttonPressed = true;
    mode = (mode + 1) % numModes;
    switchColors();
  }
  if (buttonValue == 1) {
    buttonPressed = false;
  }

  //depending on the state adjust the brightness quickly
  /*

    if(onOff && overallBrightness<maxBrightness){
      overallBrightness+=brightnessChangeSpeed;
      //strip.setBrightness(overallBrightness); //this should be adjusted to affect the brightnesses
      FastLED.setBrightness(overallBrightness);
    }

    if(!onOff && overallBrightness>minBrightness){
      overallBrightness-=brightnessChangeSpeed;
      FastLED.setBrightness(overallBrightness);
    }
  */
  //Arms
  counterAdderForArm = map(constrain(smooForArm, 0, 200), 0, 200, 800, 3000) / 1000.0;
  counterForArm += counterAdderForArm;
  
  //Back
  counterAdderForBack = map(constrain(smooForBack, 0, 200), 0, 200, 800, 3000) / 1000.0;
  counterForBack += counterAdderForBack;

  //setChasingLight(stripLeftX, numNeoPixelsArm, animationSpeedArm, counterForBack);

  //setChasingLight(stripLeftX, numNeoPixelsBack, animationSpeedArm, counterForBack);
  //setChasingLight(stripRightX, numNeoPixelsBack, animationSpeedArm, counterForBack);


  for (int i = 0; i < num; i++) {

    analogValueXLeft += analogRead(xpinLeft);
    analogValueYLeft += analogRead(ypinLeft);

    analogValueXRight += analogRead(xpinRight);
    analogValueYRight += analogRead(ypinRight);

    analogValueXBack += analogRead(xpinBack);

    delay(0);
  }

  analogValueXLeft = analogValueXLeft / num;
  analogValueYLeft = analogValueYLeft / num;

  analogValueXRight = analogValueXRight / num;
  analogValueYRight = analogValueYRight / num;

  analogValueXBack = analogValueXBack / num;

  if (startTimeCount == 0) {
    valueXStartLeft = analogValueXLeft;

    valueXStartRight = analogValueXRight;

    valueXStartBack = analogValueXBack;
  }

  analogValueXLeft = constrain(analogValueXLeft, minReadingXArm, maxReadingXArm);

  analogValueXRight = constrain(analogValueXRight, minReadingXArm, maxReadingXArm);

  analogValueXBack = constrain(analogValueXBack, minReadingXBack, maxReadingXBack);

  timeCycle = millis();

  //If a certain period of time passes enters the if
  //For the X axis in the left and right arms.
  //For the X axis on the back.
  if (timeCycle >= timeCount) {
    diffAccArmLeft = abs(analogValueXLeft - valueXStartLeft);
    diffAccArmRight = abs(analogValueXRight - valueXStartRight);

    diffAccArm = diffAccArmRight + diffAccArmLeft;
    
    accLeft = diffAccArmLeft;
    accLeft = mapValue(accLeft, minAccArm, maxAccArm, mapMinAccArm, mapMaxAccArm);

    accRight = diffAccArmRight;
    accRight = mapValue(accRight, minAccArm, maxAccArm, mapMinAccArm, mapMaxAccArm);

    accBack = abs(analogValueXBack - valueXStartBack);
    accBack = mapValue(accBack, minAccBack, maxAccBack, mapMinAccBack, mapMaxAccBack);

    //if(accBack <= 60){
    //accBack = 5;
    //}

    //Serial.println(accLeft);

    if (accSmooForBack < diffAccArm) {
      accSmooForBack = diffAccArm;
      accSmooForArm = diffAccArm;
    }

    //Add effect


    //To control the frecuency that it enters the if
    timeCount = timeCount + timePlus;
    startTimeCount = 0;
  }
  else {
    startTimeCount = 1;
  }

  //For the two finns on the Arms
  if (accSmooForArm > 0) {
    accSmooForArm = accSmooForArm - 4.0;
    if (accSmooForArm < 0) {
      accSmooForArm = 0;
    }
  }

  //For the three strips on the Back
  if (accSmooForBack > 0) {
    accSmooForBack = accSmooForBack - 1.5;
    if (accSmooForBack < 0) {
      accSmooForBack = 0;
    }
  }
  // We want a really smooForBackth value 
  //Arms
  smooForArm = smooForArm + (accSmooForArm - smooForArm) * 0.09;

  //Back
  smooForBack = smooForBack + (accSmooForBack - smooForBack) * 0.09;
  
  // Serial.println(smooForBack);

  //CONTROL THE COLORS AND OTHER VISUAL STUFF

  flickerSize = map(constrain(smooForBack, 20, 200), 20, 200, 40, 160);

  switch (mode) {
    case 0:
      hueRange = 255;
      break;
    default:
      hueRange = 180;
      break;
  }


  analogValueYLeft = mapValue(analogValueYLeft, minReadingYArm, maxReadingYArm, mapMinHeightArm, mapMaxHeightArm);
  analogValueYRight = mapValue(analogValueYRight, minReadingYArm, maxReadingYArm, mapMinHeightArm, mapMaxHeightArm);

  helpHueChange = map(max(analogValueYLeft, analogValueYRight), mapMinHeightArm, mapMaxHeightArm, 0, hueRange);
  hueChange = hueChange + (helpHueChange - hueChange) * 0.2;

  helpHue1 = map(analogValueYLeft, mapMinHeightArm, mapMaxHeightArm, 0, 255);
  helpHue2 = map(analogValueYRight, mapMinHeightArm, mapMaxHeightArm, 0, 255);
  hue1 = hue1 + (helpHue1 - hue1) * 0.2;
  hue2 = hue2 + (helpHue2 - hue2) * 0.2;


  // Serial.print(analogValueYLeft);
  // Serial.print(",");
  // Serial.println(analogValueYRight);

  //Add effect
 
  //Left Arm
  for (int i = 0; i < numNeoPixelsArm ; i++) {
    float sine = sin((i * TWO_PI / numNeoPixelsArm) + counterForArm * animationSpeedArm);
    float phase = map(sine * 1000, -1000, 1000, 0, 1000) / 1000.0;
    switch (mode) {
      case 0:
        stripLeftArm[i] = CHSV(hueLeftArm, satLeftArm, valLeftArm * phase);
        break;
      default:
        stripLeftArm[i] = CHSV(int(i * (255 / 8)), 255, valLeftArm * phase);
        break;
    }
  }

  //Right arm X
  for (int i = 0; i < numNeoPixelsArm ; i++) {
    float sine = sin(((numNeoPixelsArm - i - 1) * TWO_PI / numNeoPixelsArm) + counterForArm * animationSpeedArm);
    float phase = map(sine * 1000, -1000, 1000, 0, 1000) / 1000.0;
    switch (mode) {
      case 0:
        stripRightArm[i] = CHSV(hueRightArm, satRightArm, valRightArm * phase);
        break;
      default:
        stripRightArm[i] = CHSV(int(i * (255 / 8)), 255, valRightArm * phase);
        break;
    }
  }



  //RightBack
  for (int i = 0; i < numNeoPixelsSideBack ; i++) {
    switch (mode) {
      case 0:
        stripRightBack[i] = CHSV(int(hueRightBack + hueChange + (sin(-counterForBack * 0.3 + i * PI / 6) + 1) * flickerSize / 3) % 255, satRightBack, valRightBack - (sin(-counterForBack * 0.3 + i * PI / 6) + 1) * flickerSize);
        break;
      default:
        stripRightBack[i] = CHSV(0, 0, 20 + hueChange);
        break;
    }
  }


  //LeftBack
  for (int i = 0; i < numNeoPixelsSideBack ; i++) {
    switch (mode) {
      case 0:
        stripLeftBack[i] = CHSV(int(hueLeftBack + hueChange + (sin(-counterForBack * 0.3 + i * PI / 6) + 1) * flickerSize / 3) % 255, satMainBack, valLeftBack - (sin(-counterForBack * 0.3 + i * PI / 6) + 1) * flickerSize);
        break;
      default:
        stripLeftBack[i] = CHSV(0, 0, 20 + hueChange);
        break;
    }
  }


  //MainBack
  for (int i = 0; i < numNeoPixelsMainBack ; i++) {
    switch (mode) {
      case 0:
        stripMainBack[i] = CHSV(int(hueMainBack + hueChange + (sin(-counterForBack * 0.3 + i * PI / 6) + 1) * flickerSize / 3) % 255, satMainBack, valMainBack - (sin(-counterForBack * 0.3 + i * PI / 4) + 1) * flickerSize);
        break;
      default:
        stripMainBack[i] = CHSV(0, 0, 20 + hueChange);
        break;
    }
  }
  
  //Front
  for (int i = 0; i < numNeoPixelsUpFront ; i++) {
    switch (mode) {
      case 0:
        stripUpFront[i] = CHSV(int(hueUpFront + hueChange) % 255, satUpFront, valUpFront);
        break;
      default:
        stripUpFront[i] = CHSV(0, 0, 255);
        break;
    }
  }

  //Middle
  for (int i = 0; i < numNeoPixelsMiddleFront ; i++) {
    switch (mode) {
      case 0:
        stripMiddleFront[i] = CHSV(int(hueMiddleFront + hueChange) % 255, satMiddleFront, valMiddleFront);
        break;
      default:
        stripMiddleFront[i] = CHSV(0, 0, 255);
        break;
    }
  }

  //Down
  for (int i = 0; i < numNeoPixelsDownFront ; i++) {
    switch (mode) {
      case 0:
        stripDownFront[i] = CHSV(int(hueDownFront + hueChange) % 255, satDownFront, valDownFront);
        break;
      default:
        stripDownFront[i] = CHSV(0, 0, 255);
        break;
    }
  }

  //Reinitilize values
  //Left arm
  analogValueXLeft = 0;
  analogValueYLeft = 0;

  //Right arm
  analogValueXRight = 0;
  analogValueYRight = 0;

  //Back
  analogValueXBack = 0;

  //Update leds
  FastLED.show();

}

void switchColors() {

  switch (mode) {
    case 0:

      hueLeftArm = 255 / 6 * 5.2; //HSV
      satLeftArm = 60;
      valLeftArm = 255;

      hueRightArm = hueLeftArm; //HSV
      satRightArm = satLeftArm;
      valRightArm = valLeftArm;

      //REST

      hueLeftBack = 0; //HSV
      satLeftBack = 200;
      valLeftBack = 200;

      hueRightBack = hueLeftBack; //HSV
      satRightBack = satLeftBack;
      valRightBack = valLeftBack;

      hueMainBack = hueLeftBack; //HSV
      satMainBack = satLeftBack;
      valMainBack = valLeftBack;

      hueUpFront = hueLeftBack; //HSV
      satUpFront = satLeftBack;
      valUpFront = 200;

      hueMiddleFront = hueUpFront; //HSV
      satMiddleFront = satUpFront;
      valMiddleFront = valUpFront;

      hueDownFront = hueUpFront; //HSV
      satDownFront = satUpFront;
      valDownFront = valUpFront;
      break;
      
    default:
      hueLeftArm = 140; //HSV
      satLeftArm = 25;
      valLeftArm = 255;

      hueRightArm = hueLeftArm; //HSV
      satRightArm = satLeftArm;
      valRightArm = valLeftArm;

      //REST

      hueLeftBack = 255 / 6 * 5.3; //HSV
      satLeftBack = 200;
      valLeftBack = 200;

      hueRightBack = hueLeftBack; //HSV
      satRightBack = satLeftBack;
      valRightBack = valLeftBack;

      hueMainBack = hueLeftBack; //HSV
      satMainBack = satLeftBack;
      valMainBack = valLeftBack;

      hueUpFront = hueLeftBack; //HSV
      satUpFront = satLeftBack;
      valUpFront = 200;

      hueMiddleFront = hueUpFront; //HSV
      satMiddleFront = satUpFront;
      valMiddleFront = valUpFront;

      hueDownFront = hueUpFront; //HSV
      satDownFront = satUpFront;
      valDownFront = valUpFront;
      break;

  }


}

int mapValue(int value, int minReadingValue, int maxReadingValue, int minMapValue, int maxMapValue) {

  //Serial.print("Original Value = " );
  //Serial.print(value);

  int auxValue = 0;
  int valueOut = 0;

  auxValue = value;
  auxValue = constrain(auxValue, minReadingValue, maxReadingValue);

  //Serial.print(" analogValue = " );
  //Serial.print(auxValue);

  valueOut = map(auxValue, minReadingValue, maxReadingValue, minMapValue, maxMapValue);
  valueOut = constrain(valueOut, minMapValue, maxMapValue);

  //Serial.print(" scaled to " );
  //Serial.print(valueOut, DEC);
  //Serial.println();

  return valueOut;
}

int lerp(float in1, float in2, float percent) {
  return (1 - percent) * in1 + percent * in2;

}
