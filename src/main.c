#include <stm32f031x6.h>
#include "display.h"
#include "sound.h"
#include "musical_notes.h"
#include <stdio.h>

void initClock(void);
void initSysTick(void);
void SysTick_Handler(void);
void delay(volatile uint32_t dly);
void setupIO();
int isInside(uint16_t x1, uint16_t y1, uint16_t w, uint16_t h, uint16_t px, uint16_t py);
void enablePullUp(GPIO_TypeDef *Port, uint32_t BitNumber);
void pinMode(GPIO_TypeDef *Port, uint32_t BitNumber, uint32_t Mode);
void spawnApple();
void death();
void timer();
void update();
void menu();
void ding();
void ding2();
void deathNoise();

int rand(void);

//max of 72 spaces on screen, multiplied by 2 for x and y coordinates
int segments[144] = {};
int i;
int score = 0;
int highScore;
int gameOn = 1;
int xA;
int yA;
int hinverted;
int vinverted;
int hmoved = 0;
int vmoved = 0;
uint16_t x = 48;
uint16_t y = 64;

volatile uint32_t milliseconds;

const uint16_t apple[]=
{
	0,0,0,0,0,0,49169,33323,33323,49169,0,0,0,0,0,0,0,0,59144,61200,11792,3848,43315,58454,58454,43315,3848,52752,28168,44304,8192,0,8192,0,61456,16169,16161,24352,4667,16973,16973,62011,32544,32561,16177,48672,8448,0,8192,0,52752,57120,56352,7968,20787,17485,25677,4403,16168,32462,16334,31529,256,0,8192,0,52752,57120,40224,32032,7457,39969,48161,48160,48425,24534,32726,39977,256,0,8192,0,52752,57120,32032,40224,32032,24096,24096,7456,56617,16359,16359,48169,256,0,8192,0,52752,57120,32032,40224,40224,48416,48416,32032,15657,65147,8059,64544,256,0,8192,0,52752,57120,32032,40224,40224,40224,40224,40224,32032,48400,48400,23584,16640,0,8192,0,52752,57120,32032,40224,40224,40224,40224,40224,40224,64800,7721,31776,8448,0,8192,0,52752,57120,32032,40224,40224,40224,40224,40224,40224,40224,40480,31776,8448,0,8192,0,52752,57120,32032,40224,40224,40224,40224,40224,40224,40224,40480,23584,8448,0,8192,0,53008,65320,40224,40480,40480,40480,40480,40480,40480,40480,48672,31776,16640,0,8192,0,44560,48672,23328,23584,23584,23584,23584,23584,23584,23328,31776,14880,8448,0,0,0,8192,16640,8448,16640,16640,16640,16640,16640,16640,16640,16640,8448,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
};

const uint16_t snakeHead[]=
{
	9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,57898,57898,57898,9293,9293,0,0,0,0,9293,9293,57898,57898,0,0,0,0,57898,57898,9293,9293,0,0,0,0,9293,9293,57898,57898,0,0,0,0,57898,57898,9293,9293,65535,65535,65535,65535,9293,9293,57898,57898,65535,65535,65535,65535,57898,57898,9293,9293,65535,65535,65535,65535,9293,9293,9293,57898,65535,65535,65535,65535,57898,57898,9293,9293,65535,65535,65535,65535,9293,9293,9293,9293,65535,65535,65535,65535,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,
};

const uint16_t snakeBody[]=
{
	9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,
};

const uint16_t snakeHeadSide[]=
{
	57898,57898,57898,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,65535,65535,65535,0,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,65535,65535,65535,0,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,65535,65535,65535,0,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,65535,65535,65535,0,0,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,9293,57898,57898,57898,57898,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,65535,65535,65535,0,0,57898,57898,9293,9293,9293,9293,9293,9293,9293,9293,9293,65535,65535,65535,0,0,57898,57898,9293,57898,57898,57898,57898,9293,9293,9293,9293,65535,65535,65535,0,0,57898,57898,9293,57898,57898,57898,57898,57898,57898,9293,57898,65535,65535,65535,0,0,57898,57898,57898,57898,57898,57898,57898,57898,57898,9293,57898,57898,57898,57898,57898,57898,57898,57898,57898,57898,57898,57898,57898,57898,57898,9293,57898,57898,57898,57898,57898,57898,57898,57898,
};

