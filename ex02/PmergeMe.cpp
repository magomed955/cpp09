/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/03 15:58:06 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/09/21 16:52:17 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"
#include <cstdlib>
#include <cctype>
#include <climits>
#include <cerrno>
#include <stdexcept>
#include <sys/time.h>

/*
** ------------------------------------------------------------------------
** Ford-Johnson (merge-insert) sort, implemented once as a template so it
** can be instantiated for std::vector<int> AND std::deque<int> (the two
** required containers). Both containers offer random access iterators,
** so the exact same logic/complexity applies to both -- which is exactly
** what we want to compare fairly with the timers below.
**
** Principle:
**   1. Split the sequence into pairs, keep the larger of each pair in
**      "bigs" and the smaller in "smalls" (aligned by index).
**   2. Recursively Ford-Johnson-sort "bigs" -> this becomes the sorted
**      main chain.
**   3. Insert every element of "smalls" into that chain with a binary
**      search, in the classic Jacobsthal order, each one bounded by the
**      current position of its own pair partner (so early inserts never
**      need to search further than necessary).
**   4. If the input had an odd length, insert the leftover element last.
**
** Every leaf value carries a unique "id" (its original index) so we can
** always find, after a recursive sort, where a given pair's "big" ended
** up inside the freshly sorted chain -- values alone are not reliable
** because duplicates are allowed.
** ------------------------------------------------------------------------
*/

namespace
{
	// Returns the order (as 0-indexed ranks) in which the "smalls" should
	// be inserted into the sorted "bigs" chain, following the Jacobsthal
	// numbers (3, 5, 11, 21, 43, ...) that Ford-Johnson relies on to keep
	// every binary search as short as possible.
	std::vector<size_t> jacobsthalInsertOrder(size_t m)
	{
		std::vector<size_t> order;
		if (m == 0)
			return order;
		order.push_back(0); // rank 1 (the partner of the smallest "big") is always first
		if (m == 1)
			return order;

		std::vector<size_t> jac;
		jac.push_back(3);
		jac.push_back(5);
		while (jac.back() < m)
			jac.push_back(jac[jac.size() - 1] + 2 * jac[jac.size() - 2]);

		size_t prev = 1;
		for (size_t i = 0; i < jac.size() && prev < m; ++i)
		{
			size_t bound = jac[i] < m ? jac[i] : m;
			for (size_t r = bound; r > prev; --r)
				order.push_back(r - 1);
			prev = bound;
		}
		return order;
	}

	template <typename Container>
	void mergeInsertSort(const std::vector<int> &values,
						  const std::vector<long> &ids,
						  Container &outValues,
						  std::vector<long> &outIds)
	{
		size_t n = values.size();

		if (n <= 1)
		{
			outValues.assign(values.begin(), values.end());
			outIds = ids;
			return;
		}

		bool hasOdd = (n % 2 == 1);
		int oddValue = 0;
		long oddId = -1;
		if (hasOdd)
		{
			oddValue = values.back();
			oddId = ids.back();
		}
		size_t pairCount = n / 2;

		std::vector<int> bigsVal(pairCount), smallsVal(pairCount);
		std::vector<long> bigsId(pairCount), smallsId(pairCount);
		for (size_t i = 0; i < pairCount; ++i)
		{
			int a = values[2 * i], b = values[2 * i + 1];
			long aId = ids[2 * i], bId = ids[2 * i + 1];
			if (a > b)
			{
				bigsVal[i] = a;   bigsId[i] = aId;
				smallsVal[i] = b; smallsId[i] = bId;
			}
			else
			{
				bigsVal[i] = b;   bigsId[i] = bId;
				smallsVal[i] = a; smallsId[i] = aId;
			}
		}

		// 1. Recursively sort the "bigs" -> this is our main chain.
		Container chain;
		std::vector<long> chainIds;
		mergeInsertSort(bigsVal, bigsId, chain, chainIds);

		// 2. Work out, for every pair, the rank (0 = smallest) that its
		//    "big" ended up at inside the freshly sorted chain.
		std::vector<size_t> idToPairIndex(pairCount);
		for (size_t i = 0; i < pairCount; ++i)
			idToPairIndex[i] = i; // placeholder, filled properly below via search
		// map big-id -> pairIndex
		// (linear scan is fine: total cost stays well within budget for 3000 ints)
		std::vector<size_t> insertOrder = jacobsthalInsertOrder(pairCount);

		for (size_t k = 0; k < insertOrder.size(); ++k)
		{
			size_t pairIdx = insertOrder[k];
			int val = smallsVal[pairIdx];
			long id = smallsId[pairIdx];
			long bigId = bigsId[pairIdx];

			size_t bigPos = 0;
			for (; bigPos < chainIds.size(); ++bigPos)
				if (chainIds[bigPos] == bigId)
					break;

			size_t lo = 0, hi = bigPos;
			while (lo < hi)
			{
				size_t mid = lo + (hi - lo) / 2;
				if (chain[mid] < val)
					lo = mid + 1;
				else
					hi = mid;
			}
			chain.insert(chain.begin() + lo, val);
			chainIds.insert(chainIds.begin() + lo, id);
		}

		// 3. Insert the leftover element (odd length input), full range.
		if (hasOdd)
		{
			size_t lo = 0, hi = chain.size();
			while (lo < hi)
			{
				size_t mid = lo + (hi - lo) / 2;
				if (chain[mid] < oddValue)
					lo = mid + 1;
				else
					hi = mid;
			}
			chain.insert(chain.begin() + lo, oddValue);
			chainIds.insert(chainIds.begin() + lo, oddId);
		}

		outValues = chain;
		outIds = chainIds;
	}

