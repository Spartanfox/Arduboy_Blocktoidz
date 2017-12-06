#include <Arduboy2.h>
#include <ArduboyPlaytune.h>
#include "Sprites.h"
#include "Levels.h"
#include "Tunes.h"
#define gray 1
Arduboy2 arduboy;
ArduboyPlaytune tunes(arduboy.audio.enabled);

boolean gameOver;
boolean paused = true;

unsigned char screen = 1;
unsigned char frames = 0;
unsigned char levelNum = 0;
unsigned char preview = 0;
unsigned char store = 0;
unsigned long score = 50000;

int scoreAcc;
struct Block;
struct Level;


static boolean collides(unsigned char,unsigned char);
static void drawBlock(char, char, unsigned char);
static unsigned char getBlock(int,int,Level*);
static unsigned char resetBlock();
static void runGameOver();

struct Level{
  static const unsigned char x = 5*8;
  static const unsigned char y = 4;
  static const unsigned char block_width = 6;
  static const unsigned char block_height = 7;
  unsigned char breaker = 0;
  unsigned char blocks[block_height][block_width];

  //for some reason using progmem with the levels causes corruption
  void loadLevel(const unsigned char (*level)[block_height*block_width]){
    int acc=0;
    int check=0;
    breaker = 0;
    for(int y = 0; y < block_height;y++){
      for(int x = 0; x < block_width;x++){
        check+= (*level)[acc];
        blocks[(block_height-1)-y][x] = (*level)[acc];
        acc++;
      }
      if(check!=0)breaker++;
      check = 0;
    }
    breaker--;
  }
  void clearLevel(){
    for(int y = 0; y < block_height;y++)
      for(int x = 0; x < block_width;x++)
        blocks[y][x] = 0;
  }
} level;

struct Block{
  unsigned char x = 0;
  unsigned char y = level.block_height-1;
  unsigned char shape;
  unsigned char tris;
  unsigned char block_speed = 60;
  char cooldown = block_speed;
  void rotate(){
      unsigned char rotated = shape&B_SOLID;
      rotated += rotated;
      if((rotated>B_SOLID)!=0){rotated++;}
      shape=(rotated&B_SOLID);
}
  void try_rotate(Level *level){
        rotate();
        if(collides(shape,getBlock(x,y,level)))rotate();
        if(collides(shape,getBlock(x,y,level)))rotate();
        if(collides(shape,getBlock(x,y,level)))rotate();
        if(collides(shape,getBlock(x,y,level)))rotate();
    }
  void placeBlock(Level *level){
     
     if((y>=(level->block_height))){
        runGameOver();
      
     }
     else{
      tunes.playScore(blop);
        level->blocks[y][x] |= shape;
        y =level->block_height-1;
      }
  }
  void draw(){
      drawBlock(x*8+level.x,y*8+level.y,shape);
    }
} focused;


static void runGameOver(){
  tunes.stopScore();
  tunes.playScore(music);
  gameOver = true;
  score =score * (levelNum+1);
  }
static unsigned char resetBlock(){
  unsigned char shape = 0;
  unsigned char tris = 0;
   do{
    if(random(0,1)==0){
        shape |= B_TOP;
        tris++;
    }
    if(random(0,3)==0){
        shape |= B_RIGHT;
        tris++;
    }
    if(random(0,3)==0&&tris!=2){
        shape |= B_BOTTOM;
        tris++;
    }
    if(random(0,3)==0&&tris!=2){
        shape |= B_LEFT;
    }
  }
  while(shape==0);
  return shape;
}

static boolean collides(unsigned char shape1,unsigned char shape2){
  if(((shape1&B_SOLID)&(shape2&B_SOLID))==0)return false;
  return true;
}
static unsigned char getBlock(int x, int y,Level* level){
    if(x<0||y<0||(x>=level->block_height-1))return B_SOLID;
    if(y>=level->block_height)return 0;
    return level->blocks[y][x];
}
static void drawBlock(char x, char y,unsigned char shape){
    y = 7*8-y;
    if((shape&B_SOLID)==B_SOLID){
      arduboy.drawBitmap(x,y,solid,8,1); 
    } 
    else{
     // if(((shape&B_BOTTOM)==0)||((shape&B_RIGHT)==0)){
     //   arduboy.drawBitmap(x,y,empty[frames&gray],8,1); 
      //} 
      if((shape&B_TOP)!=0){
        arduboy.drawBitmap(x,y,top   ,8,1);  
      }
      if((shape&B_BOTTOM)!=0){
        arduboy.drawBitmap(x,y,bottom,8,1);  
      }
      if((shape&B_LEFT)!=0){
        arduboy.drawBitmap(x,y,left  ,8,1);  
      }
      if((shape&B_RIGHT)!=0){
        arduboy.drawBitmap(x,y,right ,8,1);  
      }
    }
}

  static void removeRow(Level *level,unsigned char v){
    tunes.playScore(poot);
    if(v < level->breaker)level->breaker--;
    if(level->breaker == 0){
      focused.block_speed = 60;
      focused.cooldown = 120;
      tunes.playScore(win);
      //paused = true;
      levelNum++;
      level->loadLevel(&breakers[levelNum]);
      return;
      }
    focused.block_speed-=3;
    for(int y = v; y < level->block_height; y++){
      for(int x = 0; x < level->block_width;x++){
        if((y+1)<level->block_height)
          level->blocks[y][x]=level->blocks[y+1][x];
        else
          level->blocks[y][x]=0;
      }
    }
  }
  static void drawLevel(Level *level){
    for(int y = 0; y < level->block_height;y++){
      unsigned char completed = 0;
        for(int x = 0; x < level->block_width;x++){
          unsigned char block =  level->blocks[y][x];
          if((block&B_SOLID)==B_SOLID)completed++;
          drawBlock(level->x+(x*8),level->y+(y*8),block);
          if(completed==level->block_width)removeRow(level,y);
        }
      }
  }
