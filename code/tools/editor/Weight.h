#pragma once
/*
Copyright (C) 2010 Dengwenyi

This file is part of dae2n3.

Licensed under the MIT Open Source License
*/

class Weight
{
public:
	//Weights
	float Weights[4];

	//Constructor
	Weight()
	{
		for(int i = 0; i < 4; i++)
		{
			Weights[i] = 0;
		}
	}
};