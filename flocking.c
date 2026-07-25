#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define NUMFISH 80
#define MAXSPEED 4
#define MINSPEED 1
#define MINSIZE 3
#define MAXSIZE 6
#define TURNFACTOR 0.4f //How much each boid wants to avoid the screen edge

#define VISUAL 95   //Range to see other fish
#define PROTECTED 15 //Range to avoid other fish

#define SEPARATION 0.05f //Parameter for separation
#define ALIGNMENT 0.05f //Parameter for alignment
#define COHESION 0.0005f //Parameter for cohesion

#define MARGIN 150 //How close to the screen edge they start being moved

// Create a struct to store each fishes position, velocity, and size
typedef struct {
  Vector2 pos;
  Vector2 vel;
  int size;
} fish;

fish fishSchool[NUMFISH];

void initFish(fish *f, int borderX, int borderY){
f->pos.x = GetRandomValue(0,borderX);
f->pos.y = GetRandomValue(0,borderY);
f->vel.x = GetRandomValue(-1 * MAXSPEED,MAXSPEED);
f->vel.y = GetRandomValue(-1 * MAXSPEED,MAXSPEED);
f->size = GetRandomValue(MINSIZE, MAXSIZE);
}

void accelerateFish(fish *current, fish *school, int bX, int bY){
  Vector2 sep = {0,0};
  Vector2 ali = {0,0};
  Vector2 coh = {0,0};

  int neighbours = 0;
  
  //loop though all other fish
  for(int i = 0; i < NUMFISH; i++){
    fish* other = &school[i];
    if(current == other) continue; //ignore self comparisson
 
    //find distance to the comparing fish
    float dx = current->pos.x - other->pos.x;
    float dy = current->pos.y - other->pos.y;

    int distSquare = (dx * dx) + (dy * dy);  

    //Separation
    //If the other fish is in the protected area, steer away
    if (distSquare < PROTECTED * PROTECTED){
      sep.x += dx;
      sep.y += dy;
    }

    //Alignment and Cohesion
    if (distSquare < VISUAL * VISUAL){
      //Alignment
      //swim in the same direction as other fish
      ali.x += other->vel.x;
      ali.y += other->vel.y;
      neighbours++;

      //Cohesion
      //swim towards the centre of mass of the other fish
      coh.x += other->pos.x;
      coh.y += other->pos.y;
    }
  }
  
  //Appy separation
  current->vel.x += sep.x * SEPARATION;
  current->vel.y += sep.y * SEPARATION;

  //Apply alignment and cohesion
  if(neighbours != 0) {
    //alignment
    //scale to appropriate value for the total neighbours
    ali.x = ali.x/neighbours;
    ali.y = ali.y/neighbours;

    current->vel.x += ali.x * ALIGNMENT;
    current->vel.y += ali.y * ALIGNMENT;

    //cohesion
    //scale to appropriate value for the total neighbours
    coh.x = coh.x/neighbours - current->pos.x;
    coh.y = coh.y/neighbours - current->pos.y;

    current->vel.x += coh.x * COHESION;
    current->vel.y += coh.y * COHESION;
  }

  //Apply turning at the screen edges to keep in frame
  if(current->pos.x < MARGIN){
    current->vel.x += TURNFACTOR; 
  } else if(current->pos.x > (bX - MARGIN)){
    current->vel.x -= TURNFACTOR;
  }

  if(current->pos.y < MARGIN){
    current->vel.y += TURNFACTOR; 
  } else if(current->pos.y > (bY - MARGIN)){
    current->vel.y -= TURNFACTOR;
  }

  // Apply speed limit
  float speed = sqrt((current->vel.x * current->vel.x) + 
                     (current->vel.y * current->vel.y));
  
  if(speed < 0.001f){ //If the speed is effectively 0, accelerate them
    current->vel.x = GetRandomValue(-MAXSPEED,MAXSPEED);
    current->vel.y = GetRandomValue(-MAXSPEED,MAXSPEED);
  } if (speed > MAXSPEED){ //Limit the speed if they are too fast
    current->vel.x = (current->vel.x)/speed * MAXSPEED;
    current->vel.y = (current->vel.y)/speed * MAXSPEED;
  } else if (speed < MINSPEED) { //Give them a minimum if they're too slow
    current->vel.x = (current->vel.x)/speed * MINSPEED;
    current->vel.y = (current->vel.y)/speed * MINSPEED;
  }
}

void moveFish(fish *f, int borderX, int borderY){
  // Move the position of the fish
  f->pos.x += f->vel.x;
  f->pos.y += f->vel.y;

  // Wraparound
  if (f->pos.x < -f->size){
    f->pos.x = borderX + f->size;
  } else if (f->pos.x > borderX + f->size){
    f->pos.x = -f->size;
  }

  if (f->pos.y < -f->size){
    f->pos.y = borderY + f->size;
  } else if (f->pos.y > borderY + f->size){
    f->pos.y = -f->size;
  }

}

void drawFish(fish *f){
  DrawCircleV(f->pos, f->size, BLUE);
}

int main(void){
  
  // Assign the border size
  const int borderX = 950;
  const int borderY = 700;

  // Create the fish
  for (int i = 0; i < NUMFISH; i++){
    initFish(&fishSchool[i], borderX, borderY);
  }

  // Create window with raylib
  InitWindow(borderX, borderY, "Fish School!");
  SetTargetFPS(60);

  // Draw each frame
  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Accelerate, move, and draw fish
    for(int i = 0; i < NUMFISH; i++){
      accelerateFish(&fishSchool[i], fishSchool, borderX, borderY);
      moveFish(&fishSchool[i], borderX, borderY);
      drawFish(&fishSchool[i]);
    } 
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}
