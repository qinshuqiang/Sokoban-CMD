#include <iostream>
using namespace std;
#include <string>
#include <algorithm>
const char gStageData[] = "\
########\n\
# ..   #\n\
# oo   #\n\
#  p   #\n\
########";
const int gStageWidth = 8;
const int gStageHeight = 5;
enum Object
{
	OBJ_SPACE,
	OBJ_WALL,
	OBJ_GOAL,
	OBJ_BLOCK,
	OBJ_BLOCK_ON_GOAL,
	OBJ_MAN,
	OBJ_MAN_ON_GOAL,

	OBJ_UNKNOWN,
};
void initialize(Object* state, int w, int h, const char* stageData);
void draw(const Object* state, int w, int h);
void update(Object* state, char input, int w, int h);
bool checkClear(const Object* state, int w, int h);
void initialize(Object* state, int width, int heiht, const char* stageData) {
	const char* d = gStageData;
	int x = 0;
	int y = 0;
	while (*d != '\0')
	{
		Object t;
		switch (*d)
		{
		case '#':
			t = OBJ_WALL;
			break;
		case ' ':
			t = OBJ_SPACE;
			break;
		case 'o':
			t = OBJ_BLOCK;
			break;
		case 'O':
			t = OBJ_BLOCK_ON_GOAL;
			break;
		case '.':
			t = OBJ_GOAL;
			break;
		case 'p':
			t = OBJ_MAN; 
			break;
		case 'P':
			t = OBJ_MAN_ON_GOAL; 
			break;
		case '\n'://换行
			x = 0;//x移到最左边
			++y;//y换到下一行
			t = OBJ_UNKNOWN;//没有数据
			break;
		default:
			t = OBJ_UNKNOWN;//非法数据
			break;
		}
		++d;
		if (t != OBJ_UNKNOWN) {
			state[y * width + x] = t;//写入
			++x;
		}
	}
}
void draw(const Object* state, int w, int h){
	const char font[] = { ' ','#','.','o','O','p','P' };
	for (int y = 0; y < h; y++)
	{
		for (int x = 0; x < w; x++) {
			Object o = state[y * w + x];
			cout << font[o];
		}
		cout << endl;
	}
}

void update(Object* s, char input, int w, int h) {
	int dx = 0;
	int dy = 0;
	switch (input)
	{
	case 'a':dx = -1; break;
	case 's':dx = 1; break;
	case 'w':dy = -1; break;
	case 'z':dy = 1; break;
	}
	//查找玩家位置
	int i=0;
	for (i = 0; i < w*h; i++)
	{
		if (s[i] == OBJ_MAN || s[i] == OBJ_MAN_ON_GOAL) {
			break;
		}
	}
	int x = i % w;
	int y = i / w;
	//移动后的坐标
	int tx = x + dx;
	int ty = y + dy;
	//超出范围可不行哦
	if (tx < 0 || ty < 0 || tx >= w || ty >= h) {
		return;
	}
	int p = y * w + x;
	int tp = ty * w + tx;
	if (s[tp] == OBJ_SPACE || s[tp] == OBJ_GOAL) {
		//如果是目的地，则将该处设置为“目的地的玩家”
		s[tp] = (s[tp] == OBJ_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
		//如果已经在目的地了，则将玩家所在位置设置为目的地
		s[p]=(s[p]==OBJ_MAN_ON_GOAL)? OBJ_GOAL : OBJ_SPACE;
	}//如果移动到的位置有箱子，如果沿方向的下一个网格是空白或者目的地，则自动
	else if (s[tp] == OBJ_BLOCK || s[tp] == OBJ_BLOCK_ON_GOAL) {
		int tx2 = tx + dx;
		int ty2 = ty + dy;
		if (tx2 < 0 || ty2 < 0 || tx2 >= w || ty2 >= h) {
			return;//超出范围无法移动
		}
		int tp2 = (ty + dy) * w + (tx + dx);//沿着方向的第二个网格位置
		if (s[tp2] == OBJ_SPACE || s[tp2] == OBJ_GOAL) {
			s[tp2] = (s[tp2] == OBJ_GOAL) ? OBJ_BLOCK_ON_GOAL : OBJ_BLOCK;
			s[tp] = (s[tp] == OBJ_BLOCK_ON_GOAL) ? OBJ_MAN_ON_GOAL : OBJ_MAN;
			s[p] = (s[p] == OBJ_MAN_ON_GOAL) ? OBJ_GOAL : OBJ_SPACE;
		}
	}
}
bool checkClear(const Object* state, int w, int h) {
	for (int i = 0; i < w*h; i++)
	{
		if (state[i] == OBJ_BLOCK) {
			return false;
		}
	}
	return true;

}
int main() {
	Object* state = new Object[gStageWidth * gStageHeight];
	initialize(state, gStageWidth, gStageHeight, gStageData);
	while (true)
	{
		//system("cls");
		//首先绘制
		draw(state, gStageWidth, gStageHeight);
		//通关检测
		if (checkClear(state, gStageWidth, gStageHeight)) {
			break;
		}
		//获取输入
		char input;
		cout << "a:left   s:right   w:up  z:down  command?" << endl;
		cin >> input;
		//更新
		update(state, input, gStageWidth, gStageHeight);
	}

	cout << "you win!!!" << endl;
	delete[] state;
	state = NULL;
	system("pause");
	return 0;
}