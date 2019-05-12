#ifndef __POINT_2D_H__
#define __POINT_2D_H__

class Point2D {
private:
	float x;
	float y;

public:
	Point2D(float ix = 0, float iy = 0);
	Point2D(Point2D& iPoint);
	~Point2D();

	void setX(float ix);
	void setY(float iy);
	void set(float ix, float iy);
	void set(Point2D iPoint);
	float getX();
	float getY();
	Point2D get();

	Point2D operator+(Point2D iPoint);
	Point2D operator-(Point2D iPoint);
	Point2D operator*(float i);
	Point2D operator/(float i);
	void operator=(Point2D iPoint);
};

#endif