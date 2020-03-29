#include <mpi.h>
#include <boost/mpi.hpp>
//#include <boost/serialization/serialization.hpp>
//#include <boost/archive/text_oarchive.hpp>
//#include <boost/archive/text_iarchive.hpp>
#include <string>
//#include <sstream>
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
	boost::mpi::environment env;
	boost::mpi::communicator world;

	if (world.rank() == 0)
	{
		Person p;
		p.name = "Grisha";
		p.age = 123;
		for (int i = 1; i < world.size(); i++)
			world.send(i, 0, p);
	}
	else
	{
		Person p;
		world.recv(0, 0, p);
		cout << "Hello, " << p.name << endl;
	}

	return 0;
}