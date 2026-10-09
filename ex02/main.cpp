/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:57:44 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/10/05 11:11:04 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	if (argc < 2)
	{
		std::cerr << "Error: no arguments given" << std::endl;
		return 1;
	}

	PmergeMe pmm;
	pmm.run(argc, argv);
	return pmm.succeeded() ? 0 : 1;
}
