using namespace std;

typedef int positionXY[2];
const int pitch_max_X = 7;
const int pitch_max_Y = 7;

class GrabStoresGame
{
private:
    enum squareStatus
    {
        EMPTY = 0,
        PLAYER = 1,
        STORE = 2,
    };

    int pitch[pitch_max_X][pitch_max_Y] = {0};
    int grabbedStores = 0;
    int playerX = 0;
    int playerY = 0;

    void setStores(positionXY *stores, int storesCount)
    {
        for (int i = 0; i < storesCount; i++)
        {
            this->pitch[stores[i][0]][stores[i][1]] = STORE;
        }
    }

    bool playerMoveLeft()
    {
        if (this->playerX > 0)
        {
            this->playerX--;
            if (this->pitch[this->playerX][this->playerY] == STORE)
            {
                this->pitch[this->playerX][this->playerY] = EMPTY;
                this->grabbedStores++;
            }
            return true;
        }

        return false;
    }

    bool playerMoveRight()
    {
        if (this->playerX < pitch_max_X - 1)
        {
            this->playerX++;
            if (this->pitch[this->playerX][this->playerY] == STORE)
            {
                this->pitch[this->playerX][this->playerY] = EMPTY;
                this->grabbedStores++;
            }
            return true;
        }

        return false;
    }

    bool playerMoveUp()
    {
        if (this->playerY > 0)
        {
            this->playerY--;
            if (this->pitch[this->playerX][this->playerY] == STORE)
            {
                this->pitch[this->playerX][this->playerY] = EMPTY;
                this->grabbedStores++;
            }
            return true;
        }

        return false;
    }

    bool playerMoveDown()
    {
        if (this->playerY < pitch_max_Y - 1)
        {
            this->playerY++;
            if (this->pitch[this->playerX][this->playerY] == STORE)
            {
                this->pitch[this->playerX][this->playerY] = EMPTY;
                this->grabbedStores++;
            }
            return true;
        }

        return false;
    }

public:
    // GrabStoresGame(int pitch_max_X, int pitch_max_Y, positionXY player, positionXY *stores, int storesCount)
    // {
    //     pitch_max_X = pitch_max_X;
    //     pitch_max_Y = pitch_max_Y;
    //     this->pitch = new int[pitch_max_X][pitch_max_Y];
    //     this->playerX = player[0];
    //     this->playerY = player[1];

    //     this->pitch[player[0]][player[1]] = PLAYER;
    //     this.setStores(stores, storesCount);
    // }

    GrabStoresGame()
    {
        this->playerX = 3;
        this->playerY = 6;

        this->pitch[3][6] = PLAYER;
        positionXY stores[5] = {{4, 1}, {2, 2}, {6, 3}, {1, 4}, {4, 5}};
        this->grabbedStores = 0;
        this->setStores(stores, 5);
    }

    int run(string moves)
    {
        for (int i = 0; i < moves.length(); i++)
        {
            switch (moves[i])
            {
            case 'L':
                if (!this->playerMoveLeft())
                {
                    return this->grabbedStores;
                }
                break;
            case 'R':
                if (!this->playerMoveRight())
                {
                    return this->grabbedStores;
                }
                break;
            case 'U':
                if (!this->playerMoveUp())
                {
                    return this->grabbedStores;
                }
                break;
            case 'D':
                if (!this->playerMoveDown())
                {
                    return this->grabbedStores;
                }
            }
        }

        return this->grabbedStores;
    };
};