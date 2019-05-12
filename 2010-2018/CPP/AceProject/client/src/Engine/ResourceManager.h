#ifndef __RESOURCE_MANAGER_H__
#define __RESOURCE_MANAGER_H__

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : ResourceManager(v2)
*	- Last Update : 2016.08.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : �� �� �̹���, ���� ���� ���ҽ� ������ �ε��Ͽ� �����ϴ� �Ŵ���.
*					 �̹��� ��û -> map���� �ش���� ������ �ִ��� Ȯ�� -> ������ �ش� ID ��ȯ.
*
*	- Dependency : [StdHeader.h, glm.h, al.h], SoundManager.h
*
*   - Version(v1): ���ҽ� �Ŵ��� ���� ����. (2016.05.15)
*	- Version(v2): OpenAL�� �̿��� ���� ���۸� ���� �߰� ����. (2016.08.16)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

#include "Engine/StdHeader.h"
#include "Library/glm/glm.h"
#include "openal/include/al.h"

struct SoundBuffer; // from "Engine/SoundManager.h"

class ResourceManager : public GameObject {
private:
	int resourceCounter;							// ���ҽ� ���� ī���� (.obj �� ���Ͽ� ���Ե� �ؽ�ó ������ 1���� ���ҽ��� �����Կ� ����)
	int *pResLoadedPercent;							// �ε� �Ϸ�� ���ҽ� ������� ������ ������ ����Ű�� ������
	int iTotalResourceToLoad;						// �ε��ؾ� �� ���ҽ��� �� ����
	map<const string, GLMmodel*> objMap;			// .obj�� ���ҽ� ��
	map<const string, GLuint> textureMap;			// texture ���ҽ� ��
	map<string, SoundBuffer*> soundBufferMap;		// ���� ����(������) ��

public:
	// ������ & �Ҹ���
	ResourceManager();
	~ResourceManager();

	// ���� & ����
	int getResourceCnt();							// �������� ���ҽ��� ������ ��ȯ
	GLMmodel* getObjModelPtr(const char *name);		// .obj���� �����͸� ��ȯ
	GLuint getTextureID(const char *name);			// �ؽ�ó�� ID�� ��ȯ
	SoundBuffer* getSoundBufferPtr(string name);	// ���� �����Ͱ� ��� ���� �����͸� ��ȯ

	// �޼ҵ�
	void setResLoadedPercentPtr(int *pPtr);			// �ε� �Ϸ�� ���ҽ� ������� ������ ������ ����
	bool loadOBJ(const char *name);					// .obj�𵨰� .mtl������ ���ҽ��� �ε��Ͽ� �����Ͽ� ��
	bool loadTexture(const char *name, bool useAlpha, bool useFlip, GLenum minFilter, GLenum magFilter, GLenum wrapFilter);	// �̹��� ������ �ε��Ͽ� GL�ؽ�ó ����
	bool loadWavSound(string name);					// .wav ���� �ε�
	bool remove(const char* name);					// �ε�� ���ҽ��� ����
	bool removeAll();								// ��� ���ҽ��� ����
};

#endif