#ifndef SWEEPGENBLOCK_HEADER
#define SWEEPGENBLOCK_HEADER

#include "global.h"
#include "solver.h"
#include "initblocks.h"
#include "rotationmat.h"
#ifndef SERIAL
#include <boost/mpi/communicator.hpp>
#include <boost/mpi.hpp>
#endif
using namespace boost;
using namespace std;

namespace SpinAdapted{
namespace SweepGenblock
{
  void BlockAndDecimate (SweepParams &sweepParams, SpinBlock& system, SpinBlock& newSystem, const bool &useSlater, const bool& dot_with_sys);
  double do_one(SweepParams &sweepParams, const bool &warmUp, const bool &forward, const bool &restart, const int &restartSize);
};
}

#endif
