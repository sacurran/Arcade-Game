/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>
 #include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

enum Direction {UP,RIGHT,DOWN,LEFT};
const int mazeSize=10;
const float cellSize=64.0;

class Game_Object {
public:
  Game_Object(const Point2f &position_,
              const Vector2f &size_,
              const float &theta_ = 0.0f,
              const float &speed_ = 0.0f)
  : m_position(position_),
    m_size(size_),
    m_theta(theta_),
    m_speed(speed_)
  {
  }
  const float get_radius() const {
    return 0.5f * m_size.magnitude();
  }
  Point2f getPosition()
  {
	  return m_position;
  }
  void setPosition(Point2f p)
  {
	  m_position=p;
  }
  bool collide(const Game_Object &rhs) const {
    const Vector2f dist_vec = m_position - rhs.m_position +
                              0.5f * (m_size - rhs.m_size);
    const float dist2 = dist_vec * dist_vec;
	float radius1=get_radius() ;
	float radius2=rhs.get_radius();
    const float radius_sum = radius1+radius2;
	return dist2 < radius_sum * radius_sum;
  }
  bool collideRelaxed(Game_Object &rhs) const{
	bool cond1=m_position.x <= rhs.m_position.x +rhs.m_size.x;
	bool cond2=m_position.x+m_size.x >= rhs.m_position.x ;
	bool cond3=m_position.y <= rhs.m_position.y +rhs.m_size.y;
	bool cond4=m_position.y + m_size.y  >= rhs.m_position.y;
	if (cond1 && cond2 && cond3 && cond4) 
		 return true;
	  return false;
  
  }
  // If you might delete base class pointers, you need a virtual destructor.
  virtual void render() const = 0; // pure virtual function call
  virtual ~Game_Object() {}
 protected:
  void render(const String &texture, const Color &filter = Color()) const {
    // Use a helper defined in Zeni/EZ2D.h
    render_image(
      texture, // which texture to use
      m_position, // upper-left corner
      m_position + m_size, // lower-right corner
      m_theta, // rotation in radians
      1.0f, // scaling factor
      m_position + 0.5f * m_size, // point to rotate & scale about
      false, // whether or not to horizontally flip the texture
      filter); // what Color to "paint" the texture
  }
protected:
  Point2f m_position; // Upper left corner
  Vector2f m_size; // (width, height)
private:
  float m_theta;
  float m_speed;
};


class Room: public Game_Object {
public:
	Room(const Point2f & position_,
		const Vector2f &size_)
		: Game_Object(position_,size_)
	{
	}
  void render() const {
    Game_Object::render("room");
  }
};

class Wall: public Game_Object{
	public:
	Wall(const Point2f & position_,
		const Vector2f &size_)
		: Game_Object(position_,size_)
	{
	}
  void render() const {
    Game_Object::render("wall");
  }

};

