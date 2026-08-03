#include <stdio.h>
#include <math.h>
#include "raylib.h"

#define NUMBOIDS 80
#define MAXSPEED 4
#define MINSPEED 1
#define MINSIZE 3
#define MAXSIZE 6
#define TURNFACTOR 0.4f //How much each boid wants to avoid the screen edge

#define VISUAL 95   //Range to see other boids
#define PROTECTED 15 //Range to avoid other boids

#define SEPARATION 0.05f //Parameter for separation
#define ALIGNMENT 0.05f  //Parameter for alignment
#define COHESION 0.0005f //Parameter for cohesion

#define MARGIN 150 // How close to the screen edge they start being moved

// Create a struct to store each boids position, velocity, and size
typedef struct {
  Vector2 pos;
  Vector2 vel;
  int size;
} boid;

boid allBoids[NUMBOIDS];

void initBoid(boid *b, int borderX, int borderY){
b->pos.x = GetRandomValue(0,borderX);
b->pos.y = GetRandomValue(0,borderY);
b->vel.x = GetRandomValue(-1 * MAXSPEED,MAXSPEED);
b->vel.y = GetRandomValue(-1 * MAXSPEED,MAXSPEED);
b->size = GetRandomValue(MINSIZE, MAXSIZE);
}

void accelerateBoid(boid *current, boid *boids, int bX, int bY){
  Vector2 sep = {0,0};
  Vector2 ali = {0,0};
  Vector2 coh = {0,0};

  int neighbours = 0;
 
  for(int i = 0; i < NUMBOIDS; i++){
    boid* other = &boids[i];
    if(current == other) continue; //ignore self comparisson
 
    //find distance to the comparing boid
    float dx = current->pos.x - other->pos.x;
    float dy = current->pos.y - other->pos.y;

    int distSquare = (dx * dx) + (dy * dy);  

    //Separation
    //If the other boid is in the protected area, steer away
    if (distSquare < PROTECTED * PROTECTED){
      sep.x += dx;
      sep.y += dy;
    }

    //Alignment and Cohesion
    if (distSquare < VISUAL * VISUAL){
      //Alignment
      //move in the same direction as other boid
      ali.x += other->vel.x;
      ali.y += other->vel.y;
      neighbours++;

      //Cohesion
      //move towards the centre of mass of the other boid
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
  } if (speed > MAXSPEED){ //Limit the speed if boid is too fast
    current->vel.x = (current->vel.x)/speed * MAXSPEED;
    current->vel.y = (current->vel.y)/speed * MAXSPEED;
  } else if (speed < MINSPEED) { //Give them a minimum if they're too slow
    current->vel.x = (current->vel.x)/speed * MINSPEED;
    current->vel.y = (current->vel.y)/speed * MINSPEED;
  }
}

void moveBoid(boid *b, int borderX, int borderY){
  b->pos.x += b->vel.x;
  b->pos.y += b->vel.y;

  // Wraparound
  if (b->pos.x < -b->size){
    b->pos.x = borderX + b->size;
  } else if (b->pos.x > borderX + b->size){
    b->pos.x = -b->size;
  }

  if (b->pos.y < -b->size){
    b->pos.y = borderY + b->size;
  } else if (b->pos.y > borderY + b->size){
    b->pos.y = -b->size;
  }

}

void drawBoid(boid *b){
  //change later for proper images
  DrawCircleV(b->pos, b->size, BLUE);
}

int main(void){
  
  const int borderX = 950;
  const int borderY = 700;

  // Create the boids
  for (int i = 0; i < NUMBOIDS; i++){
    initBoid(&allBoids[i], borderX, borderY);
  }

  // Create window with raylib
  InitWindow(borderX, borderY, "Boids Simulation");
  SetTargetFPS(60);

  // Draw each frame
  while(!WindowShouldClose()){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    // Accelerate, move, and draw boids
    for(int i = 0; i < NUMBOIDS; i++){
      accelerateBoid(&allBoids[i], allBoids, borderX, borderY);
      moveBoid(&allBoids[i], borderX, borderY);
      drawBoid(&allBoids[i]);
    } 
    EndDrawing();
  }
  
  CloseWindow();
  return 0;
}
