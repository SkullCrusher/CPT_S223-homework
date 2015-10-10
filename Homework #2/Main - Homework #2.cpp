//HOMEWORK 2, David Harkins

/*
	Results, anything past 10k you should use binary searching.

	100k
	(linear) 9.34 seconds
	(binary) 0.14 seconds.

	10k
	(linear) 0.89 seconds
	(binary) 0.016 seconds

	1k
	(linear) 0.0091 seconds
	(binary) 0.0002 seconds.

*/


#include <stdio.h>
#include <stdlib.h>
#include <ctime>
#include <time.h>     

using namespace std;
	


void Linear_Search(long long *Searchable, long long Searchable_Size, long long Random_Search) {
	
	for (long long i = 0; i < Searchable_Size; i++){
		if (Searchable[i] == Random_Search){

			int debug = 0;
			i++;
			break;
		}
	}
}

void Binary_Search(long long *Searchable, long long Searchable_Size, long long Random_Search, long long First, long long Last){

	long long Middle;  


	if (First == -1 && Last == -1){
		Middle = Searchable_Size / 2;

		First = 0;
		Last = Searchable_Size;

	}else{
		Middle = (First + Last) / 2;
	}

	
	//If the middle is the element.
	if (Random_Search == Searchable[Middle]){		
		return;
	}else{

		if (Random_Search < Searchable[Middle]){
			Binary_Search(Searchable, Searchable_Size, Random_Search, First, (Middle - 1));
		}else{
			Binary_Search(Searchable, Searchable_Size, Random_Search, (Middle + 1), Last);
		}

	}


}

int main(){

	srand(time(NULL)); //Random Seed.

	long long *Searchable = NULL;

	long long i = 500000000;
	long long n;	

		
	//Try to malloc less and less until we get to an amount we can.
	long long Reduce = 0;
	while (Searchable == NULL){

		Reduce += 10;

		Searchable = (long long*)malloc(((i - Reduce) * (sizeof(long long))) + 1);
	}

	for (n = 0; n < (i - Reduce); n++){
		Searchable[n] = n;
	}

	clock_t begin_linear = clock();

		//Test the search for 100000 times for linear.
	for (int g = 0; g < 100000; g++){

		Linear_Search(Searchable, (i - Reduce), (rand() % n));
	}

	clock_t end_linear = clock();
	double elapsed_secs_linear = double(end_linear - begin_linear) / CLOCKS_PER_SEC;


	clock_t begin_binary = clock();

	//Test the search for 100000 times for linear.
	for (int g = 0; g < 100000; g++){

		Binary_Search(Searchable, (i - Reduce), (rand() % n), -1, -1);
	}

	clock_t end_binary = clock();
	double elapsed_secs_binary = double(end_binary - begin_binary) / CLOCKS_PER_SEC;






	printf("Elapsed time (linear): %E \n", elapsed_secs_linear);
	printf("Elapsed time (binary): %E \n", elapsed_secs_binary);

	

		//Frees up memory.
	free(Searchable);

	
	return 0;
}