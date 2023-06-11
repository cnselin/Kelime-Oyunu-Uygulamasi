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
