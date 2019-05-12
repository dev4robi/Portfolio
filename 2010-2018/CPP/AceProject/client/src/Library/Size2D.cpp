#include "Size2D.h"

Size2D::Size2D(float iWidth, float iHeight) {
	setWidth(iWidth);
	setHeight(iHeight);
}

Size2D::Size2D(Size2D& iSize) {
	setWidth(iSize.getWidth());
	setHeight(iSize.getHeight());
}

Size2D::~Size2D() {

}

void Size2D::setWidth(float iWidth) {
	width = iWidth;
}

void Size2D::setHeight(float iHeight) {
	height = iHeight;
}

void Size2D::set(float iWidth, float iHeight) {
	setWidth(iWidth);
	setHeight(iHeight);
}

void Size2D::set(Size2D iSize) {
	setWidth(iSize.getWidth());
	setHeight(iSize.getHeight());
}

float Size2D::getWidth() {
	return width;
}

float Size2D::getHeight() {
	return height;
}

Size2D Size2D::get() {
	return Size2D(getWidth(), getHeight());
}

float Size2D::getArea() {
	return (getWidth() * getHeight());
}

Size2D Size2D::operator+(Size2D iSize) {
	return Size2D(height + iSize.getHeight(), width + iSize.getWidth());
}

Size2D Size2D::operator-(Size2D iSize) {
	return Size2D(height - iSize.getHeight(), width - iSize.getWidth());
}

Size2D Size2D::operator*(float i) {
	return Size2D(height * i, width * i);
}

Size2D Size2D::operator/(float i) {
	return Size2D(height / i, width / i);
}

void Size2D::operator=(Size2D iSize) {
	setWidth(iSize.getWidth());
	setHeight(iSize.getHeight());
}
