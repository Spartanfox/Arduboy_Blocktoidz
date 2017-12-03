#include <Arduboy2.h>
#include "Sprites.h"
Arduboy2 arduboy;

boolean gray;
unsigned char frames = 0;
struct Block;
struct Level{
  static const unsigned char x = 5*8;
  static const unsigned char y = 0;
  static const unsigned char block_width = 6;
  static const unsigned char block_height = 7;
  unsigned char blocks[block_height][block_width];
} level;

static boolean collides(unsigned char,unsigned char);
static 
static unsigned char getBlock(int,int,Level*);
static void resetBlock(Block*);

struct Block{
  unsigned char x = 0;
  unsigned char y = 0;
  unsigned char shape;
  char cooldown;
  unsigned char block_speed = 120;
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
      level->blocks[y][x] |= shape;
      resetBlock(this);
  }
  void drawBlock(){
      drawBlock(focused.x*8+level.x,focused.y*8,focused.shape);
    }
} focused;
static void resetBlock(Block *block){
      block->shape =0 ;
  unsigned char tris = 0;
   do{
    if(random(0,1)==0){
        block->shape |= B_TOP;
        tris++;
    }
    if(random(0,2)==0){
        block->shape |= B_RIGHT;
        tris++;
    }
    if(random(0,3)==0&&tris!=2){
        block->shape |= B_BOTTOM;
        tris++;
    }
    if(random(0,2)==0&&tris!=2){
        block->shape |= B_LEFT;
    }
  }
  while(block->shape==0);
  block->y = level.block_height;
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
    }else if((shape&B_SOLID)==0){
      if(gray)arduboy.drawBitmap(x,y,empty,8,1); 
    }
    
    else{
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
    focused.block_speed--;
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
          drawBlock(level->x+x*8,level->y+y*8,block);
          if(completed==level->block_width)removeRow(level,y);
        }
      }
  }
void updateGame(){
    arduboy.pollButtons();
    if(arduboy.pressed(DOWN_BUTTON)){
      focused.cooldown-=2;
      if((getBlock(focused.x,focused.y,&level)&B_SOLID)!=0)focused.placeBlock(&level);
        
    }
    if(arduboy.justPressed(LEFT_BUTTON)){
      if(!collides(focused.shape,getBlock(focused.x-1,focused.y,&level)))focused.x--;    
    }
    if(arduboy.justPressed(RIGHT_BUTTON)){
      if(!collides(focused.shape,getBlock(focused.x+1,focused.y,&level)))focused.x++;    
    }
    if(arduboy.justPressed(A_BUTTON)){
      focused.try_rotate(&level);
    }
    if(arduboy.justPressed(B_BUTTON)){
      //placeBlock(&focused,&level); 
    }
    
    if(focused.cooldown<=0){
      focused.cooldown = focused.block_speed;
        if(!collides(focused.shape,getBlock(focused.x,focused.y-1,&level))){
          focused.y--;
        }
        else{
          focused.placeBlock(&level);
        }
    }
    focused.cooldown--;
}
void setup() {
  arduboy.begin();
  arduboy.setFrameRate(120); //Framerate adjusts gamespeed
  arduboy.clear();  //Clear screen
  arduboy.display();
  srand(7/8);
  resetBlock(&focused);
  //if you want random starting blocks
  for(int y = 0; y < level.block_height/2;y++){
    for(int x = 0; x < level.block_width;x++){
    //  level.blocks[y][x] = (1 + rand() % 255)&(1 + rand() % 255);
    }
  }
}
void loop() {
    if (!(arduboy.nextFrame()))
    return;
  gray^=true;
  arduboy.clear();
  
  updateGame();
  focused.drawBlock();
  drawLevel(&level);
  arduboy.display();
}
