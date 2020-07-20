#include<stdio.h>
#include<math.h>
#include<stdlib.h>

// float cost_function(double *x, int d, int n){
// 
// }
double calc_distance(double *vec1, double *vec2, int dim){
	double value;
	value = 0;

	for (int i=0; i < dim; i++){
		value += pow(*(vec1 + i) - *(vec2 + i));
	}

	return(value)
}

void * assignment(double *X, double *y, double *micro, int n_clusters, \
		int dim, int nums){
	double min_dist, dist;
	double * X_mem;


	for(int i=0; i < nums; i++){
		X_mem = X + dim*i

		// calculate distance and findout minimum
		min_dist = calc_distance(X_mem, (micro + 0));
		for(int j=1; j < n_clusters; j++){
			dist = calc_distance(X_mem, (micro + dim*j));
			if(min_dist > dist){
				min_dist = dist;
			}
		}


	}

}
void * fit(double *X, double *y, double *micro, int n_clusters, \
		int dim, int nums){
	for(int i=0; i < nums; i++){
		for(int j=0; j < dim; j++){
			*(X + dim*i + j) =
		}
	}
}

int rule_of_thumb(int n){
	// TO-DO
	// return( floor( sqrt((double) n / 2) + 0.5 ) );
	return(3);
}

void * init_micro_i(double *X, double *micro, int n_clusters, \
		int dim, int nums){
	// use random partition
	
	int * micro_counts;
	int which;
	int micro_cnt;

	micro_counts = (int *) malloc(sizeof(int) * n_clusters);

	// init micro_counts by 0
	for(int i=0; i < n_clusters; i++){
		*(micro_counts + i) = 0;
	}

	// init micro
	for(int i=0; i < nums; i++){
		which = rand() % n_clusters;
		micro_cnt = *(micro_counts + which);

//		if(*(micro_counts + which) == 0){
//			for(int j=0; j < dim; j++){
//				*(micro + dim*which + j) = *(X + dim*i + j);
//		}
		// }else{
		for(int j=0; j < dim; j++){
			*(micro + dim*which + j) = \
				*(micro + dim*which + j) \
				* (micro_cnt / (micro_cnt+1)) \
				+ *(X + dim*i + j) / (micro_cnt+1);
		}
		*(micro_counts + which) += 1;
	}
	
	free(micro_counts)
}

int main(){
	double *X; // input
	double *y; // output
	double *micro;
	int dim = 2;
	int nums = 3;
	int n_clusters;

	X = (double *) malloc(sizeof(double) * dim * nums);
	y = (double *) malloc(sizeof(double) * nums);

	// init with random
	for (int i=0; i < nums; i++){ // rows
		for(int j=0; j < dim; j++){ // cols
			*(X + dim*i +j)  = (double) (rand() % 1000);
		}
	}

	n_clusters = rule_of_thumb(nums);

	micro = (double *) malloc(sizeof(double) * dim *  n_clusters);


	init_micro_i(X, micro, n_clusters, dim, nums);
	fit(X, y, micro, n_clusters, dim, nums);

	free(X);
	free(y);
	free(micro);

	return(0);
}
