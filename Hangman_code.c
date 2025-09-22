#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

// Defined a struct
typedef struct {
    char word1[50];
    char word2[50];
    char word3[50];
} WordCategory;

// Function prototypes
void get_a_word(int category, WordCategory *w);
int guess_check(char word[], char guess, char guesses[]);
void hangman_display(int tries);
void word_display(char word[], int length, char guesses[], int *word_found);
int was_already_guessed(const char guesses[], char guess);
int read_category(void);
char read_yes_no(void);
char read_guess(const char guesses[]);
void print_guessed_letters(const char guesses[]);

#define MAX_TRIES 6
#define WORD_SIZE 50
#define GUESS_SIZE 26

#define CATEGORY_CAR 1
#define CATEGORY_PERSON 2
#define CATEGORY_COUNTRY 3

int main() {
    const int words_number = 9;
    char word[WORD_SIZE];
    char guesses[WORD_SIZE] = {0};
    int tries, word_found, category;
    char play_again, guess;
    WordCategory words;
    
    srand(time(0));
    
    // this input validation loop allows user to play multiple games 
    do {
    printf("WELCOME TO HANGMAN! \n");
    category = read_category();   // validated category input (1-3)
    
    get_a_word(category, &words);  // get a random word from selected category

        tries = 0;  // set tries initially to zero
        word_found = 0;
        for (int i = 0; i < GUESS_SIZE; i++) 
            guesses[i] = 0;
        
        int x = rand() % 3;  // a random word is selected from the chosen category
        switch (x) {
            case 0:
                strcpy(word, words.word1);  // copies a random word selected for the user in main function to be guessed by the user
                break;
            case 1:
                strcpy(word, words.word2);
                break;
            case 2:
                strcpy(word, words.word3);
                break;
        }
        
        int word_length = strlen(word);
        
        while (tries < MAX_TRIES && !word_found) {
            system("cls");
            hangman_display(tries);  // displays hangman figure 
            printf("Tries left: %d\n", MAX_TRIES - tries);  // display amount of tries left
            printf("Guessed letters: ");
            print_guessed_letters(guesses);
            word_display(word, word_length, guesses, &word_found);
            
            if (!word_found) {
                guess = read_guess(guesses);

                // Add guess to list if not already present (track both right and wrong)
                if (!was_already_guessed(guesses, guess)) {
                    size_t glen = strlen(guesses);
                    guesses[glen] = guess;
                    guesses[glen + 1] = '\0';
                }

                if (!guess_check(word, guess, guesses)) {
                    tries++;  // tries increment by 1 if wrong letter is guessed
                }
            }
        }
        
        system("cls");
        hangman_display(tries);
        printf("Guessed letters: ");
        print_guessed_letters(guesses);
        word_display(word, word_length, guesses, &word_found);
        
        if (word_found) {
            printf("Congratulations! You won!\n");
        } else {
            printf("You lost! The word was: %s\n", word);
        }
        
        printf("Do you want to play again? (Enter 'y' for yes and 'n' for no)\n");
        play_again = read_yes_no();
        
    } while (play_again == 'y');
    
    return 0;
}

void get_a_word(int category, WordCategory *w) {
    switch(category) {
        case CATEGORY_CAR:
            strcpy(w->word1, "bently");  // copies the second string in the struct's string (word1)
            strcpy(w->word2, "tucson");
            strcpy(w->word3, "mehran");
            break;
        case CATEGORY_PERSON:
            strcpy(w->word1, "ayesha");
            strcpy(w->word2, "fatima");
            strcpy(w->word3, "zainab");
            break;
        case CATEGORY_COUNTRY:
            strcpy(w->word1, "brazil");
            strcpy(w->word2, "canada");
            strcpy(w->word3, "mexico");
            break;
        default:
            printf("Invalid category. Try again! \n");
            break;
    }
}

