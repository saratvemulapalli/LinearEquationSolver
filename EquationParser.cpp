//
//  EquationParser.cpp
//  EquationEvaluator
//
//  Created by Sarat Vemulapalli on 8/14/17.
//  Copyright © 2017 Sarat Vemulapalli. All rights reserved.
//

#include "EquationParser.hpp"

namespace {
    /* Parser States */
    enum ParserState
    {
        PARSE_TERM,
        PARSE_OPERATOR
    };
};

void EquationParser::Reset()
{
    equal_sign_in_equation_flag_ = false;
    at_least_one_var_in_equation_flag_ = false;
    term_before_equal_sign_exists_flag_ = false;
    term_after_equal_sign_exists_flag_ = false;
    equation_index_ = 0;
    
    return;
}

void EquationParser::ResetForNewEquation()
{
    equal_sign_in_equation_flag_ = false;
    at_least_one_var_in_equation_flag_ = false;
    term_before_equal_sign_exists_flag_ = false;
    term_after_equal_sign_exists_flag_ = false;
    equation_index_++;

    return;
}

bool EquationParser::GetNumber(const std::string & input_line_string,
                               int & position,
                               std::string & number_string)
{
    unsigned int digit_length = 0;
    bool have_number_flag = false;
    bool continue_flag = position < (int)(input_line_string.length());
    
    while (continue_flag)
    {
        char c = input_line_string[position];
        
        if (isdigit(c))
        {
            have_number_flag = true;
            number_string += c;
            ++position;
            ++digit_length;
            continue_flag = position < (int)(input_line_string.length());
        }
        else {
            break;
        }
    }
    
    if (digit_length > 6) {
        parser_state_ = EquationParser::ERROR;
    }
    return have_number_flag;
}

bool EquationParser::GetSign(const std::string & input_line_string,
                             int & position)
{
    bool have_sign_flag = false;
    
    if (position < (int)(input_line_string.length()))
    {
        char c = input_line_string[position];
        
        if (c == '+')
        {
            have_sign_flag = true;
            ++position;
        }
    }
    return have_sign_flag;
}

bool EquationParser::GetVariableName(const std::string & input_line_string,
                                     int & position,
                                     std::string & variable_name_string)
{
    bool have_variable_name_flag = false;
    bool continue_flag = position < (int)(input_line_string.length());
    
    while (continue_flag)
    {
        char c = input_line_string[position];
        
        continue_flag = isalpha(c);
        
        if (continue_flag)
        {
            have_variable_name_flag = true;
            variable_name_string += c;
            ++position;
            continue_flag = position < (int)(input_line_string.length());
        }
    }
    return have_variable_name_flag;
}

bool EquationParser::GetOperator(const std::string & input_line_string,
                                 int & position)
{
    SkipSpaces(input_line_string, position);
    
    bool have_equal_sign_flag = false;
    
    if (position < (int)(input_line_string.length()))
    {
        if (input_line_string[position] == '=')
        {
            have_equal_sign_flag = true;
            ++position;
        }
    }
    return have_equal_sign_flag;
}

void EquationParser::SkipSpaces(const std::string & input_line_string,
                                int & position)
{
    if (!(position < input_line_string.length())) {
        return;
    }
    while(position < input_line_string.length()) {
        char c = input_line_string[position];
        
        if (iswspace((int)(c)) != 0) {
            position++;
        }
        else {
            break;
        }
    }
}

