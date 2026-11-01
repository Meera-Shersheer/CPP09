/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   PmergeMe.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:20:47 by mshershe          #+#    #+#             */
/*   Updated: 2026/11/01 20:02:09 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "PmergeMe.hpp"

PmergeMe::PmergeMe(): has_straggler(false), straggler(0)
{}
PmergeMe::PmergeMe(int num_elements, char *seq[])
{
	int i = 1;
	while (i < num_elements)
	{
		char *end;
		errno = 0;

		long value = std::strtol(seq[i], &end, 10);
		if (end == seq[i] || *end != '\0')
			throw std::runtime_error("Expected a positive integer.");
		if (errno == ERANGE)// Overflow
			throw std::runtime_error("Input integer is out of range.");
		if (value <= 0)
			throw std::runtime_error("All integers must be greater than zero.");
		if (value > INT_MAX)
			throw std::runtime_error("Input integer exceeds INT_MAX.");
		vector_seq.push_back(static_cast<int>(value));
		deque_seq.push_back(static_cast<int>(value));	
		i++;
	}
	if (vector_seq.size() % 2 == 0)
		this->has_straggler = 0;
	else
		this->has_straggler = 1;
}

PmergeMe::PmergeMe(const PmergeMe &other)
{
	*this = other;
}

PmergeMe& PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
    {
        vector_seq = other.vector_seq;
        deque_seq = other.deque_seq;
        vec_pairs = other.vec_pairs;
        deque_pairs = other.deque_pairs;
        has_straggler = other.has_straggler;
        straggler = other.straggler;
    }
    return *this;
}

PmergeMe::~PmergeMe()
{}

void PmergeMe::form_pairs_vec()
{
	vec_pairs.clear();
	size_t i = 0;
	int first;
	int second;
	
	while ((i + 1) < vector_seq.size())
	{
		first = 0;
		second = 0;
		if (vector_seq.at(i) > vector_seq.at(i + 1))
		{
			first = vector_seq.at(i + 1);
			second = vector_seq.at(i);
		}
		else
		{
			first = vector_seq.at(i);
			second = vector_seq.at(i + 1);
		}
		vec_pairs.push_back(std::make_pair(first, second));
		i+= 2;
	}
	if (i < vector_seq.size() && has_straggler)
		straggler = vector_seq.at(i);
}

std::vector<std::pair<int, int> > PmergeMe::sortPairsVec(std::vector<std::pair<int, int> > pairs)
{
	std::vector<int> ids;
    for (size_t i = 0; i < pairs.size(); i++)
        ids.push_back(i);

    std::vector<int> sorted_ids = sortPairIdsVec(pairs, ids);
    std::vector<std::pair<int, int> > result;

    for (size_t i = 0; i < sorted_ids.size(); ++i)
        result.push_back(pairs[sorted_ids[i]]);
		
    return result;
}

std::vector<int> PmergeMe::gen_jacobsthal_seq(int size)
{
	std::vector<int> jacobsthal;
	
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);
    while (jacobsthal.back() < size)
    {
        size_t n = jacobsthal.size();
        int next = jacobsthal[n - 1] + 2 * jacobsthal[n - 2];
        jacobsthal.push_back(next);
    }
	return jacobsthal;
}

std::vector<int>  PmergeMe::genJacobsthalOrder(int size)
{
	std::vector<int> jacobsthal = gen_jacobsthal_seq(size);
	std::vector<int> insertion_order;

    for (size_t k = 3; k < jacobsthal.size(); ++k)
    {
        int upper = jacobsthal[k];
        int lower = jacobsthal[k - 1] + 1;
		
        if (upper > size)
            upper = size;
        if (lower > size)
            continue;

        int j = upper;
        while (j >= lower)
        {
            insertion_order.push_back(j);
            if (j == lower)
                break;
            --j;
        }
    }
	return insertion_order;
}

std::vector<std::pair<int, int> > PmergeMe::makePairings_v( const std::vector<std::pair<int, int> >& pairs, const std::vector<int>& ids)
{
	std::vector<std::pair<int, int> > pairings;
    size_t  i = 0;

    while (i + 1 < ids.size())
    {
        int id1 = ids[i];
        int id2 = ids[i + 1];

        if (pairs[id1].second > pairs[id2].second)
            pairings.push_back(std::make_pair(id1, id2));
        else
            pairings.push_back(std::make_pair(id2, id1));
		i += 2;
    }

    return pairings;
}

std::vector<int> PmergeMe::sortPairIdsVec(const std::vector<std::pair<int, int> >& pairs, const std::vector<int>& ids)
{
	if (ids.size() <= 1)
        return ids;

    std::vector<int> winners;
	std::vector<std::pair<int, int> > pairings = makePairings_v(pairs, ids);
	for (size_t i = 0; i < pairings.size(); i++)
        winners.push_back(pairings[i].first);

	std::vector<int> sorted_winners = sortPairIdsVec(pairs, winners);
	std::vector<int> chain = sorted_winners;// build main chain
	//Generate Jacobsthal insertion order.
	std::vector<int> insertion_order =  genJacobsthalOrder(sorted_winners.size()); // Insert losers in Jacobsthal order.
	
	insertion_order.insert(insertion_order.begin(), 1);
	for (size_t i = 0; i < insertion_order.size(); i++)
    {
        int winner_index = insertion_order[i] - 1;
        int loser_id = -1;

        for (size_t j = 0; j < pairings.size(); ++j) // Find the loser associated with this winner.
        {
            if (pairings[j].first == sorted_winners[winner_index])
            {
                loser_id = pairings[j].second;
                break;
            }
        }
        int partner_pos = 0; // Find the winner's current position in the chain.
        while (chain[partner_pos] != sorted_winners[winner_index])
            partner_pos++;
		
		int loser = pairs[loser_id].second;	
		std::vector<int> value_chain;

		for (size_t j = 0; j < chain.size(); ++j)
			value_chain.push_back(pairs[chain[j]].second);

		std::vector<int>::iterator insert_pos = std::lower_bound( value_chain.begin(), value_chain.begin() + partner_pos,  loser);	
		int position = insert_pos - value_chain.begin();
		if (position < 0 || position > static_cast<int>(chain.size()))
			throw std::runtime_error("Invalid insertion position.");
		chain.insert(chain.begin() + position, loser_id);
    }

    if (ids.size() % 2 != 0)// Insert the straggler
    {
		int straggler_id = ids.back();
		int straggler = pairs[straggler_id].second;
		std::vector<int> value_chain;
			
		for (size_t j = 0; j < chain.size(); ++j)
			value_chain.push_back(pairs[chain[j]].second);

		std::vector<int>::iterator insert_pos = std::lower_bound(value_chain.begin(), value_chain.end(), straggler);
		int position = insert_pos - value_chain.begin();			
		if (position < 0 || position > static_cast<int>(chain.size()))
			throw std::runtime_error("Invalid insertion position.");
		chain.insert(chain.begin() + position, straggler_id);

	}
    
    return chain;
}

std::vector<int> PmergeMe::fordJohnsonVec()
{
	form_pairs_vec();
	std::vector<std::pair<int, int> > sorted_pairs = sortPairsVec(vec_pairs);
	std::vector<int> chain;

    for (size_t i = 0; i < sorted_pairs.size(); i++)
	{
        chain.push_back(sorted_pairs[i].second);
	}
	if (!sorted_pairs.empty())
	{
		std::vector<int> insertion_order = genJacobsthalOrder(sorted_pairs.size());
		insertion_order.insert(insertion_order.begin(), 1);
		
		for (size_t i = 0; i < insertion_order.size(); i++)
		{
			int pair_index = insertion_order[i] - 1;
			int loser = sorted_pairs[pair_index].first;
			int partner = sorted_pairs[pair_index].second;
			size_t partner_pos = 0;
			
			while (partner_pos < chain.size() && chain[partner_pos] != partner)
				partner_pos++;
			
			std::vector<int>::iterator insert_pos = std::lower_bound(chain.begin(), chain.begin() + partner_pos, loser);
			chain.insert(insert_pos, loser);
		}
	}

    if (has_straggler)
    {	
		std::vector<int>::iterator insert_pos = std::lower_bound(chain.begin(), chain.end(), straggler);
		chain.insert(insert_pos, straggler);
    }

    return chain;
	
}

///////////////////////////////////////////////////////
void PmergeMe::form_pairs_deque()
{
	deque_pairs.clear();
	size_t i = 0;
	int first;
	int second;
	
	while ((i + 1) < deque_seq.size())
	{
		first = 0;
		second = 0;
		if (deque_seq.at(i) > deque_seq.at(i + 1))
		{
			first = deque_seq.at(i + 1);
			second = deque_seq.at(i);
		}
		else
		{
			first = deque_seq.at(i);
			second = deque_seq.at(i + 1);
		}
		deque_pairs.push_back(std::make_pair(first, second));
		i+= 2;
	}
	if (i < deque_seq.size() && has_straggler)
		straggler = deque_seq.at(i);
}

std::deque<std::pair<int, int> > PmergeMe::sortPairsDeque( const std::deque<std::pair<int, int> >& pairs)
{
	std::deque<int> ids;
    for (size_t i = 0; i < pairs.size(); i++)
        ids.push_back(i);
		

    std::deque<int> sorted_ids = sortPairIdsDeque(pairs, ids);
    std::deque<std::pair<int, int> > result;

    for (size_t i = 0; i < sorted_ids.size(); ++i)
        result.push_back(pairs[sorted_ids[i]]);

    return result;
}

std::deque<int> PmergeMe::gen_jacobsthal_seq_deq(int size)
{
	std::deque<int> jacobsthal;
	
    jacobsthal.push_back(0);
    jacobsthal.push_back(1);
    while (jacobsthal.back() < size)
    {
        size_t n = jacobsthal.size();
        int next = jacobsthal[n - 1] + 2 * jacobsthal[n - 2];
        jacobsthal.push_back(next);
    }
	return jacobsthal;
}

std::deque<int> PmergeMe::genJacobsthalOrder_deque(int size)
{
	std::deque<int> jacobsthal = gen_jacobsthal_seq_deq(size);
	std::deque<int> insertion_order;

    for (size_t k = 3; k < jacobsthal.size(); ++k)
    {
        int upper = jacobsthal[k];
        int lower = jacobsthal[k - 1] + 1;

        if (upper > size)
            upper = size;
        if (lower > size)
            continue;

        int j = upper;
        while (j >= lower)
        {
            insertion_order.push_back(j);
            if (j == lower)
                break;
            --j;
        }
    }
	return insertion_order;
}

std::deque<std::pair<int, int> > PmergeMe::makePairings_d( const std::deque<std::pair<int, int> >& pairs, const std::deque<int>& ids)
{
	std::deque<std::pair<int, int> > pairings;
    size_t  i = 0;
	
    while (i + 1 < ids.size())
    {
        int id1 = ids[i];
        int id2 = ids[i + 1];

        if (pairs[id1].second > pairs[id2].second)
            pairings.push_back(std::make_pair(id1, id2));
        else
            pairings.push_back(std::make_pair(id2, id1));
		i += 2;
    }
    return pairings;
}

std::deque<int> PmergeMe::sortPairIdsDeque(const std::deque<std::pair<int, int> >& pairs, const std::deque<int>& ids)
{
	if (ids.size() <= 1)
        return ids;

    std::deque<int> winners;
	std::deque<std::pair<int, int> > pairings = makePairings_d(pairs, ids);
	
	for (size_t i = 0; i < pairings.size(); i++)
        winners.push_back(pairings[i].first);

	std::deque<int> sorted_winners = sortPairIdsDeque(pairs, winners);
	std::deque<int> chain = sorted_winners;// build main chain

	//Generate Jacobsthal insertion order.
	std::deque<int> insertion_order =  genJacobsthalOrder_deque(sorted_winners.size()); // Insert losers in Jacobsthal order.
	
	insertion_order.insert(insertion_order.begin(), 1);
	for (size_t i = 0; i < insertion_order.size(); i++)
    {
        int winner_index = insertion_order[i] - 1;
        int loser_id = -1;

        for (size_t j = 0; j < pairings.size(); ++j) // Find the loser associated with this winner.
        {
            if (pairings[j].first == sorted_winners[winner_index])
            {
                loser_id = pairings[j].second;
                break;
            }
        }
        
        int partner_pos = 0; // Find the winner's current position in the chain.

        while (chain[partner_pos] != sorted_winners[winner_index])
            partner_pos++;
		
		int loser = pairs[loser_id].second;	
		std::deque<int> value_chain;

		for (size_t j = 0; j < chain.size(); ++j)
			value_chain.push_back(pairs[chain[j]].second);
		
		std::deque<int>::iterator insert_pos = std::lower_bound(value_chain.begin(), value_chain.begin() + partner_pos, loser);
		int position = insert_pos - value_chain.begin();
		if (position < 0 || position > static_cast<int>(chain.size()))
			throw std::runtime_error("Invalid insertion position.");
		chain.insert(chain.begin() + position, loser_id);
    }

    if (ids.size() % 2 != 0)// Insert the straggler
    {
		int straggler_id = ids.back();
		int straggler = pairs[straggler_id].second;
		std::deque<int> value_chain;
			
		for (size_t j = 0; j < chain.size(); ++j)
			value_chain.push_back(pairs[chain[j]].second);
			
		std::deque<int>::iterator insert_pos = std::lower_bound( value_chain.begin(), value_chain.end(), straggler);
		int position = insert_pos - value_chain.begin();
		if (position < 0 || position > static_cast<int>(chain.size()))
			throw std::runtime_error("Invalid insertion position.");
		chain.insert(chain.begin() + position, straggler_id);
	}
    return chain;
}

std::deque<int> PmergeMe::fordJohnsonDeque()
{
	form_pairs_deque();
	std::deque<std::pair<int, int> > sorted_pairs = sortPairsDeque(deque_pairs);
	std::deque<int> chain;

    for (size_t i = 0; i < sorted_pairs.size(); i++)
	{
        chain.push_back(sorted_pairs[i].second);
	}
	if (!sorted_pairs.empty())
	{
		std::deque<int> insertion_order = genJacobsthalOrder_deque(sorted_pairs.size());
		insertion_order.insert(insertion_order.begin(), 1);

		for (size_t i = 0; i < insertion_order.size(); i++)
		{
			int pair_index = insertion_order[i] - 1;
			int loser = sorted_pairs[pair_index].first;
			int partner = sorted_pairs[pair_index].second;
			size_t partner_pos = 0;
			
			while (partner_pos < chain.size() &&chain[partner_pos] != partner)
				partner_pos++;
			
			std::deque<int>::iterator insert_pos = std::lower_bound(chain.begin(),chain.begin() + partner_pos, loser);
			chain.insert(insert_pos, loser);
		}
	}

    if (has_straggler)
    {
		std::deque<int>::iterator insert_pos = std::lower_bound(chain.begin(),chain.begin() + chain.size(), straggler);
        chain.insert(insert_pos, straggler);
    }

    return chain;
}
