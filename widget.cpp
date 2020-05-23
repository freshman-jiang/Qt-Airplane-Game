#include "widget.h"
#include "config.h"
#include<QtMath>
#include<QIcon>
#include<QPushButton>
#include<QDir>
Plane myPlane;

//////头节点，用于创建链表，不可删除///////
Plane *enemy_plane_head=new Plane;

MyBullet *head=new MyBullet;

EnemyBullet *ebullet_head=new EnemyBullet(0,0);

//////////////////////////////////////
static int timeCount=0;
static int getpoint=0;

///////////////////////////////////////
QPen red_pen(QColor(255,0,0),2);
QPen blue_pen(QColor(0,0,255),2);

QPen green_pen(QColor(0,255,0),5);
QPen yellow_pen(QColor(255,255,0),3);




Widget::Widget(QWidget *parent)
    : QWidget(parent)
{

    QIcon* myico=new QIcon("://AirPlane.ico");
    setWindowIcon(*myico);
    setFixedSize(800,600);
    myPlane.pos_x=400;
    myPlane.pos_y=550;


    QPushButton *exit=new QPushButton(this);
    exit->setGeometry(10,10,70,30);
    exit->setText("退出");
    connect(exit,&QPushButton::clicked,this,&QWidget::close);

    QPushButton *restart=new QPushButton(this);
    restart->setGeometry(150,10,120,30);
    restart->setText("分数清零并继续");
    connect(restart,&QPushButton::clicked,this,&Widget::revive_MyPlane);

    startTimer(40);


}

Widget::~Widget()
{
}

void Widget::paintEvent(QPaintEvent *)
{

 QPainter painter(this);
 QPixmap background;
 background.load("://background.jpg");
 //set background设置背景：

painter.drawPixmap(0,0,800,600,background,0,5200-timeCount,800,600);
 //draw End picture 结束画面
 if(myPlane.HP<=0)
 {
     QFont endfont;
     endfont.setPointSize(25);
     endfont.setBold(true);
     painter.setFont(endfont);
     painter.setPen(yellow_pen);
     painter.drawText(QPoint(300,300),"游戏结束！得分：");
     QString tempstring;
     painter.drawText(QPoint(400,400),tempstring.number(getpoint));
     return;
 }

 //draw score 打印得分
 painter.setPen(yellow_pen);
 QString tempstring;
 painter.drawText(QPoint(630,300),"得分：");
 painter.drawText(QPoint(700,300),tempstring.number(getpoint));

 //draw HP slot 画血量槽
 painter.setPen(green_pen);
 painter.drawLine(QPoint(550,550),QPoint(550+2*myPlane.HP,550));

//draw myPlane 画己方飞机
 myPlane.draw_MyPlane(painter);

 //draw MyBullet 画己方子弹
     for(MyBullet*visit=head->next;visit!=NULL;visit=visit->next)
     {

         if(timeCount%2==0)
         {painter.setPen(blue_pen);}
         if(timeCount%2==1)
         {painter.setPen(red_pen);}
         painter.drawLine(QPoint(visit->pos_x,visit->pos_y),QPoint(visit->pos_x,visit->pos_y-5));
     }

 //draw EnemyPlane 画敌方飞机
     for(Plane *visit=enemy_plane_head->pnext;visit!=NULL;visit=visit->pnext)
     {

         visit->draw_enemyPlane(painter);

     }


 //draw Enemy bullet 画敌方子弹
     painter.setPen(yellow_pen);
     for(EnemyBullet *visit=ebullet_head->next;visit!=NULL;visit=visit->next)
     {
         painter.drawEllipse(QPoint(visit->pos_x,visit->pos_y),4,4);
     }


}


void Widget::keyPressEvent(QKeyEvent *ev)
{
if (myPlane.HP<0) return;
if (ev->key() == Qt::Key_W && myPlane.pos_y>0)
{
    myPlane.pos_y-=5;
    update();
    return;
}
if (ev->key() == Qt::Key_S && myPlane.pos_y<590)
{
    myPlane.pos_y+=5;
    update();
    return;
}
if (ev->key() == Qt::Key_A && myPlane.pos_x>0)
{

    myPlane.pos_x-=6;
    update();
    return;
}
if (ev->key() == Qt::Key_D && myPlane.pos_x<800)
{
    myPlane.pos_x+=6;
    update();
    return;
}
}



