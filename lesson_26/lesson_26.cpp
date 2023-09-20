#include <iostream>
#include <thread>

#include "console.hpp"

#include "thread_utils.hpp"


int main()
{
    vector elements_nums{ 10'000, 100'000, 1'000'000, 10'000'000, 100'000'000 };
    vector<unsigned> threads_nums{ 1, 2, std::thread::hardware_concurrency(), std::thread::hardware_concurrency() * 2};

    std::cout << std::endl << std::endl;
    std::cout << "Use Thread Scope Sum Accumulator:\n\n";
    build_table_with_report(elements_nums, threads_nums, true);

    std::cout << std::endl << std::endl;
    std::cout << "Use Global Sum Accumulator:\n\n";
    build_table_with_report(elements_nums, threads_nums, false);

    std::cout << std::endl;

    utl::wait_to_continue();

    return 0;
}
