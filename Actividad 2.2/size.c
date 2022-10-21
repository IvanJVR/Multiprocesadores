#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define NUM_THREADS 50

typedef struct{
    unsigned char b;
    unsigned char g;
    unsigned char r;
}pixel;

pixel **createMatrix(int width, int height);

int main(int argc, char const *argv[]){
    pixel ** p = createMatrix(5, 4);

    printf("%ld\n", sizeof p/sizeof * pixel);
    
    return 0;
}

pixel **createMatrix(int width, int height){
    pixel **m = (pixel**)malloc(sizeof(pixel*)*height);
    for (int i = 0; i < height; i++){
        m[i] = (pixel*)malloc(sizeof(pixel)*width);
        if(m[i] == NULL){
            printf("Falling malloc");
            exit(-1);
        }
    }

    return m;
}