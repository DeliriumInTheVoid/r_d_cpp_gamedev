#pragma once

#include <vector>

using std::vector;

struct report
{
    long long time{ 0 };
    long long result{};
    float boost{ 0 };
    unsigned threads_num{ 0 };
    int elements_num{ 0 };
};

report run_threads_sum(
    const unsigned threads_num,
    const int random_elements,
    const long long one_thread_time,
    const bool use_thread_scope_sum);

void print_reports(const vector<report>& reports, const unsigned threads_num);

vector<report> create_reports_by_thread_num(
    const vector<int>& elements_nums,
    const unsigned threads_num,
    const vector<long long>& one_thread_times,
    const bool use_thread_scope_sum);


void build_table_with_report(
    const vector<int>& elements_nums,
    const vector<unsigned>& threads_nums,
    const bool use_thread_scope_sum);
