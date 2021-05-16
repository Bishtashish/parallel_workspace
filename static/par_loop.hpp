#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>
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
  void parfor (size_t t, size_t end, size_t nbthread,
	       std::function<void(int)> f) {
	  if((end-beg)>nbthread){
	  int beg = t*(end/nbthread);
	  end = beg+(end/nbthread);
	  }
    for (size_t i=beg; i<end; i+= 1) {
      f(i);
    }
  }

  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel.
  ///
  /// Each thread that participate in the calculation will have its
  /// own TLS object.
  ///
  /// Each thread will execute function before prior to any
  /// calculation on a TLS object.
  ///
  /// Each thread will be passed the TLS object it executed before on.
  ///
  /// Once the iterations are complete, each thread will execute after
  /// on the TLS object. No two thread can execute after at the same time.
  template<typename TLS>
  void parfor (size_t beg, size_t end, size_t increment, size_t nbthread,
	       std::function<void(TLS&)> before,
	       std::function<float(TLS&, int)> f,
	       std::function<void(TLS&)> after
	       ) {
    vector<TLS> tlsVector(nbthread);
    vector<thread> threads;
    for(int i=0;i<nbthread;i++){	    
    before(tlsVector[i]);
    }
    // for (size_t i=beg; i<end; i+= increment) {
    //f(tls,i);
    //}
    
    float sum =0;
	  
//     for(size_t a = beg;a<end;a+=nbthread){    
//     for(size_t t = a;t<a+nbthread && t<end; ++t){
	 for(size_t t=0;t<end;++t){
	 threads.push_back(thread(&ParLoop::parfor, this, t, end,nbthread,[&,t](int i) -> void{
		 f(tlsVector[t],i);
	 }));
    }
	  for(auto &itr :threads){
		  itr.join();
	  }
//     }
//       tls = 0;
//       for( size_t j = t;j<end; j+=nbthread){
// 	tls =f(tls, j);
//       }
      //
      //sum+=tls; 
    for((auto &itr : tlsVector){	    
    after(itr);
  }
	}
  
};

#endif

