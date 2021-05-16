#ifndef __PAR_LOOP_H
#define __PAR_LOOP_H

#include <functional>
#include <vector>
#include <thread>
using namespace std;

class ParLoop
{
public:
  /// @brief execute the function f multiple times with different
  /// parameters possibly in parallel
  ///
  /// f will be executed multiple times with parameters starting at
  /// beg, no greater than end, in inc increment. These execution may
  /// be in parallel
  void parfor(size_t beg, size_t end, size_t val, size_t nbthread,
              std::function<void(int)> f)
  {
    val = val + 1;
    if ((end - val) > nbthread)
    {
      beg = val * (end / nbthread);
      end = beg + (end / nbthread);
    }
    for (size_t i = val; i < end; i += 2)
    {
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
  template <typename TLS>

  void parfor(size_t beg, size_t end, size_t increment, size_t nbthread,
              std::function<void(TLS &, int)> before,
              std::function<void(vector<TLS>&, int)> f,
              // std::function<void(TLS&, int)> g,
              std::function<void(TLS &, int)> after)
  {
    vector<TLS> tlsVector(end);
    vector<thread> threads;
    for (int i = 0; i < nbthread; i++)
    {
      before(tlsVector[i], i);
    }

    size_t x;
    for (size_t t = 0; t < end; ++t)
    {
      x = t % 2;
      if (x == 1)
      {
        threads.push_back(thread(&ParLoop::parfor, this, t, end, x, nbthread, [&, tlsVector](int j) -> void {
          // int i=j;
          f(&tlsVector, (int)j);
        }));
      }
      else
      {
        threads.push_back(thread(&ParLoop::parfor, this, t, end, x, nbthread, [&, tlsVector](int j) -> void {
          // int i=j;
          f(&tlsVector, (int)j);
        }));
      }
    }
    for (auto &itr : threads)
    {
      itr.join();
    }

    for (int i = 0; i < nbthread; i++)
    {
      after(tlsVector[i], i);
    }
  }
};

#endif
