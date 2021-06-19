#include <ncurses.h>
#include <list>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>
using namespace std;

bool quit = false;
int way = 2; // way : head 방향
int stage = 0;
bool stage_clear = false; //모든 스테이지를 클리어했는지 여부
int snake_s,  growth_s, poison_s; //속도 조절

//item, gate 좌표
int growth_x, growth_y;
int poison_x, poison_y;
int gate1_x, gate1_y;
int gate2_x, gate2_y;

int item_g, item_p, use_g, pass; // pass : gate를 body가 다 통과할 때까지 새 gate가 생기지 않도록 하기 위함
int max_length, goal_b, goal_gr, goal_p, goal_g;

void set_stage(int stage);
void move();
void show();
void get_growth();
void get_poison();
void get_gate();

int map[4][21][21] =
	{
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		},
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		},
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		},
		{
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1},
			{2, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2}
		}
};

class snake {
public:
	int x, y;
	snake(int x, int y) {
		this->x = x;
		this->y = y;
	}
};

//body들의 각 좌표들까지 저장하기 위함
list<snake> body;
list<snake>::iterator it;

int main() {
	initscr();
	noecho(); //입력 자동으로 화면 출력 X
	curs_set(0);
	nodelay(stdscr, true);
	
	start_color();
	init_pair(1, COLOR_WHITE, COLOR_WHITE);
	init_pair(2, COLOR_CYAN, COLOR_CYAN);
	init_pair(3, COLOR_BLUE, COLOR_BLUE);
	init_pair(4, COLOR_YELLOW, COLOR_YELLOW);
	init_pair(5, COLOR_GREEN, COLOR_GREEN);
	init_pair(6, COLOR_RED, COLOR_RED);
	init_pair(7, COLOR_MAGENTA, COLOR_MAGENTA);
	
	keypad(stdscr, TRUE);
	set_stage(stage);

	while(quit != true) {
		snake_s --;
		growth_s --;
		poison_s --;
		
		int key = getch(); //키패드 입력을 받아옴
		switch(key) {
			case KEY_LEFT:
				if(way == 2)	
					quit = true;
				else
					way = 1;
				break;
			case KEY_RIGHT:
				if(way == 1)	
					quit = true;
				else
					way = 2;
				break;
			case KEY_UP:
				if(way == 4)	
					quit = true;
				else	
					way = 3;
				break;
			case KEY_DOWN:
				if(way == 3)	
					quit = true;
				else
					way = 4;
				break;
			case 'q':
				quit = true;
				break;
		}
		
		if(snake_s == 0) { //snake 이동 속도 조절
			snake_s = 5;
			move();
		}

		if(growth_s == 0) { //growth 생성 속도 조절
			growth_s = 100;
			map[stage][growth_y][growth_x] = 0;
			get_growth();
		}
		
		if(poison_s == 0) { //poison 생성 속도 조절
			poison_s = 70;
			map[stage][poison_y][poison_x] = 0;
			get_poison();
		}
	
		show();
		usleep(50000);
	}
	
	if(stage_clear == false) {
		clear();
		mvprintw(10, 15, "GAME OVER\n");
	}
	
	timeout(-1); //자동 종료 방지
	refresh();
	getch();  // 사용자 입력 대기 -> 아무거나 눌러도 종료
	endwin();
	return 0;
}

void set_stage(int n) {
	clear();
	mvprintw(10, 15, "Stage %d", stage + 1);
	refresh();
	
	//속도 조절위함(값 변경 가능)
	snake_s = 5;
	growth_s = 100;
	poison_s = 70;
	
	item_g = 0;
	item_p = 0;
	use_g = 0;
	
	srand(time(NULL));
	goal_b = (rand() % 8) + 5;
	goal_gr = (rand() % 5) + 2;
	goal_p = (rand() % 5) + 2;
	goal_g = (rand() % 4) + 1;
	
	body.clear();
	
	get_growth();
	get_poison();
	get_gate();
	
	if(stage == 0) {
		for(int i = 0; i < 3; i++)
		body.push_front(snake(i+9, 10));
		way = 1;
	}
	else if(stage == 1) {
		for(int i = 0; i < 3; i++)
		body.push_front(snake(i+9, 6));
		way = 2;
	}
	else if(stage == 2) {
		for(int i = 0; i < 3; i++)
		body.push_front(snake(6, i+5));
		way = 4;
	}
	else if(stage == 3) {
		for(int i = 0; i < 3; i++)
		body.push_front(snake(6, i+4));
		way = 3;
	}
	max_length = body.size();
	
	usleep(1000000);
}

