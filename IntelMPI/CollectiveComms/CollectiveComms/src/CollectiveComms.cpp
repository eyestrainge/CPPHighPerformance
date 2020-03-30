#include <mpi.h>
#include <random>
#include <iostream>
using namespace std;

int broadcast(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	int value = 42;
	MPI_Bcast(&value, 1, MPI_INT, 0, MPI_COMM_WORLD);

	cout << "Rank " << rank << " received from 0 the value " << value << endl;
	MPI_Barrier(MPI_COMM_WORLD);
	cout << "Rank " << rank << " is done working" << endl;

	MPI_Finalize();
	return 0;
}

int reduce(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);
	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	const int ItemsPerProcess = 10;
	const int count = size * ItemsPerProcess;
	int* data = new int[count];
	if (rank == 0)
	{
		for (int i = 0; i < count; i++)
			data[i] = rand() % 10;
	}

	int* LocalData = new int[ItemsPerProcess];
	MPI_Scatter(
		data, ItemsPerProcess, MPI_INT,
		LocalData, ItemsPerProcess, MPI_INT,
		0, MPI_COMM_WORLD
	);

	int LocalSum = 0;
	for (int i = 0; i < ItemsPerProcess; i++)
		LocalSum += LocalData[i];
	delete[] LocalData;

	int GlobalSum;
	MPI_Reduce(
		&LocalSum, &GlobalSum, 1, MPI_INT,
		MPI_SUM, 0, MPI_COMM_WORLD
	);

	if (rank == 0)
	{
		cout << "Total sum = " << GlobalSum << endl;
		delete[] data;
	}

	MPI_Finalize();
	return 0;
}

int main(int argc, char* argv[])
{
	random_device rd{};
	srand(rd());
	//return broadcast(argc, argv);
	return reduce(argc, argv);
}