/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:58:15 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/09/17 17:31:38 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PMERGEME_HPP
#define PMERGEME_HPP

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

		template <typename T>
		static void printSequence(const char *label, const T &seq)
		{
			std::cout << label;
			typename T::const_iterator it = seq.begin();
			for (; it != seq.end(); ++it)
				std::cout << " " << *it;
			std::cout << std::endl;
		}
};

#endif
