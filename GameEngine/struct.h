#pragma once
#include <climits>
#include <memory>

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

	bool operator>(const Vector2 _other) const
	{
		if (y != _other.y)
			return y > _other.y;
		return x > _other.x;
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

struct Node
{
	int		F;
	int		G;
	int     Turn;
	Vector2 Pos;
	std::shared_ptr<Node>	PrevNode;

	Node()
	{
		F = INT_MAX;
		G = 0;
		Turn = INT_MAX;
		Pos = Vector2(-1,-1);
		PrevNode = nullptr;
	}

	Node(int _F, int _G, int _Turn, Vector2 _Pos, std::shared_ptr <Node> _PrevNode)
	{
		F = _F;
		G = _G;
		Turn = _Turn;
		Pos = _Pos;
		PrevNode = _PrevNode;
	}

	bool operator< (const Node _other) const
	{
		if (Turn == _other.Turn)
			return F < _other.F;
		return Turn < _other.Turn;
	}

	bool operator> (const Node _other) const
	{
		if (Turn == _other.Turn)
			return F > _other.F;
		return Turn > _other.Turn;
	}

	bool operator<= (const Node _other) const
	{
		if (Turn == _other.Turn)
			return F <= _other.F;
		return Turn <= _other.Turn;
	}

	bool operator>= (const Node _other) const
	{
		if (Turn == _other.Turn)
			return F >= _other.F;
		return Turn >= _other.Turn;
	}
};

struct Cmp {
	bool operator() (const std::shared_ptr<Node>& a,
		const std::shared_ptr<Node>& b) const {
		return *a > *b;
	}
};