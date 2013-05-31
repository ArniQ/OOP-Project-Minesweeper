#include "Minesweeper.h"
#include <iostream>
using namespace std;

const string Minesweeper::validCommands[14] = {"open", "flag", "unflag", "menu", "restart", "scoreboard", "help", "exit", "game", "1", "2", "3", "4", "5"};

Minesweeper::Minesweeper(): playingField(0, 0, 0)
{
	state = UNINITIALIZED;
	action = &Minesweeper::drawMenu;
}

void Minesweeper::play()
{
	string command = "menu";

	while (state != EXIT)
	{
		enterCommand(command, action);
		executeCommand(command);
		playingField.update();

		if (state == INITIALIZED)
		{
			bool youWin = winCondition();
			bool youLose = lossCondition();
			
			if (youWin || youLose)
			{
				playingField.detonateAllMines();
				drawMineField();

				if (youWin)
				{
					drawWinMessage();
				}
				else
				{
					drawLossMessage();
				}

				state = PENDING;
				updateScoreboard();
				action = &Minesweeper::drawScoreboard;
				system("pause");
			}
		}
	}
}

void Minesweeper::executeExceptions()
{
	cout << endl;
	while (!raisedExceptions.isEmpty())
	{
		cout << raisedExceptions.back() << endl;
		raisedExceptions.pop_back();
	}
}

bool Minesweeper::validateCommand(string command) const
{
	for (int i = 0; i < 14; i++)
	{
		if (command == validCommands[i])
		{
			return 1;
		}
	}

	return 0;
}

void Minesweeper::enterCommand(string &command, void (Minesweeper::*currentCommand)() const)
{
	bool isValid;

	do
	{
		(this->*currentCommand)();
		executeExceptions();

		cout << "\nEnter command(type 'help' for more info): ";
		cin >> command;

		isValid = validateCommand(command);
		if (!isValid)
		{
			raisedExceptions.addElement("Invalid command!");
		}
	}
	while(!isValid);
}

void Minesweeper::executeCommand(string command)
{
	if (command == "help")
	{
		action = &Minesweeper::drawHelp;
	}
	else if (command == "exit")
	{
		state = EXIT;
	}
	else if (command == "scoreboard")
	{
		action = &Minesweeper::drawScoreboard;
	}
	else if (command == "restart")
	{
		if (state == UNINITIALIZED)
		{
			raisedExceptions.addElement("There is no initialized game!");
			return;
		}
		state = INITIALIZED;
		restart();
		action = &Minesweeper::drawMineField;
	}
	else if (command == "menu")
	{
		action = &Minesweeper::drawMenu;
	}
	else if (command == "game")
	{
		if (state == UNINITIALIZED)
		{
			raisedExceptions.addElement("There is no initialized game!");
			return;
		}
		action = &Minesweeper::drawMineField;
	}
	else if (action == &Minesweeper::drawMenu)
	{
		if (command == "1")
		{
			state = INITIALIZED;
			action = &Minesweeper::drawMineField;
			playingField = MineField(9, 9, 10);
		}
		else if (command == "2")
		{
			state = INITIALIZED;
			action = &Minesweeper::drawMineField;
			playingField = MineField(16, 16, 40);
		}
		else if (command == "3")
		{
			state = INITIALIZED;
			action = &Minesweeper::drawMineField;
			playingField = MineField(16, 30, 99);
		}
		else if (command == "4")
		{
			int height, width, mines;

			cout << endl;
			do
			{
				if (!cin)
					flushingCin();

				cout << "Enter field height: ";
				cin >> height;
			}
			while (!cin);
	
			do
			{
				if (!cin)
					flushingCin();

				cout << "Enter field width: ";
				cin >> width;
			}
			while (!cin);

			do
			{
				if (!cin)
					flushingCin();

				cout << "Enter number of mines(0-" << width * height <<"): ";
				cin >> mines;
			}
			while (!cin || mines > width * height);

			state = INITIALIZED;
			action = &Minesweeper::drawMineField;
			playingField = MineField(height, width, mines);
		}
		else if (command == "5")
		{
			state = EXIT;
		}
	}
	else 
	{
		if (state == INITIALIZED)
		{
			int row, column;
			if (command == "flag" || command == "unflag" || command == "open")
			{
				drawMineField();

				do
				{
					cout << "\nEnter row and column: ";
					if (!cin)
					{
						flushingCin();
					}
					cin >> row >> column;
				}
				while(!cin || row < 0 || row > playingField.getHeight() || column < 0 || column > playingField.getWidth());
			}

			if (command == "open")
			{
				if (playingField.getCellStatus(row, column) == OPEN)
				{
					raisedExceptions.addElement("The cell is already open!");
				}
				else if (playingField.getCellStatus(row, column) == FLAGGED)
				{
					raisedExceptions.addElement("Cannot open flagged cell!");
				}
				else
				{
					playingField.openCell(row, column);
				}
			}
			else if (command == "flag")
			{
				if (playingField.getCellStatus(row, column) == OPEN)
				{
					raisedExceptions.addElement("Cannot flag open cell!");
				}
				else if (playingField.getCellStatus(row, column) == FLAGGED)
				{
					raisedExceptions.addElement("The cell is already flagged");
				}
				else
				{
					playingField.flagCell(row, column);
				}
			}
			else if (command == "unflag")
			{
				if (playingField.getCellStatus(row, column) != FLAGGED)
				{
					raisedExceptions.addElement("You can only unflag flagged cells!");
				}
				else 
				{
					playingField.unflagCell(row, column);
				}
			}
		}
		else if (state == PENDING && (command == "open" || command == "flag" || command == "unflag"))
		{
			raisedExceptions.addElement("The game has already finished!");
		}
	}

}

