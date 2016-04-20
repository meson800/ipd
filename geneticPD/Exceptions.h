#pragma once
#include <stdexcept>

class GenomeException : public std::runtime_error
{
public:
	GenomeException(const std::string &err) : runtime_error(err) {}
};