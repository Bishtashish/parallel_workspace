#include <iostream>
#include <chrono>

//
float polynomial(float x, float *poly, int degree)
{
  float out = 0.;
  float xtothepowerof = 1.;
  for (int i = 0; i <= degree; ++i)
  {
    out += xtothepowerof * poly[i];
    xtothepowerof *= x;
  }
  return out;
}

void polynomial_expansion(float *poly, int degree,
                          int n, float *array)
{

#pragma omp parallel for schedule(runtime)
  for (int i = 0; i < n; ++i)
  {
    array[i] = polynomial(array[i], poly, degree);
  }
}

__global__ void polynomial_expansion_cuda(float *poly int degree, int n, float *array)
{
  float out = 0;
  float xdegree = 1.;
  int i = blockIdx.x * blockDim.x + threadIdx.x;
  if (i < n)
  {
    float x = array[i];
    for (int k = 0; k < degree; ++k)
    {
      out = +xdegree * poly[k];
      xdegree *= x;
    }
    array[i] = out;
  }
}

int main(int argc, char *argv[])
{
  //TODO: add usage

  int n = atoi(argv[1]); //TODO: atoi is an unsafe function
  int degree = atoi(argv[2]);
  int nbiter = atoi(argv[3]);

  float *array = new float[n];
  float *poly = new float[degree + 1];
  for (int i = 0; i < n; ++i)
    array[i] = 1.;

  for (int i = 0; i < degree + 1; ++i)
    poly[i] = 1.;

  std::chrono::time_point<std::chrono::system_clock> begin, end;
  begin = std::chrono::system_clock::now();

  //  pre cuda

  float *d_array, *d_poly;
  //  float size = sizeof(float);
  cudaMalloc((void **)&d_array, n*sizeof(float));
  cudaMalloc((void **)&d_poly, (degree + 1)*sizeof(flaot));

  cudaMemcpy(d_array, &array, n, cudaMemcpyHostToDevice);
  cudaMemcpy(d_poly, &poly, degree + 1, cudaMemcpyHostToDevice);

  for (int iter = 0; iter < nbiter; ++iter){
    // polynomial_expansion (poly, degree, n, array);
    polynomial_expansion_cuda<<<(n+255)/256,256>>>(d_poly degree, n, d_array);
  }
  cudaDeviceSynchronize();
  cudaMemcpy(array, d_array, n*sizeof(float), cudaMemcpyDeviceToHost);



  end = std::chrono::system_clock::now();
  std::chrono::duration<double> totaltime = (end - begin) / nbiter;

  std::cerr << array[0] << std::endl;
  std::cout << n << " " << degree << " " << totaltime.count() << std::endl;

  delete[] array;
  delete[] poly;
  cudaFree(d_array); 
  cudaFree(d_poly);

  return 0;
}
