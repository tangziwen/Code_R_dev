#ifndef GAME_CAMERA_H
#define GAME_CAMERA_H

class GameCamera{
public:
    GameCamera();
    void setPos(int x, int y);
    int getX();
    int getY();
private:
    int m_pos_x;
    int m_pos_y;
};

#endif