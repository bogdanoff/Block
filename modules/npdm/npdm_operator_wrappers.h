/*                                                                           
Developed by Sandeep Sharma and Garnet K.-L. Chan, 2012                      
Copyright (c) 2012, Garnet K.-L. Chan                                        
                                                                             
This program is integrated in Molpro with the permission of 
Sandeep Sharma and Garnet K.-L. Chan
*/

#ifndef NPDM_OP_WRAPPERS_H
#define NPDM_OP_WRAPPERS_H

#include "operatorfunctions.h"
#include "npdm_patterns.h"

namespace SpinAdapted{

//===========================================================================================================================================================
//  BASE CLASS
//===========================================================================================================================================================
//FIXME constructors / destructors
class NpdmSpinOps {

  public:
//FIXME filename for disk-based storage
    int size() { return size_; }
    virtual bool set_local_ops( int idx ) { assert(false); }
    // Input file stream for disk-based operators used to build NPDM
    std::ifstream ifs;

    // Numerical representation of the operators for several total spins (e.g. 2-index op has two forms with spin-1/2 particles)
    std::vector< boost::shared_ptr<SparseMatrix> > opReps_;
    // Spin multiplicity of each operator (this info should be in each OpReps element, but we can use this for diagnostics)
    std::vector<int> mults_;
    // How the operator is built (e.g. 3-index from product of 2-index cre-cre and 1-index destruction, otherwise)
    std::string build_pattern_;
    // Do we need to transpose the representation before using it?
    bool transpose_;
    // Do we need to multiply by any constant factors when using it (due to implicit use of commutation relations or such like)?
    double factor_;
    // Effective spatial orbital indices (since due to use of transposition / commutation may not match OpRep.get_orbs() etc)
    std::vector<int> indices_;

  protected:
    boost::shared_ptr<SparseMatrix> build_compound_operator( bool is_fermion, int sign,
                                                             boost::shared_ptr<SparseMatrix> lhsOp,
                                                             boost::shared_ptr<SparseMatrix> rhsOp,
                                                             int ispin, std::vector<int> indices, bool transpose );

    SpinBlock * spinBlock_;
    // Number of spatial orbital combinations
    int size_;
};

//===========================================================================================================================================================
//  4-INDEX compound Ops (build using RI approximation, exact on dot block)
//===========================================================================================================================================================

class Npdm_op_wrapper_compound_CCDD : public NpdmSpinOps {
  public:
    Npdm_op_wrapper_compound_CCDD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================
//  4-INDEX Ops
//===========================================================================================================================================================

class Npdm_op_wrapper_CCDD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CCDD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================
//  3-INDEX compound Ops (build using RI approximation, exact on dot block)
//===========================================================================================================================================================

class Npdm_op_wrapper_compound_CCD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_compound_CCD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_compound_CDD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_compound_CDD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_compound_CDC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_compound_CDC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_compound_CCC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_compound_CCC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_compound_DCD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_compound_DCD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================
//  3-INDEX Ops
//===========================================================================================================================================================

class Npdm_op_wrapper_CCC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CCC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_CCD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CCD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_CDD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CDD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_CDC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CDC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_DCD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_DCD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_DDC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_DDC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_DCC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_DCC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_DDD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_DDD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================
//  2-INDEX Ops
//===========================================================================================================================================================

class Npdm_op_wrapper_CC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_CD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_CD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_DC : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_DC( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_DD : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_DD( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================
//  1-INDEX Ops
//===========================================================================================================================================================

class Npdm_op_wrapper_C : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_C( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

class Npdm_op_wrapper_D : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_D( SpinBlock * spinBlock );
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================
//  NULL case (for empty creation-destruction patterns)
//===========================================================================================================================================================

class Npdm_op_wrapper_NULL : public NpdmSpinOps {
  public: 
    Npdm_op_wrapper_NULL();
    bool set_local_ops( int idx );
};

//===========================================================================================================================================================

}

#endif
 