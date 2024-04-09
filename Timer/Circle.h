#pragma once
#include "Windows.h"
#include "math.h"
#include "corecrt_math_defines.h"
class Circle
{
	float radius=0.f;
	POINT center{0,0};
	RECT selfRect;
public:
	Circle();
	Circle(POINT center);
	Circle(float radius);
	Circle(POINT center, float radius);
	Circle(float radius,POINT center);

	Circle(const Circle& other);



	float GetRadius() const;
	void SetRadius(float radius);

	const POINT& GetCenter() const;
	void SetCenter(const POINT& center);



	double FindSquare();
	POINT GetRingPoint(double angle, float radiusBias = 0.f)const;
	
	const RECT& GetRect()const;



	bool operator==(const Circle& other);
	bool operator!=(const Circle& other);
protected:
	const RECT& FindSelfRect();
};

