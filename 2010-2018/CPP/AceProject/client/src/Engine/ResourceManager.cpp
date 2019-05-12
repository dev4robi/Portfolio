#include "ResourceManager.h"
#include "Engine/SoundManager.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

ResourceManager::ResourceManager() {
	// 멤버 초기화
	resourceCounter = 0;
	iTotalResourceToLoad = 80;	// 총 80개의 리소스를 로드 (임시로 지정된 수치로 정확한 수치가 아님)
	pResLoadedPercent = NULL;
}

ResourceManager::~ResourceManager() {
	// 리소스 자원 해제
	objMap.clear();
	textureMap.clear();
	soundBufferMap.clear();
	resourceCounter = 0;
}

int ResourceManager::getResourceCnt() {
	return resourceCounter;
}

GLMmodel* ResourceManager::getObjModelPtr(const char *name) {
	map<const string, GLMmodel*>::iterator iter = objMap.find(string(name));

	// 찾는 자료가 존재하면 반환
	if (iter != objMap.end()) {
		GLMmodel *pRtModel = iter->second;
		return pRtModel;
	}

	// 찾는 자료가 존재하지 않으면
	return NULL;
}

GLuint ResourceManager::getTextureID(const char *name) {
	map<const string, GLuint>::iterator iter = textureMap.find(string(name));

	// 찾는 자료가 존재하면 반환
	if (iter != textureMap.end()) {
		GLuint rtTexID = iter->second;
		return rtTexID;
	}

	// 찾는 자료가 존재하지 않으면
	return 0;
}

SoundBuffer* ResourceManager::getSoundBufferPtr(string name) {
	map<string, SoundBuffer*>::iterator iter = soundBufferMap.find(name);

	// 찾는 자료가 존재하면 반환
	if (iter != soundBufferMap.end()) {
		SoundBuffer *pSndBuffer = iter->second;
		return pSndBuffer;
	}

	// 찾는 자료가 존재하지 않으면
	return NULL;
}

void ResourceManager::setResLoadedPercentPtr(int *pPtr) {
	pResLoadedPercent = pPtr;
}

bool ResourceManager::loadOBJ(const char *name) {
	// 이미 리소스 맵에 로드된 리소스인지 확인
	if (getObjModelPtr(name) != NULL) {
		printf("ResourceManager::loadOBJ - GLMmodel(%s) already loaded.\n", name);
		return false;
	}

	// .obj모델 로드
	GLMmodel *model = glmReadOBJ(name);

	if (model == NULL) { // 모델 로드 실패
		printf("ResourceManager::loadOBJ - GLMmodel(%s) load has failed.\n", name);
		return false;
	}
	else { // 모델 로드 성공
		++resourceCounter;

		// 오브젝트 리소스 맵에 삽입
		objMap.insert(pair<const char*, GLMmodel*>(name, model));

		// 퍼센테이지 계산
		if (pResLoadedPercent != NULL) {
			*pResLoadedPercent = (int)((resourceCounter * 100) / iTotalResourceToLoad);
		}

		return true;
	}
}

bool ResourceManager::loadTexture(const char *name, bool useAlpha, bool useFlip, GLenum minFilter, GLenum magFilter, GLenum wrapFilter) {
	// 이미 리소스 맵에 로드된 리소스인지 확인
	if (getTextureID(name) != 0) {
		printf("ResourceManager::loadTexture - Texture(%s) already loaded.\n", name);
		return false;
	}

	// OpenCV를 이용하여 이미지 파일을 오픈
	int cvtFilter = CV_BGR2RGB;
	int colorFormat = GL_RGB;
	GLuint texID = 0;
	cv::Mat img = cv::imread(name, CV_LOAD_IMAGE_UNCHANGED); // 8 unsigned char with 4 channel = 32 bits (0~255)
	if (img.data == NULL) { // 이미지 오픈 실패
		printf("ResourceManager::loadTexture - Texture(%s) img open has failed.\n", name);
		return false;
	}

	// 색상 순서 변경 (BGR -> RGB)
	if (useAlpha == true) {
		cvtFilter = CV_BGRA2RGBA;
		colorFormat = GL_RGBA;
	}
	cv::cvtColor(img, img, cvtFilter);

	// Flip 옵션을 사용하지 (중요:"않을") 경우 이미지 회전 (CV -> GL 상하 반전)
	if (useFlip == false)
		cv::flip(img, img, 0);

	// 텍스처 생성
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, img.cols, img.rows, 0, colorFormat, GL_UNSIGNED_BYTE, img.ptr());

	if (texID == 0) { // 텍스처 로드 실패
		printf("ResourceManager::loadTexture - Texture(%s) load has failed.\n", name);
		return false;
	}
	else { // 텍스처 로드 성공
		++resourceCounter;

		// 텍스처 리소스 맵에 삽입
		textureMap.insert(pair<const char*, GLuint>(name, texID));
		printf("ResourceManager::loadTexture - Texture(%s) load has been completed! (ID:%d)\n", name, texID);

		// 퍼센테이지 계산
		if (pResLoadedPercent != NULL) {
			*pResLoadedPercent = (int)((resourceCounter * 100) / iTotalResourceToLoad);
		}

		return true;
	}
}

