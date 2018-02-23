/*The author of these code: Zhou Yi 167312 and Hao Yuwei 167308.*/

//The headers
#include "SDL/SDL.h"
#include "SDL/SDL_image.h"
#include "SDL/SDL_ttf.h"
#include "SDL/SDL_mixer.h"
#include <string>
#include<math.h>//we use "abs" function of math.h

int over=1;
int buildingalive=1;
int disappearplane=1;
int disappearufo=1;
int disappeartank=1;
int disappeardot=1;
int bulletalive=1;
int dotalive=1;
int disappearbomb=1;
int capturealive=1;
int win=1;
int presskey=0;
int disappearZhadan=1;
int fly=0;

//some flags using for judgement;


 Uint8 *keystates = SDL_GetKeyState( NULL );
//Screen attributes
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_BPP = 32;


//The frame rate
const int FRAMES_PER_SECOND = 20;

//The dot dimensions(dot means player's plane)
const int DOT_WIDTH = 95;
const int DOT_HEIGHT = 80;

//the bullet size(bullet means bullet sent by my plane)
const int BULLET_WIDTH=21;
const int BULLET_HEIGHT=20;

//the zhadan size(zhadan means bullet sent by enemy's plane)
const int ZHADAN_HEIGHT=50;
const int ZHADAN_WIDTH=50;

//the bomb size(bomb means bullet sent by enemy's tank)
const int BOMB_WIDTH=20;
const int BOMB_HEIGHT=20;

//The dimensions of the level
const int LEVEL_WIDTH = 1280;
const int LEVEL_HEIGHT = 600;

const int PLANE_WIDTH = 100;
const int PLANE_HEIGHT = 50;

const int UFO_WIDTH =100;
const int UFO_HEIGHT =50;

const int TANK_WIDTH = 100;
const int TANK_HEIGHT = 40;



//The surfaces
SDL_Surface *dot = NULL;
SDL_Surface *background = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *building = NULL;
SDL_Surface *home = NULL;
SDL_Surface *bullet = NULL;
SDL_Surface *tank = NULL;
SDL_Surface *plane = NULL;
SDL_Surface *ufo = NULL;
SDL_Surface *capture = NULL;
SDL_Surface *bomb = NULL;
SDL_Surface *zhadan = NULL;
SDL_Surface *message =NULL;
SDL_Surface *message1 =NULL;



//The event structure
SDL_Event event;

TTF_Font *font = NULL;
SDL_Color textColor = { 0,0,0};

//The music that will be played
Mix_Music *music = NULL;



//The camera
SDL_Rect camera = { 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT };


class Tank
{   public:
    int x, y,xm,ym;
	Tank(){
		x=750;
		y=520;
		xm=0;
		ym= 0 ;
	}
	void showtank();


};

class Plane
{  public:
	int x, y,xm,ym;
	int xVel,yVel;
	void showplane();

Plane(){
   x=100;
   y=100;
   xm=0;
   ym=0;
   xVel= 10;

}
void moveplane();
};
class Ufo
{  public:
   int x, y,xm,ym;
   int xVel,yVel;
	Ufo(){
    x = 500;
    y = 100;
	xm =0;
	ym=0;
    xVel = 10;
    yVel = 10;
}
	void showufo();
	void moveufox();
	void moveufoy();
	void moveufox1();
	void moveufoy1();
};


class Capture
{  int x, y;
public:
	void showcapture();
	void showcapture1();
};


class Home
{
    int x, y;
public:
	void showhome();
};




//the building
class Building
{ //The X and Y offsets of the building
    int x, y;
public:
	void showbuilding();
};


//define the bullet
class Bullet
{public:
int x,y,xm,ym;
int xVel,yVel;


Bullet(){
    x = 600;
    y = 100;
    xVel = 0;
    yVel = 0;
}
void bullet_handle_input();
void movebullet();
void showbullet();

};

//define the zhadan(zhadan means bullet sent by enemy's plane)
class Zhadan
{   public:
    int x, y,xm,ym;
    int yVel;


    //Initializes the variables
Zhadan(){
    x = 1200;
    y = 100;
	xm=0;
	ym=0;
    yVel =10;
}
    void movezhadan();
    void showzhadan();


};
class Bomb
{   public:
    int x, y,xm,ym;
    int yVel;


