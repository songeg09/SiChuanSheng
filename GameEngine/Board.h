#pragma once
#include "pch.h"

class Block;
class Texture;

class Board
{
private:
	int						m_iBlocksLeft;
	Texture*				m_pBackGroundTexture;
	std::shared_ptr<Block>	m_Blocks[TOTAL_BLOCKS];

public:
	void Init();
	void Render();
	void Update();
	void ShuffleBlocks();
		
};