bool ResourceManager::loadWavSound(string name) {
	// 이미 리소스 맵에 로드된 리소스인지 확인
	if (getSoundBufferPtr(name) != NULL) {
		printf("ResourceManager::loadWavSound - Sound(%s) already loaded.\n", name);
		return false;
	}

	// .wav 파일 데이터 로드
	FILE *fp = NULL;
	if ((fp = fopen(name.c_str(), "rb")) == NULL) {
		printf("ResourceManager::loadWavSound - Sound(%s) can't find file...\n", name);
		return false;
	}

	char type[4];
	DWORD size, chunkSize;
	short formatType, channels;
	DWORD sampleRate, avgBytesPerSec;
	short bytesPerSample, bitsPerSample;
	DWORD dataSize;

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'R' || type[1] != 'I' || type[2] != 'F' || type[3] != 'F') {
		printf("ResourceManager::loadWavSound - Sound(%s) no RIFF.\n", name);
		fclose(fp);
		return false;
	}
	
	fread(&size, sizeof(DWORD), 1, fp);
	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'W' || type[1] != 'A' || type[2] != 'V' || type[3] != 'E') {
		printf("ResourceManager::loadWavSound - Sound(%s) not .WAV file.\n", name);
		fclose(fp);
		return false;
	}

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'f' || type[1] != 'm' || type[2] != 't' || type[3] != ' ') {
		printf("ResourceManager::loadWavSound - Sound(%s) not FMT.\n", name);
		fclose(fp);
		return false;
	}

	fread(&chunkSize, sizeof(DWORD), 1, fp);
	fread(&formatType, sizeof(short), 1, fp);
	fread(&channels, sizeof(short), 1, fp);
	fread(&sampleRate, sizeof(DWORD), 1, fp);
	fread(&avgBytesPerSec, sizeof(DWORD), 1, fp);
	fread(&bytesPerSample, sizeof(short), 1, fp);
	fread(&bitsPerSample, sizeof(short), 1, fp);

	fread(type, sizeof(char), 4, fp);
	if (type[0] != 'd' || type[1] != 'a' || type[2] != 't' || type[3] != 'a') {
		printf("ResourceManager::loadWavSound - Sound(%s) missing DATA.\n", name);
		fclose(fp);
		return false;
	}

	fread(&dataSize, sizeof(DWORD), 1, fp);

	unsigned char *buf = new unsigned char[dataSize];
	fread(buf, sizeof(BYTE), dataSize, fp);

	// OpenAL 사운드 버퍼 생성
	SoundBuffer *pSndBuffer = new SoundBuffer();
	pSndBuffer->fileName = string(name); // 파일명

	alGenBuffers(1, &pSndBuffer->ID); // AL 버퍼 생성

	if (bitsPerSample == 8) { // 포맷 설정
		if (channels == 1)
			pSndBuffer->format = AL_FORMAT_MONO8;
		else if (channels = 2)
			pSndBuffer->format = AL_FORMAT_STEREO8;
	}
	else if (bitsPerSample == 16) {
		if (channels == 1)
			pSndBuffer->format = AL_FORMAT_MONO16;
		else if (channels == 2)
			pSndBuffer->format = AL_FORMAT_STEREO16;
	}

	pSndBuffer->frequency = sampleRate; // 주파수

	alBufferData(pSndBuffer->ID, pSndBuffer->format, buf, dataSize, pSndBuffer->frequency); // 버퍼에 데이터 바인딩

	// 사운드 재생 시간 저장
	ALfloat playtime = 0.00;

	switch (pSndBuffer->format) {
	case AL_FORMAT_MONO8:		playtime = 8.0;		break;
	case AL_FORMAT_STEREO8:		playtime = 16.0;	break;
	case AL_FORMAT_MONO16:		playtime = 16.0;	break;
	case AL_FORMAT_STEREO16:	playtime = 32.0;	break;
	} // 모노 = 1, 스테레오 = 2 와 비트수를 곱함

	playtime *= sampleRate;				// 주파수를 곱함
	playtime /= 8.0;					// 바이트로 표현하기 위해 8을 나눔
	playtime /= dataSize;				// 데이터 크기와 나누어서 전체 길이를 계산
	pSndBuffer->playtime = playtime;	// 사운드 버퍼 구조체에 저장

	// 데이터 삭제 및 파일 닫기
	fclose(fp);
	delete[] buf;
	buf = NULL;

	// 오류 검사
//	int err = 0;
//	if ((err = alGetError()) != 0) {
//		printf("ResourceManager::loadWavSound - OpenAL error: %d\n", err);
//		return false;
//	}

	// 사운드 버퍼 맵에 삽입
	soundBufferMap.insert(pair<string, SoundBuffer*>(name, pSndBuffer));

	// 리소스 개수 증가
	++resourceCounter;

	// 퍼센테이지 계산
	if (pResLoadedPercent != NULL) {
		*pResLoadedPercent = (int)((resourceCounter * 100) / iTotalResourceToLoad);
	}

	// 성공 반환
	return true;
}

bool ResourceManager::remove(const char *name) {
	return true;
}

bool ResourceManager::removeAll() {
	return true;
}
