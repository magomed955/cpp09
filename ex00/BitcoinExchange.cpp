/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   BitcoinExchange.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmutsulk <mmutsulk@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 14:53:38 by mmutsulk          #+#    #+#             */
/*   Updated: 2026/10/05 11:18:49 by mmutsulk         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <cctype>
#include <stdexcept>

BitcoinExchange::BitcoinExchange()
{   
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &src)
{
    *this = src;
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &src)
{
    if (this != &src)
        _database = src._database;
    return (*this);
}

BitcoinExchange::~BitcoinExchange()
{
}

void BitcoinExchange::loadDatabase(const std::string &filename)
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
    {
        std::cerr <<"Error: could not open databes file." << std::endl;
        return;
    }
    
    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line); //flux de type stringstream
        std::string date;
        std::string rateStr;

        std::getline(ss, date, ',');
        std::getline(ss, rateStr, ',');

        std::stringstream rateConv(rateStr);
        float rate;
        rateConv >> rate;

        _database[date] = rate;
    }
}

bool BitcoinExchange::isValidDate(const std::string &date) const
{
    if (date.size() != 10 || date[4] != '-' || date[7] != '-')
        return false;

    for (size_t i = 0; i < date.size(); i++)
    {
        if (i == 4 || i == 7)
            continue;
        if (!isdigit(date[i]))
            return false;
    }

    int year  = atoi(date.substr(0, 4).c_str());
    int month = atoi(date.substr(5, 2).c_str());
    int day   = atoi(date.substr(8, 2).c_str());

    if (month < 1 || month > 12)
        return false;

    int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    bool isLeap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
    if (isLeap)
        daysInMonth[1] = 29;

    if (day < 1 || day > daysInMonth[month - 1])
        return false;

    return true;
}

bool BitcoinExchange::isValidValue(const std::string &value, float &result) const
{
    std::stringstream ss(value);
    float num;

    ss >> num;

    if (ss.fail())
        return false;

    std::string leftover;
    ss >> leftover;
    if (!leftover.empty())
        return false;

    result = num;
    return true;
}

float BitcoinExchange::getRate(const std::string &date) const
{
    std::map<std::string, float>::const_iterator it;

    it = _database.lower_bound(date);

    if (it != _database.end() && it->first == date)
        return it->second;

    if (it == _database.begin())
        throw std::runtime_error("no matching date in database");

    --it;
    return it->second;
}

void BitcoinExchange::processInputFile(const std::string &filename)
{
    std::ifstream file(filename.c_str());

    if (!file.is_open())
    {
        std::cerr << "Error: could not open file." << std::endl;
        return;
    }

    std::string line;
    std::getline(file, line);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string date;
        std::string sep;
        std::string valueStr;
        std::string extra;

        ss >> date >> sep >> valueStr;

        if (ss.fail() || sep != "|")
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (ss >> extra)
        {
	        std::cerr << "Error: bad input => " << line << std::endl;
	        continue;
        }

        if (!isValidDate(date))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        float value;
        if (!isValidValue(valueStr, value))
        {
            std::cerr << "Error: bad input => " << line << std::endl;
            continue;
        }

        if (value < 0)
        {
            std::cerr << "Error: not a positive number." << std::endl;
            continue;
        }
        if (value > 1000)
        {
            std::cerr << "Error: too large a number." << std::endl;
            continue;
        }

        try
        {
            float rate = getRate(date);
            std::cout << date << " => " << value << " = " << (value * rate) << std::endl;
        }
        catch (std::exception &e)
        {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}
