// ==================================================== 
// Adapted from the original code by Jae Yeong Bae 
// https://www.youtube.com/watch?v=6Kt2A1aB3o4
// ====================================================

#include <Gamer.h>
#include <SoftwareSerial.h>
#include <TrueRandom.h>

// ====================================================
// Variables
// ====================================================

Gamer gamer;

int score = 0;

unsigned char blocktype;
unsigned char blockrotation;

boolean  block[8][10]; //2 extra for rotation
boolean  pile[8][8];

boolean started = false;

int delayCount = 0;
int delayInterval = 8;

byte numbers[10][2][8];
byte n1[8],n1b[8],n2[8],n2b[8],n3[8],n3b[8],n4[8],n4b[8],n5[8],n5b[8],n6[8],n6b[8],n7[8],n7b[8],n8[8],n8b[8],n9[8],n9b[8],n0[8],n0b[8];

// ====================================================
// Control Flow
// ====================================================

void setup()
{
  Serial.begin(9600);
  gamer.begin();
  setupNumbers();
  resetAll();
}

void loop()
{
  if(!started && gamer.isPressed(START)){
    clearScreen();
    newBlock();
    updateScreen();
    delay(500);
    started = true;
    Serial.println("Started");
  }
  
  if(started)
  {    
    clearScreen();
    
    if(delayCount >= delayInterval){
      movedown();
      delayCount = 0; 
    }
    else
    {
      delayCount++; 
    }
    
    if(gamer.isPressed(UP)) {
      rotate();
    } else if(gamer.isPressed(RIGHT)) {
      moveright();
    } else if(gamer.isPressed(DOWN)) {
      movedown();
    } else if(gamer.isPressed(LEFT)) {
      moveleft();
    }
  }

  updateScreen();
  delay(100);
}

// ====================================================
// Control Methods
// ====================================================

void newBlock()
{
  if(check_gameover())
    return;
  
  blocktype = TrueRandom.random(7);
  
  if (blocktype == 0)
  // 0
  // 0
  // 0
  // 0
  {
    block[3][0]=1;
    block[3][1]=1;
    block[3][2]=1;
    block[3][3]=1;  
    Serial.println("----");    
  }

  if (blocktype == 1)
  // 0
  // 0 0 0
  {
    block[2][0]=1;
    block[2][1]=1;
    block[3][1]=1;
    block[4][1]=1;      
    Serial.println("|__");     
  }
  
  if (blocktype == 2)
  //     0
  // 0 0 0
  {
    block[4][0]=1;
    block[2][1]=1;
    block[3][1]=1;
    block[4][1]=1;  
    Serial.println("__|");        
  }

  if (blocktype == 3)
  // 0 0
  // 0 0
  {
    block[3][0]=1;
    block[3][1]=1;
    block[4][0]=1;
    block[4][1]=1;    
    Serial.println("||");        
  }    

  if (blocktype == 4)
  //   0 0
  // 0 0
  {
    block[4][0]=1;
    block[5][0]=1;
    block[3][1]=1;
    block[4][1]=1;   
    Serial.println("_|-");        
  }    
  
  if (blocktype == 5)
  //   0
  // 0 0 0
  {
    block[4][0]=1;
    block[3][1]=1;
    block[4][1]=1;
    block[5][1]=1;   
    Serial.println("_|_");     
  }        

  if (blocktype == 6)
  // 0 0
  //   0 0
  {
    block[3][0]=1;
    block[4][0]=1;
    block[4][1]=1;
    block[5][1]=1;  
    Serial.println("-|_");         
  }    

  blockrotation = 0;
}

boolean moveleft()
{  
  if (space_left())
  {
    int i;
    int j;
    for (i=0;i<7;i++)
    {
      for (j=0;j<8;j++)      
      {
        block[i][j]=block[i+1][j];
      }
    }
    
    for (j=0;j<8;j++)      
    {
      block[7][j]=0;
    }    

    return 1;
  }

  return 0;
}

boolean moveright()
{
  Serial.print(space_right());
  if (space_right())
  {
    int i;
    int j;
    for (i=7;i>=0;i--)
    {
      for (j=0;j<8;j++)      
      {
        block[i][j]=block[i-1][j];
      }
    }

    for (j=0;j<8;j++)    
    {
      block[0][j]=0;
    }    
    
   return 1;   
  }
  return 0;
}