class Player: public Game_Object{
public:
	Player(const Point2f &position_,
       const Vector2f &size_)
  : Game_Object(position_, size_)
  {
  }
   void move_left(const float &move_) {
	 m_position.x-=move_;
  }
  void move_right(const float &move_) {
	 m_position.x+=move_;
  }
   void move_down(const float &move_) {
	 m_position.y+=move_;
  }
  void move_up(const float &move_) {
	 m_position.y-=move_;
  }

};
class Maze
{
public:
	Maze()
	{
		srand (time(NULL));
		width=mazeSize*3/2;
		height=mazeSize;
        walls= new Wall* **[width];
		for(int i = 0; i < width; i++)
        {
            walls[i]=new Wall* *[height];
            for(int j=0;j<height;j++)
            {
                walls[i][j]=new Wall* [4];
                for(int k=0;k<4;k++)
                {
                    walls[i][j][k]= NULL;
                }
            }
        }
	}
	Point2f generate_new_maze()
	{
        //generate border
        for(int i=0;i<width;i++)
        {
            carve(i, 0, UP);
            carve(i, height-1,DOWN);
        }
        for(int i=0;i<height;i++)
        {
            carve(0, i, LEFT);
            carve(width-1, i, RIGHT);
        }
        
        
		vector<bool> visit(width*height,false);
		stack<Point2f> stack1;
		Point2f cell=Point2f(0.0,0.0);
		stack1.push(cell);
		int* neighbors=new int[4];
		neighbors[0]=0;
		neighbors[1]=0;
		neighbors[2]=0;
		neighbors[3]=0;
		Point2f setRoom= Point2f(9,9);
		do
		{
            visit[cell.y*width + cell.x]=true;
			int freeNeighborCount=0;
			for(int i=0;i<4;i++)
			{
                switch (i) {
                    case UP:
                        if (cell.y > 0 && !visit[(cell.y - 1)*width + cell.x]) {
                            neighbors[freeNeighborCount++] = i;
                        }
                        break;
                    case RIGHT:
                        if (cell.x < width - 1 && !visit[cell.y*width + (cell.x + 1)]) {;
                            neighbors[freeNeighborCount++] = i;
                        }
                        break;
                    case DOWN:
                        if (cell.y < height - 1 && !visit[(cell.y + 1)*width + cell.x]) {
                            neighbors[freeNeighborCount++] = i;
                        }
                        break;
                    case LEFT:
                        if (cell.x > 0 && !visit[cell.y*width + (cell.x - 1)]) {
                            neighbors[freeNeighborCount++] = i;
                        }
                        break;
                }
			}
			if (freeNeighborCount > 0) {
				if(cell.x>=0 && cell.y>=0)
                    stack1.push(cell);
				cell =Point2f(cell.x, cell.y);

                switch (neighbors[random(freeNeighborCount)]) {
                    case UP:
                        carve(cell.x, cell.y, DOWN);
                        cell.y--;
                        break;
                    case RIGHT:
                        carve(cell.x, cell.y, LEFT);
                        cell.x++;
                        break;
                    case DOWN:
                        carve(cell.x, cell.y, UP);
                        cell.y++;
                        break;
                    case LEFT:
                        carve(cell.x, cell.y, RIGHT);
                        cell.x--;
                        break;
				}
			}
			else
			{
				cell=stack1.top();
				stack1.pop();
			}
		}while(!stack1.empty());
       if(!complete_maze(Point2f(0.0,0.0), setRoom))
        {
            clear_maze();
            generate_new_maze();
        }
        return setRoom;
    }
	void render()
	{
		for(int i=0;i<width; i++)
		{
			for (int j=0;j<height;j++)
			{
				for(int k=0;k<4;k++)
				{
					if(walls[i][j][k]!=NULL)
                        walls[i][j][k]->render();
				}
			}
		}
	}
	bool collideWithSurroundings(Player * rhs)
	{
		Point2f pos=rhs->getPosition();
		int x=positionToCell(pos.x);
		int y=positionToCell(pos.y);
        
        int xLow= x<1?x:x-1;
        int yLow=y<1?y:y-1;
		int xHigh=x>width-1?x:x+1;
		int yHigh=y>height-1?y:y+1;
		for(int i=xLow;i<xHigh; i++)
		{
			for (int j=yLow;j<yHigh;j++)
			{
				for(int k=0;k<4;k++)
				{
					if(walls[i][j][k]!=NULL && walls[i][j][k]->collideRelaxed(*rhs))
                    {
                        return true;
                    }
                }
			}
		}
		return false;
	}
private:
    bool complete_maze(Point2f start, Point2f end)
    {
        Point2f current=start;
        deque<Point2f> path;
        Wall* check=NULL;
        Wall* check2=NULL;
        vector<bool> visit(width*height,false);
        do
        {
            if(visit[current.y*width + current.x])
            {
                if(path.empty())
                    return false;
                current=path.front();
                path.pop_front();
                continue;
            }
            visit[current.y*width + current.x]=true;
            if(current.x==end.x && current.y==end.y)
            {
                return true;
            }
            if(current.y>0)
            {
                check=walls[(int)current.x][(int)current.y-1][DOWN];
                check2=walls[(int)current.x][(int)current.y][UP];
                if(check==NULL && check2==NULL)
                {
                    path.push_back(Point2f(current.x,current.y-1));
                }
            }
            if(current.x<width-1)
            {
                check=walls[(int)current.x+1][(int)current.y][LEFT];
                check2=walls[(int)current.x][(int)current.y][RIGHT];
                if(check==NULL && check2==NULL)
                {
                    path.push_back(Point2f(current.x+1,current.y));
                }
            }
            if(current.y<height-1)
            {
                check=walls[(int)current.x][(int)current.y+1][UP];
                check2=walls[(int)current.x][(int)current.y][DOWN];
                if(check==NULL && check2==NULL)
                {
                    path.push_back(Point2f(current.x,current.y+1));
                }
            }
            if(current.x>0)
            {
                check=walls[(int)current.x-1][(int)current.y][LEFT];
                check2=walls[(int)current.x][(int)current.y][RIGHT];
                if(check==NULL && check2==NULL)
                {
                    path.push_back(Point2f(current.x-1,current.y));
                }
            }
            if(path.empty())
                return false;
            current=path.front();
            path.pop_front();
        }while (path.size()<150);
        return false;
    }
	void clear_maze()
	{
		for(int i=0;i<width; i++)
		{
			for (int j=0;j<height;j++)
			{
				for(int k=0;k<4;k++)
				{
                    if(walls[i][j][k]!=NULL)
                    {
                        Wall * victim= walls[i][j][k];
                        walls[i][j][k]=NULL;
                        delete victim;
                    }
                }
			}
		}
	}
	int random(int max)
	{
		return rand() % max;
	}
	void carve(int x, int y, int dir)
	{
		Point2f base=Point2f(x*cellSize,y*cellSize);
		Point2f wallPoint;
		Vector2f wallVec;
		switch (dir)
		{
			case LEFT:
				wallVec=Vector2f(3,64);
				wallPoint=base;
				break;
			case RIGHT:
				wallVec=Vector2f(3,64);
				wallPoint=Point2f(base.x+cellSize,base.y);
				break;
			case UP:
				wallVec=Vector2f(64,3);
				wallPoint=base;
				break;
			case DOWN:
				wallVec=Vector2f(64,3);
				wallPoint=Point2f(base.x,base.y+cellSize);
				break;
		}
			Wall * add=new Wall(wallPoint,wallVec);
			walls[x][y][dir]=add;
	}
	int positionToCell(float dim)
	{
		float div=dim/64.0;
		if(div/((int)div))
			return (int) ceil(div);
		else 
			return (int) floor(div);
	}
Wall* *** walls;
int width;
int height;
};


