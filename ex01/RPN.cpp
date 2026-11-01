/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:19:56 by mshershe          #+#    #+#             */
/*   Updated: 2026/10/30 17:51:43 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RPN.hpp"

RPN::RPN(const std::string& expr)
{
	int r_op;
	int l_op;
	long long result = 0;
	std::istringstream stream(expr);
	std::string token;
	bool found_token = false;
	
	while (stream >> token)
	{
		found_token = true;
		if (token.find_first_not_of("0123456789+/-*") != std::string::npos)
			throw std::runtime_error("Invalid expression");
		else if (token == "+" || token == "-" || token == "/" || token == "*")
		{
			if (rpn_stack.size() >= 2)
			{
				r_op = rpn_stack.top();
				rpn_stack.pop();
				l_op = rpn_stack.top();
				rpn_stack.pop();
			}
			else
				throw std::runtime_error("Not enough operands");
			if (token == "+")
				result = l_op + r_op;
			else if(token == "-")
				result = l_op - r_op;
			else if(token == "/")
			{
				if (r_op != 0)
				{
					result = l_op / r_op;
				}
				else
					throw std::runtime_error("Invalid operation: division by zero");
			}
			else if(token == "*")
				result = l_op * r_op;
				
			rpn_stack.push(result);
		}
		else if (token.size() == 1 && token[0] >= '0' && token[0] <= '9')
		{
			int num = std::atoi(token.c_str());
			this->rpn_stack.push(num);
		}
		else 
			throw std::runtime_error("Invalid expression");
	}
	if (!found_token)
		throw std::runtime_error("Empty expression");
	if (rpn_stack.size() != 1)
		throw std::runtime_error("Malformed expression");
	else
		this->result = rpn_stack.top();
}

RPN::RPN()
{
	this->result = 0;
}

RPN::RPN(const RPN& other)
{
	this->result = other.result;
	this->rpn_stack = other.rpn_stack;
}

RPN& RPN::operator=(const RPN& other)
{
	if (this != &other)
	{
		this->result = other.result;
		this->rpn_stack = other.rpn_stack;
	}
	return (*this);
}

RPN::~RPN()
{
	
}

long long RPN::get_result() const
{
	return this->result;
}