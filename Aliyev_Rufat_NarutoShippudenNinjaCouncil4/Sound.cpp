#include "pch.h"
#include "Sound.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <iostream>

Sound* Sound::m_pInstance{ nullptr };

Sound* Sound::GetInstance()
{
	if (!m_pInstance)
	{
		m_pInstance = new Sound();
	}
	return m_pInstance;
}

Sound::Sound()
	:m_SEAvatarAttack1{ "Resources/Sounds/AvatarAttack1.wav" }
	, m_SEAvatarAttack2{ "Resources/Sounds/AvatarAttack2.wav" }
	, m_SEAvatarAttack3{ "Resources/Sounds/AvatarAttack3.wav" }
	, m_SEAvatarDamage{ "Resources/Sounds/AvatarDamage.wav" }
	, m_SEAvatarDamage2{ "Resources/Sounds/AvatarDamage2.wav" }
	, m_SEAvatarStartSound{ "Resources/Sounds/AvatarStartVoice.wav" }
	, m_SEAvatarTextOnTeleportEvent{ "Resources/Sounds/AvatarTextOnTeleportEvent.wav" }
	, m_SEAvatarSpecialAttackBegin{ "Resources/Sounds/AvatarSpecialAttackBegin.wav" }
	, m_SEAvatarSpecialAttack{ "Resources/Sounds/AvatarSpecialAttack.wav" }
	, m_SEAvatarJump{ "Resources/Sounds/AvatarJump.wav" }
	, m_SEAvatarThrow{ "Resources/Sounds/AvatarThrow.wav" }
	, m_SEAvatarDead{ "Resources/Sounds/AvatarDead.wav" }
	, m_SSBackgroundMusic{ "Resources/Sounds/BackgroundMusic.mp3" }
	, m_SEEnemyAttack{ "Resources/Sounds/EnemyAttack.wav" }
	, m_SEEnemyThrow{ "Resources/Sounds/EnemyThrow.wav" }
	, m_SEEnemySpecialAttack{ "Resources/Sounds/EnemySpecialAttack.wav" }
	, m_SEExplosion{ "Resources/Sounds/Explosion.wav" }
	, m_SSStartScreenMusic{ "Resources/Sounds/StartScreenMusic.mp3" }
	, m_SSGameOverMusic{ "Resources/Sounds/GameOverMusic.mp3" }
	, m_SEPause{ "Resources/Sounds/Pause.wav" }
	, m_PreviousVolume{ 30 }
{
	m_SEAvatarAttack1.SetVolume(m_PreviousVolume);
	m_SEAvatarAttack2.SetVolume(m_PreviousVolume);
	m_SEAvatarAttack3.SetVolume(m_PreviousVolume);
	m_SEAvatarDamage.SetVolume(m_PreviousVolume);
	m_SEAvatarDamage2.SetVolume(m_PreviousVolume);
	m_SEAvatarStartSound.SetVolume(m_PreviousVolume);
	m_SEAvatarTextOnTeleportEvent.SetVolume(m_PreviousVolume);
	m_SEAvatarSpecialAttackBegin.SetVolume(m_PreviousVolume);
	m_SEAvatarSpecialAttack.SetVolume(m_PreviousVolume);
	m_SEAvatarJump.SetVolume(m_PreviousVolume);
	m_SEAvatarThrow.SetVolume(m_PreviousVolume);
	m_SEAvatarDead.SetVolume(m_PreviousVolume);
	m_SEEnemyAttack.SetVolume(m_PreviousVolume);
	m_SEEnemyThrow.SetVolume(m_PreviousVolume);
	m_SEEnemySpecialAttack.SetVolume(m_PreviousVolume);
	m_SEExplosion.SetVolume(m_PreviousVolume);
	//
	m_SSBackgroundMusic.SetVolume(30);
	m_SSStartScreenMusic.SetVolume(30);
	m_SSGameOverMusic.SetVolume(100);
}

