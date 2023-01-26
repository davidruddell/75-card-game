/*
Scat (31)
ECE 175 Lazous
April 3rd, 2021
Group: Solo
David Ruddell
*/

#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>
#include<assert.h>
#include<math.h>
#include<stdbool.h>

typedef struct card_s { //given card struct
	char suit[9];
	int value;
	char name[9]; //added a name so the face cards could all be distinguished (bc values are all 10)
	struct card_s* pt; 
} card;

typedef struct player_s { //created a player struct to organize each character's elements such as name, money, and hand card array
	char playerName[20];
	int money;
	card playerHand[4];
} player;

//here is a list where I initialize function prototypes
int LinkListLength(card* HN);
void BuildLinkList(card** HN, card** TN);
int rand_gen(int count);
void CardSwap(card* HN, int i, int j);
void PrintList(card* HN);
void AddDiscard(card** HN, card** HNDiscard, card** TNDiscard);
void PrintDiscard(card* HN);
void DiscardToDeck(card** HN, card** HNDiscard, card** TN);
void PrintPlayerCards(player* p);
int PointsVal(card playerHand[]);

int main(void) {
	char yn; 
	char mrf;
	char mf;
	card* HN = NULL, * TN = NULL;
	srand((int)time(NULL)); //used to get always random shuffle in shuffle function
	player* p1 = NULL;
	player* p2 = NULL;
	card* HNDiscard = NULL, * TNDiscard = NULL;
	int round = 1;

	printf("Welcome to 31!\n");

	BuildLinkList(&HN, &TN); //reads in deck, creates deck linked list

	printf("There are %d cards in the deck\n", LinkListLength(HN));//the point of this is to just check to make sure the listlinklength function works correctly	

	printf("This game is made for 2 players each starting with $100\n");
	p1 = (player*)malloc(sizeof(player));//allocates memory for the player structs to hold information
	p2 = (player*)malloc(sizeof(player));
	p1->money = 100;//both players start with $100
	p2->money = 100;

	printf("\nEnter player 1 name:\n");//player 1 gets to choose a name up to 19 letters
	fgets(p1->playerName, 20, stdin);
	if (p1->playerName[strlen(p1->playerName) - 1] == '\n') {
		p1->playerName[strlen(p1->playerName) - 1] = '\0';
	}

	printf("%s has $%d\n", p1->playerName, p1->money);//shows player's name works along with their money is $100

	printf("\nEnter player 2 name:\n");//same as getting player1 info
	fgets(p2->playerName, 20, stdin);
	if (p2->playerName[strlen(p2->playerName) - 1] == '\n') {
		p2->playerName[strlen(p2->playerName) - 1] = '\0';
	}
	printf("%s has $%d\n", p2->playerName, p2->money);//same as showing player1 info

	printf("\nWould you like to shuffle? (y/n)\n");

	scanf(" %c%*c", &yn);

	while (yn != 'y' && yn != 'n') {//if the input is not 'y' or 'n', it says invalid input so the user can try again.
		printf("Invalid input. Would you like to shuffle? (y/n)\n");
		scanf(" %c%*c", &yn);
	}

	if (yn == 'y') {//if the user chooses to shuffle, the for loop swaps random cards 5000 times
		for (int i = 0; i < 5000; i++) {
			CardSwap(HN, rand_gen(LinkListLength(HN)), rand_gen(LinkListLength(HN)));//this is where i shuffle random cards 5000 times for the "shuffle function"
		}
	}
	PrintList(HN); //shows a deck either chosen to be not shuffled or shuffled.If we run the program again and choose the shuffle the shuffle will never look the same



	while (p1->money > 0 && p2->money > 0) { //this is the start of the game really. the game does not end until one player has all the money and the other has no money.
		int p1bet = 0;	//resets the bets to 0 after every round
		int p2bet = 0;
		bool betsmatched = false;	//reset all bool functions to false for proper operating after restart of while
		bool wantgametoend = false;
		bool somebodyHit31 = false;
		int p1Points = 0;	//resets points to 0. not necessarily needed. just to clean slate them for personal brain organization
		int p2Points = 0;		

				if (round > 1) { //for every round except the first, we rebuild the linked list and shuffle. this is so that if you choose to not shuffle on the first it won't
			BuildLinkList(&HN, &TN);
			for (int i = 0; i < 5000; i++) {
				CardSwap(HN, rand_gen(LinkListLength(HN)), rand_gen(LinkListLength(HN)));//this is where i shuffle random cards 5000 times for the "shuffle function"
			}
		}

		for (int i = 0; i < 3; i++) {//moves first three cards to the first three elements of player one's hand array
			p1->playerHand[i] = *HN;
			*HN = *HN->pt;	//moves the head of the deck 3 cards over now that the first 3 are in p1's hand
		}
		for (int i = 0; i < 3; i++) {//moves first three cards to the first three elements of player two's hand array
			p2->playerHand[i] = *HN;
			*HN = *HN->pt;	//moves the head of the deck 3 cards over now that the first 3 are in p2's hand
		}

		printf("\nThe cards are now dealt. There are %d cards left in the deck\n", LinkListLength(HN));//should print 46 cards in the deck bc i moved three to each player
		AddDiscard(&HN, &HNDiscard, &TNDiscard);//moves one card from the deck to the discard

		printf("The discard pile is now made. There are %d cards left in the deck\n", LinkListLength(HN));//should show that the deck has 45 cards left bc 1 went to discard
		printf("There is %d card in the discard pile\n", LinkListLength(HNDiscard));	//should output only 1 card being in the discard pile
		PrintDiscard(TNDiscard);	//the tail of the discard is sort of like the "top", can be justified as the discard pile is upside down.

		printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));	//shows points based on cards in hand
		
		printf("\n%s's cards are:\n", p1->playerName);//prints the player's cards and money
		PrintPlayerCards(p1);
		printf("\n%s's money is: $%d\n", p1->playerName, p1->money);

		//START OF BETTING
		printf("%s place a bet greater than $0 and less than or equal to $%d:\n", p1->playerName, p1->money);

		scanf("%d", &p1bet);

		while (p1bet <= 0 || p1bet > p1->money) {//makes user choose bet greater than 0 but below the amount of money they have

			printf("Invalid input. Place a bet greater than $0 and less than or equal to $%d\n", p1->money);
			scanf("%d", &p1bet);
			printf("%s's bet is $%d\n", p1->playerName, p1bet);
		}

		printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));//copy of same things as player 1
		printf("\n%s's cards are:\n", p2->playerName);
		PrintPlayerCards(p2);
		printf("\n%s's money is: $%d\n", p2->playerName, p2->money);
		
		printf("%s would you like to match, raise, or fold %s's bet? (m/r/f)\n", p2->playerName, p1->playerName);
		scanf(" %c%*c", &mrf);

		while (mrf != 'm' && mrf != 'r' && mrf != 'f') { //makes sure user inputs 'm' 'r' or 'f' or else it will repeat question
			printf("Invalid input. %s would you like to match, raise, or fold %s's bet? (m/r/f)\n", p2->playerName, p1->playerName);
			scanf(" %c%*c", &mrf);
		}

		if (mrf == 'm') {	//if player chooses to match
			if (p2->money < p1bet) {	//if player chooses to match but they do not have enough money to match
				printf("Not enough money. %s is all in\n", p2->playerName);
				printf("The bet is capped at $%d", p2->money);
				p2bet = p2->money;
				p1bet = p2->money;
				betsmatched = true;
			}
			else {	//if the player matches and has enough money
				p2bet = p1bet;
				betsmatched = true;
			}
		}
		else if (mrf == 'r' && p1bet == p1->money) {	//if the player wants to raise but the other went all in, it won't allow them to
			printf("Sorry you can't raise. Either match or fold (m/f)\n");
			while (mrf != 'm' && mrf != 'f') {
				scanf(" %c%*c", &mrf);
			}
			if (mrf == 'm') {
				if (p2->money < p1bet) {
					printf("Not enough money. %s is all in\n", p2->playerName);
					printf("The bet is capped at $%d", p2->money);
					p2bet = p2->money;
					p1bet = p2->money;
					betsmatched = true;
				}
				else {
					p2bet = p1bet;
					betsmatched = true;
				}
			}
			else if (mrf == 'f') {
				printf("\n%s wins the round!\n", p1->playerName);
				printf("%s has $%d\n", p1->playerName, p1->money);
				printf("%s has $%d\n", p2->playerName, p2->money);
				printf("\nNew round! Reshuffling deck!\n");
			}
		}
		else if (mrf == 'r') {	//if the player chooses to raise the bet (each player can only raise once)
			printf("What is the new bet?\n");
			scanf("%d", &p2bet);

			while (p2bet <= p1bet || p2bet > p2->money) {	//makes sure valid raise. greater than the old bet but no greater than their money balance
				printf("Invalid input. Choose a value between %d and %d. What is the new bet?\n", p1bet, p2->money);
				scanf("%d", &p2bet);
			}

			printf("%s would you like to match the raise or fold? (m/f)\n", p1->playerName);
			scanf(" %c%*c", &mf);

			while (mf != 'm' && mf != 'f') {//makes sure input is 'm' or 'f'
				printf("Invalid input. %s Would you like to match the raise or fold? (m/f)\n", p1->playerName);
				scanf(" %c%*c", &mf);
			}

			if (mf == 'm') {				
				if (p1->money < p2bet) {	//if player matches but does not have enough money, they go all in and bet is adjusted to their money value
					printf("Not enough money. %s is all in\n", p1->playerName);
					printf("The bet is capped at $%d", p1->money);
					p1bet = p1->money;
					p2bet = p1->money;
					betsmatched = true;
				}
				else {
					p1bet = p2bet;
					betsmatched = true;
				}

			}
			else if (mf == 'f') {	//if player folds, the other player wins
				p2->money = p2->money + p1bet;
				p1->money = p1->money - p1bet;
				printf("\n%s wins the round!\n", p2->playerName);
				printf("%s has $%d\n", p1->playerName, p1->money);
				printf("%s has $%d\n", p2->playerName, p2->money);
				printf("\nNew round! Reshuffling deck!\n");
			}
		}
		else if (mrf == 'f') {	//if player folds, the other player wins
			printf("\n%s wins the round!\n", p1->playerName);
			printf("%s has $%d\n", p1->playerName, p1->money);
			printf("%s has $%d\n", p2->playerName, p2->money);
			printf("\nNew round! Reshuffling deck!\n");
			}

		while (betsmatched == true && wantgametoend == false && p1Points != 31 && p2Points != 31) { 
			//START OF PLAYING. the bets for each player match and the loop restarts if somebody knocks or somebody gets 31
			char dg;
			char dg2;
			char kc;
			char kc2;
			int oneTwoThree;
			int oneTwoThree2;
			
			//START OF PLAYER 1'S TURN

			printf("\nIt is %s's turn\n", p1->playerName);
			printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));	//prints p1's points
			p1Points = PointsVal(p1->playerHand);	//makes sure p1's points are not 31
			printf("%s your hand is:\n", p1->playerName);
			PrintPlayerCards(p1);	//shows cards
			PrintDiscard(TNDiscard);	//shows top of discard
			printf("\n%s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p1->playerName);
			scanf(" %c%*c", &dg);
			
			while (dg != 'd' && dg != 'g') {	//makes sure input is either 'd' or 'g' for deck or discard
				printf("Invalid input. %s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p1->playerName);
				scanf(" %c%*c", &dg);
			}

			if (dg == 'd') {	//if pull from deck
				
				AddDiscard(&HN, &HNDiscard, &TNDiscard);	//i move the deck card straight to discard so i can copy it to the hand and then overwrite this card w/ new discard from an array card (to go from array to linked list)

				p1->playerHand[3] = *TNDiscard;	//fourth temporary element is the card from top of deck

				printf("Which card would you like to discard (1/2/3/4)?\n");
				PrintPlayerCards(p1);	//prints first 3 hand cards
				printf("(4) %s %d %s\n", p1->playerHand[3].suit, p1->playerHand[3].value, p1->playerHand[3].name);	//prints fourth card same as first three that is new from deck
				scanf("%d", &oneTwoThree);
				while (oneTwoThree != 1 && oneTwoThree != 2 && oneTwoThree != 3 && oneTwoThree != 4) {	//makes sure input is '1' '2' '3' or '4' or else repeat question
					printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
					scanf("%d", &oneTwoThree);
				}

				if (oneTwoThree == 1) {		//if we discard option 1
					*TNDiscard = p1->playerHand[0];		//discard top card is overwritten w/ info from array
					p1->playerHand[0] = p1->playerHand[3];		//deck card moves in to replace position of card that is discarded
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));	//checks new points total
					p1Points = PointsVal(p1->playerHand);	//makes sure points are not 31
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);

				}

				if (oneTwoThree == 2) {	//if we discard option 2
					*TNDiscard = p1->playerHand[1];
					p1->playerHand[1] = p1->playerHand[3];
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);
				}

				if (oneTwoThree == 3) {	//if we discard option 3
					*TNDiscard = p1->playerHand[2];
					p1->playerHand[2] = p1->playerHand[3];
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);
				}

				if (oneTwoThree == 4) {	//if we discard option 4, points stay the same and the discard top card stays the same since we moved it from deck previously
					*TNDiscard = p1->playerHand[3]; //useless. just to keep my thoughts in order.
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);					
				}

			}

			else if (dg == 'g') {	//if we pull from the discard pile
				//	everything is going to be the same as if we pulled from the deck, except we don't move a card from the deck to discard first

				p1->playerHand[3] = *TNDiscard;

				printf("Which card would you like to discard (1/2/3/4)?\n");
				PrintPlayerCards(p1);
				printf("(4) %s %d %s\n", p1->playerHand[3].suit, p1->playerHand[3].value, p1->playerHand[3].name);
				scanf("%d", &oneTwoThree);

				while (oneTwoThree != 1 && oneTwoThree != 2 && oneTwoThree != 3 && oneTwoThree != 4) {
					printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
					scanf("%d", &oneTwoThree);
				}

				if (oneTwoThree == 1) {
					*TNDiscard = p1->playerHand[0];
					p1->playerHand[0] = p1->playerHand[3];
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);
				}

				else if (oneTwoThree == 2) {
					*TNDiscard = p1->playerHand[1];
					p1->playerHand[1] = p1->playerHand[3];
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);
				}

				else if (oneTwoThree == 3) {
					*TNDiscard = p1->playerHand[2];
					p1->playerHand[2] = p1->playerHand[3];
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);
				}

				else if (oneTwoThree == 4) {
					*TNDiscard = p1->playerHand[3]; //useless. just to keep my thoughts in order.
					printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
					p1Points = PointsVal(p1->playerHand);
					printf("%s, your hand is now:\n", p1->playerName);
					PrintPlayerCards(p1);
					PrintDiscard(TNDiscard);
				}


			}

			// END OF PLAYER 1'S TURN

			//START OF PLAYER 2'S TURN. exact same as player 1's turn
			printf("\nIt is %s's turn\n", p2->playerName);
			printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
			p2Points = PointsVal(p2->playerHand);
			printf("%s your hand is:\n", p2->playerName);
			PrintPlayerCards(p2);
			PrintDiscard(TNDiscard);
			printf("\n%s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p2->playerName);
			scanf(" %c%*c", &dg2);

			while (dg2 != 'd' && dg2 != 'g') {
				printf("Invalid input. %s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p2->playerName);
				scanf(" %c%*c", &dg2);
			}

			if (dg2 == 'd') {

				AddDiscard(&HN, &HNDiscard, &TNDiscard);

				p2->playerHand[3] = *TNDiscard;

				printf("Which card would you like to discard (1/2/3/4)?\n");
				PrintPlayerCards(p2);
				printf("(4) %s %d %s\n", p2->playerHand[3].suit, p2->playerHand[3].value, p2->playerHand[3].name);
				scanf("%d", &oneTwoThree2);
				while (oneTwoThree2 != 1 && oneTwoThree2 != 2 && oneTwoThree2 != 3 && oneTwoThree2 != 4) {
					printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
					scanf("%d", &oneTwoThree2);
				}

				if (oneTwoThree2 == 1) {
					*TNDiscard = p2->playerHand[0];
					p2->playerHand[0] = p2->playerHand[3];
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
					}

				if (oneTwoThree2 == 2) {
					*TNDiscard = p2->playerHand[1];
					p2->playerHand[1] = p2->playerHand[3];
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}

				if (oneTwoThree2 == 3) {
					*TNDiscard = p2->playerHand[2];
					p2->playerHand[2] = p2->playerHand[3];
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}

				if (oneTwoThree2 == 4) {
					*TNDiscard = p2->playerHand[3]; //useless. just to keep my thoughts in order.
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}

			}

			else if (dg2 == 'g') {
				//make a temp place to store the grave. 				
				p2->playerHand[3] = *TNDiscard;

				printf("Which card would you like to discard (1/2/3/4)?\n");
				PrintPlayerCards(p2);
				printf("(4) %s %d %s\n", p2->playerHand[3].suit, p2->playerHand[3].value, p2->playerHand[3].name);
				scanf("%d", &oneTwoThree2);

				while (oneTwoThree2 != 1 && oneTwoThree2 != 2 && oneTwoThree2 != 3 && oneTwoThree2 != 4) {
					printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
					scanf("%d", &oneTwoThree2);
				}

				if (oneTwoThree2 == 1) {
					*TNDiscard = p2->playerHand[0];
					p2->playerHand[0] = p2->playerHand[3];
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}

				else if (oneTwoThree2 == 2) {
					*TNDiscard = p2->playerHand[1];
					p2->playerHand[1] = p2->playerHand[3];
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}

				else if (oneTwoThree2 == 3) {
					*TNDiscard = p2->playerHand[2];
					p2->playerHand[2] = p2->playerHand[3];
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}

				else if (oneTwoThree2 == 4) {
					*TNDiscard = p2->playerHand[3]; //useless. just to keep my thoughts in order.
					printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
					p2Points = PointsVal(p2->playerHand);
					printf("%s, your hand is now:\n", p2->playerName);
					PrintPlayerCards(p2);
					PrintDiscard(TNDiscard);
				}


			}

			// END OF PLAYER 2'S TURN

			printf("%s, would you like to knock or continue (k/c)?\n", p1->playerName);	//after both players have one turn, p1 chooses to knock or continue
			scanf(" %c%*c", &kc);

			while (kc != 'k' && kc != 'c') {	//makes sure input is 'k' or 'c' or else repeats question
				printf("Invalid input. %s, would you like to knock or continue (k/c)?\n", p1->playerName);
				scanf(" %c%*c", &kc);
			}

			if (kc == 'k') {	//if p1 knocks, p2 has one more turn to improve their hand
				printf("Round ending. %s has one more turn\n", p2->playerName);
				wantgametoend = true;	//allows us to leave loop
				//ALL THIS TABBED STUFF IS JUST ONE MORE ROUND COPY AND PASTED FROM MY ROUND FOR PLAYER 2
									printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
									p2Points = PointsVal(p2->playerHand);
									printf("%s your hand is:\n", p2->playerName);
									PrintPlayerCards(p2);
									PrintDiscard(TNDiscard);
									printf("\n%s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p2->playerName);
									scanf(" %c%*c", &dg2);

									while (dg2 != 'd' && dg2 != 'g') {
										printf("Invalid input. %s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p2->playerName);
										scanf(" %c%*c", &dg2);
									}

									if (dg2 == 'd') {

										AddDiscard(&HN, &HNDiscard, &TNDiscard);

										p2->playerHand[3] = *TNDiscard;

										printf("Which card would you like to discard (1/2/3/4)?\n");
										PrintPlayerCards(p2);
										printf("(4) %s %d %s\n", p2->playerHand[3].suit, p2->playerHand[3].value, p2->playerHand[3].name);
										scanf("%d", &oneTwoThree2);
										while (oneTwoThree2 != 1 && oneTwoThree2 != 2 && oneTwoThree2 != 3 && oneTwoThree2 != 4) {
											printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
											scanf("%d", &oneTwoThree2);
										}

										if (oneTwoThree2 == 1) {
											*TNDiscard = p2->playerHand[0];
											p2->playerHand[0] = p2->playerHand[3];
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

										if (oneTwoThree2 == 2) {
											*TNDiscard = p2->playerHand[1];
											p2->playerHand[1] = p2->playerHand[3];
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

										if (oneTwoThree2 == 3) {
											*TNDiscard = p2->playerHand[2];
											p2->playerHand[2] = p2->playerHand[3];
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

										if (oneTwoThree2 == 4) {
											*TNDiscard = p2->playerHand[3]; //useless. just to keep my thoughts in order.
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

									}

									else if (dg2 == 'g') {
										//make a temp place to store the grave. 				
										p2->playerHand[3] = *TNDiscard;

										printf("Which card would you like to discard (1/2/3/4)?\n");
										PrintPlayerCards(p2);
										printf("(4) %s %d %s\n", p2->playerHand[3].suit, p2->playerHand[3].value, p2->playerHand[3].name);
										scanf("%d", &oneTwoThree2);

										while (oneTwoThree2 != 1 && oneTwoThree2 != 2 && oneTwoThree2 != 3 && oneTwoThree2 != 4) {
											printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
											scanf("%d", &oneTwoThree2);
										}

										if (oneTwoThree2 == 1) {
											*TNDiscard = p2->playerHand[0];
											p2->playerHand[0] = p2->playerHand[3];
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

										else if (oneTwoThree2 == 2) {
											*TNDiscard = p2->playerHand[1];
											p2->playerHand[1] = p2->playerHand[3];
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

										else if (oneTwoThree2 == 3) {
											*TNDiscard = p2->playerHand[2];
											p2->playerHand[2] = p2->playerHand[3];
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}

										else if (oneTwoThree2 == 4) {
											*TNDiscard = p2->playerHand[3]; //useless. just to keep my thoughts in order.
											printf("\n%s's points are: %d\n", p2->playerName, PointsVal(p2->playerHand));
											p2Points = PointsVal(p2->playerHand);
											printf("%s, your hand is now:\n", p2->playerName);
											PrintPlayerCards(p2);
											PrintDiscard(TNDiscard);
										}


									}

			}
			else {
				printf("%s, would you like to knock or continue (k/c)?\n", p2->playerName);	//now asks player 2 if they want to knock or continue
				scanf(" %c%*c", &kc);

				while (kc != 'k' && kc != 'c') {
					printf("Invalid input. %s, would you like to knock or continue (k/c)?\n", p2->playerName);
				}

				if (kc == 'k') {	//if player2 chooses to knock, player 1 gets one more round to try and improve hand
					printf("Round ending. %s has one more turn\n", p1->playerName);
					wantgametoend = true;
					//ALL THE TABBED STUFF IS A COPY AND PASTE OF ONE MORE TURN FOR PLAYER 1
												printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
												p1Points = PointsVal(p1->playerHand);
												printf("%s your hand is:\n", p1->playerName);
												PrintPlayerCards(p1);
												PrintDiscard(TNDiscard);
												printf("\n%s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p1->playerName);
												scanf(" %c%*c", &dg);

												while (dg != 'd' && dg != 'g') {
													printf("Invalid input. %s, do you want to pull from the deck(d) or pull from the discard(g)?\n", p1->playerName);
													scanf(" %c%*c", &dg);
												}

												if (dg == 'd') {

													AddDiscard(&HN, &HNDiscard, &TNDiscard);

													p1->playerHand[3] = *TNDiscard;

													printf("Which card would you like to discard (1/2/3/4)?\n");
													PrintPlayerCards(p1);
													printf("(4) %s %d %s\n", p1->playerHand[3].suit, p1->playerHand[3].value, p1->playerHand[3].name);
													scanf("%d", &oneTwoThree);
													while (oneTwoThree != 1 && oneTwoThree != 2 && oneTwoThree != 3 && oneTwoThree != 4) {
														printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
														scanf("%d", &oneTwoThree);
													}

													if (oneTwoThree == 1) {
														*TNDiscard = p1->playerHand[0];
														p1->playerHand[0] = p1->playerHand[3];
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);

													}

													if (oneTwoThree == 2) {
														*TNDiscard = p1->playerHand[1];
														p1->playerHand[1] = p1->playerHand[3];
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}

													if (oneTwoThree == 3) {
														*TNDiscard = p1->playerHand[2];
														p1->playerHand[2] = p1->playerHand[3];
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}

													if (oneTwoThree == 4) {
														*TNDiscard = p1->playerHand[3]; //useless. just to keep my thoughts in order.
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}

												}

												else if (dg == 'g') {
													//make a temp place to store the grave. 				
													p1->playerHand[3] = *TNDiscard;

													printf("Which card would you like to discard (1/2/3/4)?\n");
													PrintPlayerCards(p1);
													printf("(4) %s %d %s\n", p1->playerHand[3].suit, p1->playerHand[3].value, p1->playerHand[3].name);
													scanf("%d", &oneTwoThree);

													while (oneTwoThree != 1 && oneTwoThree != 2 && oneTwoThree != 3 && oneTwoThree != 4) {
														printf("Invalid input. Which card would you like to discard (1/2/3/4)?\n");
														scanf("%d", &oneTwoThree);
													}

													if (oneTwoThree == 1) {
														*TNDiscard = p1->playerHand[0];
														p1->playerHand[0] = p1->playerHand[3];
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}

													else if (oneTwoThree == 2) {
														*TNDiscard = p1->playerHand[1];
														p1->playerHand[1] = p1->playerHand[3];
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}

													else if (oneTwoThree == 3) {
														*TNDiscard = p1->playerHand[2];
														p1->playerHand[2] = p1->playerHand[3];
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}

													else if (oneTwoThree == 4) {
														*TNDiscard = p1->playerHand[3]; //useless. just to keep my thoughts in order.
														printf("\n%s's points are: %d\n", p1->playerName, PointsVal(p1->playerHand));
														p1Points = PointsVal(p1->playerHand);
														printf("%s, your hand is now:\n", p1->playerName);
														PrintPlayerCards(p1);
														PrintDiscard(TNDiscard);
													}


												}



				}
				else {	//if player 2 also chooses 'c' to continue, they both play another round and get asked about knock again
					printf("Both players choose to continue. Next turn\n");
				}
			}

		}
		if (p2Points == p1Points) {	//for the case of the tie, the money stays the same and we start a new round
			printf("\n%s and %s tied the round!\n", p1->playerName, p2->playerName);
			printf("%s has $%d\n", p1->playerName, p1->money);	//since money would be split, money just stays the same so we don't add or subtract
			printf("%s has $%d\n", p2->playerName, p2->money);
			printf("\nNew round! Reshuffling deck!\n");
		}

		else if (p1Points == 31) {	//for the case of p1 auto won because they hit 31
			p1->money = p1->money + p1bet;	//p1 wins the bet
			p2->money = p2->money - p1bet;	//p2 loses the bet
			printf("\n%s wins the round!\n", p1->playerName);
			printf("%s has $%d\n", p1->playerName, p1->money);
			printf("%s has $%d\n", p2->playerName, p2->money);
			printf("\nNew round! Reshuffling deck!\n");			
		}
		else if (p2Points == 31) {	//for the case of p2 auto won because they hit 31
			p2->money = p2->money + p1bet;
			p1->money = p1->money - p1bet;
			printf("\n%s wins the round!\n", p2->playerName);
			printf("%s has $%d\n", p1->playerName, p1->money);
			printf("%s has $%d\n", p2->playerName, p2->money);
			printf("\nNew round! Reshuffling deck!\n");
		}
		else if (p1Points > p2Points) {	//if p1 has more points than p2, p1 wins
			p1->money = p1->money + p1bet;
			p2->money = p2->money - p1bet;
			printf("\n%s wins the round!\n", p1->playerName);
			printf("%s has $%d\n", p1->playerName, p1->money);
			printf("%s has $%d\n", p2->playerName, p2->money);
			printf("\nNew round! Reshuffling deck!\n");
		}
		else if (p2Points > p1Points) {	//if p2 has more points than p1, p2 wins
			p2->money = p2->money + p1bet;
			p1->money = p1->money - p1bet;
			printf("\n%s wins the round!\n", p2->playerName);
			printf("%s has $%d\n", p1->playerName, p1->money);
			printf("%s has $%d\n", p2->playerName, p2->money);
			printf("\nNew round! Reshuffling deck!\n");
		}
		
		//the start of the closing of the round

		AddDiscard(&HN, &HNDiscard, &TNDiscard); //the following for loop to move discards to deck works when more than 1 card in the discard
		for (int i = 0; i < LinkListLength(HNDiscard); i++) {
			DiscardToDeck(&HN, &HNDiscard, &TN);//adding all of the discard cards back into the main deck
		}
	//set the heads of both linked lists to null so that we can build new shuffled linked list starting at HN		
	HN = NULL;
	HNDiscard = NULL;
	round++;

	}
	if (p2->money <= 0) {	//if p2 has no money so p1 wins
		printf("Nevermind. Not new round...\n");
		printf("\n\n%s WINS THE GAME!!!\n", p1->playerName);
	}
	else if (p1->money <= 0) {	//if p1 has no money so p2 wins
		printf("Nevermind. Not new round...\n");
		printf("\n\n%s WINS THE GAME!!!\n", p2->playerName);
	}
	//THIS IS WHERE I FREE ALL OF MY LINKED LISTS. FREE THE MAIN DECK AND THEN THE DISCARD DECK.
	BuildLinkList(&HN, &TN);
	for (int i = 0; i < 5000; i++) {//i rebuild the deck like at the beginning of every round just so its easy to make a for loop to free the linked list
		CardSwap(HN, rand_gen(LinkListLength(HN)), rand_gen(LinkListLength(HN)));
	}

	AddDiscard(&HN, &HNDiscard, &TNDiscard); //just makes sure theres only one card in the discard linked list so i just free it once w/ no function

	for (int i = 0; i < 51; i++) {//goes through all of the cards (except the last one) and frees them
		card* temp = HN->pt;
		free(HN);
		HN = temp;
	}
	free(HN); //frees the final remaining element in the deck linked list
	free(HNDiscard); //there is only one card in the discard, so one free function should 
	
	return 0;
}

void PrintPlayerCards(player* p) {//a function to easily print the hand array to display which cards each player has
	printf("(1) %s %d %s\n", p->playerHand[0].suit, p->playerHand[0].value, p->playerHand[0].name);
	printf("(2) %s %d %s\n", p->playerHand[1].suit, p->playerHand[1].value, p->playerHand[1].name);
	printf("(3) %s %d %s\n", p->playerHand[2].suit, p->playerHand[2].value, p->playerHand[2].name);
}


void AddDiscard(card** HN, card** HNDiscard, card** TNDiscard) {//this function adds a card from the deck linked list to the discard linked list
	card* temp = *HN;
	*HN = (*HN)->pt;

	if (*HNDiscard == NULL) {//if there are no cards in the discard linked list, it will add one
		*HNDiscard = temp;
	}
	else {
		(*TNDiscard)->pt = temp;//if there are cards present in the discard LL, it will add one from the deck to the discard tail/"top of deck"
	}
	*TNDiscard = temp;
	temp->pt = NULL;

}

void DiscardToDeck(card** HN, card** HNDiscard, card** TN) {//using a for loop this function moves discard cards to back to the deck linked list
	card* temp = *HNDiscard;
	(*TN)->pt = temp;
	*HNDiscard = (*HNDiscard)->pt;
	*TN = temp;
	(*TN)->pt = NULL;
} 

int rand_gen(int count) {//from Lab 12
	double frac;
	frac = (double)rand() / ((double)RAND_MAX + 1); // gives a different frac every time is executed
	return floor(count * frac); //random index in [0, max]
}

void CardSwap(card* HN, int i, int j) {//based off of LAB 12. This is also the function used to shuffle cards by swapping random card #s around in loops
	card* temp1 = HN, * temp2 = HN, temp3;

	//the for loops swap the (i) and (j) elements
	while (i > 0) { //McBride gave a simple example of this code
		temp1 = temp1->pt;
		i--;
	}
	while (j > 0) {
		temp2 = temp2->pt;
		j--;
	}
	temp3.value = temp1->value; //swapping the values around for cards
	temp1->value = temp2->value;
	temp2->value = temp3.value; //temp 3 does not matter at end, just placeholder

	strcpy(temp3.suit, temp1->suit); //swapping the suits around for the cards
	strcpy(temp1->suit, temp2->suit);
	strcpy(temp2->suit, temp3.suit);

	strcpy(temp3.name, temp1->name); //swapping the names around for the cards
	strcpy(temp1->name, temp2->name);
	strcpy(temp2->name, temp3.name);
}

void BuildLinkList(card** HN, card** TN) {//allows me to scan in the deck file, allocate memory, and make linked list for the deck
	card* temp = NULL;
	FILE* inp;
	char cardinfo[10];
	inp = fopen("Deck.txt", "r"); //opens the file containing 52 playing cards
	assert(inp != NULL); //make sure the deck file opens correctly
	//derived from singly linked lists video
	while (!feof(inp)) {
		temp = (card*)malloc(sizeof(card)); //memory allocation

		fscanf(inp, "%s", temp->suit);//scans in elements of the cards
		fscanf(inp, "%d", &temp->value);
		fscanf(inp, "%s", temp->name);

		if (*HN == NULL) {//for first card
			(*HN) = temp;
		}
		else {
			(*TN)->pt = temp;//for cards not the first
		}

		temp->pt = NULL;
		*TN = temp;
	}
	fclose(inp);
}

int LinkListLength(card* HN) {//function to count length of link list
	int length = 0;

	while (NULL != HN) {
		HN = HN->pt;//head pointer moves on and adds 1 to the count until it is at the end of LL
		length += 1;
	}
	return length;
}

void PrintList(card* HN) { //derived from linked lists slides
	card* temp;
	temp = HN;
	printf("\n");

	while (temp != NULL) {//until we are at the end of the linked list
		printf("%s %d %s\n", temp->suit, temp->value, temp->name);//print the card elements as they appear in the text document
		temp = temp->pt;
	}
}

void PrintDiscard(card* HN) { //i copied the printlist function and just removed the loop to reveal just one card so I could show just the top card for the discard.
	card* temp;
	temp = HN;
	printf("\n");

	printf("The top card of the discard pile is:\n%s %d %s\n", temp->suit, temp->value, temp->name);
}

int PointsVal(card playerHand[4]){//allows me to add up the sums
int points = 0;

int check[4];//makes array check w/ all elements set to 0
for (int i = 0; i < 4; i++) {
	check[i] = 0;
}
  for (int i = 0; i < 3; i++){//checks if the suits are the same
      if (!strcmp(playerHand[i].suit, "clubs")){
        check[0] = check[0] + playerHand[i].value;
      }
      else if (!strcmp(playerHand[i].suit, "diamonds")){
        check[1] = check[1] + playerHand[i].value;
      }
      else if (!strcmp(playerHand[i].suit, "hearts")){
        check[2] = check[2] + playerHand[i].value;
      }
      else if (!strcmp(playerHand[i].suit, "spades")){
        check[3] = check[3] + playerHand[i].value;
      }
  }
  points = check[0];//points is set to the first club suit as a starting point

  for (int i = 0; i < 4; i++){//chooses the highest suit sum as points value
    if (points < check[i]){
       points = check[i];
    }
  }
if (playerHand[0].value == playerHand[1].value && playerHand[1].value == playerHand[2].value) {//if there are three cards with the same value, the points are 30
		points = 30;
}
return points;//returning points allows me to use the points as an integer for printing or comparing scores
}