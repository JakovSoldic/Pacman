#include <iostream>
#include <windows.h>
#include <stdio.h>
using namespace std;
#pragma comment(lib, "Winmm.lib")

#include "Music.h"

void Music::playIntro()
{
    if (introPlayed)
        return;

    PlaySound(L"./MusicPlaylist/pacman_beginning.wav", NULL, SND_FILENAME | SND_SYNC);

    introPlayed = true;
}

void Music::playMovementSound()
{
    if (!movementSoundPlayed)
    {
        mciSendString(L"open \"./MusicPlaylist/pacman_chomp.wav\" type mpegvideo alias chomp", NULL, 0, NULL);
        mciSendString(L"set chomp speed 4", NULL, 0, NULL);
        mciSendString(L"play chomp repeat", NULL, 0, NULL);

        movementSoundPlayed = true;
    }
}

void Music::stopMovementSound()
{
    mciSendString(L"stop chomp", NULL, 0, NULL);
    mciSendString(L"close chomp", NULL, 0, NULL);
}

void Music::playAteGhost()
{
    PlaySound(L"./MusicPlaylist/pacman_eatghost.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void Music::playDeath()
{
    PlaySound(L"./MusicPlaylist/pacman_death.wav", NULL, SND_FILENAME | SND_ASYNC);
}

void Music::stopDeath()
{
    PlaySound(NULL, NULL, SND_PURGE);
}