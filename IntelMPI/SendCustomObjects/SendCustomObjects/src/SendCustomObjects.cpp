#include <mpi.h>
#include <boost/serialization/serialization.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <string>
#include <sstream>
#include <iostream>
using namespace std;

struct Person
{
	string name;
	int age;

	friend class boost::serialization::access;

	template <typename Ar>
	void serialize(Ar& ar, const unsigned int version)
	{
		ar& name;
		ar& age;
	}
};

int main(int argc, char* argv[])
{
	MPI_Init(&argc, &argv);

	int size, rank;
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	if (rank == 0)
	{
		Person p;
		p.name = "Grisha";
		p.age = 123;

		ostringstream oss;
		boost::archive::text_oarchive ar(oss);
		ar << p;

		auto s = oss.str();
		int len = s.length() + 1;
		for (int i = 1; i < size; i++)
			MPI_Send(s.c_str(), len, MPI_CHAR, i, 0, MPI_COMM_WORLD);
	}
	else
	{
		MPI_Status status;
		MPI_Probe(0, 0, MPI_COMM_WORLD, &status);
		int count;
		MPI_Get_count(&status, MPI_CHAR, &count);
		cout << "We have " << count;

		char* buffer = new char[count];
		// Throws bad termination error. Couldn't debug why though.
		MPI_Recv(&buffer, count, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &status);
		cout << "We got " << buffer << endl;

		istringstream iss(buffer);
		boost::archive::text_iarchive ar(iss);
		Person p;
		ar >> p;
		cout << "Hello, " << p.name << "! You are " << p.age << " years old." << endl;

		delete[] buffer;
	}

	MPI_Finalize();

	return 0;
}