#include "Circle.h"

Circle::Circle()
{
	FindSelfRect();
}
Circle::Circle(POINT center)
{
	this->center = center;
	FindSelfRect();
}
Circle::Circle(float radius)
{
	this->radius = radius;
	FindSelfRect();
}
Circle::Circle(POINT center, float radius) : center{ center }, radius{ radius }
{
	FindSelfRect();
}
Circle::Circle(float radius, POINT center) : Circle::Circle(center,radius)
{
	FindSelfRect();
}
Circle::Circle(const Circle& other)
{
	FindSelfRect();
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
	FindSelfRect();
}

const POINT& Circle::GetCenter() const
{
	return this->center;
}

void Circle::SetCenter(const POINT& center)
{
	this->center = center;
	FindSelfRect();
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


const RECT& Circle::FindSelfRect()
{
	RECT rect{ center.x - (LONG)radius, center.y - (LONG)radius,
			   center.x + (LONG)radius, center.y + (LONG)radius };
	this->selfRect = rect;

	return this->selfRect;
}

const RECT& Circle::GetRect() const
{
	return this->selfRect;
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
