/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:18:00 by mshershe          #+#    #+#             */
/*   Updated: 2026/11/02 17:55:34 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"

BitcoinExchange::BitcoinExchange()
{
	BitcoinExchange::load_database("data.csv");
	BitcoinExchange::load_input_file("input.txt");
}

BitcoinExchange::BitcoinExchange(const std::string& database, const std::string& inputfile)
{
	BitcoinExchange::load_database(database);
	BitcoinExchange::load_input_file(inputfile);

}
BitcoinExchange::BitcoinExchange(const BitcoinExchange& other)
{
	this->Record = other.Record;
}
BitcoinExchange& BitcoinExchange::operator=(const BitcoinExchange& other)
{
	if (this != &other)
	{
		this->Record = other.Record;
	}
	return (*this);
}
BitcoinExchange::~BitcoinExchange()
{

}

void BitcoinExchange::load_database(const std::string& filename)
{
	std::ifstream stream(filename.c_str());
	std::string::size_type comma_pos;
	std::string line;
	char extra;

	if (!stream.is_open())
		throw std::runtime_error("Error: Could not open database file");

	if (!std::getline(stream , line))
		throw std::runtime_error("Error: The database is empty");
	if (header_validation(line, "date,exchange_rate"))
		throw std::runtime_error("Error: Malformed database header");

	while (std::getline(stream , line))
	{
		comma_pos = line.find(',');
		if (comma_pos != std::string::npos)
		{
			std::string date = line.substr(0, comma_pos);
			std::string rate_str = line.substr(comma_pos + 1);
			std::istringstream strm(rate_str); //creating a stream from the string (the string is treated as the input)
			double rate; // the destination variable

			if (!(strm >> rate))
				std::cout << "Warning: malformed database row (invalid exchange_rate): "<< line << std::endl;
			else if (strm >> extra)
				std::cout << "Warning: malformed database row (Unexpected non-whitespace characters after the exchange_rate): "<< line << std::endl;//reject trailing characters   e.g. 0.05abc
			else
				Record.insert(std::make_pair(date, rate));
		}
		else
			std::cout << "Warning: malformed database row (missing comma): "<< line << std::endl;//no comma is found in the line
	}
	if (Record.empty())
		throw std::runtime_error("Error: Empty valid database");
}


double BitcoinExchange::rate_lookup(const std::string& key_date)
{
	if (date_validation(key_date))
		return -1;
	std::map<std::string, double>::iterator entry = this->Record.find(key_date);
	if (entry != this->Record.end())
		return ((*entry).second);
	else
	{
		entry = this->Record.lower_bound(key_date);
		if (entry != Record.begin())
		{
			entry--;
			return ((*entry).second);
		}
		else
		{
			std::cout << "No exchange rate is available for the date given nor before "<< std::endl;
			return -1;
		}
	}
}


void BitcoinExchange::load_input_file(const std::string& filename)
{
	std::ifstream stream(filename.c_str());
	std::string::size_type sep_pos;
	std::string line;
	char extra;

	if (!stream.is_open())
		throw std::runtime_error("Error: Could not open the input file");

	if (!std::getline(stream , line))
		throw std::runtime_error("Error: The input file is empty");
	if (header_validation(line, "date | value"))
		throw std::runtime_error("Error: Malformed input file header");

	while (std::getline(stream , line))
	{
		sep_pos = line.find('|');
		if (sep_pos != std::string::npos)
		{
			std::string date = trim(line.substr(0, sep_pos));
			std::string amount_str =  trim(line.substr(sep_pos + 1));
			std::istringstream strm(amount_str);
			double amount;

			if (!(strm >> amount))
				std::cout << "Warning: malformed input row (invalid amount): "<< line << std::endl;
			else if (strm >> extra)
				std::cout << "Warning: malformed input row (Unexpected non-whitespace characters after the amount): "<< line << std::endl;
			else if (date_validation(date))
				std::cout << "Error: bad input => "<< line << std::endl;
			else if (!amount_validation(amount) && !date_validation(date))
				calculate_et_print(std::make_pair(date, amount));
		}
		else
			std::cout << "Warning: malformed input row (missing '|'): "<< line << std::endl;//no | is found in the line
	}
}

void BitcoinExchange::calculate_et_print(const std::pair<std::string, double>& input_cell)
{
	double rate = rate_lookup(input_cell.first);
	if (rate == -1)
		std::cout << "no refrence rate found"<< std::endl;
	else
	{
		double result = rate * input_cell.second;
		std::cout<< input_cell.first <<" => " <<  input_cell.second << " = "<< result <<std::endl;
	}
}

int date_validation(const std::string& date)
{
	if (date.length() != 10 || date.at(4) != '-' ||  date.at(7) != '-')
		return 1;
	std::string year_str = date.substr(0, 4);
	std::string month_str = date.substr(5, 2);
	std::string day_str = date.substr(8, 2);

	if (year_str.find_first_not_of("0123456789") != std::string::npos ||
		month_str.find_first_not_of("0123456789") != std::string::npos ||
		day_str.find_first_not_of("0123456789") != std::string::npos)
		return 1;

	int year = std::atoi(year_str.c_str());
	int month = std::atoi(month_str.c_str());
	int day = std::atoi(day_str.c_str());

	if (month <= 0 || month > 12 || day < 1 || day > 31)
		return 1;
	if ((month == 4 || month == 6 || month == 9 || month ==11) && day == 31)
		return 1;
	else if (month == 2 && (day == 30 || day == 31))
		return 1;
	else if (month == 2 && (year % 400 ==  0 || (year % 4 == 0 && year % 100 != 0)))//leap year
	{
		if (day > 29)
			return 1;
	}
	else if (month == 2 && day > 28)
		return 1;
	return 0;
}

int amount_validation(double amount)
{
	if (amount < 0)
	{
		std::cout << "Error: not a positive number." <<std::endl;
		return 1;
	}
	else if (amount > 1000)
	{
		std::cout << "Error: too large a number." <<std::endl;
		return 1;
	}
	else
		return 0;

}

int exchange_rate_validation(double rate)
{
	if (rate < 0)
	{
		std::cout << "Error: a rate can't be negative" <<std::endl;
		return 1;
	}
	else
		return 0;
}

//csv header: "date,exchange_rate"
//input header: "date | value"
int header_validation(const std::string& to_check, const std::string& refrence)
{
	if (trim(to_check) == refrence)
		return 0;
	else
		return 1;
}

std::string trim(const std::string& str)
{
    std::string::size_type first = str.find_first_not_of(" \t\r\n");

    if (first == std::string::npos)
        return "";

    std::string::size_type last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, last - first + 1);
}
