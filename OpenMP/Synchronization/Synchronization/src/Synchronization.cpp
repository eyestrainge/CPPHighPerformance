#include <omp.h>
#include <vector>
#include <chrono>
#include <iostream>
#include <cstdio>
using namespace std;

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
#pragma omp single nowait
		{
			int n;
			cin >> n;
			printf("gathering input: %d\n", omp_get_thread_num());
			// Implied barrier
		}

		printf("in parallel on %d\n", omp_get_thread_num());
#pragma omp barrier

#pragma omp master
		printf("output on: %d\n", omp_get_thread_num());
	}
}

void sync()
{
	printf("\nATOMIC\n");
	int sum = 0;
#pragma omp parallel for num_threads(128)
	for (int i = 0; i < 100; i++)
	{
#pragma omp atomic
		++sum;
	}
	cout << sum;

	printf("\nORDERED\n");
	vector<int> squares; // std::vector typically not a thread safe collection
#pragma omp parallel for ordered
	for (int i = 0; i < 20; ++i)
	{
		printf("%d : %d\t", omp_get_thread_num(), i);
		int j = i * i;
#pragma omp ordered
		squares.push_back(j); // Non thread safety is no big deal on this scale
	}
	printf("\n");
	for (auto v : squares) printf("%d\t", v);
}

int main(int argc, char* argv[])
{
	//sections();
	single_master();

	//sync();

	getchar();
	return 0;
}