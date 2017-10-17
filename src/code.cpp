#include<iostream>
#include<ctime>
#include<Windows.h>
#include<conio.h>

#define X 80
#define Y 25
#define Field(Cord) field[Cord.x-1][Cord.y-3]
#define sleep Sleep(diff)

using namespace std;

struct Cord
{
	char x, y;
	Cord(char x = 0, char y = 0){
		this->x = x, this->y = y;
	}
	Cord operator +(Cord add)
	{
		Cord ans(x + add.x, y + add.y);
		return ans;
	}
};

class Snake
{
	Cord * snake;
	short cSize, size, head, tail;
	void resize();
public:
	Snake();
	Cord move(Cord);
	void add(Cord);
	~Snake(){
		delete[]snake;
	}
};

Snake::Snake() :head(0), tail(0)
{
	size = 5;
	snake = new Cord[size];
	cSize = 0;
}

Cord Snake::move(Cord direction)
{
	Cord temp(snake[tail]);
	head = ++head%size;
	tail = ++tail%size;
	snake[head] = direction;
	return temp;
}

void Snake::add(Cord direction)
{
	if (++cSize == size){
		resize();
	}
	head = ++head%size;
	snake[head] = direction;
}

void Snake::resize()
{
	Cord * temp = new Cord[size * 2];
	for (int i = 0; i<cSize; i++){
		temp[i] = snake[(tail + i) % size];
	}
	head = cSize - 1;
	tail = 0;
	delete[]snake;
	snake = temp;
	size *= 2;
}

/***********************************************************************************************************/

class FeedTheSnake
{
	char field[78][21];
	char face;
	Snake snake;
	Cord head, direction;
	int score, diff;
public:
	FeedTheSnake();
	void printFrame();
	void start();
	void food();
	void gotoxy(int x, int y);
	void gotoxy(Cord direction);
	void input();
	bool move();
	void dead(); // to be implemented
	void updateScore();
};

FeedTheSnake::FeedTheSnake() :score(0), diff(70), head(38, 14), face('>'), direction(1)
{
	for (int i = 0; i<78; i++){
		for (int j = 0; j<21; j++){
			field[i][j] = NULL;
		}
	}
	srand(time(0));
	printFrame();
	start();
	food();
	do{
		sleep;
		input();
	} while (move());
}

void FeedTheSnake::printFrame()
{
	gotoxy(31, 0);
	cout << "<Feed the Snake>" << "\nYour Score : " << score << "\n";
	//	char border=177;//char(ASCII) ±=177;
	cout << "±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±\n±";
	for (int i = 3; i<24; i++){
		gotoxy(79, i);
		cout << "±\n±";
	}
	cout << "±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±";
	gotoxy(0, 0);
}


void FeedTheSnake::start()
{
	snake.add(head);
	Field(head) = 's';
	gotoxy(head);
	cout << ">";
	for (int i = 0; i<3; i++){
		sleep;
		cout << "\b*>";
		head.x++;
		snake.add(head);
		Field(head) = 's';
	}
}

void FeedTheSnake::food()
{
	Cord temp;
	do{
		temp.x = rand() % 78 + 1;
		temp.y = rand() % 21 + 3;
	} while (Field(temp));
	Field(temp) = 'f';
	gotoxy(temp);
	cout << 'F';
}

void FeedTheSnake::gotoxy(int x, int y)
{
	COORD coord = { x, y };
	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(console, coord);
}

void FeedTheSnake::gotoxy(Cord direction)
{
	gotoxy(direction.x, direction.y);
}

void FeedTheSnake::input()
{
	int KB_code = NULL;
	if (_kbhit())
	{
		_getch();
		KB_code = _getch();
		switch (KB_code)
		{
		case 75://left
			if (direction.x - 1){
				face = '<';
				direction.x = -1;
				direction.y = 0;
			}
			break;
		case 77://right
			if (direction.x + 1){
				face = '>';
				direction.x = 1;
				direction.y = 0;
			}
			break;
		case 72://up
			if (direction.y - 1){
				face = '^';
				direction.x = 0;
				direction.y = -1;
			}
			break;
		case 80://down
			if (direction.y + 1){
				face = 'V';
				direction.x = 0;
				direction.y = 1;
			}
			break;
		}
		fflush(stdin);
	}
}

bool FeedTheSnake::move()
{
	Cord next = head + direction,temp;
	if (next.x == 0 || next.x == 79 || next.y == 2 || next.y == 24 || Field(next) == 's'){
		return false;
	}
	else if (Field(next) == 'f'){
		snake.add(next);
		updateScore();
		food();
	}
	else{
		temp = snake.move(next);
		gotoxy(temp);
		cout << ' ';
		Field(temp) = NULL;
	}

	Field(next) = 's';
	gotoxy(head);
	cout << '*';
	gotoxy(next);
	cout << face;
	head = next;

	return true;
}

void FeedTheSnake::updateScore()
{
	gotoxy(13,1);
	cout << ++score;
}

/***************************************************************************************************************/


int main()
{
	FeedTheSnake run;
	getchar();
	return 0;
}