    //Initializes the variables
Bomb(){
    x = 800;
    y = 500;
	xm=0;
	ym=0;
    yVel =-10;
}
    void movebomb();
    void showbomb();


};


//The dot(the player's helicopter)
class Dot
{
    public:
    //The X and Y offsets of the helicopter
    int x, y,w,h,xm,ym;
    //The velocity of the helicopter
    int xVel, yVel;
    //Initializes the variables
    Dot();
    //Handles with key pressing and adjusts the dot's velocity
    void handle_input();
    //Moves the dot
    void move();
    //Shows the dot on the screen
    void show();
    //sets the camera over the dot
    void set_camera();
    void judge();
};



//The timer
class Timer
{
    private:
    //The clock time when the timer started
    int startTicks;

    //The ticks stored when the timer was paused
    int pausedTicks;

    //The timer status
    bool paused;
    bool started;

    public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void pause();
    void unpause();

    //Gets the timer's time
    int get_ticks();

    //Checks the status of the timer
    bool is_started();
    bool is_paused();
};

SDL_Surface *load_image( std::string filename )
{
    //The image that's loaded
    SDL_Surface* loadedImage = NULL;

    //The optimized surface that will be used
    SDL_Surface* optimizedImage = NULL;

    //Load the image
    loadedImage = IMG_Load( filename.c_str() );

    //If the image loaded
    if( loadedImage != NULL )
    {
        //Create an optimized surface
        optimizedImage = SDL_DisplayFormat( loadedImage );

        //Free the old surface
        SDL_FreeSurface( loadedImage );

        //If the surface was optimized
        if( optimizedImage != NULL )
        {
            //Color key surface
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 255, 255, 255 ) );
        }
    }

    //Return the optimized surface
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip = NULL )
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    //Initialize all SDL subsystems
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    //Set up the screen
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    //If there was an error in setting up the screen
    if( screen == NULL )
    {
        return false;
    }

    if( TTF_Init()== -1)
    {
        return false;
    }

     if( Mix_OpenAudio( 22050, MIX_DEFAULT_FORMAT, 2, 4096 ) == -1 )
    {
        return false;
    }

    //Set the window caption
    SDL_WM_SetCaption( "helicopter", NULL );

    //If everything initialized fine
    return true;
}

bool load_files()
{
    bullet=load_image("bullet.png");

    //Load the images
    dot = load_image( "helicopter.png" );
    background = load_image( "bg.jpg" );
	building = load_image("building.png");
    home = load_image("home.png");
    tank = load_image("tank.png");
    plane = load_image("plane.png");
	ufo = load_image("UFO.png");
	capture = load_image("capture.png");
	bomb = load_image("bomb.png");
	zhadan = load_image("zhadan.jpg");
	font = TTF_OpenFont("lazy.ttf",90);





	  if( building == NULL||tank == NULL || plane==NULL||ufo==NULL||capture==NULL||bullet==NULL||home == NULL||dot == NULL||background == NULL||font==NULL)
    {
        return false;
    }

     music = Mix_LoadMUS( "music.mp3" );

     if( music == NULL )
    {
        return false;
    }



    //If everything loaded fine
    return true;
}




void clean_up()
{
    //Free the surfaces
    SDL_FreeSurface( dot );
    SDL_FreeSurface( background );
	SDL_FreeSurface(building);
	SDL_FreeSurface( message );
	SDL_FreeSurface( message1 );
	SDL_FreeSurface( home );
	SDL_FreeSurface( tank );
	SDL_FreeSurface( plane );
	SDL_FreeSurface( screen );
	SDL_FreeSurface( bullet );
	SDL_FreeSurface( ufo );
	SDL_FreeSurface( capture);
	SDL_FreeSurface( bomb );
	SDL_FreeSurface( zhadan );

	 Mix_FreeMusic( music );

	TTF_CloseFont( font );
	Mix_CloseAudio();

	TTF_Quit();


    //Quit SDL
    SDL_Quit();
}



Dot::Dot()
{
    //Initialize the offsets
    x = 800;
    y = 0;
    xm=0;
	ym=0;
    //Initialize the velocity
    xVel = 0;
    yVel = 0;
}

