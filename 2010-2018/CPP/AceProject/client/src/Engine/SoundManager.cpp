#include "Engine/SoundManager.h"
#include "Engine/ResourceManager.h"
#include "Engine/DrawManager.h"
#include "Engine/GameCamera.h"
#include "Engine/GameWidget.h"
#include "Engine/GameFrame.h"
#include "Game/Object/Player.h"

/*************************************************************************************************************/
/*************************************************** Sound ***************************************************/
/*************************************************************************************************************/

Sound::Sound(Vector3 pos, string soundFileName, ALboolean use3D, ALboolean loop) : GameObject() {
	ID = 0;
	channelNum = -1; // -1 => 모두에게 재생
	state = AL_STOPPED;
	position = pos;
	velocity = Vector3(0.0, 0.0, 0.0);
	gain = 1.0;
	pitch = 1.0;
	looping = loop;
	pSndData = g_pGameManager->getResourceManagerPtr()->getSoundBufferPtr(soundFileName);
	use3DSound = use3D;
	pAttachTargetWidget = NULL;

	// 데이터 오류가 없을 시
	if (pSndData != NULL) {
		currentTime = pSndData->playtime;
		// OpenAL 사운드 소스 셋팅
		ALfloat vec3Pos[3] = { position.x, position.y, position.z };
		ALfloat vec3Vel[3] = { velocity.x, velocity.y, velocity.z };
		alGenSources(1, &ID);
		alSourcei(ID, AL_BUFFER, pSndData->ID);
		alSourcef(ID, AL_PITCH, pitch);
		alSourcef(ID, AL_GAIN, gain);
		alSourcefv(ID, AL_POSITION, vec3Pos);
		alSourcefv(ID, AL_VELOCITY, vec3Vel);
		alSourcei(ID, AL_LOOPING, looping);

		// 사운드 재생
		play();

		// 사운드 매니저에 추가
		g_pGameManager->getSoundManagerPtr()->addSound(this);
	}
}

Sound::Sound(Sound &obj) : GameObject(obj) {
	ID = obj.ID;
	channelNum = obj.channelNum;
	currentTime = obj.currentTime;
	state = obj.state;
	position = obj.position;
	velocity = obj.velocity;
	gain = obj.gain;
	pitch = obj.pitch;
	looping = obj.looping;
	pSndData = obj.pSndData;
	pAttachTargetWidget = obj.pAttachTargetWidget;
}

Sound::~Sound() {
	alDeleteSources(1, &ID);
	pSndData = NULL;
	pAttachTargetWidget = NULL;
}

ALuint Sound::getID() {
	return ID;
}

void Sound::setChannelNum(ALint number) {
	channelNum = number;
}

ALint Sound::getChannelNum() {
	return channelNum;
}

void Sound::setPosition(Vector3 pos) {
	position = pos;
}

Vector3 Sound::getPosition() {
	return position;
}

void Sound::setVelocity(Vector3 vel) {
	velocity = vel;
}

Vector3 Sound::getVelocity() {
	return velocity;
}

void Sound::setGain(REAL amount) {
	if (amount < 0.0) amount = 0.0;

	gain = amount;
}

REAL Sound::getGain() {
	return gain;
}

void Sound::setPitch(REAL newPitch) {
	pitch = newPitch;
}

REAL Sound::getPitch() {
	return pitch;
}

void Sound::setCurrentTime(REAL time) {
	currentTime = time;
}

REAL Sound::getCurrentTime() {
	return currentTime;
}

REAL Sound::getSoundPlayTime() {
	return (pSndData != NULL) ? pSndData->playtime : 0.00;
}

ALenum Sound::getState() {
	return state;
}

void Sound::setLooping(ALboolean loop) {
	looping = loop;
}

ALboolean Sound::getLooping() {
	return looping;
}

void Sound::attach(GameWidget *pWidget) {
	pAttachTargetWidget = pWidget;
}

void Sound::play() {
	alGetSourcei(ID, AL_SOURCE_STATE, &state); // 상태 검사를 위해 받아옴
	if (state != AL_PLAYING) {
		alSourcePlay(ID);							// OpenAL 에게 ID번 소스를 재생하도록 명령
		alGetSourcei(ID, AL_SOURCE_STATE, &state);	// 변경된 상태를 state에 저장
	}
}

void Sound::pause() {
	alSourcePause(ID);
	alGetSourcei(ID, AL_SOURCE_STATE, &state);
}

void Sound::stop() {
	alSourceStop(ID);
	alGetSourcei(ID, AL_SOURCE_STATE, &state);
}

void Sound::destory() {
	stop();					// 재생을 정지하고
	looping = false;		// 루프 제거
	currentTime = 0.00;		// 음악을 종료 시간으로 설정하여 다음 update시 파괴되도록 함
}

bool Sound::update(REAL timeElapsed) {
	// 시간 경과
	if (state == AL_PLAYING)
		currentTime -= timeElapsed;

	// 상태 받아옴
	alGetSourcei(ID, AL_SOURCE_STATE, &state);

	// 3차원 사운드가 아닐 경우 항상 카메라의 위치에 사운드를 위치시킴
	if (use3DSound == false) {
		GameCamera *pCam = NULL;
		if ((pCam = g_pGameManager->getDrawManagerPtr()->getCameraFromFrame()) != NULL) {
			position = pCam->getPosition();
		}
	}
	// 3차원 사운드 일 때
	else {
		// 부착된 대상이 있으면 부착 대상의 위치로 이동
		if (pAttachTargetWidget != NULL) {
			position = pAttachTargetWidget->getPos();
		}
	}

	// 음악이 정지되어 있다면
	if (state == AL_STOPPED) {
		if (looping == false) return false; // 루프 재생이 아닐 시 파괴
	}

	// 위치 등 정보 업데이트
	ALfloat vec3Pos[3] = { position.x, position.y, position.z };
	ALfloat vec3Vel[3] = { velocity.x, velocity.y, velocity.z };
	alSourcei(ID, AL_BUFFER, pSndData->ID);
	alSourcef(ID, AL_PITCH, pitch);
	alSourcef(ID, AL_GAIN, gain);
	alSourcefv(ID, AL_POSITION, vec3Pos);
	alSourcefv(ID, AL_VELOCITY, vec3Vel);
	alSourcei(ID, AL_LOOPING, looping);

	// 성공 반환
	return true;
}

