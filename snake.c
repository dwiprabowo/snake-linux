#include <ncurses.h>
#include <time.h>
#include <stdlib.h>

#define MOVING_SCALE 1
#define MOVING_SPEED 5

#define EXIT_KEY 2

#define CENTER_X (COLS/2)
#define CENTER_Y (LINES/2)

#define RANDOM_X (rand()%(COLS))
#define RANDOM_Y (rand()%(LINES))
#define MAX_POIN 9
#define RANDOM_POIN ((rand()%MAX_POIN)+1)

#define DIRECTION_LEFT 0
#define DIRECTION_RIGHT 1
#define DIRECTION_UP 2
#define DIRECTION_DOWN 3

#define CHARACTHER_HEAD 187
#define CHARACTHER_BODY 178

struct snake
{
  int x;
  int y;
  int direction;
  bool move;
  int move_step;
  struct snake* _next;
};

struct food
{
  int x;
  int y;
  int poin;
};

struct snake* _head = NULL;
struct snake* _body = NULL;

void create(int x, int y, bool move, int direction)
{
  struct snake* _ptr = (struct snake*)malloc(sizeof(struct snake));
  if(NULL == _ptr)
  {
    return; 
  }
  _ptr->x = x;
  _ptr->y = y;
  _ptr->move = move;
  _ptr->direction = direction;
  _ptr->_next = NULL;
  _head = _body = _ptr;
}

void add(int x, int y, bool move, int direction)
{
  struct snake* _ptr = (struct snake*)malloc(sizeof(struct snake));
  if(NULL == _ptr)
  {
    return;
  }
  _ptr->x = x;
  _ptr->y = y;
  _ptr->move = move;
  _ptr->move_step = 0;
  _ptr->direction = direction;
  _ptr->_next = NULL;
  _body->_next = _ptr;
  _body = _ptr;
}


int main()
{
  initscr();
  int ch;
  struct food to_eat;
  srand((unsigned)time(NULL));
  int total_poin = 0;
  to_eat.x = RANDOM_X;
  to_eat.y = RANDOM_Y;
  to_eat.poin = RANDOM_POIN;
	cbreak();
  noecho();
	keypad(stdscr, true);
  create(CENTER_X, CENTER_Y, true, DIRECTION_RIGHT);

  halfdelay(MOVING_SPEED);
  while(true)
  {
    if(_head->x == to_eat.x && _head->y == to_eat.y)
    {
      add(_head->x, _head->y, false, _head->direction);
      to_eat.x = RANDOM_X;
      to_eat.y = RANDOM_Y;
      total_poin += to_eat.poin;
      to_eat.poin = RANDOM_POIN;
    }
    clear();
    struct snake* _ptr = NULL;
    _ptr = _head;
    int count = 0;
    while(_ptr != NULL)
    {
      if(_ptr->move_step == count)
      {
        _ptr->move = true;
      }
      move(_ptr->y, _ptr->x);  
      addch(CHARACTHER_HEAD);
      _ptr = _ptr->_next;
      count++;
    }
    mvprintw(to_eat.y, to_eat.x, "%d", to_eat.poin);
    mvprintw(3, 3, "Poin : %d", total_poin);
    if((ch = getch()) != ERR)
    {
      switch(ch)
      {
        case KEY_F(EXIT_KEY):
          goto exit;
        break;
        case KEY_LEFT:
          _head->direction = DIRECTION_LEFT;
        break;
        case KEY_RIGHT:
          _head->direction = DIRECTION_RIGHT;
        break;
        case KEY_UP:
          _head->direction = DIRECTION_UP;
        break;
        case KEY_DOWN:
          _head->direction = DIRECTION_DOWN;
        break;
      }
    }
    refresh();

    _ptr = NULL;
    _ptr = _head;
    while(_ptr != NULL)
    {
      if(_ptr->move == true)
      switch(_ptr->direction)
      {
        case DIRECTION_LEFT:
          _ptr->x-=MOVING_SCALE;
        break;
        case DIRECTION_RIGHT:
          _ptr->x+=MOVING_SCALE;
        break;
        case DIRECTION_UP:
          _ptr->y-=MOVING_SCALE;
        break;
        case DIRECTION_DOWN:
          _ptr->y+=MOVING_SCALE;
        break;
      }
      _ptr = _ptr->_next;
    }

    _ptr = NULL;
    _ptr = _head;
    while(_ptr != NULL)
    {
      if(_ptr->y == LINES && _ptr->direction == DIRECTION_DOWN)
      {
        _ptr->y = 0;
      }
      if(_ptr->y == -1 && _ptr->direction == DIRECTION_UP)
      {
        _ptr->y = LINES-1;
      }
      if(_ptr->x == COLS && _ptr->direction == DIRECTION_RIGHT)
      {
        _ptr->x = 0;
      }
      if(_ptr->x == -1 && _ptr->direction == DIRECTION_LEFT)
      {
        _ptr->x = COLS-1;
      }
      _ptr = _ptr->_next;
    }

    _ptr = NULL;
    _ptr = _head;
    int temp_direction1, temp_direction2;
    temp_direction1 = _head->direction;
    _ptr = _ptr->_next;
    while(_ptr != NULL)
    {
      temp_direction2 = DIRECTION_LEFT;
      temp_direction2 = _ptr->direction;
      _ptr->direction = temp_direction1;
      temp_direction1 = temp_direction2;
      _ptr->move_step++;
      _ptr = _ptr->_next;
    }


  }
  exit:
	endwin();
	return 0;
}