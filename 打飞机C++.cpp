#include <iostream>
#include<cstdio>
#include <Windows.h>
#include<conio.h>
#include<forward_list>
#include<time.h>
#define BORDLENTH 100
#define BORDHEIGH 20
#define DEATH false
#define LIVE true
using namespace std;
/*======================== All structer ========================*/
typedef struct Frame{
	COORD position[2];
	int flag;
}Fram;

/*====================== All static avaible ===================== */

static int healthPointer = 100;
static int kill_count=0;
static int damage = 30;
/*======================= All Functions ========================*/


void setPos(COORD pos){
	
	HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(out,pos);

}

void setPos(int x, int y){//设置光标的坐标
	COORD loc = { x, y };
	setPos(loc);
}

void hideCursor(){//隐藏光标
	CONSOLE_CURSOR_INFO cursor_info = { 1 , 0 };
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE),&cursor_info);
}

void drawRow(int y,int x1,int x2,char ch){
	setPos(x1,y);
	for (int index = 0; index < (x2 - x1);++index){
		cout << ch;
	}
}

void drawRow(COORD pos1,COORD pos2,char ch){
	if (pos1.Y == pos2.Y)
		drawRow(pos1.Y,pos1.X,pos2.X,ch);
	else{
		setPos(0,25);
		cout << "error 001 : 无法进行填充，纵坐标不相等。";
		system("pause");
	}
}

void drawColum(int x,int y1,int y2,char ch){
	setPos(x, y1);
	for (int index = 0; index < (y2 - y1);++index){
		setPos(x, y1+index);
		cout << ch;
	}
}
void drawColum(COORD pos1,COORD pos2,char ch){
	if (pos1.X == pos2.X)
		drawColum(pos1.X, pos1.Y, pos2.Y, ch);
	else{
		setPos(0, 25);
		cout << "error 002 : 无法进行填充，横坐标不相等。";
		system("pause");
	}
}


bool drawMenu(){
	COORD leftColBordUp = {1,1};
	COORD leftColBordDo = { leftColBordUp.X, leftColBordUp.Y+BORDHEIGH };

	COORD rightColBordUp = { leftColBordUp.X+BORDLENTH+1, leftColBordUp.Y };
	COORD rightColBordDo = {rightColBordUp.X,rightColBordUp.Y+BORDHEIGH};

	COORD upBordLeft = { leftColBordUp.X+1 , leftColBordUp.Y - 1 };
	COORD upBordRight = {upBordLeft.X+BORDLENTH, leftColBordUp.Y - 1 };

	COORD downBordLeft = { upBordLeft.X, upBordRight.Y + BORDHEIGH };
	COORD downBordRight = { downBordLeft.X + BORDLENTH, upBordLeft.Y + BORDHEIGH };

	hideCursor();

	drawColum(leftColBordUp,leftColBordDo,'|');
	drawRow(upBordLeft,upBordRight,'_');
	drawColum(rightColBordUp,rightColBordDo,'|');
	drawRow(downBordLeft,downBordRight,'_');
	cout << endl;

	COORD midRowLeft = { upBordLeft.X, upBordLeft.Y+3 };
	COORD midRowRight = { midRowLeft.X + BORDLENTH, midRowLeft.Y };
	COORD middle = { (midRowLeft.X + BORDLENTH) / 2 ,(upBordLeft.Y+BORDHEIGH)/2};

	drawRow(midRowLeft,midRowRight,'_');
	setPos(middle.X - 4 , midRowLeft.Y - 1);
	cout << "打";
	setPos(middle.X , midRowLeft.Y - 1);
	cout << "飞";
	setPos(middle.X + 4 , midRowLeft.Y - 1);
	cout << "机";


	COORD chosen = { middle.X -10, middle.Y+1 };
	setPos(chosen.X,chosen.Y);
	cout << "->  简单的电脑";
	setPos(chosen.X,chosen.Y+1);
	cout << "    令人发狂的电脑";

	setPos(middle.X-5,middle.Y+(BORDHEIGH/3));
	cout << "Maker : Black_Sheep ";

	while (true){
		if (_kbhit()){
			char ch = _getch();
			tolower(ch);
			switch (ch){
			case 'w': if (middle.Y+1 != chosen.Y){
				setPos(chosen.X, --chosen.Y);
				cout << "->  简单的电脑";
				
				setPos(chosen.X , chosen.Y+1);
				cout << "    令人发狂的电脑"; break;
			}
			case 's':if (middle.Y + 2 != chosen.Y){
				setPos(chosen.X, chosen.Y);
				cout << "    简单的电脑";
				setPos(chosen.X , ++chosen.Y);
				cout << "->  令人发狂的电脑";  break;
			}
			default:;
			}
			if (ch == 'j'){
				if (chosen.Y == middle.Y + 1)
					return true;
				else
					return false;
			}
				
		}
	}

	getchar();

}

