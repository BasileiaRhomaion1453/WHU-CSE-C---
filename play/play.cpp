#include<stdio.h>							
#include<stdlib.h>								//包含system函数
#include<graphics.h>							//使用easyx来制作图形窗口
#include<windows.h>								//包含键盘检测函数
#include<time.h>								//包含计时器所需要的函数
#include<mmsystem.h>							//包含声音播放函数mciSendString
#pragma comment(lib,"Winmm.lib")				//包含所需声音播放函数mciSendString

int kill = 0;									//设置分数，击败敌人即可计分
enum my {
	WIDTH = 691,
	HEIGHT = 431,
	bull_num = 999,
	enemy_num = 25,
	BA,
	MO
};												
//定义所需数据如场景长宽，子弹敌人数量，敌人种类

IMAGE bk;										//定义背景图
IMAGE img_player[3];							//定义玩家角色图片
IMAGE img_bull[2];								//定义玩家子弹图片
IMAGE img_enemy[2][2];							//定义敌人图片
IMAGE img_boss[2];								//定义boss图片
IMAGE img_bossbull[2];							//定义boss子弹图片
int x=0;										//用来判断boss移动方向
struct plance{
	double x;
	double y;
	bool live;
	int width;
	int height;
	int hp;
	int type;									//定义单位属性，如位置，是否存活，血量，种类
}player,bull[bull_num],enemy[enemy_num],boss,bossbull[bull_num];//定义各种单位
void loadimg()									//加载图片
{
	
	loadimage(&bk, "./image/bk.png");	//加载背景图

	loadimage(&img_player[0], "./image/van2.png");	//加载自己
	loadimage(&img_player[1], "./image/van.png");
	loadimage(&img_player[2], "./image/van3.png");

	loadimage(&img_bull[0], "./image/go2.png");		//加载子弹
	loadimage(&img_bull[1], "./image/go.png");
	loadimage(&img_bossbull[0], "./image/ebull2.png");
	loadimage(&img_bossbull[1], "./image/ebull.png");

	loadimage(&img_enemy[0][0], "./image/banana2.png");//加载敌人
	loadimage(&img_enemy[0][1], "./image/banana.png");
	loadimage(&img_enemy[1][0], "./image/MO2.png");
	loadimage(&img_enemy[1][1], "./image/MO.png");
	loadimage(&img_boss[0], "./image/bili2.png");
	loadimage(&img_boss[1], "./image/bili.png");
}	
//图片加载函数
void enemyhp(int i)											//决定生成敌人种类
{	
		if (rand() % 10 == 0||rand()%10==1)							//强力敌人生成属性
		{
			enemy[i].type = MO;
			enemy[i].hp = 5;
			enemy[i].width = 65;
			enemy[i].height = 40;
		}
		else                                                //普通敌人生成属性
		{
			enemy[i].type = BA;
			enemy[i].hp = 2;
			enemy[i].width = 34;
			enemy[i].height = 35;
		}
	
}
//敌人分化函数
void gameinit()												//游戏初始化
{
	loadimg();												//图片加载初始化
	player.x = 0;
	player.y = HEIGHT / 2;
	player.live = true;
	player.hp = 3;											//玩家初始化
	for (int i = 0; i < bull_num; i++)						//子弹初始化
	{
		bull[i].x = 0;
		bull[i].y = 0;
		bull[i].live = false;
	}
	for (int i = 0; i < enemy_num; i++)
	{
		enemy[i].live = false;
		enemyhp(i);
	}
	mciSendString(" play ./voice/beijing.mp3 ", NULL, 0, NULL);//背景音乐初始化
	mciSendString(" play ./voice/beijing.mp3 repeat ", NULL, 0, NULL);
	boss.hp = 60;												//boss生命值初始化
}
//游戏初始化函数
void gamedraw()													//图片生成
{

	putimage(0, 0, &bk);
	if (player.hp > 1)											//玩家高生命时的图片
	{
		putimage(player.x, player.y, &img_player[0], NOTSRCERASE);//使用图片位操作达到透明效果
		putimage(player.x, player.y, &img_player[1], SRCINVERT);
	}
	else                                                       //低生命值时变成红色
	{
		putimage(player.x, player.y, &img_player[0], NOTSRCERASE);
		putimage(player.x, player.y, &img_player[2], SRCINVERT);
	}
	for (int i = 0; i < bull_num; i++)						//子弹图片
	{
		if (bull[i].live)									//判断子弹已生成时打印图片
		{
			putimage(bull[i].x, bull[i].y, &img_bull[0], NOTSRCERASE);
			putimage(bull[i].x, bull[i].y, &img_bull[1], SRCINVERT);
		}
	}
	for (int i = 0; i < bull_num; i++)
	{
		if (bossbull[i].live)								//判断boss子弹已生成时打印图片
		{
			putimage(bossbull[i].x, bossbull[i].y, &img_bossbull[0], NOTSRCERASE);
			putimage(bossbull[i].x, bossbull[i].y, &img_bossbull[1], SRCINVERT);
		}
	}
	for (int i = 0; i < enemy_num; i++)						//依据敌人种类打印对应图片
	{
		if (enemy[i].live)
		{
			if (enemy[i].type == MO)
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[1][1], SRCINVERT);
			}
			else
			{
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][0], NOTSRCERASE);
				putimage(enemy[i].x, enemy[i].y, &img_enemy[0][1], SRCINVERT);
			}

		}
	}
	if (boss.live)											//boss已生成时打印boss图片		
	{
		putimage(boss.x, boss.y, &img_boss[0], NOTSRCERASE);
		putimage(boss.x, boss.y, &img_boss[1], SRCINVERT);
	}
}
//输出图像函数
void create_bull()
{
	for (int i = 0; i < bull_num; i++)
	{
		if (!bull[i].live)						//将子弹位置生成在玩家面前
		{
			bull[i].x = player.x+10;
			bull[i].y = player.y;
			bull[i].live = true;
			break;								//一次生成一发后跳出
		}
	}
}
//生成玩家子弹函数
void create_enemy()
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (!enemy[i].live&&!boss.live)				//当敌人被消灭后且boss未出现时敌人重生
		{
			enemy[i].live = true;
			enemy[i].x = WIDTH-35;
			enemy[i].y = rand()%(HEIGHT-40);		//依据随机数来随机安排重生位置
			
			break;
		}
	}
	if (kill == 100||kill==101)						//当分数达标时生成boss
	{

		mciSendString(" play ./voice/here.mp3  ", NULL, 0, NULL);
		boss.live = true;
		boss.width = 104;
		boss.height = 100;
		boss.x = WIDTH-100;
		boss.y = HEIGHT-boss.height;
	}

}
//敌人生成函数
void enemymove(double speed)
{
	for (int i = 0; i < enemy_num; i++)
	{
		if (enemy[i].live)					//当敌人存活时控制敌人移动	
		{
			enemy[i].x -= speed;
			if (enemy[i].x < 0)				//敌人出版边时移除
				enemy[i].live = false;
		}
	}
}
//敌人移动控制函数
void bullmove()
{
	for (int i = 0; i < bull_num; i++)
	{
		if (bull[i].live)				//生成子弹后控制子弹移动
		{
			bull[i].x += 0.1;
			if (bull[i].x > WIDTH + 10)
				bull[i].live = false;	//出版边后移除
		
		}
	}
}
//玩家子弹移动控制函数
void playermove(double speed)
{	//使用windows非阻塞函数检测键盘输入wasd
	//传入一个参数控制移动速度
	if (GetAsyncKeyState('W'))
		if(player.y>=-20)
		player.y -= speed;
	
	if (GetAsyncKeyState('S') )
		if(player.y<=HEIGHT-20)
		player.y += speed;
	
	if (GetAsyncKeyState('A'))
		if(player.x>=-20)
		player.x -= speed;
	
	if (GetAsyncKeyState('D') )
		if(player.x<=WIDTH-30)
		player.x += speed;
	static DWORD t1, t2;//控制射击间隔
	if (GetAsyncKeyState(VK_SPACE) && t2 - t1 > 200)//敲击空格且经过一定时间间隔后才能成功发射
	{
		create_bull();
		mciSendString(" play ./voice/woo.mp3 ", NULL, 0, NULL);//发射音
		t1 = t2;
	}
	t2 = GetTickCount();//返回时间间隔
	
}
//玩家移动输入函数
int timer(unsigned int s)
{
	static DWORD t1 = 0, t2 = 0;
	if (t2 - t1 > s)
	{
		return 1;
		t1 = t2;
	}
	t2 = clock();
	return 0;
}
//计时器原型函数
void hit()
{
	for (int i = 0; i < enemy_num; i++) 
	{
		if (!enemy[i].live)//敌人是否存活受击判定
			continue;
		for (int k = 0; k < 999; k++)
		{
			if (!bull[k].live)//子弹是否存在受击判定
				continue;
			if (bull[k].y+20 > enemy[i].y && bull[k].y<enemy[i].y + enemy[i].height
				&& bull[k].x>=enemy[i].x && bull[k].x < enemy[i].x + enemy[i].width)//子弹命中目标是开始判定伤害
			{
				bull[k].live = false;												//命中后移除子弹
				enemy[i].hp--;														//受击伤害
				if(enemy[i].type==MO)												//依据敌人种类播放不同敌人的受击音效
				mciSendString(" play ./voice/enemyhit.mp3 ", NULL, 0, NULL);
				else
				mciSendString(" play ./voice/enemyhit1.mp3 ", NULL, 0, NULL);
			}
			
		}
		if (player.y + 40 > enemy[i].y && player.y < enemy[i].y + enemy[i].height	//撞击判定
			&& player.x >= enemy[i].x && player.x < enemy[i].x + enemy[i].width)
		{
			static DWORD t1, t2;		//玩家受击后给予无敌时间
			if (t2 - t1 > 500)
			{
				player.hp--;
				mciSendString(" play ./voice/playerhit.mp3 ", NULL, 0, NULL);//玩家受击音效
				t1 = t2;
			}
			t2 = clock();
			
			if (player.hp <= 0)				//玩家生命值判断是否死亡
			{
				player.live = false;
				
			}
		}
		if (enemy[i].hp <= 0)				//敌人生命值判断是否死亡
		{
			enemy[i].live = false;
			if (enemy[i].type == MO)
			{
				enemy[i].hp = 3;
				kill += 2;					//击杀后奖励分数
			}
			else
			{
				enemy[i].hp = 1;
				kill += 1;
			}
		}
	}
}
//玩家敌人受击判定函数
void bosshit()
{
	for (int k = 0; k < 999; k++)
	{
		if (!bull[k].live)//同上敌人受击判定
			continue;
		if (bull[k].y + 20 > boss.y && bull[k].y < boss.y + boss.height
			&& bull[k].x >= boss.x && bull[k].x < boss.x + boss.width)
		{
			bull[k].live = false;
			boss.hp--;
			mciSendString(" play ./voice/bosshit.mp3 ", NULL, 0, NULL);//boss受击音效
		}
		
	}
	for (int k = 0; k < 999; k++)					//boss子弹对玩家判定伤害
	{
		if (!bossbull[k].live)
			continue;
		if (bossbull[k].y + 20 > player.y && bossbull[k].y < player.y + 40
			&& bossbull[k].x <= player.x + 25 && bossbull[k].x >= player.x)
		{
			
			bossbull[k].live = false;
			player.hp--;
			mciSendString(" play ./voice/playerhit.mp3 ", NULL, 0, NULL);
			break;
		}
	}

	if (player.y + 40 > boss.y && player.y < boss.y + boss.height
		&& player.x >= boss.x && player.x < boss.x + boss.width)//boss对玩家碰撞伤害判断并给予无敌时间
	{
		static DWORD t1, t2;
		if (t2 - t1 > 500)
		{
			player.hp--;
			mciSendString(" play ./voice/playerhit.mp3 ", NULL, 0, NULL);
			t1 = t2;
		}
		t2 = clock();
	}
		if (player.hp <= 0)
		{
			player.live = false;

		}
		if (boss.hp <= 0)
			boss.live = false;			//双方死亡判断
	
}
//玩家boss受击判断函数
void boss_attack()
{	if(boss.live)
	for (int i = 0; i < bull_num; i++)//判断boss存活，若是，则自动开始攻击，类似玩家射击
	{
		if (!bossbull[i].live)
		{
			bossbull[i].x = boss.x;
			bossbull[i].y = boss.y + boss.height / 2;
			bossbull[i].live = true;
			break;
		}
	}
}
//boss攻击函数
void bossmove(double speed)
{	//传入boss移动速度参数
	if (boss.live)

	{
		if (x % 2 == 0)//开始时boss生成在下，开始向上移动，每碰撞一次版边转向，偶次向上移动至顶部，奇次向下移动至底部
		{
			boss.y -= speed;

			if (boss.y - speed < 0)
			{
				x++;

			}
		}
		if (x % 2 != 0)
		{
			boss.y += speed;
			if (boss.y + speed > HEIGHT - boss.height)
				x++;
		}



		static DWORD t1, t2;//boss射击间隔
		if (t2 - t1 > 1000)
		{
			boss_attack();
			mciSendString(" play ./voice/bossattack.mp3 ", NULL, 0, NULL);//boss射击音效
			t1 = t2;
		}
		t2 = clock();//返回时间间隔
	}
}
//boss移动与攻击控制函数
void bossbullmove()
{
	for (int i = 0; i < bull_num; i++)//boss子弹移动控制，同玩家子弹移动
	{
		if (bossbull[i].live)
		{
			bossbull[i].x -= 0.1;
			if (bossbull[i].x < 0)
				bossbull[i].live = false;

		}
		
	}

}
//boss子弹移动控制函数



