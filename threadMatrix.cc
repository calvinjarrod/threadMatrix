/** 
	CSCI144 - Operating Systems, Practice 2: Chapter 4 Problem 5
	threadMatrix.cc
	Purpose: Perform matrix multiplication using threading

	@author Calvin Jarrod Smith
	@version 1.0 10/02/2019
*/

#include <cstdlib>
#include <pthread.h>
#include <iostream>
#include <ctime>

using namespace std;

/*------ Global constants ------*/
const int A_ROWS = 3;
const int A_COLUMNS = 4;
const int B_ROWS = 4;
const int B_COLUMNS = 3;
const int C_ROWS = 3;
const int C_COLUMNS = 3;
#define NUM_THREADS C_ROWS*C_COLUMNS
/*------------------------------*/

/*------ Global Variables ------*/
int A[A_ROWS][A_COLUMNS];
int B[B_ROWS][B_COLUMNS];
int C[C_ROWS][C_COLUMNS];
/*------------------------------*/

/*---- Function Prototypes -----*/
void fillMatricies(int min, int max);
void multMatricies();
void printMatricies();
void *sumOfMults(void *threadargs);
/*------------------------------*/

struct thread_data {
	int i;
	int j;
};

int main() {
	fillMatricies(1,3);
	multMatricies();
	printMatricies();
	return 0;
}

/** 
	Fills two matricies, A and B, with random values specified by user.

	@param minimum and maximum values to be inserted
	@return none
*/
void fillMatricies(int min, int max) {
	srand(time(NULL));
	for (int i = 0; i < A_ROWS; i++) {
		for (int j = 0; j < A_COLUMNS; j++) {
			A[i][j] = rand() % (max) + min;
		}
	}
	for (int i = 0; i < B_ROWS; i++) {
		for (int j = 0; j < B_COLUMNS; j++) {
			B[i][j] = rand() % (max) + min;
		}
	}
	return;
}

/** 
	Uses threading to perform matrix multiplication of A and B. New thread is 
	used to compute each element of matrix C.

	@param none
	@return none
*/
void multMatricies() {
	if (A_COLUMNS != B_ROWS && A_ROWS != B_COLUMNS) {
		cout<<"Invalid Matrix Dimensions\n";
		return;
	} else {
		pthread_t threads[NUM_THREADS];
		// thread data must be declared statically to ensure its available when
		// needed by the thread
		struct thread_data data[NUM_THREADS];
		int rc;
		for (int i = 0; i < A_ROWS; i++) {
			for (int j = 0; j < B_COLUMNS; j++) {
				data[i*A_ROWS + j] = {i,j};
				rc = pthread_create(
					&threads[i*A_ROWS + j],
					NULL,
					sumOfMults,
					(void*)&data[i*A_ROWS+j]
				);
				if (rc) {
					cout<<"Unable to create thread,"<<rc<<"\n";
					exit(-1);
				}

			}
		}
		for (int i = 0; i < A_ROWS; i++) {
			for (int j = 0; j < B_COLUMNS; j++) {
				data[i*A_ROWS + j] = {i,j};
				rc = pthread_join(threads[i*A_ROWS + j],NULL);
				if (rc) {
					cout<<"Unable to join thread,"<<rc<<"\n";
					exit(-1);
				}

			}
		}
	}	
}


/** 
	Calculates the sum of multiplications necessary for matrix multiplication. 
	This will be run as a thread so proper use of thread arguments and closing
	the thread is necessary.

	@param thread arguments
	@return none
*/
void *sumOfMults(void *threadargs) {
	struct thread_data *mydata;
	mydata = (struct thread_data*) threadargs;

	int I = mydata->i;
	int J = mydata->j;
	C[I][J] = 0;
	int MAX_SPAN = (A_ROWS > A_COLUMNS) ? A_ROWS : A_COLUMNS;
	for (int i = 0; i < MAX_SPAN; i++) {
		C[I][J] += A[I][i]*B[i][J];
		// USED FOR DEBUGGING
		//cout<<"C["<<I<<"]["<<J<<"]="<<C[I][J]<<endl;
	}
	pthread_exit(NULL);
}

/**
	Prints all three matricies, A, B and C.

	@param none
	@return none
*/
void printMatricies() {
	cout<<"A:\n";
	for (int i = 0; i < A_ROWS; i++) {
		for (int j = 0; j < A_COLUMNS; j++) {
			cout<<A[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"B:\n";
	for (int i = 0; i < B_ROWS; i++) {
		for (int j = 0; j < B_COLUMNS; j++) {
			cout<<B[i][j]<<" ";
		}
		cout<<"\n";
	}
	cout<<"C:\n";
	for (int i = 0; i < C_ROWS; i++) {
		for (int j = 0; j < C_COLUMNS; j++) {
			cout<<C[i][j]<<" ";
		}
		cout<<"\n";
	}
}
