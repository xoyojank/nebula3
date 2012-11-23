#pragma once

/*
Copyright (C) 2010 Dengwenyi

This file is part of dae2n3.

Licensed under the MIT Open Source License
*/

class JointIndex
{
public:
	//Indices
	short Indices[4];

	//Constructor
	JointIndex()
	{
		for(int i = 0; i < 4; i++)
		{
			Indices[i] = 0;
		}
	}
};
