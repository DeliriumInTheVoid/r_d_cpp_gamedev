#pragma once

#include <string_view>

static constexpr unsigned game_words_num = 1000;

static constexpr std::string_view game_words_file_path  = "./data/words.txt";
static constexpr std::string_view daily_words_file_path = "./data/daily_words.txt";
static constexpr std::string_view best_score_file_path  = "./save/best_score.txt";
static constexpr std::string_view daily_guess_file_path = "./save/daily_guess.txt";
