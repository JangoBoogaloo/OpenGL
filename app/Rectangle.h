#pragma once

#include <vector>

/// <summary>
///   1 * * 3
///   * *   *
///   *   * *
///   0 * * 2
/// </summary>
const std::vector<float> RectangleShapeVertices = {
	-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
	-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,
	 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
	 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
};

/// <summary>
///   1 * * 3
///   * *   *
///   *   * *
///   0 * * 2
/// </summary>
const std::vector<unsigned int> RectangleIndices =
{
	0, 1, 2,
	1, 3, 2
};