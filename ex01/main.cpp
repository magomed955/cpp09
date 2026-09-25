/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:56:16 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/09/07 17:45:51 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"
#include <iostream>

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        std::cerr << "Error" << std::endl;
        return (1);
    }

    RPN rpn;

    try
    {
        int result = rpn.solve(argv[1]);
        std::cout << result << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "Error" << std::endl;
        return (1);
    }

    return (0);
}