void movedown()
{
  if (space_below())
  {
    //move down
    int i;
    for (i=7;i>=0;i--)
    {
      int j;
      for (j=0;j<8;j++)
      {
        block[j][i] = block[j][i-1];
      }
    }
    for (i=0;i<7;i++)
    {
      block[i][0] = 0;
    }
  }
  else
  {
    //merge and new block
    int i;
    int j;    
    for (i=0;i<8;i++)
    {
     for(j=0;j<8;j++)
     {
       if (block[i][j])
       {
         pile[i][j]=1;
         block[i][j]=0;
       }
     }
    }
    newBlock();   
  }
}

void rotate()
{
  
  //skip for square block(3)
  if (blocktype == 3) return;
  
  int xi;
  int yi;
  int i;
  int j;
  //detect left
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
      if (block[i][j])
      {
        xi = i;
      }
    }
  }
  
  //detect up
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
      if (block[j][i])
      {
        yi = i;
      }
    }
  }  
    
  if (blocktype == 0)
  {
    if (blockrotation == 0) 
    {
      
      
      if (!space_left())
      {
        if (space_right3())
        {
          if (!moveright())
            return;
          xi++;
        }
        else return;
      }     
      else if (!space_right())
      {
        if (space_left3())
        {
          if (!moveleft())
            return;
          if (!moveleft())
            return;          
          xi--;
          xi--;        
        }
        else
          return;
      }
      else if (!space_right2())
      {
        if (space_left2())
        {
          if (!moveleft())
            return;          
          xi--;      
        }
        else
          return;
      }   
   
      
   
      
      
      block[xi][yi]=0;
      block[xi][yi+2]=0;
      block[xi][yi+3]=0;      
      
      block[xi-1][yi+1]=1;
      block[xi+1][yi+1]=1;
      block[xi+2][yi+1]=1;      

      blockrotation = 1;
    }
    else
    {
      block[xi][yi]=0;
      block[xi+2][yi]=0;
      block[xi+3][yi]=0;
      
      block[xi+1][yi-1]=1;
      block[xi+1][yi+1]=1;
      block[xi+1][yi+2]=1;

      blockrotation = 0;
    }    
  }
  
  //offset to mid
  xi ++;  
  yi ++;  
  
  if (blocktype == 1)
  {
    if (blockrotation == 0)
    {
      block[xi-1][yi-1] = 0;
      block[xi-1][yi] = 0;
      block[xi+1][yi] = 0;

      block[xi][yi-1] = 1;
      block[xi+1][yi-1] = 1;
      block[xi][yi+1] = 1;      
      
      blockrotation = 1;
    }
    else if (blockrotation == 1)
    {
      if (!space_left())
      {
        if (!moveright())
          return;
        xi++;
      }        
      xi--;
      
      block[xi][yi-1] = 0;
      block[xi+1][yi-1] = 0;
      block[xi][yi+1] = 0;      
      
      block[xi-1][yi] = 1;
      block[xi+1][yi] = 1;
      block[xi+1][yi+1] = 1;      
      
      blockrotation = 2;      
    }
    else if (blockrotation == 2)
    {
      yi --;
      
      block[xi-1][yi] = 0;
      block[xi+1][yi] = 0;
      block[xi+1][yi+1] = 0;      
      
      block[xi][yi-1] = 1;
      block[xi][yi+1] = 1;
      block[xi-1][yi+1] = 1;      
      
      blockrotation = 3;            
    }
    else
    {
      if (!space_right())
      {
        if (!moveleft())
          return;
        xi--;
      }
      block[xi][yi-1] = 0;
      block[xi][yi+1] = 0;
      block[xi-1][yi+1] = 0;        

      block[xi-1][yi-1] = 1;
      block[xi-1][yi] = 1;
      block[xi+1][yi] = 1;
      
      blockrotation = 0;          
    }  
  }



  if (blocktype == 2)
  {
    if (blockrotation == 0)
    {
      block[xi+1][yi-1] = 0;
      block[xi-1][yi] = 0;
      block[xi+1][yi] = 0;

      block[xi][yi-1] = 1;
      block[xi+1][yi+1] = 1;
      block[xi][yi+1] = 1;      
      
      blockrotation = 1;
    }
    else if (blockrotation == 1)
    {
      if (!space_left())
      {
        if (!moveright())
          return;
        xi++;
      }              
      xi--;
      
      block[xi][yi-1] = 0;
      block[xi+1][yi+1] = 0;
      block[xi][yi+1] = 0;      
      
      block[xi-1][yi] = 1;
      block[xi+1][yi] = 1;
      block[xi-1][yi+1] = 1;      
      
      blockrotation = 2;      
    }
    else if (blockrotation == 2)
    {
      yi --;
      
      block[xi-1][yi] = 0;
      block[xi+1][yi] = 0;
      block[xi-1][yi+1] = 0;      
      
      block[xi][yi-1] = 1;
      block[xi][yi+1] = 1;
      block[xi-1][yi-1] = 1;      
      
      blockrotation = 3;            
    }
    else
    {
      if (!space_right())
      {
        if (!moveleft())
          return;
        xi--;
      }      
      block[xi][yi-1] = 0;
      block[xi][yi+1] = 0;
      block[xi-1][yi-1] = 0;        

      block[xi+1][yi-1] = 1;
      block[xi-1][yi] = 1;
      block[xi+1][yi] = 1;
      
      blockrotation = 0;          
    }  
  }
  
  if (blocktype == 4)
  {
    if (blockrotation == 0)
    {
      block[xi+1][yi-1] = 0;
      block[xi-1][yi] = 0;

      block[xi+1][yi] = 1;
      block[xi+1][yi+1] = 1;      
      
      blockrotation = 1;
    }
    else
    {
      if (!space_left())
      {
        if (!moveright())
          return;
        xi++;
      }              
      xi--;
      
      block[xi+1][yi] = 0;
      block[xi+1][yi+1] = 0;      
      
      block[xi-1][yi] = 1;
      block[xi+1][yi-1] = 1;
      
      blockrotation = 0;          
    }  
  }  


  if (blocktype == 5)
  {
    if (blockrotation == 0)
    {
      block[xi][yi-1] = 0;
      block[xi-1][yi] = 0;
      block[xi+1][yi] = 0;

      block[xi][yi-1] = 1;
      block[xi+1][yi] = 1;
      block[xi][yi+1] = 1;      
      
      blockrotation = 1;
    }
    else if (blockrotation == 1)
    {
      if (!space_left())
      {
        if (!moveright())
          return;
        xi++;
      }              
      xi--;
      
      block[xi][yi-1] = 0;
      block[xi+1][yi] = 0;
      block[xi][yi+1] = 0;
      
      block[xi-1][yi] = 1;
      block[xi+1][yi] = 1;
      block[xi][yi+1] = 1;
      
      blockrotation = 2;      
    }
    else if (blockrotation == 2)
    {
      yi --;
      
      block[xi-1][yi] = 0;
      block[xi+1][yi] = 0;
      block[xi][yi+1] = 0;     
      
      block[xi][yi-1] = 1;
      block[xi-1][yi] = 1;
      block[xi][yi+1] = 1;      
      
      blockrotation = 3;            
    }
    else
    {
      if (!space_right())
      {
        if (!moveleft())
          return;
        xi--;
      }      
      block[xi][yi-1] = 0;
      block[xi-1][yi] = 0;
      block[xi][yi+1] = 0;      
      
      block[xi][yi-1] = 1;
      block[xi-1][yi] = 1;
      block[xi+1][yi] = 1;
      
      blockrotation = 0;          
    }  
  }
  
  if (blocktype == 6)
  {
    if (blockrotation == 0)
    {
      block[xi-1][yi-1] = 0;
      block[xi][yi-1] = 0;

      block[xi+1][yi-1] = 1;
      block[xi][yi+1] = 1;      
      
      blockrotation = 1;
    }
    else
    {
      if (!space_left())
      {
        if (!moveright())
          return;
        xi++;
      }              
      xi--;
      
      block[xi+1][yi-1] = 0;
      block[xi][yi+1] = 0;      
      
      block[xi-1][yi-1] = 1;
      block[xi][yi-1] = 1;
      
      blockrotation = 0;          
    }  
  } 

  //if rotating made block and pile overlap, push rows up
  while (!check_overlap())
  {
    for (i=0;i<10;i++)
    {
      for (j=0;j<8;j++)
      {
         block[j][i] = block[j][i+1];
      }
    }
    //delays = millis() + delay_;
  }
}