void updateGame(){
    arduboy.pollButtons();
    if(arduboy.justPressed(UP_BUTTON)){
      tunes.playScore(poot);
      paused^=true;
    }
    if(!paused){
      if(arduboy.pressed(DOWN_BUTTON)){
        if(arduboy.justPressed(DOWN_BUTTON))tunes.playScore(blop);
        focused.cooldown-=2;
        if((getBlock(focused.x,focused.y,&level)&B_SOLID)!=0){
          focused.placeBlock(&level);
            focused.shape = preview;
            preview = resetBlock();
        }
      }
      if(arduboy.justPressed(LEFT_BUTTON)){
        if(!collides(focused.shape,getBlock(focused.x-1,focused.y,&level))){
          tunes.playScore(blop);
          focused.x--;
        }
      }
      if(arduboy.justPressed(RIGHT_BUTTON)){
        tunes.playScore(blop);
        if(!collides(focused.shape,getBlock(focused.x+1,focused.y,&level)))focused.x++;    
      }
      if(arduboy.justPressed(A_BUTTON)){
        tunes.playScore(blop);
        focused.try_rotate(&level);
      }
      if(arduboy.justPressed(B_BUTTON)){
        tunes.playScore(blop);
        unsigned char tmp = preview;
        preview = store;
        store = tmp;
        
      }
      else{
        //update the block
        if(focused.cooldown<=0){
          focused.cooldown = focused.block_speed;
            if(!collides(focused.shape,getBlock(focused.x,focused.y-1,&level))){
              focused.y--;
            }
            else{
             
              focused.placeBlock(&level);
              focused.shape = preview;
              preview = resetBlock();
              
            }
        }
        focused.cooldown--;
        score--;
        if(score == 0)runGameOver();
      }
    }
    
    if(levelNum == levels){runGameOver();}
}
void runGame(){
  if(!gameOver)updateGame();
  if(!paused){
  focused.draw();
  //draw preview like ass untill i implement big blocks
  drawBlock(8*13+4,8*5+4,preview);
  //draw stored also like ass 
  drawBlock(8*13+4,8*1+4,store);

  }
  drawLevel(&level);
  //draw top and bottom borders
  for(int i = 0; i < 10 ; i+=3){
      if(i==6){
        arduboy.drawBitmap((8*i)+8*5,0,crossTop    [frames&gray],8,1);
        arduboy.drawBitmap((8*i)+8*5,60,crossBottom[frames&gray],8,1);
        i++;
      }
      arduboy.drawBitmap((8*i)+8*5,0,border [frames&gray],8*3,1);
      arduboy.drawBitmap((8*i)+8*5,60,border[frames&gray],8*3,1);
  }
  for(int i = 0; i < 8 ; i++){
        arduboy.drawBitmap(15*8,i*8,column[frames&gray],8,1);
  }
  arduboy.drawBitmap(11*8,3*8+4,crossMid[frames&gray],8,1);
  arduboy.drawBitmap(15*8,3*8+4,crossMid[frames&gray],8,1);
  //line that shit up
 
  arduboy.drawLine(11*8,3,11*8,(8*3)+4,WHITE);
  arduboy.drawLine(12*8-1,3,12*8-1,(8*3)+4,WHITE);
  
  arduboy.drawLine(11*8,8*4+3,11*8,(8*7)+4,WHITE);
  arduboy.drawLine(12*8-1,8*4+3,12*8-1,(8*7)+4,WHITE);
  
  arduboy.drawLine(12*8,3*8+4,15*8,3*8+4,WHITE);
  arduboy.drawLine(12*8,4*8+3,15*8,4*8+3,WHITE);

      //draw UI data
  //writing tt seems to murder the performance so ill probably
  //replace this all with bitmaps too
  arduboy.setCursor(2,8);
  arduboy.print(F("Score"));
  arduboy.setCursor(2,16);
  arduboy.print(score);
  
  arduboy.setCursor(2,32);
  arduboy.print(F("Time"));
  arduboy.setCursor(2,40);
  arduboy.print(arduboy.cpuLoad());
}
void setup() {
  arduboy.begin();
  arduboy.setFrameRate(60);
  arduboy.clear();
  arduboy.display();
  
  tunes.initChannel(PIN_SPEAKER_1);
  #ifndef AB_DEVKIT
    tunes.initChannel(PIN_SPEAKER_2);
  #else
    tunes.initChannel(PIN_SPEAKER_1);
    tunes.toneMutesScore(true);
  #endif
  arduboy.initRandomSeed();
  level.loadLevel(&breakers[levelNum]);
  focused.shape = resetBlock();
  preview = resetBlock();
  store = resetBlock();
}
void loop() {
  if (!(arduboy.nextFrame()))
    return;
  frames++;
  arduboy.clear();
  switch(screen){
    case 0:
      //runMenu();
      break;
    case 1:
      runGame();
      break;
        
  }
  

  
  
  random(0,3);
  arduboy.display();
}
