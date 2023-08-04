#pragma once
#include <vector>

/// <summary>
///   1
///   * *
///   *   *
///   0 * * 2
/// </summary>
const std::vector<float> TriangleShapeVertices = {
	-0.5f, -0.5f, 0.0f,
	-0.5f,  0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f
};

/// <summary>
///   1
///   * *
///   *   *
///   0 * * 2
/// </summary>
const std::vector<unsigned int> TriangleIndices =
{
	0, 1, 2
};