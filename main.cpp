#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <limits>

#include "quiz.h"
#include "fileio.h"

int main() {
    std::cout << "Select mode:\n"
                 "1) Quiz Mode\n"
                 "2) Add Question Mode\n"
                 "3) View Stats Mode\n"
                 "Enter choice (1-3): ";
    int mode;
    std::cin >> mode;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    if (mode == 2) {
        // Authentication for Add Question Mode
        std::string user, pass;
        std::cout << "Username: ";
        std::getline(std::cin, user);
        std::cout << "Password: ";
        std::getline(std::cin, pass);

        if (!authenticate("credentials.txt", user, pass)) {
            std::cerr << "Invalid credentials. Exiting.\n";
            return 1;
        }

        if (!addQuestion("questions.txt")) {
            std::cerr << "Failed to add new question.\n";
            return 1;
        }
        return 0;
    }

    if (mode == 3) {
        if (!viewStats("results.txt")) {
            std::cerr << "Failed to read statistics.\n";
            return 1;
        }
        return 0;
    }

    // Quiz mode
    std::string username;
    std::cout << "Enter your name: ";
    std::getline(std::cin, username);

    auto questions = loadQuestions("questions.txt");
    if (questions.empty()) {
        std::cerr << "No questions available. Exiting.\n";
        return 1;
    }

    // Shuffle questions
    std::random_device rd;
    std::mt19937 gen(rd());
    std::shuffle(questions.begin(), questions.end(), gen);

    runQuiz(questions, username);
    return 0;
}