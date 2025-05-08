#ifndef FILEIO_H
#define FILEIO_H

#include <vector>
#include <string>
#include "quiz.h"

// Soruları dosyadan okur
std::vector<Question> loadQuestions(const std::string& filename);

// Kullanıcı adı, skor ve toplam soruyu dosyaya ekler
void saveResults(const std::string& filename,
                 const std::string& username,
                 int score,
                 int total);

// Dosyaya yeni soru ekler; başarılıysa true döner
bool addQuestion(const std::string& filename);

// Sonuç dosyasından istatistikleri hesaplayıp gösterir
bool viewStats(const std::string& filename);

bool authenticate(const std::string& credentialsFile, const std::string& username, const std::string& password);

#endif // FILEIO_H