void Sound::PlaySoundEffect(int number)const
{
	switch (number)
	{
	case 0:
		m_SEAvatarAttack1.Play(0);
		break;
	case 1:
		m_SEAvatarAttack2.Play(0);
		break;
	case 2:
		m_SEAvatarAttack3.Play(0);
		break;
	case 3:
		m_SEAvatarDamage.Play(0);
		break;
	case 4:
		m_SEAvatarDamage2.Play(0);
		break;
	case 5:
		m_SEAvatarStartSound.Play(0);
		break;
	case 6:
		m_SEAvatarTextOnTeleportEvent.Play(0);
		break;
	case 7:
		m_SEAvatarSpecialAttackBegin.Play(0);
		break;
	case 8:
		m_SEAvatarSpecialAttack.Play(0);
		break;
	case 9:
		m_SEAvatarJump.Play(0);
		break;
	case 10:
		m_SEAvatarThrow.Play(0);
		break;
	case 11:
		m_SEAvatarDead.Play(0);
		break;
	case 12:
		m_SEEnemyAttack.Play(0);
		break;
	case 13:
		m_SEEnemyThrow.Play(0);
		break;
	case 14:
		m_SEEnemySpecialAttack.Play(0);
		break;
	case 15:
		m_SEExplosion.Play(0);
		break;
	case 16:
		m_SEPause.Play(0);
		break;
	}
}

void Sound::PlaySoundStream(int number)const
{
	switch (number)
	{
	case 0:
		m_SSBackgroundMusic.Play(true);
		break;
	case 1:
		m_SSStartScreenMusic.Play(true);
		break;
	case 2:
		m_SSGameOverMusic.Play(true);
		break;
	}
}

void Sound::PauseAllSounds()
{
	SoundStream::Pause();
	//
	m_SEAvatarAttack1.SetVolume(0);
	m_SEAvatarAttack2.SetVolume(0);
	m_SEAvatarAttack3.SetVolume(0);
	m_SEAvatarDamage.SetVolume(0);
	m_SEAvatarDamage2.SetVolume(0);
	m_SEAvatarStartSound.SetVolume(0);
	m_SEAvatarTextOnTeleportEvent.SetVolume(0);
	m_SEAvatarSpecialAttackBegin.SetVolume(0);
	m_SEAvatarSpecialAttack.SetVolume(0);
	m_SEAvatarJump.SetVolume(0);
	m_SEAvatarThrow.SetVolume(0);
	m_SEAvatarDead.SetVolume(0);
	m_SEEnemyAttack.SetVolume(0);
	m_SEEnemyThrow.SetVolume(0);
	m_SEEnemySpecialAttack.SetVolume(0);
	m_SEExplosion.SetVolume(0);
}

void Sound::ResumeAllSounds()
{
	SoundStream::Resume();
	//
	m_SEAvatarAttack1.SetVolume(m_PreviousVolume);
	m_SEAvatarAttack2.SetVolume(m_PreviousVolume);
	m_SEAvatarAttack3.SetVolume(m_PreviousVolume);
	m_SEAvatarDamage.SetVolume(m_PreviousVolume);
	m_SEAvatarDamage2.SetVolume(m_PreviousVolume);
	m_SEAvatarStartSound.SetVolume(m_PreviousVolume);
	m_SEAvatarTextOnTeleportEvent.SetVolume(m_PreviousVolume);
	m_SEAvatarSpecialAttackBegin.SetVolume(m_PreviousVolume);
	m_SEAvatarSpecialAttack.SetVolume(m_PreviousVolume);
	m_SEAvatarJump.SetVolume(m_PreviousVolume);
	m_SEAvatarThrow.SetVolume(m_PreviousVolume);
	m_SEAvatarDead.SetVolume(m_PreviousVolume);
	m_SEEnemyAttack.SetVolume(m_PreviousVolume);
	m_SEEnemyThrow.SetVolume(m_PreviousVolume);
	m_SEEnemySpecialAttack.SetVolume(m_PreviousVolume);
	m_SEExplosion.SetVolume(m_PreviousVolume);
}

void Sound::SetSoundStreamVolume(int& volume)
{
	SoundStream::SetVolume(volume);
}

void Sound::SetSoundEffectVolume(int& volume)
{
	m_SEAvatarAttack1.SetVolume(volume);
	m_SEAvatarAttack2.SetVolume(volume);
	m_SEAvatarAttack3.SetVolume(volume);
	m_SEAvatarDamage.SetVolume(volume);
	m_SEAvatarDamage2.SetVolume(volume);
	m_SEAvatarStartSound.SetVolume(volume);
	m_SEAvatarTextOnTeleportEvent.SetVolume(volume);
	m_SEAvatarSpecialAttackBegin.SetVolume(volume);
	m_SEAvatarSpecialAttack.SetVolume(volume);
	m_SEAvatarJump.SetVolume(volume);
	m_SEAvatarThrow.SetVolume(volume);
	m_SEAvatarDead.SetVolume(volume);
	m_SEEnemyAttack.SetVolume(volume);
	m_SEEnemyThrow.SetVolume(volume);
	m_SEEnemySpecialAttack.SetVolume(volume);
	m_SEExplosion.SetVolume(volume);
	m_PreviousVolume = volume;
}

