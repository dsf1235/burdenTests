//
//  skat.cpp
//  burdenTest
//
//  Created by Corin Thummel on 7/30/18.
//  Copyright © 2018 Corin Thummel. All rights reserved.
//

#include "skat.hpp"
#include "davies.cpp"

using namespace std;

skat::skat(gsl_matrix* geno, gsl_vector* maf, gsl_matrix* covariates, gsl_vector* phenotype)
{
    //Initialize variables.
    subjectCount = (int)geno->size2;
    variantCount = (int)geno->size1;

    genoMatrix = geno;
    X = covariates;
    pheno = phenotype;
    weightMatrix = gsl_matrix_calloc(variantCount, variantCount);
    kernel = gsl_matrix_alloc(subjectCount, subjectCount);
    
    string kernel_type = "linear";
    
    //Run Test.
    setWeights(maf);
    makeKernel(kernel_type);
    setTestStatistic();
    
    
    //Cleanup after test.
    gsl_vector_free(pheno);
    gsl_matrix_free(X);
    gsl_matrix_free(weightMatrix);
    gsl_matrix_free(kernel);
    gsl_matrix_free(genoMatrix);
}

//Creates the mxm matrix of weights.
void skat::setWeights(gsl_vector *maf)
{
    for(int i = 0; i < maf->size; i++)
    {
        double tempWeight = gsl_ran_beta_pdf(gsl_vector_get(maf, i), 1, 25);
        gsl_matrix_set(weightMatrix, i, i, tempWeight * tempWeight);
    }
}

//Remember matrix multiplication goes right to left. So G'WG needs to calculate G'(WG) in a sense. (v = rows, s = columns)
//sxv vxv vxs -> sxv vxs -> sxs is kernal's final dimensions.
void skat::makeKernel(string kernel_type)
{
    if(kernel_type == "linear")
    {
        gsl_matrix *tempkernel = gsl_matrix_alloc(variantCount, subjectCount);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, weightMatrix, genoMatrix, 0, tempkernel);
        gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, genoMatrix, tempkernel, 0.0, kernel);
    }
    else if(kernel_type == "quad")
    {
        gsl_matrix *tempkernel = gsl_matrix_alloc(variantCount, subjectCount);
        gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1, weightMatrix, genoMatrix, 0, tempkernel);
        gsl_blas_dgemm(CblasTrans, CblasNoTrans, 1.0, genoMatrix, tempkernel, 0.0, kernel);
        gsl_matrix_add_constant(kernel, 1);
        //We squared the weights earlier so now they are ^4 is this correct?
        gsl_matrix_mul_elements(kernel, kernel);
    }
    else if (kernel_type == "IBS")
    {
        
    }
    
}

void skat::setTestStatistic()
{
    //Q = (y - u) * K * (y - u)'
    // 1xn nxn nx1 -> 1xn nx1 -> 1x1 = Q
    gsl_vector *pheno = gsl_vector_alloc(subjectCount);
    gsl_vector *tempstat = gsl_vector_alloc(subjectCount);
    gsl_blas_dgemv(CblasNoTrans, 1.0, weightMatrix, pheno, 0.0, tempstat);
    gsl_blas_ddot(pheno, tempstat, &testStatistic);


    gsl_matrix *P0 = gsl_matrix_alloc(subjectCount, subjectCount);
    gsl_matrix *V = gsl_matrix_calloc(subjectCount, subjectCount);
    gsl_matrix *Xtemp;

    //X is an n x (m + 1) matrix
    if(X_Count == 0)
    {
        //No covariates makes X and V formulation easy.
        Xtemp = gsl_matrix_alloc(subjectCount, 1);
        gsl_matrix_set_all(Xtemp,1);

        //P0 = ;
        double Vsum = 0;
        for(int i = 0; i < subjectCount; i++)
        {
            Vsum += gsl_matrix_get(V, i, i);
        }

        gsl_matrix *temp_V = gsl_matrix_alloc(subjectCount, subjectCount);
        gsl_blas_dgemm(CblasNoTrans, CblasTrans, 1, V, V, 0.0, temp_V);
        gsl_matrix_scale(temp_V, 1.0 / Vsum);
        gsl_matrix_memcpy(P0, V);
        gsl_matrix_sub(P0, temp_V);

    }
    else
    {
        //Xtemp is [1, X]
        Xtemp = gsl_matrix_alloc(subjectCount, X_Count + 1);
        for(int i = 0; i < subjectCount; i++)
        {
            gsl_matrix_set(Xtemp, i, 0, 1);
        }
        for(int j = 0; j < X_Count + 1; j++)
        {
            gsl_vector *tempCol = gsl_vector_alloc(subjectCount);
            gsl_matrix_get_col(tempCol, X, j);
            gsl_matrix_set_col(Xtemp, j+1, tempCol);
        }

        //Building P0

    }
    
    //V in continuous phenotype case:
    //V = sigma0^2 I where sigma0 is the estimator of sigma under null hypothesis.
    double sigma0;
    for(int i = 0; i < subjectCount; i++)
    {
        gsl_matrix_set(V, i, i, sigma0);
    }

    //V in dichotomous phenotype case:
    //V = diag(uhat_01*(1-uhat_01), ... ,uhat_0n*(1-uhat_0n))
    //uhat_0i = logit^{-1} (alphahat + alphahat' X_i)
    //When there are no covariates, uhat is the identity matrix?
    gsl_vector *uhat = gsl_vector_alloc(subjectCount);
    for(int i = 0; i < subjectCount; i++)
    {
        gsl_matrix_set(V, i, i, gsl_vector_get(uhat, i));
    }
    
}

void skat::logisticRegression()
{
    if(X->size1 == 0)
    {
        //No covariates
    }
    else
    {

    }
}


void skat::linearRegression()
{


}









