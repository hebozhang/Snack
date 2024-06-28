#include<iostream>
#include<easyx.h>
#include<vector>
#include<ctime>
using namespace std;

/*������*/
class Sprite {

public:
	Sprite() :Sprite(0, 0) {};
	Sprite(int x, int y) :m_x(x), m_y(y),m_color(RED) {};
	//���ƾ���
	virtual void draw() {    //�麯��virtual
		//���������ɫ
		setfillcolor(m_color);
		//���ƾ���
		fillrectangle(m_x, m_y, m_x+10, m_y+10);
	}
	//�ƶ� 
	void moveBy(int dx, int dy) {
		m_x += dx;
		m_y += dy;
	}
	//��ײ���
	bool collision(const Sprite& other) {
		return m_x == other.m_x && m_y == other.m_y;
	}
	
protected:
	int m_x,m_y;
	COLORREF m_color;

};

//����
class Snake :public Sprite {
public:
	Snake():Snake(0,0){}
	Snake(int x,int y):Sprite(x,y),dir(VK_RIGHT) //Ĭ�������ƶ�
	{
		//��ʼ���ߣ����ڣ�
		nodes.push_back(Sprite(20, 0));
		nodes.push_back(Sprite(10, 0));
		nodes.push_back(Sprite(0, 0));
	}
	void draw() override {    //override���ظ����draw
		for (int i = 0; i < nodes.size(); i++)
		{
			nodes[i].draw();
		}
	}
	//��������ƶ�
	void bodyMove() {
		//���������ͷ�ƶ�
		for (size_t i = nodes.size() - 1; i > 0; i--)  //����ÿһ���ڵ㣬��ǰһ���λ�ø�����һ��
		{
			nodes[i] = nodes[i - 1];
		}
		//�ƶ���ͷ
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
	//��ͷ��ײ���
	bool collision(const Sprite& other) {
		return nodes[0].collision(other);
	}
	//������һ��
	void incrment(){
		nodes.push_back(Sprite());
	}
	int dir; //�ߵķ���
private:
	vector<Sprite> nodes;   //�ߵ����нڵ�
};

//ʳ����
class Food :public Sprite {
public:
	Food():Sprite(0,0){
		changeFood();
	}
	void draw() override {   //override���ظ����draw
		setfillcolor(m_color);
		solidellipse(m_x, m_y, m_x + 10, m_y + 10);
	}
	//�ı�ʳ���λ��
	void changeFood() {
		//�������ʳ��λ��
		m_x = rand() % 64 * 10;  //��֤ʳ��������10��������
		m_y = rand() % 48 * 10;
	}
};

//��Ϸ����
class GameScenc {
public:
	GameScenc(){}
	void run() {
		BeginBatchDraw();//˫�����ͼ,���EndBatchDraw������������
		cleardevice();
		snake.draw();
		food.draw();
		EndBatchDraw();

		//�ƶ��ߣ��ı��ߵ�����
		snake.bodyMove();
		//��ʳ��
		snakeEatFood();
		
		//��ȡ������Ϣ
		ExMessage msg = { 0 };
		while (peekmessage(&msg,EX_KEY))
		{
			onMsg(msg);
		}
	}

	//��Ӧ��Ϣ�������Ϣ�ͼ�����Ϣ
	void onMsg(const ExMessage& msg) {
		//����а�����Ϣ����û�а������£�
		//if (msg.vkcode==VK_UP ||
		//	msg.vkcode == VK_DOWN ||
		//	msg.vkcode == VK_LEFT ||
		//	msg.vkcode == VK_RIGHT)
		//{
		
		//	snake.dir = msg.vkcode;
		//}
		//�жϾ������ĸ���������
		
		switch (msg.vkcode)//��ֹ��ͷ�ܵ�ͷ
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
	//�ж����Ƿ��ܳԵ�ʳ��
	void snakeEatFood() {
		if (snake.collision(food))
		{
			//������һ��
			snake.incrment();
			//ʳ�ﻻλ��
			food.changeFood();
		}
		
	}
private:
	Snake snake;
	Food food;
};

int main() {
	initgraph(640, 480);
	//�������������
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