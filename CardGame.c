#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>

//Declare struct to represent cards
struct card {
    int suit;
    int number;
    int seed; 
};

//List to represent suits and numbers in deck
char suits[4][9] = {"Spades", "Hearts", "Diamonds", "Clubs"};
char numbers[13][6] = {"Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine" , "Ten", "Jack", "Queen", "King", "Ace"};

//Struct to represent deck of 52 cards
typedef struct card Card;
Card deck[52];

//Struct to represent hand of 5 cards for each player
struct card player_1[5];
struct card player_2[5];
struct card player_3[5];
struct card player_4[5];

//Function to initialise a deck of 52 cards (4 suits and 13 cards each)
//Card number represented 1 -> 13 and 2 -> Ace
//Suit represented 1 -> 4 and Heart -> Clubs
void init_deck() 
{
    int a, b;
    int c = 0;
    for(a = 0; a < 4; ++a) {
        for(b = 0; b < 13; ++b) {
            deck[c].suit = a;
            deck[c].number = b;
            ++c;
      }
    }
    return;
}

//Function to issue cards to each player with seed representing where first card should be drawn from within the deck
void *issue_cards(void *arg) 
{
    struct card *player = (struct card *)arg;
    int i;
    int x;
    int seed = player[0].seed;
    for (i = 0; i < 5; ++i) {
        x = seed + i;
        player[i] = deck[x];
    }
    return 0;
}

//Function to shuffle the cards in the deck
void shuffle_deck() 
{
    Card temp;
    int i; int j; 
    int count = 0;
    for (count = 0; count < 100; ++count) {
        for (i = 0; i < 52; ++i) {
            srand(time(0));
            j = rand() % 51;
            temp = deck[i];
            deck[i] = deck[j];
            deck[j] = temp;
        } 
    }
    return;
}

//Function used to sort cards in the hand according to card number
int compare_sort(const void *av, const void *bv)
{
    const struct card *a = av, *b = bv;
    if (a->number < b->number) return -1;
    if (a->number > b->number) return 1;
    return 0;
}

//Fucnction to check if single hand contains a straight
int check_straight (struct card *hand) 
{
    int min = hand[0].number;
    int max = hand[4].number;
    min = min + 4;
    if (min == max) return 1;
    else return 0;
}

//Function to check if single hand contains a flush
int check_flush (struct card *hand) 
{
    int suit = hand[0].suit;
    int i;
    for (i = 1; i < 5; ++i) {
        if (suit != hand[i].suit) {
            return 0;
        }
    }
    return 1;
}

//Function to check if either hand has a flush and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has a flush)
int compare_flush(struct card *hand_1, struct card *hand_2)
{
    if ((check_flush(hand_1)) && (check_flush(hand_2))){
        if (hand_1[0].suit < hand_2[0].suit) {
            return 1;
        } else {
            return 2;
        }
    } else if (check_flush(hand_1)) {
        return 1;
    } else if (check_flush(hand_2)) {
        return 2;
    } else {
        return 0;
    }
}

//Function to check if either hand has a straight flush and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher , 0 = no hand has a straight flush)
int compare_straight_flush(struct card *hand_1, struct card *hand_2)
{
    if ((check_flush(hand_1) && check_straight(hand_1)) && (check_flush(hand_2) && check_straight(hand_2))) {
        if (hand_1[4].number > hand_2[4].number) {
            return 1; 
        } else {
            return 2;
        }
    } else if (check_flush(hand_1) && check_straight(hand_1)) {
        return 1;
    } else if (check_flush(hand_2) && check_straight(hand_2)) {
        return 2;
    }
    return 0;
}

//Function to check if single hand has 4 of a kind
int check_four(struct card *hand) 
{
    int a, b, c, d, e;
    a = hand[0].number; b = hand[1].number; c = hand[2].number; d = hand[3].number; e = hand[4].number;
    if ((a == b) && (a == c) & (a == d)) return 1;
    else if ((e == b) && (e == c) && (e == d)) return 1;
    else return 0;
}

