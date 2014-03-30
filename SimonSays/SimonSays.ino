#include <Gamer.h>
#include <SoftwareSerial.h>
#include <TrueRandom.h>
#include <Tone.h>

Gamer gamer;

int sequence[100];
int sequenceLength = 0;
int currentIndex = 0;
int currentDir = 0;
int score = 0;

int mode = 1;

boolean started = false;

byte numbers[10][2][8];
byte n1[8],n1b[8],n2[8],n2b[8],n3[8],n3b[8],n4[8],n4b[8],n5[8],n5b[8],n6[8],n6b[8],n7[8],n7b[8],n8[8],n8b[8],n9[8],n9b[8],n0[8],n0b[8];

Tone dummy;
Tone buzzer;

void setup()
{
  // We need to declare two tones as the Tone library
  // assigns them in a set sequence, TIMER2 being first
  // and TIMMER1 being second. We need to use TIMMER1
  // though as the gamer library uses TIMER2.
  dummy.begin(12); // Unused port
  buzzer.begin(2); 
  
  gamer.begin();
  setupNumbers();
  resetAll();
}

void loop()
{
  
  // Wait for start button before starting
  if(!started && gamer.isPressed(START)){
    resetAll();
    extendSequence();
    started = true; 
  }
  
  // Do stuff
  clearDisplay();
  drawPlayArea();
  
  // See if we have started
  if(started){
  
    if(currentDir != 0)
    {
      // If we have a dir still, reset and skip a cycle (ensures we see a gap between keypresses)
      currentDir = 0; 
    } 
    else 
    {
    
      // Capture direction
      if(mode == 1){
        getSequenceStep();
      } else if(mode == 2) {
        captureInput();
      }
      
      // Draw the dir
      if(currentDir != 0){
        if(currentDir == 1) { // Up
          drawUp();
        } else if(currentDir == 2) {  // Right
          drawRight();
        } else if(currentDir == 3) {  // Down
          drawDown();
        } else if(currentDir == 4) {  // Left
          drawLeft();
        }
      }
    
    }
  }
  
  // Update display
  gamer.updateDisplay();
      
  // Decide what to do next
  if(currentDir != 0){
    
    // Pause for a moment (showing shape)
    if(mode == 1){
      
      // Gradually speed up when in playback mode
      if(score < 5) {
        delay(300); 
      } else if (score < 10){
        delay(250); 
      } else if (score < 15){
        delay(200); 
      } else  if (score < 20){
        delay(150); 
      } else {
        delay(100); 
      }
      
    } else {
      delay(300); 
    }
    
    buzzer.stop();
    
    if(mode == 1){ // Playback mode
    
      if(currentIndex == sequenceLength - 1){ // Reached the end of the sequence
        mode = 2; // Swicth to input mode
        currentIndex = 0; // Reset current index
      } else {
        currentIndex++; // Move on to next step
      }
      
    } else if(mode == 2){ // Input mode
    
      if(currentDir == sequence[currentIndex]){ // Does it match sequence
        
        if(currentIndex == sequenceLength - 1){ // Reached the end of the sequence
          extendSequence(); // Extend the sequence
          score++; // Increment score
          mode = 1; // Switch back to playback mode
          currentIndex = 0; // Reset the index
        } else {
          currentIndex++; // Move on to next step
        }
        
      } else {
        
         // Error
         gamer.clear();
         delay(20);
        
         // Show score
         int dig2 = score % 10;  //split score into two digits (eg 10 -> 1 and 0)
         int dig1 = (score-(score%10))/10;
         showScore(dig1,dig2);
         delay(600);
    
         // Start over
         resetAll(); 
      }
      
    }
    
  } else {
    
    if(currentIndex == 0 && mode == 1){
      delay(400); // Pause before starting playback
    } else {
      delay(100); 
    }
   
  }
}

void extendSequence()
{
  sequence[sequenceLength] = random(1,5);
  sequenceLength++;
}

void getSequenceStep()
{
  currentDir = sequence[currentIndex];
}

void captureInput()
{
  if(gamer.isPressed(UP))          currentDir = 1;
  else if(gamer.isPressed(RIGHT))  currentDir = 2;
  else if(gamer.isPressed(DOWN))   currentDir = 3;
  else if(gamer.isPressed(LEFT))   currentDir = 4;
  else currentDir = 0;
}

void clearDisplay()
{
  for(int x=0;x<8;x++) {
    for(int y=0;y<8;y++) {
      gamer.display[x][y] = LOW;
    }
  }
}

void resetAll()
{
  gamer.clear();
  currentIndex = 0;
  currentDir = 0;
  score = 0;
  resetSequence();
  clearKeysCache();
  
  mode = 1;
  started = false;
}

void resetSequence()
{
 for(int i =0; i < 100; i++){
  sequence[i] = 0;
 } 
 sequenceLength = 0;
}