/*======================= the game class ========================*/

class player{
public:
	COORD &getPosition();
	void setPosition(COORD position);
	void drawGameObj();
	bool changeHP(int pointer);
	void clearGameObj();
	static void take_damage(int damage);
	player();
	~player();
private:
	
	bool status;
	COORD pos;
	int lenth;
	int height;
};

COORD &player::getPosition(){
	return this->pos;

}
void player::setPosition(COORD position){
	this->pos = position;
}

void player::clearGameObj(){
	setPos(this->pos);
	cout << "           ";
	setPos(pos.X, pos.Y + 1);
	cout << "           ";
	setPos(pos.X, pos.Y + 2);
	cout << "           ";
}

void player::take_damage(int damage){

	healthPointer -= damage;

}

player::player(){
	this->pos = { 30,37 };
	this->status = LIVE;
	this->lenth = 12;
	this->height = 3;
	healthPointer = 100;
}

void player::drawGameObj(){
	setPos(this->pos);
	cout << "     |     ";
	setPos(pos.X,pos.Y+1);
	cout << "===========";
	setPos(pos.X, pos.Y + 2);
	cout << "   =====   ";
}
player::~player(){
}

typedef struct enemy_coord{
	COORD position;
	bool flag=false;
	int hp=100;
	int fire_count=0;
}enemy_coord;
class enemy{
public:
	~enemy();
	void drawGameObj();
	void clearGameObj();
	void enemy_move(COORD planePos);
	void shoot(COORD planePos);
	enemy_coord* getEnemy_coord();
	static void take_damage(enemy_coord* enemyPos,int damage);
	void randomPos();
	bool isempty();
	int enemy_count();
private:
	int hp[15];
	enemy_coord pos[15];
	void fire(COORD position,COORD planePos);
};

enemy_coord* enemy::getEnemy_coord(){
	return this->pos;
}

int enemy::enemy_count(){
	int count=0;
	for (const enemy_coord *ite = &pos[0]; ite != &pos[15];++ite){
		if ((*ite).flag)
			++count;
	}
	return count;
}

void enemy::take_damage(enemy_coord *enemyPos,int damage){
	enemyPos->hp -= damage;
	if (enemyPos->hp <= 0){
		setPos(enemyPos->position);
		cout << "       ";
		enemyPos->flag = false;
		enemyPos->hp = 100;
		++kill_count;
	}
}

bool enemy::isempty(){
	for (int index=0;index<15;++index){
		if (pos[index].flag){
			return false;
		}
	}
	return true;
}

void enemy::randomPos(){

	int x=rand()%40;
	int y = 2;
	if (x <= 2){
		x = 3;
	}

	for (int index=0;index<15;++index){
		if (pos[index].flag==false){
			pos[index].position = { x , y };
			pos[index].flag = true;
			break;
		}
	}
}

