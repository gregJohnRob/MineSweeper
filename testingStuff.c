/* this folder is just a collection of different things that have been tried
 * while I have been working on this and is not ment to be compiled.

*/


/* an alternative to the standard populate function. Basically loops over each
 * and will determine randomly if it should be a mine or not. Runs at about the
 * same rate however clustering can be an issue if the minefield is really large,
 * in a 16 by 16 field with 40 mines, it had to be reduced to a 10% chance of
 * a point becoming a mine for the whole field to be traversed, and I imagine
 * that would only get worse as the number of mines decreased as a percentage
 * of the size of the field.
*/
void alternativePopulate(){
  srand(time(NULL));
  int i = 0;
  int x;
  int y;
  int chance;
  while (i < numberOfMines) {
    for (x = 0; x < xDim; x++) {
      for (y = 0; y < yDim; y++) {
        chance = rand() % 100;
        if (chance < 10) {
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
          i++;
          if (i >= numberOfMines){
            break;
          }
        }
      }
    }
  }
}
