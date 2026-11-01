/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:20:54 by mshershe          #+#    #+#             */
/*   Updated: 2026/11/01 19:10:02 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once 

#include <iostream>
#include <vector>
#include <deque>
#include <utility>
#include <algorithm>
#include <exception>
#include <string>
#include <cstdlib>
#include <climits>
#include <cerrno>

class PmergeMe
{
	private:
		std::vector<int> vector_seq;
		std::deque<int>  deque_seq;
		std::vector<std::pair<int, int> > vec_pairs;
		std::deque<std::pair<int, int> >  deque_pairs;
		bool has_straggler;
		int straggler;
		PmergeMe();
	
		void form_pairs_vec();
		std::vector<std::pair<int, int> > sortPairsVec(std::vector<std::pair<int, int> > pairs);
		std::vector<int> sortPairIdsVec(const std::vector<std::pair<int, int> >& pairs, const std::vector<int>& ids);
		std::vector<int> gen_jacobsthal_seq(int size);
		std::vector<int> genJacobsthalOrder(int size);
		std::vector<std::pair<int, int> > makePairings_v( const std::vector<std::pair<int, int> >& pairs, const std::vector<int>& ids);
		
		void form_pairs_deque();
		std::deque<std::pair<int, int> > sortPairsDeque( const std::deque<std::pair<int, int> >& pairs);
		std::deque<int> gen_jacobsthal_seq_deq(int size);
		std::deque<int> genJacobsthalOrder_deque(int size);
		std::deque<std::pair<int, int> > makePairings_d( const std::deque<std::pair<int, int> >& pairs, const std::deque<int>& ids);
		std::deque<int> sortPairIdsDeque(const std::deque<std::pair<int, int> >& pairs, const std::deque<int>& ids);

	public:
	    PmergeMe(int num_elements, char *seq[]);
		PmergeMe(const PmergeMe &other);
		PmergeMe &operator=(const PmergeMe &other);
		~PmergeMe();
	
		std::vector<int> fordJohnsonVec();
		std::deque<int> fordJohnsonDeque();
};		
	


