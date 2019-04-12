#ifndef LOGIC_H
#define LOGIC_H

#include "gba.h"

#define OBJ_WIDTH 6
#define MOVE_UP 1
#define MOVE_DOWN 2
#define MOVE_RIGHT 3
#define MOVE_LEFT 4

typedef struct {
    int x;
    int y;
    int width;
    volatile u16 color;
} Diver;

typedef struct {
    int x;
    int y;
    int width;
    volatile u16 color;
    int direction;
} Shark;


typedef struct {
   int x;
   int y;
} Location;

enum Stage {
    FIRST_STAGE,
    SECOND_STAGE
};

typedef struct {
    // Store whether or not the game is over in this member:
    int gameOver;
    int cntGoal;

    /*
    * TA-TODO: Add any logical elements you need to keep track of in your app.
    *
    * For example, for a Snake game, those could be:
    *
    * Snake snake;
    * Food foods[10];
    * int points;
    *
    */
    Diver diver;
    Shark *shark;
    enum Stage stage;
    //Map map;

} AppState;

/*
* TA-TODO: Add any additional structs that you need for your app.
*
* For example, for a Snake game, one could be:
*
* typedef struct {
*   int heading;
*   int length;
*   int x;
*   int y;
* } Snake;
*
*/

// This function can initialize an unused AppState struct.
void initializeAppState(AppState *appState);

// This function will be used to process app frames.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow);

// If you have anything else you need accessible from outside the logic.c
// file, you can add them here. You likely won't.

#endif