int guess_check(char word[], char guess, char guesses[]) {
    int is_guess = 0;  // Initially set to 0 (false)
    int guesses_length = strlen(guesses);
    
    for (int i = 0; i < strlen(word); i++) {
        if (guess == word[i]) {        // if guessed letter found at any index of given word
            is_guess = 1;              // the flag condition becomes true
            break;
        }
    }
    
    // Ensure the guess is tracked in the guesses list (only once). Caller also handles this,
    // but keep this to remain backward-compatible if guess_check is used elsewhere.
    int already_guessed = 0;
    for (int i = 0; i < guesses_length; i++) {
        if (guesses[i] == guess) {
            already_guessed = 1;
            break;
        }
    }
    if (!already_guessed) {
        guesses[guesses_length] = guess;
        guesses[guesses_length + 1] = '\0';
    }
    
    return is_guess;
}

void hangman_display(int tries) {
    const char *hangman[] = {
        "  +---+\n  |   |\n      |\n      |\n      |\n      |\n=========\n",
        "  +---+\n  |   |\n  O   |\n      |\n      |\n      |\n=========\n",
        "  +---+\n  |   |\n  O   |\n  |   |\n      |\n      |\n=========\n",
        "  +---+\n  |   |\n  O   |\n /|   |\n      |\n      |\n=========\n",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n      |\n      |\n=========\n",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n /    |\n      |\n=========\n",
        "  +---+\n  |   |\n  O   |\n /|\\  |\n / \\  |\n      |\n=========\n"
    };
    
    printf("%s\n", hangman[tries]);  // prints strings present in the hangman array according to indexes based on the number of tries used
}

void word_display(char word[], int length, char guesses[], int *word_found) {
    *word_found = 1;
    for (int i = 0; i < length; i++) {
        int letter_found = 0;         // assuming that initially letter has not been found by user
        for (int j = 0; j < strlen(guesses); j++) {
            if (word[i] == guesses[j]) {   // checks if the entered letter matches any letter already present in guesses array
                letter_found = 1;   // letter has been found which means it has been guessed correctly by the user
                break;
            }
        }
        if (!letter_found) {
            *word_found = 0;  // indicates word has not been gussed by user [0='false']
            printf("_ ");
        } else {
            printf("%c ", word[i]);  // if guessed coeectly, the entered letter is printed
        }
    }
    printf("\n");
}

int was_already_guessed(const char guesses[], char guess) {
    for (int i = 0; i < (int)strlen(guesses); i++) {
        if (guesses[i] == guess) return 1;
    }
    return 0;
}

int read_category(void) {
    int category = 0;
    while (1) {
        printf("Select a category:\n"); 
        printf("1. Car names\n");
        printf("2. Person names\n");
        printf("3. Country names\n");
        printf("Enter your choice: ");
        if (scanf("%d", &category) == 1 && category >= 1 && category <= 3) {
            return category;
        }
        // Invalid input: clear line
        printf("Invalid choice. Please enter 1, 2, or 3.\n");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

char read_yes_no(void) {
    char ch = 0;
    while (1) {
        if (scanf(" %c", &ch) == 1) {
            ch = (char)tolower((unsigned char)ch);
            if (ch == 'y' || ch == 'n') return ch;
        }
        printf("Please enter 'y' or 'n': ");
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

char read_guess(const char guesses[]) {
    char guess = 0;
    while (1) {
        printf("Enter a letter to guess: ");
        if (scanf(" %c", &guess) == 1) {
            guess = (char)tolower((unsigned char)guess);
            if (isalpha((unsigned char)guess)) {
                if (was_already_guessed(guesses, guess)) {
                    printf("You already guessed '%c'. Try a different letter.\n", guess);
                } else {
                    return guess;
                }
            } else {
                printf("Please enter a valid alphabet letter.\n");
            }
        }
        int c;
        while ((c = getchar()) != '\n' && c != EOF) {}
    }
}

void print_guessed_letters(const char guesses[]) {
    if (strlen(guesses) == 0) {
        printf("(none)\n");
        return;
    }
    for (int i = 0; i < (int)strlen(guesses); i++) {
        printf("%c ", guesses[i]);
    }
    printf("\n");
}
