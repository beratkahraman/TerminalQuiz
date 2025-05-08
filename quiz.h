#ifndef QUIZ_H
#define QUIZ_H

#include <string>
#include <vector>

// Tek bir soruyu temsil eden yapı
struct Question {
    std::string text;                // Soru metni
    std::vector<std::string> options; // Şıklar (A, B, C, D…)
    char correct;                    // Doğru şık ('A', 'B', …)
};

// Soruları dosyadan okuyan fonksiyon
std::vector<Question> loadQuestions(const std::string& filename);

// Quiz'i çalıştıran, soruları gösterip cevapları kontrol eden fonksiyon
void runQuiz(const std::vector<Question>& questions, const std::string& username);

#endif // QUIZ_H
