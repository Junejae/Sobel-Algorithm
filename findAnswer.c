// Original code is from Dr. Niels Da Vitoria Lobo
// Modified by Junejae Kim
// 8/29/2020

#include <stdio.h>                          /* Sobel.c */
#include <math.h>
#include <stdlib.h> // added to use atoi()

// Define high and low thresholds
#define HI 110
#define LO 40

        int pic[256][256];
        int mag[256][256];
        int hi[256][256];
        int lo[256][256];
        int my[256][256];
        int mymag[256][256];
        int outpicx[256][256];
        int outpicy[256][256];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        double ival[256][256],maxival;

void makeImage(int threshold, int (*mag)[256], int (*my)[256])
{
  int i,j;

  for (i=0;i<256;i++)
    for(j=0;j<256;j++)
      my[i][j] = (mag[i][j] > threshold) ? 255 : 0;

  return;
}

int areTheyDifferent(int (*original)[256], int (*my)[256])
{
  int flag = 0;
  int i,j;

  for (i=0;i<256;i++)
  {
    for(j=0;j<256;j++)
    {
      flag = (original[i][j] == my[i][j]) ? 0 : 1;

      if (flag == 1) break;
    }

    if (flag == 1) break;
  }
    

  return flag;
}

int main(argc,argv)
int argc;
char **argv;
{
        int i,j,p,q,mr,sum1,sum2;
        int threshold,high,low,flag;
        FILE *fo0, *fo1, *fo2, *fp1, *fopen();
        char *foobar;

        printf("Getting files...\n");

        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

	      argc--; argv++;
	      foobar = *argv;
	      fo0=fopen(foobar,"rb");

        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"rb");

        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"rb");


        // Load the image
        for (i=0;i<15;i++) // cut off the header
          getc(fp1);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  pic[i][j]  =  getc (fp1);
                  pic[i][j]  &= 0377; // clean the data with 1111 1111
                }
        }

        for (i=0;i<15;i++) // cut off the header
          getc(fo0);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  mag[i][j]  =  getc (fo0);
                  mag[i][j]  &= 0377; // clean the data with 1111 1111
                }
        }

        for (i=0;i<15;i++) // cut off the header
          getc(fo1);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  hi[i][j]  =  getc (fo1);
                  hi[i][j]  &= 0377; // clean the data with 1111 1111
                }
        }

        for (i=0;i<15;i++) // cut off the header
          getc(fo2);

        for (i=0;i<256;i++)
        { for (j=0;j<256;j++)
                {
                  lo[i][j]  =  getc (fo2);
                  lo[i][j]  &= 0377; // clean the data with 1111 1111
                }
        }

        printf("Calculation starts...\n");

        // Scanning couvolution
        mr = 1; // set the radious of mask array
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             sum1 = 0;
             sum2 = 0;
             for (p=-mr;p<=mr;p++)
             {
                for (q=-mr;q<=mr;q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
             }
             outpicx[i][j] = sum1;
             outpicy[i][j] = sum2;
          }
        }

        printf("Calculation finish...\n");

        // Store the magnitude of the gradient & find the highest value to scale
        maxival = 0;
        for (i=mr;i<256-mr;i++)
        { for (j=mr;j<256-mr;j++)
          {
             ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) +
                                      (outpicy[i][j]*outpicy[i][j])));
             if (ival[i][j] > maxival)
                maxival = ival[i][j];
           }
        }

        printf("Mag image processing...\n");
        // Magnitude image
        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             mymag[i][j] = (ival[i][j] / maxival) * 255;
            }
          }

        if (areTheyDifferent(mag, mymag))
          return 0;
        
        printf("Start comparing...\n");
        // Check the hi and low thresholds

        flag = 0;

        while (flag == 0)
        {
          makeImage(threshold, mag, my);
          
          if(areTheyDifferent(lo,my) == 0)
          {
            flag = 1;
            low = threshold;
          }
          threshold++;
        }

        flag = 0;

        while (flag == 0)
        {
          makeImage(threshold, mag, my);
          
          if(areTheyDifferent(hi,my) == 0)
          {
            flag = 1;
            high = threshold;
          }
          
          threshold++;
        }
        

        // Close files
        fclose(fo0);
        fclose(fo1);
        fclose(fo2);
        fclose(fp1);

        printf("%d, %d\n", high, low);

        printf("All tasks are done successfully!\n");

        return 0;

}
