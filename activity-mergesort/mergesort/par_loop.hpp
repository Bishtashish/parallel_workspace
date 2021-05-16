#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>
#include <thread>
#include <vector>
using namespace std; 

class ParLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parforIn (size_t beg, size_t end, size_t increment,size_t nbthread,
	       std::function<void(int)> f) {
	  
	  if((end-beg) >nbthread){
          beg = beg*(end/nbthread);
	      end = beg+ (end/nbthread);
	  }
		  for(size_t i=beg;i<end;i+=increment){
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
	       std::function<void(TLS&, int)> before,
	       std::function<void(TLS&, int, int)> f,
	       std::function<void(TLS&, int)> after
	       ) {
    std::vector<thread> threads;
    std::vector<TLS> tlsVector(end+1);
    for(int i =0;i<nbthread;i+=1){		  
      before(tlsVector[i],i);  
    }
	     
	for( size_t t = beg; t<nbthread; t+=increment){
         threads.push_back(thread(&ParLoop::parforIn, this, 0, end, increment, nbthread, 
             [&, t](int j) -> void {
               f(tlsVector[0], j, t);
             })); 
           } 
    for (auto &itr : threads)
    {
        itr.join();
    }
    for (int i=0;i<nbthread;++i)
    {
            after(tlsVector[i],i);
    }
         }
};

#endif
