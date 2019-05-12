#include "ResourceManager.h"
#include "Engine/SoundManager.h"
#include "opencv2/opencv.hpp"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

ResourceManager::ResourceManager() {
	// ��� �ʱ�ȭ
	resourceCounter = 0;
	iTotalResourceToLoad = 80;	// �� 80���� ���ҽ��� �ε� (�ӽ÷� ������ ��ġ�� ��Ȯ�� ��ġ�� �ƴ�)
	pResLoadedPercent = NULL;
}

ResourceManager::~ResourceManager() {
	// ���ҽ� �ڿ� ����
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

	// ã�� �ڷᰡ �����ϸ� ��ȯ
	if (iter != objMap.end()) {
		GLMmodel *pRtModel = iter->second;
		return pRtModel;
	}

	// ã�� �ڷᰡ �������� ������
	return NULL;
}

GLuint ResourceManager::getTextureID(const char *name) {
	map<const string, GLuint>::iterator iter = textureMap.find(string(name));

	// ã�� �ڷᰡ �����ϸ� ��ȯ
	if (iter != textureMap.end()) {
		GLuint rtTexID = iter->second;
		return rtTexID;
	}

	// ã�� �ڷᰡ �������� ������
	return 0;
}

SoundBuffer* ResourceManager::getSoundBufferPtr(string name) {
	map<string, SoundBuffer*>::iterator iter = soundBufferMap.find(name);

	// ã�� �ڷᰡ �����ϸ� ��ȯ
	if (iter != soundBufferMap.end()) {
		SoundBuffer *pSndBuffer = iter->second;
		return pSndBuffer;
	}

	// ã�� �ڷᰡ �������� ������
	return NULL;
}

void ResourceManager::setResLoadedPercentPtr(int *pPtr) {
	pResLoadedPercent = pPtr;
}

bool ResourceManager::loadOBJ(const char *name) {
	// �̹� ���ҽ� �ʿ� �ε�� ���ҽ����� Ȯ��
	if (getObjModelPtr(name) != NULL) {
		printf("ResourceManager::loadOBJ - GLMmodel(%s) already loaded.\n", name);
		return false;
	}

	// .obj�� �ε�
	GLMmodel *model = glmReadOBJ(name);

	if (model == NULL) { // �� �ε� ����
		printf("ResourceManager::loadOBJ - GLMmodel(%s) load has failed.\n", name);
		return false;
	}
	else { // �� �ε� ����
		++resourceCounter;

		// ������Ʈ ���ҽ� �ʿ� ����
		objMap.insert(pair<const char*, GLMmodel*>(name, model));

		// �ۼ������� ���
		if (pResLoadedPercent != NULL) {
			*pResLoadedPercent = (int)((resourceCounter * 100) / iTotalResourceToLoad);
		}

		return true;
	}
}

