#include <amp.h>
#include <iostream>
using namespace concurrency;
using namespace std;

const int sz = 3;

void add_in_cpp()
{
	// h = host, d = device
	int ah[] = { 1,2,3 };
	int bh[] = { 5,7,9 };
	int sumh[sz];

	for (int i = 0; i < sz; i++)
		sumh[i] = ah[i] + bh[i];
	
	for (int i = 0; i < sz; i++)
		cout << sumh[i] << "\t";
	cout << endl;
}

void add_in_amp()
{
	// h = host, d = device
	int ah[] = { 1,2,3 };
	int bh[] = { 5,7,9 };
	int sumh[sz];

	array_view<const int, 1> ad(sz, ah);
	array_view<const int, 1> bd(sz, bh);
	array_view<int, 1> sumd(sz, sumh);
	sumd.discard_data();
	
	parallel_for_each(
		sumd.extent,
		[=](index<1> idx) restrict(amp)
		{
			int a = ad[idx];
			int b = bd[idx];
			sumd[idx] = a + b;
		}
	);

	sumd.synchronize();

	for (int i = 0; i < sz; i++)
		cout << sumh[i] << "\t";
	cout << endl;
}

int main(int argc, char* argv[])
{
	//add_in_cpp();
	
	add_in_amp();

	return 0;
}