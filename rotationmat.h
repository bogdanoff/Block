#ifndef SPIN_ROTATION_MAT_HEADER
#define SPIN_ROTATION_MAT_HEADER 
#include <vector>
#include "Operators.h"
#include "multiarray.h"

namespace SpinAdapted{

void SaveRotationMatrix (const std::vector<int>& sites, const std::vector<Matrix>& m1);
void LoadRotationMatrix (const std::vector<int>& sites, std::vector<Matrix>& m1);
void diagonalise_dm(SparseMatrix& tracedMatrix, SparseMatrix& transformMatrix, std::vector<DiagonalMatrix>& eigenMatrix);
void sort_weights(std::vector<DiagonalMatrix>& eigenMatrix, vector<pair<int, int> >& inorderwts, vector<vector<int> >& weightsbyquanta);
double assign_matrix_by_dm(std::vector<Matrix>& rotatematrix, std::vector<DiagonalMatrix>& eigenmatrix, SparseMatrix& transformmatrix, vector<pair<int, int> >& inorderwts, vector<vector<int> >& wtsbyquanta, int totalstatesbydm, int totalstatesbyquanta, int left_block_size, int right_block_size);
int get_total_states(const int &this_size, const int &other_size);
bool can_connect(int n, int spin, int right_block_size);
}
#endif
