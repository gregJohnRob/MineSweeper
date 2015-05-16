// compile using gcc MineSweeper.c -lncurses -o minesweeper
/*
 * TODO: allow the user to specify a size and number of mines: will look at pointers and 2D arrays
 * TODO: add in error checking (for example make sure the user types in two numbers for a point
 * TODO: Make code neater
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>

// you can change the size of the mine field and the number of mines
// note however the higher the mine to free space ratio, the slower populate will run
// also make sure that the xDim and yDim match the dimensions of mine and displayMine
static int xDim = 16;
static int yDim = 16;
static int numberOfMines = 100;
static int mine[16][16] = {0};
static int mineValue = 100;
static int displayMine[16][16] = {0};


void populate(){
  srand(time(NULL));
  int x = rand() % xDim;
  int y = rand() % yDim;
  int i;
  for (i = 0; i < numberOfMines; i++){
    while(mine[x][y] == mineValue){
      x = rand() % xDim;
      y = rand() % yDim;
    }
    mine[x][y] = mineValue;
    int xtemp;
    int ytemp;
    for (xtemp = x - 1; xtemp < x + 2 && xtemp < xDim; xtemp++) {
      for (ytemp = y - 1; xtemp >= 0 && ytemp < y + 2 && ytemp < yDim; ytemp++) {
        if (ytemp >= 0 && mine[xtemp][ytemp] != mineValue) {
          mine[xtemp][ytemp]++;
        }
      }
    }
  }
}


// the display function that can be used if you want to test out different things
void displayForTesting(){
  int x;
  int y;
  int largestValue = 0;
  printw("      ");
  for (x = 0; x < xDim; x++) {
    if (x < 10) {
      printw("00%d ", x);
    } else {
      printw("0%d ", x);
    }
  }
  printw("\n \n");
  for (y = 0; y < yDim; y++) {
    if (y < 10) {
      printw("00%d   ", y);
    } else {
      printw("0%d   ", y);
    }
    for (x = 0; x < xDim; x++) {
      if (mine[x][y] > largestValue && mine[x][y] != mineValue){
        largestValue = mine[x][y];
      }
      if (mine[x][y] < 10) {
        printw("00%d ", mine[x][y]);
      } else if (mine[x][y] < 100){
        printw("0%d ", mine[x][y]);
      } else {
        printw("%d ", mine[x][y]);
      }
    }
    printw("\n");
  }
  printw("the largest grouping is: %d\n", largestValue);
}

void displayLastMove(int x, char point[]){
  switch(x){
  case 1:
    printw("You looked at %s\n", point);
    break;
  case 2:
    printw("You flagged %s\n", point);
    break;
  default:
    break;
  }
}

void display(){
  int x;
  int y;
  printw("      ");
  for (x = 0; x < xDim; x++) {
    if (x < 10) {
      printw("00%d ", x);
    } else {
      printw("0%d ", x);
    }
  }
  printw("\n \n");
  for (y = 0; y < yDim; y++) {
    if (y < 10) {
      printw("00%d   ", y);
    } else {
      printw("0%d   ", y);
    }
    for (x = 0; x < xDim; x++) {
      if (displayMine[x][y] == 0){
        printw("??? ");
      } else if (displayMine[x][y] == 1) {
        if (mine[x][y] < 10) {
          printw("00%d ", mine[x][y]);
        } else if (mine[x][y] < 100){
          printw("0%d ", mine[x][y]);
        }
      } else if (displayMine[x][y] == 2) {
        printw("fla ");
      }
    }
    printw("\n");
  }
}

void show(int x, int y) {
  if (x < 0 || y < 0 || x >= xDim || y >= yDim || mine[x][y] == mineValue || displayMine[x][y] != 0) {
    return;
  } else if (mine[x][y] != 0) {
    displayMine[x][y] = 1;
    return;
  } else {
    displayMine[x][y] = 1;
    int i;
    int j;
    for (i = x - 1; i < x + 2; i++) {
      for (j = y - 1; j < y + 2; j++) {
        show(i, j);
      }
    }
  }
}

int check(){
  int x;
  int y;
  for(x = 0; x < xDim; x++) {
    for (y = 0; y < yDim; y++){
      if (displayMine[x][y] == 0) {
        return 1;
      } else if (displayMine[x][y] == 2 && mine[x][y] != mineValue) {
        return 1;
      }
    }
  }
  return 0;
}

int main(void) {
  initscr();
  populate();
  int x;
  int y;
  char command[10];
  int  lastCommand = 0;

  while(1==1){
    clear();
    printw("commands: \n l: check a location \n e: exit the game \n f: flag a location\n");
    printw("When writing coordinates write as 'x y' where x and y are the x and y coordinates and %s",
              "are positive and less than 16\n");
    display();
    displayLastMove(lastCommand, command);
    printw("What would you like to do?\n ");
    refresh();
    getstr(command);
    if (strncmp(command, "e", 1) == 0) {
      printw("Bye\n");
      refresh();
      getstr(command);
      break;
    } else if (strncmp(command, "l", 1) == 0) {
      lastCommand = 1;
      printw("Where would you like to look?\n");
      refresh();
      getstr(command);
      sscanf(command, "%d %d", &x, &y);
      if (mine[x][y] == mineValue) {
        printw("You are dead\nWould you like to restart? Y/N\n");
        refresh();
        getstr(command);
        if (strncmp(command, "Y", 1) == 0) {
          memset(displayMine, 0, sizeof(displayMine));
          memset(mine, 0, sizeof(mine));
          populate();
          lastCommand = 0;
        } else {
          break;
        }
      } else {
        show(x, y);
      }
    } else if (strncmp(command, "f", 1) == 0) {
      lastCommand = 2;
      printw("Where would you like to flag?\n");
      getstr(command);
      refresh();
      sscanf(command, "%d %d", &x, &y);
      if (displayMine[x][y] == 0) {
        displayMine[x][y] = 2;
      } else if (displayMine[x][y] == 2) {
        displayMine[x][y] = 0;
      }
    }
    if (check() == 0) {
      printw("You win!\n");
      refresh();
      break;
    }
  }
  endwin();
  return 0;
}
