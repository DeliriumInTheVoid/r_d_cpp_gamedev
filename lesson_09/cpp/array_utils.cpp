#include <cstring>
#include <cctype>

#include "../headers/array_utils.hpp"


void translate_array(int numbers[], const int size)
{
	for (int i = 0; i < size; ++i)
	{
		numbers[i] > 0 ? numbers[i] *= 2 : numbers[i] = 0;
	}
}

void to_uppercase(char str[])
{
	for (int i = 0; str[i] != '\0'; i++) {
		if (str[i] >= 'a' && str[i] <= 'z') {
			str[i] -= 'a' - 'A';
		}
	}
}

bool is_palindrome(const char str[])
{
	const int length = std::strlen(str);
	int start = 0;
	int end = length - 1;

	while (start < end) {
		if (str[start] != str[end]) {
			return false;
		}
		start++;
		end--;
	}

	return true;
}

void parse_string_letters(const char str[], int& vowels_count, int& consonants_count)
{
	vowels_count = 0;
	consonants_count = 0;

	for (int i = 0; str[i] != '\0'; i++) {
		if (const char c = static_cast<char>(std::tolower(str[i])); std::isalpha(c)) {
			if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u') {
				vowels_count++;
			}
			else {
				consonants_count++;
			}
		}
	}
}

bool is_equal(const char str1[], const char str2[])
{
	int const len = strlen(str1);

	if (len != strlen(str2))
	{
		return false;
	}

	for (int i = 0; i < len; ++i)
	{
		if (str1[i] != str2[i])
		{
			return false;
		}
	}

	return true;
}
