/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:56:30 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/09/30 14:50:59 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <iostream>
#include <sstream>
#include <cstdlib>
#include <stdexcept>

RPN::RPN()
{
}

RPN::RPN(const RPN &src)
{
    *this = src;
}

RPN &RPN::operator=(const RPN &src)
{
    if (this != &src)
        _stack = src._stack;
    return (*this);
}

RPN::~RPN()
{
}

bool RPN::isNumber(const std::string &token) const
{
    return (token.size() == 1 && isdigit(token[0]));
}

bool RPN::isOperator(const std::string &token) const
{
    if (token.size() != 1)
        return false;
    if (token[0] != '+' && token[0] != '-' && token[0] != '*' && token[0] != '/')
        return false;
    return true; 
}

int RPN::applyOperator(int a, int b, char op) const
{
    if (op == '+')
        return (a + b);
    if (op == '-')
        return (a - b);
    if (op == '*')
        return (a * b);
    if (op == '/')
    {
        if (b == 0)
            throw std::runtime_error("division by zero");
        return (a / b);
    }
    throw std::runtime_error("unknown operator");
}

int RPN::solve(const std::string &expression)
{
    std::stringstream ss(expression);
    std::string token;

    while (ss >> token)
    {
        if (isNumber(token))
        {
            _stack.push(atoi(token.c_str()));
        }
        else if (isOperator(token))
        {
            if (_stack.size() < 2)
                throw std::runtime_error("Error");

            int b = _stack.top();
            _stack.pop();
            int a = _stack.top();
            _stack.pop();
 
            _stack.push(applyOperator(a, b, token[0]));
        }
        else
        {
            throw std::runtime_error("Error");
        }
    }

    if (_stack.size() != 1)
        throw std::runtime_error("Error");

    return (_stack.top());
}

