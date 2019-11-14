#pragma once
#include <Siv3D.hpp>
#include "PegSolitia.hpp"

// ���[�U�f�[�^�ƃQ�[���̃X�R�A���L�^����\����
struct GameScore
{
	PegSolitia::Difficulty diff;

	Duration score;

	// �V���A���C�Y�ɑΉ������邽�߂̃����o�֐����`����
	template <class Archive>
	void SIV3D_SERIALIZE(Archive& archive)
	{
		archive(diff, score);
	}
};