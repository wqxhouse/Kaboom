#pragma once

class ShadowTile
{
public:
	ShadowTile();
	inline int getId()
	{
		return _highest_id;
	}



private:
	int _id;
	
	static int _highest_id;
};