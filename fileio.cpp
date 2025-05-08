#include "fileio.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include <iomanip>   // std::fixed, std::setprecision

std::vector<Question> loadQuestions(const std::string& filename) {
    std::vector<Question> questions;
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: Cannot open file '" << filename << "'\n";
        return questions;
    }

    std::string line;
    while (std::getline(in, line)) {
        if (line.rfind("SORU:", 0) == 0) {
            Question q;
            q.text = line.substr(6);

            for (int i = 0; i < 4; ++i) {
                if (!std::getline(in, line)) break;
                if (line.size() > 3 && line[1] == ')')
                    q.options.push_back(line.substr(3));
            }
            if (std::getline(in, line) && line.rfind("DOGRU:", 0) == 0) {
                q.correct = line[7];
            }
            questions.push_back(std::move(q));
        }
    }
    return questions;
}

void saveResults(const std::string& filename,
                 const std::string& username,
                 int score,
                 int total) {
    std::ofstream out(filename, std::ios::app);
    if (!out) {
        std::cerr << "Error: Cannot open results file\n";
        return;
    }
    double pct = total > 0 ? (100.0 * score) / total : 0.0;
    out << username
        << " - Score: " << score << " / " << total
        << " (" << std::fixed << std::setprecision(2) << pct << "%)"
        << "\n";
}

bool addQuestion(const std::string& filename) {
    std::ofstream out(filename, std::ios::app);
    if (!out) {
        std::cerr << "Error: Cannot open questions file\n";
        return false;
    }

    std::string q;
    std::cout << "Enter the question text: ";
    std::getline(std::cin, q);

    std::vector<std::string> opts(4);
    for (int i = 0; i < 4; ++i) {
        char label = 'A' + i;
        std::cout << "Option " << label << ": ";
        std::getline(std::cin, opts[i]);
    }

    char correct;
    while (true) {
        std::cout << "Enter correct option (A-D): ";
        std::cin >> correct;
        correct = std::toupper(correct);
        if (correct >= 'A' && correct <= 'D') {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            break;
        }
        std::cout << "Invalid. Please enter A, B, C, or D.\n";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    out << "SORU: " << q << "\n";
    for (int i = 0; i < 4; ++i) {
        out << char('A' + i) << ") " << opts[i] << "\n";
    }
    out << "DOGRU: " << correct << "\n\n";
    std::cout << "Question added successfully.\n";
    return true;
}

bool viewStats(const std::string& filename) {
    std::ifstream in(filename);
    if (!in) {
        std::cerr << "Error: Cannot open results file\n";
        return false;
    }

    int count = 0;
    double sumPct = 0.0;
    double maxPct = 0.0;
    std::string line;

    while (std::getline(in, line)) {
        auto pos1 = line.find("Score:");
        if (pos1 == std::string::npos) continue;
        auto posL = line.find('(', pos1);
        auto posP = line.find('%', posL);
        if (posL == std::string::npos || posP == std::string::npos) continue;
        double pct = std::stod(line.substr(posL + 1, posP - posL - 1));
        sumPct += pct;
        maxPct = std::max(maxPct, pct);
        ++count;
    }

    if (count == 0) {
        std::cout << "No statistics to display.\n";
        return true;
    }

    double avgPct = sumPct / count;
    std::cout << "Number of attempts: " << count << "\n"
              << "Average score: " << std::fixed << std::setprecision(2)
              << avgPct << "%\n"
              << "Highest score: " << maxPct << "%\n";
    return true;
}

// Kullanıcı adı ve şifreyi credentials.txt'ten okuyup doğrular
bool authenticate(const std::string& credentialsFile,
                  const std::string& username,
                  const std::string& password) {
    std::ifstream in(credentialsFile);
    if (!in) {
        std::cerr << "Error: Cannot open credentials file\n";
        return false;
    }

    std::string line;
    while (std::getline(in, line)) {
        auto sep = line.find(':');
        if (sep == std::string::npos) continue;
        std::string user = line.substr(0, sep);
        std::string pass = line.substr(sep + 1);
        if (user == username && pass == password) {
            return true;
        }
    }
    return false;
}