int Minesweeper::getCurrentScore() const
{
	int currentScore = 0;
	for (int i = 0; i < playingField.getHeight(); i++)
	{
		for (int j = 0; j < playingField.getWidth(); j++)
		{
			if ((playingField.getCellStatus(i, j) == OPEN || playingField.getCellStatus(i, j) == FLAGGED) &&
				playingField.getCellValue(i, j) != -1)
			{
				currentScore++;
			}
		}
	}

	return currentScore;
}

int Minesweeper::getLongestName() const
{
	int longestName = 0;
	int playerNumber = scores.getSize();

	for (int i = 0; i < playerNumber; i++)
	{
		if (longestName < scores[i].playerName.length())
		{
			longestName = scores[i].playerName.length();
		}
	}

	return longestName;
}

void Minesweeper::updateScoreboard()
{
	int currentScore = getCurrentScore();
	
	if (scores.getSize() >= 10)
	{
		if (currentScore > sortedScores.back()->score)
		{
			for (int i = 0; i < scores.getSize(); i++)
			{
				if (&scores[i] == sortedScores.back())
				{
					scores.removeElement(i);
					break;
				}
			}
		}
		else
		{
			return;
		}
	}

	string playerName;
	do
	{
		if (!cin)
		{
			flushingCin();
		}
		cout << "\nPlease enter your name for the scoreboard: ";
		cin >> playerName;
	}
	while(!cin);

	Score newScore = {currentScore, playerName};
	scores.addElement(newScore);
	
	sortedScores.clear();
	for (int i = 0; i < scores.getSize(); i++)
	{
		sortedScores.addElement(&scores[i]);
	}

	sortScores();
}

void Minesweeper::sortScores()
{
	int vecSize = sortedScores.getSize();

	for (int i = 0; i < vecSize - 1; i++)
	{
		int max = i;
		for (int j = i + 1; j < vecSize; j++)
		{
			if (sortedScores[max]->score < sortedScores[j]->score)
			{
				max = j;
			}
		}

		if (max != i)
		{
			swap(sortedScores[max], sortedScores[i]);
		}
	}
}

