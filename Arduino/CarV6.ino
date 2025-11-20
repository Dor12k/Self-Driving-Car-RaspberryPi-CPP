
// Left side motor
const int EnableL = 5;
const int HighL = 6;     
const int LowL = 7;

// Right side motor
const int HighR = 8;     
const int LowR = 9;
const int EnableR = 10;

// Define raspberry pi
const int D0 = 0;       //Raspberry pin 21    LSB
const int D1 = 1;       //Raspberry pin 22
const int D2 = 2;       //Raspberry pin 23
const int D3 = 3;       //Raspberry pin 24    MSB

const int speed = 200;

int a, b, c, d, data;

void setup() {
  
  // Set the left digital pins as output
  pinMode(EnableL, OUTPUT);
  pinMode(HighL, OUTPUT);
  pinMode(LowL, OUTPUT);
  
  // Set the right digital pins as output
  pinMode(EnableR, OUTPUT);
  pinMode(HighR, OUTPUT);
  pinMode(LowR, OUTPUT);

  // Set the Raspberry input data pin
  pinMode(D0, INPUT_PULLUP);
  pinMode(D1, INPUT_PULLUP);
  pinMode(D2, INPUT_PULLUP);
  pinMode(D3, INPUT_PULLUP);
}

// Function get data from raspberry pi
void Data()
{
   a = digitalRead(D0);
   b = digitalRead(D1);
   c = digitalRead(D2);
   d = digitalRead(D3);

   // Binary value
   data = 8*d+4*c+2*b+1*a;
}

// Function stop car movement
void Stop()
{
  
  // Turn off left motor
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, LOW);
  analogWrite(EnableL, 0);

  // Turn off right motor
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, LOW);
  analogWrite(EnableR, 0);
}

// Function move car backward
void Backward()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, speed-55); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, speed-55); // Set speed to 255
  //delay(1000);
}

// Function move car backward
void Backward_left1()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, 255); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, 155); // Set speed to 255
}

// Function move car backward
void Backward_left2()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, 255); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, 105); // Set speed to 255
}

// Function move car backward
void Backward_left3()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, 255); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, 55); // Set speed to 255
}

// Function move car backward
void Backward_right1()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, 155); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, 255); // Set speed to 255
}

// Function move car backward
void Backward_right2()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, 105); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, 255); // Set speed to 255
}

// Function move car backward
void Backward_right3()
{
  // Set values for left motor 
  digitalWrite(HighL, HIGH);  // Set digital pin 6 off
  digitalWrite(LowL, LOW); // Set digital pin 7 on  
  analogWrite(EnableL, 55); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, HIGH); // Set digital pin 8 off
  digitalWrite(LowR, LOW); // Set digital pin 9 on
  analogWrite(EnableR, 255); // Set speed to 255
}

// Function move car forward
void Forward()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed-55); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed-55); // Set speed to 255
  //delay(1000);
}

// Function move car left
void Forward_left1()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed-100); // Set speed to 155

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed-35); // Set speed to 255
  //delay(1000);
}

void Forward_left2()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed-120); // Set speed to 100

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed+55); // Set speed to 255
  //delay(1000);
}

void Forward_left3()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed-170); // Set speed to 100

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed+55); // Set speed to 255
  //delay(1000);
}

// Function move car right
void Forward_right1()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed-35); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed-100); // Set speed to 100
  //delay(1000);
}

void Forward_right2()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed+55); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed-120); // Set speed to 100
  //delay(1000);
}

void Forward_right3()
{
  // Set values for left motor 
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed+55); // Set speed to 255

  // Set values for right motor
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed-170); // Set speed to 100
  //delay(1000);
}

// Function make car uturn
void UTurn_left(){

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // curve right
  digitalWrite(HighL, LOW);  // Set digital pin 6 off
  digitalWrite(LowL, HIGH); // Set digital pin 7 on  
  analogWrite(EnableL, speed+55); // Set speed to 255
  digitalWrite(HighR, LOW); // Set digital pin 8 off
  digitalWrite(LowR, HIGH); // Set digital pin 9 on
  analogWrite(EnableR, speed-120); // Set speed to 100
  delay(400);
  
  // -----------------------------------------------------------------------------

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(1500);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Turn left
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(1100); // 1100

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(1000);
  
  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(500);
}


// Function make car uturn
void UTurn_right(){

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Forward
  analogWrite(EnableL, 250);
  analogWrite(EnableR, 250);
  delay(1000);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move right
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 0);
  delay(700);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(900);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move right
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 0);
  delay(900);
  
  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(3000);
}

// Function make obstacle movement
void Obstacle(){


  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move left
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(500);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(850); //750
  
  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Right
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(500);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);
  
  // Move Forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(1600);

  // --------------------------- half ---------------


  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Right
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, HIGH);
  digitalWrite(LowR, LOW);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(250);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(950); // 800
  
  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move left
  digitalWrite(HighL, HIGH);
  digitalWrite(LowL, LOW);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 255);
  analogWrite(EnableR, 255);
  delay(400);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(400);

  // Move Forward
  digitalWrite(HighL, LOW);
  digitalWrite(LowL, HIGH);
  digitalWrite(HighR, LOW);
  digitalWrite(LowR, HIGH);
  analogWrite(EnableL, 150);
  analogWrite(EnableR, 150);
  delay(250);

  // Stop driving
  analogWrite(EnableL, 0);
  analogWrite(EnableR, 0);
  delay(500);
}

void loop() {

  // Translate the data from Raspberry Pi
  Data();

  if(data==0){
    Stop();
  }
  else if(data==1){
    Backward();
  }   
  else if(data==2){
    Backward_left1();
  }   
  else if(data==3){
    Backward_left2();
  }
  else if(data==4){
    Backward_left3();
  }
  else if(data==5){
    Backward_right1();
  }  
  else if(data==6){
    Backward_right2();
  }
  else if(data == 7){
    Obstacle();
  }
  else if(data == 8){
    Forward();
  }
  else if(data == 9){
    Forward_left1();
  }
  else if(data == 10){
    Forward_left2();
  }
  else if(data == 11){
    Forward_left3();
  }
  else if(data == 12){
    Forward_right1();
  }
  else if(data == 13){
    Forward_right2();
  }
  else if(data == 14){
    Forward_right3();
  }
  else if(data == 15){
    UTurn_left();
  }
  else if(data == 16){
    // UTurn left
    UTurn_right();
  }
}