boolean space_below()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (i == 7)
           return false;
         if (pile[j][i+1])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean space_left2()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (j == 0 || j == 1)
           return false;
         if (pile[j-1][i] | pile[j-2][i])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean space_left3()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (j == 0 || j == 1 ||j == 2 )
           return false;
         if (pile[j-1][i] | pile[j-2][i]|pile[j-3][i])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean space_left()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (j == 0)
           return false;
         if (pile[j-1][i])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean space_right()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (j == 7)
           return false;
         if (pile[j+1][i])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean space_right3()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (j == 7||j == 6||j == 5)
           return false;
         if (pile[j+1][i] |pile[j+2][i] | pile[j+3][i])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean space_right2()
{ 
  int i;
  int j;  
  for (i=7;i>=0;i--)
  {
    for (j=0;j<8;j++)
    {
       if (block[j][i])
       {
         if (j == 7 || j == 6)
           return false;
         if (pile[j+1][i] |pile[j+2][i])
         {
           return false;
         }      
       }        
    }
  }
  return true;
}

boolean check_overlap()
{
  int i;
  int j;  
  for (i=0;i<8;i++)
  {
    for (j=0;j<7;j++)
    {
       if (block[j][i])
       {
         if (pile[j][i])
           return false;
       }        
    }
  }
  for (i=8;i<10;i++)
  {
    for (j=0;j<7;j++)
    {
       if (block[j][i])
       {
         return false;
       }        
    }
  }  
  return true;
}

