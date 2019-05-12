#include "Point2D.h"

Point2D::Point2D(float ix, float iy) {
	setX(ix);
	setY(iy);
}

Point2D::Point2D(Point2D& iPoint) {
	x = iPoint.getX();
	y = iPoint.getY();
}

Point2D::~Point2D() {

}

void Point2D::setX(float ix) {
	x = ix;
}

void Point2D::setY(float iy) {
	y = iy;
}

void Point2D::set(float ix, float iy) {
	setX(ix);
	setY(iy);
}

void Point2D::set(Point2D iPoint) {
	x = iPoint.getX();
	y = iPoint.getY();
}

float Point2D::getX() {
	return x;
}

float Point2D::getY() {
	return y;
}

Point2D Point2D::get() {
	return Point2D(x, y);
}

Point2D Point2D::operator+(Point2D iPoint) {
	return Point2D(x + iPoint.getX(), y + iPoint.getY());
}

Point2D Point2D::operator-(Point2D iPoint) {
	return Point2D(x - iPoint.getX(), y - iPoint.getY());
}

Point2D Point2D::operator*(float i) {
	return Point2D(x * i, y * i);
}

Point2D Point2D::operator/(float i) {
	return Point2D(x / i, y / i);
}

void Point2D::operator=(Point2D iPoint) {
	set(iPoint.getX(), iPoint.getY());
}
