#include "ShadowTile.h"

ShadowTile::ShadowTile()
{
	_id = _highest_id++;
}

int ShadowTile::_highest_id = 0;