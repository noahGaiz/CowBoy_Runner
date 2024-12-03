#include <stm32f031x6.h>
#include <stdlib.h> // this header file is required for the rand() function
#define C0  16//{all notes used for the buzzer output}
#define C4  262
#define G3  196
#define E3  165
#define C3  131
#define FS0_Gb0 23
#include "musical_notes.h"
#include "display.h"
#include "sound.h"
#include "core_cm0.h"//this library is used to restart the system
#include "serial.h"

void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void playNote(uint32_t freq);
int UpPressed(void);
int DownPressed(void);
int laneChooser();//this function chooses which cactus moves
void restart();//this function restarts the program
void timer();//prints the timer on the screen

volatile uint32_t milliseconds;//helps count the time while running
volatile uint32_t immune =0;//immunity to the user when they get hit
volatile uint32_t start_time = 0;//used for the timer


const uint16_t deco1[]=
{
    0,0,0,2569,2569,2569,2569,2569,2569,0,0,0,0,2569,2569,2569,2569,2569,2569,2569,2569,2569,2569,0,0,0,0,40375,0,40375,40375,0,40375,0,0,0,0,0,40224,40375,40375,40375,40375,40375,40375,40224,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,22355,40224,40224,40224,40224,22355,0,0,0,0,0,22355,22355,22355,22355,22355,22355,22355,22355,0,0,0,22355,22355,22355,22355,22355,22355,22355,22355,22355,22355,0,0,22355,22355,22355,22355,22355,22355,22355,22355,22355,22355,0,0,40375,40375,22355,22355,22355,22355,22355,22355,40375,40375,0,0,0,0,16142,16142,16142,16142,16142,61307,61307,0,0,0,0,0,2569,2569,2569,2569,2569,2569,61307,0,0,0,0,0,2569,2569,0,0,2569,2569,61307,0,0,0,0,0,2569,2569,0,0,2569,2569,0,0,0,0,0,2569,2569,2569,0,0,2569,2569,2569,0,0,
};
const uint16_t movdeco1[]=
{
	0,0,0,0,0,28434,28434,28434,28434,28434,0,0,0,0,0,28434,28434,28434,28434,28434,28434,28434,28434,28434,0,0,0,0,0,40375,40375,40375,0,40375,0,0,0,0,0,0,0,40375,40224,40375,40375,40375,40224,0,0,0,0,0,0,40375,40375,40224,40224,40224,0,0,0,0,0,0,0,0,40375,40224,40224,40224,0,0,0,0,0,0,0,22355,22355,40224,40224,40224,0,0,0,0,0,0,22355,22355,22355,22355,0,0,0,0,0,0,0,0,22355,22355,22355,22355,22355,22355,40375,0,0,0,0,0,22355,22355,22355,22355,22355,22355,40375,0,0,0,0,0,22355,22355,22355,22355,22355,22355,40375,0,0,0,0,0,16142,16142,16142,16142,61307,61307,0,0,0,0,0,0,0,28434,28434,28434,28434,61307,0,0,0,0,0,0,28434,28434,28434,28434,28434,0,0,0,0,0,0,28434,28434,28434,28434,28434,28434,0,0,0,0,0,0,0,28434,0,28434,28434,28434,28434,0,0,
};
const uint16_t deco2[]= 
{
    0,0,0,2569,2569,2569,2569,2569,2569,0,0,0,0,2569,2569,2569,2569,2569,2569,2569,2569,2569,2569,0,0,0,0,40375,0,40375,40375,0,40375,0,0,0,0,0,40224,40375,40375,40375,40375,40375,40375,40224,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,22355,40224,40224,40224,40224,22355,0,0,0,0,0,22355,22355,22355,22355,22355,22355,22355,22355,0,0,0,22355,22355,22355,22355,22355,22355,22355,22355,22355,22355,0,0,22355,22355,22355,22355,22355,22355,22355,22355,22355,22355,0,0,40375,40375,22355,22355,22355,22355,22355,22355,40375,40375,0,0,0,0,16142,16142,16142,16142,16142,61307,61307,0,0,0,0,0,2569,2569,2569,2569,2569,2569,61307,0,0,0,0,0,2569,2569,0,0,2569,2569,61307,0,0,0,0,0,2569,2569,0,0,2569,2569,0,0,0,0,0,2569,2569,2569,0,0,2569,2569,2569,0,0,
};
const uint16_t deco3[]= 
{
    0,0,0,2569,2569,2569,2569,2569,2569,0,0,0,0,2569,2569,2569,2569,2569,2569,2569,2569,2569,2569,0,0,0,0,40375,0,40375,40375,0,40375,0,0,0,0,0,40224,40375,40375,40375,40375,40375,40375,40224,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,40224,40224,40224,40224,40224,40224,0,0,0,0,0,0,22355,40224,40224,40224,40224,22355,0,0,0,0,0,22355,22355,22355,22355,22355,22355,22355,22355,0,0,0,22355,22355,22355,22355,22355,22355,22355,22355,22355,22355,0,0,22355,22355,22355,22355,22355,22355,22355,22355,22355,22355,0,0,40375,40375,22355,22355,22355,22355,22355,22355,40375,40375,0,0,0,0,16142,16142,16142,16142,16142,61307,61307,0,0,0,0,0,2569,2569,2569,2569,2569,2569,61307,0,0,0,0,0,2569,2569,0,0,2569,2569,61307,0,0,0,0,0,2569,2569,0,0,2569,2569,0,0,0,0,0,2569,2569,2569,0,0,2569,2569,2569,0,0,
};
const uint16_t dg1[]=
{
    0,0,0,49152,40960,0,24576,32768,0,0,0,0,0,0,0,16384,35436,19061,27492,0,0,0,0,0,0,0,0,49152,2141,2405,2396,16384,40960,57344,49152,0,0,0,0,32768,18261,1357,26708,1,2140,27508,8192,0,0,0,0,57344,26453,58709,42837,49153,34661,51044,40960,0,40960,0,57344,32768,42837,25933,9821,26205,3222,2148,32768,0,24576,52068,35948,40960,26197,42317,58965,26461,34908,52076,16384,0,32768,43356,43356,2,25933,50517,42837,8193,0,8192,40960,0,24576,10588,26197,43126,42061,17749,10589,0,0,0,0,0,16384,3189,59492,34133,17493,17749,51292,16384,0,0,0,0,49152,16384,0,8192,42325,42333,51300,0,0,0,0,0,0,0,0,40960,59229,34133,18276,40960,0,0,0,0,0,0,0,57344,10061,34133,34916,57344,0,0,0,0,0,0,0,24576,2141,9813,10596,0,0,0,0,0,0,0,0,8192,43620,35172,61853,49152,0,0,0,0,0,0,0,49152,49152,16385,1,57344,0,0,0,0,
};
const uint16_t dg2[]=
{
   0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4368,0,0,0,4368,0,0,0,0,0,0,4368,4368,4368,0,4368,4368,4368,0,0,0,0,4368,65535,65535,4368,4368,4368,4368,4368,4368,0,0,0,4368,65535,4368,4368,4368,4368,4368,4368,4368,0,0,0,0,4368,4368,4368,4368,4368,4368,4368,0,0,0,0,0,0,4368,4368,4368,4368,4368,0,0,0,0,0,0,0,0,4368,4368,4368,0,0,0,0,0,0,0,0,0,0,4368,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};
