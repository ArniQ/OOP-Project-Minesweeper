#ifndef MINESWEEPER_H
#define MINESWEEPER_H

#include "MineField.h"

struct Score
{
	int score;
	string playerName;
};

class Minesweeper
{
public:
	Minesweeper();

	void play();

private:
	const static string validCommands[14];
	
	enum {UNINITIALIZED, INITIALIZED, PENDING, EXIT} state;
	MineField playingField;
	void (Minesweeper::*action)() const;

	Vector<Score> scores;
	Vector<Score*> sortedScores;
	Vector<char*> raisedExceptions;

	int getCurrentScore() const;
	int getLongestName() const;
	void updateScoreboard();
	void sortScores();

	void drawMenu() const;
	void drawMineField() const;
	void drawHelp() const;
	void drawScoreboard() const;
	void drawLossMessage() const;
	void drawWinMessage() const;

	void restart();

	bool validateCommand(string) const;
	void enterCommand(string&, void (Minesweeper::*)() const);
	void executeCommand(string);
	void executeExceptions();

	void flushingCin() const;

	bool winCondition() const;
	bool lossCondition() const;
};

#endif