void move() {
	snake head = body.front(); // snake의 head 부분
	int x = head.x;
	int y = head.y;
		
	snake tail = body.back(); // snake의 tail 부분
	map[stage][tail.y][tail.x] = 0;
	body.pop_back(); //가장 마지막 좌표 삭제

	if(way == 1) //왼쪽
		x--;
	else if(way == 2) //오른쪽
		x++;
	else if(way == 3) //위쪽
		y--;
	else if (way == 4) //아래쪽
		y++;
			
	body.push_front(snake(x, y)); //새로 입력 받은 방향에 따른 좌표 추가

	if(map[stage][y][x] == 1 || map[stage][y][x] == 4) // map 밖으로 나가거나 자신의 Body를 통과하려는 경우 게임 오버
		quit = true;
	
	bool growth = false, poison = false;
	//growth 아이템을 먹게 되는 경우
	if(map[stage][y][x] == 5) {
		growth = true;
		max_length++;
		item_g++;
		body.push_back(snake(tail.x, tail.y)); //지웠던 마지막 좌표를 다시 추가해 길이를 1 증가시킴
	}
	
	//poison 아이템을 먹게 되는 경우
	else if (map[stage][y][x] == 6) {
		if(body.size() <= 3) //poison 아이템을 먹고 길이가 3보다 작아지는 경우 게임 오버
			quit = true;
		else {
			poison = true;
			item_p++;
			map[stage][body.back().y][body.back().x] = 0;
			body.pop_back();
		}
	}
	
	//gate에 들어가게 되는 경우
	if(map[stage][y][x] == 7) {
		pass = body.size();
		use_g++;
		int way_t = way;
		int exit_x, exit_y;
		
		//gate가 2개이므로 입구와 출구 확인
		if(body.front().x == gate1_x && body.front().y == gate1_y) {
			exit_x = gate2_x;
			exit_y = gate2_y;
		}
		else {
			exit_x = gate1_x;
			exit_y = gate1_y;
		}
		
		if(exit_x == 0) { //왼쪽 wall에 출구 gate
			way = 2;
			body.front().x = exit_x + 1;
			body.front().y = exit_y;
		}
		else if(exit_x == 20) { //오른쪽 wall에 출구 gate
			way = 1;
			body.front().x = exit_x - 1;
			body.front().y = exit_y;
		}
		else if(exit_y == 0) { //상단 wall에 출구 gate
			way = 4;
			body.front().x = exit_x;
			body.front().y = exit_y + 1;
		}
		else if(exit_y == 20) { //하단 wall에 출구 gate
			way = 3;
			body.front().x = exit_x;
			body.front().y = exit_y - 1;
		}
		else { // gate가 가장자리에 있지 않은 경우
			//진입 방향과 일치하는 방향이 우선
			switch(way) {
				case 1:
					body.front().x = exit_x - 1;
					body.front().y = exit_y;
					break;
				case 2:
					body.front().x = exit_x + 1;
					body.front().y = exit_y;
					break;
				case 3:
					body.front().x = exit_x;
					body.front().y = exit_y - 1;
					break;
				case 4:
					body.front().x = exit_x;
					body.front().y = exit_y + 1;
					break;
			}
			
			if(map[stage][body.front().y][body.front().x] != 0) {   
				//진입 방향의 시계 방향으로 회전하는 방향
				if(way_t == 1) //진입 방향 : 왼쪽
					way = 3;
				else if(way_t == 2) //진입 방향 : 오른쪽
					way = 4;
				else if(way_t == 3) //진입 방향 : 위쪽
					way = 2;
				else if(way_t == 4) //진입 방향 : 아래쪽
					way = 1;
				
				switch(way) {
					case 1:
						body.front().x = exit_x - 1;
						body.front().y = exit_y;
						break;
					case 2:
						body.front().x = exit_x + 1;
						body.front().y = exit_y;
						break;
					case 3:
						body.front().x = exit_x;
						body.front().y = exit_y - 1;
						break;
					case 4:
						body.front().x = exit_x;
						body.front().y = exit_y + 1;
						break;
				}

				if(map[stage][body.front().y][body.front().x] != 0) {
					//진입 방향의 역시계 방향으로 회전하는 방향
					if(way_t == 1) //진입 방향 : 왼쪽
						way = 4;
					else if(way_t == 2) //진입 방향 : 오른쪽
						way = 3;
					else if(way_t == 3) //진입 방향 : 위쪽
						way = 1;
					else if(way_t == 4) //진입 방향 : 아래쪽
						way = 2;

					switch(way) {
						case 1:
							body.front().x = exit_x - 1;
							body.front().y = exit_y;
							break;
						case 2:
							body.front().x = exit_x + 1;
							body.front().y = exit_y;
							break;
						case 3:
							body.front().x = exit_x;
							body.front().y = exit_y - 1;
							break;
						case 4:
							body.front().x = exit_x;
							body.front().y = exit_y + 1;
							break;
					}
					
					if (map[stage][body.front().y][body.front().x] != 0) {
						//진입 방향과 반대 방향
						if(way_t == 1) //진입 방향 : 왼쪽
							way = 2;
						else if(way_t == 2) //진입 방향 : 오른쪽
							way = 1;
						else if(way_t == 3) //진입 방향 : 위쪽
							way = 4;
						else if(way_t == 4) //진입 방향 : 아래쪽
							way = 3;
						
						switch(way) {
							case 1:
								body.front().x = exit_x - 1;
								body.front().y = exit_y;
								break;
							case 2:
								body.front().x = exit_x + 1;
								body.front().y = exit_y;
								break;
							case 3:
								body.front().x = exit_x;
								body.front().y = exit_y - 1;
								break;
							case 4:
								body.front().x = exit_x;
								body.front().y = exit_y + 1;
								break;
						}
					}
				}
			}
		}
	}

	//snake 화면에 표시
	for(it = body.begin(); it != body.end(); it++)
		map[stage][(*it).y][(*it).x] = 4;
	map[stage][body.front().y][body.front().x] = 3;
	
	if(growth == true) { //새 growth 표시
		get_growth();
		growth_s = 100;
	}
	
	if(poison == true) { //새 poison 표시
		get_poison();
		poison_s = 70;
	}
	
	if(pass > 0) { //새 gate 표시
		pass--;
		
		if(pass ==0) {
			map[stage][gate1_y][gate1_x] = 1;
			map[stage][gate2_y][gate2_x] = 1;
			get_gate();
		}
	}
	
	if(max_length >= goal_b && item_g >= goal_gr && item_p >= goal_p && use_g >= goal_g) {
		if(stage < 3) {
			clear();
			mvprintw(10, 15, "STAGE%d CLEAR!\n", stage + 1);
			refresh();
			usleep(1000000);
			stage++;
			set_stage(stage);
		}
		else {
			stage_clear = true;
			clear();
			mvprintw(10, 15, "ALL CLEAR!\n");
			refresh();
			usleep(1000000);
			quit = true;
		}
	}
}

