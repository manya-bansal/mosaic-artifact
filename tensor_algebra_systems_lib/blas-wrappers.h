#include "blas.h"

void ublas_spmv(taco_tensor_t * A, double *b, double* c){

  int A1_dimension = (int)(A->dimensions[0]);
  int A2_dimension = (int)(A->dimensions[1]);

  int*  A2_pos = (int*)(A->indices[1][0]);
  int*  A2_crd = (int*)(A->indices[1][1]);
  double*  A_vals = (double*)(A->vals);

  blas_sparse_matrix Sp_A =  BLAS_duscr_begin(A1_dimension, A2_dimension);

  for (int32_t i = 0; i < A1_dimension; i++) { 
    for (int32_t jA = A2_pos[i]; jA < A2_pos[(i + 1)]; jA++) {
      int32_t j = A2_crd[jA];
      BLAS_duscr_insert_entry(Sp_A, A_vals[jA], i, j);
    }
  }

  for (int32_t i = 0; i < A1_dimension; i++) { 
    c[i] = 0;
  }

  BLAS_duscr_end(Sp_A);

  BLAS_dusmv(blas_no_trans, 1.0, Sp_A, b, 1, c, 1);
}