	double nowMicroseconds()
	{
		struct timeval tv;
		gettimeofday(&tv, NULL);
		return static_cast<double>(tv.tv_sec) * 1000000.0 + static_cast<double>(tv.tv_usec);
	}
}

PmergeMe::PmergeMe() : _ok(true) {}
PmergeMe::PmergeMe(const PmergeMe &other) { *this = other; }
PmergeMe &PmergeMe::operator=(const PmergeMe &other) { _ok = other._ok; return *this; }
PmergeMe::~PmergeMe() {}

bool PmergeMe::succeeded() const { return _ok; }

void PmergeMe::printSequence(const char *label, const std::vector<int> &seq)
{
	std::cout << label;
	for (size_t i = 0; i < seq.size(); ++i)
		std::cout << " " << seq[i];
	std::cout << std::endl;
}

std::vector<int> PmergeMe::parseInput(int argc, char **argv)
{
	std::vector<int> result;

	for (int i = 1; i < argc; ++i)
	{
		const char *s = argv[i];
		if (!*s)
			throw std::runtime_error("empty argument");

		size_t j = 0;
		if (s[j] == '+')
			++j;
		if (!s[j])
			throw std::runtime_error("invalid argument");
		for (; s[j]; ++j)
		{
			if (!std::isdigit(static_cast<unsigned char>(s[j])))
				throw std::runtime_error("non-digit character");
		}

		errno = 0;
		char *end = NULL;
		long val = std::strtol(s, &end, 10);
		if (errno == ERANGE || val > INT_MAX)
			throw std::runtime_error("number too large");
		if (val <= 0)
			throw std::runtime_error("only strictly positive integers are allowed");

		result.push_back(static_cast<int>(val));
	}
	return result;
}

void PmergeMe::run(int argc, char **argv)
{
	std::vector<int> input;
	try
	{
		input = parseInput(argc, argv);
		if (input.empty())
			throw std::runtime_error("no numbers given");
	}
	catch (const std::exception &e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		_ok = false;
		return;
	}

	printSequence("Before:", input);

	std::vector<long> ids(input.size());
	for (size_t i = 0; i < input.size(); ++i)
		ids[i] = static_cast<long>(i);

	// --- std::vector run ---
	double t0 = nowMicroseconds();
	std::vector<int> sortedVec;
	std::vector<long> idsVecOut;
	{
		std::vector<int> valuesCopy = input;
		std::vector<long> idsCopy = ids;
		mergeInsertSort(valuesCopy, idsCopy, sortedVec, idsVecOut);
	}
	double t1 = nowMicroseconds();

	printSequence("After:", sortedVec);

	// --- std::deque run ---
	double t2 = nowMicroseconds();
	std::deque<int> sortedDeq;
	std::vector<long> idsDeqOut;
	{
		std::vector<int> valuesCopy = input;
		std::vector<long> idsCopy = ids;
		mergeInsertSort(valuesCopy, idsCopy, sortedDeq, idsDeqOut);
	}
	double t3 = nowMicroseconds();

	std::cout << "Time to process a range of " << input.size()
			  << " elements with std::vector : " << (t1 - t0) << " us" << std::endl;
	std::cout << "Time to process a range of " << input.size()
			  << " elements with std::deque : " << (t3 - t2) << " us" << std::endl;
}