const uint16_t dg3[]=
{
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

int main()
{
    int num=0;//it chooses which lane and cactus moves
    int lives = 3;//users lives
    int hinverted = 0;
    int vinverted = 0;
    int toggle = 0;
    int hmoved = 0;
    int vmoved = 0;
	int stop=0;//resets the user input back to null
	char userCommand;//user input from keybaord
	int max_time = 600;//max time the timer will go up to
	
	

    uint16_t x = 38;//{coords for character}
    uint16_t y = 57;
    uint16_t oldx = x;
    uint16_t oldy = y;
    
    uint16_t ob1x = 102;//{oords for first cactus}
    uint16_t ob1y = 34;
    uint16_t ob2x = 110;//{coords for second cactus}
    uint16_t ob2y = 58;
    uint16_t ob3x = 84;//{coords for third cactus}
    uint16_t ob3y = 82;
    initClock();
    initSysTick();
    setupIO();
	initSound();
	initSerial();

    
    
    const uint32_t mySongNotes[] = {C4, G3, E3, C3};//plays these notes on death
    const uint32_t mySongDurations[] = {500, 500, 500, 1000};//duration of each note
    const uint32_t numNotes = sizeof(mySongNotes) / sizeof(mySongNotes[0]);
    
    

    
    //putImage(20,80,12,16,deco1,0,0);
    while(1)//main loop
    {
		
	
		printText("Welcome to",30,20,RGBToWord(0xff,0xff,0) , 0);//{introdction to game}
		printText("Cowboy Runner",20,40,RGBToWord(0xff,0xff,0) , 0);
		printText("Hold Left",30,110,RGBToWord(0xff,0xff,0) , 0);
		printText("To start",35,120,RGBToWord(0xff,0xff,0) , 0);
		putImage(25,70,12,16,dg1,0,0);//{put cactus as decoration}
		putImage(85,70,12,16,dg1,0,0);
		while(1)//loop to animate the character at the start
		{
			if(dataavailable())//connects keyboard using serial ports
			{
				userCommand = egetchar();//grabs input
				stop=1;
			}
			putImage(55,70,12,16,deco1,0,0);//{animates the characters between different sprites}
			delay(300);
			putImage(55,70,12,16,movdeco1,1,0);
			delay(300);
			putImage(55,70,12,16,deco1,0,0);
			delay(300);
			putImage(55,70,12,16,movdeco1,0,0);
			delay(300);
			if ((GPIOB->IDR & (1 << 5))==0 || ((userCommand == 'a') && (stop==1)))//stops if a or left button clicked
			{
				break;
			}
		}
		
		
		if ((GPIOB->IDR & (1 << 5))==0 || ((userCommand == 'a') && (stop==1))) // left pressed or a clicked a
		{
			stop=0;//resets the users input if they clicked a
			initTimer();//initiates timer
			fillRectangle(0,0,200,200,0);//resets the screen
			drawLine(0,30,130,30,RGBToWord(256,0,0));//hor line1
        	drawLine(0,54,130,54,RGBToWord(256,0,0));//hor line2
        	drawLine(0,78,130,78,RGBToWord(256,0,0));//hor line3
        	drawLine(0,102,130,102,RGBToWord(256,0,0));//hor line4
        	drawLine(30,30,30,102,RGBToWord(256,0,0));//vert line1
        	drawLine(55,30,55,102,RGBToWord(256,0,0));//vert line2
        	hmoved = vmoved = 0;
        	hinverted = vinverted = 0;
        
        
        	while(lives != 0)//loop where main game starts
        	{
				
				timer(max_time);//calls the timer functions
				delay(100);
				if(dataavailable())//constantly collects user input
				{
					userCommand = egetchar();
					stop=1;
				}
				
            	printText("LIVES",90,3,RGBToWord(255,255,255),0);//displays lives at the top right
            	num=laneChooser();//calls random number to move a cactus
            	if(num==1)//moves lane 1
            	{
                	if(ob1x > 10)//checks if cactus at the end
                	{
                    	fillRectangle(ob1x,ob1y,12,16,0);//{code to move lane 1 cactus left}
                    	ob1x -= 6;
                    	putImage(ob1x,ob1y,12,16,dg1,0,0);
                    	delay(100);
                    	if(ob1x < 13)//resets position if at the end
                    	{
                        	fillRectangle(ob1x,ob1y,12,16,0);
                        	ob1x=110;
                        	ob1y=34;

                    	}
                	}
            	}
            	if(num==2)//moves lane 2
            	{
                	if(ob2x > 10)//checks if cactus at the end
                	{
                    	fillRectangle(ob2x,ob2y,12,16,0);//{code to move lane 2 cactus left}
                    	ob2x -= 7;
                    	putImage(ob2x,ob2y,12,16,dg1,0,0);
                    	delay(100);
                    	if(ob2x < 13)//resets position if at the end
                    	{
                        	fillRectangle(ob2x,ob2y,12,16,0);
                        	ob2x=110;
                        	ob2y=58;
                    	}
                	}
            	}
            	if(num==3)//moves lane 3
            	{   
                	if(ob3x > 10)//checks if cactus at the end
                	{
                    	fillRectangle(ob3x,ob3y,12,16,0);//{code to move lane 3 cactus left}
                    	ob3x -= 6;
                    	putImage(ob3x,ob3y,12,16,dg1,0,0);
                    	delay(100);
                    	if(ob3x < 13)//resets position if at the end
                    	{
                        	fillRectangle(ob3x,ob3y,12,16,0);
                        	ob3x=110;
                        	ob3y=82;
                    	}
                	}
            	}
            
            	if ( (GPIOA->IDR & (1 << 11)) == 0 || ((userCommand == 's') && (stop==1))) // down pressed or s is clicked
            	{
                	if (y < 84)//note it was 160 but we chnaged it to 84 so the player cannot escape the game
                	{//makes sure character cant leave the game
                    	y = y + 3;          
                    	vmoved = 3;
                    	vinverted = 0;
						stop=0;//resets the users input
                	}
            	}
            	if ( ((GPIOA->IDR & (1 << 8)) == 0) || ((userCommand == 'w') && (stop==1))) // up pressed or w is clicked
            	{           
                	if (y > 35)//note it was 16 but we changed it to 35 so user can escape the game
                	{//makes sure character cant leave the game
                    	y = y - 3;
                    	vmoved = 3;
                    	vinverted = 0;
						stop=0;//resets the users input
                	}
            	}
            	if ((GPIOB->IDR & (1 << 4))==0 || ((userCommand == 'd') && (stop==1))) // right pressed
            	{                   
                	if (x < 41)
                	{
                    	hmoved = 3;
                    	hinverted=0;
						printText("Paused", 10, 130, RGBToWord(0xff,0xff,0), 0);//prints pause
						delay(10000);//pauses the game for 10 seconds
						fillRectangle(10,130,40,16,0);//overwrites previous printText statement
						stop=0;//resets the users input
               		}                       
            	}
            	//if ((GPIOB->IDR & (1 << 5))==0) // left pressed
            	//{           
            
               		// if (x > 32)
                	//{
                   		// x = x - 3;
                    	//hmoved = 3;
                    	//hinverted=1;
                	//}           
            	//}
            	if ((vmoved) || (hmoved))
            	{
            	// only redraw if there has been some movement (reduces flicker)
                	fillRectangle(oldx,oldy,12,16,0);//fills the old position to black
                	oldx = x;//{remembers coords}
                	oldy = y;                   
                	if (hmoved)
                	{
                    	if (toggle)
                        	putImage(x,y,12,16,deco1,hinverted,0);
                    	else
                        	putImage(x,y,12,16,deco2,hinverted,0);
                
                    	toggle = toggle ^ 1;
                	}
                	else
                	{
                    	putImage(x,y,12,16,deco3,0,vinverted);
                	}
            	// Now check for an overlap by checking to see if ANY of the 4 corners of deco are within the target area
            	//if (isInside(20,80,12,16,x,y) || isInside(20,80,12,16,x+12,y) || isInside(20,80,12,16,x,y+16) || isInside(20,80,12,16,x+12,y+16) )
            	//{
                	//printTextX2("GLUG!", 10, 20, RGBToWord(0xff,0xff,0), 0);
            	//}
            	if(isInside(x,y,12,16,ob1x+4,ob1y+5) || isInside(x,y,12,16,ob1x+4,ob1y+5) || isInside(x,y,12,16,ob1x+4,ob1y+12) || isInside(x,y,12,16,ob1x,ob1y+12))
            	{//checks if the characters hits the cactus in lane 1
                	if(immune == 0)//checks if the user has immunity
                	{
                    	lives = lives -1;//lose a life
                    	immune = 3000;//gives immunity for 3 seconds
						
                	}
                
            	}
				if(isInside(x,y,12,16,ob2x+4,ob2y+5) || isInside(x,y,12,16,ob2x+4,ob2y+5) || isInside(x,y,12,16,ob2x+4,ob2y+12) || isInside(x,y,12,16,ob2x+4,ob2y+12))
            	{//checks if the characters hits the cactus in lane 2
                	if(immune == 0)//checks if the user has immunity
                	{
                    	lives = lives -1;//lose a life
                    	immune = 3000;//immunity for 3 seconds
                	}
                
            	}
				if(isInside(x,y,12,16,ob3x+4,ob3y+5) || isInside(x,y,12,16,ob3x+4,ob3y+5) || isInside(x,y,12,16,ob3x+4,ob3y+12) || isInside(x,y,12,16,ob3x,ob3y+12))
            	{//checks if the characters hits the cactus in lane 3
                	if(immune == 0)//checks if the user has immunity
                	{
                    	lives = lives -1;//lose a life
                    	immune = 3000;//immunity for 3 seconds
						
                	}
                
            	}
                putImage(ob1x,ob1y,12,16,dg1,0,0);//{puts all cactus in their positions}
                putImage(ob2x,ob2y,12,16,dg1,0,0);
                putImage(ob3x,ob3y,12,16,dg1,0,0);
                
            	}       
            	if(lives == 3)//checks theirs lives
            	{
                	//printNumber(3,90,120,RGBToWord(256,0,0),0);

                	putImage(80,10,12,16,dg2,0,0);//{prints the sprites for hearts}
                	putImage(95,10,12,16,dg2,0,0);
                	putImage(110,10,12,16,dg2,0,0);
					
            	}
            	if(lives == 2)//checks their lives
            	{
                	//printNumber(2,90,120,RGBToWord(256,0,0),0)
                	putImage(80,10,12,16,dg3,0,0);//{prints a black sprite to cover the heart}
                	putImage(95,10,12,16,dg2,0,0);//{prints the remaining hearts}
                	putImage(110,10,12,16,dg2,0,0);
					
            	}
            	if(lives == 1)//checks their lives
            	{
                	//printNumber(1,90,120,RGBToWord(256,0,0),0);
                	putImage(80,10,12,16,dg3,0,0);//{prints a black sprite to cover the heart}
                	putImage(95,10,12,16,dg3,0,0);
                	putImage(110,10,12,16,dg2,0,0);//prints the remaining hearts
					
            	}
            	if(lives == 0)//checks their lives
            	{
                	//printNumber(0,90,120,RGBToWord(256,0,0),0);
                	putImage(80,10,12,16,dg3,0,0);//{prints a black sprite to cover the heart}
                	putImage(95,10,12,16,dg3,0,0);
                	putImage(110,10,12,16,dg3,0,0);
					
					printTextX2("GAME OVER!", 10, 68, RGBToWord(0xff,0xff,0), 0);//prints game over if lost all their lives
                	playSong(mySongNotes, mySongDurations, numNotes);  //play game over song
                	delay(10000);//wait 10 seconds
					fillRectangle(0,0,200,200,0);//clear screen
            	}
            	
				
				delay(50);


        	}
			if(dataavailable())// collects user input after all lives lost
			{
				userCommand = egetchar();
				stop=1;
			}
			if ((GPIOB->IDR & (1 << 5))==0 || ((userCommand == 'a') && (stop==1)))//is left or a is pressed
			{
				
				restart();//restart game
				stop = 0;//reset user input keyboard
			}
			
		}
        
       

        
        
    }
    return 0;//end program
}

void playSong(const uint32_t *notes, const uint32_t *durations, uint32_t numNotes) //function to play the losing song
{
    for (uint32_t i = 0; i < numNotes; i++) {//for loop to play all notes for the duration
        uint32_t freq = notes[i];
        uint32_t duration_ms = durations[i];

        // Play the note
        playNote(freq);
        
        // Delay for the duration of the note
        delay(duration_ms);
        
        // Stop the note
        playNote(0);
    }
}
void initSysTick(void)
{
    SysTick->LOAD = 48000;
    SysTick->CTRL = 7;
    SysTick->VAL = 10;
    __asm(" cpsie i "); // enable interrupts
}
void SysTick_Handler(void)
{
    milliseconds++;
    if (immune>0)
    {
        immune--;
    }
	
}
void initClock(void)
{
// This is potentially a dangerous function as it could
// result in a system with an invalid clock signal - result: a stuck system
        // Set the PLL up
        // First ensure PLL is disabled
        RCC->CR &= ~(1u<<24);
        while( (RCC->CR & (1 <<25))); // wait for PLL ready to be cleared
        
// Warning here: if system clock is greater than 24MHz then wait-state(s) need to be
// inserted into Flash memory interface
                
        FLASH->ACR |= (1 << 0);
        FLASH->ACR &=~((1u << 2) | (1u<<1));
        // Turn on FLASH prefetch buffer
        FLASH->ACR |= (1 << 4);
        // set PLL multiplier to 12 (yielding 48MHz)
        RCC->CFGR &= ~((1u<<21) | (1u<<20) | (1u<<19) | (1u<<18));
        RCC->CFGR |= ((1<<21) | (1<<19) ); 

        // Need to limit ADC clock to below 14MHz so will change ADC prescaler to 4
        RCC->CFGR |= (1<<14);

        // and turn the PLL back on again
        RCC->CR |= (1<<24);        
        // set PLL as system clock source 
        RCC->CFGR |= (1<<1);
}
void delay(volatile uint32_t dly)
{
    uint32_t end_time = dly + milliseconds;
    while(milliseconds != end_time)
        __asm(" wfi "); // sleep
}

void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber)
{
    Port->PUPDR = Port->PUPDR &~(3u << BitNumber*2); // clear pull-up resistor bits
    Port->PUPDR = Port->PUPDR | (1u << BitNumber*2); // set pull-up bit
}
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode)
{
    /*
    */
    uint32_t mode_value = Port->MODER;
    Mode = Mode << (2 * BitNumber);
    mode_value = mode_value & ~(3u << (BitNumber * 2));
    mode_value = mode_value | Mode;
    Port->MODER = mode_value;
}
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py)
{
    // checks to see if point px,py is within the rectange defined by x,y,w,h
    uint16_t x2,y2;
    x2 = x1+w;
    y2 = y1+h;
    int rvalue = 0;
    if ( (px >= x1) && (px <= x2))
    {
        // ok, x constraint met
        if ( (py >= y1) && (py <= y2))
            rvalue = 1;
    }
    return rvalue;
}

