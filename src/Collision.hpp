#pragma once
#include "Types.hpp"
#include "Vector.hpp"

// Checks if moving rect will collide with still rect
// Args:
// 	m - moving rect
// 	r - still rect
// 	s - rect size
//	p - rect position
//	w - edge of r which m will collide with (to be calculated)
//	n - normal to w (to be calculated)
//	t - time until collision (could be updated)

bool checkRectCollision(Vec2f r_p, Vec2f r_s, Vec2f m_p_0, Vec2f m_p_1, Vec2f m_s, Vec2f* w, Vec2f* n, float* tMin);
