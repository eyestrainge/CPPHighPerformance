#include <intrin.h>
#include <string>
#include <iostream>
using namespace std;

string get_cpu_name()
{
	uint32_t data[4] = { 0 };

	_asm
	{
		cpuid;
		mov data[0], ebx;
		mov data[4], edx;
		mov data[8], ecx;
	}

	return string((const char*)data);
}

void assembler()
{
	cout << "CPU: " << get_cpu_name() << endl;

	float f1[] = { 1.f, 2.f, 3.f, 4.f };
	float f2[] = { 5.f, 4.f, 3.f, 2.f };
	float result[4] = { 0.f };

	// Go to Streaming_SIMD_Extensions Wiki
	_asm
	{
		movups xmm1, f1;
		movups xmm2, f2;
		mulps xmm1, xmm2;
		movups result, xmm1;
	}

	for (size_t i = 0; i < 4; i++)
	{
		cout << result[i] << "\t";
	}
	cout << endl;

	// Go to CPUID Wiki
	int d, c;
	_asm
	{
		mov eax, 1;
		cpuid; // Wiki for feature sets table
		mov d, edx;
		mov c, ecx;
	}

	if ((d & (1 << 26)) != 0)
		cout << "SSE2 is supported " << endl;
	if ((c & 1) != 0)
		cout << "SSE3 is supported " << endl;
	if ((c & (1 << 19)) != 0)
		cout << "SSE4.1 is supported " << endl;
	if ((c & (1 << 20)) != 0)
		cout << "SSE4.2 is supported " << endl;
}

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

int main(int argc, char* argv[])
{
	//assembler();
	intrinsics();

	return 0;
}