#pragma once

#include <string>

std::string get_random_word();

std::string get_daily_word();

void save_best_score(const int score);

int load_best_score();

void save_daily_guess(const int day_num);

int load_daily_guess();
