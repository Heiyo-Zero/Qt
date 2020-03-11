#include "mainwindow.h"
#include "ui_mainwindow.h"

HERO Hero;
HINT Hint;
MAP Map[MAXFLOOR];
MONSTER Monster[MONSTER_NUM];

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setAutoFillBackground(true);

    Item_it = 0;

    this->setFixedSize(576, 448);   //固定窗口大小
    this->setFocusPolicy(Qt::StrongFocus);  //tab和鼠标获得焦点

    pi = new QGraphicsPixmapItem*[X * Y];
    Hero = HERO();
    Hint = HINT();

    loadImage();
    init_tower();
    InitLayout();
    init_monster();
    DisPlayData();
    DisPlayfloor();

    ItemTimer = new QTimer(this);
    connect(ItemTimer, SIGNAL(timeout()), this, SLOT(item()));

}

//界面布局
void MainWindow::InitLayout()
{
    QImage img = QImage(":/Image/Tilesets/Mota.png").copy(160, 0, 32, 32);
    QPalette palette;
    palette.setBrush(QPalette::Window, QPixmap::fromImage(img));
    this->setPalette(palette);

    //整体背景
    img_llayer_1 = QImage(":/Image/Tilesets/Mota.png").copy(96, 32, 32, 32);
    scene_llayer_1 = new QGraphicsScene;
    scene_llayer_1->setBackgroundBrush(QPixmap::fromImage(img_llayer_1));
    ui->graphicsView->move(0, 0);
    ui->graphicsView->setScene(scene_llayer_1);

    //游戏背景
    img_llayer_2 = QImage(":/Image/Tilesets/Mota.png").copy(96, 32, 32, 32);
    scene_llayer_2 = new QGraphicsScene;
    scene_llayer_2->setBackgroundBrush(QPixmap::fromImage(img_llayer_2));
    ui->graphicsView_2->move(448, 0);
    ui->graphicsView_2->setScene(scene_llayer_2);

    //信息背景
    ui->graphicsView_3->move(448, 256);
    ui->graphicsView_3->setScene(scene_llayer_2);

    ui->graphicsView_4->move(448, 384);
    ui->graphicsView_4->setScene(scene_llayer_2);

    //信息
    ui->label->move(480, 0);
    ui->label_2->move(450, 48);
    ui->label_3->move(450, 96);
    ui->label_4->move(450, 144);
    ui->label_5->move(450, 192);
    ui->label_6->move(496, 48);
    ui->label_7->move(496, 96);
    ui->label_8->move(496, 144);
    ui->label_9->move(496, 192);
    ui->label_10->move(456, 256);
    ui->label_11->move(456, 288);
    ui->label_12->move(456, 320);
    ui->label_13->move(508, 256);
    ui->label_14->move(508, 288);
    ui->label_15->move(508, 320);
    ui->label_16->move(460, 388);
    ui->label_17->move(512, 388);

    Hero_Head = QImage(":/Image/Characters/002-Braver01.png").copy(0, 0, 32, 32);
    ui->label->setPixmap(QPixmap::fromImage(Hero_Head));
    ui->label_10->setPixmap(QPixmap::fromImage(imgYKey));
    ui->label_11->setPixmap(QPixmap::fromImage(imgBKey));
    ui->label_12->setPixmap(QPixmap::fromImage(imgRKey));
    ui->label_18->setStyleSheet("background-color:rgba(0,0,0,255);");
    ui->label_18->hide();

    QPalette p_lb;
    QFont f_lb;
    QFont f_lb18;
    p_lb.setColor(QPalette::WindowText, Qt::white);
    f_lb.setPointSize(14);
    f_lb18.setPointSize(20);
    ui->label_2->setText(QString::fromLocal8Bit("生命"));
    ui->label_2->setPalette(p_lb);
    ui->label_2->setFont(f_lb);

    ui->label_3->setText(QString::fromLocal8Bit("攻击"));
    ui->label_3->setPalette(p_lb);
    ui->label_3->setFont(f_lb);

    ui->label_4->setText(QString::fromLocal8Bit("防御"));
    ui->label_4->setPalette(p_lb);
    ui->label_4->setFont(f_lb);

    ui->label_5->setText(QString::fromLocal8Bit("金币"));
    ui->label_5->setPalette(p_lb);
    ui->label_5->setFont(f_lb);

    ui->label_6->setPalette(p_lb);
    ui->label_6->setFont(f_lb);

    ui->label_7->setPalette(p_lb);
    ui->label_7->setFont(f_lb);

    ui->label_8->setPalette(p_lb);
    ui->label_8->setFont(f_lb);

    ui->label_9->setPalette(p_lb);
    ui->label_9->setFont(f_lb);

    ui->label_13->setPalette(p_lb);
    ui->label_13->setFont(f_lb);


    ui->label_14->setPalette(p_lb);
    ui->label_14->setFont(f_lb);

    ui->label_15->setPalette(p_lb);
    ui->label_15->setFont(f_lb);

    ui->label_16->setText(QString::fromLocal8Bit("层数"));
    ui->label_16->setPalette(p_lb);
    ui->label_16->setFont(f_lb);

    ui->label_17->setPalette(p_lb);
    ui->label_17->setFont(f_lb);

    ui->label_18->setPalette(p_lb);
    ui->label_18->setFont(f_lb18);
    ui->label_18->setAlignment(Qt::AlignCenter);


}

