#pragma once

struct Vector2
{
	int x;
	int y;
	Vector2()
	{
		x = 0.0f;
		y = 0.0f;
	}

	Vector2(int _x, int _y)
	{
		x = _x;
		y = _y;
	}

	void operator+=(Vector2 _vec2)
	{
		x += _vec2.x;
		y += _vec2.y;
	}

	bool operator==(const Vector2 _other)
	{
		return x == _other.x && y == _other.y;
	}

	bool operator<(const Vector2 _other) const
	{
		if (y != _other.y)
			return y < _other.y;
		return x < _other.x;
	}

	Vector2 operator+(const Vector2 _vec2)
	{
		return Vector2(x + _vec2.x, y + _vec2.y);
	}

	Vector2 operator-(const Vector2 _vec2)
	{
		return Vector2(x - _vec2.x, y - _vec2.y);
	}

	int GetDistance(Vector2 other)
	{
		return abs(x - other.x) + abs(y - other.y);
	}
};

struct AStarNode
{
	int		F;
	int		G;
	Vector2 Pos;

	AStarNode()
	{
		F = 0;
		G = 0;
		Pos = Vector2(0,0);
	}

	AStarNode(int _F, int _G, Vector2 _Pos)
	{
		F = _F;
		G = _G;
		Pos = _Pos;
	}

	bool operator< (const AStarNode _other) const
	{
		return F < _other.F;
	}

	bool operator> (const AStarNode _other) const
	{
		return F > _other.F;
	}
};