/*************************************************************************************************************/
/************************************************ SoundManager ***********************************************/
/*************************************************************************************************************/


SoundManager::SoundManager() {
	// 기본 출력 장치 오픈
	if ((pSoundDevice = alcOpenDevice(NULL)) == NULL) {
		MessageBox(NULL, TEXT("SoundManager::SoundManager() - Fail to open sound device. (Did you install 'oalinst'?)"), TEXT("SoundManager - Device open error."), (MB_OK | MB_ICONINFORMATION));
		throw ("SoundManager - No sound device detected...\n");
	}

	// 사운드 콘텍스트 생성 및 적용
	if ((pSoundContext = alcCreateContext(pSoundDevice, NULL)) == NULL) {
		MessageBox(NULL, TEXT("SoundManager::SoundManager() - Fail to create sound context. (Did you install 'oalinst'?)"), TEXT("SoundManager - Sound context creation error."), (MB_OK | MB_ICONINFORMATION));
		throw ("SoundManager - Fail to create sound context...\n");
	}
	alcMakeContextCurrent(pSoundContext);

	// 리스너 초기화
	listener.position = Vector3(0.0, 0.0, 0.0);
	listener.velocity = Vector3(0.0, 0.0, 0.0);
	listener.dirVector = Vector3(0.0, 0.0, 0.0);
	listener.upVector = Vector3(0.0, 0.0, 0.0);

	// 사운드 리스트 초기화
	soundList.clear();

	// 채널 초기화
	channelNum = -1; // 플레이어 번호에 맞게 갱신해 주어야 사운드 재생에 문제가 없음
}

SoundManager::SoundManager(SoundManager &obj) {
	pSoundDevice = obj.pSoundDevice;
	pSoundContext = obj.pSoundContext;
	soundList = obj.soundList;
}

SoundManager::~SoundManager() {
	// 사운드 콘텍스트 삭제
	if (pSoundContext != NULL) {
		alcDestroyContext(pSoundContext);
	}
	pSoundContext = NULL;

	// 출력 장치 닫기
	if (pSoundDevice != NULL) {
		alcCloseDevice(pSoundDevice);
	}
	pSoundDevice = NULL;

	// 리스트 초기화
	soundList.clear();
}

void SoundManager::setListenerVelocity(Vector3 vel) {
	listener.velocity = vel;
}

Vector3 SoundManager::getListenerVelocity() {
	return listener.velocity;
}

bool SoundManager::addSound(Sound *pSound) {
	soundList.push_back(pSound);

	return true;
}

bool SoundManager::destroySound(Sound *pSound) {
	list<Sound*>::iterator iter = find(soundList.begin(), soundList.end(), pSound);

	if (iter != soundList.end()) {
		(*iter)->setLooping(false);
		(*iter)->stop();

		return true;
	}

	return false;
}

bool SoundManager::destroySoundAll() {
	list<Sound*>::iterator iter = soundList.begin();

	for (; iter != soundList.end(); ++iter) {
		(*iter)->setLooping(false);
		(*iter)->stop();
	}

	return true;
}

bool SoundManager::update(REAL timeElapsed) {
	// 사운드 채널 갱신
	channelNum = g_pGameManager->getGameFramePtr()->getOwnerPlayerPtr()->getPlayerNumber();

	// 리스너 갱신
	GameCamera *pCam = g_pGameManager->getDrawManagerPtr()->getCameraFromFrame();
	if (pCam != NULL) {
		listener.position = pCam->getPosition();
		listener.velocity = pCam->getVelocity();
		listener.dirVector = pCam->getViewDirection();
		listener.upVector = pCam->getYAxis();

		REAL vec[6] = { listener.position.x, listener.position.y, listener.position.z, 0.0, 0.0, 0.0 };
		alListenerfv(AL_POSITION, vec);
		vec[0] = listener.velocity.x, vec[1] = listener.velocity.y, vec[2] = listener.velocity.z;
		alListenerfv(AL_VELOCITY, vec);
		vec[0] = listener.dirVector.x, vec[1] = listener.dirVector.y, vec[2] = listener.dirVector.z;
		vec[3] = listener.upVector.x, vec[4] = listener.upVector.y, vec[5] = listener.upVector.z;
		alListenerfv(AL_ORIENTATION, vec);
	}

	// 생명 시간 계산
	Sound *pSound = NULL;
	ALint channel = -1;
	for (list<Sound*>::iterator iter = soundList.begin(); iter != soundList.end();) {
		pSound = (*iter);
		// 특정 플레이어(채널) 전용 사운드인지 검사
		channel = pSound->getChannelNum();
		if (channel != -1 && channel != channelNum) { // 공용 채널 (-1)이 아니고 자신의 채널이 아니면
			pSound->stop(); // 해당 사운드 정지하여 파괴 유도
		}
		if (pSound->update(timeElapsed) == false) { // 사운드로부터 파괴 요청이 반환 될 경우
			delete pSound;							// 동적 할당 해제
			iter = soundList.erase(iter);			// 리스트에서 삭제
		}
		else {
			++iter;
		}
	}

	return true;
}