//
//  EquationSolver.cpp
//  EquationEvaluator
//
//  Created by Sarat Vemulapalli on 8/14/17.
//  Copyright © 2017 Sarat Vemulapalli. All rights reserved.
//

#include <iostream>
#include "EquationSolver.hpp"

void EquationSolver::InitMatrix()
{
    //Add comment why N x N + 1 is taken.
    matrix_ = new long*[number_of_variables_];
    for(int i = 0; i < number_of_variables_; ++i)
        matrix_[i] = new long[number_of_variables_ + 1];
    for(int i = 0; i < number_of_variables_; ++i)
        for(int j = 0; j < number_of_variables_ + 1; ++j)
            matrix_[i][j] = 0;
    
    solution_ = new double[number_of_variables_];
}

EquationSolver::~EquationSolver()
{
    for(int i = 0; i < number_of_variables_; ++i)
        delete [] matrix_[i];
    delete [] matrix_;
}

void EquationSolver::GaussianElimination()
{
    int singular_flag = ForwardElimination();
    
    /* if matrix is singular */
    if (singular_flag != -1)
    {
        /* Singular Matrix */
        /* if the RHS of equation corresponding to
         zero row  is 0, * system has infinitely
         many solutions, else inconsistent*/
        if (matrix_[singular_flag][number_of_variables_])
            no_solution_ = true;
        else {
            no_solution_ = true;
            infinite_solutions_ = true;
        }
        no_solution_ = true;
        return;
    }
    
    /* finding the solution using backward substitution */
    BackSub();
}

void EquationSolver::SwapRow(int row_one, int row_two)
{
    //printf("Swapped rows %d and %d\n", i, j);
    
    for (int k=0; k<=number_of_variables_; k++)
    {
        double temp = matrix_[row_one][k];
        matrix_[row_one][k] = matrix_[row_two][k];
        matrix_[row_two][k] = temp;
    }
}

void EquationSolver::Print()
{
    for (int i=0; i<number_of_variables_; i++, std::cout << std::endl)
        for (int j=0; j<=number_of_variables_; j++)
            std::cout << matrix_[i][j] << " ";
    
    std::cout << std::endl;
}

int EquationSolver::ForwardElimination()
{
    for (int k=0; k<number_of_variables_; k++)
    {
        /* Initialize maximum value and index for pivot */
        long i_max = k;
        long v_max = matrix_[i_max][k];
        
        /* find greater amplitude for pivot if any */
        for (int i = k+1; i < number_of_variables_; i++)
            if (std::abs(matrix_[i][k]) > v_max)
                v_max = matrix_[i][k], i_max = i;
        
        /* if a prinicipal diagonal element  is zero,
         * it denotes that matrix is singular, and
         * will lead to a division-by-zero later. */
        if (!matrix_[k][i_max])
            return k; // Matrix is singular
        
        /* Swap the greatest value row with current row */
        if (i_max != k)
            SwapRow(k, (int)i_max);
        
        for (int i=k+1; i<number_of_variables_; i++)
        {
            /* factor f to set current row kth elemnt to 0,
             * and subsequently remaining kth column to 0 */
            double f = matrix_[i][k]/matrix_[k][k];
            
            /* subtract fth multiple of corresponding kth
             row element*/
            for (int j=k+1; j<=number_of_variables_; j++)
                matrix_[i][j] -= matrix_[k][j]*f;
            
            /* filling lower triangular matrix with zeros*/
            matrix_[i][k] = 0;
        }
    }
    return -1;
}

void EquationSolver::BackSub()
{
    /* Calulating from last equation to the first */
    for (int i = number_of_variables_-1; i >= 0; i--)
    {
        /* starting with the RHS of the equation */
        solution_[i] = matrix_[i][number_of_variables_];
        
        /* Initialize j to i+1 since matrix is upper
         triangular */
        for (int j=i+1; j<number_of_variables_; j++)
        {
            /* subtract all the lhs values
             * except the coefficient of the variable
             * whose value is being calculated */
            solution_[i] -= matrix_[i][j]*solution_[j];
        }
        
        /* divide the RHS by the coefficient of the
         unknown being calculated */
        solution_[i] = solution_[i]/matrix_[i][i];
    }
}

void EquationSolver::CopyColumn(int new_number_of_variables)
{
    int j = new_number_of_variables;
    for (int i = 0; i < number_of_variables_; i++) {
        matrix_[i][j] = matrix_[i][number_of_variables_];
    }
}

void EquationSolver::SolveOverDeterministic(int new_number_of_variables)
{
    CopyColumn(new_number_of_variables);
    number_of_variables_ = new_number_of_variables;
    BackSub();
}
