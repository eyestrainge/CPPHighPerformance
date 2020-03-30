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
		int value = 42;
		for (int i = 1; i < size; i++)
		{
			cout << "Ready to send " << rank << "-->" << i << endl;
			// Standard: Returns as soon as message is sent 
			//MPI_Send(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			// Synchronous: Returns only when the exchange is complete (handshake)
			MPI_Ssend(&value, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
			cout << "Data sent     " << rank << "-->" << i << endl;
		}
	}
	else
	{
		int value = -1;
		MPI_Recv(&value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
		cout << rank << " received from 0 value " << value << endl;
	}

	MPI_Finalize();

	return 0;
}