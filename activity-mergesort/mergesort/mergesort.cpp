#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "par_loop.hpp"
#include <vector>
#include <chrono>
using namespace std::chrono;


#ifdef __cplusplus
extern "C" {
#endif
  void generateMergeSortData (int* arr, size_t n);
  void checkMergeSortResult (int* arr, size_t n);
#ifdef __cplusplus
}
#endif

int min(int x, int y){
  return x<y ? x:y;
}

void merge(int arr[], int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
 
    /* create temp arrays */
    int L[n1], R[n2];
 
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
 
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        if (L[i] <= R[j])
        {
            arr[k] = L[i];
            i++;
        }
        else
        {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
 
    /* Copy the remaining elements of L[], if there are any */
    while (i < n1)
    {
        arr[k] = L[i];
        i++;
        k++;
    }
 
    /* Copy the remaining elements of R[], if there are any */
    while (j < n2)
    {
        arr[k] = R[j];
        j++;
        k++;
    }
}


int main (int argc, char* argv[]) {
  
  if (argc < 3) { std::cerr<<"Usage: "<<argv[0]<<" <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int n = atoi(argv[1]);
  int nbthread = atoi(argv[2]);
  
  // get arr data
  int * arr = new int [n];
  generateMergeSortData (arr, n);
  ParLoop pl;

  
  int curr_size=1;
  int left_start;

pl.parfor<int>(1,n-1,2*curr_size,nbthread,
[&](int& tls,int i) -> void{
tls =arr[i];
},
[&](int& tls, int left_start, int curr_size) -> void{
    int mid = min(left_start+curr_size-1,n-1);
    int right_end = min(left_start+ 2*curr_size -1, n-1);

    merge(&tls, left_start, mid, right_end); 

},
[&](int& tls, int i) -> void{
arr[i]=tls;
});
// for(curr_size=1;curr_size<=n-1;curr_size=2*curr_size){
//   for(left_start=0;left_start<n-1;left_start+=2*curr_size){
//     int mid = min(left_start+curr_size-1,n-1);
//     int right_end = min(left_start+ 2*curr_size -1, n-1);

//     merge(arr, left_start, mid, right_end);
//   }
// }

  //insert sorting code here.


  
  checkMergeSortResult (arr, n);
  
  delete[] arr;

  return 0;
}