void enemy::drawGameObj(){
	for (const enemy_coord *ite = &pos[0]; ite != &pos[15]; ++ite){
		if ((*ite).flag){
			setPos((*ite).position);
			cout << "(**O**)";
		}
	}
}

void enemy::clearGameObj(){
	for (const enemy_coord *ite = &pos[0]; ite != &pos[15];++ite){
		if ((*ite).flag){
			setPos((*ite).position);
			cout << "       ";
		}
	}
}

void enemy::enemy_move(COORD planePos){
	for (enemy_coord * ite = &pos[0]; ite != &pos[15];++ite){
		if ((*ite).flag){
			
			if (((*ite).position.Y == (planePos.Y + 1))
					&& ((abs(planePos.X - (*ite).position.X + 1) < 8))){
				healthPointer-=100;
				(*ite).flag = false;
				}
			else if ((*ite).position.Y < 39){
				++((*ite).position.Y);
			}
			else{
				(*ite).flag = false;
			}
		}
	}
}

void enemy::fire(COORD position,COORD planePos){
	setPos(position.X+3,position.Y);
	int i;
	for (i=1;;++i){
		if (position.Y + i >= 40 || (((position.Y + i) == (planePos.Y + 1))&&((position.X - planePos.X >= 0) && (position.X - planePos.X) < 7))){
			healthPointer-=40;
			break;
		}
		else{
			setPos(position.X+4, position.Y + i);
			cout << '|';
		}

	}
	Sleep(100);
	for (int j=1;j<=i;++j){
		setPos(position.X + 4, position.Y + i);
		cout << ' ';
	}
}

void enemy::shoot(COORD planePos){
	int fire_flag=rand()%60;
	for (enemy_coord *ite=&pos[0];ite!=&pos[15];++ite){
		if ((*ite).flag){
				if (fire_flag<=1){
					fire((*ite).position,planePos);
				}
	
		}
	}
}

enemy::~enemy(){}

class map{
public:
	map();
	void drawMap();
	COORD getPos();
	int getMapHeight();
	int getMapLenth();
private:
	COORD leftUpPosition;
	unsigned int lenth;
	unsigned int height;
};

int map::getMapHeight(){
	return this->height;
}

int map::getMapLenth(){
	return this->lenth;
}

map::map(){
	this->leftUpPosition = { 1, 1 };
	this->lenth = 50;
	this->height= 40;
}

COORD map::getPos(){
	return this->leftUpPosition;
}

void map::drawMap(){

	system("cls");

	drawRow(leftUpPosition.Y, leftUpPosition.X, leftUpPosition.X + lenth, '_');

	drawRow(leftUpPosition.Y + height - 1, leftUpPosition.X, leftUpPosition.X + lenth, '_');

	drawColum(leftUpPosition.X,leftUpPosition.Y+1,leftUpPosition.Y+height,'|');

	drawColum(leftUpPosition.X + lenth - 1, leftUpPosition.Y+1, leftUpPosition.Y + height, '|');

}




typedef struct shellCoord{
	COORD position;
	bool flag = false;
}shellCoord;
class shell{
public:
	shell();
	~shell();
	void addNew(COORD planePos);
	void destory();
	void drawGameObj();
	void clearGameObj();
	void shellsMove(enemy_coord *enemyPos);
	bool isEmpty();

private:
	int count;
	shellCoord pos[40];
};

shell::shell(){
	count = 0;
}

bool shell::isEmpty(){
	if (count == 0)
		return true;
	else
		return false;
}

void shell::shellsMove(enemy_coord *enemyPos){
	
	for (shellCoord* ite = &pos[0]; ite != &pos[40];++ite){
		bool touched = false;
		if ((*ite).flag && (*ite).position.Y > 2){
			for (enemy_coord *it = &enemyPos[0];it!=&enemyPos[15];++it){
				if ((*it).flag){
					int y1 = (*it).position.Y;
					int x1 = (*it).position.X;
					int y2 = (*ite).position.Y;
					int x2 = (*ite).position.X;
					if (y1 == y2)
						if ((x2 - x1 >= 0) && (x2 - x1 < 8)){
						(*ite).flag = false;
						touched = true;
						enemy::take_damage(it, damage);
						break;
						}
				}
			}
			--((*ite).position.Y);
		}
		else if ((*ite).flag){
			(*ite).flag = false;
		}
	}
}

