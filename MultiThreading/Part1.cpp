#include <iostream>
#include <cmath>
#include <random>
#include <ctime>
#include <bits/stdc++.h>
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

int midIndex(int l, int r);
long int summation(int data[],int n);
float opMean(int data[],int n);

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

int main(int argc, char** argv) {
  int i;
  
	int n = stoi(argv[1]);
	int *data = new int[n];;
  int low = 1000;
  int high = 10000;
 
  srand(time(NULL));
	
	for (int i = 0; i < n; i++) {
	
  	int r = 1000 + rand() % 9001;
  	
    data[i] = r;
   	}
  auto start = high_resolution_clock::now();
   
  sort(data, data + n);
  calculateMin(data,n);
  calculateMax(data,n); 
  calculateRange(data,n);
  calculateMode(data,n);  
  calculateMedian(data,n);
  calculateSum(data,n);
  calculateMean(data,n);
  calculateHMean(data,n);
  calculateSD(data,n);
  calculateIQR(data,n);
  
  auto stop = high_resolution_clock::now();
  auto duration = duration_cast<microseconds>(stop - start);
  float time = (float) duration.count()/ 1000000;
 	
 	ofstream myfile;
  myfile.open ("output1.txt");
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

void calculateMode(int data[], int n)
{
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
