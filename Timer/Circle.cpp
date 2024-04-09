#include "Circle.h"

Circle::Circle()
{
}
Circle::Circle(POINT center)
{
	this->center = center;
}
Circle::Circle(float radius)
{
	this->radius = radius;
}
Circle::Circle(POINT center, float radius) : center{ center }, radius{ radius }
{
}
Circle::Circle(float radius, POINT center) : Circle::Circle(center,radius)
{
}
Circle::Circle(const Circle& other)
{
	this->center = other.center;
	this->radius = other.radius;
}




float Circle::GetRadius() const
{
	return this->radius;
}
void Circle::SetRadius(float radius)
{
	this->radius = radius;
}

const POINT& Circle::GetCenter() const
{
	return this->center;
}

void Circle::SetCenter(const POINT& center)
{
	this->center = center;
}




double Circle::FindSquare()
{
	return M_PI*pow((double)radius,2.);
}

POINT Circle::GetRingPoint(double angle, float radiusBias) const
{
	POINT point{ (LONG)(center.x + cos(angle) * (radius + radiusBias)),
				(LONG)(center.y + sin(angle) * (radius + radiusBias)) };

	return point;
}


RECT Circle::FindSelfRect()const
{
	RECT rect{ center.x - (LONG)radius, center.y - (LONG)radius,
			   center.x + (LONG)radius, center.y + (LONG)radius };

	return rect;
}


bool Circle::operator==(const Circle& other)
{
	return this->radius==other.radius and 
		(this->center.x==other.center.x and this->center.y == other.center.y);
}

bool Circle::operator!=(const Circle& other)
{
	return !((*this)==other);
}
