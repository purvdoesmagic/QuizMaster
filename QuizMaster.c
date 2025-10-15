#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_QUESTIONS 50
#define QUIZ_QUESTIONS 10
#define MAX_PLAYERS 100

struct Question {
    char topic[50];
    char question[200];
    char options[4][100];
    char correct;
};

struct Player {
    char name[50];
    int score;
};

#define GREEN "\033[0;32m"
#define RED "\033[0;31m"
#define RESET "\033[0m"

void shuffleQuestions(struct Question pool[], int total) {
    srand(1);
    for (int i = total - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        struct Question temp = pool[i];
        pool[i] = pool[j];
        pool[j] = temp;
    }
}

char toUpperManual(char c) {
    if (c >= 'a' && c <= 'd') return c - 32;
    return c;
}

int runRandomQuiz(struct Question pool[], int total, int numQuestions) {
    int score = 0;
    char ans;

    for (int i = 0; i < numQuestions; i++) {
        printf("\nQ%d: %s\n", i + 1, pool[i].question);
        for (int j = 0; j < 4; j++)
            printf("%s\n", pool[i].options[j]);

        while (1) {
            printf("Enter your answer (A/B/C/D): ");
            if (scanf(" %c", &ans) != 1) {
                while (getchar() != '\n');
                printf("Invalid input! Please enter only A, B, C, or D.\n");
                continue;
            }
            while (getchar() != '\n');
            ans = toUpperManual(ans);
            if (ans >= 'A' && ans <= 'D') break;
            else printf("Invalid input! Please enter only A, B, C, or D.\n");
        }

        if (ans == toUpperManual(pool[i].correct)) {
            printf(GREEN "Correct!\n" RESET);
            score++;
        } else {
            printf(RED "Wrong! Correct answer: %c\n" RESET, pool[i].correct);
        }
    }

    printf("\nYour Total Score: %d/%d\n", score, numQuestions);
    return score;
}

void rules() {
    printf("\n===== QUIZ RULES =====\n");
    printf("1. Each question carries 1 mark.\n");
    printf("2. Enter answers as A/B/C/D (case-insensitive).\n");
    printf("3. No negative marking.\n");
    printf("4. Each quiz asks only 10 questions.\n\n");
}

void saveScore(char name[], int score) {
    FILE *fp = fopen("gk_scores.txt", "a");
    if (fp == NULL) return;
    fprintf(fp, "%s %d\n", name, score);
    fclose(fp);
}

void displayLeaderboard() {
    FILE *fp = fopen("gk_scores.txt", "r");
    struct Player players[MAX_PLAYERS];
    int count = 0;

    if (fp == NULL) {
        printf("No leaderboard data found.\n");
        return;
    }

    while (fscanf(fp, "%s %d", players[count].name, &players[count].score) != EOF) {
        count++;
        if (count >= MAX_PLAYERS) break;
    }
    fclose(fp);

    for (int i = 0; i < count - 1; i++) {
        for (int j = i + 1; j < count; j++) {
            if (players[j].score > players[i].score) {
                struct Player temp = players[i];
                players[i] = players[j];
                players[j] = temp;
            }
        }
    }

    printf("\n===== LEADERBOARD =====\n");
    for (int i = 0; i < count; i++) {
        printf("%s : %d\n", players[i].name, players[i].score);
    }
}

