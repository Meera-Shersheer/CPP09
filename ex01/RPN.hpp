/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RPN.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mshershe <mshershe@student.42amman.com>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/10/18 20:20:03 by mshershe          #+#    #+#             */
/*   Updated: 2026/10/30 23:16:21 by mshershe         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include <stack>
#include <iostream>
#include <string>
#include <sstream>
#include <stdexcept>
#include <cstdlib>

class RPN
{
	private:
	std::stack<int> rpn_stack; 
	long long result;
	RPN();
	
	public:
	RPN(const std::string& expr);
	RPN(const RPN& other);
	RPN& operator=(const RPN& other);
	~RPN();
	long long get_result() const;
};