EquationParser::status EquationParser::Parse(const std::string & input_line_string,
                                             VariableNameMap & variable_name_index_map,
                                             unsigned int & number_of_equations,
                                             EquationSolver & equation_solver)
{
    int position = 0;
    parser_state_ = EquationParser::SUCCESS;
    
    //Skipping spaces if any
    SkipSpaces(input_line_string, position);
    
    if (position >= input_line_string.length()) {
        std::cout << "Error : Not a valid input - Empty Line" << std::endl;
        //Handle EquationReset() or Reset()
        return EquationParser::ERROR;
    }
    
    ParserState state = PARSE_TERM;
    
    while(position < input_line_string.length()) {
        
        //Skipping spaces if any
        
        SkipSpaces(input_line_string, position);
        
        //Check if it a variable or a number or Sign or Equal to
        
        if (state == PARSE_TERM) {
            std::string variable_if_exists;
            bool is_variable = GetVariableName(input_line_string, position,
                                               variable_if_exists);
            std::string number_if_exists;
            bool is_number = GetNumber(input_line_string, position, number_if_exists);
        
            if (is_variable) {
                
                if (!at_least_one_var_in_equation_flag_) {
                    at_least_one_var_in_equation_flag_ = true;
                }
                
                if (!term_before_equal_sign_exists_flag_ && !equal_sign_in_equation_flag_) {
                    term_before_equal_sign_exists_flag_ = true;
                }
                
                if (!term_after_equal_sign_exists_flag_ && equal_sign_in_equation_flag_) {
                    term_after_equal_sign_exists_flag_ = true;
                }
                
                // Yes it is a variable, find/stash it into the map
                int value = 1;
                
                if (equal_sign_in_equation_flag_){
                    value = -1;
                }
                int variable_index = 0;
            
                VariableNameMap::iterator it = variable_name_index_map.find(variable_if_exists);
            
                if (it != variable_name_index_map.end()) {
                    // Already in Map
                    //std::cout << "Old Variable Encountered " << variable_if_exists << std::endl;
                    variable_index = (*it).second;
                }
                else {
                    //std::cout << "New Variable = " << variable_if_exists << std::endl;
                    variable_index = (int)variable_name_index_map.size();
                    variable_name_index_map.insert(std::pair<std::string,int>(variable_if_exists,
                                                                              variable_index));
                }
                equation_solver.matrix_[number_of_equations][variable_index] = equation_solver.matrix_[number_of_equations][variable_index] + value;
            } else if (is_number) {
                
                if (parser_state_ == EquationParser::ERROR) {
                    /* Out of range */
                    return EquationParser::ERROR;
                }
                
                if (!term_before_equal_sign_exists_flag_ && !equal_sign_in_equation_flag_) {
                    term_before_equal_sign_exists_flag_ = true;
                }
                
                if (!term_after_equal_sign_exists_flag_ && equal_sign_in_equation_flag_) {
                    term_after_equal_sign_exists_flag_ = true;
                }
                
                long value = -1; /* For value on RHS */
                
                if (equal_sign_in_equation_flag_){
                    /* Negate it if = sign was already encountered */
                    value = 1;
                }
                value = value * std::stol(number_if_exists);
                
                /* Insert into the matrix */
                equation_solver.matrix_[number_of_equations][equation_solver.number_of_variables_] = equation_solver.matrix_[number_of_equations][equation_solver.number_of_variables_] + value;
            } else {
                /* Error : Expected variable or number - But not found */
                return EquationParser::ERROR;
            }
            state = PARSE_OPERATOR;
        } else {
            bool is_equal_sign = GetOperator(input_line_string, position);
            
            bool is_math_sign = GetSign(input_line_string, position);
            
            if (is_equal_sign) {
                if (equal_sign_in_equation_flag_) {
                    /* Equal sign already in the equation - Wrong input */
                    return EquationParser::ERROR;
                } else {
                    equal_sign_in_equation_flag_ = true;
                }
            } else if (is_math_sign) {
                
            } else {
                /* Error : Expected = or math symbol - But not found */
                return EquationParser::ERROR;
            }
            state = PARSE_TERM;
        }
    }
    
    if ( equal_sign_in_equation_flag_
         && at_least_one_var_in_equation_flag_
         && term_before_equal_sign_exists_flag_
        && term_after_equal_sign_exists_flag_) {
        /* Resetting for new equation */
        ResetForNewEquation();
        number_of_equations++;
        
        return EquationParser::SUCCESS;
    } else {
        return EquationParser::ERROR;
    }
}
