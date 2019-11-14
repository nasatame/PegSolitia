#pragma once
#include <bitset>
#include <functional>
#include "PegSolitia.hpp"

namespace PegSolitia {

	constexpr int MaxTurn = 100;
	constexpr int BitsetSize = 100;
	constexpr s3d::Duration EXE_TIME = 3s;

	class Board;

	struct Node {
		//10*10
		//true => exist,false => not exist;
		std::bitset<BitsetSize> board;
		std::bitset<BitsetSize> isBoard;
		int width;
		int height;
		std::array<std::pair<Point, Point>, MaxTurn> hintOrders;
		double evaluate;
		bool operator<(const Node& right) const;
	};

	class Algorithm {
	protected:
		std::array<std::pair<Point, Point>,MaxTurn> hintOrders;

	public:

		virtual bool calculateOrder(const Board& board) = 0;
		std::pair<Point, Point> getHint() const;
		std::array<std::pair<Point, Point>, MaxTurn> getOrders() const;
	};

	//�m���ɓ��������߂����̂ŕ����L���Ă����T���ɂ��邩�A
	//Chokudai�ɂ��邩�Y�񂾂������`�����̎��̑������l�����B
	class ChokudaiSearch : public Algorithm {
	private:

	public:
		bool calculateOrder(const Board& board) override final;
	};

}
