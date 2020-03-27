#include <intrin.h>
#include <string>
#include <random>
#include <chrono>
#include <iostream>
using namespace std;

void intrinsics()
{
	//int cpuInfo[4];
	//__cpuid(cpuInfo, 1); // 2nd arg as eax value

	auto a = _mm_set_ps(1, 2, 3, 4);
	auto b = _mm_set_ps(4, 3, 2, 1);
	auto c = _mm_add_ps(a, b);

	float f = c.m128_f32[0];
	cout << "f = " << f << endl;;

	__try
	{
		__m256d z = _mm256_set_pd(1, 2, 3, 4);
	}
	__except (1)
	{
		cout << "Sorry, you cannot execute this" << endl;
	}
}

void simple_mad(float* a, float* b, float* c, float* result, const int length)
{
	for (size_t i = 0; i < length; i++)
		result[i] = a[i] * b[i] + c[i];
}

void optimization()
{
	using namespace chrono;

	const int length = 1024 * 1024 * 64;
	float* a = new float[length];
	float* b = new float[length];
	float* c = new float[length];
	float* result = new float[length];

	mt19937_64 rng(random_device{}());
	uniform_real_distribution<float> dist(0, 1);

	for (size_t i = 0; i < length; i++)
	{
		a[i] = dist(rng);
		b[i] = dist(rng);
		c[i] = dist(rng);
	}

	auto begin = high_resolution_clock::now();
	simple_mad(a, b, c, result, length);
	auto end = high_resolution_clock::now();
	cout << "MAD took up " << duration_cast<milliseconds>(end - begin).count() << "msec" << endl;

	delete[] a;
	delete[] b;
	delete[] c;
	delete[] result;
}

int main(int argc, char* argv[])
{
	optimization();

	return 0;
}