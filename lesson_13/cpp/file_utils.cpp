#include <iostream>
#include <fstream>

#include "../headers/common_utils.hpp"
#include "../headers/file_utils.hpp"
#include "../headers/game_const.hpp"

std::string read_line_from_file(const std::string_view path, const int line_num);
void save_line_to_file(const std::string_view path, const std::string& line);


std::string get_random_word()
{
    const int random_line_num{ generate_random_number(0, game_words_num) };
    return read_line_from_file(game_words_file_path, random_line_num);
}

std::string get_daily_word()
{
    const int current_day_num = get_current_day_num();
    return read_line_from_file(daily_words_file_path, current_day_num);
}

void save_best_score(const int score)
{
    save_line_to_file(best_score_file_path, std::to_string(score));
}

int load_best_score()
{
    const std::string str_value = read_line_from_file(best_score_file_path, 0);
    const int best_score{ std::stoi(str_value) };
    return best_score;
}

void save_daily_guess(const int day_num)
{
    save_line_to_file(daily_guess_file_path, std::to_string(day_num));
}

int load_daily_guess()
{
    const std::string str_value = read_line_from_file(daily_guess_file_path, 0);
    const int best_score{ std::stoi(str_value) };
    return best_score;
}

std::string read_line_from_file(const std::string_view path, const int line_num)
{
    std::ifstream file(path.data());

    if (!file.is_open())
    {
        std::cerr << "Error: Can't open file! " << path << std::endl;
        return {};
    }

    std::string line;
    int current_line_num{ 0 };

    while (std::getline(file, line))
    {
        if (current_line_num == line_num)
        {
            file.close();
            return line;
        }
        ++current_line_num;
    }

    file.close();

    return {};
}

void save_line_to_file(const std::string_view path, const std::string& line)
{
    if (std::ofstream file{ path.data() }; file.is_open())
    {
        file << line;
        file.close();
    }
    else
    {
        std::cout << "can't save file " << path << std::endl;
    }
}
