#include <iostream>
#include <unordered_map>
#include <algorithm>

#include "../headers/game.hpp"
#include "../headers/file_utils.hpp"
#include "../headers/common_utils.hpp"

void play(std::string_view secret_word);
std::unordered_map<char, std::unique_ptr<std::vector<unsigned>>> parse_secret_word(const std::string_view secret_word);
bool check_guess(const std::unordered_map<char, std::unique_ptr<std::vector<unsigned>>>& chars_map, const std::string& guess, std::string& result_str);

void play_daily_word()
{
    const int daily_guess_num{ load_daily_guess() };
    const int current_day_num{ get_current_day_num() };

    if (daily_guess_num == current_day_num)
    {
        std::cout << "You already guessed the daily word: " << get_daily_word() << std::endl;
        std::cout << "Please come tomorrow!" << std::endl;
        return;
    }

    const std::string word{ get_daily_word() };
    play(word);

    save_daily_guess(current_day_num);
}

void play_random_word()
{
    const std::string word{ get_random_word() };
    play(word);
}

void play(const std::string_view secret_word)
{
    const auto chars_map = parse_secret_word(secret_word);

    int attempts{ 0 };
    int best_score{ load_best_score() };
    const size_t secret_word_len = secret_word.length();
    std::string result_str(secret_word_len, '*');

    while (true)
    {
        attempts++;

        std::cout << "your guess result: " << result_str << std::endl;
        std::string guess;
        std::cout << "enter your guess:  ";
        std::cin >> guess;
        std::cout << std::endl;

        if (secret_word_len != guess.length())
        {
            std::cerr << "Your word should have the same size as secret word: "
                << secret_word.length() << "!" << std::endl << std::endl;
            continue;
        }

        std::ranges::transform(guess, guess.begin(),
            [](const unsigned char ch) { return std::tolower(ch); });

        if (check_guess(chars_map, guess, result_str))
        {
            std::cout << "Congratulations! You guessed the word with " << attempts << " attempts!" << std::endl;

            if (attempts == 1)
            {
                std::cout << "That's incredible!!! Only one attempt was needed!" << std::endl;
            }

            if (best_score == -1 || attempts < best_score)
            {
                std::cout << "A new record!!! Fewest attempts ever: " << attempts << "! Last record was: " << best_score << std::endl;
                best_score = attempts;
                save_best_score(best_score);
            }

            std::cout << std::endl;

            break;
        }
    }
}

std::unordered_map<char, std::unique_ptr<std::vector<unsigned>>> parse_secret_word(const std::string_view secret_word)
{
    std::unordered_map<char, std::unique_ptr<std::vector<unsigned>>> chars_map{};
    const size_t len = secret_word.length();
    for (unsigned i = 0; i < len; ++i)
    {
        auto ch = secret_word[i];

        if (!chars_map.contains(ch))
        {
            auto chars_positions = std::make_unique<std::vector<unsigned>>();
            chars_map[ch] = std::move(chars_positions);
        }

        chars_map[ch]->push_back(i);
    }

    return chars_map;
}

bool check_guess(const std::unordered_map<char, std::unique_ptr<std::vector<unsigned>>>& chars_map, const std::string& guess, std::string& result_str)
{
    const size_t guess_len = guess.length();
    bool correct_guess{ true };

    for (unsigned i = 0; i < guess_len; ++i)
    {
        if (auto ch = guess[i]; chars_map.contains(ch))
        {
            auto& chars_positions = chars_map.at(ch);
            const size_t chars_pos_len = chars_positions->size();

            result_str[i] = ch;
            bool is_on_place{ false };
            for (size_t j = 0; j < chars_pos_len; ++j)
            {
                if (i == chars_positions->at(j))
                {
                    result_str[i] = toupper(ch);
                    is_on_place = true;
                    break;
                }
            }

            correct_guess &= is_on_place;
        }
        else
        {
            result_str[i] = '*';
            correct_guess = false;
        }
    }

    return correct_guess;
}