void Bullet::bullet_handle_input()
{ //If a key was pressed
    if( event.type == SDL_KEYDOWN)
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {

		   case SDLK_i: yVel = -BULLET_HEIGHT / 2;xVel=0;presskey=1; break;
            case SDLK_k: yVel = BULLET_HEIGHT / 2;xVel=0; presskey=1;break;
            case SDLK_j: xVel = -BULLET_WIDTH / 2;yVel=0; presskey=1;break;
            case SDLK_l: xVel = BULLET_WIDTH / 2;yVel=0; presskey=1;break;

        }

    }




}




void Bullet::movebullet()
{
    //Move the bullet left or right
    x += xVel;

    //If the bullet go too far to the leftward or rightward
    if( ( x < 0 ) || ( x + BULLET_WIDTH-camera.x > SCREEN_WIDTH ) )
    {
        //move back
        x -= xVel;
    }

    //Move the bullet up or down
    y += yVel;

    //If the bullet go too far upward or downward
    if( ( y < 0 ) || ( y + BULLET_HEIGHT-camera.y > SCREEN_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }

    if(x<280&&y>396&&buildingalive==1)
    {buildingalive=0;
    bulletalive=1;}


}
//UFO can move up,down,right and left
void Ufo::moveufox()
{
    x +=xVel;
    if((x-camera.x<0)||(x+UFO_WIDTH-camera.x>SCREEN_WIDTH))
    {
        x -=xVel;
    }
}

void Ufo::moveufox1()
{
    x +=-xVel;
    if((x-camera.x<0)||(x+UFO_WIDTH-camera.x>SCREEN_WIDTH))
    {
        x -=xVel;
    }
}




void Ufo::moveufoy()
{
    y +=yVel;
    if((y-camera.y<0)||(y+UFO_HEIGHT-camera.y>SCREEN_WIDTH))
    {
        y -=yVel;
    }

}

void Ufo::moveufoy1()
{
    y +=-yVel;
    if((y-camera.y<0)||(y+UFO_HEIGHT-camera.y>SCREEN_WIDTH))
    {
        y -=yVel;
    }

}


void Plane::moveplane()
{
   x += xVel;

   if((x-camera.x<0)||(x + PLANE_WIDTH-camera.x>SCREEN_WIDTH))
   {
       x -=xVel;
   }


}

void Zhadan::movezhadan()
{
	//Move the zhadan up or down
    y += yVel;


    //If the zhadan go too far upward or downward
    if( ( y-camera.y < 0 ) || ( y + ZHADAN_HEIGHT-camera.y > SCREEN_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }


}
void Bomb::movebomb()
{


    //Move the bomb up or down
    y += yVel;




}

void Plane::showplane()
{ apply_surface( x-camera.x, y-camera.y, plane, screen);
}
void Capture::showcapture()
{ apply_surface( 50-camera.x,500-camera.y, capture, screen );
}
void Capture::showcapture1()
{
    apply_surface(1100-camera.x,500-camera.y,capture,screen);
}
void Ufo::showufo()
{ apply_surface( x-camera.x, y-camera.y, ufo, screen );
}

void Home::showhome()
{ apply_surface( 1040-camera.x, 400-camera.y, home, screen );
}


void Bomb::showbomb()
{
    apply_surface(x-camera.x, y-camera.y, bomb, screen);
}

void Zhadan::showzhadan()
{
    apply_surface(x-camera.x, y-camera.y, zhadan, screen);
}


void Bullet::showbullet()
{

    apply_surface(x-camera.x,  y-camera.y, bullet, screen);
}
void Tank::showtank()
{apply_surface( 750-camera.x, 520-camera.y, tank, screen);}

void Building::showbuilding()
{
    //Show the building
    apply_surface( 0-camera.x, 396-camera.y, building, screen);
}


void Dot::handle_input()
{
    //If a key was pressed
    if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel -= DOT_HEIGHT / 2; break;
            case SDLK_DOWN: yVel += DOT_HEIGHT / 2; break;
            case SDLK_LEFT: xVel -= DOT_WIDTH / 2; break;
            case SDLK_RIGHT: xVel += DOT_WIDTH / 2; break;
        }
    }
    //If a key was released
    else if( event.type == SDL_KEYUP )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_UP: yVel += DOT_HEIGHT / 2; break;
            case SDLK_DOWN: yVel -= DOT_HEIGHT / 2; break;
            case SDLK_LEFT: xVel += DOT_WIDTH / 2; break;
            case SDLK_RIGHT: xVel -= DOT_WIDTH / 2; break;
        }
    }
}

