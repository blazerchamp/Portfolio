// Crazy8.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Classes.h"
#include <list>

//Important note: SDL files have already been implemented in this project. Go straight into development

void game(Player& player1, Player& player2, SDL_Rect deck);
void setPlayerTurns(Player& player1, Player& player2);
void finishTurn(Player& player1, Player& player2, array<int, 8>& indexes, int& length);
void update(SDL_Renderer *renderer, SDL_Texture *background, SDL_Texture *deck, SDL_Rect rect, SDL_Rect dest);

bool change = false, submit = false, eight = false;
Player opponent;

array<int, 8> indexes;

int main(int argc, char **argv)
{
	//Initialize SDL window and images here, and load title screen. Implement classes Avatar, MouseInput, and Sounds
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL Error: " << SDL_GetError() << endl;
		return 1;
	}

	SDL_Window *window = SDL_CreateWindow("Crazy 8s", 100, 100, 640, 480, SDL_WINDOW_SHOWN);
	SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_Rect rect = { 0, 0, 100, 100 }, dest = { 200, 200, 100, 100 }, cardRect = { 0, 0, 75, 97 }, cardDest = { 0, 0, 75, 97 };
	SDL_Event event;

	if (!window) {
		cerr << "SDL_CreateWindow Error: " << SDL_GetError();
		return 1;
	}

	if (!renderer) {
		cerr << "SDL_CreateRenderer Error: " << SDL_GetError();
		return 1;
	}

	SDL_Texture *background = IMG_LoadTexture(renderer, "Images/background.jpg"), *cardPics = IMG_LoadTexture(renderer, "Images/cards.png"),
		*deck = IMG_LoadTexture(renderer, "Images/deck.png");

	if (!background) {
		cerr << "SDL_LoadImage Error: " << SDL_GetError();
		return 1;
	}

	if (!cardPics) {
		cerr << "SDL_LoadImage Error: " << SDL_GetError();
		return 1;
	}

	if (!deck) {
		cerr << "SDL_LoadImage Error: " << SDL_GetError();
		return 1;
	}

	update(renderer, background, deck, rect, dest);

	string message;
	SDL_Texture *playButton = IMG_LoadTexture(renderer, "Images/play.png"), 
		*titlescreen = IMG_LoadTexture(renderer, "Images/title-screen.png"),
		*instructions = IMG_LoadTexture(renderer, "Images/instructions.jpg");
	Player player1(renderer, playButton, IMG_LoadTexture(renderer, "Images/player1.png"), 300, 50, 350), player2(renderer, playButton, IMG_LoadTexture(renderer, "Images/player2.jpg"), 150, 500, 50);
	Game gameObject;
	Card::displayCardInField(new Card(renderer, 350, 200));
	player1.setIsTurn(true);
	bool play = false;

	for (int i = 0; i < indexes.size(); ++i) {
		indexes[i] = -1;
	}

	SDL_RenderCopy(renderer, titlescreen, NULL, NULL);
	SDL_RenderPresent(renderer);

	while (SDL_PollEvent(&event)) {
		switch (event.type) {
		case SDL_MOUSEBUTTONUP:
			if (event.button.button == SDL_BUTTON_LEFT) {

			}
		}
	}

	//Implement play button that starts the game once clicked. Once clicked, 

	//Checks if player 1 or 2 has gotten rid of all their cards
		while (player1.cardCount() != 0 && player2.cardCount() != 0) {
			if (!play) {
				while (SDL_PollEvent(&event)) {
					switch (event.type) {
					case SDL_MOUSEBUTTONUP:
						if (event.button.button == SDL_BUTTON_LEFT) {
							cout << "X: " << event.button.x << "\tY: " << event.button.y << endl;
							if (event.button.x >= 98
								&& event.button.x <= 396
								&& event.button.y >= 210
								&& event.button.y <= 237) {
								SDL_RenderClear(renderer);
								SDL_RenderCopy(renderer, instructions, NULL, NULL);
								SDL_RenderPresent(renderer);
							}

							else if (event.button.x >= 98
								&& event.button.x <= 396
								&& event.button.y >= 240
								&& event.button.y <= 255) {
								SDL_RenderClear(renderer);
								update(renderer, background, deck, rect, dest);

								for (int i = 0; i < 8; ++i) {
									player1.getCard(i)->draw();
									player2.getCard(i)->draw();
								}
								play = true;
							}
						}
					}
				}
			}

			else {
				player1.getIsTurn() ? game(player1, player2, dest) : game(player2, player1, dest);

				if (change) {
					update(renderer, background, deck, rect, dest);

					for (int i = 0; i < 8; ++i) {
						if (player1.getCard(i) != nullptr) {
							player1.getCard(i)->draw();
						}

						if (player2.getCard(i) != nullptr) {
							player2.getCard(i)->draw();
						}
					}
					change = false;
				}
			}
		}

		gameObject.WinLose(player1);

	//redraw window to update changes

	return 0;
}

