#include <iostream>
#include <mpi.h>

using namespace std;

void worker_process(int myRank, int numProcs) {
	int value;
	// worker receives and sends.
	// wait for msg from my neighbor to the left. 
	int src = myRank -1;
	int count = 1;
	int tag = 0;
	MPI_Status status;
	MPI_Recv(&value, count, MPI_INT, src, tag, MPI_COMM_WORLD, &status);

	cout << "Worker " << myRank << " received " << value << "\n";
	value++;
	cout << "Value is now " << value << "\n";
	// then I send to my neighbor to the right
	int dest = myRank + 1;
	//check if im the last one
	if (dest >= numProcs) { 
		dest = 0; // send to main. 
	}
	MPI_Send(&value, count, MPI_INT, dest, tag, MPI_COMM_WORLD);

}

void main_process(int numProcs) {
	MPI_Status status;
	int value; 
	cout << "Please enter a value to pass around: ";
	cin >> value;

	int dest = 1; 
	int tag = 0; 
	int count = 1;

	// this blocks everything
	MPI_Send(&value, count, MPI_INT, dest, tag, MPI_COMM_WORLD);
	
	int src = numProcs -1;

	// this blocks everything
	MPI_Recv(&value, count, MPI_INT, src, tag, MPI_COMM_WORLD, &status);
	// we use &value because the code blocks, so we know that memory address will be ok to overwrite. 
	// race conditions, deadlock, can go wrong. 

	// value++;
	cout << "Final value = " << value << endl;
	
}



int main(int argc, char* argv[])
{
	int myRank, numProcs;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numProcs);
	MPI_Comm_rank(MPI_COMM_WORLD, &myRank);

	if (myRank > 0) { // worker:
		worker_process(myRank, numProcs);
		//cout << "Hello from worker: rank=" << myRank << endl;

	}
	else { // main:
		main_process(numProcs);
		//cout << "Hello from main: rank=" << myRank
		//	<< ", total of " << numProcs << " processes"
		//	<< endl;

	}

	MPI_Finalize();
	return 0;
}
