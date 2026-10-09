/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:58:15 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/10/05 11:11:12 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <cstddef>
#include <vector>
#include <deque>
#include <iostream>

class PmergeMe
{
public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();

	void run(int argc, char **argv);
	bool succeeded() const;

private:
	bool _ok;

	static std::vector<int> parseInput(int argc, char **argv);
	static void printSequence(const char *label, const std::vector<int> &seq);
};

#endif