void setupIO()
{
    RCC->AHBENR |= (1 << 18) + (1 << 17); // enable Ports A and B
	RCC->AHBENR |= (1 << 22);
    display_begin();
    pinMode(GPIOB,4,0);
    pinMode(GPIOB,5,0);
    pinMode(GPIOA,8,0);
    pinMode(GPIOA,11,0);
    enablePullUp(GPIOB,4);
    enablePullUp(GPIOB,5);
    enablePullUp(GPIOA,11);
    enablePullUp(GPIOA,8);
}
int laneChooser()//randomly chooses a lane
{
    int lane = 0;//assign value 0
    lane = (rand() % 10) + 1;//ranint between 1 and 3
    return lane;//return back to main()
}

void restart()//function that resets the system by writing a key and a bit mask to a specific register in the ARM Cortex-M series microcontrollers
{
	NVIC_SystemReset();
}
void initTimer() //initiates the timer to 0
{
    start_time = 0;
    milliseconds = 0;
}
void timer(uint32_t max_time) //function to print timer
{
    
    uint32_t elapsed_time = (milliseconds - start_time) / 1000;// calculate the elapsed time

   
    int timer_str[50];
    snprintf(timer_str, sizeof(timer_str), "%02u", elapsed_time); // display the elapsed time
    printTextX2(timer_str, 10, 5, RGBToWord(0xff,0xff,0), 0); // display the elapsed time

   
    if (elapsed_time >= max_time) //check if reached max time
	{
        initTimer(); // reset the timer
    }
}