//Function to check if either hand has four of a kind and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has four of a kind)
int compare_four(struct card *hand_1, struct card *hand_2)
{
    if (check_four(hand_1) && check_four(hand_2)) {
        if (hand_1[3].number > hand_2[3].number) {
            return 1;
        } else {
            return 2;
        }
    } else if (check_four(hand_1)){
        return 1;
    } else if (check_four(hand_2)) {
        return 2;
    } else {
        return 0;
    }
}

//Function to check if a single hand contains a full house
int check_full_house(struct card *hand) 
{
    int a, b, c, d, e;
    a = hand[0].number; b = hand[1].number; c = hand[2].number; d = hand[3].number; e = hand[4].number;
    if (((a == b) && (a == c)) && (d == e)) return a;
    else if (((e == d) && (e == c)) && (a == b)) return e;
    else return -1;
}

//Function to check if either hand has a full house return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has full house)
int compare_full_house(struct card *hand_1, struct card *hand_2) 
{
    int a = check_full_house(hand_1);
    int b = check_full_house(hand_2);
    if ((a > -1) && (b > -1)) {
        if (a > b) {
            return 1;
        } else {
            return 2;
        }
    } else if (a > -1) {
        return 1;
    } else if (b > -1) {
        return 2;
    } else {
        return 0;
    }
}

//Function to check if either hand has a straight and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has a straight)
int compare_straight(struct card *hand_1, struct card *hand_2)
{
    if ((check_straight(hand_1)) && (check_straight(hand_2))) {
        if (hand_1[4].number > hand_2[4].number) {
            return 1; 
        } else {
            return 2;
        }
    } else if (check_straight(hand_1)) {
        return 1;
    } else if (check_straight(hand_2)) {
        return 2;
    }
    return 0;
}

//Function to check if single hand has three of a kind
int check_three(struct card *hand) 
{
    int a, b, c, d, e;
    a = hand[0].number; b = hand[1].number; c = hand[2].number; d = hand[3].number; e = hand[4].number;
    if ((a == b) && (a == c)) return 1;
    else if ((e == b) && (e == c)) return 1;
    else if ((b == c) && (b == d)) return 1;
    else return 0;
}

//Function to check if either hand has a three of a kind and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has three of a kind)
int compare_three(struct card *hand_1, struct card *hand_2)
{
    if (check_three(hand_1) && check_three(hand_2)) {
        if (hand_1[3].number > hand_2[3].number) {
            return 1;
        } else {
            return 2;
        }
    } else if (check_three(hand_1)){
        return 1;
    } else if (check_three(hand_2)) {
        return 2;
    } else {
        return 0;
    }
}

//Function to check if single hand has TWO pairs
int check_two_pair(struct card *hand) 
{
    int a, b, c, d, e;
    a = hand[0].number; b = hand[1].number; c = hand[2].number; d = hand[3].number; e = hand[4].number;
    if ((a == b) && (c == d)) {
        if (a > c) {
            return a; 
        } else {
            return c;
        }
    } else if ((a == b) && (d == e)) {
         if (a > d) {
            return a; 
        } else {
            return d;
        }
    } else if ((b == c) && (d == e)) {
         if (b > d) {
            return b; 
        } else {
            return d;
        }
    } else {
        return -1;
    }
}

//Function to check if either hand has TWO pairs and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has two pairs)
int compare_two_pair(struct card *hand_1, struct card *hand_2) 
{
    int a = check_two_pair(hand_1);
    int b = check_two_pair(hand_2);
    if ((a > -1) && (b > -1)) {
        if (a > b){
            return 1;
        } else {
            return 2;
        }
    } else if (a > -1) {
        return 1;
    } else if (b > -1) {
        return 2;
    } else {
        return 0;
    }
}

//Function to check if a single hand has ONE pair
int check_pair(struct card *hand) 
{
    int a, b, c, d, e;
    a = hand[0].number; b = hand[1].number; c = hand[2].number; d = hand[3].number; e = hand[4].number;
    if (a == b) return a;
    else if (b == c) return b;
    else if (c == d) return c;
    else if (d == e) return d;
    else return -1;
}