void Minesweeper::drawMenu() const
{
	system("cls");

	cout << "=============MENU=============\n";
	cout << "|1  - Beginner(9x9)          |\n";
	cout << "|2  - Intermediate(16x16)    |\n";
	cout << "|3  - Advanced(16x30)        |\n";
	cout << "|4  - Custom                 |\n";
	cout << "|5  - Quit                   |\n";
	cout << "==============================\n";
}

void Minesweeper::drawMineField() const
{
	system("cls");
	playingField.draw();
}

void Minesweeper::drawHelp() const
{
	system("cls");
	cout << "================================================================================";
	cout << "Command        Descripstion                                                     ";
	cout << "================================================================================";
	cout << "'open'       - Open cell                                          \n";
	cout << "'flag'       - Flag cell                                          \n";
	cout << "'unflag'     - Unflag cell                                        \n";
	cout << "'help'       - Go to help menu                                    \n";
	cout << "'menu'       - Go to main menu                                    \n";
	cout << "'game'       - Go to the game board                               \n";
	cout << "'scoreboard' - Show the scoreboard                                \n";
	cout << "'restart'    - Restart current game                               \n";
	cout << "'exit'       - Quit the game                                      \n";
	cout << "'1'          - Choose Beginner mode, while your in  the main menu \n";
	cout << "'2'          - Choose Intermediate mode, while in the main menu   \n";
	cout << "'3'          - Choose Advanced mode, while in the main menu       \n";
	cout << "'4'          - Choose Custom mode, while in the main menu         \n";
	cout << "'5'          - Quit the game, while in the main menu              \n";
	cout << "================================================================================";
}

void Minesweeper::drawScoreboard() const
{
	system("cls");
	
	int scoreNumber = sortedScores.getSize();
	int longestName = getLongestName();
	
	cout << "===================================SCOREBOARD===================================";

	for (int i = 0; i < scoreNumber; i++)
	{
		cout << i + 1 << ". ";
		if (i < 9)
		{
			cout << " "; 
		}
		cout << sortedScores[i]->playerName;

		int currentNameLength = sortedScores[i]->playerName.length();
		for (int j = 0; j < longestName - currentNameLength; j++)
		{
			cout << " ";
		}

		cout << " --> ";
		cout << sortedScores[i]->score << (sortedScores[i]->score == 1? " cell":" cells");
		cout << endl;
	}

	cout << "================================================================================\n";
}

void Minesweeper::drawLossMessage() const
{
	cout << "\n\nYou Lose!\n";
	cout << "You revealed " << getCurrentScore() << (getCurrentScore()==1? " cell.":" cells.") << endl;
}

void Minesweeper::drawWinMessage() const
{
	cout << "\n\nYou Win!\n";
	int cells = playingField.getHeight() * playingField.getWidth();
	cout << "You revealed " << cells << (cells == 1? "cell.":"cells.") << endl;
}

void Minesweeper::flushingCin() const
{
	cin.clear(); 
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

void Minesweeper::restart()
{
	playingField.closeAllCells();
}

bool Minesweeper::lossCondition() const
{
	for (int i = 0; i < playingField.getHeight(); i++)
	{
		for (int j = 0; j < playingField.getWidth(); j++)
		{
			if (playingField.getCellStatus(i, j) == OPEN && playingField.getCellValue(i, j) == -1)
			{
				return 1;
			}
		}
	}

	return 0;
}

bool Minesweeper::winCondition() const
{
	int cells = 0;
	for (int i = 0; i < playingField.getHeight(); i++)
	{
		for (int j = 0; j < playingField.getWidth(); j++)
		{
			if ((playingField.getCellStatus(i, j) == CLOSED || playingField.getCellStatus(i, j) == FLAGGED) &&
				playingField.getCellValue(i, j) != -1)
			{
				return 0;
			}
		}
	}
	
	return 1;
}