#include "PegSolitia.hpp"

void PegSolitia::Board::randomInit()
{
	for (auto i : step(board.size())) {
		board.at(i) = Hole(i * 70 + Size{ 170, 150 }, (i.x % 6) >= 3 || (i.y % 6) >= 3, RandomBool());
	}

	moveNum = 0;
	history.clear();
	history << board;

	return;
}

void PegSolitia::Board::randomInitEasy()
{
	for (auto i : step(board.size())) {
		board.at(i) = Hole(i * 70 + Size{ 170, 150 }, (i.x % 7) >= 2 && (i.y % 7) >= 2, false);
	}

	board.at(4, 4).isPeg = true;

	for (int count = 0; count < 100000; count++) {
		int32 x = Random(0, board.size().x - 1);
		int32 y = Random(0, board.size().y - 1);

		if (isIn(x, y) && isPegSide(Point{ x,y })) {
			int dxy[] = { 0,1,0,-1,0 };

			for (int i = 0; i < 4; i++) {
				if (isIn(x + dxy[i], y + dxy[i + 1]) &&
					board.at(y + dxy[i + 1], x + dxy[i]).isPeg &&
					isIn(x - dxy[i], y - dxy[i + 1]) &&
					!board.at(y, x).isPeg &&
					!board.at(y - dxy[i + 1], x - dxy[i]).isPeg
					) {

					board.at(y + dxy[i + 1], x + dxy[i]).isPeg = false;
					board.at(y, x).isPeg = true;
					board.at(y - dxy[i + 1], x - dxy[i]).isPeg = true;

					/*
					int pegNum = 0;

					for (auto j : step(board.size())) {
						pegNum += board.at(j).isPeg;
					}

					Print << pegNum;
					*/

				}
			}
		}

	}

	moveNum = 0;
	history.clear();
	history << board;

	return;
}

void PegSolitia::Board::randomInitHard()
{

	for (auto i : step(board.size())) {
		board.at(i) = Hole(i * 70 + Size{ 170, 150 }, (i.x % 6) >= 3 || (i.y % 6) >= 3, false);
	}

	board.at(4, 4).isPeg = true;

	for (int count = 0; count < 100000; count++) {
		int32 x = Random(0, board.size().x - 1);
		int32 y = Random(0, board.size().y - 1);

		if (isIn(x, y) && isPegSide(Point{ x,y })) {
			int dxy[] = { 0,1,0,-1,0 };

			for (int i = 0; i < 4; i++) {
				if (isIn(x + dxy[i], y + dxy[i + 1]) &&
					board.at(y + dxy[i + 1], x + dxy[i]).isPeg &&
					isIn(x - dxy[i], y - dxy[i + 1]) &&
					!board.at(y, x).isPeg &&
					!board.at(y - dxy[i + 1], x - dxy[i]).isPeg
					) {

					board.at(y + dxy[i + 1], x + dxy[i]).isPeg = false;
					board.at(y, x).isPeg = true;
					board.at(y - dxy[i + 1], x - dxy[i]).isPeg = true;

					/*
					int pegNum = 0;

					for (auto j : step(board.size())) {
						pegNum += board.at(j).isPeg;
					}

					Print << pegNum;
					*/

				}
			}
		}

	}

	moveNum = 0;
	history.clear();
	history << board;

	return;
}

void PegSolitia::Board::randomInitHell()
{
	//‚±‚±‚Å‚Ç‚Ì—Ìˆæ‚ðŽg—p‚·‚é‚©Œˆ‚ß‚Ä‚¢‚éB
	//T,L,O,Y,U,S,C‚È‚Ç

	const int areaNum = Random(0, 5);


	const bool isArea[6][3][3] = {
		{
			{1,1,1},
			{0,1,0},
			{0,1,0}
		},
		{
			{1,1,1},
			{1,0,1},
			{1,1,1}
		},
		{
			{1,0,0},
			{1,0,0},
			{1,1,1}
		},
		{
			{1,0,1},
			{1,1,1},
			{0,1,0}
		},
		{
			{0,1,1},
			{0,1,0},
			{1,1,0}
		},
		{
			{1,0,1},
			{1,1,1},
			{1,0,1}
		}
	};

	for (auto i : step(board.size())) {
		board.at(i) = Hole(i * 70 + Size{ 170, 150 }, isArea[areaNum][(i.y / 3)][(i.x / 3)], false);
	}

	if (areaNum == 1)
		board.at(1, 4).isPeg = true;
	else if (areaNum == 2)
		board.at(7, 1).isPeg = true;
	else
		board.at(4, 4).isPeg = true;


	for (int count = 0; count < 100000; count++) {
		int32 x = Random(0, board.size().x - 1);
		int32 y = Random(0, board.size().y - 1);

		if (isIn(x, y) && isPegSide(Point{ x,y })) {
			int dxy[] = { 0,1,0,-1,0 };

			for (int i = 0; i < 4; i++) {
				if (isIn(x + dxy[i], y + dxy[i + 1]) &&
					board.at(y + dxy[i + 1], x + dxy[i]).isPeg &&
					isIn(x - dxy[i], y - dxy[i + 1]) &&
					!board.at(y, x).isPeg &&
					!board.at(y - dxy[i + 1], x - dxy[i]).isPeg
					) {

					board.at(y + dxy[i + 1], x + dxy[i]).isPeg = false;
					board.at(y, x).isPeg = true;
					board.at(y - dxy[i + 1], x - dxy[i]).isPeg = true;

					/*
					int pegNum = 0;

					for (auto j : step(board.size())) {
						pegNum += board.at(j).isPeg;
					}

					Print << pegNum;
					*/

				}
			}
		}

	}

	moveNum = 0;
	history.clear();
	history << board;

	return;
}

Line PegSolitia::Board::hint()
{

	ChokudaiSearch chokudai;

	bool result = chokudai.calculateOrder(*this);

	if (result == false) {
		return Line(Point(410, PegSolitia::WindowSize.y - 110), Point(410, PegSolitia::WindowSize.y - 60));
	}

	auto hint = chokudai.getHint();

	return Line(board[hint.first].pos, board[hint.second].pos);
}