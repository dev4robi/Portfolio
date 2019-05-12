#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include "Engine/StdHeader.h"
#include "openal/include/al.h"
#include "openal/include/alc.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*   [ 참고 ]
*
*	> SoundBuffer -> ResourceManager -> "class Sound" -> SoundManager;
*	  (소리 데이터)  (소리 데이터 보관) (소리 클래스)    (소리 클래스 관리)
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

struct SoundBuffer {
	ALuint ID;			// 버퍼 아이디
	string fileName;	// 음원 파일명
	ALuint frequency;	// 사운드 주파수
	ALenum format;		// 사운드 포맷
	ALfloat playtime;	// 사운드 재생 시간

	inline SoundBuffer() {
		ID = 0;
		fileName = "";
		frequency = 0;
		format = 0;
	}
};

struct SoundListener {
	Vector3 position;	// 리스너 위치
	Vector3 velocity;	// 리스너 속도
	Vector3 dirVector;	// 리스너가 방향벡터
	Vector3 upVector;	// 리스너 의 업벡터
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : Sound (v2)
*	- Last Update : 2016.10.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임에서 재생되는 사운드 정보를 담은 클래스.
*
*	- Dependency : [StdHeader.h, al.h, alc.h], ResourceManager.h, DrawManager.h, GameCamera.h
*
*	- Version(v1 - 16.08.16) : 클래스 기초 정의.
*	- Version(v2 - 16.10.16) : 메소드 Pitch의 게터, 세터 추가.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class GameWidget;

class Sound : public GameObject {
private:
	ALuint ID;						// 소스 아이디
	ALint channelNum;				// 채널 번호
	REAL currentTime;				// 소스의 현재 재생 시간
	ALint state;					// 사운드의 현재 상태
	Vector3 position;				// 소리의 위치
	Vector3 velocity;				// 소리의 속도 및 방향
	ALfloat gain;					// 소리 크기
	ALfloat pitch;					// 소리 피치
	ALboolean looping;				// 반복 사용 여부 (Default: AL_FALSE)
	ALfloat use3DSound;				// 3차원 사운드 사용 여부
	SoundBuffer *pSndData;			// 음원 데이터
	GameWidget *pAttachTargetWidget;// 부착된 위젯 포인터

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
	REAL getSoundPlayTime();			// 음악의 재생 길이를 반환
	ALenum getState();
	void setLooping(ALboolean loop);
	ALboolean getLooping();

	// Method
	void attach(GameWidget *pWidget);	// 해당 위젯에 부착
	void play();						// 사운드 재생
	void pause();						// 사운드 일시 정지
	void stop();						// 사운드 정지
	void destory();						// 사운드 파괴

	virtual bool update(REAL timeElapsed);	// 업데이트
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
*
*	- Class : SoundManager (v1)
*	- Last Update : 2016.08.16
*	- Code by : Taehoon Lee (robin9202@naver.com)
*
*	- Introduction : 게임에서 재생되는 모든 사운드를 관리하는 클래스. OpenAL 기반으로 작성.
*
*	- Dependency : [StdHeader.h, al.h, alc.h], DrawManager.h, GameCamera.h
*
*	- Version(v1 - 16.08.16) : 클래스 기초 정의.
*
* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

class SoundManager : public GameObject {
private:
	ALint channelNum;					// 특정 채널(플레이어)을 가진 사운드만 재생하기 위한 구분 
	ALCdevice *pSoundDevice;			// 사운드 디바이스 포인터
	ALCcontext *pSoundContext;			// 사운드 콘텍스트 포인터
	SoundListener listener;				// 사운드를 듣는 리스너의 정보가 담긴 구조체

	list<Sound*> soundList;				// 사운드들이 담긴 리스트

public:
	SoundManager();						// 생성자
	SoundManager(SoundManager &obj);	// 복사 생성자
	~SoundManager();					// 소멸자

	void setListenerVelocity(Vector3 vel);	// 리스너의 속도를 설정 (도플러효과에 의한 사운드 재생을 위함)
	Vector3 getListenerVelocity();			// 리스너의 속도를 반환

	void setChannelNum(ALint);			// 채널 번호를 설정
	ALint getChannelNum();				// 채널 번호를 반환
	bool addSound(Sound *pSound);		// 사운드를 리스트에 추가
	bool destroySound(Sound *pSound);	// 해당 사운드 파괴
	bool destroySoundAll();				// 모든 사운드 파괴

	virtual bool update(REAL timeElapsed); // 업데이트
};


#endif