void clearKeysCache()
{
  gamer.isPressed(UP);
  gamer.isPressed(RIGHT);
  gamer.isPressed(DOWN);
  gamer.isPressed(LEFT); 
  gamer.isPressed(START); 
  
  //TODO: LDR
}

void drawPlayArea()
{
  for(int x=0;x<8;x++) {
    gamer.display[x][x] = HIGH;
    gamer.display[7-x][x] = HIGH;
  }
}

void drawUp()
{
  gamer.display[1][0] = HIGH;
  gamer.display[2][0] = HIGH;
  gamer.display[3][0] = HIGH;
  gamer.display[4][0] = HIGH;
  gamer.display[5][0] = HIGH;
  gamer.display[6][0] = HIGH;
  
  gamer.display[2][1] = HIGH;
  gamer.display[3][1] = HIGH;
  gamer.display[4][1] = HIGH;
  gamer.display[5][1] = HIGH;
  
  gamer.display[3][2] = HIGH;
  gamer.display[4][2] = HIGH;
  
  buzzer.play(NOTE_B2);
}

void drawRight()
{
  gamer.display[7][1] = HIGH;
  gamer.display[7][2] = HIGH;
  gamer.display[7][3] = HIGH;
  gamer.display[7][4] = HIGH;
  gamer.display[7][5] = HIGH;
  gamer.display[7][6] = HIGH;
  
  gamer.display[6][2] = HIGH;
  gamer.display[6][3] = HIGH;
  gamer.display[6][4] = HIGH;
  gamer.display[6][5] = HIGH;
  
  gamer.display[5][3] = HIGH;
  gamer.display[5][4] = HIGH;
  
  buzzer.play(NOTE_B3);
}

void drawDown()
{
  gamer.display[3][5] = HIGH;
  gamer.display[4][5] = HIGH;
  
  gamer.display[2][6] = HIGH;
  gamer.display[3][6] = HIGH;
  gamer.display[4][6] = HIGH;
  gamer.display[5][6] = HIGH;
  
  gamer.display[1][7] = HIGH;
  gamer.display[2][7] = HIGH;
  gamer.display[3][7] = HIGH;
  gamer.display[4][7] = HIGH;
  gamer.display[5][7] = HIGH;
  gamer.display[6][7] = HIGH;
  
  buzzer.play(NOTE_B4);
}

void drawLeft()
{
  gamer.display[0][1] = HIGH;
  gamer.display[0][2] = HIGH;
  gamer.display[0][3] = HIGH;
  gamer.display[0][4] = HIGH;
  gamer.display[0][5] = HIGH;
  gamer.display[0][6] = HIGH;
  
  gamer.display[1][2] = HIGH;
  gamer.display[1][3] = HIGH;
  gamer.display[1][4] = HIGH;
  gamer.display[1][5] = HIGH;
  
  gamer.display[2][3] = HIGH;
  gamer.display[2][4] = HIGH;
  
  buzzer.play(NOTE_B5);
}

void showScore(int dig1,int dig2) {
  byte result[8];
  for(int p=0;p<8;p++) {
    result[p]=numbers[dig1][0][p]^numbers[dig2][1][p];
  }
  gamer.printImage(result);
}

