#include<stdio.h>
#include<math.h>
#include<stdlib.h>
#include<time.h>

void init_clusters(double *X, double *clusters, int num_clusters, int dim, int nums);

double calc_distance(double *vec1, double *vec2, int dim){
	double value;
	double tmp;
	value = 0;

	for (int i=0; i < dim; i++){
		tmp = *(vec1 + i) - *(vec2 + i);
		value += tmp * tmp;
	}
	return(value);
}


// findout closest clusters and assign data.
void assign_y(double *X, int *y, double *clusters, int num_clusters, \
		int dim, int nums){
	double min_dist, dist;
	double * X_mem;
        int * y_mem;
	int min_idx;

	// accessing member of X and y
	for(int i=0; i < nums; i++){
		X_mem = X + dim*i;
		y_mem = y + i;

		// calculate distance and findout minimum
		min_dist = calc_distance(X_mem, (clusters + 0), dim);
		min_idx = 0;
		for(int j=1; j < num_clusters; j++){
			dist = calc_distance(X_mem, (clusters + dim*j), dim);
			if(dist < min_dist){
				min_dist = dist;
				min_idx = j;
			}
		}

		*(y_mem) = min_idx;
	}
}

// return 1 if update of clusters had been occured
int update_clusters(double *X, int *y, double *clusters, int num_clusters, \
		int dim, int nums){
	double * X_mem;
	int * y_mem;
	double * clusters_temp;
	double * clusters_temp_mem;
	double * clusters_mem;
	int * clusters_cnts;
	int clusters_cnts_mem;
	int is_updated;
	double how_far;

	clusters_cnts = (int *) malloc(sizeof(int) * num_clusters);
	clusters_temp = (double *) malloc(sizeof(double) * dim * num_clusters);

	// init clusters_cnts by 0
	for(int i=0; i < num_clusters; i++){
		*(clusters_cnts + i) = 0;
	}

	// init clusters_temp
	for(int i=0; i < nums; i++){
		X_mem = X + dim * i;
		y_mem = y + i;

		clusters_cnts_mem = (double) *(clusters_cnts + *(y_mem));
		clusters_temp_mem = clusters_temp + dim * *(y_mem);

		// update cluster_temp
		for(int j=0; j < dim; j++){
			*(clusters_temp_mem + j) = \
				*(clusters_temp_mem + j) \
				/ (clusters_cnts_mem+1) \
				* clusters_cnts_mem \
				+ *(X_mem + j) / (clusters_cnts_mem+1);
		}
		*(clusters_cnts + *(y_mem)) += 1;

	}

	// compare clusters with clusters_temp.
	is_updated = 0;
	for(int i=0; i < num_clusters; i++){
		clusters_mem = clusters + dim * i;
		clusters_temp_mem = clusters_temp + dim * i;

		// compare each clusters with clusters_temp.
		how_far = calc_distance(clusters_mem, clusters_temp_mem, dim);
		/* TODO:  <22-07-20, yourname> */
		// allow error
		
		// copy clusters_temp to cluster
		if(how_far != 0){
			is_updated = 1;
			for(int j=0; j < dim; j++){
				*(clusters_mem + j) = *(clusters_temp_mem + j);
			}
		}
	}

	free(clusters_cnts);
	free(clusters_temp);

	return(is_updated);
}

int * fit(double *X, int num_clusters, int dim, int nums){
	int is_updated;
	int i;
	int * y;
	double *clusters;

	init_clusters(X, clusters, num_clusters, dim, nums);
	y = (int *) malloc(sizeof(int) * nums);
	clusters = (double *) malloc(sizeof(double) * dim *  num_clusters);

	/*
	printf("initial clusters:\n");
	for (int i=0; i < num_clusters; i++){
		for(int j=0; j < dim; j++){ // cols
			printf("%4.2f ", *(clusters + dim*i +j));
		}
		printf("\n");
	}
	*/

	for(i=0; i < 10000; i++){
		assign_y(X, y, clusters, num_clusters, dim, nums);
		is_updated = \
			update_clusters(X, y, clusters, num_clusters, \
				dim, nums);

		if( is_updated == 0 ){
			//printf("fit: break!\n");
			break;
		}
	}
	// printf("cluster updated for %d of times\n", i);

	return(y);
}

int rule_of_thumb(int n){
	// TO-DO
	// return( (int) floor( sqrt((double) n / 2) + 0.5 ) );
	return(3);
}

void init_clusters(double *X, double *clusters, int num_clusters, int dim, \
		int nums){
	/*
	 * Use forgy method: select cluster in X randomly.
	 */
	
	int which;
	int * selected_as_clusters;
	int had_been_selected;
	int i;
	time_t t;
	double * clusters_mem;
	double * X_mem;

	srand((unsigned) time(&t));
	if ( nums > RAND_MAX ){
		printf("WARNNING: number of input data (X) is larger than");
		printf(" RAND_MAX\n");
	}

	if( num_clusters > nums ){
		printf("ERROR: number of clusters is larger");
		printf(" than number of input data (X)\n");
		num_clusters = nums;
	}

	selected_as_clusters = (int *) malloc(sizeof(int) * num_clusters);

	i = 0;
	while( i < num_clusters){
		which = rand() % nums;

		had_been_selected = 0;
		for(int j=0; j<i; j++){
			if(*(selected_as_clusters + j) == which){
				had_been_selected = 1;
				break;
			}
		}

		if( had_been_selected == 1 ){
			continue;
		}else{
			*(selected_as_clusters + i) = which;
			i++;
		}
	}

	// assign clusters
	for(int i=0; i < num_clusters; i++){
		clusters_mem = clusters + dim * i;
		X_mem = X + dim * *(selected_as_clusters + i);
		for(int j=0; j < dim; j++){
			*(clusters_mem + j) = *(X_mem + j);
		}
	}
	free(selected_as_clusters);
}

int main(){
	double *X; // input
	int *y; // output
	double *clusters;
	int dim = 2;
	int nums = 1500;
	int num_clusters=3;
	time_t t;
	FILE *fp;

	X = (double *) malloc(sizeof(double) * dim * nums);

	srand((unsigned) time(&t));

	// init with random
	// for (int i=0; i < nums; i++){ // rows
	// 	for(int j=0; j < dim; j++){ // cols
	// 		*(X + dim*i +j)  = (double) (rand() % 1000);
	// 	}
	// }
	

	// grep data
	fp = fopen("dataX", "r");
	fscanf(fp, "%d %d %d", &nums, &dim, &num_clusters);
	for(int i=0; i< nums * dim; i++){
		// fscanf(fp, "%lf %lf", X + dim*i, X+ dim*i +1);
		fscanf(fp, "%lf", X + i);
	}
	fclose(fp);



	y = fit(X, num_clusters, dim, nums);

	for(int i=0; i < nums-1; i++){
		fprintf(stdout, "%d ", *(y+i));
	}
	fprintf(stdout, "%d", *(y+nums-1));

	// print
	// for (int i=0; i < nums; i++){ // rows
	// 	for(int j=0; j < dim; j++){ // cols
	// 		printf("%.4f ", *(X + dim*i +j));
	// 	}
	// 	printf("    | %d", *(y + i));
	// 	printf("\n");
	// }
	// printf("print clusters\n");
	// for (int i=0; i < num_clusters; i++){
	// 	for(int j=0; j < dim; j++){ // cols
	// 		printf("%4.2f ", *(clusters + dim*i +j));
	// 	}
	// 	printf("\n");
	// }

	free(X);
	free(y);
	free(clusters);

	return(0);
}
