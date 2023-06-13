#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define MAX_TIME 240
#define QUESTION_TIME 20
#define MAX_ANSWER_LENGTH 10
#define MAX_QUESTIONS 14
#define LETTER_SCORE 20

typedef struct {
    char question[100];
    char answer[MAX_ANSWER_LENGTH];
} Question;

typedef struct {
    Question questions[MAX_QUESTIONS];
    int questionCount;
    int score;
} GameData;

void shuffleQuestions(Question* questions, int questionCount) {
    srand(time(NULL));
    for (int i = 0; i < questionCount; i++) {
        int j = rand() % (i + 1);
        Question temp = questions[i];
        questions[i] = questions[j];
        questions[j] = temp;
    }
}

void loadQuestions(GameData* gameData) {
    FILE* file = fopen("abc.txt", "r");
    if (file == NULL) {
        printf("Sorular dosyasi bulunamadi.\n");
        exit(1);
    }
    
    char line[150];
    int questionIndex = 0;
    while (fgets(line, sizeof(line), file)) {
        line[strcspn(line, "\r\n")] = '\0'; // Satır sonundaki geçersiz karakterleri temizler
        char* question = strtok(line, ",");
        char* answer = strtok(NULL, ",");
        strcpy(gameData->questions[questionIndex].question, question);
        strcpy(gameData->questions[questionIndex].answer, answer);
        questionIndex++;
    }

    gameData->questionCount = questionIndex;

    fclose(file);
   
    shuffleQuestions(gameData->questions, gameData->questionCount);
}

void revealLetter(char* hiddenAnswer, const char* answer, char letter, int* score, int* revealedLetterCount) {
    int letterCount = 0;
    for (int i = 0; i < strlen(answer); i++) {
        if (answer[i] == letter) {
            hiddenAnswer[i] = letter;
            letterCount++;
            (*revealedLetterCount)++;
        }
    }
    *score -= letterCount * LETTER_SCORE;
}
void playGame(GameData* gameData, const char* playerName) {
    int totalScore = 0;
    int remainingTime = MAX_TIME;

    srand(time(NULL));

    for (int i = 0; i < gameData->questionCount; i++) {
        Question currentQuestion = gameData->questions[i];
        printf("\nSoru %d: %s\n", i + 1, currentQuestion.question);

        char hiddenAnswer[MAX_ANSWER_LENGTH + 1];
        for (int j = 0; j < strlen(currentQuestion.answer); j++) {
            hiddenAnswer[j] = '_';
        }
        hiddenAnswer[strlen(currentQuestion.answer)] = '\0';

        printf("Cevap: ");
        for (int j = 0; j < strlen(hiddenAnswer); j++) {
            printf("%c ", hiddenAnswer[j]);
        }
        printf("\n");

        int attempt = 1;
        int timeLeft = QUESTION_TIME;
        int revealedLetterCount = 0;
        time_t questionStartTime = time(NULL);

        int questionFinished = 0;  // Sorunun tamamlanıp tamamlanmadığının kontrolü

        while (timeLeft > 0 && !questionFinished) {
            time_t currentTime = time(NULL);
            int elapsedSeconds = difftime(currentTime, questionStartTime);
            timeLeft = QUESTION_TIME - elapsedSeconds;

            if (timeLeft <= 0) {
                printf("Süre doldu! Oyun bitti.\n");
                break;
            }

            printf("Kalan Süre: %d saniye\n", timeLeft);
            printf("Toplam Kalan Süre: %d saniye\n", remainingTime - elapsedSeconds);

            char guess[MAX_ANSWER_LENGTH];
            printf("\nTahmininizi girin: ");
            scanf("%s", guess);

            if (strstr(currentQuestion.answer, guess) != NULL) {
                int score = 100 * strlen(currentQuestion.answer);
                totalScore += score;
                totalScore -= revealedLetterCount * LETTER_SCORE; // Her yanlış tahminde açılan harf sayısı * 20 puanı toplam puandan çıkar

                printf("Doğru cevap! Puan: %d\n", totalScore);
                questionFinished = 1;  // Soru tamamlandı
            } else if (strcmp(guess, "harf") == 0) {
                if (attempt > strlen(currentQuestion.answer)) {
                    printf("Tüm harfler açıklandı!\n");
                    questionFinished = 1;  // Soru tamamlandı
                } else {
                    char randomLetter;
                    do {
                        randomLetter = currentQuestion.answer[rand() % strlen(currentQuestion.answer)];
                    } while (strchr(hiddenAnswer, randomLetter) != NULL);

                    revealLetter(hiddenAnswer, currentQuestion.answer, randomLetter, &totalScore, &revealedLetterCount);
                    printf("Açılan harf: %c\n", randomLetter);
                    printf("Cevap: ");
                    for (int j = 0; j < strlen(hiddenAnswer); j++) {
                        printf("%c ", hiddenAnswer[j]);
                    }
                    printf("\n");

                    attempt++;
                }
            } else {
                printf("Yanlış tahmin! Tekrar deneyin.\n");
                totalScore -= revealedLetterCount * LETTER_SCORE; // Her yanlış tahminde açılan harf sayısı * 20 puanı toplam puandan çıkar
                printf("Puan: %d\n", totalScore);
            }
        }

        remainingTime -= QUESTION_TIME;
    }

    printf("\nOyun bitti!\n");
    printf("Oyuncu Adı: %s\n", playerName);
    printf("Puanınız: %d\n", totalScore);
    printf("Kalan Süre: %d saniye\n", MAX_TIME - (totalScore / 100));
    time_t currentTime = time(NULL);
    printf("Oynama Tarihi ve Saati: %s", ctime(&currentTime));
}

int main() {
    GameData gameData;
    gameData.questionCount = 0;
    gameData.score = 0;

    loadQuestions(&gameData);

    printf("---------Hoş Geldiniz!----------\n\n");
    printf("-Öncelikle yarışmanın kuralları ile başlayalım!\n\n");
    printf("-Yarışmamız 240 saniyedir, toplamda 14 soru ve cevap bulunmaktadır.\n\n");
    printf("-Sorular ekrana geldiğinde cevap için çizgiler de ekrana yazacaktır.\n\n");
    printf("-Tahminde bulunmak için 20 saniye süreniz bulunmaktadır, doğru bilirseniz diğer soruya geçeriz ve 20 saniyelik süre yenilenir.\n\n");
    printf("-Eğer harf isterseniz tahminde bulunacağınız kısma 'harf' yazmaniz yeterli. Rastgele bir harf açılacak ve sorunun puanından açılan harf sayısı * 20 puan azalacaktır.\n\n");
    printf("-Soruların cevaplarını küçük harflerle yazmalısınız!\n\n");
    printf("-Başarılar ve iyi eğlenceler dileriz...\n\n");
    printf("---------------------------------\n\n");
    printf("KELİME OYUNU BAŞLIYOR!!!\n");

    char playerName[50];
    printf("Oyuncu Adınızı Girin: ");
    scanf("%s", playerName);

    playGame(&gameData, playerName);

    return 0;
}
