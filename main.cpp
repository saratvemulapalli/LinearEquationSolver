//
//  main.cpp
//  EquationEvaluator
//
//  Created by Sarat Vemulapalli on 8/14/17.
//  Copyright © 2017 Sarat Vemulapalli. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "EquationParser.hpp"
#include "EquationSolver.hpp"


void ReportError(std::vector<std::string> input_string);

int main(int argc, const char * argv[]) {
    
    if (argc <= 1) //number of arguments are less than required
    {
        std::cout << std::endl << "Linear Equations Solver";
        std::cout << std::endl;
        std::cout << std::endl << "This programs solves linear equations stored in a file";
        std::cout << std::endl;
        std::cout << std::endl << "To run:";
        std::cout << std::endl;
        std::cout << std::endl << "LinearEquationSolver -f filename.txt";
        std::cout << std::endl;
        return 0;
    }
    
    std::string program_mode;
    std::string input_file_name;
    
    program_mode = argv[1];
    
    
    switch(program_mode[1]) {
        case 'f' : //filemode
            break;
        case 'h' : std::cout << "To run: LinearEquationSolver -f filename.txt" << std::endl;
            return 0;
        default:
            std::cout << "Unknown parameter, please use: LinearEquationSolver -h for help" << std::endl;
            return 0;
    }
    
    if (argc < 3) {
        std::cout << "Insufficient arguments, Please try again!" << std::endl;
        return 0;
    }
    
    input_file_name = argv[2];
    
    if (input_file_name.empty())
    {
        /* File name is empty, cannot go further */
        std::cout << "No input file specified." << std::endl;
        return 0;
    }
    
    if (input_file_name.find('.') == -1)
    {
        /* If no .txt exists, adding it */
        input_file_name += ".txt";
    }
    
    std::ifstream input_file;
    input_file.open(input_file_name.c_str(), std::ios::in);
    
    if (input_file.fail())
    {
        /* Reading of file failed */
        std::cout << "File " << input_file_name << " not found." << std::endl;
        return 0;
    }
    
    std::vector<std::string> input_string;
    
    while (!input_file.eof())
    {
        std::string input_data_array;
        std::getline(input_file, input_data_array);
        
        if (input_data_array.empty()) {
            /* Line read is empty, skipping adding to vector input_string */
            continue;
        }
        input_string.push_back(input_data_array);
    }
    
    EquationParser::VariableNameMap variable_name_index_map;
    EquationParser equation_parser;
    unsigned int number_of_equations = 0;
    EquationParser::status status = EquationParser::SUCCESS;
    
    /*Assuming N lines would have N equations and N unkowns, hence initializing solver 
      with N x N + 1 Matrix. */
    EquationSolver equation_solver((int)input_string.size());
    
    for (auto input_line : input_string) {
        status = equation_parser.Parse(input_line, variable_name_index_map, number_of_equations, equation_solver);
        if (status != EquationParser::SUCCESS) {
            /* if any failure parsing the equation, dumping out the input */
            ReportError(input_string);
            return 0;
        }
    }
    
    if (number_of_equations < variable_name_index_map.size()) {
        /* Number of equations cannot be less than number of variables inorder to have a solution,
           hence dumping out the input */
        ReportError(input_string);
        return 0;
    }
    
    /* Using Gaussian Elimination to solve linear equations, as it is more efficient with scale */
    equation_solver.GaussianElimination();
    
    /* If no solution is found, it could be either inconsistent system or has infinite solutions */
    if (equation_solver.no_solution_) {
        /* If number of equations > number of unknowns it is overdeterministic, and this check try's
           to find a valid solution if exists, if not it dumps out the input */
        if (number_of_equations == variable_name_index_map.size() + 1 && equation_solver.infinite_solutions_) {
            equation_solver.SolveOverDeterministic((int)variable_name_index_map.size());
        } else {
            ReportError(input_string);
            return 0;
        }
    }
    
    for (auto variable : variable_name_index_map) {
        /* If a solution is found, iterate over the map and display the variable and it's
           corresponding values */
        std::cout << variable.first << " = " << equation_solver.solution_[variable.second] <<std::endl;
    }
    
    return 0;
}


void ReportError(std::vector<std::string> input_string)
{
    for ( auto input_line : input_string) {
        std::cout << input_line << std::endl;
    }
}