int main() {
    char playerName[50];
    printf("Enter your name: ");
    if (fgets(playerName, sizeof(playerName), stdin)) {
        size_t len = strlen(playerName);
        if (len > 0 && playerName[len - 1] == '\n') playerName[len - 1] = '\0';
    }

    rules();

    struct Question pool[TOTAL_QUESTIONS] = {
        // Science
        {"Science", "Chemical symbol for water?", {"A. O2","B. CO2","C. H2O","D. H2"}, 'C'},
        {"Science", "Which planet is Red Planet?", {"A. Mars","B. Venus","C. Jupiter","D. Mercury"}, 'A'},
        {"Science", "Gas absorbed by plants?", {"A. Oxygen","B. Carbon Dioxide","C. Nitrogen","D. Hydrogen"}, 'B'},
        {"Science", "Speed of light?", {"A. 3x10^8 m/s","B. 3x10^6 m/s","C. 3x10^5 m/s","D. 3x10^7 m/s"}, 'A'},
        {"Science", "Theory of relativity by?", {"A. Newton","B. Galileo","C. Einstein","D. Tesla"}, 'C'},
        {"Science", "Hardest natural substance?", {"A. Diamond","B. Gold","C. Iron","D. Quartz"}, 'A'},
        {"Science", "Vitamin from sunlight?", {"A. A","B. B","C. C","D. D"}, 'D'},
        {"Science", "Human heart chambers?", {"A. 2","B. 3","C. 4","D. 5"}, 'C'},
        {"Science", "Organ purifying blood?", {"A. Heart","B. Kidney","C. Liver","D. Lungs"}, 'B'},
        {"Science", "Sound travels fastest in?", {"A. Air","B. Water","C. Steel","D. Vacuum"}, 'C'},

        // History
        {"History", "First President of USA?", {"A. Washington","B. Lincoln","C. Jefferson","D. Adams"}, 'A'},
        {"History", "WWII ended in?", {"A. 1942","B. 1945","C. 1948","D. 1950"}, 'B'},
        {"History", "Who discovered America?", {"A. Columbus","B. Gama","C. Magellan","D. Polo"}, 'A'},
        {"History", "Great Wall protected against?", {"A. Japanese","B. Mongols","C. Russians","D. Indians"}, 'B'},
        {"History", "Iron Man of India?", {"A. Gandhi","B. Nehru","C. Patel","D. Bose"}, 'C'},
        {"History", "First PM of India?", {"A. Nehru","B. Gandhi","C. Prasad","D. Shastri"}, 'A'},
        {"History", "Renaissance began in?", {"A. France","B. Italy","C. England","D. Germany"}, 'B'},
        {"History", "Printing press inventor?", {"A. Gutenberg","B. Edison","C. Bell","D. Tesla"}, 'A'},
        {"History", "North vs South war USA?", {"A. WWI","B. Civil War","C. Rev War","D. Vietnam War"}, 'B'},
        {"History", "First man on moon?", {"A. Gagarin","B. Armstrong","C. Aldrin","D. Collins"}, 'B'},

        // Sports
        {"Sports", "Football team players?", {"A.10","B.11","C.12","D.9"}, 'B'},
        {"Sports", "Olympics held every?", {"A.2","B.3","C.4","D.5"}, 'C'},
        {"Sports", "2018 FIFA World Cup winner?", {"A.Brazil","B.Germany","C.France","D.Argentina"}, 'C'},
        {"Sports", "'Love' term in?", {"A.Cricket","B.Tennis","C.Football","D.Hockey"}, 'B'},
        {"Sports", "Michael Jordan sport?", {"A.Football","B.Basketball","C.Cricket","D.Baseball"}, 'B'},
        {"Sports", "Basketball team players?", {"A.5","B.6","C.7","D.11"}, 'A'},
        {"Sports", "Wimbledon hosted in?", {"A.USA","B.UK","C.Australia","D.France"}, 'B'},
        {"Sports", "Shuttlecock used in?", {"A.Cricket","B.Badminton","C.Tennis","D.Table Tennis"}, 'B'},
        {"Sports", "First FIFA World Cup winner?", {"A.Brazil","B.Uruguay","C.Italy","D.Germany"}, 'B'},
        {"Sports", "Olympic rings number?", {"A.4","B.5","C.6","D.7"}, 'B'},

        // Geography
        {"Geography", "Largest ocean?", {"A.Atlantic","B.Indian","C.Pacific","D.Arctic"}, 'C'},
        {"Geography", "Highest mountain?", {"A.K2","B.Kangchenjunga","C.Everest","D.Makalu"}, 'C'},
        {"Geography", "Longest river?", {"A.Nile","B.Amazon","C.Mississippi","D.Yangtze"}, 'A'},
        {"Geography", "Capital of Japan?", {"A.Tokyo","B.Osaka","C.Kyoto","D.Hiroshima"}, 'A'},
        {"Geography", "Largest desert?", {"A.Sahara","B.Gobi","C.Kalahari","D.Arabian"}, 'A'},
        {"Geography", "Land of Rising Sun?", {"A.China","B.Japan","C.India","D.Thailand"}, 'B'},
        {"Geography", "Mount Fuji located in?", {"A.China","B.India","C.Japan","D.Nepal"}, 'C'},
        {"Geography", "Amazon rainforest in?", {"A.Africa","B.Australia","C.South America","D.Asia"}, 'C'},
        {"Geography", "Longest river in India?", {"A.Ganga","B.Brahmaputra","C.Yamuna","D.Narmada"}, 'A'},
        {"Geography", "Largest country by area?", {"A.USA","B.China","C.Russia","D.Canada"}, 'C'},

        // Current Affairs
        {"Current Affairs", "Current UN Secretary-General?", {"A.António Guterres","B.Kofi Annan","C.Ban Ki-moon","D.Paul Kagame"}, 'A'},
        {"Current Affairs", "Currency of Japan?", {"A.Yen","B.Rupee","C.Dollar","D.Euro"}, 'A'},
        {"Current Affairs", "World Economic Forum held in?", {"A.Paris","B.Davos","C.London","D.New York"}, 'B'},
        {"Current Affairs", "2024 Summer Olympics host?", {"A.Paris","B.Los Angeles","C.Tokyo","D.Berlin"}, 'A'},
        {"Current Affairs", "Current US President?", {"A.Biden","B.Trump","C.Obama","D.Clinton"}, 'A'},
        {"Current Affairs", "World Health Day month?", {"A.March","B.April","C.May","D.June"}, 'B'},
        {"Current Affairs", "Global Climate Summit 2023?", {"A.COP26","B.COP27","C.COP28","D.COP25"}, 'B'},
        {"Current Affairs", "Digital India launched in?", {"A.2009","B.2010","C.2011","D.2012"}, 'C'},
        {"Current Affairs", "BRICS countries count?", {"A.4","B.5","C.6","D.7"}, 'B'},
        {"Current Affairs", "First female PM of India?", {"A.Indira Gandhi","B.Sonia Gandhi","C.Mamata Banerjee","D.Mayawati"}, 'A'}
    };

    shuffleQuestions(pool, TOTAL_QUESTIONS);
    int totalScore = runRandomQuiz(pool, TOTAL_QUESTIONS, QUIZ_QUESTIONS);
    saveScore(playerName, totalScore);

    printf("\nView Leaderboard? (1 for Yes, 0 for No): ");
    int view;
    if (scanf("%d", &view) == 1 && (view == 1)) displayLeaderboard();
    while (getchar() != '\n');

    printf("\nThank you for playing QuizMaster! \n");
    return 0;
}
