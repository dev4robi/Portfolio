#ifndef __SIZE_2D__
#define __SIZE_2D__

class Size2D {
private:
	float width;
	float height;

public:
	Size2D(float iWidth = 0, float iHeight = 0);
	Size2D(Size2D& iSize);
	~Size2D();

	void setWidth(float iWidth);
	void setHeight(float iHeight);
	void set(float iWidth, float iHeight);
	void set(Size2D iSize);
	float getWidth();
	float getHeight();
	Size2D get();
	float getArea();				// 면적을 반환

	Size2D operator+(Size2D iSize);
	Size2D operator-(Size2D iSize);
	Size2D operator*(float i);
	Size2D operator/(float i);
	void operator=(Size2D iSize);
};

#endif