#include <Adafruit_CircuitPlayground.h>;
#include <AsyncDelay.h>;

int playerPos = 0;
int playerGoal = -1;
int playerScore = 0;
int roundCounter = 0;
int timer = 7000;
int score = 0;
const int buttonPin1 = 4;
const int buttonPin2 = 5;
const int switchPin = 7;
bool firstRound = 1;
bool gameFlag = 0;
bool setupBool = 1;
bool switchCheck = 0;
bool timeoutStarted = 0;
volatile bool button1Flag = 0;
volatile bool button2Flag = 0;
volatile bool switchFlag = 0;
AsyncDelay levelTimer;
AsyncDelay SecondTimer;
AsyncDelay startTimer;
AsyncDelay startTimeout;

void setup() {
  // put your setup code here, to run once:
  CircuitPlayground.begin();
  Serial.begin(9600);
  randomSeed(analogRead(0));
  attachInterrupt(digitalPinToInterrupt(buttonPin1), button1Press, RISING);
  attachInterrupt(digitalPinToInterrupt(buttonPin2), button2Press, RISING);
  attachInterrupt(digitalPinToInterrupt(switchPin), switchFlip, CHANGE);
}

int findAngleLED() {
  // Variables for Angle
  int ledPosition, led, previousLed = 0;
  float x, y, nx, ny, angle;
  // Calculate Angle, Taken from https://learn.adafruit.com/circuit-playground-kaleidoscope/inside
  x = CircuitPlayground.motionX();   // Get the CP accelerometer x and y positions 
  y = CircuitPlayground.motionY();   //   (we ignore the z axis for this one)
  nx = x / 10.0;
  ny = y / 10.0;
  angle = atan((ny/nx)) * 180 / 3.14;  // Figure out the angle of the accelerometer

  if(angle > 0.0) {        // Adjust based on arctangent function (in degrees)
    if(nx < 0.0)
      angle += 180;
  } 
  else {
    if(ny > 0.0)
      angle += 180;
    else
      angle += 360;
  }
  if(angle == 360.0)      // a 360 degree angle is the same as a zero degree angle 
    angle = 0;
  // End of copied code! #############################################################################
  led = map(angle, 0, 360, 0, 10);
  switch (led) {
    case 0: 
    led += 2;
    break;
    
    case 1: 
    led += 2;
    break;
    
    case 2: 
    led += 2;
    break;

    case 3: 
    led += 2;
    break;

    case 4: 
    led += 2;
    break;

    case 5: 
    led += 2;
    break;

    case 6: 
    led += 2;
    break;

    case 7: 
    led += 2;
    break;

    case 8: 
    led = 0;
    break;

    case 9: 
    led = 1;
    break;
  }
  return led;
}