class Couple : public Player {
public:
  Couple(const Point2f &position_,
       const Vector2f &size_)
  : Player(position_, size_)
  {
	  in_room=false;
  }
  bool did_collide(Room * r)
  {
	  if(collideRelaxed(*r))
		  in_room=true;
	  return in_room;
  }
  void render() const {
	  if(!in_room)
		Game_Object::render("couple");
  }
private:
	 bool in_room;
};

class Aaron: public Player{
public:
    Aaron(const Point2f &position_,
          const Vector2f &size_, Point2f endIn)
    : visit(mazeSize*mazeSize*3/2,0),
    Player(position_, size_)
    {
        end=Point2f(endIn.x,endIn.y);
        path.push_back(position_);
        travelDir=DOWN;
    }
    void render() const{
        Game_Object::render("wall");
    }
    bool seen(Player * player)
    {
        int playerCellx=player->getPosition().x/cellSize;
        int playerCelly=player->getPosition().y/cellSize;
        
        if( playerCellx==(int)getPosition().x/cellSize || playerCelly==(int) getPosition().y/cellSize)
            return true;
        return false;

    }
    bool visted()
    {
        Point2f current=getPosition();
        int x=current.x/cellSize;
        int y= current.y/cellSize;
        if(visit[y*mazeSize*3/2 + x]>1)
            return true;
        visit[y*mazeSize*3/2 + x]++;
        return false;
    }
    
