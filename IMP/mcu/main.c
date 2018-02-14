/*********************************************************

	Author: Jan Gula
	Login: xgulaj00
	Project: [IMP] Game Snake
	Changes: Approx. 1/6th of the code is from dr. Vasicek's demo

*********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <fitkitlib.h>
#include <keyboard/keyboard.h>
#include <lcd/display.h>

#define max_range 8
#define UP 2
#define LEFT 4
#define RIGHT 6
#define DOWN 8
#define HEAD 10
void print(char message[50], int var);
void print_user_help(void);
int keyboard_idle();
unsigned int random();
unsigned char decode_user_cmd(char *cmd_ucase, char *cmd);
void fpga_initialized();
void generate_meal();


char last_ch, char_cnt;
int directions[max_range][max_range];

int main(){

	int printing_arr[max_range][max_range]; 
	int count = 0, old_score = 0, score = 0, x_head, y_head, x_tail, y_tail, i, j;
	char c = 0;

	char_cnt = 0;
	last_ch = 0;

	initialize_hardware();
	keyboard_init();
	
	set_led_d6(1);                       
	set_led_d5(1);
	P6DIR = 255;
	P6OUT = 0;
	P4DIR = 15;
	P4OUT = 0;
	P2DIR =	240;
	P2OUT = 0;

	for (i = 0; i < max_range; i++){
		for(j = 0; j < max_range; j++){
			printing_arr[i][j] = 0;
			directions[i][j] = 0;
		}
	}

	printing_arr[3][3] = 1;
	printing_arr[3][4] = 1;
	printing_arr[3][5] = 1;
	x_head = 3;
	y_head = 5;
	directions[3][3] = HEAD;
	directions[3][4] = RIGHT;
	directions[3][5] = RIGHT;

	generate_meal();
	for(;;) {
		delay_ms(1);
		for (i = 0; i < max_range; i++) {
			for (j = 0; j < max_range; j++) {
				if (directions[i][j] == HEAD) {
					x_tail = i;
					y_tail = j;
				}
			}
		}
		count++;
		if (c != last_ch) {
			c = last_ch;
			if (c == '2') {
				x_head--;
				if ((x_head < 0) || (directions[x_head][y_head] > 1)) {
					print("You LOOSE ", score);
					break;
				}
				if (directions[x_head][y_head] == 1) {
					directions[x_head][y_head] = UP;
					score++;
					generate_meal();
				}
				else {
					directions[x_head][y_head] = UP;
					directions[x_tail][y_tail] = 0;
					if (directions[x_tail + 1][y_tail] == DOWN) {
						directions[x_tail + 1][y_tail] = HEAD;
					}
					else if (directions[x_tail - 1][y_tail] == UP) {
						directions[x_tail - 1][y_tail] = HEAD;
					}
					else if (directions[x_tail][y_tail + 1] == RIGHT) {
						directions[x_tail][y_tail + 1] = HEAD;
					}
					else if (directions[x_tail][y_tail - 1] == LEFT) {
						directions[x_tail][y_tail - 1] = HEAD;
					}
				}

			}
			else if (c == '4') {
				y_head--;
				if ((y_head < 0) || (directions[x_head][y_head] > 1)) {
					print("You LOOSE", score);
					break;
				}
				if (directions[x_head][y_head] == 1) {
					directions[x_head][y_head] = LEFT;
					score++;
					generate_meal();
				}
				else {
					directions[x_head][y_head] = LEFT;
					directions[x_tail][y_tail] = 0;
					if (directions[x_tail + 1][y_tail] == DOWN) {
						directions[x_tail + 1][y_tail] = HEAD;
					}
					else if (directions[x_tail - 1][y_tail] == UP) {
						directions[x_tail - 1][y_tail] = HEAD;
					}
					else if (directions[x_tail][y_tail + 1] == RIGHT) {
						directions[x_tail][y_tail + 1] = HEAD;
					}
					else if (directions[x_tail][y_tail - 1] == LEFT) {
						directions[x_tail][y_tail - 1] = HEAD;
					}
				}
			}
			else if (c == '6') {
				y_head++;
				if ((y_head > 7) || (directions[x_head][y_head] > 1)) {
					print("You LOOSE ", score);
					break;
				}
				if (directions[x_head][y_head] == 1) {
					directions[x_head][y_head] = RIGHT;
					score++;
					generate_meal();
				}
				else {
					directions[x_head][y_head] = RIGHT;
					directions[x_tail][y_tail] = 0;
					if (directions[x_tail + 1][y_tail] == DOWN) {
						directions[x_tail + 1][y_tail] = HEAD;
					}
					else if (directions[x_tail - 1][y_tail] == UP) {
						directions[x_tail - 1][y_tail] = HEAD;
					}
					else if (directions[x_tail][y_tail + 1] == RIGHT) {
						directions[x_tail][y_tail + 1] = HEAD;
					}
					else if (directions[x_tail][y_tail - 1] == LEFT) {
						directions[x_tail][y_tail - 1] = HEAD;
					}
				}
			}
			else if (c == '8') {
				x_head++;
				if ((x_head > 7) || (directions[x_head][y_head] > 1)) {
					print("You LOOSE ", score);
					break;
				}
				if (directions[x_head][y_head] == 1) {
					directions[x_head][y_head] = DOWN;
					score++;
					generate_meal();
				}
				else {
					directions[x_head][y_head] = DOWN;
					directions[x_tail][y_tail] = 0;
					if (directions[x_tail + 1][y_tail] == DOWN) {
						directions[x_tail + 1][y_tail] = HEAD;
					}
					else if (directions[x_tail - 1][y_tail] == UP) {
						directions[x_tail - 1][y_tail] = HEAD;
					}
					else if (directions[x_tail][y_tail + 1] == RIGHT) {
						directions[x_tail][y_tail + 1] = HEAD;
					}
					else if (directions[x_tail][y_tail - 1] == LEFT) {
						directions[x_tail][y_tail - 1] = HEAD;
					}
				}
			}
			else {
				continue;
			}
			c = last_ch;
		}
		
		for (i = 0; i < max_range; i++) {
				for (j = 0; j < max_range; j++) {
					if (directions[i][j] != 0) {
						printing_arr[i][j] = 1;
					}
					else{
						printing_arr[i][j] = 0;
					}
				}
		}
	int decimal_value = 0;
	int exponent = 1;
	for (i = 0; i < max_range; i++)  
	{
		for (j = max_range - 1; j >= 0; j--)
		{
			decimal_value += printing_arr[i][j] * exponent;
			exponent *= 2; 
		}
		delay_ms(2);
		P4OUT = decimal_value;
		P2OUT = decimal_value;
		decimal_value = 0;
		exponent = 1; 

		switch (i){
			case 0: 
				P6OUT = 254; 
				break;
			case 1: 
				P6OUT = 253; 
				break;
			case 2: 
				P6OUT = 251; 
				break;
			case 3:
				P6OUT = 247; 
				break;
			case 4: 
				P6OUT = 239; 
				break;
			case 5: 
				P6OUT = 223; 
				break;
			case 6: 
				P6OUT = 191; 
				break;
			case 7: 
				P6OUT = 127; 
				break;
		}

	}
		keyboard_idle();   
		terminal_idle();  
	}
	return 0;
}

void print_user_help(void){
}

int keyboard_idle()
{
  char ch;
  ch = key_decode(read_word_keyboard_4x4());
  if (ch != last_ch) 
  {
		last_ch = ch;
    if (ch != 0) 
    {
      char_cnt++;
      
    }
  }
  return 0;
}
 
unsigned int random(){
	static int seed = 0;
	if(seed == max_range - 1)
		seed = 0;
	return seed++;

}

unsigned char decode_user_cmd(char *cmd_ucase, char *cmd)
{
  return CMD_UNKNOWN;
}

void fpga_initialized()
{
  LCD_init();
  LCD_clear();
}


void generate_meal(){
	int x,y;
	x = random(); y = random();
	while (directions[x][y] != 0){
		x = random(); y = random();
	}
	directions[x][y] = 1; 
}

void print(char message[50], int var)
{
	char pom[50];
	LCD_clear();
	LCD_append_string(message);
	sprintf(pom, "%d", var);
	LCD_append_string(pom);
}

