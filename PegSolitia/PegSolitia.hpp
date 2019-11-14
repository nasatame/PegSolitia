#pragma once
# include <Siv3D.hpp>
# include "Algorithm.hpp"

namespace PegSolitia {

	constexpr int32 HoleSize = 30;
	constexpr int32 PegSize = 25;
	constexpr Size WindowSize = Size(900, 900);

	enum class Difficulty : short {
		Easy = 0,
		Hard,
		Hell
	};

	class Hole {
	private:
		Circle hole;
		Circle peg;
	public:
		bool exist;
		bool isPeg;
		bool isClickedPeg;
		bool isClickedHole;
		Point pos;

		Hole(Point p, bool e, bool peg) : pos(p), exist(e), isPeg(peg) {
			hole = Circle(p, HoleSize);
			this->peg = Circle(p, PegSize);
			isClickedPeg = false;
			isClickedHole = false;

		}

		Hole() : Hole({ 0,0 }, false, false) {

		}

		void updata() {
			if (exist && hole.leftClicked() && isPeg) {
				isClickedPeg = true;
			}
			isClickedHole = false;
			if (exist && hole.leftClicked() && !isPeg) {
				isClickedHole = true;
			}
		}

		void draw() const {
			if (exist) {
				hole.draw(Palette::White);
				if (isClickedPeg) {
					if (isPeg)
						peg.draw(Palette::Aqua);
				}
				else {
					if (isPeg)
						peg.draw(Palette::Brown);
					else
						peg.draw(Palette::White);
				}
			}
		}
	};


	using GridHole = Grid<Hole>;


	struct MovePegEffect : IEffect {

		Vec2 start;
		Vec2 middle;
		Vec2 end;
		Circle hole;
		Circle middlePeg;
		Circle peg;

		MovePegEffect(Vec2 s, Vec2 m, Vec2 e) : start(s), middle(m), end(e) {
			peg = Circle(s, PegSize);
			hole = Circle(e, HoleSize);
			middlePeg = Circle(m, PegSize);
		}

		bool update(double t) override {

			hole.draw(Palette::White);
			const Vec2 pos = start + (end - start) * (t);
			const double pegSize = PegSize * Max(0.0, 1 - 3 * t);
			peg.setCenter(pos);
			peg.draw(Palette::Brown);
			middlePeg.setR(pegSize);
			middlePeg.draw(Palette::Brown);

			return t < 1.0;
		}

	};


	class Board {
	public:

		Point nowClickedPeg;

		GridHole board;

		int32 moveNum;
		Array<GridHole> history;

		Effect effect;

		Size size() const {
			return board.size();
		}

		Hole& at(Point p) {
			return board.at(p);
		}

		const Hole& at(Point p) const {
			return board.at(p);
		}

		Board() : Board(0, 0) {
		}

		Board(int x, int y) : board(x, y) {
			nowClickedPeg = Size(-1, -1);
			moveNum = 0;
		}

		bool updata() {

			bool isChange = false;

			for (auto i : step(board.size())) {
				board.at(i).updata();

				if (board.at(i).isClickedPeg && nowClickedPeg != i) {
					if (nowClickedPeg != Size{ -1, -1 }) {
						for (auto j : step(board.size())) {
							if (j != i)
								board.at(j).isClickedPeg = false;
						}
					}
					nowClickedPeg = i;
				}
				if (board.at(i).isClickedHole) {

					bool distanceOk =
						abs((nowClickedPeg - i).x) + abs((nowClickedPeg - i).y) == 2
						&& abs((nowClickedPeg - i).x) != 1;

					bool isMiddlePeg = board.at((i - nowClickedPeg) / 2 + nowClickedPeg).isPeg;

					if (distanceOk && isMiddlePeg) {

						effect.add<MovePegEffect>(board.at(nowClickedPeg).pos, board.at((i - nowClickedPeg) / 2 + nowClickedPeg).pos, board.at(i).pos);

						board.at((i - nowClickedPeg) / 2 + nowClickedPeg).isPeg = false;
						board.at(nowClickedPeg).isPeg = false;
						board.at(nowClickedPeg).isClickedPeg = false;
						board.at(i).isPeg = true;
						nowClickedPeg = Size{ -1, -1 };

						{
							if (moveNum + 1 != history.size())
								history.erase(history.begin() + (moveNum + 1), history.end());
							history << this->board;
							this->moveNum++;
						}

						isChange = true;
					}
					else {

					}
				}
			}

			return isChange;
		}


		void randomInit();

		void randomInitEasy();

		void randomInitHard();

		void randomInitHell();

		const bool isIn(int x, int y) const {
			if (0 <= x && x < board.size().x && 0 <= y && y < board.size().y) {
				return board.at(y, x).exist;
			}
			return false;
		}

		const bool isPegSide(Point pos) const {
			int dxy[] = { 0,1,0,-1,0 };

			for (int i = 0; i < 4; i++) {
				if (isIn(pos.x + dxy[i], pos.y + dxy[i + 1]) && board.at(pos.y + dxy[i + 1], pos.x + dxy[i]).isPeg) {
					return true;
				}
			}
			return false;
		}

		void draw()const {

			for (auto i : step(board.size())) {
				board.at(i).draw();
			}

			const int dxy[] = { 0,1,0,-1,0 };

			for (auto i : step(board.size())) {
				//TODO:クリックに対応して動かせる手を表示する。
				if (board.at(i).isClickedPeg) {
					for (int j = 0; j < 4; j++) {
						if (isIn(i.x + dxy[j], i.y + dxy[j + 1]) && board.at(i.y + dxy[j + 1], i.x + dxy[j]).isPeg &&
							isIn(i.x + 2 * dxy[j], i.y + 2 * dxy[j + 1]) && !board.at(i.y + 2 * dxy[j + 1], i.x + 2 * dxy[j]).isPeg) {
							Line(board.at(i).pos + 10 * Point(dxy[j], dxy[j + 1]), board.at(i.y + 2 * dxy[j + 1], i.x + 2 * dxy[j]).pos).drawArrow(10, Vec2(15, 15), Palette::Yellow);
							//Circle(board.at(i.y + 2 * dxy[j + 1], i.x + 2 * dxy[j]).pos, 10).draw(Palette::Aqua);
							//Circle(board.at(i.y + dxy[j + 1], i.x + dxy[j]).pos, 10).draw(Palette::Red);
							//Circle(board.at(i.y, i.x).pos, 10).draw(Palette::Green);
						}
					}

				}
			}

			effect.update();
		}

		bool back() {
			if (moveNum > 0) {
				moveNum--;
				board = history[moveNum];
				return true;
			}
			return false;
		}

		bool advance() {
			if (history.size() - 1 > moveNum) {
				moveNum++;
				board = history[moveNum];
				return true;
			}
			return false;
		}

		bool reset() {
			board = history[0];
			history.clear();
			history << board;
			moveNum = 0;
			return true;
		}

		Line hint();


		//問題が解かれたか確認
		bool checkSolved() {

			int32 count = 0;

			for (auto i : step(board.size())) {
				//TODO:クリックに対応して動かせる手を表示する。
				if (isIn(i.x, i.y) && board.at(i).isPeg) {
					count++;
				}
			}

			if (count == 1)
				return true;

			return false;
		}

		//問題が解けるか確認
		bool canSolve() {




			return true;
		}

		bool loadFile(FilePath fileName) {

			return true;
		}

	};

};


