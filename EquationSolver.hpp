//
//  EquationSolver.hpp
//  EquationEvaluator
//
//  Created by Sarat Vemulapalli on 8/14/17.
//  Copyright © 2017 Sarat Vemulapalli. All rights reserved.
//

#ifndef EquationSolver_hpp
#define EquationSolver_hpp

#include <stdio.h>

class EquationSolver
{
public:

    long** matrix_;
    int number_of_variables_;
    double* solution_;
    bool no_solution_;
    bool infinite_solutions_;
    
    /* @brief : Constructor.
     * @input : number_of_variables - number of variables in the given set of equations.
     */
    EquationSolver(int number_of_variables) : number_of_variables_(number_of_variables),
                                              no_solution_(false), infinite_solutions_(false) {
        InitMatrix();
    };
    
    /* @brief : Destructor.
     */
    ~EquationSolver();
    
    /* @brief : Member Function - Print :  Function to print the matrix.
     * @input : None
     * @output: None
     */
    void Print();
    
    /* @brief : Member Function - GaussianElimination :  Function to execute Gaussian Elimination
     *          on the given Matrix.
     * @input : None
     * @output: None
     */
    void GaussianElimination();
    
    /* @brief : Member Function - SolveOverDeterministic :  Function to solve if the set of equations
     *          are over deterministc and has infinite solutions, find one possible solution.
     * @input : None
     * @output: None
     */
    void SolveOverDeterministic(int new_number_of_variables);
    
protected:
    
    /* @brief : Member Function - InitMatrix :  Function to allocate memory for matrix, solution array
                and initialize it.
     * @input : None
     * @output: None
     */
    void InitMatrix();
    
    /* @brief : Member Function - ForwardElimination :  Function to execute forward elimination
     *          on the given Matrix in the process of gaussian elimination.
     * @input : None
     * @output: int : returns singular flag i.e if the matrix is singular after forward elimination.
     */
    int ForwardElimination();
    
    
    
    /* @brief : Member Function - BackSub :  Function to execute backward substitution
     *          on the given Matrix in the process of gaussian elimination.
     * @input : None
     * @output: None
     */
    void BackSub();
    
    /* @brief : Member Function - SwapRow :  Function to swap rows of matrix.
     * @input : None
     * @output: None
     */
    void SwapRow(int row_one, int row_two);
    
    /* @brief : Member Function - CopyColumn :  Function to copy column
     *          on the given Matrix, used for over deterministic solving.
     * @input : None
     * @output: None
     */
    void CopyColumn(int new_number_of_variables);
    
};

#endif /* EquationSolver_hpp */
