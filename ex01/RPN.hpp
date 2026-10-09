/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:56:38 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/10/05 11:31:27 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <stack>
#include <string>

class RPN
{
    private:
        std::stack<int, std::list<int> > _stack;

        bool isOperator(const std::string &token) const;
        bool isNumber(const std::string &token) const;
        int  applyOperator(int a, int b, char op) const;

    public:
        RPN();
        RPN(const RPN &src);
        RPN &operator=(const RPN &src);
        ~RPN();

        int solve(const std::string &expression);
};

#endif
