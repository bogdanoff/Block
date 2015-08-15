#ifndef SPIN_STACKMATRIX_HEADER
#define SPIN_STACKMATRIX_HEADER

#include <boost/serialization/map.hpp>
#include <boost/serialization/shared_ptr.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include "alloc.h"

using namespace std;

namespace SpinAdapted{

  //very simple Matrix class that provides a Matrix type interface for a double array. it does not own its own data. 
class StackMatrix 
{
 protected:
  int ncols;
  int nrows;
  double* data; //this data is not owned by StackMatrix

  //we cannot serialize the pointer because it does not belong to the class
  friend class boost::serialization::access;
  template<class Archive> void serialize(Archive & ar, const unsigned int version)
  {
    ar & ncols & nrows;
  }

 public:
  StackMatrix() : data(0), nrows(0), ncols(0) {};
  StackMatrix(double* pData, int pnrows, int pncols) : data(pData), nrows(pnrows), ncols(pncols) {};
  StackMatrix(const StackMatrix& sm) : data(sm.Store()), nrows(sm.Nrows()), ncols(sm.Ncols()) {};
  void allocate(double* pData, int pnrows, int pncols) {data=pData; nrows=pnrows; ncols=pncols;}
  double* Store() const {return data;}
  int Storage() const {return ncols*nrows;}
  int Ncols() const {return ncols;}
  int Nrows() const {return nrows;}
  double& operator()(int i, int j) { return data[(i-1)*ncols + j-1];}  //numbering starts from 1,1
  const double& operator()(int i, int j) const { return data[(i-1)*ncols + j-1];} //numbering starts from 1,1

  friend ostream& operator<< (ostream& os, const StackMatrix& a) {
    for (int i=0; i<a.Nrows(); i++) {
      for (int j=0; j<a.Ncols(); j++)
	os << a(i+1,j+1)<<"  ";
      os<<endl;
    }
    return os;
  }
};

};

#endif