//显示数据
void MainWindow::DisPlayData()
{
    ui->label_6->setText(QString::number(Hero.hp));
    ui->label_7->setText(QString::number(Hero.atk));
    ui->label_8->setText(QString::number(Hero.pdef));
    ui->label_9->setText(QString::number(Hero.gold));
    ui->label_13->setText(QString::number(Hero.key1));
    ui->label_14->setText(QString::number(Hero.key2));
    ui->label_15->setText(QString::number(Hero.key3));
    ui->label_17->setText(QString::number(Hero.floor + 1));
}

void MainWindow::GameStart()
{
    //loadImage();
}

//事件响应
//int flag 0 上 / 1 下 / 2 左 / 3 右
//int pos 坐标
//return 0:不动 / 1:动 / 2:动+提示信息
int response(int flag, int pos)
{
    pos = -1;
    switch(flag)
    {
    case 0:
        pos = (Hero.pos_y - 1) * X + Hero.pos_x;
        break;
    case 1:
        pos = (Hero.pos_y + 1) * X + Hero.pos_x;
        break;
    case 2:
        pos = Hero.pos_y * X + (Hero.pos_x - 1);
        break;
    case 3:
        pos = Hero.pos_y * X + (Hero.pos_x + 1);
        break;
    default:break;
    }
    if(0 == Map[Hero.floor][pos])
    {//平地 走过去
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        return 1;
    }
    else if(1 == Map[Hero.floor][pos])
    {//@1    墙
        return 0;
    }
    else if (2 == Map[Hero.floor][pos])
    {//@2    黄门
        if(0 == Hero.key1)
        {
            Hint.keyhint = QString::fromLocal8Bit("没有一把黄钥匙");
            return 2;
        }
        else
        {
            Map[Hero.floor][pos] = 0;
            Hero.pos_x = pos % X;
            Hero.pos_y = pos / X;
            Hero.key1 -= 1;
            return 1;
        }
    }
    else if (3 == Map[Hero.floor][pos])
    {//@3    蓝门
        if(0 == Hero.key2)
        {
            Hint.keyhint = QString::fromLocal8Bit("没有一把蓝钥匙");
            return 2;
        }
        else
        {
            Map[Hero.floor][pos] = 0;
            Hero.pos_x = pos % X;
            Hero.pos_y = pos / X;
            Hero.key2 -= 1;
            return 1;
        }
    }
    else if (4 == Map[Hero.floor][pos])
    {//@4    红门
        if(0 == Hero.key3)
        {
            Hint.keyhint = QString::fromLocal8Bit("没有一把红钥匙");
            return 2;
        }
        else
        {
            Map[Hero.floor][pos] = 0;
            Hero.pos_x = pos % X;
            Hero.pos_y = pos / X;
            Hero.key3 -= 1;
            return 1;
        }
    }
    else if(5 == Map[Hero.floor][pos])
    {//@5    守卫门
        return 0;
    }
    else if(6 == Map[Hero.floor][pos])
    {//@6    黄钥匙
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hero.key1 += 1;
        Hint.keyhint = QString::fromLocal8Bit("获得一把黄钥匙");
        return 2;
    }
    else if(7 == Map[Hero.floor][pos])
    {//@7    蓝钥匙
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hero.key2 += 1;
        Hint.keyhint = QString::fromLocal8Bit("获得一把蓝钥匙");
        return 2;
    }
    else if(8 == Map[Hero.floor][pos])
    {//@8    红钥匙
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hero.key3 += 1;
        Hint.keyhint = QString::fromLocal8Bit("获得一把红钥匙");
        return 2;
    }
    else if(9 == Map[Hero.floor][pos])
    {//@9    楼梯(上)
        Hero.floor += 1;
        int m_pos = 0;
        for(int i = 0; i < X * Y; i++)
        {
            if(10 == Map[Hero.floor][i])
            {
                m_pos = i;
                break;
            }
        }
        Hero.pos_x = m_pos % X;
        Hero.pos_y = m_pos / X;
        return 1;
    }
    else if(10 == Map[Hero.floor][pos])
    {//@10   楼梯(下)
        Hero.floor -= 1;
        int m_pos = 0;
        for(int i = 0; i < X * Y; i++)
        {
            if(9 == Map[Hero.floor][i])
            {
                m_pos = i;
                break;
            }
        }
        Hero.pos_x = m_pos % X;
        Hero.pos_y = m_pos / X;
        return 1;
    }
    else if(11 == Map[Hero.floor][pos])
    {//@11   红药水
        Hero.hp += 50;
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hint.keyhint = QString::fromLocal8Bit("获得红药水，生命+50");
        return 2;
    }
    else if(12 == Map[Hero.floor][pos])
    {//@12   蓝药水
        Hero.hp += 200;
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hint.keyhint = QString::fromLocal8Bit("获得蓝药水，生命+200");
        return 2;
    }
    else if(13 == Map[Hero.floor][pos])
    {//@13   铁门
        return 0;
    }
    else if(17 == Map[Hero.floor][pos])
    {//@17   红宝石
        Hero.atk += 1;
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hint.keyhint = QString::fromLocal8Bit("获得红宝石，攻击+1");
        return 2;
    }
    else if(18 == Map[Hero.floor][pos])
    {//@18   蓝宝石
        Hero.pdef += 1;
        Map[Hero.floor][pos] = 0;
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        Hint.keyhint = QString::fromLocal8Bit("获得蓝宝石，防御+1");
        return 2;
    }
    else if(Map[Hero.floor][pos] >= 21 && Map[Hero.floor][pos] <= 54)
    {
        if(Monster[Map[Hero.floor][pos] - 21].atk <= Hero.pdef &&
                Monster[Map[Hero.floor][pos] - 21].pdef < Hero.atk)
        {
            Hero.gold += Monster[Map[Hero.floor][pos] - 21].gold;
            Map[Hero.floor][pos] = 0;
            Hero.pos_x = pos % X;
            Hero.pos_y = pos / X;
            Hint.keyhint = QString::fromLocal8Bit("战斗胜利，获得金币") +
                                QString::number(Monster[Map[Hero.floor][pos]].gold);
            return 2;
        }
        else if(Monster[Map[Hero.floor][pos] - 21].atk > Hero.pdef)
        {
            if(Monster[Map[Hero.floor][pos] - 21].pdef >= Hero.atk)
            {
                Hint.keyhint = QString::fromLocal8Bit("你还不能打败他");
                return 2;
            }
            else if(Monster[Map[Hero.floor][pos] - 21].pdef < Hero.atk)
            {
                int cnt = (Monster[Map[Hero.floor][pos] - 21].hp - 1) /
                        (Hero.atk - Monster[Map[Hero.floor][pos] - 21].pdef);
                if(Hero.hp < cnt * (Monster[Map[Hero.floor][pos] - 21].atk - Hero.pdef))
                {
                    Hint.keyhint = QString::fromLocal8Bit("你还不能打败他");
                    return 2;
                }
                else
                {
                    Hero.hp -= cnt * (Monster[Map[Hero.floor][pos] - 21].atk - Hero.pdef);
                    Hero.gold += Monster[Map[Hero.floor][pos] - 21].gold;
                    Map[Hero.floor][pos] = 0;
                    Hero.pos_x = pos % X;
                    Hero.pos_y = pos / X;
                    Hint.keyhint = QString::fromLocal8Bit("战斗胜利，获得金币") +
                                        QString::number(Monster[Map[Hero.floor][pos]].gold);
                    return 2;
                }
                return 2;
            }
            return 2;
        }
        return 2;
    }
    else
    {
        Hero.pos_x = pos % X;
        Hero.pos_y = pos / X;
        return 1;
    }
}

