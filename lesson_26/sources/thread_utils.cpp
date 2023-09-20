#include <iostream>
#include <thread>
#include <chrono>
#include <iomanip>
#include <arrays.hpp>

#include "thread_utils.hpp"


report run_threads_sum(
    const unsigned threads_num,
    const int random_elements,
    const long long one_thread_time,
    const bool use_thread_scope_sum)
{
    vector<std::thread> threads{};
    threads.resize(threads_num);

    vector<int> random_arr{};
    random_arr.resize(random_elements);

    utl::array_random_fill(random_arr.data(), random_elements, 0, 100);

    auto parts = random_elements / threads_num;

    std::mutex mut{};

    long long sum{ 0 };

    const auto start = std::chrono::high_resolution_clock::now();

    for (unsigned i = 0; i < threads_num; ++i)
    {
        auto th = std::thread{ [&random_arr, &sum, i, parts, &mut, use_thread_scope_sum]()
            {
                int local_sum{ 0 };

                for (unsigned j = i * parts; j < (i + 1) * parts; ++j)
                {
                    if (use_thread_scope_sum)
                    {
                        local_sum += random_arr[j];
                    }
                    else
                    {
                        std::lock_guard lock{ mut };
                        sum += random_arr[j];
                    }
                }

                if (use_thread_scope_sum)
                {
                    std::lock_guard lock{ mut };
                    sum += local_sum;
                }

            } };

        threads[i] = std::move(th);
    }

    for (unsigned i = 0; i < threads_num; ++i)
    {
        threads[i].join();
    }

    const auto end = std::chrono::high_resolution_clock::now();

    report r;
    const auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    r.time = time > 0 ? time : 1;
    r.boost = threads_num == 1 ? 1 : static_cast<float>(r.time) / static_cast<float>(one_thread_time);
    r.threads_num = threads_num;
    r.elements_num = random_elements;
    r.result = sum;

    return r;
}

void print_reports(const vector<report>& reports, const unsigned threads_num)
{
    std::cout << "  |       " << std::setw(2) << threads_num << "         |";

    for (const report rep : reports)
    {
        std::cout << std::setw(6) << rep.time << "  |" << std::setw(9) << std::fixed << std::setprecision(3) << rep.boost << "|";
    }

}

vector<report> create_reports_by_thread_num(
    const vector<int>& elements_nums,
    const unsigned threads_num,
    const vector<long long>& one_thread_times,
    const bool use_thread_scope_sum)
{
    const std::size_t elements_num{ elements_nums.size() };

    vector<report> reports{ elements_num };

    for (std::size_t i = 0; i < elements_num; ++i)
    {
        const report rep = run_threads_sum(threads_num, elements_nums[i], one_thread_times[i], use_thread_scope_sum);
        reports[i] = rep;
    }

    return reports;
}

void build_table_with_report(
    const vector<int>& elements_nums,
    const vector<unsigned>& threads_nums,
    const bool use_thread_scope_sum)
{
    vector<long long> one_thread_times{ 0, 0, 0, 0, 0 };
    std::cout << "  -------------------------------------------------------------------------------------------------------------------\n";
    std::cout << "  |                  |                                       elements number                                        |\n";
    std::cout << "  |                  |----------------------------------------------------------------------------------------------|\n";
    std::cout << "  |                  |      10'000      |      100'000     |    1'000'000     |    10'000'000    |    100'000'000   |\n";
    std::cout << "  |------------------|------------------|------------------|------------------|------------------|------------------|\n";
    std::cout << "  |  threads number  |  Time  |  Boost  |  Time  |  Boost  |  Time  |  Boost  |  Time  |  Boost  |  Time  |  Boost  |\n";
    std::cout << "  |------------------|--------|---------|--------|---------|--------|---------|--------|---------|--------|---------|\n";
    for (const unsigned int threads : threads_nums)
    {
        const vector<report> reports = create_reports_by_thread_num(elements_nums, threads, one_thread_times, use_thread_scope_sum);
        if (threads == 1)
        {
            for (std::size_t j = 0; j < reports.size(); ++j)
            {
                one_thread_times[j] = reports[j].time;
            }
        }
        print_reports(reports, threads);
        std::cout << std::endl;
    }
    std::cout << "  -------------------------------------------------------------------------------------------------------------------\n";
}