boolean check_gameover()
{
  int i;
  int j;
  int cnt=0;;
  
  // Loop through bottom 7 rows
  for(i=7;i>=0;i--)
  {
    // Count how many blocks we have lit
    cnt=0;
    for (j=0;j<8;j++)
    {
      if (pile[j][i])
      {
        cnt ++;
      }
    }    
    if (cnt == 8) // We have 8 in a row
    {
      // Clear the line
      for (j=0;j<8;j++)
      {
        pile[j][i]=0;
      }
      
      // Shift everything down
      int k;
      for(k=i;k>0;k--)
      {
        for (j=0;j<8;j++)
        {
          pile[j][k] = pile[j][k-1];
        }                
      }
      for (j=0;j<8;j++)
      {
        pile[j][0] = 0;
      }        
      
      // Increase score
      score++;
      
      // Move to next row
      i++;         
    }
  }  
  
  // Blocks in line one, game over
  for(i=0;i<8;i++)
  {
    if (pile[i][0])
    {
      gameover();
      return true;
    }
  }
  return false;
}

void gameover()
{
   Serial.println("game over");
  
   // Clear everything
   gamer.clear();
   delay(20);
  
   // Show score
   int dig2 = score % 10;  //split score into two digits (eg 10 -> 1 and 0)
   int dig1 = (score-(score%10))/10;
   showScore(dig1,dig2);
   delay(2000);
  
   // Start over
   resetAll(); 
}

void updateScreen()
{
  for (int x=0;x<8;x++)
  {
    for (int y=0;y<8;y++)
    {
      if(block[x][y] | pile[x][y])
      {
        gamer.display[x][y] = HIGH;
      }
    }
  }
  
  gamer.updateDisplay();
}

// ====================================================
// Resets
// ====================================================

void resetAll()
{
  score = 0;
  started = false;
  clearBlock();
  clearPile();
  delayCount = 0;
  drawIntroScreen();
}

void clearBlock()
{
  for(int x=0;x<8;x++) {
    for(int y=0;y<10;y++) {
      block[x][y]=0;
    }
  }
}

void clearPile()
{
  for(int x=0;x<8;x++) {
    for(int y=0;y<8;y++) {
      pile[x][y]=0;
    }
  }
}

void clearScreen()
{
  for(int x=0;x<8;x++) {
    for(int y=0;y<8;y++) {
      gamer.display[x][y] = LOW;
    }
  }
}

void drawIntroScreen()
{
  gamer.clear();
  
  gamer.display[0][0] = HIGH;
  gamer.display[1][0] = HIGH;
  gamer.display[2][0] = HIGH;
  gamer.display[3][0] = HIGH;
  gamer.display[4][0] = HIGH;
  gamer.display[5][0] = HIGH;
  gamer.display[6][0] = HIGH;
  gamer.display[7][0] = HIGH;
  
  gamer.display[0][1] = HIGH;
  gamer.display[7][1] = HIGH;
  
  gamer.display[0][2] = HIGH;
  gamer.display[7][2] = HIGH;
  
  gamer.display[0][3] = HIGH;
  gamer.display[1][3] = HIGH;
  gamer.display[2][3] = HIGH;
  gamer.display[5][3] = HIGH;
  gamer.display[6][3] = HIGH;
  gamer.display[7][3] = HIGH;
  
  gamer.display[2][4] = HIGH;
  gamer.display[5][4] = HIGH;
  
  gamer.display[2][5] = HIGH;
  gamer.display[5][5] = HIGH;
  
  gamer.display[2][6] = HIGH;
  gamer.display[5][6] = HIGH;
  
  gamer.display[2][7] = HIGH;
  gamer.display[3][7] = HIGH;
  gamer.display[4][7] = HIGH;
  gamer.display[5][7] = HIGH;
  
  gamer.updateDisplay();
}

// ====================================================
// Scoring
// ====================================================

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