void Widget::timerEvent(QTimerEvent *)
{

    update();
    if (myPlane.HP<=0) return;
    timeCount++;

    if(timeCount%2==1)
    {
        for(MyBullet*visit=head;visit!=NULL;visit=visit->next)
        {
            if(visit->next==NULL)
            {
                visit->next=new MyBullet;
                break;
            }

        }

    }//产生己方子弹，单链表储存

    for(MyBullet*visit=head->next;visit!=NULL;visit=visit->next)
    {
        visit->pos_y-=MY_BULLET_SPEED;

    }//己方子弹移动

    for(MyBullet*visit=head->next,*visit_front=head;visit!=NULL;)
    {
        if(visit->pos_y<=10)
        {
        MyBullet *temp_ptr=visit;
        visit=visit->next;
        visit_front->next=visit;
        delete temp_ptr;
        continue;
        }
        visit_front=visit;
        visit=visit->next;
    }//己方子弹超出屏幕后，删除


 ////////以下为敌方设计/////////////////////
if (timeCount%45==1)
{
    for(Plane *visit=enemy_plane_head;visit!=NULL;visit=visit->pnext)
    {
        if(visit->pnext==NULL)
        {
            visit->pnext=new Plane;

            break;
        }

    }
}//敌方飞机生成

for(Plane *visit=enemy_plane_head->pnext;visit!=NULL;visit=visit->pnext)
{
    visit->pos_y+=ENEMY_SPEED;
}//敌方飞机移动

for(Plane*visit=enemy_plane_head->pnext,*visit_front=enemy_plane_head;visit!=NULL;)
{
    if(visit->pos_y>=590||visit->HP<=0)
    {
    if(visit->HP<=0) getpoint+=10;
    Plane *temp_ptr=visit;
    visit=visit->pnext;
    visit_front->pnext=visit;
    delete temp_ptr;
    continue;
    }
    visit_front=visit;

    visit=visit->pnext;
}//超出屏幕/HP<0则敌方飞机删除



if(timeCount%15==1 && timeCount>160)
{
    for(Plane *visit_ep=enemy_plane_head->pnext;visit_ep!=NULL;visit_ep=visit_ep->pnext)
    {
        for(EnemyBullet *visit=ebullet_head;visit!=NULL;visit=visit->next)
        {
            if(visit->next==NULL)
            {
                visit->next=new EnemyBullet(visit_ep->pos_x,visit_ep->pos_y);
                break;
            }
        }
    }

}//敌方飞机生成敌方子弹


for(EnemyBullet *visit=ebullet_head->next;visit!=NULL;visit=visit->next)
{
    visit->pos_y+=ENEMY_BULLET_SPEED*qCos(visit->direction);
    visit->pos_x+=ENEMY_BULLET_SPEED*qSin(visit->direction);

}//敌方子弹移动，向己方匀速运动

for(EnemyBullet *visit=ebullet_head->next,*visit_front=ebullet_head;visit!=NULL;)
{
    if(visit->pos_y>=590)
    {
    EnemyBullet *temp_ptr=visit;
    visit=visit->next;
    visit_front->next=visit;
    delete temp_ptr;
    continue;
    }
    visit_front=visit;

    visit=visit->next;
}//敌方子弹删除

//以下为敌方碰撞检测
for(MyBullet *visit_mb=head->next;visit_mb!=NULL;visit_mb=visit_mb->next)
{
    for(Plane *visit_ep=enemy_plane_head->pnext;visit_ep!=NULL;visit_ep=visit_ep->pnext)
    {
        if(visit_mb->pos_x>(visit_ep->pos_x-10) && visit_mb->pos_x<(visit_ep->pos_x+10) && visit_mb->pos_y>(visit_ep->pos_y-20) && visit_mb->pos_y<(visit_ep->pos_y+20))
        {
            visit_ep->HP-=MY_BULLET_DAMAGE;
        }
    }
}

//以下为己方碰撞检测
for(EnemyBullet*visit_eb=ebullet_head->next;visit_eb!=NULL;visit_eb=visit_eb->next)
{
    if(visit_eb->pos_x>(myPlane.pos_x-8) && visit_eb->pos_x<(myPlane.pos_x+8) && visit_eb->pos_y>(myPlane.pos_y-15) && visit_eb->pos_y<(myPlane.pos_y+15))
    {
        myPlane.HP-=ENEMY_BULLET_DAMAGE;
    }
}


}


MyBullet::MyBullet()
{
    this->pos_x=myPlane.pos_x;
    this->pos_y=myPlane.pos_y-10;
    this->next=NULL;
}

EnemyBullet::EnemyBullet(int x,int y)
{
    this->pos_x=x;
    this->pos_y=y;
    this->next=NULL;
    this->direction=qAtan((pos_x-myPlane.pos_x+0.0001)/(pos_y-myPlane.pos_y+0.001));
}//0.001防止分母为0，几乎不影响结果
Plane::Plane()
{
    this->pnext=NULL;
    this->HP=100;
    qsrand(timeCount);
    this->pos_x=qrand()%800;
    this->pos_y=0;
}
void Plane::draw_MyPlane(QPainter&painter)
{
    //draw myPlane
   painter.setPen(red_pen);
   painter.drawLine(QPoint(myPlane.pos_x-10,myPlane.pos_y),QPoint(myPlane.pos_x+10,myPlane.pos_y));
   painter.drawLine(QPoint(myPlane.pos_x,myPlane.pos_y-5),QPoint(myPlane.pos_x,myPlane.pos_y+10));
   painter.drawLine(QPoint(myPlane.pos_x-5,myPlane.pos_y+10),QPoint(myPlane.pos_x+5,myPlane.pos_y+10));

}
void Plane::draw_enemyPlane(QPainter &painter)
{
    painter.setPen(yellow_pen);
    painter.drawLine(QPoint(this->pos_x-10,this->pos_y),QPoint(this->pos_x+10,this->pos_y));
    painter.drawLine(QPoint(this->pos_x,this->pos_y+5),QPoint(this->pos_x,this->pos_y-10));
    painter.drawLine(QPoint(this->pos_x-5,this->pos_y-10),QPoint(this->pos_x+5,this->pos_y-10));
}
void Widget::revive_MyPlane()
{
    getpoint=0;
    myPlane.HP=100;

}