//Function to check if either hand has ONE pair and return the higher hand (1 = hand_1 higher, 2 = hand_2 higher, 0 = no hand has one pair)
int compare_pair(struct card *hand_1, struct card *hand_2) 
{
    int a = check_pair(hand_1);
    int b = check_pair(hand_2);
    if ((a > -1) && (b > -1)) {
        if (a > b){
            return 1;
        } else {
            return 2;
        }
    } else if (a > -1) {
        return 1;
    } else if (b > -1) {
        return 2;
    } else {
        return 0;
    }
}

//Function to check which hand has a high card (1 = hand_1 higher, 2 = hand_2 higher)
int compare_high_card(struct card *hand_1, struct card *hand_2) 
{
    if (hand_1[4].number == hand_2[4].number) {
        if (hand_1[4].suit < hand_2[4].suit){
            return 1;
        } else {
            return 2;
        }
    } else if (hand_1[4].number > hand_2[4].number) {
        return 1;
    } else {
        return 2;
    }
}

//Logic to compare two hands and return the highest hand
//Logical order for poker hands found here https://www.cardplayer.com/rules-of-poker/hand-rankings
Card* compare(struct card *hand_1, struct card *hand_2) 
{
        if (compare_straight_flush(hand_1, hand_2) == 1) return hand_1;
        else if (compare_straight_flush(hand_1, hand_2) == 2) return hand_2;
        else if (compare_four(hand_1, hand_2) == 1) return hand_1;
        else if (compare_four(hand_1, hand_2) == 2) return hand_2;
        else if (compare_full_house(hand_1, hand_2) == 1) return hand_1;
        else if (compare_full_house(hand_1, hand_2) == 2) return hand_2;
        else if (compare_flush(hand_1, hand_2) == 1) return hand_1;
        else if (compare_flush(hand_1, hand_2) == 2) return hand_2;
        else if (compare_straight(hand_1, hand_2) == 1) return hand_1;
        else if (compare_straight(hand_1, hand_2)== 2) return hand_2;
        else if (compare_three(hand_1, hand_2) == 1) return hand_1;
        else if (compare_three(hand_1, hand_2)== 2) return hand_2;
        else if (compare_two_pair(hand_1, hand_2) == 1) return hand_1;
        else if (compare_two_pair(hand_1, hand_2) == 2) return hand_2;
        else if (compare_pair(hand_1, hand_2) == 1) return hand_1;
        else if (compare_pair(hand_1, hand_2) == 2) return hand_2;
        else if (compare_high_card(hand_1, hand_2) == 1) return hand_1;
        else if (compare_high_card(hand_1, hand_2) == 2) return hand_2;
        else return 0;
}

//Function to print out cards in a players hand
void print_hand(struct card *hand)
{
    int i;
    for (i = 0; i < 5; ++i){
        int a = hand[i].suit;
        int b = hand[i].number;
        printf("%s - %s\n", suits[a], numbers[b]);
    }
}

int main() 
{
    init_deck();
    shuffle_deck();

    Card (*winningHand);

    //Intialise seed value for each player (card in deck from which we will start drawing the hand)
    player_1[0].seed = 0; player_2[0].seed = 15; player_3[0].seed = 30; player_4[0].seed = 45;

    //Create thread to generate each players hand from deck
    pthread_t thread_id;
    pthread_create(&thread_id, NULL, issue_cards, &player_1);
    pthread_create(&thread_id, NULL, issue_cards, &player_2);
    pthread_create(&thread_id, NULL, issue_cards, &player_3);
    pthread_create(&thread_id, NULL, issue_cards, &player_4);
    pthread_join(thread_id, NULL);

    //Sort hand according to card number
    qsort(player_1, 5, sizeof(struct card), compare_sort);
    qsort(player_2, 5, sizeof(struct card), compare_sort);
    qsort(player_3, 5, sizeof(struct card), compare_sort);
    qsort(player_4, 5, sizeof(struct card), compare_sort);
    
    winningHand = compare(player_1, player_2);
    winningHand = compare(winningHand, player_3);
    winningHand = compare(winningHand, player_4);

    printf("Winning Hand\n");
    print_hand(winningHand);
    printf("Player 1\n");
    print_hand(player_1);
    printf("Player 2\n");
    print_hand(player_2);
    printf("Player 3\n");
    print_hand(player_3);
    printf("Player 4\n");
    print_hand(player_4);
}
  