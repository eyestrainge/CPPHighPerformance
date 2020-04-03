#include <amp.h>
#include <vector>
#include <iostream>
using namespace concurrency;
using namespace std;

const int sz = 3;

accelerator_view setup()
{
	vector<accelerator> all = accelerator::get_all();
	for (accelerator& a : all)
	{
		wcout << a.description;
		cout << " has " << a.dedicated_memory / 1e6 << "GB dedicated memory,";
		if (a.supports_cpu_shared_memory)
			cout << " supports CPU shared memory, ";
		if (a.supports_double_precision)
			cout << " suppots double precision";
		cout << endl;

		//bool success = accelerator::set_default(a.device_path);
	}

	accelerator gpu(accelerator::default_accelerator);
	//accelerator::direct3d_ref; // emulator for debugging purposes
	//accelerator::direct3d_warp; // fallback to cpu cores and sse insturctions
	return gpu.default_view;
}

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

void add_in_amp(accelerator_view acc_view)
{
	// h = host, d = device
	int ah[] = { 1,2,3 };
	int bh[] = { 5,7,9 };
	int sumh[sz];

	array_view<const int, 1> ad(sz, ah);
	array_view<const int, 1> bd(sz, bh);
	array_view<int, 1> sumd(sz, sumh);
	sumd.discard_data();

	parallel_for_each( // acc_view could go here explicitly
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
	auto acc_view = setup();
	//add_in_cpp();
	add_in_amp(acc_view);

	return 0;
}