#ifndef MKL_WRAPPERS_H
#define MKL_WRAPPERS_H

#include "mkl.h"

void sgemm_mkl_internal(int m, int n, int k,  float * A_vals, float * b_vals, float * d_vals){

    MKL_INT dimm = m;
    MKL_INT dimn = n;
    MKL_INT dimk = k;
    float alpha = 1;
    float beta = 0;

    sgemm("n", "n", &dimm, &dimn, &dimk, &alpha, A_vals, &dimm, b_vals, &dimk, &beta, d_vals, &dimm);
}

void ssymv_mkl_internal(int dim, float * A_vals, float * b_vals, float * d_vals){

    MKL_INT dimA = dim;
    MKL_INT inc = 1;
    float alpha = 1;
    float beta = 0;

    ssymv("u", &dimA, &alpha, A_vals, &dimA, b_vals, &inc, &beta, d_vals, &inc);
}

float sdot_mkl_internal(int dim, float * A_vals, float * b_vals){

    MKL_INT dimA = dim;
    MKL_INT inc = 1;

    return sdot(&dimA, A_vals, &inc, b_vals, &inc);
}


void mkl_sparse_s_add_internal(int m, taco_tensor_t * A, taco_tensor_t * B, taco_tensor_t * C){

  sparse_matrix_t A_csr;
  int*  A_pos = (int*)(A->indices[1][0]);
  mkl_sparse_s_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, m, m, A_pos, A_pos+1, (int*)A->indices[1][1], (float*)A->vals);

  sparse_matrix_t B_csr;
  int*  B_pos = (int*)(B->indices[1][0]);
  mkl_sparse_s_create_csr(&B_csr, SPARSE_INDEX_BASE_ZERO, m, m, B_pos, B_pos+1, (int*)B->indices[1][1], (float*)B->vals);

 struct matrix_descr desc;
  desc.type = SPARSE_MATRIX_TYPE_GENERAL;

 sparse_matrix_t * C_csr;
  mkl_sparse_s_add(SPARSE_OPERATION_NON_TRANSPOSE, A_csr, (float) 1, B_csr, C_csr);
}

taco_tensor_t * convert_coo_to_csr(taco_tensor_t *COO) {
    taco_mode_t mode_types[] = {taco_mode_dense, taco_mode_sparse};
    taco_tensor_t *CSR = init_taco_tensor_t(COO->order, COO->csize, 
    COO->dimensions, COO->mode_ordering, mode_types);

    int*  COO_pos = (int*)(COO->indices[0][0]);
    int32_t start = COO_pos[0];
    int32_t end = COO_pos[1];

    int CSR_dimension = (int)(COO->dimensions[0]);
    CSR->indices[1][0] = malloc(sizeof(int) * (COO->dimensions[0] + 1));
    CSR->indices[1][1] = COO->indices[1][1];
    CSR->vals = COO->vals;
    int* CSR_pos = (int*)CSR->indices[1][0];

    for (int i = 0; i < CSR_dimension + 1; i++){
        CSR_pos[i] = 0;
    }

    int*  COO1_crd = (int*)(COO->indices[0][1]);
    int*  COO2_crd = (int*)(COO->indices[1][1]);

    while (start < end) {
        CSR_pos[COO1_crd[start] + 1]++;
        start++;
    }

    for (int i = 0; i < CSR_dimension; i++){
        CSR_pos[i+1] += CSR_pos[i];
    }

    return CSR;
}

void mkl_sparse_s_mm_internal(int m, taco_tensor_t * A, taco_tensor_t * b, float * c)
{ sparse_matrix_t A_csr;
 struct matrix_descr desc;
 desc.type = SPARSE_MATRIX_TYPE_GENERAL;
 int*  A_pos = (int*)(A->indices[1][0]); 
 mkl_sparse_s_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, m, m, A_pos, A_pos+1, (int*)A->indices[1][1], (float*)A->vals);
 mkl_sparse_s_mm(SPARSE_OPERATION_NON_TRANSPOSE, (float)1, A_csr, desc, SPARSE_LAYOUT_ROW_MAJOR, (float*)b->vals, m, m, 0, c, m);
}

void wrapper_convert(int m, taco_tensor_t * A, taco_tensor_t * B, float * C){
    taco_tensor_t *CSR_A = convert_coo_to_csr(A);
     printf("out1\n");
    mkl_sparse_s_mm_internal(m, CSR_A, B, C);
    printf("out\n");
    free(CSR_A->indices[1][0]);
}

void mkl_scsrgemv_internal(int m, taco_tensor_t * A, taco_tensor_t * b, taco_tensor_t * c){
    sparse_matrix_t A_csr;
    struct matrix_descr desc;
    desc.type = SPARSE_MATRIX_TYPE_GENERAL;
    int*  A_pos = (int*)(A->indices[1][0]);
    mkl_sparse_s_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, m, m, A_pos, A_pos+1, (int*)A->indices[1][1], (float*)A->vals);
    mkl_sparse_s_mv(SPARSE_OPERATION_NON_TRANSPOSE, (float)1, A_csr, desc, (float*)b->vals, (float)0, (float*)c->vals);
}

void mkl_dcsrgemv_internal(int m, taco_tensor_t * A, taco_tensor_t * b, taco_tensor_t * c){
    sparse_matrix_t A_csr;
    struct matrix_descr desc;
    desc.type = SPARSE_MATRIX_TYPE_GENERAL;
    int*  A_pos = (int*)(A->indices[1][0]);
    mkl_sparse_d_create_csr(&A_csr, SPARSE_INDEX_BASE_ZERO, m, m, A_pos, A_pos+1, (int*)A->indices[1][1], (double*)A->vals);
    mkl_sparse_d_mv(SPARSE_OPERATION_NON_TRANSPOSE, (float)1, A_csr, desc, (double*)b->vals, (float)0, (double*)c->vals);
}

#endif