void setupNumbers() {
  n1[0] = B10000000;
  n1[1] = B10000000;
  n1[2] = B10000000;
  n1[3] = B10000000;
  n1[4] = B10000000;
  n1[5] = B10000000;
  n1[6] = B10000000;
  n1[7] = B10000000;

  n1b[0] = B00000100;
  n1b[1] = B00000100;
  n1b[2] = B00000100;
  n1b[3] = B00000100;
  n1b[4] = B00000100;
  n1b[5] = B00000100;
  n1b[6] = B00000100;
  n1b[7] = B00000100;

  n2[0] = B11100000;
  n2[1] = B00100000;
  n2[2] = B00100000;
  n2[3] = B11100000;
  n2[4] = B10000000;
  n2[5] = B10000000;
  n2[6] = B10000000;
  n2[7] = B11100000;

  n2b[0] = B00000111;
  n2b[1] = B00000001;
  n2b[2] = B00000001;
  n2b[3] = B00000111;
  n2b[4] = B00000100;
  n2b[5] = B00000100;
  n2b[6] = B00000100;
  n2b[7] = B00000111;

  n3[0] = B11100000;
  n3[1] = B00100000;
  n3[2] = B00100000;
  n3[3] = B01100000;
  n3[4] = B00100000;
  n3[5] = B00100000;
  n3[6] = B00100000;
  n3[7] = B11100000;

  n3b[0] = B00000111;
  n3b[1] = B00000001;
  n3b[2] = B00000001;
  n3b[3] = B00000011;
  n3b[4] = B00000001;
  n3b[5] = B00000001;
  n3b[6] = B00000001;
  n3b[7] = B00000111;

  n4[0] = B10100000;
  n4[1] = B10100000;
  n4[2] = B10100000;
  n4[3] = B11100000;
  n4[4] = B00100000;
  n4[5] = B00100000;
  n4[6] = B00100000;
  n4[7] = B00100000;

  n4b[0] = B00000101;
  n4b[1] = B00000101;
  n4b[2] = B00000101;
  n4b[3] = B00000111;
  n4b[4] = B00000001;
  n4b[5] = B00000001;
  n4b[6] = B00000001;
  n4b[7] = B00000001;

  n5[0] = B11100000;
  n5[1] = B10000000;
  n5[2] = B10000000;
  n5[3] = B11100000;
  n5[4] = B00100000;
  n5[5] = B00100000;
  n5[6] = B00100000;
  n5[7] = B11100000;

  n5b[0] = B00000111;
  n5b[1] = B00000100;
  n5b[2] = B00000100;
  n5b[3] = B00000111;
  n5b[4] = B00000001;
  n5b[5] = B00000001;
  n5b[6] = B00000001;
  n5b[7] = B00000111;

  n6[0] = B11100000;
  n6[1] = B10000000;
  n6[2] = B10000000;
  n6[3] = B11100000;
  n6[4] = B10100000;
  n6[5] = B10100000;
  n6[6] = B10100000;
  n6[7] = B11100000;

  n6b[0] = B00000111;
  n6b[1] = B00000100;
  n6b[2] = B00000100;
  n6b[3] = B00000111;
  n6b[4] = B00000101;
  n6b[5] = B00000101;
  n6b[6] = B00000101;
  n6b[7] = B00000111;

  n7[0] = B11100000;
  n7[1] = B00100000;
  n7[2] = B00100000;
  n7[3] = B00100000;
  n7[4] = B00100000;
  n7[5] = B00100000;
  n7[6] = B00100000;
  n7[7] = B00100000;

  n7b[0] = B00000111;
  n7b[1] = B00000001;
  n7b[2] = B00000001;
  n7b[3] = B00000001;
  n7b[4] = B00000001;
  n7b[5] = B00000001;
  n7b[6] = B00000001;
  n7b[7] = B00000001;

  n8[0] = B11100000;
  n8[1] = B10100000;
  n8[2] = B10100000;
  n8[3] = B11100000;
  n8[4] = B10100000;
  n8[5] = B10100000;
  n8[6] = B10100000;
  n8[7] = B11100000;

  n8b[0] = B00000111;
  n8b[1] = B00000101;
  n8b[2] = B00000101;
  n8b[3] = B00000111;
  n8b[4] = B00000101;
  n8b[5] = B00000101;
  n8b[6] = B00000101;
  n8b[7] = B00000111;

  n9[0] = B11100000;
  n9[1] = B10100000;
  n9[2] = B10100000;
  n9[3] = B11100000;
  n9[4] = B00100000;
  n9[5] = B00100000;
  n9[6] = B00100000;
  n9[7] = B11100000;

  n9b[0] = B00000111;
  n9b[1] = B00000101;
  n9b[2] = B00000101;
  n9b[3] = B00000111;
  n9b[4] = B00000001;
  n9b[5] = B00000001;
  n9b[6] = B00000001;
  n9b[7] = B00000111;

  n0[0] = B11100000;
  n0[1] = B10100000;
  n0[2] = B10100000;
  n0[3] = B10100000;
  n0[4] = B10100000;
  n0[5] = B10100000;
  n0[6] = B10100000;
  n0[7] = B11100000;

  n0b[0] = B00000111;
  n0b[1] = B00000101;
  n0b[2] = B00000101;
  n0b[3] = B00000101;
  n0b[4] = B00000101;
  n0b[5] = B00000101;
  n0b[6] = B00000101;
  n0b[7] = B00000111;

  for(int x=0;x<8;x++) {
    numbers[0][0][x] = n0[x];
    numbers[1][0][x] = n1[x];
    numbers[2][0][x] = n2[x];
    numbers[3][0][x] = n3[x];
    numbers[4][0][x] = n4[x];
    numbers[5][0][x] = n5[x];
    numbers[6][0][x] = n6[x];
    numbers[7][0][x] = n7[x];
    numbers[8][0][x] = n8[x];
    numbers[9][0][x] = n9[x];
    numbers[0][1][x] = n0b[x];
    numbers[1][1][x] = n1b[x];
    numbers[2][1][x] = n2b[x];
    numbers[3][1][x] = n3b[x];
    numbers[4][1][x] = n4b[x];
    numbers[5][1][x] = n5b[x];
    numbers[6][1][x] = n6b[x];
    numbers[7][1][x] = n7b[x];
    numbers[8][1][x] = n8b[x];
    numbers[9][1][x] = n9b[x];
  }
}
