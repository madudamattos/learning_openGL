#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

// this class is a singleton
// make it static

class GameManager
{
    int lifePlayer1, lifePlayer2;
    public:
        GameManager()
        {
            this->lifePlayer1 = 3;
            this->lifePlayer2 = 3;
        }
};

#endif