//按键处理
void MainWindow::keyPressEvent(QKeyEvent *event)
{
    int flag;
    int pos = -1;
    if(event->key() == Qt::Key_Up)
    {
        flag = response(0, pos);
    }
    else if(event->key() == Qt::Key_Down)
    {
        flag = response(1, pos);
    }
    else if(event->key() == Qt::Key_Left)
    {
        flag = response(2, pos);
    }
    else if(event->key() == Qt::Key_Right)
    {
        flag = response(3, pos);
    }
    if(flag == 1)
    {
        DisPlayData();
        DisPlayfloor();
    }
    else if(flag == 2)
    {
        ItemTimer->start(200);
        DisPlayData();
        DisPlayfloor();
    }

}

//加载图片资源
void MainWindow::loadImage()
{
    imgHero[0] = QImage(":/Image/Characters/002-Braver01.png").copy(0, 0, 32, 32);
    imgHero[1] = QImage(":/Image/Characters/002-Braver01.png").copy(0, 32, 32, 32);
    imgHero[2] = QImage(":/Image/Characters/002-Braver01.png").copy(0, 64, 32, 32);
    imgHero[3] = QImage(":/Image/Characters/002-Braver01.png").copy(0, 96, 32, 32);

    //地形
    imgWall = QImage(":/Image/Tilesets/Mota.png").copy(192, 0, 32, 32);
    imgYDoor = QImage(":/Image/Characters/men001.png").copy(0, 0, 32, 32);
    imgRDoor = QImage(":/Image/Characters/men001.png").copy(64, 0, 32, 32);
    imgBDoor = QImage(":/Image/Characters/men001.png").copy(32, 0, 32, 32);
    imgPDoor = QImage(":/Image/Characters/men001.png").copy(96, 0, 32, 32);
    imgUp_Stairs = QImage(":/Image/Tilesets/Mota.png").copy(32, 992, 32, 32);
    imgDown_Stairs = QImage(":/Image/Tilesets/Mota.png").copy(0, 992, 32, 32);
    imgIron_Gate = QImage(":/Image/Characters/men003.png").copy(96, 0, 32, 32);
    imgRNPC = QImage(":/Image/Characters/001-npc01.png").copy(0, 32, 32, 32);
    imgBNPC = QImage(":/Image/Characters/001-npc01.png").copy(0, 0, 32, 32);
    imgThief = QImage(":/Image/Characters/001-npc01.png").copy(0, 64, 32, 32);
    imgStore[0] = QImage(":/Image/Tilesets/Mota.png").copy(96, 992, 32, 32);
    imgStore[1] = QImage(":/Image/Tilesets/Mota.png").copy(128, 992, 32, 32);
    imgStore[2] = QImage(":/Image/Tilesets/Mota.png").copy(160, 992, 32, 32);

    //道具
    imgYKey = QImage(":/Image/Characters/item01.png").copy(0, 0, 32, 32);
    imgBKey = QImage(":/Image/Characters/item01.png").copy(32, 0, 32, 32);
    imgRKey = QImage(":/Image/Characters/item01.png").copy(64, 0, 32, 32);
    imgRed_Agentia = QImage(":/Image/Characters/item03.png").copy(0, 32, 32, 32);
    imgBlue_Agentia = QImage(":/Image/Characters/item03.png").copy(32, 32, 32, 32);
    imgRGem = QImage(":/Image/Characters/item03.png").copy(0, 0, 32, 32);
    imgBGam = QImage(":/Image/Characters/item03.png").copy(32, 0, 32, 32);
    imgLucky_Coin = QImage(":/Image/Characters/item01.png").copy(96, 96, 32, 32);
    imgFly_Wand = QImage(":/Image/Characters/item05.png").copy(64, 0, 32, 32);
    imgMonster_Manual = QImage(":/Image/Characters/item01.png").copy(0, 96, 32, 32);
    imgIron_Sword = QImage(":/Image/Characters/item04.png").copy(0, 0, 32, 32);
    imgNotepad = QImage(":/Image/Characters/item01.png").copy(32, 96, 32, 32);
    imgIron_Shield = QImage(":/Image/Characters/item04.png").copy(0, 64, 32, 32);


    //怪物 @21~64
    imgGreen_Slime = QImage(":/Image/Characters/003-Monster01.png").copy(0, 0, 32, 32);
    imgRed_Slime = QImage(":/Image/Characters/003-Monster01.png").copy(0, 32, 32, 32);
    imgBig_Slime = QImage(":/Image/Characters/003-Monster01.png").copy(0, 64, 32, 32);
    imgSlime_Man = QImage(":/Image/Characters/006-Monster04.png").copy(96, 0, 32, 32);
    imgSlime_Lord = QImage(":/Image/Characters/003-Monster01.png").copy(96, 0, 32, 32);
    imgPriest = QImage(":/Image/Characters/007-Monster05.png").copy(0, 0, 32, 32);
    imgSuperion_Priest = QImage(":/Image/Characters/007-Monster05.png").copy(32, 0, 32, 32);
    imgGreat_Magic_Master = QImage(":/Image/Characters/014-Monster11.png").copy(0, 0, 32, 32);
    imgBat = QImage(":/Image/Characters/004-Monster02.png").copy(0, 0, 32, 32);
    imgBig_Bat = QImage(":/Image/Characters/004-Monster02.png").copy(32, 0, 32, 32);
    imgVampire_Bat = QImage(":/Image/Characters/004-Monster02.png").copy(64, 0, 32, 32);
    imgVampire = QImage(":/Image/Characters/004-Monster02.png").copy(96, 0, 32, 32);
    imgSkeleton_A = QImage(":/Image/Characters/005-Monster03.png").copy(0, 64, 32, 32);
    imgSkeleton_B = QImage(":/Image/Characters/005-Monster03.png").copy(0, 32, 32, 32);
    imgSkeleton_C = QImage(":/Image/Characters/005-Monster03.png").copy(0, 0, 32, 32);
    imgGate_Keeper_A = QImage(":/Image/Characters/008-Monster06.png").copy(0, 64, 32, 32);
    imgGate_Keeper_B = QImage(":/Image/Characters/008-Monster06.png").copy(0, 32, 32, 32);
    imgGate_Keeper_C = QImage(":/Image/Characters/008-Monster06.png").copy(0, 0, 32, 32);
    imgMagician_A = QImage(":/Image/Characters/007-Monster05.png").copy(96, 0, 32, 32);
    imgMagician_B = QImage(":/Image/Characters/007-Monster05.png").copy(64, 0, 32, 32);
    imgZombie = QImage(":/Image/Characters/006-Monster04.png").copy(0, 0, 32, 32);
    imgZombie_Knight = QImage(":/Image/Characters/006-Monster04.png").copy(32, 0, 32, 32);
    imgRock = QImage(":/Image/Characters/006-Monster04.png").copy(64, 0, 32, 32);
    imgSoldier = QImage(":/Image/Characters/009-Monster07.png").copy(0, 0, 32, 32);
    imgGhost_Soldier= QImage(":/Image/Characters/017-Monster22.png").copy(0, 0, 32, 32);
    imgKnight = QImage(":/Image/Characters/014-Monster12.png").copy(96, 0, 32, 32);
    imgGolden_Knight = QImage(":/Image/Characters/009-Monster07.png").copy(32, 0, 32, 32);
    imgDark_Knight = QImage(":/Image/Characters/009-Monster07.png").copy(96, 0, 32, 32);
    imgGiant_Octopus = QImage(":/Image/Characters/011-Monster09.png").copy(96, 0, 96, 96);
    imgDragon = QImage(":/Image/Characters/011-Monster09.png").copy(0, 0, 96, 96);
    imgSwordsman = QImage(":/Image/Characters/008-Monster06.png").copy(96, 0, 32, 32);
    imgThe_Magic_Sergeant = QImage(":/Image/Characters/010-Monster08.png").copy(32, 0, 32, 32);
    imgThe_Magic_Sergeant_Zeno = QImage(":/Image/Characters/010-Monster08.png").copy(64, 0, 32, 32);
    imgBOSS = QImage(":/Image/Characters/014-Monster12.png").copy(0, 0, 32, 32);
}

