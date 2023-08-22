#include <cstdlib>
#include <chrono>

#include "../headers/common_utils.hpp"

int generate_random_number(const int min, const int max)
{
    return min + std::rand() % (max - min + 1);
}

int get_current_day_num()
{
    const std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    const std::time_t time_now = std::chrono::system_clock::to_time_t(now);
    const std::tm local_time = *std::localtime(&time_now);
    const int day_number = local_time.tm_yday + 1;

    return day_number;
}
