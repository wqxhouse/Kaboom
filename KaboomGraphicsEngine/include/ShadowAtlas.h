#pragma once
#include <vector>

class ShadowAtlas
{
public:
	ShadowAtlas();

	void createAtlas(int size);
	inline int getSize()
	{
		return _size;
	}

	inline int getTileSize()
	{
		return _tileSize;
	}

	inline int getFreeTileCount()
	{
		return _freeTiles;
	}

	inline int getTotalTileCount()
	{
		return _tileCount * _tileCount;
	}

	void removeTile(int id);
	osg::Vec2i createTile(int id, int width, int height);
	// osg::Vec2i convertTilePosToTexelPos(const osg::Vec2i &tilePos);

private:
	int _size;
	int _freeTiles;
	int _tileSize;
	int _tileCount;
	
	std::vector<std::vector<int> > _tiles;

};