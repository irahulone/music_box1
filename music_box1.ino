
int ldrPin = A3;
int motorPin = 11;
int ledPin = 13;                   
int ledRpin = 10;  // vibrator motor
int inputPin = 2;                

int ldrVal = 0;
int ambMax = 0;
int ldrThL = 5;
int ldrThH = 110;

int playFlag = 0;
int phraseCount = 0;
int phraseCountMaria = 0;
int phraseCountSon = 0;

int motorpwm = 255;
int ledRpwm = 255;

char readChar = ' ';


unsigned long previousMillis = 0;  
unsigned long currentMillis = 0;     
long interval = 600010;           


unsigned long signal_len,t1,t2;   //time for which button is pressed


String code = " ";                 //string in which one alphabet is stored

void setup() {
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP); //internal pullup resistor is used to simplify the circuit
  pinMode(ledPin,OUTPUT);
  pinMode(ledRpin,OUTPUT); digitalWrite(ledRpin,0);
  pinMode(motorPin,OUTPUT); digitalWrite(motorPin,0);


  // wait for the box to open 
  int dv = analogRead(ldrPin);
  while(dv < 20)
  {
    dv = analogRead(ldrPin);
    Serial.println("box closed");
  }

  Serial.println("box opened"); delay(4000);
  
  Serial.println("calibrating light sensor ..."); 

  // set threshold
  long  x = 0;
  for(int i = 0; i <= 100; i++)
  {
    x += analogRead(ldrPin);
    //Serial.println(x);
  }
  x = x/100;
  Serial.println(x);
  ambMax = x + 10;

  int r = map(analogRead(ldrPin), 0, ambMax, 0, 100);
  Serial.println(r);
  Serial.println("... calibration done!");
  

  firstMessage();

 
  
}

/*
int ldrRead()
{
  ldrVal = map(analogRead(ldrPin), 20, 500, 0, 100);
  //Serial.println (ldrVal);
  int x = 1;
  if(ldrVal < 15)
  x = 0;
  return x;
}
*/

void music(int x = 0)
{
  playFlag = 1;
  
  if(x == 0)
  {
    digitalWrite(motorPin,0);
    Serial.println("music stop");  
    playFlag = 0;
  }

  if(x == 1) // mom
  {
    interval = 600010;
    digitalWrite(motorPin,1);
    currentMillis = millis();
    previousMillis = currentMillis;
    Serial.println("music begin");  
  }

  if(x == 2) // maria
  {
    interval = random(4000, 20000);
    motorpwm = 100;
    analogWrite(motorPin,motorpwm);
    currentMillis = millis();
    previousMillis = currentMillis;
    Serial.println("music begin");  
  }
  
}


void loop()
{

  //int r = map(analogRead(ldrPin), 0, ambMax, 0, 100);
  //Serial.println(r);
  //delay(500);

  NextDotDash:
  //while (digitalRead(inputPin) == HIGH) {}
  //while (map(analogRead(ldrPin), 20, 500, 0, 100) > ldrThL || map(analogRead(ldrPin), 20, 500, 0, 100) < ldrThH) {}
  while (map(analogRead(ldrPin), 20, 500, 0, 100) > ldrThL) 
  {
    currentMillis = millis();
    if (currentMillis - previousMillis >= interval) 
    {
      //previousMillis = currentMillis;
      if(playFlag == 1)
      music(0);
    }
  }
  t1 = millis();                            //time at button press
  digitalWrite(ledPin, HIGH);               //LED on while button pressed
  analogWrite(ledRpin,50);
 
  
  //while (digitalRead(inputPin) == LOW) {}
  //while (map(analogRead(ldrPin), 20, 500, 0, 100) <= ldrThL || map(analogRead(ldrPin), 20, 500, 0, 100) > ldrThH) {}
  while (map(analogRead(ldrPin), 20, 500, 0, 100) <= ldrThL) {}
  t2 = millis();                            //time at button release
  digitalWrite(ledPin, LOW);                //LED off on button release
  analogWrite(ledRpin,0);
  signal_len = t2 - t1;                     //time for which button is pressed
  if (signal_len > 50)                      //to account for switch debouncing
  {
    code += readio();                       //function to read dot or dash
  }
  while ((millis() - t2) < 500)           //if time between button press greater than 0.5sec, skip loop and go to next alphabet
  {     
    //if (digitalRead(inputPin) == LOW)
    if (map(analogRead(ldrPin), 20, 500, 0, 100) <= ldrThL)
    {
      goto NextDotDash;
    }
  }
  convertor();                          //function to decipher code into alphabet
  
  

  
}
