#include <mpi.h>
#include <iostream>
using namespace std;

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		cout << "Enter a number: ";
		int n;
		cin >> n;
		for (int i = 1; i < size; i++)
			MPI_Send(&n, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
	}
	else
	{
		int n;
		MPI_Recv(&n, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		// MPI is smart enough to proxy results to remote machine
		// and print locally whats printing on another machine
		// thus very helpful to set up such demonstrations
		cout << n << "^" << rank << " = " << pow(n, rank) << endl;
	}

	MPI_Finalize();

	return 0;
}