// 初始化地图资源
void init_tower()
{
    int f_1[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 9, 0, 21, 22, 21, 0, 0, 0, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1,
        1, 11, 0, 0, 2, 0, 1, 17, 6, 0, 1, 0, 1,
        1, 0, 35, 0, 1, 0, 1, 18, 11, 0, 1, 0, 1,
        1, 1, 2, 1, 1, 0, 1, 1, 1, 2, 1, 0, 1,
        1, 6, 0, 0, 1, 0, 2, 29, 26, 29, 1, 0, 1,
        1, 0, 34, 0, 1, 0, 1, 1, 1, 1, 1, 0, 1,
        1, 1, 2, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 0, 1, 1, 2, 1, 1, 1, 2, 1, 1,
        1, 11, 0, 6, 1, 6, 0, 0, 1, 0, 29, 0, 1,
        1, 11, 95, 6, 1, 0, 0, 0, 1, 21, 12, 21, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_2[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 10, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 0, 0, 1, 1, 0, 37, 0, 37, 0, 1, 1, 1,
        1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1,
        1, 0, 1, 6, 6, 1, 0, 0, 0, 1, 0, 15, 1,
        1, 0, 1, 6, 0, 13, 0, 0, 0, 13, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
        1, 0, 1, 16, 0, 1, 0, 0, 0, 1, 0, 14, 1,
        1, 0, 1, 0, 0, 13, 0, 0, 0, 13, 0, 0, 1,
        1, 0, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1,
        1, 0, 1, 12, 12, 1, 0, 0, 0, 1, 0, 0, 1,
        1, 9, 1, 12, 0, 13, 0, 0, 0, 13, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_3[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 6, 18, 1, 6, 12, 6, 1, 0, 1, 0, 11, 1,
        1, 0, 11, 1, 12, 6, 12, 1, 0, 2, 29, 0, 1,
        1, 26, 0, 1, 6, 7, 6, 1, 0, 1, 1, 1, 1,
        1, 2, 1, 1, 1, 0, 1, 1, 0, 1, 0, 15, 1,
        1, 0, 0, 29, 0, 0, 0, 21, 0, 0, 0, 0, 1,
        1, 2, 1, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1,
        1, 35, 0, 1, 1, 0, 1, 1, 0, 1, 0, 11, 1,
        1, 0, 6, 1, 0, 0, 0, 1, 0, 2, 26, 6, 1,
        1, 11, 0, 1, 0, 0, 0, 1, 0, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 0, 1, 1, 22, 1, 0, 0, 1,
        1, 10, 0, 0, 0, 0, 0, 1, 0, 2, 0, 9, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_4[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 7, 0, 1, 97, 98, 99, 1, 0, 15, 0, 1,
        1, 11, 0, 6, 1, 0, 0, 0, 1, 6, 0, 12, 1,
        1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 34, 0, 1,
        1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1, 1,
        1, 0, 26, 0, 2, 0, 22, 0, 0, 35, 0, 0, 1,
        1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 22, 0, 21, 0, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 2, 1, 1, 2, 1, 1, 1, 2, 1, 1, 2, 1,
        1, 0, 1, 0, 29, 0, 1, 0, 26, 0, 1, 0, 1,
        1, 0, 1, 21, 0, 6, 1, 17, 0, 11, 1, 0, 1,
        1, 9, 1, 6, 21, 6, 1, 0, 21, 0, 1, 10, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_5[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 9, 1, 0, 22, 2, 0, 1, 0, 0, 2, 0, 1,
        1, 0, 1, 0, 0, 1, 6, 1, 21, 21, 1, 22, 1,
        1, 0, 2, 29, 0, 1, 0, 1, 6, 6, 1, 0, 1,
        1, 1, 1, 1, 2, 1, 29, 1, 6, 6, 1, 0, 1,
        1, 6, 0, 26, 0, 1, 0, 1, 1, 1, 1, 0, 1,
        1, 6, 0, 0, 29, 1, 0, 21, 0, 0, 0, 0, 1,
        1, 1, 34, 1, 1, 1, 0, 1, 1, 1, 1, 22, 1,
        1, 0, 0, 0, 0, 1, 21, 1, 0, 0, 0, 0, 1,
        1, 18, 6, 11, 92, 1, 0, 1, 2, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 0, 1,
        1, 10, 0, 0, 0, 0, 0, 1, 0, 1, 0, 93, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_6[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 10, 1, 6, 6, 1, 0, 26, 0, 6, 21, 0, 1,
        1, 0, 1, 6, 6, 1, 0, 1, 1, 1, 1, 2, 1,
        1, 0, 1, 1, 22, 1, 0, 1, 11, 0, 35, 0, 1,
        1, 0, 2, 2, 0, 2, 0, 1, 14, 0, 0, 29, 1,
        1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 22, 26, 0, 6, 0, 35, 34, 0, 0, 1,
        1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 0, 1,
        1, 26, 0, 0, 15, 1, 0, 2, 2, 0, 2, 0, 1,
        1, 0, 29, 0, 18, 1, 0, 1, 1, 22, 1, 22, 1,
        1, 2, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1,
        1, 0, 21, 0, 0, 35, 0, 1, 11, 11, 1, 9, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_7[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 9, 1, 17, 1, 0, 14, 0, 1, 6, 1, 21, 1,
        1, 0, 1, 11, 1, 0, 0, 0, 1, 6, 1, 22, 1,
        1, 0, 1, 29, 1, 22, 1, 34, 1, 11, 1, 21, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 2, 1, 2, 1, 3, 1, 2, 1, 35, 1, 2, 1,
        1, 0, 34, 0, 26, 0, 0, 0, 0, 0, 0, 0, 1,
        1, 2, 1, 2, 1, 2, 1, 2, 1, 34, 1, 2, 1,
        1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1,
        1, 0, 1, 0, 1, 29, 1, 22, 1, 12, 1, 0, 1,
        1, 21, 1, 21, 1, 6, 1, 26, 1, 6, 1, 0, 1,
        1, 0, 22, 0, 1, 6, 1, 12, 1, 6, 1, 10, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_8[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 10, 0, 2, 2, 0, 9, 0, 1, 6, 0, 6, 1,
        1, 0, 0, 1, 1, 0, 0, 21, 1, 0, 8, 0, 1,
        1, 2, 1, 1, 1, 1, 2, 1, 1, 12, 0, 11, 1,
        1, 0, 1, 2, 2, 2, 0, 0, 1, 1, 5, 1, 1,
        1, 11, 1, 1, 1, 1, 1, 26, 1, 38, 0, 38, 1,
        1, 0, 22, 21, 22, 0, 1, 0, 1, 0, 0, 0, 1,
        1, 1, 1, 1, 1, 2, 1, 29, 1, 1, 2, 1, 1,
        1, 0, 0, 0, 29, 0, 35, 0, 26, 0, 0, 0, 1,
        1, 2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 1,
        1, 21, 0, 1, 17, 6, 1, 7, 11, 1, 0, 35, 1,
        1, 0, 29, 3, 6, 18, 1, 6, 0, 2, 34, 0, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_9[X * Y] = {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        1, 0, 0, 35, 2, 0, 10, 0, 2, 21, 0, 11, 1,
        1, 0, 6, 0, 1, 0, 0, 0, 1, 0, 21, 0, 1,
        1, 34, 1, 1, 1, 1, 3, 1, 1, 1, 1, 0, 1,
        1, 0, 6, 0, 1, 6, 0, 6, 2, 2, 0, 0, 1,
        1, 18, 0, 29, 2, 0, 17, 0, 1, 1, 1, 1, 1,
        1, 1, 1, 1, 1, 1, 1, 22, 1, 0, 0, 34, 1,
        1, 6, 0, 2, 34, 6, 1, 0, 1, 91, 1, 0, 1,
        1, 34, 0, 1, 0, 0, 1, 0, 1, 1, 1, 2, 1,
        1, 2, 1, 1, 1, 2, 1, 0, 1, 6, 0, 26, 1,
        1, 0, 11, 1, 0, 35, 1, 29, 1, 0, 35, 0, 1,
        1, 9, 0, 3, 0, 0, 2, 0, 2, 26, 0, 11, 1,
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
    };
    int f_10[X * Y] = {0};
    for(int i = 0; i < X * Y; i++)
    {
        Map[0][i] = f_1[i];
        Map[1][i] = f_2[i];
        Map[2][i] = f_3[i];
        Map[3][i] = f_4[i];
        Map[4][i] = f_5[i];
        Map[5][i] = f_6[i];
        Map[6][i] = f_7[i];
        Map[7][i] = f_8[i];
        Map[8][i] = f_9[i];
    }
}

// 画地图
void MainWindow::DisPlayfloor()
{
    int i = 0;
    scene_llayer_1->clear();
    for(int x = 0; x <= X - 1; x++)
    {
        for(int y = 0; y <= Y - 1; y++)
        {
            if(Hero.pos_x == x && Hero.pos_y == y)
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgHero[0]));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 1)         //@1    墙
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgWall));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 2)        //@2    黄门
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgYDoor));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 3)        //@3    蓝门
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBDoor));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 4)        //@4    红门
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRDoor));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 5)        //@5    守卫门
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgPDoor));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 6)        //@6    黄钥匙
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgYKey));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 7)        //@7    蓝钥匙
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBKey));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 8)        //@8    红钥匙
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRKey));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 9)        //@9    楼梯(上)
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgUp_Stairs));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 10)        //@10    楼梯(下)
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgDown_Stairs));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 11)        //@11   红药水
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRed_Agentia));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 12)        //@12   蓝药水
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBlue_Agentia));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 13)        //@13   铁门
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgIron_Gate));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 14)        //@14   NPC红
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRNPC));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 15)        //@15   NPC蓝
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBNPC));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 16)        //@16   小偷
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgThief));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 17)        //@17   红宝石
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRGem));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 18)        //@18   蓝宝石
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBGam));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }

            else if(Map[Hero.floor][y * X + x] == 21)       //@21   绿史莱姆
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGreen_Slime));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 22)       //@22   红史莱姆
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRed_Slime));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 23)       //@23   大史莱姆
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBig_Slime));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 24)       //@24   史莱姆人
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSlime_Man));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 25)       //@25   史莱姆王
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSlime_Lord));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 26)       //@26   初级法师
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgPriest));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 27)       //@27   高级法师
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSuperion_Priest));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 28)       //@28   大法师
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGreat_Magic_Master));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 29)       //@29   小蝙蝠
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBat));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 30)       //@30   大蝙蝠
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBig_Bat));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 31)       //@31   吸血蝙蝠
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgVampire_Bat));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 32)       //@32   吸血鬼
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgVampire));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 33)       //@33   骷髅队长
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSkeleton_A));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 34)       //@34   骷髅士兵
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSkeleton_B));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 35)       //@35   骷髅人
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSkeleton_C));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 36)       //@36   高级卫兵
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGate_Keeper_A));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 37)       //@37   中级卫兵
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGate_Keeper_B));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 38)       //@38   初级卫兵
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGate_Keeper_C));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 39)       //@39   高级巫师
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgMagician_A));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 40)       //@40   初级巫师
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgMagician_B));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 41)       //@41   兽人
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgZombie));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 42)       //@42   兽人武士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgZombie_Knight));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 43)       //@43   石头人
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgRock));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 44)       //@44   战士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSoldier));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 45)       //@45   鬼战士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGhost_Soldier));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 46)       //@46   骑士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgKnight));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 47)       //@47   金骑士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGolden_Knight));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 48)       //@48   黑暗骑士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgDark_Knight));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 49)       //@49   大乌贼
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgGiant_Octopus));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 50)       //@50   魔龙
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgDragon));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 51)       //@51   双手剑士
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgSwordsman));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 52)       //@52   魔法警卫
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgThe_Magic_Sergeant));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 53)       //@53   魔王齐诺
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgThe_Magic_Sergeant_Zeno));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 54)       //@54   魔王
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgBOSS));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 91)       //@91   铁盾
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgIron_Shield));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 92)       //@92   记事本
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgNotepad));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 93)       //@93   铁剑
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgIron_Sword));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 94)       //@94   怪物手册
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgMonster_Manual));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 95)       //@95   飞行魔杖
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgFly_Wand));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 96)       //@96   幸运金币
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgLucky_Coin));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 97)       //@97   商店(左)
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgStore[0]));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 98)       //@98   商店(中)
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgStore[1]));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
            else if(Map[Hero.floor][y * X + x] == 99)       //@99   商店(右)
            {
                pi[i] = new QGraphicsPixmapItem;
                pi[i]->setPixmap(QPixmap::fromImage(imgStore[2]));
                pi[i]->setPos(QPoint(32 * x, 32 * y));
                scene_llayer_1->addItem(pi[i]);
                i++;
            }
        }
    }
}

