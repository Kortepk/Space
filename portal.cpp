#include <SFML/Graphics.hpp>  
#include <SFML/Audio.hpp>
#include <iostream>
#include <cmath>
#include <Windows.h>
#include "set.h"  //Подкючаем файл с настройками

#define ANTI_ALIASING 8
#define NUM_STARS 5000
#define MATH_PI 3.1415926535
#define SCALE_POS 35

using namespace sf;  
using namespace std;

RenderWindow window(VideoMode(WIDTH, HEIGHT), "Space X", Style::None, ContextSettings(0,0,ANTI_ALIASING));

vector < vector <int> > Vector;

vector <String> files; // список файлов

int CENTER[2] = {WIDTH/2, HEIGHT/2};


vector <Color> COLOR_LIB = {Color::Red, 
						   Color::Green,
						   Color::Blue,
						   Color(255, 128, 0),
						   Color(255, 0, 255),
						   Color(0, 255, 255)
						   };
						   
vector <Color> COLOR_LIB2 = {Color(0,255,255),
							Color(0,255,128),
							Color(175,219,255),
							Color(255,170,253),
							Color(100,100,255),
							Color::White,
							Color::Cyan
							};


class Star{
	public:
		float pos3d[3]; // x=0 y=1 z=2
		float screen_pos[2]; //x=0 y=1
		float vel;
		Color color;
		float size = 2;
		
		void update();
		void draw();
		void get_pos3d();
		void Rotar(float ang);
		Star();
};

Star::Star(){ // get_pos3d()
	Star::get_pos3d();
	
	float a = rand();
	vel = (a/RAND_MAX) * 0.09 + 0.01;
	//color = COLOR_LIB[rand() % COLOR_LIB.size()];
	color = COLOR_LIB2[rand() % COLOR_LIB.size()];
}

void Star::get_pos3d(){
	float a = rand();
	float angle = (a/RAND_MAX) * 2 * MATH_PI;
	a = rand();
	//float radius = (rand() % (HEIGHT * (1 -  1/ SCALE_POS)) + HEIGHT / SCALE_POS) * SCALE_POS;
	float radius = 2000;
	pos3d[0] = radius * sin(angle);
	pos3d[1] = radius * cos(angle);
	pos3d[2] = Z_DISTANCE;
}

void Star::Rotar(float ang){
	double Rad = sqrt(pos3d[0]*pos3d[0] + pos3d[1]*pos3d[1]);
	
	double new_ang = 0;
	bool flag = false;
	
	if(pos3d[0] < 0 and pos3d[1] > 0)
		new_ang = acos(pos3d[0]/Rad); 
	
	if(pos3d[0] < 0 and pos3d[1] < 0){
		new_ang = abs(asin(pos3d[1]/Rad)) + MATH_PI;
	}
	
	if(pos3d[0] > 0 and pos3d[1] > 0)
		new_ang = asin(pos3d[1]/Rad);

	if(pos3d[0] > 0 and pos3d[1] < 0)
		new_ang = asin(pos3d[1]/Rad);
	
		
	new_ang += ang;
	
	pos3d[1] = Rad * sin(new_ang);
	pos3d[0] = Rad * cos(new_ang);
	
}

void Star::update(){
	pos3d[2] = pos3d[2] - vel * SPEED;
	if (pos3d[2] < 1)
		Star::get_pos3d();
	
	screen_pos[0] =  pos3d[0] / pos3d[2] + CENTER[0];
	screen_pos[1] =  pos3d[1] / pos3d[2] + CENTER[1];
	
	size = Z_DISTANCE / pos3d[2]; // (Z_DISTANCE - pos3d[2]) / (0.2 * pos3d[2]);
	
	Star::Rotar(0.002 * SPEED);
	
	if(not VANISH){
		screen_pos[0] += (CENTER[0] - POS_X);
		screen_pos[1] += (CENTER[1] - POS_Y);
	}
}

void Star::draw(){
	RectangleShape rectangle(Vector2f(size, size));  
	
 	rectangle.setPosition(screen_pos[0], screen_pos[1]);
	rectangle.setFillColor(color);  
 
	window.draw(rectangle);	 
}

class Starfield{
	public:
		Star stars[NUM_STARS];
		void run();
		void Cl_swap(int L, int R);
		void QSort(int nStart, int nEnd, int deep);
};

