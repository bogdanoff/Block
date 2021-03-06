#include "global.h"
#include "wrapper.h"
#ifndef SERIAL
#include "mpi.h"
#endif
#include "stdio.h"
#include "stdlib.h"
#include <string>

using namespace std;


int main(int argc, char* argv []) {

  int rank=0, size=1;
#ifndef SERIAL
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Group orig_group, calc_group;
  MPI_Comm_group(MPI_COMM_WORLD, &orig_group);
  MPI_Comm_split(MPI_COMM_WORLD, 0, rank, &Calc);
  calc = boost::mpi::communicator(Calc, boost::mpi::comm_attach);

#endif
  initBoostMPI(argc, argv);
  ReadInputFromC(argv[1], 0);
  dmrginp.matmultFlops.resize(numthrds, 0.);

  double* stackmemory = new double[dmrginp.getMemory()];
  Stackmem.resize(numthrds);
  Stackmem[0].data = stackmemory;
  Stackmem[0].size = dmrginp.getMemory();
  //************
  //memset(stackmemory, 0, dmrginp.getMemory()*sizeof(double));

  int mpsstate=0;
  
  //readMPSFromDiskAndInitializeStaticVariables();
  //initializeGlobalMPS(mpsstate);

  if (rank ==0) 
    printf("Reading file %s\n", argv[2]);
  test(argv[2]);
  //exit(0);
  //
  //

  /*
  double overlap, hvalue;

  unsigned long temp=1, occ=0;

  string occstring = "1 1 0 0 1 1 0 0 1 1 0 0 0 0 1 1";
  intFromString(occ, occstring.c_str());

  evaluateOverlapAndHamiltonian(&occ, 1, &overlap, &hvalue);

  printf("<D|0> = %18.10f   <D|H|0> = %18.10f   <D|H|0>/<D|0> = %18.10f \n", overlap, hvalue, hvalue/overlap);
 
  occstring = "1 0 0 1 1 1 0 0 1 1 0 0 0 0 1 1";
  intFromString(occ, occstring.c_str());

  evaluateOverlapAndHamiltonian(&occ, 1, &overlap, &hvalue);

  printf("<D|0> = %18.10f   <D|H|0> = %18.10f   <D|H|0>/<D|0> = %18.10f \n", overlap, hvalue, hvalue/overlap);
  */ 



#ifndef SERIAL
  MPI_Comm_free(&Calc);
  MPI_Finalize();
#endif
  return 0;
}
