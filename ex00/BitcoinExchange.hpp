/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:18:07 by mshershe          #+#    #+#             */
/*   Updated: 2026/10/29 20:31:45 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <map>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <utility>
#include <stdexcept>
#include <cstdlib>

class BitcoinExchange
{
	private:
		std::map<std::string, double> Record;
		
		BitcoinExchange();
		void load_database(const std::string& filename);
		void load_input_file(const std::string& filename);
		double rate_lookup(const std::string& key_date);
		
	public:
		BitcoinExchange(const std::string& database, const std::string& inputfile);
		BitcoinExchange(const BitcoinExchange& other);
		BitcoinExchange& operator=(const BitcoinExchange& other);
		~BitcoinExchange();
		void calculate_et_print(const std::pair<std::string, double>& input_cell);

	
};

int date_validation(const std::string& date);
int amount_validation(double amount);
int exchange_rate_validation(double rate);
int header_validation(const std::string& to_check, const std::string& refrence);
std::string trim(const std::string& str);