//Starfield(); Starfield::Starfield(){}

void Starfield::Cl_swap(int L, int R){
	swap(stars[L].pos3d[2], stars[R].pos3d[2]);
	
	swap(stars[L].pos3d[0], stars[R].pos3d[0]);
	swap(stars[L].pos3d[1], stars[R].pos3d[1]);
	
	swap(stars[L].screen_pos[0], stars[R].screen_pos[0]);
	swap(stars[L].screen_pos[1], stars[R].screen_pos[1]);
	swap(stars[L].vel, stars[R].vel);
	swap(stars[L].color, stars[R].color);
	swap(stars[L].size, stars[R].size);
}

void Starfield::QSort(int nStart, int nEnd, int deep){
	int L, R, c; 
	float X;
	if(nStart >= nEnd) return;
	L = nStart; R = nEnd;
	X = stars[(L+R) / 2].pos3d[2];
	while(L <= R){
		while(stars[L].pos3d[2] > X) L++;
		while(stars[R].pos3d[2] < X) R--;
		if(L <= R){
			Cl_swap(L, R);
			L++; R--;
		}
	}
	Starfield::QSort(nStart, R, deep+1);
	Starfield::QSort(L, nEnd, deep+1);
}


void Starfield::run(){
	for(int i=0; i<NUM_STARS;i++)
		stars[i].update();
	
	Starfield::QSort(0, NUM_STARS - 1, 0);
		
	for(int i=0; i<NUM_STARS;i++)
		stars[i].draw();
}

void But_close(){
	RectangleShape rectangle(Vector2f(40, 25)); 
	rectangle.setPosition(WIDTH - 40, 0); 
	rectangle.setFillColor(Color(128, 0, 0));  
	window.draw(rectangle);	
	
	RectangleShape line(Vector2f(20.f, 1));  	
	line.setPosition(WIDTH - 25, 5);  
	line.rotate(45);
	line.setFillColor(Color(209, 164, 164)); 
	window.draw(line);
	
	line.move(14, 2);
	line.rotate(90);
	window.draw(line);
}

void Change_sc(){
	FULLSC = not FULLSC;
	if(FULLSC){
		WIDTH = VideoMode::getDesktopMode().width;
		HEIGHT = VideoMode::getDesktopMode().height; 
		window.setPosition(Vector2i(0, 0));
	}
	else{
		window.setPosition(Vector2i(WIDTH/2 - 400, HEIGHT/2 - 300));
		WIDTH = 800;
		HEIGHT = 600;   
	}     
	CENTER[0] = WIDTH/2;
	CENTER[1] = HEIGHT/2;
	
	window.setSize(Vector2u(WIDTH, HEIGHT));
	
	window.setView(
	                View(
	                   Vector2f(WIDTH / 2.0, HEIGHT / 2.0), 
	                   Vector2f(WIDTH, HEIGHT)
	                )
	            );
}

void Music_start(){
	WIN32_FIND_DATAW wfd;
    HANDLE const hFind = FindFirstFileW(L"music/*", &wfd);
     	
    if (INVALID_HANDLE_VALUE != hFind)
    {
        do
        {
            wstring ws(&wfd.cFileName[0]);
			string s(ws.begin(), ws.end());
			if(s[s.size()-4] == '.' and s[s.size()-3] == 'w' and s[s.size()-2] == 'a' and s[s.size()-1] == 'v')
			{
				cout<<s<<"\n";
				files.push_back(s);
			}
        } while (NULL != FindNextFileW(hFind, &wfd));
 
        FindClose(hFind);
    }
    cout<<"=====================\n";
}

void Otris(){
	// туман 
	if(VANISH){
		CircleShape sm(25, 60); 
	    sm.setPosition(WIDTH/2, HEIGHT/2);  
	       //window.draw(sm); 
		sm.setFillColor(Color(0,0,0,1));  //Устанавливаем цвет треугольника - синий
		for(float i = 255; i >= 0; i--){
			sm.setRadius(i/7);
			//sm.setFillColor(Color(i,0,0));
			sm.setPosition(WIDTH/2 - i/7, HEIGHT/2 - i/7);
			window.draw(sm);
		}
	}
}

