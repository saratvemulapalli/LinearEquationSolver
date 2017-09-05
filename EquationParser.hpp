//
//  EquationParser.hpp
//  EquationEvaluator
//
//  Created by Sarat Vemulapalli on 8/14/17.
//  Copyright © 2017 Sarat Vemulapalli. All rights reserved.
//

#ifndef EquationParser_hpp
#define EquationParser_hpp

#include <stdio.h>
#include <iostream>
#include <map>
#include <string>
#include "EquationSolver.hpp"


class EquationParser
{
public:
    typedef std::map<std::string, int> VariableNameMap;
    
    enum status {
        SUCCESS,
        ERROR
    };
    
    
    /* @brief : Constructor.
     * @input : None
     * @output: None
     */
    EquationParser() { Reset(); };
    
    /* @brief : Destructor.
     * @input : None
     * @output: None
     */
    ~EquationParser() {};
    
    /* @brief : Member Function - Parse :  Function to parse the input equation
     *          and put data in matrix for solving.
     * @input : None
     * @output: status - EquationParser::SUCCESS for successful parsing.
     *                 - EquationParser::ERROR for failure in parsing.
     */
    status Parse(const std::string & input_line_string,
                 VariableNameMap & variable_name_index_map,
                 unsigned int & number_of_equations,
                 EquationSolver & equation_solver);
    
protected:
    
    bool equal_sign_in_equation_flag_;
    bool at_least_one_var_in_equation_flag_;
    bool term_before_equal_sign_exists_flag_;
    bool term_after_equal_sign_exists_flag_;
    status parser_state_;
    int equation_index_;
    
    /* @brief : Member Function - SkipSpaces :  Function to skip spaces.
     * @input : std::string & input_line_string - Reference of input line from the file.
     *          int & position - Reference of position where parser is at.
     * @output: None
     */
    void SkipSpaces(const std::string & input_line_string,
                    int & position);
    
    /* @brief : Member Function - ResetForNewEquation :  Function to reset state machine
     *          to parse another equation.
     * @input : None
     * @output: None
     */
    void ResetForNewEquation();
    
    /* @brief : Member Function - Reset :  Function to reset state machine
     *          to parse another set of new equations.
     * @input : None
     * @output: None
     */
    void Reset();
    
    /* @brief : Member Function - GetNumber :  Function to get number
     *          if exists.
     * @input : std::string & input_line_string - Reference of input line from the file.
     *          int & position - Reference of position where parser is at.
     * @output: bool - true if number is found.
     *               - false if not found.
     */
    bool GetNumber(const std::string & input_line_string,
                   int & position,
                   std::string & number_string);
    
    /* @brief : Member Function - GetSign :  Function to get sign
     *          if exists.
     * @input : std::string & input_line_string - Reference of input line from the file.
     *          int & position - Reference of position where parser is at.
     * @output: bool - true if sign is found.
     *               - false if not found.
     */
    bool GetSign(const std::string & input_line_string,
                 int & position);
    
    /* @brief : Member Function - GetVariableName :  Function to get variable name
     *          if exists.
     * @input : std::string & input_line_string - Reference of input line from the file.
     *          int & position - Reference of position where parser is at.
     * @output: bool - true if variable is found.
     *               - false if not found.
     */
    bool GetVariableName(const std::string & input_line_string,
                         int & position,
                         std::string & variable_name_string);
    
    /* @brief : Member Function - GetOperator :  Function to get operator
     *          if exists.
     * @input : std::string & input_line_string - Reference of input line from the file.
     *          int & position - Reference of position where parser is at.
     * @output: bool - true if operator is found.
     *               - false if not found.
     */
    bool GetOperator(const std::string & input_line_string,
                     int & position);

};
#endif /* EquationParser_hpp */
