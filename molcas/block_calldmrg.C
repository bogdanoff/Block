#include "block_calldmrg.h"
#include "molpro_fcidump.h"
#include "sortNpdm.h"

/// Fortran wrapper
void block_calldmrg_ (
      const FORTINT* Restart,
      const FORTINT* N_roots,
      const FORTINT* N_act,
      const FORTINT* N_elec,
      const FORTINT* M_s,
      const char* Sym,
      const FORTINT* iSym,
      const FORTINT* OrbSym,
      const double* E_core,
      const double* h0,
      const double* tuvx,
      const FORTINT* M_state,
      const FORTINT* N_pdm,
      const double* T_sweep,
      const double* T_noise,
            double* E_sweep,
      const FORTINT* hf_occ,
            FORTINT* nrs2t)
{
  block_calldmrg(*Restart, *N_roots, *N_act, *N_elec, *M_s, Sym, *iSym, OrbSym, *E_core, h0, tuvx, *M_state, *N_pdm, *T_sweep, *T_noise, E_sweep, hf_occ, *nrs2t);
}

extern int calldmrg(char*, char*);

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

#include <vector>
using std::vector;
#include <sweep_params.h>

#include <communicate.h> // enable Boost MPI wrappers if defined SERIAL

#include <boost/filesystem.hpp>
#include <boost/serialization/string.hpp>
#include <boost/algorithm/string.hpp>

