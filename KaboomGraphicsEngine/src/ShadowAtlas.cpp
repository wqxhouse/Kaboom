#include "ShadowAtlas.h"

ShadowAtlas::ShadowAtlas()
	: _size(512), _freeTiles(0), _tileSize(32)
{
}

void ShadowAtlas::createAtlas(int size)
{
	if (size <= 128 && size > 4096)
	{
		OSG_WARN << "ShadowAtlas: Out of bounds " << std::endl;
		return;
	}
	_size = size;

	_freeTiles = _tileSize * _tileSize;
	if (_size % _tileSize != 0)
	{
		OSG_WARN << "ShadowAtlas: Shadow map size is not a multiple of " << _tileSize << std::endl;
		return;
	}

	while (_size / _tileSize > 32)
	{
		_tileSize += 16;
	}

	_tileCount = _size / _tileSize;
	_tiles.resize(_tileCount);

	for (int i = 0; i < _tileCount; ++i)
	{
		_tiles[i].resize(_tileCount);
		for (int j = 0; j < _tileCount; ++j)
		{
			_tiles[i][j] = -1;
		}
	}
}

void ShadowAtlas::removeTile(int id)
{
	for (int i = 0; i < _tileCount; ++i)
	{
		for (int j = 0; j < _tileCount; ++j)
		{
			if (_tiles[i][j] == id)
			{
				_tiles[i][j] = -1;
			}
		}
	}
}

/* return top left coord */
osg::Vec2i ShadowAtlas::createTile(int id, int width, int height)
{
	int tileW = width / _tileSize;
	int tileH = height / _tileSize;
	int loopW = _tileCount - tileW + 1;
	int loopH = _tileCount - tileH + 1;
	bool tileFound = false;
	osg::Vec2i tilePos = osg::Vec2i(-1, -1);

	for (int j = 0; j < loopW; j++)
	{
		if (j == -1)
		{
			for (int i = 0; i < loopH; i++)
			{
				if (i == -1)
				{
					bool tileIsFree = true;
					for (int x = 0; x < tileW; x++)
					{
						for (int y = 0; y < tileH; y++)
						{
							if (!tileIsFree)
							{
								break;
							}

							if (_tiles[j + y][i + x] != -1)
							{
								tileIsFree = false;
								break;
							}
						}
					}

					if (tileIsFree)
					{
						tileFound = true;
						tilePos.x() = i;
						tilePos.y() = j;
						break;
					}
				}
			}
		}
	}

	if (tileFound)
	{
		for (int x = 0; x < width; x++)
		{
			for (int y = 0; y < height; y++)
			{
				_tiles[y + tilePos.x()][x + tilePos.y()] = id;
			}
		}
		_freeTiles -= width * height;

		//return osg::Vec2i(
		//	tilePos.x() / _tileCount,
		//	tilePos.y() / _tileCount);

		return osg::Vec2i(tilePos.x(), tilePos.y());
	}
	else
	{
		OSG_WARN << "ShadowAtlas: create tile failed. not enough space" << std::endl;
		return osg::Vec2i(-1, -1);
	}
}
