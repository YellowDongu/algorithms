#pragma once
#include "Vector2.h"
#include <vector>

double ShoelaceFormula(const std::vector<Vector2>& vertices)
{
	double area = 0.0;
	int length = static_cast<int>(vertices.size());
	if (length < 2)
		return 0.0;
	if (length == 2)
		return Vector2::distance(vertices[0], vertices[1]);

	for (int i = 0; i < length; i++)
	{
		int next = (i + 1) % length; // tiling
		area += static_cast<double>(vertices[i].x) * static_cast<double>(vertices[next].y);
		area -= static_cast<double>(vertices[i].y) * static_cast<double>(vertices[next].x);
	}

	return std::abs(area) / 2.0;
}