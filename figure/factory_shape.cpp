/*
 * factory_shape.cpp
 *
 *  Created on: Sep 12, 2019
 *      Author: bone
 */

#include "factory_shape.hpp"

#include "primitive/point.hpp"

namespace figure
{
Point FactoryShape::point(primitive::Point const& point) const
{
  return Point{renderer_, static_cast<int>(point.x), static_cast<int>(point.y)};
}
}  // namespace figure