    void turnBody()
    {
        switch (travelDir) {
            case UP:
                travelDir=DOWN;
                break;
            case DOWN:
                travelDir=UP;
                break;
            case LEFT:
                travelDir=RIGHT;
                break;
            case RIGHT:
                travelDir=LEFT;
                break;
            default:
                break;
        }
        if(visted())
            travelDir=(Direction) ((int)rand()%4);
    }
    Direction headDir;
    Direction travelDir;
private:
    Point2f end;
    deque<Point2f> path;
    vector<int> visit;
};



class Play_State : public Gamestate_Base {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State()
    : m_couple(Point2f(4.0f, 4.0f), Vector2f(50.0f, 50.0f)),
		m_room(Point2f(0,0),Vector2f(64,64)),
       m_aaron(Point2f(4*cellSize+4,4*cellSize+4),Vector2f(50,50), Point2f(9*cellSize,9*cellSize)),
	 m_time_passed(0.0f)
  {
    set_pausable(true);
	Point2f randy=m_maze.generate_new_maze();
	m_room.setPosition(Point2f(randy.x*cellSize,randy.y*cellSize));
      d=UP;
      showAaron=false;
  }


private:
  void on_push() {
    //get_Window().mouse_grab(true);
    get_Window().mouse_hide(true);
	 m_chrono.start();
    //get_Game().joy_mouse.enabled = false;
  }

  void on_pop() {
    //get_Window().mouse_grab(false);
    get_Window().mouse_hide(false);
	m_chrono.stop();
    //get_Game().joy_mouse.enabled = true;
  }
  void on_key(const SDL_KeyboardEvent &event) {
      if(event.type==SDL_KEYDOWN)
      {
    switch(event.keysym.sym) {
        case SDLK_UP:
            d=UP;
            break;
        case SDLK_LEFT:
            d=LEFT;
            break;
      case SDLK_DOWN:
            d=DOWN;
            break;
      case SDLK_RIGHT:
            d=RIGHT;
            break;
        
      default:
          Gamestate_Base::on_key(event);
    }}
      else{
          Gamestate_Base::on_key(event); // Let Gamestate_Base handle it
      }
  }
  void render() {
    get_Video().set_2d();
 
    m_couple.render();
	m_room.render();
	m_maze.render();
      if(showAaron)
      m_aaron.render();
  }
  private:
  Chronometer<Time> m_chrono;
  float m_time_passed;
 
  void perform_logic() {
    const float time_passed = m_chrono.seconds();
    m_time_passed = time_passed;
    if(time_passed>5){
        showAaron=true;
        Point2f origa=m_aaron.getPosition();
        switch(m_aaron.travelDir)
        {
            case LEFT:
                m_aaron.move_left(1.0f);
                break;
            case RIGHT:
                m_aaron.move_right(1.0f);
                break;
            case UP:
                m_aaron.move_up(1.0f);
                break;
            case DOWN:
                m_aaron.move_down(1.0f);
                break;
        }
        if(m_maze.collideWithSurroundings(&m_aaron) )
        {
            m_aaron.setPosition(origa);
            m_aaron.turnBody();
        }
    }
      
	Point2f orig=m_couple.getPosition();
	// without the '100.0f', it would move at ~1px/s
      switch(d)
      {
          case LEFT:
              m_couple.move_left(1.0f);
              break;
          case RIGHT:
              m_couple.move_right(1.0f);
              break;
          case UP:
             m_couple.move_up(1.0f);
              break;
          case DOWN:
              m_couple.move_down(1.0f);
              break;
      }
	if(m_maze.collideWithSurroundings(&m_couple))
		m_couple.setPosition(orig);
    if(m_couple.did_collide(&m_room) || m_aaron.seen(&m_couple))
        get_Game().pop_state();
    
  }
  Couple m_couple;
  Room m_room;
    Aaron m_aaron;
  Maze m_maze;
    
 Direction d;
    bool showAaron;
};

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
                   "Use Arrows to Move\n Try to get\n the couple to\n the ball.",
                   Point2f(400.0f,200.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

class Bootstrap {
  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("Cheaters Try to Win");

      get_Joysticks();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();
      get_Game().joy_mouse.enabled = true;

      return new Title_State<Play_State, Instructions_State>("Cheaters Try to Win\nby:Stephanie Curran");
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
