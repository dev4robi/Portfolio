#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "Engine/StdHeader.h"
#include "Engine/GameWidget.h"

class Skybox : public GameWidget {
private:
	bool useSkydomeMode;					// 스카이돔 모드 여부 (true 시 스카이 돔을, false시 스카이 박스를 그림)
	GLuint skyTextureID[7];					// 스카이박스 텍스처 번호 (순서대로 탑, 봇, 서, 북, 동, 남, 스카이박스 전용)

public:
	Skybox();								// 생성자
	~Skybox();								// 소멸자

	virtual bool draw();					// 그리기
	virtual bool destroy();					// 파괴
	virtual bool update(REAL timeElapsed);	// 업데이트
};

#endif