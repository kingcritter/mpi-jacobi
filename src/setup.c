#include <stdio.h>
#include <stdlib.h>

#include "jacobi.h"
#include "setup.h"

void InitializeGrids()
{
  size_t i, j;

  for (i = 0; i < y; i++)
  {
    for (j = 0; j < x; j++)
    {
      grid1[idx(x, i ,j)] = 0.0;
      grid2[idx(x, i, j)] = 0.0;
    }
  }

  for (i = 0; i < y; i++)
  {
    grid1[idx(x, i, 0)] = 1.0;
    grid1[idx(x, i, xm1)] = 1.0;
    grid2[idx(x, i, 0)] = 1.0;
    grid2[idx(x, i, xm1)] = 1.0;
  }

  for (j = 0; j < x; j++)
  {
    grid1[idx(x, 0, j)] = 1.0;
    grid2[idx(x, 0, j)] = 1.0;
    grid1[idx(x, ym1, j)] = 1.0;
    grid2[idx(x, ym1, j)] = 1.0;
  }
}

void ReadFromStdIn() {
  size_t i, j;
  double d;
  for (i = 0; i < y; i++) {
    for (j = 0; j < x; j++) {
      scanf("%lg",&d);
      //      printf("%lg\n",d);
      grid1[idx(x, i, j)]=d;
      /* put the boundary values into the second grid as well 
   they never change      */
      if ((j==0) || (j==x-1) || (i==0) || (i==y-1))
      {
        grid2[idx(x, i, j)]=grid1[idx(x, i, j)];
      } else {
        grid2[idx(x, i, j)]=0.0;
      } // don't really need to do this but I want clean numbers to debug output
    }
  }
}

void CalculateSlices()
{
  size_t ym2, id, first, last, slice, resize;

  ym2 = y - 2;
  slice = ((ym2 - 1) / numWorkers) + 1;
  resize = (numWorkers - (ym2 % numWorkers)) % numWorkers;
  last = 1;

  for (id = 0; id < numWorkers; id++)
  {
    first = last;
    last = first + slice;

    if (resize > 0)
    {
      last -= 1;
      resize--;
    }

    firstY[id] = first;
    lastY[id] = last;

#ifdef _DEBUG
    printf("Slice %lu: %lu - %lu\n", id, firstY[id], lastY[id]);
#endif
  }
}
