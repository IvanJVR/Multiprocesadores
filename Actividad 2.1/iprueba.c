#include <stdio.h>
#include <stdlib.h>

typedef struct{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}pixel;

void writeImage(pixel **m, int width, int height, FILE *file);
void toGrayScale(pixel **m, int width, int height, FILE *file);
void changeVertical(pixel **m, int width, int height);
void changeHorizontal(pixel **m, int width, int height);

int main(int argc, char const *argv[]){
    FILE *image, *outputImage, *lecturas, *try;
    image = fopen("sample4.bmp","rb");  
    outputImage = fopen("pruebaHorizontal.bmp","wb"); 
    try = fopen("pruebaResultado.bmp", "wb");

    unsigned char *header;
    header = (unsigned char*)malloc(sizeof(unsigned char)*54);

    unsigned char r, g, b;

    fread(header, sizeof(unsigned char), 54, image);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    pixel ** pixels;
    pixels = (pixel**)malloc(sizeof(pixel*)*height);

    if(pixels == NULL){
        printf("Falling malloc\n");
        exit(-1);
    }

    for (int i = 0; i < height; i++){
        pixels[i] = (pixel*)malloc(sizeof(pixel)*width);
        if(pixels[i] == NULL){
            printf("Falling malloc");
            exit(-1);
        }
    }

    fseek(image, 54, SEEK_SET);

    for(int i=0; i<54; i++) fputc(header[i], outputImage);   //Copia cabecera a nueva imagen
    for(int i=0; i<54; i++) fputc(header[i], try);   //Copia cabecera a nueva imagen

    toGrayScale(pixels, width, height, image);
    changeHorizontal(pixels, width, height);
    writeImage(pixels, width, height, outputImage);
    changeVertical(pixels, width, height);
    writeImage(pixels, width, height, try);

    free(pixels);
    free(header);

	fclose(image);
	fclose(outputImage);
    fclose(try);
	return 0;
}

void writeImage(pixel **m, int width, int height, FILE *file){
    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            fputc(m[i][j].b, file);
            fputc(m[i][j].g, file);
            fputc(m[i][j].r, file);
        }
    }
}

void toGrayScale(pixel **m, int width, int height, FILE *file){
    unsigned char r, g, b;

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            b = fgetc(file);
            g = fgetc(file);
            r = fgetc(file);

            unsigned char p = 0.21*r+0.72*g+0.07*b;

            m[i][j].b = p;
            m[i][j].g = p;
            m[i][j].r = p;           
        }
    }
}

void changeVertical(pixel **m, int width, int height){
    pixel * array;
    array = (pixel*)malloc(sizeof(pixel)*height);

    for (int i = 0; i < width; i++){
        for (int j = 0; j < height; j++){
            array[j] = m[j][i];
        }

        for (int j = 0; j < height; j++){
            m[j][i] = array[height - j - 1];
        }
    }

    free(array);
}

void changeHorizontal(pixel **m, int width, int height){
    pixel * array;
    array = (pixel*)malloc(sizeof(pixel)*width);

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            array[j] = m[i][j];
        }

        for (int j = 0; j < width; j++){
            m[i][j] = array[width - j - 1];
        }
    }

    free(array);
}