void show() {
	clear();
	
	//map, snake, grwoth, poison, gate 화면에 색을 이용해 표시
	for(int i = 0; i < 21; i++) {
		for(int j = 0; j <21; j++) {
			if(map[stage][i][j] == 0)
				printw(" ");
			else if(map[stage][i][j] == 1) {
				attron(COLOR_PAIR(1));
				printw("-");
				attroff(COLOR_PAIR(1));
			}
			else if(map[stage][i][j] == 2) {
				attron(COLOR_PAIR(2));
				printw("+");
				attroff(COLOR_PAIR(2));
			}
			else if(map[stage][i][j] == 3) {
				attron(COLOR_PAIR(3));
				printw("H");
				attroff(COLOR_PAIR(3));
			}
			else if(map[stage][i][j] == 4) {
				attron(COLOR_PAIR(4));
				printw("B");
				attroff(COLOR_PAIR(4));
			}
			else if(map[stage][i][j] == 5) {
				attron(COLOR_PAIR(5));
				printw("G");
				attroff(COLOR_PAIR(5));
			}
			else if(map[stage][i][j] == 6) {
				attron(COLOR_PAIR(6));
				printw("P");
				attroff(COLOR_PAIR(6));
			}
			else if(map[stage][i][j] == 7) {
				attron(COLOR_PAIR(7));
				printw("D");
				attroff(COLOR_PAIR(7));
			}
		}
		printw("\n");
	}
	
	mvprintw(1, 25, "Score Board");
	mvprintw(2, 25, "B : %d/ %d", body.size(), max_length);
	mvprintw(3, 25, "+ : %d", item_g);
	mvprintw(4, 25, "- : %d", item_p);
	mvprintw(5, 25, "G : %d", use_g);
	mvprintw(7, 25, "Mission");
	mvprintw(8, 25, "B : %d ", goal_b);
	if(max_length < goal_b)
		printw("( )");
	else
		printw("(V)");
	mvprintw(9, 25, "+ : %d ", goal_gr);
	if(item_g < goal_gr)
		printw("( )");
	else
		printw("(V)");
	mvprintw(10, 25, "- : %d ", goal_p);
	if(item_p< goal_p)
		printw("( )");
	else
		printw("(V)");
	mvprintw(11, 25, "G : %d ", goal_g);
	if(use_g < goal_g)
		printw("( )");
	else
		printw("(V)");
		
	refresh();
}

void get_growth() {
	while(true) {
		srand(time(NULL));
		growth_x = (rand() % 19) + 1;
		growth_y = (rand() % 19) + 1;

		if(map[stage][growth_y][growth_x] == 0) { //growth 아이템이 빈 공간에 생기도록 조정
			map[stage][growth_y][growth_x] = 5;
			break;
		}
	}
}

void get_poison() {
	while(true) {
		srand(time(NULL));
		poison_x = (rand() % 19) + 1;
		poison_y = (rand() % 19) + 1;

		if(map[stage][poison_y][poison_x] == 0) { //poison 아이템이 빈 공간에 생성되도록 조정
			map[stage][poison_y][poison_x] = 6;
			break;
		}
	}
}

void get_gate() {
	srand(time(NULL));
	while(true) {
		gate1_x = (rand() % 21);
		gate1_y = (rand() % 21);

		if(map[stage][gate1_y][gate1_x] == 1) { //gate가 wall에 생성되도록 조정
			map[stage][gate1_y][gate1_x] = 7;
			break;
		}
	}
	while(true) {
		gate2_x = (rand() % 21);
		gate2_y = (rand() % 21);

		if(map[stage][gate2_y][gate2_x] == 1 && map[stage][gate2_y][gate2_x] != 7) {
			map[stage][gate2_y][gate2_x] = 7; //gate가 wall에 생성되고 다른 gate와 겹치지 않도록 조정
			break;
		}
	}
}