bool ResourceManager::loadTexture(const char *name, bool useAlpha, bool useFlip, GLenum minFilter, GLenum magFilter, GLenum wrapFilter) {
	// �̹� ���ҽ� �ʿ� �ε�� ���ҽ����� Ȯ��
	if (getTextureID(name) != 0) {
		printf("ResourceManager::loadTexture - Texture(%s) already loaded.\n", name);
		return false;
	}

	// OpenCV�� �̿��Ͽ� �̹��� ������ ����
	int cvtFilter = CV_BGR2RGB;
	int colorFormat = GL_RGB;
	GLuint texID = 0;
	cv::Mat img = cv::imread(name, CV_LOAD_IMAGE_UNCHANGED); // 8 unsigned char with 4 channel = 32 bits (0~255)
	if (img.data == NULL) { // �̹��� ���� ����
		printf("ResourceManager::loadTexture - Texture(%s) img open has failed.\n", name);
		return false;
	}

	// ���� ���� ���� (BGR -> RGB)
	if (useAlpha == true) {
		cvtFilter = CV_BGRA2RGBA;
		colorFormat = GL_RGBA;
	}
	cv::cvtColor(img, img, cvtFilter);

	// Flip �ɼ��� ������� (�߿�:"����") ��� �̹��� ȸ�� (CV -> GL ���� ����)
	if (useFlip == false)
		cv::flip(img, img, 0);

	// �ؽ�ó ����
	glGenTextures(1, &texID);
	glBindTexture(GL_TEXTURE_2D, texID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapFilter);

	glTexImage2D(GL_TEXTURE_2D, 0, colorFormat, img.cols, img.rows, 0, colorFormat, GL_UNSIGNED_BYTE, img.ptr());

	if (texID == 0) { // �ؽ�ó �ε� ����
		printf("ResourceManager::loadTexture - Texture(%s) load has failed.\n", name);
		return false;
	}
	else { // �ؽ�ó �ε� ����
		++resourceCounter;

		// �ؽ�ó ���ҽ� �ʿ� ����
		textureMap.insert(pair<const char*, GLuint>(name, texID));
		printf("ResourceManager::loadTexture - Texture(%s) load has been completed! (ID:%d)\n", name, texID);

		// �ۼ������� ���
		if (pResLoadedPercent != NULL) {
			*pResLoadedPercent = (int)((resourceCounter * 100) / iTotalResourceToLoad);
		}

		return true;
	}
}

bool ResourceManager::loadWavSound(string name) {
	// �̹� ���ҽ� �ʿ� �ε�� ���ҽ����� Ȯ��
	if (getSoundBufferPtr(name) != NULL) {
		printf("ResourceManager::loadWavSound - Sound(%s) already loaded.\n", name);
		return false;
	}

	// .wav ���� ������ �ε�
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

	// OpenAL ���� ���� ����
	SoundBuffer *pSndBuffer = new SoundBuffer();
	pSndBuffer->fileName = string(name); // ���ϸ�

	alGenBuffers(1, &pSndBuffer->ID); // AL ���� ����

	if (bitsPerSample == 8) { // ���� ����
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

	pSndBuffer->frequency = sampleRate; // ���ļ�

	alBufferData(pSndBuffer->ID, pSndBuffer->format, buf, dataSize, pSndBuffer->frequency); // ���ۿ� ������ ���ε�

	// ���� ��� �ð� ����
	ALfloat playtime = 0.00;

	switch (pSndBuffer->format) {
	case AL_FORMAT_MONO8:		playtime = 8.0;		break;
	case AL_FORMAT_STEREO8:		playtime = 16.0;	break;
	case AL_FORMAT_MONO16:		playtime = 16.0;	break;
	case AL_FORMAT_STEREO16:	playtime = 32.0;	break;
	} // ��� = 1, ���׷��� = 2 �� ��Ʈ���� ����

	playtime *= sampleRate;				// ���ļ��� ����
	playtime /= 8.0;					// ����Ʈ�� ǥ���ϱ� ���� 8�� ����
	playtime /= dataSize;				// ������ ũ��� ����� ��ü ���̸� ���
	pSndBuffer->playtime = playtime;	// ���� ���� ����ü�� ����

	// ������ ���� �� ���� �ݱ�
	fclose(fp);
	delete[] buf;
	buf = NULL;

	// ���� �˻�
//	int err = 0;
//	if ((err = alGetError()) != 0) {
//		printf("ResourceManager::loadWavSound - OpenAL error: %d\n", err);
//		return false;
//	}

	// ���� ���� �ʿ� ����
	soundBufferMap.insert(pair<string, SoundBuffer*>(name, pSndBuffer));

	// ���ҽ� ���� ����
	++resourceCounter;

	// �ۼ������� ���
	if (pResLoadedPercent != NULL) {
		*pResLoadedPercent = (int)((resourceCounter * 100) / iTotalResourceToLoad);
	}

	// ���� ��ȯ
	return true;
}

bool ResourceManager::remove(const char *name) {
	return true;
}

bool ResourceManager::removeAll() {
	return true;
}
