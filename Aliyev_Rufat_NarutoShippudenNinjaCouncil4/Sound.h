#pragma once
#include "SoundEffect.h"
#include "SoundStream.h"

class Sound
{
public:
	static Sound* GetInstance();
	void PlaySoundEffect(int number) const;
	void PlaySoundStream(int number)const;
	void PauseAllSounds();
	void ResumeAllSounds();
	void SetSoundStreamVolume(int& volume);
	void SetSoundEffectVolume(int& volume);
	//
	void IncreaseSoundEffectVolume(int volumeIncrease);
	void DecreaseSoundEffectVolume(int volumeDecrease);

private:
	Sound();
	static Sound* m_pInstance;
	//
	SoundEffect m_SEAvatarAttack1;
	SoundEffect m_SEAvatarAttack2;
	SoundEffect m_SEAvatarAttack3;
	SoundEffect m_SEAvatarDamage;
	SoundEffect m_SEAvatarDamage2;
	SoundEffect m_SEAvatarStartSound;
	SoundEffect m_SEAvatarTextOnTeleportEvent;
	SoundEffect m_SEAvatarSpecialAttackBegin;
	SoundEffect m_SEAvatarSpecialAttack;
	SoundEffect m_SEAvatarJump;
	SoundEffect m_SEAvatarThrow;
	SoundEffect m_SEAvatarDead;
	//
	SoundEffect m_SEEnemyAttack;
	SoundEffect m_SEEnemyThrow;
	SoundEffect m_SEEnemySpecialAttack;
	SoundEffect m_SEExplosion;
	//
	SoundEffect m_SEPause;
	//
	SoundStream m_SSBackgroundMusic;
	SoundStream m_SSStartScreenMusic;
	SoundStream m_SSGameOverMusic;
	//
	int m_PreviousVolume;
};
