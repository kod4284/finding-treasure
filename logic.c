#include "logic.h"
#include "stdlib.h"
#include <stdio.h>

#include "images/stageOne.h"
#include "images/stageTwo.h"

AppState cpyState(AppState* appState);
int checkSharkCollided(Shark shark, enum Stage stage);
int checkDiverCollided(AppState *appState, int direction);
int checkGameover(Shark *sharks, Diver *diver);
int checkGoalPassed(enum Stage stage, Diver *diver);
u16 detectColor(int x, int y, const u16 *image);

void initializeAppState(AppState* appState) {
        // TA-TODO: Initialize everything that's part of this AppState struct here.
        // Suppose the struct contains random values, make sure everything gets
        // the value it should have when the app begins.
        appState->gameOver = 0;
        appState->cntGoal = 0;
        appState->stage = FIRST_STAGE;
}

// TA-TODO: Add any process functions for sub-elements of your app here.
// For example, for a snake game, you could have a processSnake function
// or a createRandomFood function or a processFoods function.
//
// e.g.:
// static Snake processSnake(Snake* currentSnake);
// static void generateRandomFoods(AppState* currentAppState, AppState* nextAppState);

// This function processes your current app state and returns the new (i.e. next)
// state of your application.
AppState processAppState(AppState *currentAppState, u32 keysPressedBefore, u32 keysPressedNow) {
        /* TA-TODO: Do all of your app processing here. This function gets called
         * every frame.
         *
         * To check for key presses, use the KEY_JUST_PRESSED macro for cases where
         * you want to detect each key press once, or the KEY_DOWN macro for checking
         * if a button is still down.
         *
         * To count time, suppose that the GameBoy runs at a fixed FPS (60fps) and
         * that VBlank is processed once per frame. Use the vBlankCounter variable
         * and the modulus % operator to do things once every (i) frames. Note that
         * you want to process button every frame regardless (otherwise you will
         * miss inputs.)
         *
         * Do not do any drawing here.
         *
         * TA-TODO: VERY IMPORTANT! READ THIS PART.
         * You need to perform all calculations on the currentAppState passed to you,
         * and perform all state updates on the nextAppState state which we define below
         * and return at the end of the function. YOU SHOULD NOT MODIFY THE CURRENTSTATE.
         * Modifying the currentAppState will mean the undraw function will not be able
         * to undraw it later.
         */
        UNUSED(keysPressedNow);
        UNUSED(keysPressedBefore);

        AppState nextAppState = cpyState(currentAppState);

        // move per every flame
        int timeOfMove = vBlankCounter % 1;

        Shark *shark = nextAppState.shark;
        int sharkCount = sizeof(shark);

        for (int i = 0; i < sharkCount; i++) {
                int direction = shark[i].direction;
                switch (direction) {
                case MOVE_UP:
                        if (!timeOfMove) {
                          (shark[i].y) -= 2;
                        }
                        if (!timeOfMove
                          && checkSharkCollided(shark[i], nextAppState.stage)) {
                          shark[i].direction = MOVE_DOWN;
                        }
                        break;
                case MOVE_DOWN:
                        if (!timeOfMove) {
                          (shark[i].y) += 2;
                        }
                        if (!timeOfMove
                          && checkSharkCollided(shark[i], nextAppState.stage)) {
                          shark[i].direction = MOVE_UP;
                        }
                        break;
                case MOVE_RIGHT:
                        if (!timeOfMove) {
                          (shark[i].x) += 2;
                        }
                        if (!timeOfMove
                          && checkSharkCollided(shark[i], nextAppState.stage)) {
                          shark[i].direction = MOVE_LEFT;
                        }
                        break;
                case MOVE_LEFT:
                        if (!timeOfMove) {
                          (shark[i].x) -= 2;
                        }
                        if (!timeOfMove
                          && checkSharkCollided(shark[i], nextAppState.stage)) {
                          shark[i].direction = MOVE_RIGHT;
                        }
                        break;
                }
        }

        //Update location of diver and check for collision
        Diver *diver = &(nextAppState.diver);

        if (KEY_DOWN(BUTTON_UP, BUTTONS)) {
                if (!timeOfMove
                  && !checkDiverCollided(&nextAppState, MOVE_UP)) {
                        diver->y--;
                }
        } else if (KEY_DOWN(BUTTON_DOWN, BUTTONS)) {
                if (!timeOfMove
                  && !checkDiverCollided(&nextAppState, MOVE_DOWN)) {
                    diver->y++;
                  }
        } else if (KEY_DOWN(BUTTON_RIGHT, BUTTONS)) {
                if (!timeOfMove
                  && !checkDiverCollided(&nextAppState, MOVE_RIGHT)) {
                    diver->x++;
                }
        } else if (KEY_DOWN(BUTTON_LEFT, BUTTONS)) {
                if (!timeOfMove
                  && !checkDiverCollided(&nextAppState, MOVE_LEFT)) {
                    diver->x--;
                }
        }

        nextAppState.gameOver = checkGameover(nextAppState.shark, diver);
        nextAppState.cntGoal = checkGoalPassed(nextAppState.stage, diver);
        return nextAppState;
}
//This method check if the game is over
int checkGameover(Shark *sharks, Diver *diver) {
        int numOfSharks = sizeof(sharks);
        int width = diver->width;
        int xCord = diver->x;
        int yCord = diver->y;
        Location diverCords[4] = {
                {.x = xCord, .y = yCord},
                {.x = xCord + width, .y = yCord},
                {.x = xCord + width, .y = yCord + width},
                {.x = xCord, .y = yCord + width}
        };

        for (int i = 0; i < numOfSharks; i++) {
                int xCord2 = sharks[i].x;
                int yCord2 = sharks[i].y;
                Location sharkCords[4] = {
                        {.x = xCord2, .y = yCord2},
                        {.x = xCord2 + width, .y = yCord2 + width},
                };
                for (int m = 0; m < 4; m++) {
                        if (diverCords[m].x > sharkCords[0].x
                          && diverCords[m].x < sharkCords[1].x
                          && diverCords[m].y > sharkCords[0].y
                          && diverCords[m].y < sharkCords[1].y) {
                                return 1;
                        }
                }
        }
        return 0;
}
//This method copy the state
AppState cpyState(AppState* appState) {
        AppState curState;
        curState.diver = appState->diver;
        curState.stage = appState->stage;

        int sharkSize = sizeof(appState->shark);
        curState.shark = malloc(sizeof(Shark) * sharkSize);
        for (int x = 0; x < sharkSize; x++) {
                curState.shark[x] = appState->shark[x];
        }

        return curState;
}
//This method check if the diver passes the goal
int checkGoalPassed(enum Stage stage, Diver *diver) {
        if (stage == FIRST_STAGE) {
          int goalCord = 208 - diver->width;
          return diver->x > goalCord;
        } else if (stage == SECOND_STAGE) {
          int goalCord = 140 - diver->width;
          return diver->y > goalCord;
        } else {
                return 0;
        }
}
//This method makes first stage
void makeFirstStage(AppState *appState) {
        appState->gameOver = 0;
        appState->cntGoal = 0;

        Diver *diver = &(appState->diver);
        diver->x = 15;
        diver->y = 75;
        diver->width = OBJ_WIDTH;
        diver->color = YELLOW;

        appState->shark = malloc(sizeof(Shark) * 4);

        int xCord[4] = {60, 100, 140, 180};
        Shark *shark = appState->shark;

        int changeDirec = 0;
        for (int i = 0; i < 4; i++) {
                shark[i].x = xCord[i];
                shark[i].y = 80;
                shark[i].width = OBJ_WIDTH;
                shark[i].color = RED;
                if (!changeDirec) {
                        shark[i].direction = MOVE_DOWN;
                        changeDirec = 1;
                } else {
                        shark[i].direction = MOVE_UP;
                        changeDirec = 0;
                }

        }

        appState->stage = FIRST_STAGE;
}
//This method makes second stage
void makeSecondStage(AppState *appState) {
        appState->gameOver = 0;
        appState->cntGoal = 0;

        Diver *diver = &(appState->diver);
        diver->x = 120;
        diver->y = 20;
        diver->color = YELLOW;
        diver->width = OBJ_WIDTH;

        Shark *preShark = appState->shark;
        free(preShark);
        appState->shark = malloc(sizeof(Shark) * 4);


        int yCord[4] = {50, 70, 90, 110};
        Shark *shark = appState->shark;

        int changeDirec = 0;
        for (int i = 0; i < 4; i++) {
                shark[i].x = 120;
                shark[i].y = yCord[i];
                shark[i].width = OBJ_WIDTH;
                shark[i].color = RED;
                if (!changeDirec) {
                        shark[i].direction = MOVE_RIGHT;
                        changeDirec = 1;
                } else {
                        shark[i].direction = MOVE_LEFT;
                        changeDirec = 0;
                }
        }

        appState->stage = SECOND_STAGE;
}