void Sound::IncreaseSoundEffectVolume(int volumeIncrease)
{
	if (m_PreviousVolume < 100)
	{
		m_PreviousVolume += volumeIncrease;
	}
	SetSoundEffectVolume(m_PreviousVolume);
	//m_SEAvatarAttack1.SetVolume(m_SEAvatarAttack1.GetVolume() + volumeIncrease);
	//m_SEAvatarAttack2.SetVolume(m_SEAvatarAttack2.GetVolume() + volumeIncrease);
	//m_SEAvatarAttack3.SetVolume(m_SEAvatarAttack3.GetVolume() + volumeIncrease);
	//m_SEAvatarDamage.SetVolume(m_SEAvatarDamage.GetVolume() + volumeIncrease);
	//m_SEAvatarDamage2.SetVolume(m_SEAvatarDamage2.GetVolume() + volumeIncrease);
	//m_SEAvatarStartSound.SetVolume(m_SEAvatarStartSound.GetVolume() + volumeIncrease);
	//m_SEAvatarTextOnTeleportEvent.SetVolume(m_SEAvatarTextOnTeleportEvent.GetVolume() + volumeIncrease);
	//m_SEAvatarSpecialAttackBegin.SetVolume(m_SEAvatarSpecialAttackBegin.GetVolume() + volumeIncrease);
	//m_SEAvatarSpecialAttack.SetVolume(m_SEAvatarSpecialAttack.GetVolume() + volumeIncrease);
	//m_SEAvatarJump.SetVolume(m_SEAvatarJump.GetVolume() + volumeIncrease);
	//m_SEAvatarThrow.SetVolume(m_SEAvatarThrow.GetVolume() + volumeIncrease);
	//m_SEAvatarDead.SetVolume(m_SEAvatarDead.GetVolume() + volumeIncrease);
	//m_SEEnemyAttack.SetVolume(m_SEEnemyAttack.GetVolume() + volumeIncrease);
	//m_SEEnemyThrow.SetVolume(m_SEEnemyThrow.GetVolume() + volumeIncrease);
	//m_SEEnemySpecialAttack.SetVolume(m_SEEnemySpecialAttack.GetVolume() + volumeIncrease);
	//m_SEExplosion.SetVolume(m_SEExplosion.GetVolume() + volumeIncrease);
}

void Sound::DecreaseSoundEffectVolume(int volumeDecrease)
{
	if (m_PreviousVolume > 0)
	{
		m_PreviousVolume -= volumeDecrease;
	}
	SetSoundEffectVolume(m_PreviousVolume);
	//m_SEAvatarAttack1.SetVolume(m_SEAvatarAttack1.GetVolume() - volumeDecrease);
	//m_SEAvatarAttack2.SetVolume(m_SEAvatarAttack2.GetVolume() - volumeDecrease);
	//m_SEAvatarAttack3.SetVolume(m_SEAvatarAttack3.GetVolume() - volumeDecrease);
	//m_SEAvatarDamage.SetVolume(m_SEAvatarDamage.GetVolume() - volumeDecrease);
	//m_SEAvatarDamage2.SetVolume(m_SEAvatarDamage2.GetVolume() - volumeDecrease);
	//m_SEAvatarStartSound.SetVolume(m_SEAvatarStartSound.GetVolume() - volumeDecrease);
	//m_SEAvatarTextOnTeleportEvent.SetVolume(m_SEAvatarTextOnTeleportEvent.GetVolume() - volumeDecrease);
	//m_SEAvatarSpecialAttackBegin.SetVolume(m_SEAvatarSpecialAttackBegin.GetVolume() - volumeDecrease);
	//m_SEAvatarSpecialAttack.SetVolume(m_SEAvatarSpecialAttack.GetVolume() - volumeDecrease);
	//m_SEAvatarJump.SetVolume(m_SEAvatarJump.GetVolume() - volumeDecrease);
	//m_SEAvatarThrow.SetVolume(m_SEAvatarThrow.GetVolume() - volumeDecrease);
	//m_SEAvatarDead.SetVolume(m_SEAvatarDead.GetVolume() - volumeDecrease);
	//m_SEEnemyAttack.SetVolume(m_SEEnemyAttack.GetVolume() - volumeDecrease);
	//m_SEEnemyThrow.SetVolume(m_SEEnemyThrow.GetVolume() - volumeDecrease);
	//m_SEEnemySpecialAttack.SetVolume(m_SEEnemySpecialAttack.GetVolume() - volumeDecrease);
	//m_SEExplosion.SetVolume(m_SEExplosion.GetVolume() - volumeDecrease);
}