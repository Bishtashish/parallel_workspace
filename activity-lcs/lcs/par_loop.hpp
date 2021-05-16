#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>
#include <omp.h>
#include <vector>
#include <thread>
using namespace std;

class ParLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parforIn (size_t beg, size_t end, size_t nbthread,
	       std::function<void(int)> f) {

    if((end-beg)>nbthread){
    beg = beg*(end/nbthread);
    end =beg+(end/nbthread);
    }
    for (size_t i=beg; i<end; i+= 1) {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different

  template<typename TLS>
  
  void parfor (size_t beg, size_t end_X, size_t end_Y, size_t nbthread,
	       std::function<void()> before,
	       std::function<void( int, int, int)> f,
	       std::function<void()> after
	       ) {
    //TLS tls= new int*[end_X+1];
    vector<TLS> tlsVector(nbthread);
    vector<thread> threads;
    for(int i=0;i<nbthread;i++){
    before();
    }
    int ad = 0;
    for( int i =0,j=0;j<=end_Y &&i<=end_X;j++){
      ad = std::min(j,(int)end_X-i);

      for(int t=0;t<=ad;t+=1){
	threads.push_back(thread(&ParLoop::parforIn, this, t,ad,nbthread,[&,t](int k) -> void{
	f(k,i,j);
	if(j>=end_Y)
	  j=end_Y-1,i++;
	    }));  
      }
      for(auto &itr :threads){
	itr.join();
      }
    }
	after();
      
  }
      
};

#endif
