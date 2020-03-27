#include <omp.h>
#include <chrono>
#include <iostream>
#include <cstdio>
using namespace std;

void hello_openmp()
{
	omp_set_num_threads(8);

#pragma omp parallel
	{
#pragma omp critical
		cout << "Hello, OpenMP " << omp_get_thread_num() << "/" << omp_get_num_threads() << endl;
	}
}

void pfor()
{
	const int length = 1024 * 1024 * 64;
	float* a = new float[length],
		* b = new float[length],
		* c = new float[length],
		* result = new float[length];

#pragma omp parallel for
	for (int i = 0; i < length; i++)
		result[i] = a[i] + b[i] * erff(c[i]);

	delete[] a;
	delete[] b;
	delete[] c;
	delete[] result;
}

void sections()
{
#pragma omp parallel sections
	{
#pragma omp section
		{
			for (int i = 0; i < 1000; i++)
				cout << i;
		}
#pragma omp section
		{
			for (int i = 0; i < 1000; i++)
				cout << static_cast<char>('a' + (i % 26));
		}
	}
}

void single_master()
{
#pragma omp parallel
	{
#pragma omp single
		printf("gathering input: %d\n", omp_get_thread_num());
		// Implied barrier

		printf("in parallel on %d\n", omp_get_thread_num());
#pragma omp barrier

#pragma omp master
		printf("output on: %d\n", omp_get_thread_num());
	}
}

int main(int argc, char* argv[])
{
	//hello_openmp();

	//pfor();
	//sections();
	single_master();

	return 0;
}