//提示槽函数
void MainWindow::item()
{
    if (Item_it == 0)
    {
        ui->label_18->setText(Hint.keyhint);
        ui->label_18->show();
        Item_it++;
    }
    else if (Item_it <= 2)
    {
        Item_it++;
    }
    else
    {
        ItemTimer->stop();
        ui->label_18->hide();
        Item_it = 0;
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

void init_monster()
{
    Monster[0].name = QString::fromLocal8Bit("绿色史莱姆");
    Monster[0].hp = 35;
    Monster[0].atk = 18;
    Monster[0].pdef = 1;
    Monster[0].gold = 1;
    Monster[0].id = 21;

    Monster[1].name = QString::fromLocal8Bit("红色史莱姆");
    Monster[1].hp = 45;
    Monster[1].atk = 20;
    Monster[1].pdef = 2;
    Monster[1].gold = 2;
    Monster[1].id = 22;

    Monster[2].name = QString::fromLocal8Bit("大史莱姆");
    Monster[2].hp = 130;
    Monster[2].atk = 60;
    Monster[2].pdef = 3;
    Monster[2].gold = 8;
    Monster[2].id = 23;

    Monster[3].name = QString::fromLocal8Bit("史莱姆人");
    Monster[3].hp = 320;
    Monster[3].atk = 140;
    Monster[3].pdef = 20;
    Monster[3].gold = 30;
    Monster[3].id = 24;

    Monster[4].name = QString::fromLocal8Bit("史莱姆王");
    Monster[4].hp = 360;
    Monster[4].atk = 310;
    Monster[4].pdef = 20;
    Monster[4].gold = 40;
    Monster[4].id = 25;

    Monster[5].name = QString::fromLocal8Bit("初级法师");
    Monster[5].hp = 60;
    Monster[5].atk = 32;
    Monster[5].pdef = 8;
    Monster[5].gold = 5;
    Monster[5].id = 26;

    Monster[6].name = QString::fromLocal8Bit("高级法师");
    Monster[6].hp = 100;
    Monster[6].atk = 95;
    Monster[6].pdef = 30;
    Monster[6].gold = 22;
    Monster[6].id = 27;

    Monster[7].name = QString::fromLocal8Bit("大法师");
    Monster[7].hp = 4500;
    Monster[7].atk = 560;
    Monster[7].pdef = 310;
    Monster[7].gold = 1000;
    Monster[7].id = 28;

    Monster[8].name = QString::fromLocal8Bit("小蝙蝠");
    Monster[8].hp = 35;
    Monster[8].atk = 38;
    Monster[8].pdef = 3;
    Monster[8].gold = 3;
    Monster[8].id = 29;

    Monster[9].name = QString::fromLocal8Bit("大蝙蝠");
    Monster[9].hp = 60;
    Monster[9].atk = 100;
    Monster[9].pdef = 8;
    Monster[9].gold = 12;
    Monster[9].id = 30;

    Monster[10].name = QString::fromLocal8Bit("吸血蝙蝠");
    Monster[10].hp = 200;
    Monster[10].atk = 390;
    Monster[10].pdef = 90;
    Monster[10].gold = 50;
    Monster[10].id = 31;

    Monster[11].name = QString::fromLocal8Bit("吸血鬼");
    Monster[11].hp = 444;
    Monster[11].atk = 199;
    Monster[11].pdef = 66;
    Monster[11].gold = 144;
    Monster[11].id = 32;

    Monster[12].name = QString::fromLocal8Bit("骷髅队长");
    Monster[12].hp = 100;
    Monster[12].atk = 65;
    Monster[12].pdef = 15;
    Monster[12].gold = 30;
    Monster[12].id = 33;

    Monster[13].name = QString::fromLocal8Bit("骷髅士兵");
    Monster[13].hp = 55;
    Monster[13].atk = 52;
    Monster[13].pdef = 12;
    Monster[13].gold = 8;
    Monster[13].id = 34;

    Monster[14].name = QString::fromLocal8Bit("骷髅人");
    Monster[14].hp = 50;
    Monster[14].atk = 42;
    Monster[14].pdef = 6;
    Monster[14].gold = 6;
    Monster[14].id = 35;

    Monster[15].name = QString::fromLocal8Bit("高级卫兵");
    Monster[15].hp = 180;
    Monster[15].atk = 460;
    Monster[15].pdef = 360;
    Monster[15].gold = 200;
    Monster[15].id = 36;

    Monster[16].name = QString::fromLocal8Bit("中级卫兵");
    Monster[16].hp = 100;
    Monster[16].atk = 180;
    Monster[16].pdef = 110;
    Monster[16].gold = 100;
    Monster[16].id = 37;

    Monster[17].name = QString::fromLocal8Bit("初级卫兵");
    Monster[17].hp = 50;
    Monster[17].atk = 48;
    Monster[17].pdef = 22;
    Monster[17].gold = 12;
    Monster[17].id = 38;

    Monster[18].name = QString::fromLocal8Bit("高级巫师");
    Monster[18].hp = 200;
    Monster[18].atk = 380;
    Monster[18].pdef = 130;
    Monster[18].gold = 90;
    Monster[18].id = 39;

    Monster[19].name = QString::fromLocal8Bit("初级巫师");
    Monster[19].hp = 220;
    Monster[19].atk = 370;
    Monster[19].pdef = 110;
    Monster[19].gold = 80;
    Monster[19].id = 40;

    Monster[20].name = QString::fromLocal8Bit("兽人");
    Monster[20].hp = 260;
    Monster[20].atk = 85;
    Monster[20].pdef = 5;
    Monster[20].gold = 18;
    Monster[20].id = 41;

    Monster[21].name = QString::fromLocal8Bit("兽人武士");
    Monster[21].hp = 320;
    Monster[21].atk = 120;
    Monster[21].pdef = 15;
    Monster[21].gold = 30;
    Monster[21].id = 42;

    Monster[22].name = QString::fromLocal8Bit("石头人");
    Monster[22].hp = 20;
    Monster[22].atk = 100;
    Monster[22].pdef = 68;
    Monster[22].gold = 28;
    Monster[22].id = 43;

    Monster[23].name = QString::fromLocal8Bit("战士");
    Monster[23].hp = 210;
    Monster[23].atk = 200;
    Monster[23].pdef = 65;
    Monster[23].gold = 45;
    Monster[23].id = 44;

    Monster[24].name = QString::fromLocal8Bit("鬼战士");
    Monster[24].hp = 220;
    Monster[24].atk = 180;
    Monster[24].pdef = 30;
    Monster[24].gold = 35;
    Monster[24].id = 45;

    Monster[25].name = QString::fromLocal8Bit("骑士");
    Monster[25].hp = 160;
    Monster[25].atk = 230;
    Monster[25].pdef = 105;
    Monster[25].gold = 65;
    Monster[25].id = 46;

    Monster[26].name = QString::fromLocal8Bit("金骑士");
    Monster[26].hp = 120;
    Monster[26].atk = 150;
    Monster[26].pdef = 50;
    Monster[26].gold = 100;
    Monster[26].id = 47;

    Monster[27].name = QString::fromLocal8Bit("黑暗骑士");
    Monster[27].hp = 180;
    Monster[27].atk = 430;
    Monster[27].pdef = 210;
    Monster[27].gold = 210;
    Monster[27].id = 48;

    Monster[28].name = QString::fromLocal8Bit("大乌贼");
    Monster[28].hp = 1200;
    Monster[28].atk = 180;
    Monster[28].pdef = 20;
    Monster[28].gold = 100;
    Monster[28].id = 49;

    Monster[29].name = QString::fromLocal8Bit("魔龙");
    Monster[29].hp = 1500;
    Monster[29].atk = 600;
    Monster[29].pdef = 250;
    Monster[29].gold = 800;
    Monster[29].id = 50;

    Monster[30].name = QString::fromLocal8Bit("双手剑士");
    Monster[30].hp = 100;
    Monster[30].atk = 680;
    Monster[30].pdef = 50;
    Monster[30].gold = 55;
    Monster[30].id = 51;

    Monster[31].name = QString::fromLocal8Bit("魔法警卫");
    Monster[31].hp = 230;
    Monster[31].atk = 450;
    Monster[31].pdef = 100;
    Monster[31].gold = 100;
    Monster[31].id = 52;

    Monster[32].name = QString::fromLocal8Bit("魔王齐诺");
    Monster[32].hp = 800;
    Monster[32].atk = 500;
    Monster[32].pdef = 100;
    Monster[32].gold = 500;
    Monster[32].id = 53;

    Monster[33].name = QString::fromLocal8Bit("魔王");
    Monster[33].hp = 5000;
    Monster[33].atk = 1580;
    Monster[33].pdef = 190;
    Monster[33].gold = 500;
    Monster[33].id = 54;

}

//主角信息
HERO::HERO()
{
    status = 0;
    level = 1;
    hp = 1000;
    atk = 10;
    pdef = 10;
    pos_x = 6;
    pos_y = 11;
    floor = 0;
    exp = 0;
    gold = 0;
    key1 = 0;
    key2 = 0;
    key3 = 0;
    status = 0;
    face = 0;
}

//提示信息类
HINT::HINT()
{
    num1 = 0;
}
