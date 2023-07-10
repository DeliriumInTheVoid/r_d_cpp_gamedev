#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>

int generate_random_number(const int min, const int max)
{
    return min + std::rand() % (max - min + 1);
}

void save_best_score(const int score)
{
    if (std::ofstream file{"bestscore.txt"}; file.is_open())
    {
        file << std::to_string(score);
        file.close();
    }
    else
    {
        std::cout << "can't save best score" << std::endl;
    }
}

int load_best_score()
{
    std::ifstream file{"bestscore.txt"};
    int best_score = -1;
    if (file.is_open())
    {
        std::string str_value;
        file >> str_value;
        if (file.fail()) {
            std::cout << "Failed to read the best value from the file." << std::endl;
        }
        else {
            best_score = std::stoi(str_value);
        }

        file.close();
    }
    return best_score;
}

int main()
{
    std::srand(std::time(nullptr));

    int guess{ 0 };
    int secret_number{ 0 };
    int attempts{ 0 };
    int best_score{ load_best_score() };

    std::cout << "Home Work 7\n";
    std::cout << "-----------";

    do
    {
        std::cout << "\n\nTry to guess the riddle number between 0 and 50 (-1 to exit): ";
        std::cin >> guess;

        if (guess == -1)
        {
            std::cout << "Exit!";
            break;
        }

        if (guess < 0 || guess > 50)
        {
            std::cout << "Wrong diapason!" << std::endl;
            break;
        }

        if (attempts == 0)
        {
            secret_number = generate_random_number(0, 50);
        }

        attempts++;

        if (guess == secret_number)
        {
            std::cout << "Congratulations! You guessed the number " << secret_number << " with " << attempts << " attempts!" << std::endl;

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

            attempts = 0;
            std::cout << std::endl;
        }
        else
        {
            std::cout << "It's not right!" << std::endl;

            if (guess > secret_number)
            {
                if (guess - secret_number > 20)
                {
                    std::cout << "The riddle number is less than 20." << std::endl;
                }
                else if (guess - secret_number > 10)
                {
                    std::cout << "The riddle number is less than 10." << std::endl;
                }
                else if (guess - secret_number > 2)
                {
                    std::cout << "Almost there!" << std::endl;
                }
            }
            else
            {
                if (secret_number - guess > 20)
                {
                    std::cout << "The riddle number is more than 20." << std::endl;
                }
                else if (secret_number - guess > 10)
                {
                    std::cout << "The riddle number is more than 10." << std::endl;
                }
                else if (secret_number - guess > 2)
                {
                    std::cout << "Almost there!" << std::endl;
                }
            }
        }
    }
    while (true);

    return 0;
}
