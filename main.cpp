#include <SFML/Graphics.hpp>
#include <time.h>
#include <sstream>
#include <iostream>

using namespace std;
using namespace sf;

int N=30,M=20;
int size=16;

int w = size*N;
int h = size*M + 40;
int score = 0;

int control,num=4;
int dir;

bool isEndGame = false;

struct Snake
{ int x,y;}  s[100];

struct Fruit
{ int x,y;} f;

void Tick()
{
	for (int i=num;i>0;--i)
	{
		s[i].x=s[i-1].x;
		s[i].y=s[i-1].y;
	}	
	if (s[num].x != 0 && s[num].x != N-1)
	{
		if (s[num].y == s[num -1].y && s[num].x > s[num -1].x) dir = 0; // Left 2 right 
		else if (s[num].y == s[num -1].y && s[num].x < s[num - 1].x ) dir = 1; // Right 2 left 
	}
	if (s[num].y != 0 && s[num].y != M-1)
	{	
		if (s[num].x == s[num - 1].x && s[num].y > s[num - 1].y ) dir = 2; // Top 2 bottom
		else if (s[num].x == s[num - 1].x && s[num].y < s[num - 1].y ) dir = 3; // Bottom 2 Top 
	}
	if (control==0) s[0].y+=1;
	if (control==1) s[0].x-=1;        
	if (control==2) s[0].x+=1;         
	if (control==3) s[0].y-=1;

	if ((s[0].x==f.x) && (s[0].y==f.y)) 
	{
		num++; f.x=rand()%N; 
		f.y=rand()%M;
		score +=1;
	}
	// check end game
	for (int i = num ; i > 0 ; --i)
	{
		if (s[0].x == s[i].x && s[0].y == s[i].y )
			isEndGame = true; 
	}  
	// Return 
	if (s[0].x>N-1) s[0].x=0;  if (s[0].x<0) s[0].x=N;
	if (s[0].y>M-1) s[0].y=0;  if (s[0].y<0) s[0].y=M;

}

int main()
{  
	srand(time(0));

	RenderWindow window(VideoMode(w, h), "Snake Game!");

	Texture t1,t2,t3;
	Font font;
	Text text, textEnd;

	t1.loadFromFile("images/white.png");
	t2.loadFromFile("images/red.png");
	t3.loadFromFile("images/green.png");

	if (!font.loadFromFile("fonts/OpenSans-ExtraBold.ttf"));
	{
		cout << "Error" << endl;
	}
	text.setFont(font);
	textEnd.setFont(font);

	Sprite bg(t1);
	Sprite snake(t2);
	Sprite head(t3);

	Clock clock;
	float timer=0, delay=0.1;

	// Init
	f.x=10;
	f.y=10; 
	int init = 4;
	for (int i = init; i < init + num ; i ++ )
	{
		s[i].x=s[i-1].x;
		s[i].y=s[i-1].y;	
	}

	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asSeconds();
		clock.restart();
		timer+=time; 

		Event e;
		while (window.pollEvent(e))
		{
			if (e.type == Event::Closed)      
				window.close();
			if (e.type == Event::KeyPressed)

				if (e.key.code == Keyboard::Escape) 
					window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Left)) 
			if (dir !=1) control=1;   
		if (Keyboard::isKeyPressed(Keyboard::Right))
			if (dir != 0) control=2;    
		if (Keyboard::isKeyPressed(Keyboard::Up)) 
			if (dir !=3 )control=3;
		if (Keyboard::isKeyPressed(Keyboard::Down)) 
			if (dir != 2) control=0;
		if (Keyboard::isKeyPressed(Keyboard::R)) 
		{
			num = 4;
			score = 0;
			isEndGame = false;
		}
		
		if (timer>delay && !isEndGame) 
		{
			timer=0; 
			Tick();
		}

		// Draw
		window.clear(Color::White);

		for (int i=0; i<N; i++) 
			for (int j=0; j<M; j++) 
			{
				bg.setPosition(i*size, j*size);
				window.draw(bg);
			}

		for (int i=0;i<num;i++)
		{ 
			if (i == 0)
			{ 
				head.setPosition(s[i].x*size, s[i].y*size);
				window.draw(head);

			}
			else
			{
				snake.setPosition(s[i].x*size, s[i].y*size);
				window.draw(snake);
			}
		}

		// Draw score
		ostringstream out;
		out << score;

		text.setCharacterSize(15);
		text.setColor(Color::Red);
		text.setString("Score: " + out.str() + "  -  ESC: Exit  -  R: Restart");
		text.setPosition(100,330);
		window.draw(text);

		// Draw endgame
		if (isEndGame)
		{
			textEnd.setCharacterSize(50);
			textEnd.setColor(Color::Red);
			textEnd.setString("Game Over");
			textEnd.setPosition(100,120);
			window.draw(textEnd);
		}				
		snake.setPosition(f.x*size, f.y*size);
		window.draw(snake);    
		window.display();
	}

	return 0;
}

