#include "quiz.h"
#include "fileio.h"

#include <iostream>
#include <thread>
#include <chrono>
#include <random>
#include <algorithm>
#include <vector>
#include <utility>
#include <limits>
#include <cctype>
#include <conio.h>    // for _kbhit(), _getch()
#include <iomanip>    // for std::fixed, std::setprecision

// ANSI renk kodları
static constexpr const char* COLOR_RED    = "\033[31m";
static constexpr const char* COLOR_GREEN  = "\033[32m";
static constexpr const char* COLOR_YELLOW = "\033[33m";
static constexpr const char* COLOR_RESET  = "\033[0m";

static constexpr int TIME_LIMIT_SECONDS = 30;

void runQuiz(const std::vector<Question>& questions,
             const std::string& username) {
    int score = 0;
    std::mt19937 gen{ std::random_device{}() };

    for (size_t i = 0; i < questions.size(); ++i) {
        const auto& q = questions[i];

        // Şıkları ve doğru cevabı eşleştir
        int correctIndex = q.correct - 'A';
        std::vector<std::pair<std::string,bool>> opts;
        for (size_t j = 0; j < q.options.size(); ++j)
            opts.emplace_back(q.options[j], j == correctIndex);
        std::shuffle(opts.begin(), opts.end(), gen);

        // Soru ve şıkları göster
        std::cout << "\nQuestion " << (i + 1)
                  << " of " << questions.size() << ": " << q.text << "\n";
        for (size_t j = 0; j < opts.size(); ++j)
            std::cout << char('A' + j) << ") " << opts[j].first << "\n";

        // Başlangıçta prompt ve timer satırı
        int timeLeft = TIME_LIMIT_SECONDS;
        char answer = 0;
        std::cout << "Time left: " << timeLeft
                  << "s    Answer: " << std::flush;

        auto start = std::chrono::steady_clock::now();

        // Zaman aşımı ve tuş bekleme döngüsü
        while (true) {
            // Yanıt gelmiş mi?
            if (_kbhit()) {
                answer = std::toupper(_getch());
                if (answer >= 'A' && answer < char('A' + opts.size()))
                    break;
            }
            // Zaman kontrolü
            auto now = std::chrono::steady_clock::now();
            int elapsed = std::chrono::duration_cast<std::chrono::seconds>(now - start).count();
            int rem = TIME_LIMIT_SECONDS - elapsed;
            if (rem < 0) rem = 0;
            if (rem != timeLeft) {
                timeLeft = rem;
                std::cout << "\rTime left: " << timeLeft
                          << "s    Answer: " << std::flush;
            }
            if (elapsed >= TIME_LIMIT_SECONDS) {
                break;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(50));
        }

        // Satırı temizle
        std::cout << "\r" << std::string(50, ' ') << "\r";

        // Süre dolduysa
        if (answer == 0) {
            std::cout << COLOR_YELLOW
                      << "Time's up! Moving to next question."
                      << COLOR_RESET << "\n";
            continue;
        }

        // Kullanıcı yanıtını ekrana bas
        std::cout << "Answer: " << answer << "\n";

        // Doğruluk kontrolü
        size_t idx = answer - 'A';
        if (opts[idx].second) {
            std::cout << COLOR_GREEN << "Correct!" << COLOR_RESET << "\n";
            ++score;
        } else {
            // Doğru seçeneğin harfini bul
            char real = 'A';
            for (size_t k = 0; k < opts.size(); ++k)
                if (opts[k].second) { real = char('A' + k); break; }
            std::cout << COLOR_RED
                      << "Wrong! Correct answer is " << real << "."
                      << COLOR_RESET << "\n";
        }

        // Anlık başarı oranını göster
        double accuracy = 100.0 * score / double(i + 1);
        std::cout << "Current accuracy: "
                  << std::fixed << std::setprecision(2)
                  << accuracy << "%\n";
    }

    // Sonuçları göster ve kaydet
    std::cout << "\nQuiz finished. Your final score: "
              << score << " / " << questions.size()
              << " (" << std::fixed << std::setprecision(2)
              << (100.0 * score / questions.size()) << "%)\n";
    saveResults("results.txt", username, score, questions.size());
    std::cout << "Your result has been saved to results.txt\n";
}