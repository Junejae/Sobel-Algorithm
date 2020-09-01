// Original code is from Dr. Niels Da Vitoria Lobo
// Modified by Junejae Kim
// 8/29/2020

#include <stdio.h>                          /* Sobel.c */
#include <math.h>
#include <stdlib.h> // added to use atoi()

// Define high and low thresholds
#define HI 109
#define LO 39

        int pic[256][256];
        int outpicx[256][256];
        int outpicy[256][256];
        int maskx[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}};
        int masky[3][3] = {{1,2,1},{0,0,0},{-1,-2,-1}};
        double ival[256][256],maxival;

int main(argc,argv)
int argc;
char **argv;
{
        int i,j,p,q,mr,sum1,sum2;
        double threshold;
        FILE *fo0, *fo1, *fo2, *fp1, *fopen();
        char *foobar;

        printf("Getting files...\n");

        argc--; argv++;
        foobar = *argv;
        fp1=fopen(foobar,"rb");

	      argc--; argv++;
	      foobar = *argv;
	      fo0=fopen(foobar,"wb");

        argc--; argv++;
        foobar = *argv;
        fo1=fopen(foobar,"wb");

        argc--; argv++;
        foobar = *argv;
        fo2=fopen(foobar,"wb");

        /* 
        argc--; argv++;
        foobar = *argv;
        threshold = atoi(foobar); // changed atof -> atoi
         */

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

        // Add header to see it on image viewer
        fprintf(fo0, "P5\n");
        fprintf(fo0, "%d %d\n", 256, 256);
        fprintf(fo0, "255\n");

        fprintf(fo1, "P5\n");
        fprintf(fo1, "%d %d\n", 256, 256);
        fprintf(fo1, "255\n");

        fprintf(fo2, "P5\n");
        fprintf(fo2, "%d %d\n", 256, 256);
        fprintf(fo2, "255\n");

        // Magnitude image
        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             ival[i][j] = (ival[i][j] / maxival) * 255;
             fprintf(fo0,"%c",(char)((int)(ival[i][j])));
            }
          }

        // Low threshold image
        threshold = LO;

        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             if (ival[i][j] > threshold)
             {
                fprintf(fo1,"%c",(char)255);
             }
             
             else
             {
                fprintf(fo1,"%c",(char)0);
             }
            }
          }
        
        // High threshold image
        threshold = HI;

        for (i=0;i<256;i++)
          { for (j=0;j<256;j++)
            {
             if (ival[i][j] > threshold)
             {
                fprintf(fo2,"%c",(char)255);
             }
             
             else
             {
                fprintf(fo2,"%c",(char)0);
             }
            }
          }
        

        // Close files
        fclose(fo0);
        fclose(fo1);
        fclose(fo2);
        fclose(fp1);

        printf("All tasks are done successfully!\n");

        return 0;

}