void loop() {
  if(!gameFlag) {
    // Run blue run at startup ##########################################################################
    if(setupBool) {
      for(int i = 0; i <= 9; i++) {
        CircuitPlayground.setPixelColor(i, 0, 0, 150);
        delay(50);
      }
      for(int i = 0; i <= 9; i++) {
        CircuitPlayground.setPixelColor(i, 0, 0, 0);
        delay(50);
      }
      setupBool = 0;
    }
    // Menu Animation ####################################################################################
    int randomLEDMenu = random(0, 9);
    int randomBlue = random(0, 25);
    int randomRed = random(0, 25);
    int randomGreen = random(0, 25);
    for(int i = 0; i < 7; i++) {
      CircuitPlayground.setPixelColor(randomLEDMenu, randomRed*i, randomGreen*i, randomBlue*i);
      delay(25);
    } for(int i = 7; i >= 0; i--) {
      CircuitPlayground.setPixelColor(randomLEDMenu, randomRed*i, randomGreen*i, randomBlue*i);
      delay(25);
    }

    // Illuminate Player Movement Chouce ##################################################################
    if(switchCheck) {
      if(switchFlag) {
        for(int i = 0; i <= 3; i++) {
          for(int i = 0; i <= 9; i++) {
            CircuitPlayground.setPixelColor(i, 128, 0, 128);
          }
          delay(250);
          CircuitPlayground.clearPixels();
          delay(250);
        }
      } else {
        for(int i = 0; i <= 3; i++) {
          for(int i = 0; i <= 9; i++) {
            CircuitPlayground.setPixelColor(i, 255, 165, 0);
          }
          delay(250);
          CircuitPlayground.clearPixels();
          delay(250);
        }
      }
      switchCheck = 0;
    }
  }
  // Start game if both buttons have been pressed ###################################
  if(button1Flag && button2Flag) {
    gameFlag = 1;
    button1Flag = 0;
    button2Flag = 0;
  } if (button1Flag || button2Flag) {
    if(!timeoutStarted) {
      startTimeout.start(2000, AsyncDelay::MILLIS);
      timeoutStarted = 1;
    }
  } if(startTimeout.isExpired() && timeoutStarted == 1) {
    button1Flag = 0;
    button2Flag = 0;
    timeoutStarted = 0;
    Serial.println("TIMOUT!!");
  }
  // #########################################################################################################
  // Play Game ###############################################################################################
  if(gameFlag) {
    // Setup First Round ##########################################
    if(roundCounter == 0) {
      Serial.println("Running Initial Round");
      timer = 7000;
      CircuitPlayground.clearPixels();
      playerGoal = random(0, 9);
      Serial.println(playerGoal);
      CircuitPlayground.setPixelColor(playerGoal, 150, 150, 0);
      levelTimer.start(timer, AsyncDelay::MILLIS);
      SecondTimer.start(1000, AsyncDelay::MILLIS);
      roundCounter++;
      score = 0;
    }

    // Set player Position Orange/White ##########################################
      if(playerPos == playerGoal) {
        CircuitPlayground.setPixelColor(playerPos, 255, 100, 0);
        Serial.println("Set Goal");
      } else {
        CircuitPlayground.setPixelColor(playerPos, 255, 255, 255);
        Serial.println("Set Normal");
      }
    // Angle Movement #####################################################
    if(!switchFlag) {
      bool returnGoal;
      int newPlayerPos = findAngleLED();
      if (newPlayerPos != playerPos) {
        if(playerPos == playerGoal) {
          returnGoal = 1;
          
        }
        CircuitPlayground.setPixelColor(playerPos, 0, 0, 0);
        CircuitPlayground.setPixelColor(newPlayerPos, 255, 255, 255);
        playerPos = newPlayerPos;
      }
      if(returnGoal) {
        CircuitPlayground.setPixelColor(playerGoal, 150, 150, 0);
        returnGoal = 0;
      }
      delay(50);
    } else {
      // Button 1 Movement ##########################################
      if(button1Flag) {
        CircuitPlayground.setPixelColor(playerPos, 0, 0, 0);
        bool returnGoal = 0;
        if(playerPos == playerGoal) {
          CircuitPlayground.setPixelColor(playerGoal, 150, 150, 0);
          returnGoal = 1;
        }
        if(playerPos == 0) {
          playerPos = 9;
        } else {
          playerPos--;
        }
        CircuitPlayground.setPixelColor(playerPos, 255, 255, 255);
        if(returnGoal) {
          CircuitPlayground.setPixelColor(playerPos+1, 150, 150, 0);
          returnGoal = 0;
        }
        button1Flag = 0;
      }

      // Button 2 Movement ##########################################
      if(button2Flag) {
        CircuitPlayground.setPixelColor(playerPos, 0, 0, 0);
        bool returnGoal = 0;
        if(playerPos == playerGoal) {
          CircuitPlayground.setPixelColor(playerGoal, 150, 150, 0);
          returnGoal = 1;
        }
        if(playerPos == 9) {
          playerPos = 0;
        } else {
          playerPos++;
        }
        CircuitPlayground.setPixelColor(playerPos, 255, 255, 255);
        if(returnGoal) {
          CircuitPlayground.setPixelColor(playerPos-1, 150, 150, 0);
        }
        button2Flag = 0;
      }
    }

    // Timer Check ##########################################
    if(levelTimer.isExpired()) {
      Serial.println("Timer is out!");

      // If player Fails ##########################################
      if(playerPos != playerGoal || score == 10) {
        gameFlag = 0;
        roundCounter = 0;
        for(int i = 0; i < 3; i++) {
          for(int i = 0; i < 10; i++) {
            if(score == 10) {
              CircuitPlayground.setPixelColor(i, 0, 0, 150);
            } else {
              CircuitPlayground.setPixelColor(i, 150, 0, 0);
            }
          }
          delay(250);
          CircuitPlayground.clearPixels();
          delay(250);
        }
        for(int i = 0; i < score; i++) {
          CircuitPlayground.setPixelColor(i, 150, 150, 0);
          delay(100);
        }
        if(score == 10) {
          delay(200);
          for(int i = 0; i < 3; i++) {
            CircuitPlayground.clearPixels();
            delay(500);
            for(int i = 0; i <=9; i++) {
              CircuitPlayground.setPixelColor(i, 150, 150, 0);
            }
            delay(500);
          }
        }
        delay(3000);
        setupBool = 1;
        // Otherwise ##########################################
      } else {
        for(int i = 0; i < 3; i++) {
          for(int i = 0; i < 10; i++) {
            CircuitPlayground.setPixelColor(i, 0, 150, 0);
          }
          delay(250);
          CircuitPlayground.clearPixels();
          delay(250);
          roundCounter = 0;
        }
        CircuitPlayground.clearPixels();
        playerGoal = random(0, 9);
        CircuitPlayground.setPixelColor(playerGoal, 150, 150, 0);

        if(timer > 1000) {
          timer -= 1000;
        }
        levelTimer.start(timer, AsyncDelay::MILLIS);
        Serial.print("Timer: ");
        Serial.println(timer);
        score++;
        roundCounter++;
      }
    // End of Timer Check
    }
    if(SecondTimer.isExpired()) {
//      CircuitPlayground.playTone(440, 100);
      SecondTimer.repeat();
    }
  // End of GAME loop
  }
// End of Loop
}

// ####################################################################################
// ########### Interrupts
void button1Press() {
  button1Flag = 1;
  Serial.println("Running Button 1 Press");
}

void button2Press() {
  button2Flag = 1;
  Serial.println("Running Button 2 Press");
}

void switchFlip() {
  if(!gameFlag) {
    switchFlag = !switchFlag;
    switchCheck = 1;
    Serial.println("Running Switch Flip");
  }
}