int main()
{	
	window.setFramerateLimit(60);
	window.setKeyRepeatEnabled(true);
	Starfield starfield;
	
	Music music1, music2, music_off;
	Music_start();	
	
	music1.openFromFile("music/"+files[0]);
	music_off.openFromFile("music/"+files[0]);
	//music1.setPlayingOffset(sf::seconds(295.f));
	music_off.setPlayingOffset(sf::seconds(10.f));
	music1.setVolume(100.f);
	music_off.setVolume(0.f);
	
	music1.play();
	music_off.play();
	
	Clock clock_mus; 
	Time elapsed;
	bool musone = true;
	int track = 0;
	
	int vol = 100;
	
	while (window.isOpen())  
	{
		Event event;  
		while (window.pollEvent(event))  
		{
			if (event.type == Event::Closed)  
				window.close(); 
				
			if (event.type == Event::Resized)  
			{
	            WIDTH = event.size.width;
	            HEIGHT = event.size.height;        
				CENTER[0] = WIDTH/2;
				CENTER[1] = HEIGHT/2;
	            window.setView(
	                View(
	                   Vector2f(WIDTH / 2.0, HEIGHT / 2.0), 
	                   Vector2f(WIDTH, HEIGHT)
	                )
	            );
	        }
	        if (event.type == sf::Event::MouseMoved){
				POS_X = event.mouseMove.x;
				POS_Y = event.mouseMove.y; 
			}
			
			if (event.mouseButton.button == sf::Mouse::Left){
				CLICK = not CLICK;
				int x = event.mouseButton.x, y = event.mouseButton.y;
				if(y < 50 and x > WIDTH - 100 and not CLICK)
					window.close(); 
			}
			if (event.type == sf::Event::KeyPressed){
                if((FULLSC and event.key.code == Keyboard::Escape) or event.key.code == sf::Keyboard::F11)
                    Change_sc();
                if(event.key.code == sf::Keyboard::W)
                	SPEED += 0.1; 
                	//if(SPEED)
                if(event.key.code == sf::Keyboard::S){
                	SPEED -= 0.1; 
                	if(SPEED < 0)
                		SPEED = 0;
                }
                if(event.key.code == sf::Keyboard::E){
                	window.setMouseCursorVisible(VANISH);
                	VANISH = not VANISH;
                }
            }
		}
		//window.clear(Color(0, 0, 0));
		RectangleShape rectangle(Vector2f(WIDTH, HEIGHT));  
		rectangle.setFillColor(Color(25, 25, 25, 100));  
		window.draw(rectangle);	  
		
		
		starfield.run();
		Otris();
		
		if(POS_Y < 50 and POS_X > WIDTH - 100)	
			But_close();
		
		
		if(music_off.getStatus() == 0){ //конец спешашщий музыки
			if(track != files.size() - 1){
				elapsed = clock_mus.getElapsedTime();
				if(musone){
					if(elapsed.asMilliseconds() > 1000){
						track ++;
						music2.openFromFile("music/"+files[track]);
						music2.setVolume(0);
						music2.play();
					}
					
					if(elapsed.asMilliseconds() >= 100){
						clock_mus.restart();
						vol --;
						music1.setVolume(vol);
						music2.setVolume(100 - vol);
						//cout<<vol<<endl;
					}
					if(music1.getStatus() == 0){
						music2.setVolume(100);
						music1.setVolume(0);
						vol = 100;
						musone = false;
						music_off.openFromFile("music/"+files[track]);
						music_off.setVolume(0);
						music_off.setPlayingOffset(sf::seconds(20.f));
						music_off.play();
					}
				}
				else{
					if(elapsed.asMilliseconds() > 1000){
						track ++;
						music1.openFromFile("music/"+files[track]);
						music1.setVolume(0);
						music1.play();
					}
					
					if(elapsed.asMilliseconds() >= 100){
						clock_mus.restart();
						vol --;
						music2.setVolume(vol);
						music1.setVolume(100 - vol);
						//cout<<vol<<endl;
					}
					if(music2.getStatus() == 0){
						music1.setVolume(100);
						music2.setVolume(0);
						vol = 100;
						musone = true;
						music_off.openFromFile("music/"+files[track]);
						music_off.setVolume(0);
						music_off.setPlayingOffset(sf::seconds(20.f));
						music_off.play();
					}
				}
			}
		}
		
		window.display();
	}
	return 0;
}