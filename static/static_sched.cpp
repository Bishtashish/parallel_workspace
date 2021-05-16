#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <chrono>
#include <cmath>
#include "par_loop.hpp"
using namespace std::chrono;

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif


int main (int argc, char* argv[]) {

  if (argc < 7) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads>"<<std::endl;
    return -1;
  }
  
  auto start = high_resolution_clock::now();
  int functionid = atoi(argv[1]);
  float a = strtof(argv[2],NULL);
  float b = strtof(argv[3],NULL);
  float n = strtof(argv[4],NULL);
  int intensity = atoi(argv[5]);
  int nbthread = atoi(argv[6]);
  float x;
  float t1 = (b - a) / n;
  float sum = 0;
  ParLoop pl;

  pl.parfor<float>(0,n,1,nbthread,
		   [&](float& tls) -> void{
		     tls =0;
		 },
		   [&](float&tls,int i) -> float{
		    x = a + ((i + 0.5) * t1);

		       	switch(functionid)
		{
      			case 1: tls += f1(x,intensity);
				break;
			case 2: tls += f2(x,intensity);
	  			break;
	  		case 3: tls += f3(x,intensity);
	    			break;
	    		case 4: tls += f4(x,intensity);
	      			break;
	      		default: return 0;
    		}
			return tls;
		  },
		 [&](float tls) -> void{sum +=tls;
		 });


  

  
  //  for(int i = 0; i < n; i++)
  //{
  //	x = a + ((i + 0.5) * t1);
  //
  //	switch(functionid)
  //		{
  //  			case 1: sum += f1(x,intensity);
  //				break;
  //			case 2: sum += f2(x,intensity);
  //	  			break;
  //	  		case 3: sum += f3(x,intensity);
  //	    			break;
  //	    		case 4: sum += f4(x,intensity);
  //	      			break;
  //	      		default: return -1;
  //		}
  //}

  auto stop = high_resolution_clock::now();
  fprintf(stdout,"%f\n", sum * t1);
  
  duration<double> count1 = duration_cast<duration<double>>(stop - start);
  fprintf(stderr,"%f\n",count1.count());
  
  return 0;
}
