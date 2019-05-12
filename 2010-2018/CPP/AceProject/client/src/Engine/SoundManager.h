#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "Engine/StdHeader.h"
#include "openal/include/al.h"
#include "openal/include/alc.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   [ ���� ]
*
*	> SoundBuffer -> ResourceManager -> "class Sound" -> SoundManager;
*	  (�Ҹ� ������)  (�Ҹ� ������ ����) (�Ҹ� Ŭ����)    (�Ҹ� Ŭ���� ����)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct SoundBuffer {
	ALuint ID;			// ���� ���̵�
	string fileName;	// ���� ���ϸ�
	ALuint frequency;	// ���� ���ļ�
	ALenum format;		// ���� ����
	ALfloat playtime;	// ���� ��� �ð�

	inline SoundBuffer() {
		ID = 0;
		fileName = "";
		frequency = 0;
		format = 0;
	}
};

struct SoundListener {
	Vector3 position;	// ������ ��ġ
	Vector3 velocity;	// ������ �ӵ�
	Vector3 dirVector;	// �����ʰ� ���⺤��
	Vector3 upVector;	// ������ �� ������
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Sound (v2)
*	- Last Update : 2016.10.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���ӿ��� ����Ǵ� ���� ������ ���� Ŭ����.
*
*	- Dependency : [StdHeader.h, al.h, alc.h], ResourceManager.h, DrawManager.h, GameCamera.h
*
*	- Version(v1 - 16.08.16) : Ŭ���� ���� ����.
*	- Version(v2 - 16.10.16) : �޼ҵ� Pitch�� ����, ���� �߰�.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class GameWidget;

class Sound : public GameObject {
private:
	ALuint ID;						// �ҽ� ���̵�
	ALint channelNum;				// ä�� ��ȣ
	REAL currentTime;				// �ҽ��� ���� ��� �ð�
	ALint state;					// ������ ���� ����
	Vector3 position;				// �Ҹ��� ��ġ
	Vector3 velocity;				// �Ҹ��� �ӵ� �� ����
	ALfloat gain;					// �Ҹ� ũ��
	ALfloat pitch;					// �Ҹ� ��ġ
	ALboolean looping;				// �ݺ� ��� ���� (Default: AL_FALSE)
	ALfloat use3DSound;				// 3���� ���� ��� ����
	SoundBuffer *pSndData;			// ���� ������
	GameWidget *pAttachTargetWidget;// ������ ���� ������

public:
	// Con & Copy & Des
	Sound(Vector3 pos, string soundFileName, ALboolean use3D, ALboolean loop);
	Sound(Sound &obj);
	~Sound();

	// Setter & Getter
	ALuint getID();
	void setChannelNum(ALint number);
	ALint getChannelNum();
	void setPosition(Vector3 pos);
	Vector3 getPosition();
	void setVelocity(Vector3 vel);
	Vector3 getVelocity();
	void setGain(REAL amount);
	REAL getGain();
	void setPitch(REAL newPitch);
	REAL getPitch();
	void setCurrentTime(REAL time);
	REAL getCurrentTime();
	REAL getSoundPlayTime();			// ������ ��� ���̸� ��ȯ
	ALenum getState();
	void setLooping(ALboolean loop);
	ALboolean getLooping();

	// Method
	void attach(GameWidget *pWidget);	// �ش� ������ ����
	void play();						// ���� ���
	void pause();						// ���� �Ͻ� ����
	void stop();						// ���� ����
	void destory();						// ���� �ı�

	virtual bool update(REAL timeElapsed);	// ������Ʈ
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : SoundManager (v1)
*	- Last Update : 2016.08.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : ���ӿ��� ����Ǵ� ��� ���带 �����ϴ� Ŭ����. OpenAL ������� �ۼ�.
*
*	- Dependency : [StdHeader.h, al.h, alc.h], DrawManager.h, GameCamera.h
*
*	- Version(v1 - 16.08.16) : Ŭ���� ���� ����.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class SoundManager : public GameObject {
private:
	ALint channelNum;					// Ư�� ä��(�÷��̾�)�� ���� ���常 ����ϱ� ���� ���� 
	ALCdevice *pSoundDevice;			// ���� ����̽� ������
	ALCcontext *pSoundContext;			// ���� ���ؽ�Ʈ ������
	SoundListener listener;				// ���带 ��� �������� ������ ��� ����ü

	list<Sound*> soundList;				// ������� ��� ����Ʈ

public:
	SoundManager();						// ������
	SoundManager(SoundManager &obj);	// ���� ������
	~SoundManager();					// �Ҹ���

	void setListenerVelocity(Vector3 vel);	// �������� �ӵ��� ���� (���÷�ȿ���� ���� ���� ����� ����)
	Vector3 getListenerVelocity();			// �������� �ӵ��� ��ȯ

	void setChannelNum(ALint);			// ä�� ��ȣ�� ����
	ALint getChannelNum();				// ä�� ��ȣ�� ��ȯ
	bool addSound(Sound *pSound);		// ���带 ����Ʈ�� �߰�
	bool destroySound(Sound *pSound);	// �ش� ���� �ı�
	bool destroySoundAll();				// ��� ���� �ı�

	virtual bool update(REAL timeElapsed); // ������Ʈ
};


#endif