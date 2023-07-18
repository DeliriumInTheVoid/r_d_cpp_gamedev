#include "../headers/recursive_functions.hpp"

long long factorial_recursion(const int count)
{
	if (count == 0 || count == 1)
	{
		return 1;
	}

	return count * factorial_recursion(count - 1);
}

void print_natural_numbers_recurse_ascending(int count)
{
	if (count < 1)
	{
		return;
	}

	print_natural_numbers_recurse_ascending(count - 1);

	std::cout << count << "; ";
}

void print_natural_numbers_recurse_descending(int count)
{
	if (count < 1)
	{
		return;
	}

	std::cout << count << "; ";

	print_natural_numbers_recurse_descending(--count);
}
