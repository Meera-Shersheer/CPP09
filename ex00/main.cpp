/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:21:06 by mshershe          #+#    #+#             */
/*   Updated: 2026/10/29 20:33:41 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"


int main (int argc, char *argv[])
{
	if (argc != 2)
		std::cout<< "Error: could not open file."<< std::endl;
	else
	{
		BitcoinExchange Recordbook("data.csv", argv[1]);
	}
	return (0);
}




