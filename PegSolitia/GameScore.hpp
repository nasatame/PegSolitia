#pragma once
#include <Siv3D.hpp>
#include "PegSolitia.hpp"

// ユーザデータとゲームのスコアを記録する構造体
struct GameScore
{
	PegSolitia::Difficulty diff;

	Duration score;

	// シリアライズに対応させるためのメンバ関数を定義する
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(diff, score);
	}
};