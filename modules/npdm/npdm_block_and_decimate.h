/*                                                                           
Developed by Sandeep Sharma and Garnet K.-L. Chan, 2012                      
Copyright (c) 2012, Garnet K.-L. Chan                                        
                                                                             
This program is integrated in Molpro with the permission of 
Sandeep Sharma and Garnet K.-L. Chan
*/


#ifndef NPDM_BLOCK_AND_DECIMATE_HEADER
#define NPDM_BLOCK_AND_DECIMATE_HEADER
#include "global.h"
#include "solver.h"
#include "initblocks.h"
#include "rotationmat.h"
#include "davidson.h"
#include "linear.h"
#include "density.h"
#include "pario.h"
#include "guess_wavefunction.h"
#include "npdm_driver.h"

#ifndef SERIAL
#include <boost/mpi/communicator.hpp>
#include <boost/mpi.hpp>
#endif
using namespace boost;
using namespace std;

namespace SpinAdapted{
namespace Npdm{

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

void BlockAndDecimate( Npdm_driver& npdm_driver, SweepParams &sweepParams, SpinBlock& system, SpinBlock& newSystem, 
                       const bool &useSlater, const bool& dot_with_sys, int state);

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

}
}

#endif
