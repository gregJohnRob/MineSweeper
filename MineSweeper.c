// compile using gcc MineSweeper.c -lncurses -o minesweeper
/*
 * TODO: add in error checking (for example make sure the user types in two numbers for a point
 * TODO: Make code neater
 */
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ncurses.h>
typedef struct {
  int value;
  int displayValue;
} Position;


Position** createMineField(int xDimension, int yDimension) {
  Position** mineField;
  mineField = (Position**) malloc(xDimension*sizeof(Position*));
  int i;
  int j;
  for (i = 0; i < xDimension; i++) {
    mineField[i] = (Position*) malloc(yDimension*sizeof(Position));
    for (j = 0; j < yDimension; j++) {
      (mineField[i] + j)->value = 0;
      (mineField[i] + j)->displayValue = 0;
    }
  }
  return mineField;
}
/*
Summary: If you have a multidimensional array defined as int [][], then x = y[a][b] is equivalent to x = *((int *)y + a * NUMBER_OF_COLUMNS + b);
*/
void populate(Position** mineField,
              int xDimension, int yDimension,
              int numberOfMines, int mineValue) {

  srand(time(NULL));
  int x = rand() % xDimension;
  int y = rand() % yDimension;
  int i;
  for (i = 0; i < numberOfMines; i++){
    while((mineField[x] + y)->value == mineValue){
      x = rand() % xDimension;
      y = rand() % yDimension;
    }
    (mineField[x] + y)->value = mineValue;
    int xtemp;
    int ytemp;
    for (xtemp = x - 1; xtemp < x + 2 && xtemp < xDimension; xtemp++) {
      for (ytemp = y - 1; xtemp >= 0 && ytemp < y + 2 && ytemp < yDimension; ytemp++) {
        if (ytemp >= 0 && (mineField[xtemp] + ytemp)->value != mineValue) {
          (mineField[xtemp] + ytemp)->value += 1;
        }
      }
    }
  }
}


// the display function that can be used if you want to test out different things
void displayForTesting(Position** mineField,
              int xDimension, int yDimension, int mineValue){
  int x;
  int y;
  int largestValue = 0;
  printw("      ");
  for (x = 0; x < xDimension; x++) {
    if (x < 10) {
      printw("00%d ", x);
    } else {
      printw("0%d ", x);
    }
  }
  printw("\n\n");
  for (y = 0; y < yDimension; y++) {
    if (y < 10) {
      printw("00%d   ", y);
    } else {
      printw("0%d   ", y);
    }
    for (x = 0; x < xDimension; x++) {
      if ((mineField[x] + y)->value > largestValue && (mineField[x] + y)->value != mineValue){
        largestValue = (mineField[x] + y)->value;
      }
      if ((mineField[x] + y)->value < 10) {
        printw("00%d ", (mineField[x] + y)->value);
      } else if ((mineField[x] + y)->value < 100){
        printw("0%d ", (mineField[x] + y)->value);
      } else {
        printw("%d ", (mineField[x] + y)->value);
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

void display(Position** mineField,
              int xDimension, int yDimension, int mineValue){
  int x;
  int y;
  printw("      ");
  for (x = 0; x < xDimension; x++) {
    if (x < 10) {
      printw("00%d ", x);
    } else {
      printw("0%d ", x);
    }
  }
  printw("\n \n");
  for (y = 0; y < yDimension; y++) {
    if (y < 10) {
      printw("00%d   ", y);
    } else {
      printw("0%d   ", y);
    }
    for (x = 0; x < xDimension; x++) {
      if ((mineField[x] + y)->displayValue == 0){
        printw("??? ");
      } else if ((mineField[x] + y)->displayValue == 1) {
        if ((mineField[x] + y)->value < 10) {
          printw("00%d ", (mineField[x] + y)->value);
        } else if ((mineField[x] + y)->value < 100){
          printw("0%d ", (mineField[x] + y)->value);
        }
      } else if ((mineField[x] + y)->displayValue == 2) {
        printw("fla ");
      }
    }
    printw("\n");
  }
}

void show(int x, int y, int xDimension, int yDimension, Position** mineField) {
  if (x < 0 || y < 0 || x >= xDimension || y >= yDimension || (mineField[x] + y)->displayValue != 0) {
    return;
  } else if ((mineField[x] + y)->value != 0) {
    (mineField[x] + y)->displayValue = 1;
    return;
  } else {
    (mineField[x] + y)->displayValue = 1;
    int i;
    int j;
    for (i = x - 1; i < x + 2; i++) {
      for (j = y - 1; j < y + 2; j++) {
        show(i, j, xDimension, yDimension, mineField);
      }
    }
  }
}

int check(Position** mineField, int xDimension, int yDimension, int mineValue){
  int x;
  int y;
  for(x = 0; x < xDimension; x++) {
    for (y = 0; y < yDimension; y++){
      if ((mineField[x] + y)->displayValue == 0) {
        return 1;
      } else if ((mineField[x] + y)->displayValue == 2 && (mineField[x] + y)->value != mineValue) {
        return 1;
      }
    }
  }
  return 0;
}

int main(void) {
  int mineValue = 100;
  int xDimension;
  int yDimension;
  int numberOfMines;
  int x;
  int y;
  char command[20];
  int  lastCommand = 0;

  initscr();
  printw("Enter the size of the mine field. Enter as: X Y \n");
  refresh();
  getstr(command);
  sscanf(command, "%d %d", &xDimension, &yDimension);
  Position** mineField = createMineField(xDimension, yDimension);
  printw("How many mines would you like? ");
  refresh();
  getstr(command);
  sscanf(command, "%d", &numberOfMines);
  populate(mineField, xDimension, yDimension, numberOfMines, mineValue);
  while(1==1){
    clear();
    printw("commands: \n l: check a location \n e: exit the game \n f: flag a location\n");
    printw("When writing coordinates write as 'x y' where x and y are the x and y coordinates and %s",
              "are positive and less than 16\n");
    display(mineField, xDimension, yDimension, mineValue);
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
      if ((mineField[x] + y)->value == mineValue && (mineField[x] + y)->displayValue != 2) {
        printw("You are dead\nWould you like to restart? Y/N\n");
        refresh();
        getstr(command);
        if (strncmp(command, "Y", 1) == 0) {
          mineField = createMineField(xDimension, yDimension);
          populate(mineField, xDimension, yDimension, numberOfMines, mineValue);
          lastCommand = 0;
        } else {
          break;
        }
      } else {
        show(x, y, xDimension, yDimension, mineField);
      }
    } else if (strncmp(command, "f", 1) == 0) {
      lastCommand = 2;
      printw("Where would you like to flag?\n");
      getstr(command);
      refresh();
      sscanf(command, "%d %d", &x, &y);
      if ((mineField[x] + y)->displayValue == 0) {
        (mineField[x] + y)->displayValue = 2;
      } else if ((mineField[x] + y)->displayValue == 2) {
        (mineField[x] + y)->displayValue = 0;
      }
    }
    if (check(mineField, xDimension, yDimension, mineValue) == 0) {
      printw("You win!\n");
      refresh();
      break;
    }
  }
  endwin();
  return 0;
}
