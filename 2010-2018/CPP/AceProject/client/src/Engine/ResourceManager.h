#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ResourceManager(v2)
*	- Last Update : 2016.08.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 모델 및 이미지, 사운드 등의 리소스 파일을 로드하여 관리하는 매니저.
*					 이미지 요청 -> map에서 해당명의 파일이 있는지 확인 -> 있으면 해당 ID 반환.
*
*	- Dependency : [StdHeader.h, glm.h, al.h], SoundManager.h
*
*   - Version(v1): 리소스 매니저 기초 설계. (2016.05.15)
*	- Version(v2): OpenAL을 이용한 사운드 버퍼맵 파일 추가 지원. (2016.08.16)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Library/glm/glm.h"
#include "openal/include/al.h"

struct SoundBuffer; // from "Engine/SoundManager.h"

class ResourceManager : public GameObject {
private:
	int resourceCounter;							// 리소스 개수 카운터 (.obj 모델 파일에 포함된 텍스처 묶음도 1개의 리소스로 간주함에 주의)
	int *pResLoadedPercent;							// 로딩 완료된 리소스 백분율을 저장할 변수를 가르키는 포인터
	int iTotalResourceToLoad;						// 로드해야 할 리소스의 총 개수
	map<const string, GLMmodel*> objMap;			// .obj모델 리소스 맵
	map<const string, GLuint> textureMap;			// texture 리소스 맵
	map<string, SoundBuffer*> soundBufferMap;		// 사운드 버퍼(데이터) 맵

public:
	// 생성자 & 소멸자
	ResourceManager();
	~ResourceManager();

	// 게터 & 세터
	int getResourceCnt();							// 관리중인 리소스의 개수를 반환
	GLMmodel* getObjModelPtr(const char *name);		// .obj모델의 포인터를 반환
	GLuint getTextureID(const char *name);			// 텍스처의 ID를 반환
	SoundBuffer* getSoundBufferPtr(string name);	// 사운드 데이터가 담긴 버퍼 포인터를 반환

	// 메소드
	void setResLoadedPercentPtr(int *pPtr);			// 로딩 완료된 리소스 백분율을 저장할 변수를 설정
	bool loadOBJ(const char *name);					// .obj모델과 .mtl파일의 리소스를 로드하여 관리하에 둠
	bool loadTexture(const char *name, bool useAlpha, bool useFlip, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);	// 이미지 파일을 로드하여 GL텍스처 생성
	bool loadWavSound(string name);					// .wav 파일 로드
	bool remove(const char* name);					// 로드된 리소스를 제거
	bool removeAll();								// 모든 리소스를 제거
};

#endif