//Runs through a player's turn by either selecting the cards they want to play or draw a card
void game(Player& player1, Player& player2, SDL_Rect deck) {

	SDL_RenderCopy(player1.renderer, player1.avatar, &player1.avSrc, &player1.avDest);
	SDL_RenderCopy(player1.renderer, player1.play, &player1.playSrc, &player1.playDest);
	SDL_RenderPresent(player1.renderer);
	opponent = player2;

	//replace the cin input system with mouse events instead
	//replace code from here
	/*cout << "How many cards do you want to play? (if you plan to draw just type '-1') ";
	cin >> count;*/
	/*cout << "Choice (type '-1' to draw): ";
	cin >> index;*/

	SDL_PumpEvents();

	//SDL_MOUSEBUTTONUP
	player1.checkInput(indexes, deck, player2, change, submit, eight);
	finishTurn(player1, player2, indexes, player1.count);

	//to here
	//setPlayerTurns(player1, player2);

	/*if (count == -1) {
		player1.drawCard();
		Card::displayCardInField(&Card::currentCard);
		//set drawn card in hand
	}

	else {
		Card* cards[8];
		player1.dropPairs(cards, indexes, count, index);

		if (cards[0]->compare(&Card::currentCard) || cards[0]->getSuit() == player2.getSuitCall()) {
			switch (cards[0]->getNum()) {
			case 'J':
				setPlayerTurns(player2, player1);
				//reset graphics to show skip
				break;
			case '2':
				player2.drawTwo();
				//set drawn card in hand
				break;
			case '8':
				//edit this method to include graphics
				player1.crazy8();
				break;
			}

			finishTurn(player1, player2, cards, indexes, count);
		}

		else {
			cout << "Invalid choice, pick a card again\n\n";
			setPlayerTurns(player2, player1);
		}
	}*/
}

//Transitioning player turns
void setPlayerTurns(Player& player1, Player& player2) {
	player1.setIsTurn(false);
	player2.setIsTurn(true);
}

//Finishes every turn by displaying a new card in field and removing said cards from the player's hand
//Edit this method to include the SDL properties to redraw window
void finishTurn(Player& player1, Player& player2, array<int, 8>& indexes, int& length) {
	if (length > 0 && submit) {
		Card* lastCard = player1.getCard(indexes[length - 1]);
		lastCard->setDestPos(350, 200);
		Card::displayCardInField(lastCard);

		for (int i = 0; i < length; ++i) {
			player1.removeCard(indexes[i]);
			indexes[i] = -1;
		}
		player2.clearSuitCall();
		length = 0;
		submit = false;
	}
}

void update(SDL_Renderer *renderer, SDL_Texture *background, SDL_Texture *deck, SDL_Rect rect, SDL_Rect dest) {
	SDL_RenderClear(renderer);
	SDL_RenderCopy(renderer, background, NULL, NULL);
	//SDL_RenderCopy(renderer, cardPics, &cardRect, &cardDest);
	SDL_RenderCopy(renderer, deck, &rect, &dest);
	Card::currentCard.draw();
	SDL_RenderPresent(renderer);

	if (eight) {
		SDL_RenderCopy(renderer, opponent.choice, &opponent.avSrc, &opponent.avDest);
		SDL_RenderPresent(renderer);
	}
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