void shell::addNew(COORD planePos){

		for (int i=0;i<40;++i){
			if (pos[i].flag == false){
				pos[i].position = { planePos.X + 5, planePos.Y - 1 }; 
				pos[i].flag = true;
				++count; break;
			}
			else if (i == 39){
				i = -1;
			}
		}
	}

void shell::clearGameObj(){
	for (const shellCoord *ite = &pos[0]; ite != &pos[40];++ite){
		if ((*ite).flag == true){
			setPos((*ite).position);
			cout << ' ';
		}
	}
}

void shell::drawGameObj(){
	for (const shellCoord *ite = &pos[0]; ite != &pos[40]; ++ite){
		if ((*ite).flag != false){
			setPos((*ite).position);
			cout << '^';
		}
	}
}

shell::~shell(){
}


/*================ the game main function ===============*/
int main()
{
	int mapHeight;
	int mapLenth;
	bool is_easymode;
start:
	map gameMap;
	COORD mapPos;
	player gamer;
	COORD &playerPos = gamer.getPosition();
	shell shells;
	enemy enemys;

	const int frame3 = 3;
	int frame_count=0;
	srand(time(NULL));

	mapHeight = gameMap.getMapHeight();
	mapLenth = gameMap.getMapLenth();

	healthPointer = 100;
	kill_count = 0;
	is_easymode= drawMenu();
	gameMap.drawMap();
	gamer.drawGameObj();
	mapPos = gameMap.getPos();
	enemy_coord* enemyPos = enemys.getEnemy_coord();

	if (!is_easymode){
		damage = 10;
	}
	while (true){
		hideCursor();
		srand(time(NULL));

		if (_kbhit()){
			COORD gamerPos = playerPos;
			char ch = _getch();
			tolower(ch);
			switch (ch){
			case 'w':if (playerPos.Y > mapPos.Y + 1){
				gamer.clearGameObj(); --(playerPos.Y); break;
			}
					 else{
						 break;
					 }
			case 's':if (playerPos.Y < (mapPos.Y + mapHeight - 4)){
				gamer.clearGameObj(); ++(playerPos.Y); break;
			}
					 else{
						 break;
					 }
			case 'a':if (playerPos.X > mapPos.X + 1){
				gamer.clearGameObj(); --(playerPos.X); break;
			}
					 else{
						 break;
					 }
			case 'd':if (playerPos.X < (mapPos.X + mapLenth - 12)){
				gamer.clearGameObj(); ++(playerPos.X); break;
			}
					 else{
						 break;
					 }
			case 'j':shells.addNew(playerPos); break;

			default:break;
			}

		}
	

		gamer.drawGameObj();
		shells.drawGameObj();
		enemys.drawGameObj();
		
		
		Sleep(50);
		shells.clearGameObj();
		shells.shellsMove(enemyPos);
		
	
		if (frame_count==frame3){
			frame_count = 0;
			enemys.randomPos();
			Sleep(100);
			enemys.clearGameObj();
			enemys.enemy_move(playerPos);
		}

		++frame_count;
		if (healthPointer < 0){
			
			system("cls");
			while (true){
				setPos(50, 30);
				cout << "你本次击杀:" << kill_count;
				setPos(50, 31);
				cout << "按y键返回开始界面，按n键退出";
				char ch = _getch();
				if (ch == 'y'){
					system("cls");
					goto start;
				}
				else if(ch == 'n'){
					return 0;
				}
			}
		}
	}

	return 0;
}