//This method makes a shark is not collieded with wall
int checkSharkCollided(Shark shark, enum Stage stage) {
        if (stage == FIRST_STAGE) {
          int sharkMoveMax = 145 - shark.width;
          int sharkMoveMin = shark.y < 25;
          return shark.y > sharkMoveMax || sharkMoveMin;
        } else if (stage == SECOND_STAGE) {
          int sharkMoveMax = 190 - shark.width;
          int sharkMoveMin = shark.x < 45;
          return shark.x > sharkMoveMax || sharkMoveMin;
        }
        return 0;
}

//This method makes a diver is not collieded with wall
int checkDiverCollided(AppState *appState, int direction) {
        Diver *diver = &(appState->diver);
        int stage = appState->stage;
        int width = diver->width;

        const u16 *image;
        if (stage == FIRST_STAGE) {
          image = stageOne;
        } else if (stage == SECOND_STAGE) {
          image = stageTwo;
        } else {
          image = stageOne;
        }
        int diverX = diver->x;
        int diverY = diver->y;
        switch (direction) {
        case MOVE_UP:
                return (detectColor(diverX, diverY-1, image) == 0) ||
                       (detectColor(diverX+width-1, diverY-1, image) == 0);
                break;
        case MOVE_DOWN:
                return (detectColor(diverX, diverY+width, image) == 0) ||
                       (detectColor(diverX+width-1, diverY+width, image) == 0);
                break;
        case MOVE_RIGHT:
                return (detectColor(diverX+width, diverY, image) == 0) ||
                       (detectColor(diverX+width, diverY+width-1, image) == 0);
                break;
        case MOVE_LEFT:
                return (detectColor(diverX-1, diverY, image) == 0) ||
                       (detectColor(diverX-1, diverY+width-1, image) == 0);
                break;
        }
        return 0;
}