void Dot::move()
{
    //Move the dot left or right
    x += xVel;

    //If the dot go too far to the left or right
    if( ( x < 0 ) || ( x + DOT_WIDTH > LEVEL_WIDTH ) )
    {
        //move back
        x -= xVel;
    }

    //Move the dot up or down
    y += yVel;

    //If the dot went too far up or down
    if( ( y < 0 ) || ( y + DOT_HEIGHT > LEVEL_HEIGHT ) )
    {
        //move back
        y -= yVel;
    }
}

void Dot::show()
{
    //Show the dot
    apply_surface( x - camera.x, y - camera.y, dot, screen );
}



void Dot::set_camera()
{
    //Center the camera over the dot
    camera.x = ( x + DOT_WIDTH / 2 ) - SCREEN_WIDTH / 2;
    camera.y = ( y + DOT_HEIGHT / 2 ) - SCREEN_HEIGHT / 2;

    //Keep the camera in boundary.
    if( camera.x < 0 )
    {
        camera.x = 0;
    }
    if( camera.y < 0 )
    {
        camera.y = 0;
    }
    if( camera.x > LEVEL_WIDTH - camera.w )
    {
        camera.x = LEVEL_WIDTH - camera.w;
    }
    if( camera.y > LEVEL_HEIGHT - camera.h )
    {
        camera.y = LEVEL_HEIGHT - camera.h;
    }
}



Timer::Timer()
{
    //Initialize the variables
    startTicks = 0;
    pausedTicks = 0;
    paused = false;
    started = false;
}

void Timer::start()
{
    //Start the timer
    started = true;

    //Unpause the timer
    paused = false;

    //Get the current clock time
    startTicks = SDL_GetTicks();
}

void Timer::stop()
{
    //Stop the timer
    started = false;

    //Unpause the timer
    paused = false;
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( ( started == true ) && ( paused == false ) )
    {
        //Pause the timer
        paused = true;

        //Calculate the paused ticks
        pausedTicks = SDL_GetTicks() - startTicks;
    }
}

