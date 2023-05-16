#include "gsl/gsl_spmatrix.h"
#include "gsl/gsl_spblas.h"

void set_gsl_float_data(gsl_vector_float * vec, float * data){ 
    vec->data = data;
 }
  void set_gsl_mat_data_row_major_s(gsl_matrix_float * mat, float * data){
    mat->data = data;
 } 
  void set_tensor_data_s(tensor_float * t, float * data){ 
    t->data = data;
 } 
  void print_array(float * vals, int num_elem){ 
     for (int i = 0; i < num_elem; i++){ 
       printf("elem[%d]=%f\\n", i, vals[i]); 
     } 
     printf("next\\n"); 
 } 


void print_array_double(double * vals, int num_elem){ 
     for (int i = 0; i < num_elem; i++){ 
       printf("elem[%d=]=%f\n", i, vals[i]); 
     } 
     printf("next\\n"); 
 } 


void print_array_int(int * vals, int num_elem){ 
     for (int i = 0; i < num_elem; i++){ 
       printf("elem[%d=]=%d\n", i, vals[i]); 
     } 
     printf("next\\n"); 
 } 


 void gsl_spmv(taco_tensor_t * A, int m, int n, double * b, double * c){

  int*  A_pos = (int*)(A->indices[1][0]);
  
  gsl_spmatrix * Sp_A = gsl_spmatrix_alloc_nzmax(m, n, A_pos[m], GSL_SPMATRIX_CSR);

  Sp_A->i = (int*)(A->indices[1][1]);
  Sp_A->p = A_pos;
  Sp_A->data = (double*)A->vals;
  Sp_A->nz = A_pos[m];

  gsl_vector* vec = gsl_vector_calloc(n);
  vec->data = b;

  gsl_vector * result = gsl_vector_calloc(m);
  result->data = c;

  gsl_spblas_dgemv(CblasNoTrans, 1, Sp_A, vec, 0, result);

 }