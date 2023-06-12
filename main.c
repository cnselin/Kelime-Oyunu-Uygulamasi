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
