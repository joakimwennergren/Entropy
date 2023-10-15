#pragma once 

#include "leaf.hpp"

class LeafSystem
{
public:
	LeafSystem()
	{
		for(int i = 0; i < 30; i++)
		{	
			auto leaf = std::make_unique<Leaf>(0.0, 500.0, rand() % 2000, 60.0, 40.0);
			leaves.push_back(std::move(leaf));
		}
	}

	void Update(float screenWidth, float screenHeight)
	{
		for(auto &leaf : leaves)
		{
			leaf->Update(screenWidth, screenHeight);
		}
	}
private:
	std::vector<std::unique_ptr<Leaf>> leaves;
};