#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "information.h"
#include "common.h"
#include <QMainWindow>
#include <QGraphicsPixmapItem>
#include <QPalette>
#include <QImage>
#include <QFont>


using namespace std;

#define X 13
#define Y 13
#define MAXFLOOR 50
#define MONSTER_NUM 34

typedef int MAP[X * Y];

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitLayout();
    void DisPlayData();

    void DisPlayfloor();
    void loadImage();

    void GameStart();

protected:
    void keyPressEvent(QKeyEvent *event);   //按键处理

private:
    Ui::MainWindow *ui;

    int Item_it;

    //背景
    QImage img_llayer_1;
    QImage img_llayer_2;

    QImage imgHero[4];                     //主角
    QImage Hero_Head;

    //地形图片
    QImage imgWall;                     //@1    墙
    QImage imgYDoor;                    //@2    黄门
    QImage imgBDoor;                    //@3    蓝门
    QImage imgRDoor;                    //@4    红门
    QImage imgPDoor;                    //@5    守卫门
    QImage imgUp_Stairs;                //@9    楼梯(上)
    QImage imgDown_Stairs;              //@10   楼梯(下)
    QImage imgIron_Gate;                //@13   铁门
    QImage imgRNPC;                     //@14   NPC红
    QImage imgBNPC;                     //@15   NPC蓝
    QImage imgThief;                    //@16   小偷
    QImage imgStore[3];                 //@97 左 98 中 99 右   商店

    //道具图片
    QImage imgYKey;                     //@6    黄钥匙
    QImage imgBKey;                     //@7    蓝钥匙
    QImage imgRKey;                     //@8    红钥匙
    QImage imgRed_Agentia;              //@11   红药水
    QImage imgBlue_Agentia;             //@12   蓝药水
    QImage imgRGem;                     //@17   红宝石
    QImage imgBGam;                     //@18   蓝宝石
    QImage imgLucky_Coin;               //@96   幸运金币
    QImage imgFly_Wand;                 //@95   飞行魔杖
    QImage imgMonster_Manual;           //@94   怪物手册
    QImage imgIron_Sword;               //@93   铁剑
    QImage imgNotepad;                  //@92   记事本
    QImage imgIron_Shield;              //@91   铁盾

    //怪物图片
    QImage imgGreen_Slime;              //@21   绿史莱姆
    QImage imgRed_Slime;                //@22   红史莱姆
    QImage imgBig_Slime;                //@23   大史莱姆
    QImage imgSlime_Man;                //@24   史莱姆人
    QImage imgSlime_Lord;               //@25   史莱姆王
    QImage imgPriest;                   //@26   初级法师
    QImage imgSuperion_Priest;          //@27   高级法师
    QImage imgGreat_Magic_Master;       //@28   大法师
    QImage imgBat;                      //@29   小蝙蝠
    QImage imgBig_Bat;                  //@30   大蝙蝠
    QImage imgVampire_Bat;              //@31   吸血蝙蝠
    QImage imgVampire;                  //@32   吸血鬼
    QImage imgSkeleton_A;               //@33   骷髅队长
    QImage imgSkeleton_B;               //@34   骷髅士兵
    QImage imgSkeleton_C;               //@35   骷髅人
    QImage imgGate_Keeper_A;            //@36   高级卫兵
    QImage imgGate_Keeper_B;            //@37   中级卫兵
    QImage imgGate_Keeper_C;            //@38   初级卫兵
    QImage imgMagician_A;               //@39   高级巫师
    QImage imgMagician_B;               //@40   初级巫师
    QImage imgZombie;                   //@41   兽人
    QImage imgZombie_Knight;            //@42   兽人武士
    QImage imgRock;                     //@43   石头人
    QImage imgSoldier;                  //@44   战士
    QImage imgGhost_Soldier;            //@45   鬼战士
    QImage imgKnight;                   //@46   骑士
    QImage imgGolden_Knight;            //@47   金骑士
    QImage imgDark_Knight;              //@48   黑暗骑士
    QImage imgGiant_Octopus;            //@49   大乌贼
    QImage imgDragon;                   //@50   魔龙
    QImage imgSwordsman;                //@51   双手剑士
    QImage imgThe_Magic_Sergeant;       //@52   魔法警卫
    QImage imgThe_Magic_Sergeant_Zeno;  //@53   魔王齐诺
    QImage imgBOSS;                     //@54   魔王

    QGraphicsScene *scene_llayer_1;
    QGraphicsScene *scene_llayer_2;
    QGraphicsPixmapItem **pi;

    QTimer *ItemTimer;

public slots:
    void item();
};

//主角信息
class HERO
{
public:
    int status; //状态
    int level; //等级
    int hp; //血量
    int atk; //攻击力
    int pdef; //防御力
    int pos_x; //当前x位置
    int pos_y; //当前y位置
    int floor; //当前层数
    int exp; //经验值
    int gold; //金币
    int key1; //钥匙1
    int key2; //钥匙2
    int key3; //钥匙3
    int face; //朝向
    HERO();
};

struct MONSTER
{
    QString name;
    int hp; //血量
    int atk; //攻击力
    int pdef; //防御力
    int gold; //金币
    int id;//id号
};

class HINT
{
  public:
    int num1;
    QString keyhint;
    HINT();
};

void init_tower();
int response(int flag, int pos);
void init_monster();


#endif // MAINWINDOW_H