void Timer::unpause()
{
    //If the timer is paused
    if( paused == true )
    {
        //Unpause the timer
        paused = false;

        //Reset the starting ticks
        startTicks = SDL_GetTicks() - pausedTicks;

        //Reset the paused ticks
        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    //If the timer is running
    if( started == true )
    {
        //If the timer is paused
        if( paused == true )
        {
            //Return the number of ticks when the timer was paused
            return pausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            return SDL_GetTicks() - startTicks;
        }
    }

    //If the timer isn't running
    return 0;
}



bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

void Dot::judge()
{
  if(x<280&&y>396&&buildingalive==1)
  {over=0;
  dotalive=0;}



}




int main( int argc, char* args[] )
{


    //Quit flag
    bool quit = false;

    //the left column means "class", the right column is alia for each class
    Dot myDot;

	Building myBuilding;

	Bullet myBullet;

	Home myHome;

	Tank yourTank;

	Plane yourPlane;

	Ufo yourUfo;

	Capture yourCapture;

	Bomb yourBomb;

	Zhadan yourZhadan;




    //The frame rate regulator
    Timer fps;

    //Initialize
    if( init() == false )
    {
        return 1;
    }

    //Load the files
    if( load_files() == false )
    {
        return 1;
    }
//when you lose the game, the screen will display "GAME OVER"
    message = TTF_RenderText_Solid( font,"GAME OVER",textColor);
  if( message == NULL )
  {
    return 1;
  }
//when you win the game,the screen will display "YOU WIN"
    message1 = TTF_RenderText_Solid( font,"YOU WIN",textColor);
  if( message1 == NULL )
  {
    return 1;
  }

    //While the user hasn't quitted
    while( quit == false )
    {
        //Start the frame timer
        fps.start();

        //While there's events to handle
        while( SDL_PollEvent( &event ) )
        {
            //Handle events for the dot
            myDot.handle_input();


                  if( Mix_PlayingMusic() == 0 )
                    {
                        //²¥·ÅÒôÀÖ
                        if( Mix_PlayMusic( music, -1 ) == -1 )
                        {
                            return 1;
                        }
                    }

            //If the user has quitted out the window
            if( event.type == SDL_QUIT )
            {
                //Quit the program
                quit = true;
            }
        }
		myBullet.movebullet();
//make the bullet's coordinate same to our helicopter everytime when the player throw the bullet
if(bulletalive==1||presskey==1)
{


		myBullet.x=myDot.x;
		myBullet.y=myDot.y;
		presskey=0;
}



		myBullet.bullet_handle_input();


        //Move the helicopter
        myDot.move();
//move the enemy's plane
        yourPlane.moveplane();

//when the helicopter flies in low sky, enemy's tank will throw the bomb upwards to explode the helicopter
        if(myDot.y>400)
        {
		fly=1;
		disappearbomb=0;

        }
        if(fly==1)
        {


        yourBomb.movebomb();
        }

        //Set the camera
        myDot.set_camera();

        //make the enemy's UFO can fly following the helicopter

        if(yourUfo.x-myDot.x<0)
        {
        yourUfo.moveufox();
        }

         if(yourUfo.x-myDot.x>0)
        {
        yourUfo.moveufox1();
        }


        if(yourUfo.y-myDot.y<0)
        {
            yourUfo.moveufoy();
        }

          if(yourUfo.y-myDot.y>0)
        {
            yourUfo.moveufoy1();
        }

        //Show the background
        apply_surface( 0, 0, background, screen, &camera );
        //when content the judging condition ,display you win or game over on the screen
        if(over==0&&win==1)
        {
             apply_surface(150,200,message,screen);
        }

        if(win==0&&over==1)
        {
            apply_surface(150,200,message1,screen);
               yourCapture.showcapture1();
        }




//show our home
		myHome.showhome();
		myDot.judge();

//control display the picture of capture
	if(capturealive==1)
		{
		    yourCapture.showcapture();
		}



//control the picture of building display or disappear
        if(buildingalive==1)
		{myBuilding.showbuilding();
		}
//when the helicopter rescue the capture
		if(buildingalive==0&&myDot.xm<300&&myDot.ym>500)
		{
              capturealive=0;
		}
//judge when the the zhadan hit the player's helicopter
		yourZhadan.xm=yourZhadan.x+ZHADAN_WIDTH/2;
		yourZhadan.ym=yourZhadan.y+ZHADAN_HEIGHT/2;
		if(abs(myDot.xm-yourZhadan.xm)<(ZHADAN_WIDTH+DOT_WIDTH)/2&&abs(myDot.ym-yourZhadan.ym)<(ZHADAN_HEIGHT+DOT_HEIGHT)/2&&dotalive==1&&disappearZhadan==0)
		{
		    over=0;
		    dotalive=0;
		}







        //the following four lines check if the dot and plane have a collision
		yourPlane.xm=yourPlane.x+PLANE_WIDTH/2;
	    yourPlane.ym=yourPlane.y+PLANE_HEIGHT/2;
        myDot.xm=myDot.x+DOT_WIDTH/2;
        myDot.ym=myDot.y+DOT_HEIGHT/2;
	    //printf("myDot.xm=%d,myDot.ym=%d\n",myDot.xm,myDot.ym); this sentence using for output coordinate so we can know where is wrong.

	    //judge if the player's helicopter rushes into enemy' plane
        if(abs(myDot.xm-yourPlane.xm)<(PLANE_WIDTH+DOT_WIDTH)/2&&abs(myDot.ym-yourPlane.ym)<(PLANE_HEIGHT+DOT_HEIGHT)/2&&dotalive==1&&disappearplane==1)
		{over=0;
		dotalive=0;
		disappearplane=0;}

		//judge if tank's bomb hits the player's helicopter
        yourBomb.xm=yourBomb.x+BOMB_WIDTH/2;
		yourBomb.ym=yourBomb.y+BOMB_HEIGHT/2;
		if(abs(myDot.xm-yourBomb.xm)<(DOT_WIDTH+BOMB_WIDTH)/2&&abs(myDot.ym-yourBomb.ym)<(DOT_HEIGHT+BOMB_HEIGHT)/2&&dotalive==1)
		{over=0;
		dotalive=0;
		disappearbomb=0;}

       //when the player's helicopter rush into something, it will disappear
	    if(over==0)
		{disappeardot=0;}
			if(dotalive==1)
		{myDot.show();}


		//check the collsion of bullet and plane
        myBullet.xm=myBullet.x+BULLET_WIDTH/2;
		myBullet.ym=myBullet.y+BULLET_HEIGHT/2;
        //printf("myBullet.xm=%d,myBullet.ym=%d,yourPlane.xm=%d,yourPlane.ym=%d\n",myBullet.xm,myBullet.ym,yourPlane.xm,yourPlane.ym);
		if(abs(myBullet.xm-yourPlane.xm)<(PLANE_WIDTH+BULLET_WIDTH)/2&&abs(myBullet.ym-yourPlane.ym)<(PLANE_HEIGHT+BULLET_HEIGHT)/2&&bulletalive==0)
		{disappearplane=0;
		bulletalive=1;}
       if(disappearplane==1)
        {yourPlane.showplane();}

       //check the collsion of bullet and ufo
	   yourUfo.xm=yourUfo.x+UFO_WIDTH/2;
	   yourUfo.ym=yourUfo.y+UFO_HEIGHT/2;
       //printf("myBullet.xm=%d,myBullet.ym=%d,yourUfo.xm=%d,yourUfo.ym=%d\n",myBullet.xm,myBullet.ym,yourUfo.xm,yourUfo.ym);
	   if(abs(myBullet.xm-yourUfo.xm)<(BULLET_WIDTH+UFO_WIDTH)/2&&abs(myBullet.ym-yourUfo.ym)<(BULLET_HEIGHT+UFO_HEIGHT)/2&&bulletalive==0)
       {disappearufo=0;
       bulletalive=1;}
       //judge when the player's helicopter rushes into enemy's UFO
       if(abs(myDot.xm-yourUfo.xm)<(DOT_WIDTH+UFO_WIDTH)/2&&abs(myDot.ym-yourUfo.ym)<(DOT_HEIGHT+UFO_HEIGHT)/2&&dotalive==1&&disappearufo==1)
       {disappearufo=0;
       dotalive=0;
       over=0;
       }

       //judge whether display the picture of UFO or not
	   if(disappearufo==1)
	   {yourUfo.showufo();}


      //when press key the bullet will display
         if( event.type == SDL_KEYDOWN )
    {
        //Adjust the velocity
        switch( event.key.keysym.sym )
        {
            case SDLK_i: bulletalive=0;
            case SDLK_k: bulletalive=0;
            case SDLK_j: bulletalive=0;
            case SDLK_l: bulletalive=0;

    }
    }
    if(bulletalive==0)
	   {myBullet.showbullet();
	   }





       //check the collsion of tank and bullet
      yourTank.xm=yourTank.x+TANK_WIDTH;
	   yourTank.ym=yourTank.y+TANK_HEIGHT;
	   //with this sentence ,when our bullet hit enemy's tank,the tank disappear,but we don't kown why with the sentence the tank disappear in the begining.
	  // if(abs(myBullet.xm-yourTank.xm)<(BULLET_WIDTH+TANK_WIDTH)/2&&(myBullet.ym-yourUfo.ym)<(BULLET_HEIGHT+TANK_HEIGHT)/2)
	   //{disappeartank=0;}

	   //when the player's helicopter rushes into tank
	   if(abs(myDot.xm-yourTank.xm)<(DOT_WIDTH+TANK_WIDTH)/2&&abs(myDot.ym-yourTank.ym)<(TANK_HEIGHT+DOT_HEIGHT)/2)
        {
            disappeartank=0;
            over=0;
            dotalive=0;
        }
	   if(disappeartank==1)
	   {yourTank.showtank();}
       //when the bomb of enemy's plane was thrown into the player's home , the player lose the game
	   if(yourZhadan.y>500)
	   {
	       over=0;
	   }

       //when content the judgement condition ,the player win the game
	   if(capturealive==0&&myDot.xm>1040&&myDot.ym>500&&over==1)
	   {

	       win=0;

	   }



       // when the enemy's plane flies to the headover of the player's home,it will throw the bomb
       if(disappearbomb==0)
       {


		yourBomb.showbomb();
       }
		if(yourPlane.x>1100&&disappearplane==1)
		{yourZhadan.movezhadan();
		yourZhadan.showzhadan();
		disappearZhadan=0;}

        //Update the screen
        if( SDL_Flip( screen ) == -1 )
        {
            return 1;
        }

        //Cap the frame rate
        if( fps.get_ticks() < 1000 / FRAMES_PER_SECOND )
        {
            SDL_Delay( ( 1000 / FRAMES_PER_SECOND ) - fps.get_ticks() );
        }
    }

    //Clean up
    clean_up();

    return 0;
}


