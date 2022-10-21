# include <stdlib.h>
# include <stdio.h>
# include <math.h>
# include <omp.h>

void setValue();

int main(int argc, char const *argv[]){

	int i, j;
	int * array = (int*)malloc(N * sizeof(int));
	// omp_set_num_threads(NUM_THREADS);

	int *sum = (int*)malloc(N * sizeof(int));

	for (int i = 0; i < N; ++i){
		array[i] = i;
	}

	#pragma omp parallel shared (sum, array) private ( i, j )
	{
		#pragma omp sections
		{
			#pragma omp section
				
		}
		// #pragma omp master
		// {
		// 	omp_get_num_threads(NUM_THREADS);
		// }

		#pragma omp for
    		for ( i = 0; i < N; i++ )
    		{
    			int aux =  array[i];
    			array[i] = array[N - i -1];
    		}

		#pragma omp barrier

    	#pragma omp for
    		for (i = 0; i < N; i++)
    		{
    			sum[i] = array[i];
    		}		

	}
	
	for (i = 0; i < N; ++i){
		printf("%d\n", sum[i]);
	}

	free(array);
	free(sum);
	return 0;
}