/// Dump 2-el integrals to formatted file
void block_calldmrg (
      const FORTINT& Restart,
      const FORTINT& N_roots,
      const FORTINT& N_act,
      const FORTINT& N_elec,
      const FORTINT& M_s,
      const char* Sym,
      const FORTINT& iSym,
      const FORTINT* OrbSym,
      const double& E_core,
      const double* h0,
      const double* tuvx,
            FORTINT  M_state,
            FORTINT  N_pdm,
            double  T_sweep,
            double  T_noise,
            double* E_sweep,
      const FORTINT* hf_occ,
            FORTINT nrs2t)
{
  using std::endl;
  using std::setw;

#ifndef SERIAL
  boost::mpi::communicator world;
#endif

//std::string symlab(Sym,3);
//boost::algorithm::tolower(symlab);

  std::string prefix;
  if(mpigetrank() == 0) prefix = boost::filesystem::current_path().c_str();

#ifndef SERIAL
  boost::mpi::broadcast(world, prefix, 0);
#endif

  char input[64];
  {
    std::ostringstream oss; oss << "dmrg.conf." << mpigetrank();
    strcpy(input, oss.str().c_str());
  }
  char output[64];
  {
    std::ostringstream oss; oss << "dmrg.out." << mpigetrank();
    strcpy(output, oss.str().c_str());
  }

// if(mpigetrank() == 0) // branch by mpi rank == 0
  if(1) // create config file for every procs/nodes
  {
    /// Dump integrals as MOLPRO format
    molpro_fcidump(N_act,N_elec,M_s,iSym,OrbSym,E_core,h0,tuvx);

    /// Create config file
    std::ofstream fcon(input);

    std::string symlab(Sym,3);

    fcon << "nelec " << setw(2) << N_elec << endl;
    fcon << "spin  " << setw(2) << M_s << endl;
    fcon << "irrep  " << iSym << endl;

    int N_sweep = 0;
    int M_start = (Restart == 0) ? 250 : M_state;
    double T_start = T_noise;

    fcon << "schedule" << endl;
    while(M_start < M_state) {
      fcon << setw(2) << N_sweep << setw(5) << M_start << " " << T_start << " " << T_start << endl;
      N_sweep += 8;
      M_start *= 2;
    }
//  while(T_start > T_sweep && T_start > 1.0e-6) {
//    fcon << setw(2) << N_sweep << setw(5) << M_state << " " << T_start << " " << T_start << endl;
//    N_sweep += 2;
//    T_start /= 10;
//  }
    while(T_start >= T_sweep) {
      fcon << setw(2) << N_sweep << setw(5) << M_state << " " << T_start << " " << T_start << endl;
      N_sweep += 4;
      T_start /= 10;
    }
    fcon << setw(2) << N_sweep << setw(5) << M_state << " " << T_sweep/10 << " 0.0" << endl;
    if(Restart != 1) {
      N_sweep += 8;
      fcon << setw(2) << N_sweep << setw(5) << M_state << " " << T_sweep/10 << " 0.0" << endl;
    }
    fcon << "end" << endl;

    if(Restart == 0) {
      fcon << "warmup local_4site" << endl;
    }
    else {
      if(Restart == 1 && N_roots > 1)
        fcon << "fullrestart" << endl;
      else
        fcon << "fullrestart" << endl;
        fcon << "reset_iter" << endl;
    }

    if(Restart != 1)
      fcon << "twodot_to_onedot " << N_sweep << endl;
    else
      fcon << "onedot" << endl;

    fcon << "maxiter " << N_sweep+20 << endl;
    fcon << "sweep_tol " << T_sweep << endl;

    switch (N_pdm) {
      case 1:
        fcon << "onepdm" << endl;
        fcon << "new_npdm_code" << endl;
        break;
      case 2:
        fcon << "twopdm" << endl;
        fcon << "new_npdm_code" << endl;
        break;
      case 3:
        fcon << "threepdm" << endl;
//        fcon << "disk_dump_pdm" << endl;
//        fcon << "npdm_no_intermediate" << endl; // FIXME: this is temporary fix to avoid failure in multi-node/disk run...
        break;
      case 4:
        fcon << "fourpdm" << endl;
        fcon << "disk_dump_pdm" << endl;
        fcon << "npdm_no_intermediate" << endl; // FIXME: this is temporary fix to avoid failure in multi-node/disk run...
        break;
      default:
        exit(1); // Block only supports 1-4RDMs
    }

//  fcon << "store_spinpdm" << endl;
//  fcon << "prefix " << prefix << endl;
    fcon << "orbitals FCIDUMP" << endl;
    fcon << "symmetry " << symlab << endl;
//    fcon << "gaopt default" << endl;

    int nhf_occ = 0;
    for (int k=0;k<nrs2t;++k) {
          nhf_occ = nhf_occ + hf_occ[k];
    }

    if (nhf_occ == N_elec)
    {
        fcon << "hf_occ ";
        for(int k=0;k<nrs2t;++k){
        fcon << hf_occ[k] << ' ';
        }
        fcon << endl;
    }
    else
    {
        fcon << "hf_occ integral" <<  endl;
    }


    if (std::ifstream("ReOrder.dat"))
    {
           fcon << "reorder ReOrder.dat" << endl;
           std::cout << "Read orbital order from ReOrder.dat" << std::endl;
    }
    else
    {
           fcon << "gaopt default" << endl;
//           std::cout << "Use genetic algorithm for orbital reordering" << std::endl;
    }

    if(N_roots > 1) {
      fcon << "nroots " << N_roots << endl;
      fcon << "weights ";
      for(int i = 0; i < N_roots; ++i) fcon << 1.0/N_roots << " ";
      fcon << endl;
    }

//  fcon << "outputlevel 2" << endl;
    char* pMem;
    pMem = getenv ("MOLCAS_MEM");
    if (pMem!=NULL)
      fcon << "memory " << pMem << " m" << endl;
    else
      fcon << "memory 2000 m" << endl;

    char* pONT;
    pONT = getenv ("OMP_NUM_THREADS");
    if (pONT!=NULL)
      fcon << "num_thrds " << pONT << endl;
    else
      fcon << "num_thrds 1" << endl;

    fcon.close();
  }

  if(mpigetrank() == 0) {
    boost::filesystem::path path_to_reorder("./RestartReorder.dat");
    if(Restart == 0 && boost::filesystem::exists(path_to_reorder))
      boost::filesystem::remove(path_to_reorder);
  }

  std::streambuf *backup;
  backup = std::cout.rdbuf();
  std::ofstream fout;

  fout.open(output);
  std::cout.rdbuf(fout.rdbuf());

  calldmrg(input,0);
//calldmrg(input,output);

  SpinAdapted::SweepParams param;
  bool forward; int size;
  param.restorestate(forward, size);

//assert(param.get_lowest_energy().size() == N_roots);
  for(int i = 0; i < N_roots; ++i)
    E_sweep[i] = param.get_lowest_energy()[i];

  // Sorting NPDMs as Chemist's order
  for(int i = 0; i < N_roots; ++i) {
    switch (N_pdm) {
      case 1:
        sort1pdm(N_act,i,i);
        break;
      case 2:
        sort2pdm(N_act,i,i);
        break;
      case 3:
        sort3pdm(N_act,i,i);
        break;
      default:
        exit(1); // sorting RDMs are implemented up to 3RDM
    }
  }

  std::cout.rdbuf(backup);
  fout.close();
}
