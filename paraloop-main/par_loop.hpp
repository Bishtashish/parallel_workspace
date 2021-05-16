#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>

class ParLoop {
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parforIn (size_t beg, size_t end, size_t nbthread,size_t granularity,
	       std::function<void(int)> f) {
	  beg = beg*(end/nbthread);
	  end = beg+ (end/nbthread);
	  if((end-beg) >granularity){
    		for (size_t i=beg; i<end; i+=1) {
      			f(i);
    		}
	  }
	  else {
		  for(size_t i=beg;i<beg+granularity;i+=1){
			  if(i==end) break;
			  f(i);
		  }
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
	       size_t granularity,
	       std::function<void(TLS&)> before,
	       std::function<float(TLS&, int)> f,
	       std::function<void(TLS&)> after
	       ) {
    vector<thread> threads;
    vector<TLS> tlsVector(nbthread);
    for(int i =0;i<nbthread;i+=1){		  
      before(tlsVector[i]);  
    }
    

      
//      for( size_t t =beg; t<nbthread; ++t){
//        tls = 0;
//        for(size_t j =t; j<end;j+=nbthread){	 
//     	  tls=f(tls,j);
//      }
//     	sum+=tls;
//      }
	     
	for( size_t t = beg; t<nbthread; t+=1){
         threads.push_back(thread(&ParLoop::parforIn, this, t, end, nbthread, granularity, 
             [&, t](int j) -> void {
               f(tlsVector[t], j);
             })); 
           } 
           for (auto &itr : threads)
           {
             itr.join();
           }
           for (auto &itr : tlsVector)
           {
             after(itr);
           }
         }
};

#endif

