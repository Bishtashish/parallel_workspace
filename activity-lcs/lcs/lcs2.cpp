#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <chrono>
#include "par_loop.hpp"
#include <mutex>
#include <thread>
#include <omp.h>





#ifdef __cplusplus
extern "C" {
#endif

  void generateLCS(char* X, int m, char* Y, int n);
  void checkLCS(char* X, int m, char* Y, int n, int result);

#ifdef __cplusplus
}
#endif
int LCS(char* X, int m, char* Y, int n, int nbthread){

  	   
  int** C = new int*[m+1];
  int result =0;
  for (int i=0,j=0; j<=n && i<=m;j++)
	{

	  int ad=std::min(j,(int)m-i);		//Size of the anti-diagonal
		#pragma omp parallel for
		for(int k=0;k<=ad;++k)				//parallel for loop for each anti-diagonal element
		{
			int a=i+k,b=j-k;		        //for index of the diagonal elements 							
			if (a == 0 || b == 0)
				C[a][b] = 0;

			else if(X[a-1]==Y[b-1])
				C[a][b]=C[a-1][b-1]+1;

			else
			  C[a][b] = std::max(C[a-1][b],C[a][b-1]);
		}
		if(j>=n)
		{								
			j=n-1,i++;
		}
	}
  //	return C[x.length()][y.length()];

  result = C[m][n];
  for(int i=0; i<=m;++i){
  delete[] C[i];
  }
  delete[] C;
  return result;
}


int main (int argc, char* argv[]) {

  if (argc < 4) { std::cerr<<"usage: "<<argv[0]<<" <m> <n> <nbthreads>"<<std::endl;
    return -1;
  }

  int m = atoi(argv[1]);
  int n = atoi(argv[2]);
  int nbthread = atoi(argv[3]);

  // get string data 
  char *X = new char[m];
  char *Y = new char[n];
  generateLCS(X, m, Y, n);
  std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
  
  //insert LCS code here.

  int result = LCS(X, m, Y, n, nbthread); // length of common subsequence
  
  std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
  std::chrono::duration<double> elpased_seconds = end-start;


  checkLCS(X, m, Y, n, result);
  std::cerr<<elpased_seconds.count()<<std::endl;
  delete[] X;
  delete[] Y;


  return 0;
}