int main()				
{
	printf("欢迎来到VANd大战比利王\n");
	printf("wasd操作移动，空格射击，您的生命值只有3\n当杀敌数达到一定程度时，将会召唤boss，击败boss，取得游戏的胜利吧！\n");
	system("pause");			//暂停后查看说明，按任意键开始
	gameinit();					//游戏初始化
	initgraph(WIDTH, HEIGHT);	//生成窗口
	BeginBatchDraw();			//开始绘图
	
	while (1)					//游戏开始
	{
		
		gamedraw();				//即时绘图
		playermove(0.05);		//控制函数
		bullmove();
		if (timer(500))			//控制敌人生成时间
		{
			create_enemy();
		}
		enemymove(0.02);
		bossmove(0.02);
		hit();
		bosshit();
		bossbullmove();
		if (player.live == false)	//游戏结束条件判断，玩家死亡或是boss死亡
		{
			printf("die!\n");
			printf("您的分数是%d分\n", kill);
			break;
		}
		else if (boss.hp<0)
		{
			printf("win!win!win!您击败了比利王，新日暮里将再次重回Deep Dark Fantasy的怀抱！\n");
			printf("您分数为%d分\n",kill);
			break;
		}
		FlushBatchDraw();			//内存中绘图输出
	}
	EndBatchDraw();		//结束绘图
	mciSendString(" stop ./voice/beijing.mp3 ", NULL, 0, NULL);
	closegraph();
	system("pause");
	return 0;
}
//游戏开始，通过调用其它函数来实现游戏功能
//备注：字符集设置为多字符集