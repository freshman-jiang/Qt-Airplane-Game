#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QPaintEvent>
#include<QPainter>
#include<QKeyEvent>


class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
     void timerEvent(QTimerEvent *e);
     void revive_MyPlane();
protected:
    void paintEvent(QPaintEvent *);
    void keyPressEvent(QKeyEvent *ev);
};
#endif // WIDGET_H





class GameObject
{
public:
    int pos_x;
    int pos_y;
};
class MyBullet:public GameObject
{
public:
    MyBullet();
   MyBullet *next;
};
class Plane:public GameObject
{
public:
    int HP;

    Plane *pnext;
    Plane();
    void draw_MyPlane(QPainter&);
    void draw_enemyPlane(QPainter&);

};
class EnemyBullet:public GameObject
{
public:
    EnemyBullet(int x,int y);
    EnemyBullet *next;
    double direction;
};

