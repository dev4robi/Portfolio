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
	channelNum = -1; // -1 => ��ο��� ���
	state = AL_STOPPED;
	position = pos;
	velocity = Vector3(0.0, 0.0, 0.0);
	gain = 1.0;
	pitch = 1.0;
	looping = loop;
	pSndData = g_pGameManager->getResourceManagerPtr()->getSoundBufferPtr(soundFileName);
	use3DSound = use3D;
	pAttachTargetWidget = NULL;

	// ������ ������ ���� ��
	if (pSndData != NULL) {
		currentTime = pSndData->playtime;
		// OpenAL ���� �ҽ� ����
		ALfloat vec3Pos[3] = { position.x, position.y, position.z };
		ALfloat vec3Vel[3] = { velocity.x, velocity.y, velocity.z };
		alGenSources(1, &ID);
		alSourcei(ID, AL_BUFFER, pSndData->ID);
		alSourcef(ID, AL_PITCH, pitch);
		alSourcef(ID, AL_GAIN, gain);
		alSourcefv(ID, AL_POSITION, vec3Pos);
		alSourcefv(ID, AL_VELOCITY, vec3Vel);
		alSourcei(ID, AL_LOOPING, looping);

		// ���� ���
		play();

		// ���� �Ŵ����� �߰�
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
	alGetSourcei(ID, AL_SOURCE_STATE, &state); // ���� �˻縦 ���� �޾ƿ�
	if (state != AL_PLAYING) {
		alSourcePlay(ID);							// OpenAL ���� ID�� �ҽ��� ����ϵ��� ���
		alGetSourcei(ID, AL_SOURCE_STATE, &state);	// ����� ���¸� state�� ����
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
	stop();					// ����� �����ϰ�
	looping = false;		// ���� ����
	currentTime = 0.00;		// ������ ���� �ð����� �����Ͽ� ���� update�� �ı��ǵ��� ��
}

bool Sound::update(REAL timeElapsed) {
	// �ð� ���
	if (state == AL_PLAYING)
		currentTime -= timeElapsed;

	// ���� �޾ƿ�
	alGetSourcei(ID, AL_SOURCE_STATE, &state);

	// 3���� ���尡 �ƴ� ��� �׻� ī�޶��� ��ġ�� ���带 ��ġ��Ŵ
	if (use3DSound == false) {
		GameCamera *pCam = NULL;
		if ((pCam = g_pGameManager->getDrawManagerPtr()->getCameraFromFrame()) != NULL) {
			position = pCam->getPosition();
		}
	}
	// 3���� ���� �� ��
	else {
		// ������ ����� ������ ���� ����� ��ġ�� �̵�
		if (pAttachTargetWidget != NULL) {
			position = pAttachTargetWidget->getPos();
		}
	}

	// ������ �����Ǿ� �ִٸ�
	if (state == AL_STOPPED) {
		if (looping == false) return false; // ���� ����� �ƴ� �� �ı�
	}

	// ��ġ �� ���� ������Ʈ
	ALfloat vec3Pos[3] = { position.x, position.y, position.z };
	ALfloat vec3Vel[3] = { velocity.x, velocity.y, velocity.z };
	alSourcei(ID, AL_BUFFER, pSndData->ID);
	alSourcef(ID, AL_PITCH, pitch);
	alSourcef(ID, AL_GAIN, gain);
	alSourcefv(ID, AL_POSITION, vec3Pos);
	alSourcefv(ID, AL_VELOCITY, vec3Vel);
	alSourcei(ID, AL_LOOPING, looping);

	// ���� ��ȯ
	return true;
}

/*************************************************************************************************************/
/************************************************ SoundManager ***********************************************/
/*************************************************************************************************************/


SoundManager::SoundManager() {
	// �⺻ ��� ��ġ ����
	if ((pSoundDevice = alcOpenDevice(NULL)) == NULL) {
		MessageBox(NULL, TEXT("SoundManager::SoundManager() - Fail to open sound device. (Did you install 'oalinst'?)"), TEXT("SoundManager - Device open error."), (MB_OK | MB_ICONINFORMATION));
		throw ("SoundManager - No sound device detected...\n");
	}

	// ���� ���ؽ�Ʈ ���� �� ����
	if ((pSoundContext = alcCreateContext(pSoundDevice, NULL)) == NULL) {
		MessageBox(NULL, TEXT("SoundManager::SoundManager() - Fail to create sound context. (Did you install 'oalinst'?)"), TEXT("SoundManager - Sound context creation error."), (MB_OK | MB_ICONINFORMATION));
		throw ("SoundManager - Fail to create sound context...\n");
	}
	alcMakeContextCurrent(pSoundContext);

	// ������ �ʱ�ȭ
	listener.position = Vector3(0.0, 0.0, 0.0);
	listener.velocity = Vector3(0.0, 0.0, 0.0);
	listener.dirVector = Vector3(0.0, 0.0, 0.0);
	listener.upVector = Vector3(0.0, 0.0, 0.0);

	// ���� ����Ʈ �ʱ�ȭ
	soundList.clear();

	// ä�� �ʱ�ȭ
	channelNum = -1; // �÷��̾� ��ȣ�� �°� ������ �־�� ���� ����� ������ ����
}

SoundManager::SoundManager(SoundManager &obj) {
	pSoundDevice = obj.pSoundDevice;
	pSoundContext = obj.pSoundContext;
	soundList = obj.soundList;
}

SoundManager::~SoundManager() {
	// ���� ���ؽ�Ʈ ����
	if (pSoundContext != NULL) {
		alcDestroyContext(pSoundContext);
	}
	pSoundContext = NULL;

	// ��� ��ġ �ݱ�
	if (pSoundDevice != NULL) {
		alcCloseDevice(pSoundDevice);
	}
	pSoundDevice = NULL;

	// ����Ʈ �ʱ�ȭ
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
	// ���� ä�� ����
	channelNum = g_pGameManager->getGameFramePtr()->getOwnerPlayerPtr()->getPlayerNumber();

	// ������ ����
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

	// ���� �ð� ���
	Sound *pSound = NULL;
	ALint channel = -1;
	for (list<Sound*>::iterator iter = soundList.begin(); iter != soundList.end();) {
		pSound = (*iter);
		// Ư�� �÷��̾�(ä��) ���� �������� �˻�
		channel = pSound->getChannelNum();
		if (channel != -1 && channel != channelNum) { // ���� ä�� (-1)�� �ƴϰ� �ڽ��� ä���� �ƴϸ�
			pSound->stop(); // �ش� ���� �����Ͽ� �ı� ����
		}
		if (pSound->update(timeElapsed) == false) { // ����κ��� �ı� ��û�� ��ȯ �� ���
			delete pSound;							// ���� �Ҵ� ����
			iter = soundList.erase(iter);			// ����Ʈ���� ����
		}
		else {
			++iter;
		}
	}

	return true;
}