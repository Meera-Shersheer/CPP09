/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:21:20 by mshershe          #+#    #+#             */
/*   Updated: 2026/10/30 17:52:45 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

int main (int argc, char *argv[])
{
	if (argc != 2)
	{
		std::cerr << "RNP take one argument that represnts an inverted Polish mathematical expression" <<std::endl;
		return 1;
	}
	try
	{
		RPN record(argv[1]);
		std::cout << record.get_result() << std::endl;
	}
	catch (const std::exception& e)
	{
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}

	return 0;
}
