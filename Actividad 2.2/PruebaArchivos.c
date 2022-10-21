#include <stdio.h>
#include <stdlib.h>

#define N 4

int main(int argc, char const *argv[]){
	// FILE *files[numfiles];
	// for (int i = 0; i < numfiles; i++){
	//     char filename[20];
	//     sprintf(filename, "%d-Image.txt", i);
	//     files[i] = fopen(filename, "wt");
	// }

	FILE *images[N];
	for(int i = 0; i < N; i++){
		char filename[20];
		sprintf(filename, "%d-image.txt", i + 1);
		images[i] = fopen(filename,"wt");
	}

	for (int i = 0; i < N; i++){
		fprintf(images[i], "%d-hola, mundo", i);
	}
    
	for (int i = 0; i < N; i++){
		fclose(images[i]);
	}
	return 0;
}