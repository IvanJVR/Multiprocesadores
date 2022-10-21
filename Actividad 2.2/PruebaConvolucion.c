#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N 20

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}pixel;

pixel **createMatrix(int width, int height);
pixel **createPadding(pixel **m, int width, int height, int padding);
pixel **copyMatrix(pixel **original, int width, int height);
pixel getAvarage(pixel **p, int width, int height);
void convolution(pixel **m, pixel **p, int width, int height, int widthC, int heightC);
void setZeros(pixel **m, int width, int height);
void setCenter(pixel **p, pixel **m,int width, int height, int padding);
void fillMatrix(pixel **m, int width, int height);
void printMatrix(pixel ** m, int width, int height);
void writeImage(pixel **m, int width, int height, FILE *file);
void toGrayScale(pixel **m, int width, int height, FILE *file);
void changeHorizontal(pixel **m, int width, int height);
void blurring(pixel **original, FILE *result, int width, int height, int padding);
void mirrorBlurring(pixel **original, FILE *result, int width, int height, int padding);

int main(int argc, char const *argv[]){
    FILE *image, *pruebaResultados[N];
    image = fopen("sample2.bmp","rb");  

    for(int i = 0; i < N; i++){
        char filename[20];
        sprintf(filename, "%d-tryImage.bmp", i + 1);
        pruebaResultados[i] = fopen(filename,"wb");
    } 

    clock_t t;

    unsigned char *header;
    header = (unsigned char*)malloc(sizeof(unsigned char)*54);

    fread(header, sizeof(unsigned char), 54, image);

    int width = *(int*)&header[18];
    int height = *(int*)&header[22];

    fseek(image, 54, SEEK_SET);

    for (int i = 0; i < N; i++){
        for (int j = 0; j < 54; j++){
            fputc(header[j], pruebaResultados[i]);   
        }
    }

    // for(int i=0; i<54; i++) fputc(header[i], outputImage);   //Copia cabecera a nueva imagen
    // for(int i=0; i<54; i++) fputc(header[i], try);   //Copia cabecera a nueva imagen

	// int width = 3;
	// int height = 4;

    // int padding = 36;
    // int widthP = width + padding;
    // int heightP = height + padding;

    // int widthC = padding + 1;
    // int heightC = padding + 1;

    t = clock();

    pixel ** pixels = createMatrix(width, height);
    // fillMatrix(pixels, width, height);
    // printMatrix(pixels, width, height);

    toGrayScale(pixels, width, height, image);
    // writeImage(pixels, width, height, outputImage);

    // pixel **paddedMatrix = createPadding(pixels, widthP, heightP, padding);
    // printMatrix(paddedMatrix, widthP, heightP);

	// convolution(pixels, paddedMatrix, width, height, widthC, heightC);
	// printMatrix(pixels, width, height);
	// printMatrix(paddedMatrix, widthP, heightP);

	// writeImage(pixels, width, height, try);

    int paddingB = 0;
    int paddingMB = 0;

	for (int i = 0; i < N; ++i){
		if(i < N / 2){
			blurring(pixels, pruebaResultados[i], width, height, paddingB);
			paddingB += 4;
		}
		else{
			mirrorBlurring(pixels, pruebaResultados[i], width, height, paddingMB);
			paddingMB += 4;
		}
	}

	t = clock() - t;
	double finalTime = ((double)t)/CLOCKS_PER_SEC;

	printf("tomo %f segundos\n", finalTime);

    free(pixels);
    free(header);

    fclose(image);
    for (int i = 0; i < N; i++){
        fclose(pruebaResultados[i]);
    }
	return 0;
}

pixel **createPadding(pixel **m, int width, int height, int padding){
	pixel **p = createMatrix(width, height);
	setZeros(p, width, height);
	setCenter(p, m, width, height, padding);

	return p;
}

pixel **createMatrix(int width, int height){
    pixel **m = (pixel**)malloc(sizeof(pixel*)*height);
    
    if(m == NULL){
        printf("Falling malloc\n");
        exit(-1);
    }

    for (int i = 0; i < height; i++){
        m[i] = (pixel*)malloc(sizeof(pixel)*width);
        if(m[i] == NULL){
            printf("Falling malloc");
            exit(-1);
        }
    }

    return m;
}

pixel **copyMatrix(pixel **original, int width, int height){
    pixel **copy = createMatrix(width, height);

    for (int i = 0; i < height; i++){
        for (int j = 0; j < width; j++){
            copy[i][j] = original[i][j];   
        }
    }

    return copy;  
}

void convolution(pixel **m, pixel **p, int width, int height, int widthC, int heightC){

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			unsigned int value = 0;
			for (int x = 0; x < heightC; x++){
				for (int y = 0; y < widthC; y++){
					value += p[i + x][j + y].b;				
				}
			}
			m[i][j].b = value/(heightC*widthC);
			m[i][j].g = value/(heightC*widthC);
			m[i][j].r = value/(heightC*widthC);
		}
	}
}

pixel getAvarage(pixel **m, int width, int height){
	pixel result;
	unsigned char a = 0;

	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++)
		{
			a += m[i][j].b;
		}
	}

	a = (a)/((height*width));
	result.b = a;
	result.g = a;
	result.r = a;

	return result;
}

void setZeros(pixel **m, int width, int height){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			m[i][j].b = 0;
			m[i][j].g = 0;
			m[i][j].r = 0;		
		}
	}
}

void setCenter(pixel **p, pixel **m,int width, int height, int padding){
	for (int i = padding/2; i < height - padding/2; i++){
		for (int j = padding/2; j < width - padding/2; j++){
			p[i][j] = m[i - padding/2][j - padding/2];
		}
	}
}

void fillMatrix(pixel **m, int width, int height){
	int contador = 1;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			m[i][j].b = contador;
			m[i][j].g = contador;
			m[i][j].r = contador;
			contador++;
		}
	}
}

void printMatrix(pixel ** m, int width, int height){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			printf("%3u %3u %3u    ", m[i][j].b, m[i][j].g, m[i][j].r);
		}
		printf("\n");
	}
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

void blurring(pixel **original, FILE *result, int width, int height, int padding){
    if(padding > 0){
        pixel **m = copyMatrix(original, width, height);
        int widthP = width + padding;
        int heightP = height + padding;

        int widthC = padding + 1;
        int heightC = padding + 1;


        pixel **paddedMatrix = createPadding(m, widthP, heightP, padding);
        convolution(m, paddedMatrix, width, height, widthC, heightC);
        
        free(paddedMatrix);
        
        writeImage(m, width, height, result);
        free(m);
    }
    else{
        pixel **m = copyMatrix(original, width, height);
        writeImage(m, width, height, result);  
        free(m);     
    }
}

void mirrorBlurring(pixel **original, FILE *result, int width, int height, int padding){

    if(padding > 0){
        pixel **m = copyMatrix(original, width, height);

        int widthP = width + padding;
        int heightP = height + padding;

        int widthC = padding + 1;
        int heightC = padding + 1;

        changeHorizontal(m, width, height);

        pixel **paddedMatrix = createPadding(m, widthP, heightP, padding);
        convolution(m, paddedMatrix, width, height, widthC, heightC);
        
        writeImage(m, width, height, result);
        
        free(paddedMatrix);
        free(m);
    }
    else{
        pixel **m = copyMatrix(original, width, height);
        changeHorizontal(m, width, height);
        writeImage(m, width, height, result);
        free(m);
    }    
}