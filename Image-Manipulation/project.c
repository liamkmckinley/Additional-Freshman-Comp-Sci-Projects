#include <stdio.h>
#include "ppm_io.h"
#include <stdlib.h>
#include "string.h"
#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))
#define M_PI 3.14159265358979323846
#include "math.h"
unsigned char saturate(double value)//clamp value of pixel
{
    return MIN(MAX(value, 0),255);
}
void modifyBrightness(Image *img, int value)//increse the value of each channel of color with a scalar
{
    for (int i = 0; i < img->rows; i++) {
        for (int j = 0; j < img->cols; j++) {//loop through pixels
            Pixel *pix=getPixel(img,i,j);
            pix->b=saturate((double)value+pix->b);
            pix->g=saturate((double)value+pix->g);
            pix->r=saturate((double)value+pix->r);

        }
    }

}
void convertGrayscale(Image *img)
{
    for (int i = 0; i < img->rows; ++i) {
        for (int j = 0; j < img->cols; ++j) {//loop through pixels
            Pixel *p=getPixel(img,i,j);
            unsigned char intensity=0.3*p->r+0.59*p->g+0.11*p->b;
            p->g=intensity;//set all channels equal to grayscale value
            p->b=intensity;
            p->r=intensity;
        }
    }
}
void deallocImage(Image *im)
{
    free(im->data);//used to free memory of image
    free(im);
}
Image *cropImagge(Image *im, int top_X,int top_Y,int bot_X,int bot_Y)
{
    int nrRows=(bot_X-top_X+1);//new size of cropped image
    int nrCols=(bot_Y-top_Y+1);
    Image *new=(Image*)malloc(sizeof(Image));
    new->rows=nrRows;
    new->cols=nrCols;
    new->data=(Pixel*)malloc(new->rows*new->cols*sizeof(Pixel));//create new image
    for (int i = 0; i < new->rows; ++i) {
        for (int j = 0; j < new->cols; ++j) {//loop and copy the cropped pixels
            Pixel *pix=getPixel(im,i+top_X,j+top_Y);
            setPixel(new,i,j,*pix);
        }
    }
    deallocImage(im);//delete old image
    return new;
}
double **GaussianKernel(double sigma,int w)
{


    double **kernel=(double**)malloc(w * sizeof(double*));//alloca kernel memory
    for (int i = 0; i < w; ++i) {
        kernel[i]=(double*)malloc(w*sizeof(double));

    }
    double mean = w/2;
    double sum = 0.0; // For accumulating the kernel values
    for (int x = 0; x < w; ++x)
        for (int y = 0; y < w; ++y) {//create kernel values
            kernel[x][y] = exp( -0.5 * (pow((x-mean)/sigma, 2.0) + pow((y-mean)/sigma,2.0)) )
                           / (2 * M_PI * sigma*sigma );

            // Accumulate the kernel values
            sum += kernel[x][y];
        }

// Normalize the kernel
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < w; ++y) {
            kernel[x][y] /= sum;
            printf("%f", kernel[x][y]);
        }
        printf("\n");
    }
    return kernel;
}
void convolvePixel(Image* im, int x,int y,double** kernel,int w)
{
    double r=0,g=0,b=0;
    double mean = w/2;
    for (int i = 0; i < w; ++i) {//loop into the kernell
        for (int j = 0; j < w; ++j) {
            int posX=x+i-mean;//absolute position if the pixel in the image
            int posY=y+j-mean;
            if(posX>-1 && posX<im->rows && posY>-1 && posY<im->cols) {
                Pixel *pix = getPixel(im, posX, posY);//if valid pixel coordinates
                r += pix->r * kernel[i][j];//multiply pixel values by kernel coeffs
                g += pix->g * kernel[i][j];
                b += pix->b * kernel[i][j];
            }
        }
    }
    Pixel *change=getPixel(im,x,y);//update new pixel values

    change->r=r;
    change->b=b;
    change->g=g;
}
void freeKernel(double **ker,int size)//dealloc kernel after usage
{
    for (int i = 0; i < size; ++i) {
        free(ker[i]);
    }
    free(ker);
}
void applyGauss(Image* im, double sigma)
{
    int w = ceil(10*sigma);
    if(w%2==0)
        w++;
    double **kernel=GaussianKernel(sigma,w);//loop through image pixels and apply the Gauss
    for (int i = 0; i < im->rows ; ++i) {
        for (int j = 0; j < im->cols ; ++j) {
            convolvePixel(im,i,j,kernel,w);
        }
    }
    freeKernel(kernel,w);
}
void Occlude(Image* im, int top_X,int top_Y,int bot_X,int bot_Y)
{
    for (int i = top_X; i < bot_X; ++i) {//loop through occluded pixels and make them black
        for (int j = top_Y ; j <bot_Y ; ++j) {
            Pixel* pix=getPixel(im,i,j);
            pix->r=0;
            pix->b=0;
            pix->g=0;
        }
    }
}

