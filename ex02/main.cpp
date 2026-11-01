/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:21:29 by mshershe          #+#    #+#             */
/*   Updated: 2026/11/01 20:10:48 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "PmergeMe.hpp"
#include <ctime>
#include <iomanip>

int main (int argc, char *argv[])
{
	if (argc < 2)
	{
		std::cerr << "Error: PmergeMe requires a positive integer sequence as an argument"<< std::endl;
		return 1;
	}
	try
	{
		PmergeMe pool(argc, argv);

		std::cout << "Before: ";
		for (int i = 1; i < argc ;i++)
			std::cout << argv[i] << " ";
		std::cout << std::endl;

		std::clock_t start_v = std::clock();
        std::vector<int> sorted_vec = pool.fordJohnsonVec();
		std::clock_t end_v = std::clock();
		double vector_time = static_cast<double>(end_v - start_v) * 1000000.0 / CLOCKS_PER_SEC;

		std::clock_t start_d = std::clock();
        std::deque<int> sorted_deque = pool.fordJohnsonDeque();
		std::clock_t end_d = std::clock();
		double deque_time = static_cast<double>(end_d - start_d) * 1000000.0 / CLOCKS_PER_SEC; // the result are in micro seconds
        
		std::cout << "After: ";
		for (size_t i = 0; i < sorted_vec.size() ;i++)
			std::cout << sorted_vec.at(i) << " ";
		std::cout << std::endl;
		
		std::cout << std::fixed << std::setprecision(5);
		std::cout << "Time to process a range of " << argc - 1 << " elements with std::vector: " << vector_time << " us" << std::endl;
		std::cout << "Time to process a range of " << argc - 1 << " elements with std::deque: "  << deque_time << " us" << std::endl;

	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}


/* 
!		cases:
!	x	5	One element 
!	x	2 1	Smallest pair
!	x	1 2 3	Odd number of elements
!	x	3 2 1	Reverse order
!	x	5 5 5 5	Duplicates
!	x	3 1 3 2 1	Duplicates and odd size
!	x	1 2 3 4 5 6	Already sorted
!	x	6 5 4 3 2 1	Reverse sorted
!	x	2147483647	Maximum positive int
!	x	0 1 2	Zero should be rejected
!	x	-1 2 3	Negative input should be rejected
!		3 abc 5	Invalid input
!		2147483648	Integer overflow during parsing
!		Small sequence	./PmergeMe 3 5 9 7 4
!		Already sorted	./PmergeMe 1 2 3 4 5 6
!		Reverse sorted	./PmergeMe 6 5 4 3 2 1
!	x	Duplicates	./PmergeMe 5 3 5 1 3 1 
!	x	One element	./PmergeMe 42
!		Odd count	./PmergeMe 9 2 7 4 1
!		Even count	./PmergeMe 8 3 6 1 9 2
!		Invalid zero	./PmergeMe 0 2 3
!		Negative	./PmergeMe -1 2 3
!		Invalid text	./PmergeMe 2 abc 3
!		Overflow	./PmergeMe 2147483648
!	x	./PmergeMe $(shuf -i 1-100000 -n 3000) 
*/

/*
* Concepts to understand
*	
*	STL containers: vector and deque
*	- vector has contiguous memory, while deque use segmented storage
*	- An iterator is not guaranteed to remain valid after modifying its container.
*	
*	std::pair and data relationships
*		std::vector<std::pair<int, int> > pairs;
*		pairs.push_back(std::make_pair(3, 9));
*		straggler
*	
*	Recursion
*	
*	Binary search and insertion
*	- Binary Search :
*		- assuming we have a sorted sequence.
*		- repeatedly examines the middle element and 
*		eliminate a half of the sequence where the 
*		number is not located.
*		- Each comparison eliminates approximately half the remaining elements.
*		- it decrements the number of needed comparisions drastically.
*	- Binary search is provided in the C++98 standard library:
*		std::lower_bound(first, last, value);
*		- it returns the first element that is not less than value
*		- If the value already exists, lower_bound points to its first occurrence.
*		- If the value is greater than every element, it returns numbers.end()
*		- The last iterator is an exclusive boundary: it is not included in the search.
*	- Restricted search ranges:
*		- we already know an upper bound for each pending element because of its pair relationship.
*	
*	Jacobsthal sequence
*	- The number of comparisons required by binary search depends on the size of the search range.
*	- The Jacobsthal sequence is a sequence of integers defined by :
*			J(n) = J(n-1) + 2J(n-2) , J(0)=0, J(1)=1
*	
*			0,1,1,3,5,11,21,43,85, ...
*	
*	- it provides the insertion schedule which determines the order of insertion.
*	
*	std::clock()
*	double elapsed = static_cast<double>(end - start) * 1000000.0 / CLOCKS_PER_SEC;
*	- The result is the elapsed CPU time in microseconds
*	- CLOCKS_PER_SEC is a constant provided by <ctime> which tells you how many clock ticks correspond to one second.
*	
*/

/*
?	Ford–Johnson algorithm
?	- known as Merge-Insertion Sort.
?	- Its importance comes from a particular problem:
?		How can we sort a collection of elements using as few comparisons as possible?
?	- In some situations the number of comparisons matters independently of the number of other operations.
?	- Most sorting algorithms focus on reducing overall computational complexity. 
?	  While Ford–Johnson takes a different approach: it tries to minimize the number of comparisons required to sort the elements.
?	  Its key idea is to use information obtained from previous comparisons to make subsequent comparisons more efficient.
?
?	- It does this through three main mechanisms: Pairing elements, Recursively sorting the larger elements
?	  and Binary insertion with restricted search ranges.
?
?	- Ford–Johnson also uses the Jacobsthal sequence to determine the insertion order. This helps keep 
?	  the binary-search ranges small and makes efficient use of comparisons.
?	
?	- Advatages:
?		Excellent comparison efficiency -> famous for using very few comparisons.
?	- Disadvantages:
?		Not necessarily the fastest in practic
?		Requires additional memory and bookkeeping
?
?
?
?
?	
?	fordJohnson()
?	    |-- 1. Form pairs
?	    |-- 2. Recursively sort pairs by their larger elements
?	    |-- 3. Construct the main chain
?	    |-- 4. Construct the pending sequence
?	    |-- 5. Generate the Jacobsthal insertion order
?	    |-- 6. Insert pending elements using binary insertion
?	    |-- 7. Insert the straggler, if present
?	    |-- 8. Return the sorted sequence

	
*/