#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>
using namespace std;

/*精灵类*/
class Sprite {

public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x, int y) :m_x(x), m_y(y),m_color(RED) {};
	//绘制精灵
	virtual void draw() {    //虚函数virtual
		//设置填充颜色
		setfillcolor(m_color);
		//绘制矩形
		fillrectangle(m_x, m_y, m_x+10, m_y+10);
	}
	//移动 
	void moveBy(int dx, int dy) {
		m_x += dx;
		m_y += dy;
	}
	//碰撞检测
	bool collision(const Sprite& other) {
		return m_x == other.m_x && m_y == other.m_y;
	}
	
protected:
	int m_x,m_y;
	COLORREF m_color;

};

//蛇类
class Snake :public Sprite {
public:
	Snake():Snake(0,0){}
	Snake(int x,int y):Sprite(x,y),dir(VK_RIGHT) //默认向右移动
	{
		//初始化蛇（三节）
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}
	void draw() override {    //override重载父类的draw
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}
	}
	//蛇身体的移动
	void bodyMove() {
		//身体跟着蛇头移动
		for (size_t i = nodes.size() - 1; i > 0; i--)  //遍历每一个节点，将前一格的位置赋给后一格
		{
			nodes[i] = nodes[i - 1];
		}
		//移动蛇头
		switch (dir)
		{
		case VK_UP:
			nodes[0].moveBy(0, -10);
			break;
		case VK_DOWN:
			nodes[0].moveBy(0, 10);
			break;
		case VK_LEFT:
			nodes[0].moveBy(-10, 0);
			break;
		case VK_RIGHT:
			nodes[0].moveBy(10, 0);
			break;
		}
	}
	//蛇头碰撞检测
	bool collision(const Sprite& other) {
		return nodes[0].collision(other);
	}
	//蛇增加一节
	void incrment(){
		nodes.push_back(Sprite());
	}
	int dir; //蛇的方向
private:
	vector<Sprite> nodes;   //蛇的所有节点
};

//食物类
class Food :public Sprite {
public:
	Food():Sprite(0,0){
		changeFood();
	}
	void draw() override {   //override重载父类的draw
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}
	//改变食物的位置
	void changeFood() {
		//随机生成食物位置
		m_x = rand() % 64 * 10;  //保证食物坐标是10的整数倍
		m_y = rand() % 48 * 10;
	}
};

//游戏场景
class GameScenc {
public:
	GameScenc(){}
	void run() {
		BeginBatchDraw();//双缓冲绘图,配合EndBatchDraw（消除闪屏）
		cleardevice();
		snake.draw();
		food.draw();
		EndBatchDraw();

		//移动蛇，改变蛇的坐标
		snake.bodyMove();
		//吃食物
		snakeEatFood();
		
		//获取键盘消息
		ExMessage msg = { 0 };
		while (peekmessage(&msg,EX_KEY))
		{
			onMsg(msg);
		}
	}

	//响应消息：鼠标消息和键盘消息
	void onMsg(const ExMessage& msg) {
		//如果有按键消息（有没有按键按下）
		//if (msg.vkcode==VK_UP ||
		//	msg.vkcode == VK_DOWN ||
		//	msg.vkcode == VK_LEFT ||
		//	msg.vkcode == VK_RIGHT)
		//{
		
		//	snake.dir = msg.vkcode;
		//}
		//判断具体是哪个按键按下
		
		switch (msg.vkcode)//防止蛇头能掉头
		{
		case VK_UP:
			if (snake.dir != VK_DOWN) 
				snake.dir = msg.vkcode;
			break;
		case VK_DOWN:
			if (snake.dir != VK_UP) 
				snake.dir = msg.vkcode;
			break;
		case VK_LEFT:
			if (snake.dir != VK_RIGHT) 
				snake.dir = msg.vkcode;
			break;
		case VK_RIGHT:
			if (snake.dir != VK_LEFT) 
				snake.dir = msg.vkcode;
			break;
		}

	}
	//判断蛇是否能吃到食物
	void snakeEatFood() {
		if (snake.collision(food))
		{
			//蛇增加一节
			snake.incrment();
			//食物换位置
			food.changeFood();
		}
		
	}
private:
	Snake snake;
	Food food;
};

int main() {
	initgraph(640, 480);
	//设置随机数种子
	srand(time(nullptr));
	GameScenc scenc;
	while (true)
	{
		scenc.run();
		Sleep(50);
	}

	getchar();
	return 0;
};