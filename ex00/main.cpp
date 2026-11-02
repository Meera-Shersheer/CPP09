/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:21:06 by mshershe          #+#    #+#             */
/*   Updated: 2026/11/02 17:51:48 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


int main (int argc, char *argv[])
{
	if (argc != 2)
		std::cout<< "Error: could not open file."<< std::endl;
	else
	{
		try
		{
			BitcoinExchange Recordbook("data.csv", argv[1]);

		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}

	}
	return (0);
}




