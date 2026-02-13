#include "gameManager.h"
#include <iostream>

bool GameManager::isGameOver() const
{
    return (lifePlayer1 <= 0) || (lifePlayer2 <= 0);
}

void GameManager::decLifePlayer1()
{
    if (lifePlayer1 > 0) --lifePlayer1;
    if(lifePlayer1 <= 0) this->winner = Winner::Player2;
}

void GameManager::decLifePlayer2()
{
    if(lifePlayer2 > 0) --lifePlayer2;
    if(lifePlayer2 <= 0) this->winner = Winner::Player1; 
}

void GameManager::resetGame()
{
    lifePlayer2 = lifePlayer1 = 3;
    winner = Winner::None;
}