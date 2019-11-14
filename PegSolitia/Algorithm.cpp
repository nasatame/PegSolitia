#include <queue>
#include "Algorithm.hpp"

std::pair<Point, Point> PegSolitia::Algorithm::getHint() const
{
	return hintOrders[0];
}

std::array<std::pair<Point, Point>, PegSolitia::MaxTurn> PegSolitia::Algorithm::getOrders() const
{
	return hintOrders;
}

bool PegSolitia::ChokudaiSearch::calculateOrder(const Board& board)
{

	int64 enumCount = 0;
	int64 enumCounts[MaxTurn] = {};
	int64 stoneCount = 0;
	for (int32 i = 0; i < board.board.height(); i++) {
		for (int32 k = 0; k < board.board.width(); k++) {
			if (board.board[i][k].isPeg)
				stoneCount++;
		}
	}

	auto convertPos = [board](const int32 x, const int32 y) {
		return y * board.board.width() + x;
	};

	auto convertNode = [convertPos](const Board& board) {

		PegSolitia::Node node;

		node.height = board.board.height();
		node.width = board.board.width();

		for (int32 i = 0; i < node.height; i++) {
			for (int32 k = 0; k < node.width; k++) {
				node.isBoard[convertPos(k, i)] = board.board[i][k].exist;
				node.board[convertPos(k, i)] = board.board[i][k].isPeg;
			}
		}

		return node;
	};

	//����͈�U�g��Ȃ�
	/*
	auto checkSame = [](const Node nowNode, const std::unordered_map<std::bitset<MaxTurn>, int64>& hash) {
		return false;
	};
	*/

	auto evaluate = [convertPos](const Node nowNode) {
		//���U���v�Z���Ă�����X�R�A�ɂ���

		//���ϒl
		Vec2 average;
		int32 count = 0;

		for (int y = 0; y < nowNode.height; y++) {
			for (int x = 0; x < nowNode.width; x++) {
				if (nowNode.board[convertPos(x, y)]) {
					average += Vec2(x, y);
					count++;
				}
			}
		}

		average /= count;

		//���U
		//(peg - average)
		double result = 0;
		for (int y = 0; y < nowNode.height; y++) {
			for (int x = 0; x < nowNode.width; x++) {
				if (nowNode.board[convertPos(x, y)]) {
					result += (Vec2(x, y) - average).length();
				}
			}
		}

		//���U�͎U��΂��Ă�������傫���Ȃ�̂ŋt���ɂ���B
		return -1 * Sqrt(result);
	};

	auto checkIn = [board](int32 x, int32 y) {
		return 0 <= y && y < board.board.height() && 0 <= x && x < board.board.width();
	};

	auto enumulateNode = [convertPos, checkIn, stoneCount, evaluate, &enumCount, &enumCounts](const Node nowNode, std::priority_queue<Node>& bucket,
		std::unordered_map<std::bitset<BitsetSize>, int64>& hash, const int32 turn) {
			Optional<Node> result;
			//4�����A��
			int dxy[5] = { 0,1,0,-1,0 };
			for (int y = 0; y < nowNode.height; y++) {
				for (int x = 0; x < nowNode.width; x++) {
					for (int dir = 0; dir < 4; dir++) {
						if (checkIn(x, y) && checkIn(x + dxy[dir], y + dxy[dir + 1]) && checkIn(x + dxy[dir] * 2, y + dxy[dir + 1] * 2) &&
							nowNode.isBoard[convertPos(x, y)] && nowNode.isBoard[convertPos(x + dxy[dir], y + dxy[dir + 1])] && nowNode.isBoard[convertPos(x + dxy[dir] * 2, y + dxy[dir + 1] * 2)] &&
							nowNode.board[convertPos(x, y)] && nowNode.board[convertPos(x + dxy[dir], y + dxy[dir + 1])] && !nowNode.board[convertPos(x + dxy[dir] * 2, y + dxy[dir + 1] * 2)]) {
							//���W�����݂���
							//�}�b�v�����݂���
							//�ړ��\
							//���]
							Node nextNode = nowNode;

							nextNode.board[convertPos(x, y)].flip();
							nextNode.board[convertPos(x + dxy[dir], y + dxy[dir + 1])].flip();
							nextNode.board[convertPos(x + dxy[dir] * 2, y + dxy[dir + 1] * 2)].flip();

							hash[nextNode.board]++;

							//�d���m�F
							if (hash[nextNode.board] == 1) {
								//�ǉ�
								enumCount++;
								enumCounts[turn]++;
							}
							else {
								//�ǉ����Ȃ�
								continue;
							}

							//�]��
							nextNode.evaluate = evaluate(nextNode);

							//���O�����o��
							nextNode.hintOrders[turn] = std::pair<Point, Point>(Point(x, y), Point(x + dxy[dir] * 2, y + dxy[dir + 1] * 2));

							bucket.push(nextNode);

							//�I���m�F
							if (turn == stoneCount - 2) {
								result = nextNode;
							}

						}
					}
				}
			}

			return result;
	};

	auto printNode = [convertPos](const Node& node) {

		Console.writeln(U"evaluate:", node.evaluate);

		for (int32 i = 0; i < node.height; i++) {
			for (int32 k = 0; k < node.width; k++) {
				if (node.isBoard[convertPos(k, i)]) {
					if (node.board[convertPos(k, i)]) {
						Console.write(U" O");
					}
					else {
						Console.write(U" _");
					}
				}
				else {
					Console.write(U"  ");
				}
			}
			Console.writeln();
		}

		Console.writeln();

	};

	auto printCount = [](const Stopwatch sw, const int64 enumCount, const int64 enumCounts[MaxTurn]) {
		Console.writeln(U"elapsed:", sw.elapsed());
		Console.writeln(U"enumCount:", enumCount);
		for (int i = 0; i < MaxTurn; i++) {
			Console.write(i,U":",enumCounts[i],U" ");
		}
		Console.writeln();
	};

	Node node = convertNode(board);

	//ChokudaiSearch
	std::priority_queue<Node> Buckets[MaxTurn];

	Buckets[0].push(node);

	std::unordered_map<std::bitset<BitsetSize>, int64> hash;

//#define PRINT

#ifdef PRINT
	Console.open();
#endif // PRINT

	Stopwatch sw;
	sw.start();

	while (true) {
		Node topNode;
		bool failed = true;

		//turn
		for (int turn = 0; turn < stoneCount; turn++) {
			//��Ԃ����̂�I��ŗ�
			if (Buckets[turn].size() == 0) {
				//�Ȃ����continue;
				continue;
			}
			topNode = Buckets[turn].top();
			Buckets[turn].pop();
			failed = false;

			//printNode(topNode);

			//���̏�Ԃ��
			Optional<Node> end = enumulateNode(topNode, ref(Buckets[turn + 1]), ref(hash), turn);

			//�񋓂������̂̒��ɉ񓚂�����ΏI��
			if (end.has_value()) {
#ifdef PRINT
				printCount(sw, enumCount, enumCounts);
				printNode(*end);
#endif // PRINT
				this->hintOrders = end->hintOrders;

				return true;
			}

		}

#ifdef PRINT
		printNode(topNode);
#endif // PRINT

		//�ς񂾏���
		if (sw.elapsed() >= EXE_TIME) {
			failed = true;
		}

		if (failed) {
			break;
		}
	}

#ifdef PRINT
	printCount(sw, enumCount, enumCounts);
#endif // PRINT

	return false;
}

bool PegSolitia::Node::operator<(const Node& right) const
{
	return evaluate < right.evaluate;
}
