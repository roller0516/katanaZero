#pragma once

FPOINT operator / (FPOINT& pos1, float value)
{
	FPOINT temp;

	temp.x = pos1.x * value;
	temp.y = pos1.y * value;

	return temp;
}

FPOINT operator + (FPOINT& pos1, POINT& pos2)
{
	FPOINT temp;

	temp.x = pos1.x + pos2.x;
	temp.y = pos1.y + pos2.y;

	return temp;
}
FPOINT operator - (FPOINT& pos1, POINT& pos2)
{
	FPOINT temp;

	temp.x = pos1.x - pos2.x;
	temp.y = pos1.y - pos2.y;

	return temp;
}
FPOINT operator - (FPOINT& pos1, FPOINT& pos2)
{
	FPOINT temp;

	temp.x = pos1.x - pos2.x;
	temp.y = pos1.y - pos2.y;

	return temp;
}
