#include <stdio.h>
#include <stdlib.h>

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}pixel;

void fillMatrix(pixel **m, int width, int height);
void changeHorizontal(pixel **m, int width, int height);
void changeVertical(pixel **m, int width, int height);
void printMatrix(pixel ** m, int width, int height);

int main(int argc, char const *argv[]){
	int width = 4;
	int height = 3;

	pixel **pixels = NULL;
    pixels = (pixel**)malloc(sizeof(pixel*)*height);
    for (int i = 0; i < height; i++){
        pixels[i] = (pixel*)malloc(sizeof(pixel)*width);
        if(pixels[i] == NULL){
            printf("Falling malloc");
            exit(-1);
        }
    }

    fillMatrix(pixels, width, height);
    printMatrix(pixels, width, height);

    printf("\nNueva Matrix\n");
    changeHorizontal(pixels, width, height);
    printMatrix(pixels, width, height);

	return 0;
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

void changeVertical(pixel **m, int width, int height){
	pixel * array;
	array = (pixel*)malloc(sizeof(pixel)*height);
	for (int i = 0; i < height; ++i){
		for (int j = 0; j < width; j++){
			array[j] = m[j][i];
		}

		for (int j = 0; j < width; j++){
			m[j][i] = array[width - j - 1];
		}
	}

	free(array);
}

void printMatrix(pixel ** m, int width, int height){
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			printf("b:%3u g:%3u r:%3u    ", m[i][j].b, m[i][j].g, m[i][j].r);
		}
		printf("\n");
	}
}

void fillMatrix(pixel **m, int width, int height){
	int contador = 0;
	for (int i = 0; i < height; i++){
		for (int j = 0; j < width; j++){
			m[i][j].b = contador;
			m[i][j].g = contador + 1;
			m[i][j].r = contador + 2;
			contador++;
		}
	}
}