void CannyEdge(Image *im, int threshold)
{
    for (int i = 1; i < im->rows-1; ++i) {
        for (int j = 1; j < im->cols-1; ++j) {
            double grad_X=(getPixel(im,i+1,j)->b -getPixel(im,i,j)->b);//derivative on x axis
            double grad_Y=(getPixel(im,i,j+1)->b-getPixel(im,i,j)->b);//derivative on y axis
            double value=sqrt(pow(grad_X,2.0)+pow(grad_Y,2.0));//magnitude

            Pixel *pix=getPixel(im,i,j);
            if( value > threshold)//compare magnitude with threshold
            {
                pix->b=0;
                pix->g=0;
                pix->r=0;
            } else
            {
                pix->b=255;
                pix->g=255;
                pix->r=255;
            }
        }
    }
}

void blur(char* dest, int width, int height, double sigma)
{
		int	row, col, col_r, col_g, col_b, winsize, halfsize, k, offset, count = 0, rows, count1, count2, count3;
	double  row_g, row_b, row_r, col_all;
	unsigned char  r_r, r_b, r_g, c_all;
	unsigned char *tmp = NULL;
	double *kernel; 

		winsize = win_size(sigma); 
		kernel = GKernel_1D(winsize, sigma); 
		winsize *= 3; 
		halfsize = winsize / 2;

	if ((tmp = (char*)calloc(width * height, sizeof(char))) == NULL) 
		return -1;
	for (row = 0; row < height; row++)
	{ 
		col_r = 0;
		col_g = 1;
		col_b = 2;
		for (rows = 0; rows < width; rows+=3)
		{
			row_r = row_g = row_b = 0.0;
			count1 = count2 = count3 = 0;

			for (k = 1; k < winsize; k += 3)
			{
				if ((k + col_r - halfsize >= 0) && (k + col_r - halfsize < width))
				{
					r_r = *(src + row * width + col_r + k - halfsize);
					row_r += (int)(r_r)* kernel[count1];
					count1++;
				}

				if ((k + col_g - halfsize >= 0) && (k + col_g - halfsize < width))
				{
					r_g = *(src + row * width + col_g + k - halfsize);
					row_g += (int)(r_g)* kernel[count2];
					count2++;
				}

				if ((k + col_b - halfsize >= 0) && (k + col_b - halfsize < width))
				{
					r_b = *(src + row * width + col_b + k - halfsize);
					row_b += (int)(r_b)* kernel[count3];
					count3++;
				}
			}

			*(tmp + row * width + col_r) = (unsigned char)(ceil(row_r));
			*(tmp + row * width + col_g) = (unsigned char)(ceil(row_g));
			*(tmp + row * width + col_b) = (unsigned char)(ceil(row_b));
			col_r += 3;
			col_g += 3;
			col_b += 3; 
		}
		}
	winsize /= 3;
	halfsize = winsize / 2;
	for (col = 0; col < width; col++)
		for (row = 0; row < height; row++)
		{
		col_all = 0.0;
		for (k = 0; k < winsize; k++) 
			if ((k + row - halfsize >= 0) && (k + row - halfsize < height))
			{
			c_all = *(tmp + (row + k - halfsize) * width + col);
			col_all += ((int)c_all) * kernel[k];
			}
		*(dest + row * width + col) = (unsigned char)(ceil(col_all));
		}
		
	free(tmp);
	free(kernel);

	return 0;
}

void writeImageOut(char *fname,Image *im)//make ritting out easier
{
    FILE *imOut=fopen(fname, "wb");

    write_ppm(imOut,im);
}
int main(int argc, char **argv) {//used mainly for getting argmunets and calling methods
    if(argc <3)
    {
        exit(-1);
    }
    char *filename=argv[1];
    char *outFile=argv[2];
    char *command=argv[3];
    FILE *imIn=fopen(filename, "rb");
    Image *im=read_ppm(imIn);
    printf("Got %d arguments\n",argc);
    if(strcmp(command,"bright")==0)
    {
        int bright=atoi(argv[4]);

        modifyBrightness(im,bright);
        writeImageOut(outFile,im);
        deallocImage(im);
    }
    else if(strcmp(command,"gray")==0)
    {
        convertGrayscale(im);
        writeImageOut(outFile,im);
        deallocImage(im);

    }
    else if(strcmp(command,"crop")==0)
    {
        int top_Y=atoi(argv[4]);
        int top_X=atoi(argv[5]);
        int bot_Y=atoi(argv[6]);
        int bot_X=atoi(argv[7]);

        im=cropImagge(im,top_X,top_Y,bot_X,bot_Y);

        writeImageOut(outFile,im);
        deallocImage(im);

    }
    else if(strcmp(command,"occlude")==0)
    {
        int top_Y=atoi(argv[4]);
        int top_X=atoi(argv[5]);
        int bot_Y=atoi(argv[6]);
        int bot_X=atoi(argv[7]);
        Occlude(im,top_X,top_Y,bot_X,bot_Y);
        writeImageOut(outFile,im);
        deallocImage(im);
    }
    else if(strcmp(command,"blur")==0)
    {
        double sigma;
        int top_Y=atoi(argv[4]);
        int top_X=atoi(argv[5]);
        sscanf(argv[4],"%lf",&sigma);
        applyGauss(im,sigma);
        blur(im,top_X,top_Y,sigma)
        writeImageOut(outFile,im);
        deallocImage(im);
    }
    else if(strcmp(command,"edges")==0)
    {
        double sigma;
        sscanf(argv[4],"%lf",&sigma);
        int threshold=atoi(argv[5]);
        applyGauss(im,sigma);
        CannyEdge(im,threshold);
        writeImageOut(outFile,im);
        deallocImage(im);
    }

}