int main()
{
	//initalising
	initClock();
	initSysTick();
	setupIO();
	initSound();


	//INTRO SCREEN
	menu();

	while(1){
		hmoved = vmoved = 0;
		hinverted = vinverted = 0;

		if ((GPIOB->IDR & (1 << 4))==0) // right pressed
		{	
			//changed from if to while for constant movement
			while (1)
			{
				segments[(score*2)] = x;
				segments[(score*2)+1] = y;

				if(112<=x)//right border
				{ 
					x=-16; //set to left side of screen, gives wrapping affect
				}

				//move 16 pixels right, because game operates in 16x16 pixel grid
				x = x + 16;

				//save movement
				hmoved = 1;

				//horizontal not inverted
				hinverted=0;

				//bens movement
				update();	

				if (((GPIOA->IDR & (1 << 11)) == 0)||((GPIOA->IDR & (1 << 8)) == 0)) // down pressed or up pressed
				{
					//stop moving right
					hmoved = 0;
					break;
				}

			}

		}

		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{		
			//changed from if to while for constant movement
			while (1)
			{
				segments[(score*2)] = x;
				segments[(score*2)+1] = y;
				
				if(x<1)//left border
				{ 
					x=128; //set to right 
				}

				//move 16 pixels left
				x = x - 16;

				//save movement direction
				hmoved = 1;

				//invert horizontal
				hinverted=1;
				
				//bens movement
				update();	

				if (((GPIOA->IDR & (1 << 11)) == 0)||((GPIOA->IDR & (1 << 8)) == 0)) // down pressed or up pressed
				{
					//stop moving left
					hmoved = 0;
					break;
				}
			} 
			

		}

		if ((GPIOA->IDR & (1 << 11)) == 0) // down pressed
		{ 
			//changed from if to while for constant movement
			while (1)
			{
				if(144<=y)//down border
				{ 
					y=0; //set to top 
				}
				segments[(score*2)] = x;
				segments[(score*2)+1] = y;

				//move 16 pixels down
				y = y + 16;	

				//save movement		
				vmoved = 1;

				//invert vertical
				vinverted = 1;

				update();		
				
				if ( ((GPIOB->IDR & (1 << 4))==0) || ( (GPIOB->IDR & (1 << 5))==0)) // left pressed or right pressed
				{
					//stop moving
					break;
				}


			} 
			

		}

		if ((GPIOA->IDR & (1 << 8)) == 0) // up pressed
		{		
			//changed from if to while for constant movement
			while (1)
			{
				if(y<20)//up border
				{ 
					y=160; // set to bottom
				}
				segments[(score*2)] = x;
				segments[(score*2)+1] = y;

				//move 16 pixels up
				y = y - 16;

				//save movement
				vmoved = 1;

				//vertical not inverted
				vinverted = 0;

				update();		

				if (((GPIOB->IDR & (1 << 4))==0) || ( (GPIOB->IDR & (1 << 5))==0)) // left pressed or right pressed
				{
					//stop moving
					break;
				}
				
			} 

		}
		delay(50);
	}
	return 0;
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

void spawnApple()
{
	//randomly generate a number between 0-7
	xA = (rand()%8)* 16;

	//randomly generate a number between 1-9
	yA = (rand()%9 + 1)* 16;
	//multiply by 16 to get co ordinates

	//check if apple is in a segment of snake
	for(i = 2;i <= score*2;i=i+2)
	{
		if(isInside(xA,yA,1,1,segments[i],segments[i+1]))
		{
			//if it is, spawn new apple
			spawnApple();
		}
	}
}

void death()
{
	gameOn = 0;
	int newHighScore = 0;

	//clear screen
	fillRectangle(0,16,128,144,0); 

	//reset co ordinates
	x = 48;
	y = 64;

	//move back to start postion
	putImage(x,y,16,16,snakeHeadSide,0,0);

	//black out screen
	fillRectangle(0,0,128,160,0);

	//if the score is higher than the current highScore
	if(score>highScore){
		//make the score the new HighScore
		highScore = score;
		newHighScore = 1;
	}


	//game over message
	printTextX2("Game",32,16,500,0);
	printTextX2("Over!!",32,32,500,0);

	//display score
	printText("Score:",32,72,500,0);
	printNumber(score, 32, 80, RGBToWord(0xff,0xff,0), 0);

	score = 0;

	//display if won new high score
	if (newHighScore == 1)
	{
		printText("New High score!",16,56, RGBToWord(255,192,203),0);

		//highscore sound effect
		delay(400);
		playNote(C6);
		delay(200);
		playNote(C4);
		delay(100);
		playNote(C7);
		delay(250);
		playNote(0);
	}

	//game restart
	printText("up to go to menu",8,112,500,0);//change from 8,48,500,0
	while(gameOn == 0)
	{
		//if up is pressed
		if(((GPIOA->IDR & (1 << 8)) == 0))
		{
			//black out screen
			fillRectangle(0,0,128,160,0);
			ding2();
			menu();
			gameOn = 1;
		}
	}

}

void timer()
{	
	//countdown
	for(i=3;i>0;i--)
	{
		//print number
		printNumberX2(i, 12, 80, RGBToWord(0xff,0xff,0), 0);

		//black out first 4 digits
		fillRectangle(0,80,60,16,0);

		//countdown sound
		if(i == 3){
			playNote(C4);
		}
		if(i == 2){
			playNote(C5);
		}
		if(i == 1){
			playNote(C6);
		}

		//delay
		delay(1000);
	}
	//end sound of countdown
	playNote(0);

	//black out whole section
	fillRectangle(0,80,128,16,0);

}

void update()
{
	if ((vmoved) || (hmoved))
	{
		// only redraw if there has been some movement (reduces flicker)
		fillRectangle(0,16,128,144,0); 
		//displaying the apple
		putImage(xA,yA,16,16,apple,0,0);

		//snake body
		//array arrangement
		//headx,heady,first body segment x coord,first body segment y coord,second body segment x coord,second body segment y coord, ...

		//loop backwards through the snake
		for(i = score*2; i >= 0;i = i-2)
		{
			//move current segment to segment before it
			segments[i] = segments[i-2]; //x value
			segments[i+1] = segments[i-1]; //y value
		}
		segments[0] = x;
		segments[1] = y;

		//drawing snake
		//Head direction 
		if (hmoved)
		{
			putImage(x,y,16,16,snakeHeadSide,hinverted,0);
		}
		else
		{
			putImage(x,y,16,16,snakeHead,0,vinverted);
		}
		//body
		for(i = 2;i <= score*2;i=i+2)
		{
			putImage(segments[i],segments[i+1],16,16,snakeBody,0,0);
		}
		//if snake collides with apple
		if (isInside(xA,yA,1,1,x,y))
		{
			//spawn a new apple
			spawnApple();

			//play a noise
			ding();

			delay(100);

			//increment score
			score += 1;

			//update score displayed
			printNumber(score, 4, 4, RGBToWord(0xff,0xff,0), 500);

		}
		//snake body collison check
		for(i = 2;i <= score*2;i=i+2)
		{
			if (isInside(x,y,1,1,segments[i],segments[i+1]))
			{
				//kill snake
				deathNoise();
				death();
			}
		}
	}		
	delay(400);
}

void menu(){
	int startMenu=1;
    while(startMenu)
    {
		//intro message
		printTextX2("Snake",32,8,500,0);
		printText("hold up",32,40,1000,0);
		printText("to play!",32,48,1000,0);

		printText("down for",32,72,1000,0);
		printText("creators",32,80,1000,0);

		printText("left for",32,104,1000,0);
		printText("High Score",32,112,1000,0);
		
        delay(1000);

        if(((GPIOA->IDR & (1 << 8)) == 0)) // up pressed
        {
			ding2();
			fillRectangle(0,0,128, 160, 0x0);
			fillRectangle(0,0,160,16,500); //top bar
			printNumber(score, 4, 4, RGBToWord(0xff,0xff,0), 500);

			//display first apple
			spawnApple();

			//start postion
			putImage(x,y,16,16,snakeHeadSide,hinverted,0);

			//countdown
			delay(500);
			timer();
            startMenu = 0;
        }

		if ((GPIOA->IDR & (1 << 11)) == 0) //down pressed
		{
			ding2();
			fillRectangle(0,0,128, 160, 0x0);
			printTextX2("Team:",32,8,500,0);
			printText("Code: Dean",8,48,1000,0);
        	printText("Art&Sound: Susie",8,68,1000,0);
        	printText("Code: Ben",8,88,1000,0);

			while(1){
				printText("up to exit",32,144,1000,0);
				if(((GPIOA->IDR & (1 << 8)) == 0)) // up pressed
				{
					fillRectangle(0,0,128, 160, 0x0);
					ding2();
					break;
				}
			}
		}

		if ((GPIOB->IDR & (1 << 5))==0) // left pressed
		{
			ding2();
			fillRectangle(0,0,128, 160, 0x0);
			printTextX2("Highscore:",8,48,RGBToWord(255,192,203),0);
			printNumberX2(highScore,32,64,500,0);
			printText("Highest possible:",8,96,1000,0);
			printNumber(72,32,112,1000,0);
			while(1){
				printText("up to exit",32,144,1000,0);
				if(((GPIOA->IDR & (1 << 8)) == 0)) // up pressed
				{
					fillRectangle(0,0,128, 160, 0x0);
					ding2();
					break;
				}
			}
		}

    }
}



//Eat apple sound effect
void ding()
{
	playNote(C7);
	delay(100);
	playNote(0);
}


//Press button on menu sound effect
void ding2()
{
	playNote(D4);
	delay(150);
	playNote(0);
}


//Death sound effect
void deathNoise()
{
	playNote(A6);
	delay(300);
	playNote(A4);
	delay(400);
	playNote(A3);
	delay(550);
	playNote(0);
}