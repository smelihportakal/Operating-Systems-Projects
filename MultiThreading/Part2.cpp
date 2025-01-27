#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <bits/stdc++.h>
#include <pthread.h>
#include <sys/types.h>
#include <cstdlib>
#include <chrono>

using namespace std;
using namespace std::chrono;

void calculateMin(int data[],int n);
void calculateMax(int data[],int n);
void calculateRange(int data[],int n);
void calculateMode(int data[],int n);
void calculateMedian(int data[],int n);
void calculateSum(int data[], int n);
void calculateMean(int data[],int n);
void calculateHMean(int data[],int n);
void calculateSD(int data[],int n);
void calculateIQR(int data[],int n);

void (*functions[10])(int*, int) = {calculateMin,calculateMax,calculateRange,calculateMode,
calculateMedian,calculateSum,calculateMean,calculateHMean,calculateSD,calculateIQR};


int midIndex(int l, int r);
long int summation(int data[],int n);
float opMean(int data[],int n);

int n;
int *data;

int minn;
int maxx;
int range;
int mode;
int median;
long int summ;
float amean;
float hmean;
float stdev;
int iqrange;

struct thread_data {
   int  thread_id;
   int l;
   int r;
};



void *runnerFunctions(void *threadarg) {
	struct thread_data *my_data;
	my_data = (struct thread_data *) threadarg;
	int left = my_data->l ;
	int right = my_data->r ;
	
	if (right > n) {right = n;}
	
	for (;left < right; left++) {
		functions[left](data,n);
	}


	pthread_exit(NULL);
}

int main(int argc, char** argv) {
  int i; 
  int low = 1000;
  int high = 10000;
  int numThreads = stoi(argv[2]);
  
  //cout << "take parameter of number of threads" << endl;
  
  int rc;
  int dif = 10/numThreads;
  int left = 0;
  int right = dif;
  
  clock_t time_req;
  clock_t time_req10;
  clock_t time_req5;
  clock_t time_req1;
  
  n = stoi(argv[1]);
  data = new int[n];
  
  pthread_t threads[numThreads];
  struct thread_data td[numThreads];

	

  srand(time(NULL));
	for (i = 0; i < n; i++) {
  	int r = 1000 + rand() % 9001;
  	
    data[i] = r;
  }
  
  auto start = high_resolution_clock::now();
  sort(data, data + n);
  
  
  
  for( i = 0; i < numThreads; i++ ) {  
		//cout <<"main() : creating thread, " << i << endl;
		td[i].thread_id = i;
		
		if (i == numThreads - 1) {
			td[i].l = left;
			td[i].r = 10;
		} else {
			td[i].l = left;
			td[i].r = right;
		}
		
		rc = pthread_create(&threads[i], NULL, runnerFunctions, (void *)&td[i]);

		if (rc) {
			 cout << "Error:unable to create thread," << rc << endl;
			 exit(-1);
		}

		left += dif;
		right += dif;
  }
  
  for( i = 0; i < numThreads; i++ ) {
  	pthread_join(threads[i],NULL);
  }
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  float time = (float) duration.count()/ 1000000;
  
  ofstream myfile;
  myfile.open ("output2.txt");
  myfile << minn << endl;
  myfile << maxx << endl;
  myfile << range << endl;
  myfile << mode << endl;
  myfile << median << endl;
  myfile << summ << endl;
  myfile << amean << endl;
  myfile << hmean << endl;
  myfile << stdev<< endl;
  myfile << iqrange << endl;
  myfile << time << endl;
  myfile.close();
  
}

void calculateMin(int data[],int n) {
	minn = *min_element(data, data + n);
}

void calculateMax(int data[],int n) {
	maxx = *max_element(data, data + n);
}

void calculateRange(int data[],int n) {
	int min = *min_element(data, data + n);
	int max = *max_element(data, data + n);
	range = max - min;
}

void calculateMode(int data[], int n) {
	sort(data, data + n);
  
	int number = data[0];
	int m = number;
	int count = 1;
	int countMode = 1;

	for (int i=1; i<n; i++){
		if (data[i] == number) {
			 ++count;
		} else { 
			if (count > countMode) 
			{
				    countMode = count; 
				    m = number;
			}
		 count = 1;
		 number = data[i];
		}
	}
	
	mode = m;
	
}

void calculateMedian(int data[],int n) {
	sort(data, data + n);
	
	median = data[midIndex(0, n)];
}

void calculateSum(int data[],int n){
  summ = summation(data,n);
}

void calculateMean(int data[],int n) {
	amean = opMean(data,n);
}

void calculateHMean(int data[], int n){
	float sum = 0;
  for (int i = 0; i < n; i++)
  	sum = sum + (float)1 / (float) data[i];
  hmean = (float)n/sum;
}

void calculateSD(int data[],int n) {
  float mean, standardDeviation = 0.0;
  int i;

  mean = opMean(data,n);

  for(i = 0; i < n; ++i) {
    standardDeviation += pow(data[i] - mean, 2);
  }
  
  stdev = sqrt(standardDeviation / (n-1));

}

void calculateIQR(int data[],int n) {
	sort(data, data + n);
 
  int mid_index = midIndex(0, n);
  int Q1 = data[midIndex(0, mid_index)];
 
  int Q3 = data[midIndex(mid_index + 1, n)];
 	
 	iqrange = Q3 - Q1;
}

int midIndex(int l, int r){
	int mid = r-l+1;
	mid = (mid +1)/ 2 -1 +l;
	return mid;
}

long int summation(int data[],int n) {
	long int sum = 0;
	int i;
	
  for(i = 0; i < n; ++i) {
    sum += data[i];
  }
  
  return sum;
}

float opMean(int data[],int n){
	float sum, mean;
	sum = summation(data,n); 
	mean = sum / n;
	return mean;
}
