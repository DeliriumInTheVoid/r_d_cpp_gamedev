#include "../headers/cyclic_functions.hpp"

void count_numbers_by_sign()
{
	int numbers_qty{ 0 }, positive_qty{ 0 }, negative_qty{ 0 }, zero_qty{ 0 };

	std::cout << "Enter numbers quantity:\n";
	std::cin >> numbers_qty;

	while (--numbers_qty >= 0)
	{
		int number{ 0 };
		std::cin >> number;
		if (number > 0)
		{
			++positive_qty;
		}
		else if (number < 0)
		{
			++negative_qty;
		}
		else
		{
			++zero_qty;
		}
	}

	std::cout << "Positive: " << positive_qty << "; Negative: " << negative_qty << "; Zero: " << zero_qty << std::endl;
}

long long factorial_cycle(const int count)
{
	long long result{ 0 };

	if (count == 0 || count == 1)
	{
		result = 1;
	}
	else
	{
		long long current_value = 1;

		for (int i = 2; i <= count; i++)
		{
			current_value *= i;
		}

		result = current_value;
	}

	return result;
}

void print_natural_numbers_cycle_ascending(const int count)
{
	for (int i = 1; i <= count; ++i)
	{
		std::cout << i << "; ";
	}
}

void print_natural_numbers_cycle_descending(int count)
{
	do
	{
		std::cout << count << "; ";
	}
	while (--count > 0);
}
