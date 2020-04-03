#include <amp.h>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
using namespace concurrency;
using namespace std;

string print_matrix(const float* mtx, const int dim)
{
	ostringstream oss;
	oss << "\n";
	for (int i = 0; i < dim; ++i)
	{
		oss << "[";
		for (int j = 0; j < dim; ++j)
		{
			oss << mtx[i * dim + j];
			if (j + 1 < dim)
				oss << ",\t";
		}
		oss << "\t]\n";
	}
	return oss.str();
}

void naive_multiply(float* a, float* b, float* c, const int dim)
{
	array_view<float, 2> av(dim, dim, a);
	array_view<float, 2> bv(dim, dim, b);
	array_view<float, 2> cv(dim, dim, c);
	cv.discard_data();

	parallel_for_each(
		cv.extent,
		[=](index<2> idx) restrict(amp)
		{
			auto row = idx[0];
			auto col = idx[1];
			auto sum = 0.f;

			for (int i = 0; i < dim; i++)
				sum += av(row, i) * bv(i, col);
			cv[idx] = sum;
		}
	);
	cv.synchronize();
}

template <int ts>
void tiled_multiply(float* a, float* b, float* c, const int dim)
{
	array_view<float, 2> av(dim, dim, a);
	array_view<float, 2> bv(dim, dim, b);
	array_view<float, 2> cv(dim, dim, c);
	cv.discard_data();

	parallel_for_each(
		cv.extent.tile<ts, ts>(),
		[=](tiled_index<ts, ts> idx) restrict(amp)
		{
			tile_static float al[ts][ts]; // l = local
			tile_static float bl[ts][ts];

			int rowl = idx.local[0];
			int coll = idx.local[1];
			int rowg = idx.global[0];
			int colg = idx.global[1];

			auto sum = 0.f;

			for (int i = 0; i < dim; i += ts)
			{
				al[rowl][coll] = av(rowg, coll + i);
				bl[rowl][coll] = bv(rowl + i, colg);
				idx.barrier.wait();

				for (int j = 0; j < ts; ++j)
					sum += al[rowl][j] * bl[j][coll];
				idx.barrier.wait();
			}
			cv[idx.global] = sum;
		}
	);
	cv.synchronize();
}

void matrix_multiplication()
{
	const int dim = 4;
	float a[dim * dim];
	float b[dim * dim];
	float c[dim * dim];

	for (int i = 0; i < dim; i++)
	{
		for (int j = 0; j < dim; j++)
		{
			a[i * dim + j] = i * dim + j;
			b[i * dim + j] = j * dim + i;
		}
	}

	//naive_multiply(a, b, c, dim);
	tiled_multiply<2>(a, b, c, dim);

	cout
		<< "The product of " << print_matrix(a, dim)
		<< " and " << print_matrix(b, dim)
		<< " is " << print_matrix(c, dim) << endl;
}

int main(int argc, char* argv[])
{
	matrix_multiplication();

	return 0;
}