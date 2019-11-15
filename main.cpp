//Define global variables
#define PDC_DLL_BUILD 1
#define PDC_WIDE 1
#define WINCOLORS 1
#define PROMPTCOLORS 2
#define TERMCOLORS 3
#define TERMTEXT 4
#define ESCAPE 27
#define SAVE ctrl('s')
#define LOAD ctrl('l')
#define NEW_FILE ctrl('n')
#define AUTO_COMPLETE ctrl('a')
#define COMPRESS ctrl('k')
#define ctrl(x)           ((x) & 0x1f)
#define LEFT_ARROW 260
#define	RIGHT_ARROW 261
#define UP_ARROW 259
#define DOWN_ARROW 258
#define BACKSPACE 8
#define NEWLINE 10



//Header Files
#include "curses.h"
#include "panel.h"
#include "curspriv.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Trie.h"
#include <stack>
#include <functional>
#include <queue>
#include <unordered_map>
#include <sstream>

using namespace std;


////////////////////////////////          Function Prototypes            ////////////////////////////////
bool confirm(void);
void printBuffer(WINDOW* win, vector<vector <int> > buff, int frow, int lrow, int fcol, int lcol);
string intToBinary(int some_number);



//******************************            Main Function                  *******************************
int main(int argc, char* argv[])
{

	//////////////////////////////////          This is where the window is built      ////////////////////////
		//create variables
	int term_cols = 120;
	int term_rows = 30;
	int win_cols = 0;
	int win_fcol = 0;
	int win_frow = 0;
	int win_rows = 0;
	int curs_x = 0;
	int curs_y = 0;
	int screen_y = 0;
	int screen_x = 0;
	int output_ledge = 0;
	int output_redge = 0;
	int output_bedge = 0;
	int output_tedge = 0;
	int vector_y = 0;
	int vector_x = 0;
	int buffer_max_y = 0;
	int buffer_max_x = 0;

	
	char fname[30];
	char fileChar;
	char c;
	ifstream inFile;
	ifstream keywords;
	ofstream outFile;
	vector< vector <int> > buffer;	
	vector < vector <int> > ::iterator row;	
	vector<int>::iterator col; 
	string filename = "";


	Trie dictionary{};
	stack<int> stack;
	string searchString = "";
	vector<string> wordsFound;


	unordered_map<string, int> wordFreq;
	priority_queue<pair<string, int>, vector<pair<string,int>>, greater<pair<string,int>>> maxHeap;
	string word = "";
	unordered_map<string, string> compKey;
	int orderOffHeap = 0;
	int binConversion;
	vector< vector<int> > compBuffer;
	string item = "";
	string wordToReplace = "";
	int comp_y = 0;
	int comp_x = 0;
	ostringstream result;
	string compFilename;
	string codeFilename;

	


	//initialize the terminal
	initscr();

	//Resize the terminal for initial size
	
	resize_term(term_rows, term_cols);
	getmaxyx(stdscr, term_rows, term_cols);

	//initialize the window dimensions
	win_cols = term_cols - 4;
	win_fcol = 2;
	win_frow = 2;
	win_rows = term_rows - 5;
	output_redge = win_cols;
	output_ledge = 0;
	output_bedge = win_rows;
	output_tedge = 0;
	buffer.push_back(vector<int>{});

	//initialize the main window
	WINDOW* main_window = newwin(win_rows, win_cols, win_frow, win_fcol);
	WINDOW* auto_comp = newwin(5, 15, win_rows - 16, win_cols - 6);
	


	//Set input characteristics	
	cbreak();
	keypad(main_window, true);
	//Set Scrolling
	//scrollok(main_window, true);
	//wsetscrreg(main_window, win_rows, win_cols);
		


	//Set cursor visibility
	curs_set(2);


	//turn off keyboard echo
	noecho();



	//color initializations
	if (can_change_color()) {
		//start colors
		start_color();

		// Black text on a white screen for regular Window Colors
		init_pair(WINCOLORS, COLOR_BLACK, COLOR_WHITE);

		// White text on a red screen for Prompt Colors
		init_pair(PROMPTCOLORS, COLOR_WHITE, COLOR_RED);

		// Blue border and white inserior for the Terminal Colors
		init_pair(TERMCOLORS, COLOR_BLUE, COLOR_WHITE);

		// White text to contrast with Blue Terminal border text.
		init_pair(TERMTEXT, COLOR_WHITE, COLOR_BLUE);

		// Initialize the background with WINCOLORS
		wbkgd(main_window, COLOR_PAIR(WINCOLORS));

		//Initialize the Terminal Colors
		bkgd(COLOR_PAIR(TERMCOLORS));
	}
	else {
		use_default_colors();
	}



	//creates the BLUE border around the perimeter of the terminal
	border(ACS_BLOCK, ACS_BLOCK, ACS_BLOCK, ACS_BLOCK,
		ACS_BLOCK, ACS_BLOCK, ACS_BLOCK, ACS_BLOCK);


	//Terminal text at the top and bottom of the window	
	attron(COLOR_PAIR(TERMTEXT));
	mvprintw(0, 3, "Welcome to the Airth Text Editor!    | ESC - Quit |");
	mvprintw(term_rows - 1, 3, "CTRL + : S =Save , L =Load , N =New , A =Predict, K =Compress");

	attroff(COLOR_PAIR(TERMTEXT));
	
	//This code below builds the Dictionary based on a Trie data structure
	keywords.open("keywords.txt");
	if (keywords.is_open())
	{
		string word = "";
		if (keywords.good())
		{
			while (keywords >> word)
			{
				dictionary.addWord(word);
			}
		}
	}
	keywords.close();


	if (argc >= 2)
	{

		inFile.open(argv[1]);

		if (!inFile.is_open())
		{
			attron(COLOR_PAIR(TERMTEXT));
			//
			mvprintw(term_rows - 1, term_cols - 50, "                                           ");
			//
			attroff(COLOR_PAIR(TERMTEXT));

			attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

			mvprintw(term_rows - 1, term_cols - 50, "File loading has failed");

			attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS	

			attron(COLOR_PAIR(TERMTEXT));

			mvprintw(term_rows - 1, term_cols - 50, "                                           ");

			attroff(COLOR_PAIR(TERMTEXT));

			refresh();

		}

		if (inFile.good())
		{
			int copy_y = 0;
			int copy_x = 0;
			


			while (!inFile.eof())
			{
				inFile.get(fileChar);

				if (fileChar == 10)
				{
					
					buffer.push_back(vector<int>{});
					copy_y++;
					if(vector_x<copy_x)
					{
						buffer_max_x = copy_x;
					}
					copy_x = 0;
				}
				else
				{
					if (vector_x < copy_x)
					{
						buffer_max_x = copy_x;
					}
					buffer[copy_y].push_back(fileChar);
					copy_x++;
				}

			}
			
			inFile.close();
			buffer_max_y = copy_y;

			attron(COLOR_PAIR(TERMTEXT));

			mvprintw(term_rows - 1, term_cols - 50, "                                           ");

			attroff(COLOR_PAIR(TERMTEXT));

			attron(COLOR_PAIR(PROMPTCOLORS));

			//exit prompt print
			mvprintw(term_rows - 1, term_cols - 50, "Your file has been loaded! Press Any Key.");

			mvprintw(0, term_cols-30, argv[1]);

			attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

			refresh();

			getch();

			attron(COLOR_PAIR(TERMTEXT));

			mvprintw(term_rows - 1, term_cols - 50, "                                           ");

			attroff(COLOR_PAIR(TERMTEXT));


		}
	}


		//print out the buffer to the screen based on window size
	printBuffer(main_window, buffer, output_tedge, output_bedge, output_ledge, output_redge);


	

		//sets cursor initial position	
		wmove(main_window, curs_y, curs_x);

		refresh();
		wrefresh(main_window);

		
		row = buffer.begin();
		
		col = buffer[vector_y].begin();

		///////////////////////         This is where the input characters get handled           /////////////////
			//set whether user wants to quit
		bool quit = false;
		int input;

		//this loop checks the quit bool after every key input

		while (!quit) {

			vector_y = int(curs_y + output_tedge);
			vector_x = int(curs_x + output_ledge);
     		input = wgetch(main_window);
			



			//if the terminal is resized, the terminal window is all redrawn to the new dimensions
			if (is_termresized() == true)
			{

				clear();
				//getyx(main_window, curs_y, curs_x);
				resize_term(0, 0);
				//getmaxyx(stdscr, term_rows, term_cols);

				//win_cols = term_cols - 4;
				//win_fcol = 2;
				//win_frow = 2;
				//win_rows = term_rows - 4;
				//wresize(main_window, win_cols, win_rows);

				//creates the BLUE border around the perimeter of the terminal
				border(ACS_BLOCK, ACS_BLOCK, ACS_BLOCK, ACS_BLOCK,
					ACS_BLOCK, ACS_BLOCK, ACS_BLOCK, ACS_BLOCK);

				//add Welcome String to the screen
				attron(COLOR_PAIR(TERMTEXT)); //CHANGES COLOR FOR PROMPT
				mvprintw(0, 3, "Welcome to the Airth Text Editor!");
				mvprintw(term_rows - 1, 3, "CTRL + : S =Save , L =Load , N =New , A =Predict, K =Compress");
				attroff(COLOR_PAIR(TERMTEXT)); //CHANGES COLOR BACK TO WINCOLORS

				//get new window edges
				output_redge = win_cols - 1;
				output_bedge = win_rows - 1;

				//re-print all text from the buffer onto the screen with the new dimensions
				printBuffer(main_window, buffer, output_tedge, output_bedge, output_ledge, output_redge);

				wmove(main_window, curs_y, curs_x);

				refresh();
				wrefresh(main_window);




			}
			//getyx(main_window, curs_y, curs_x);



		switch (input) {
			case (ESCAPE):
				//changes color for prompt
				attron(COLOR_PAIR(PROMPTCOLORS));
				//exit prompt print
				mvprintw(term_rows - 1, term_cols - 50, "Are you sure you want to quit? Y or N");



				attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

				attron(COLOR_PAIR(TERMTEXT));

				quit = confirm();

				mvprintw(term_rows - 1, term_cols - 45, "                                        ");

				if (!quit)
				{
					break;
				}

				attroff(COLOR_PAIR(TERMTEXT));

				//changes color for prompt
				attron(COLOR_PAIR(PROMPTCOLORS));

				mvprintw(term_rows - 1, term_cols - 50, "Would you like to save your file? Y or N");


				attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS



				if (confirm()) {
					//changes color for prompt
					attron(COLOR_PAIR(PROMPTCOLORS));

					//exit prompt print
					mvprintw(term_rows - 1, term_cols - 50, "Your file has been saved! Press any key.");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					attron(COLOR_PAIR(TERMTEXT));

					input = wgetch(main_window);

					mvprintw(term_rows - 1, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					break;

				}
				else
				{
					attron(COLOR_PAIR(PROMPTCOLORS));

					mvprintw(term_rows - 1, term_cols - 50, "Your file has not been saved! Press any key.");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					attron(COLOR_PAIR(TERMTEXT));

					input = wgetch(main_window);

					mvprintw(term_rows - 1, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					break;
				}

				wrefresh(main_window);
				break;

			case (SAVE):

				attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

				mvprintw(term_rows - 1, term_cols - 50, "Would you like to save your file? Y or N");

				attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

				if (confirm())
				{
					string filename;
					if (inFile.is_open())
					{
						inFile.close();
					}

					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(term_rows - 1, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					mvprintw(term_rows - 1, term_cols - 50, "What is the name and extension of this file?");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(0, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					refresh();

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					wmove(stdscr, 0, term_cols - 30);

					c = getch();

					while (c != 10)
					{
						filename.push_back(c);
						addch(c);
						c = getch();
					}



					strncpy_s(fname, filename.c_str(), sizeof(filename));


					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS						


					outFile.open(fname, std::ofstream::out | std::ofstream::trunc);


					if (!outFile.good())
					{
						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                                  ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

						mvprintw(term_rows - 1, term_cols - 50, "Your file has NOT been saved! Press any key.");

						attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS	

						getch();

						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                              ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(0, term_cols - 50, "                                           ");

						attroff(COLOR_PAIR(TERMTEXT));

						memset(fname, 0, sizeof(fname));

						filename.clear();

						refresh();

						break;
					}

					else if (outFile.good())

					{

						for (int i = 0; i < buffer.size(); i++)
						{
							for (int j = 0; j < buffer[i].size(); j++)
							{
								outFile.put(buffer[i][j]);
							}
							outFile.put(10);
						}

						outFile.close();

						if (!outFile.is_open())
						{
							attron(COLOR_PAIR(TERMTEXT));

							mvprintw(term_rows - 1, term_cols - 50, "                                              ");

							attroff(COLOR_PAIR(TERMTEXT));

							attron(COLOR_PAIR(PROMPTCOLORS));

							//exit prompt print
							mvprintw(term_rows - 1, term_cols - 50, "Your file has been saved! Press any key.");

							attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

							refresh();

							getch();
						}
						else
						{
							attron(COLOR_PAIR(TERMTEXT));

							mvprintw(term_rows - 1, term_cols - 50, "                                              ");

							attroff(COLOR_PAIR(TERMTEXT));

							attron(COLOR_PAIR(PROMPTCOLORS));

							//exit prompt print
							mvprintw(term_rows - 1, term_cols - 50, "An unknown error has occurred. Press any key.");

							attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

							refresh();

							getch();
						}

					}


				}
				else
				{
					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(term_rows - 1, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					mvprintw(term_rows - 1, term_cols - 50, "File saving cancelled. Press any key.");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					getch();

				}




				attron(COLOR_PAIR(TERMTEXT));

				mvprintw(term_rows - 1, term_cols - 50, "                                           ");

				attroff(COLOR_PAIR(TERMTEXT));

				refresh();
				break;

			case (LOAD):
				attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

				mvprintw(term_rows - 1, term_cols - 50, "Would you like to load a file? Y or N");

				attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

				if (confirm())
				{
					string filename;

					if (inFile.is_open())
					{
						inFile.close();
					}

					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(term_rows - 1, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					mvprintw(term_rows - 1, term_cols - 50, "What is the name of the .txt file?");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(0, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					refresh();

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					wmove(stdscr, 0, term_cols - 30);

					c = getch();

					while (c != 10)
					{
						filename.push_back(c);
						addch(c);
						c = getch();
					}



					strncpy_s(fname, filename.c_str(), sizeof(filename));


					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS						


					inFile.open(fname);


					if (!inFile.good())
					{
						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                           ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

						mvprintw(term_rows - 1, term_cols - 50, "File loading has failed. Press any key.");

						attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS	

						getch();

						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                           ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(0, term_cols - 50, "                                           ");

						attroff(COLOR_PAIR(TERMTEXT));

						memset(fname, 0, sizeof(fname));

						filename.clear();

						refresh();

						break;
					}

					else

					{
						buffer.clear();
						buffer.push_back(vector<int>{});
						curs_y, curs_x = 0;
						int copy_y = 0;
						int copy_x = 0;

						while (!inFile.eof())
						{
							inFile.get(fileChar);

							if (fileChar == 10)
							{

								buffer.push_back(vector<int>{});
								copy_y++;
								if (vector_x < copy_x)
								{
									buffer_max_x = copy_x;
								}
								copy_x = 0;
							}
							else
							{
								buffer[copy_y].push_back(fileChar);
								copy_x++;
							}

							buffer_max_y = copy_y;

						}


						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                           ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(PROMPTCOLORS));

						//exit prompt print
						mvprintw(term_rows - 1, term_cols - 50, "Your file has been loaded! Press any key");

						attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

						refresh();

						getch();


					}


				}
				else
				{

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					mvprintw(term_rows - 1, term_cols - 50, "File loading cancelled. Press any key.");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					getch();

				}




				attron(COLOR_PAIR(TERMTEXT));

				mvprintw(term_rows - 1, term_cols - 50, "                                           ");

				attroff(COLOR_PAIR(TERMTEXT));

				refresh();
				break;

			case (NEW_FILE):
				attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

				mvprintw(term_rows, term_cols - 50, "Would you like to make a new file? Y or N");

				attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS
				break;


			case (KEY_LEFT):

				//if cursor is at left edge, with more to display, the text shifts rightward and cursor doesn't move.
				if (curs_x == 0 && output_ledge > 0)
				{
					output_ledge--;
					output_redge--;
					break;
				}
				//if the cursor is at the left edge, with nothing more to display, nothing happens
				else if (curs_x == 0 && output_ledge == 0)
				{
					break;
				}
				//in all other cases, the cursor moves left.
				else
				{
					curs_x--;
					break;
				}


			case (KEY_RIGHT):

				//if cursor is at right edge, with more to display, the text shifts leftward and cursor doesn't move.
				if (curs_x == win_cols - 1 && output_redge < buffer[vector_y].size())
				{
					output_redge++;
					output_ledge++;
					break;
				}

				//if cursor is at the right edge, with nothing more to display nothing happens
				else if (curs_x == buffer[vector_y].size())
				{
					break;
				}
				else
				{
					curs_x++;

					break;

				}



			case (KEY_UP):
				//if cursor is at the top edge, with more to display, the text shifts downward and cursor doesn't move.
				if (curs_y == 0 && output_tedge > 0) {
					output_tedge--;
					output_bedge--;
					curs_x = 0;
					break;
				}
				//if cursor is at the top edge, with nothing more to display, nothing happens.
				else if (curs_y == 0 && output_tedge == 0)
				{
					break;
				}
				//in all other cases, the cursor moves upward in the window.
				else
				{
					curs_y--;
					curs_x = 0;
					break;
				}


			case (KEY_DOWN):
				//if cursor is at the bottom edge, with more to display, the text shifts upward and the cursor doesn't move vertically. 
				if (curs_y == win_rows - 1 && output_bedge < buffer.size() - 1)
				{
					curs_x = 0;
					output_tedge++;
					output_bedge++;
					break;
				}
				//if cursor is at the bottom edge, with nothing more to display, nothing happens
				else if (curs_y == win_rows - 1 && output_bedge == buffer.size() - 1)
				{
					break;
				}
				else if (curs_y == buffer.size() - 1)
				{
					break;
				}
				//in all other cases, the cursor moves downward in the window
				else if (vector_y < buffer.size())
				{
					curs_x = 0;
					curs_y++;
					break;
				}

				///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
			case BACKSPACE:


				if (vector_x == 0 && vector_y > 0)
				{
					buffer.erase(buffer.begin() + curs_y - 1);
					curs_y--;
					break;
				}
				else if (vector_x == 0 && vector_y == 0)
				{
					break;
				}
				else
				{
					buffer[vector_y].erase(buffer[vector_y].begin() + vector_x - 1);
					curs_x--;
					break;
				}


				break;

			case (NEWLINE):
				// if the cursor is at the bottom of the screen and the cursor is not at the last row of the buffer, then a new line is inserted.
				if (curs_y == win_rows - 1 && vector_y == buffer.size() - 1)
				{
					buffer.push_back(vector<int>{});
					output_tedge++;
					output_bedge++;
					output_ledge = win_fcol;
					output_redge = win_rows;
					curs_x = 0;
					curs_y++;
					break;
				}


				//in all other cases, the cursor moves downward in the window
				else if (vector_x == buffer[vector_y].size() - 1 && vector_y < buffer.size())
				{

					output_ledge = win_fcol;
					output_redge = win_rows;
					curs_x = 0;
					curs_y++;
					break;
				}

				else if (vector_x == buffer[vector_y].size() - 1 && vector_y == buffer.size())
				{
					buffer.push_back(vector<int>{});
					output_ledge = win_fcol;
					output_redge = win_rows;
					curs_x = 0;
					curs_y++;
					break;
				}
				else if (vector_x == 0)
				{
					buffer.insert(buffer.begin() + vector_y, vector<int>{});
					curs_x = 0;
					curs_y++;
				}
				else
				{
					buffer.insert(buffer.begin() + vector_y + 1, vector<int>{});
					curs_x = 0;
					curs_y++;
					break;
				}

				break;

			case (AUTO_COMPLETE):

				

				searchString = "";

				wordsFound.clear();			
			
				vector_x--;
				curs_x--;

				while (vector_x >= 0 && buffer[vector_y][vector_x] != ' ')
				{									
					stack.push(buffer[vector_y][vector_x]);							
					buffer[vector_y].erase(buffer[vector_y].begin() + vector_x);	
					vector_x--;
					curs_x--;
				} 
				vector_x++;
				curs_x++;

				while (stack.empty() == false)
				{
					searchString += stack.top();
					stack.pop();
				}				

				wordsFound = dictionary.search(searchString);

				for (int i = 0; i < wordsFound.size() && i < 4; i++)
				{
					string word = wordsFound[i];
					for (auto c : word)
					{
						stack.push(c);						
					}
					stack.push(' ');
				}

				while (stack.empty() == false)
				{
					
					buffer[vector_y].insert(buffer[vector_y].begin()+vector_x, int(stack.top()));
					curs_x++;
					
					stack.pop();
					
				}				
				
				
				break;



			case (COMPRESS):


				attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

				mvprintw(term_rows - 1, term_cols - 50, "Would you like to compress this file? Y or N");

				attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

				if (confirm())
				{
					
					if (inFile.is_open())
					{
						inFile.close();
					}

					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(term_rows - 1, term_cols - 50, "                                               ");

					attroff(COLOR_PAIR(TERMTEXT));

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					mvprintw(term_rows - 1, term_cols - 50, "What is the name and extension of this file?");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(0, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					refresh();

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					wmove(stdscr, 0, term_cols - 30);

					c = getch();

					while (c != 10)
					{
						filename.push_back(c);
						addch(c);
						c = getch();
					}		

					compFilename = filename + ".compressed.txt";

					strncpy_s(fname, compFilename.c_str(), sizeof(compFilename));


					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS						


					outFile.open(fname, std::ofstream::out | std::ofstream::trunc);


					if (!outFile.good())
					{
						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                                  ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

						mvprintw(term_rows - 1, term_cols - 50, "Your file has NOT been compressed! Press any key.");

						attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS	

						getch();

						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(term_rows - 1, term_cols - 50, "                                                  ");

						attroff(COLOR_PAIR(TERMTEXT));

						attron(COLOR_PAIR(TERMTEXT));

						mvprintw(0, term_cols - 50, "                                           ");

						attroff(COLOR_PAIR(TERMTEXT));

						memset(fname, 0, sizeof(fname));

						

						refresh();
						filename.clear();
						break;
					}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
					else if (outFile.good())

					{
						//this goes through the main_window buffer and finds all words made of
						//a-z or A-Z characters (with all other characters serving as delimiters of words)
						//and adds those words and their frequencies to an unordered map<string, int>
						for (auto row : buffer)
						{
							for (auto ch : row)
							{
								if (ch < 91 && ch >64 || ch > 96 && ch < 123)
								{
									word += ch;
								}
								else 
								{
									if (word.size() > 0)
									{
										wordFreq[word]++;
									}
									word = "";
								}
							}
							if (word.size() > 0 )
							{
								wordFreq[word]++;
								word = "";
							}

						}

						//this takes all of the words from the frequency map above
						//and puts them into a maxHeap<string, int> with the most
						//frequently occurring word at the top of the heap(priority queue).
						for (auto pair : wordFreq)
						{
							maxHeap.push(pair);
						}

						//this iterates through all pairs of strings and ints within the maxHeap
						//above and converts the position in the heap to a binary representation
						//which will be used later on as substitutes for those words incompressed file.
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						orderOffHeap = 0;

						
						while (maxHeap.empty() == false)
						{
							item = maxHeap.top().first;

							compKey[item] = intToBinary(orderOffHeap);

							orderOffHeap++;
							maxHeap.pop();

						}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

						//this goes through the main_window buffer and transfers all
						//non-words into the compBuffer and when a word is encountered,
						//that word is replaced with its binary representation created above.
						compBuffer.clear();
						compBuffer.push_back(vector<int>{});						

						for (auto row : buffer)
						{
							wordToReplace = "";

							for (auto ch : row)
							{
								if (ch == NEWLINE)
								{
									compBuffer[comp_y].push_back('/n');
									compBuffer.push_back(vector<int>{});
									comp_y++;
								}

								else if (ch > 64 && ch < 91 || ch > 96 && ch < 123)
								{
									wordToReplace.push_back(ch);
								}

								else if (ch < 65 || ch > 91 || ch < 97 || ch > 122)
								{

									if (wordToReplace.size() > 0)
									{
										for (auto num : compKey[wordToReplace])
										{
											compBuffer[comp_y].push_back(char(num));																				
										}

									}

									wordToReplace = "";

									compBuffer[comp_y].push_back(char(ch));								
									
								}
								//if (wordToReplace.size() > 0)
								//{

								//}
							}
						}


						for (auto row : compBuffer)
						{
							for (auto ch : row)
							{
								outFile.put((ch));
							}
							outFile.put(10);
						}

						outFile.close();

						if (inFile.is_open())
						{
							inFile.close();
						}

						codeFilename = filename + ".codes.txt";

						strncpy_s(fname, codeFilename.c_str(), sizeof(codeFilename));
						outFile.open(fname, std::ofstream::out | std::ofstream::trunc);

						if (outFile.good())
						{
							for (auto i = compKey.begin(); i != compKey.end(); i++)
							{	
								for (auto value : i->second)
								{
									outFile.put(value);
								}		
								
								outFile.put(' ');
								outFile.put('=');
								outFile.put(' ');

								for (auto key: i->first)
								{
									outFile.put(key);
								}
								outFile.put(NEWLINE);
							}
						}

						outFile.close();
					}

						if (!outFile.is_open())
						{
							attron(COLOR_PAIR(TERMTEXT));

							mvprintw(term_rows - 1, term_cols - 50, "                                              ");

							attroff(COLOR_PAIR(TERMTEXT));

							attron(COLOR_PAIR(PROMPTCOLORS));

							//exit prompt print
							mvprintw(term_rows - 1, term_cols - 50, "Your file has been compressed! Press any key.");

							attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

							refresh();

							getch();
						}
						else
						{
							attron(COLOR_PAIR(TERMTEXT));

							mvprintw(term_rows - 1, term_cols - 50, "                                              ");

							attroff(COLOR_PAIR(TERMTEXT));

							attron(COLOR_PAIR(PROMPTCOLORS));

							//exit prompt print
							mvprintw(term_rows - 1, term_cols - 50, "An unknown error has occurred. Press any key.");

							attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

							refresh();

							getch();
						}

					}


				
				else
				{
					attron(COLOR_PAIR(TERMTEXT));

					mvprintw(term_rows - 1, term_cols - 50, "                                           ");

					attroff(COLOR_PAIR(TERMTEXT));

					attron(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR FOR PROMPT

					mvprintw(term_rows - 1, term_cols - 50, "File saving cancelled. Press any key.");

					attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

					getch();

				}




				attron(COLOR_PAIR(TERMTEXT));

				mvprintw(term_rows - 1, term_cols - 50, "                                               ");

				attroff(COLOR_PAIR(TERMTEXT));
				
				compFilename.clear();

				codeFilename.clear();

				filename.clear();

				orderOffHeap = 0;

				refresh();

/***
				for (auto row : buffer)
				{
					for (auto ch : row)
					{
						if (ch < 91 && ch >64 || ch > 96 && ch < 123)
						{
							word += ch;
						}
						else
						{
							if (word.size() > 0)
							{
								wordFreq[word]++;
								
							}
							word = "";
						}
					}
				}

				for (auto pair : wordFreq)
				{
					maxHeap.push(pair);
				}

				
				while (maxHeap.empty() == false)
				{
					string item = maxHeap.top().first;
					int freq = maxHeap.top().second;
					orderOffHeap = int(maxHeap.size());


					while (orderOffHeap > 0)
					{
						//stack.push(orderOffHeap&1);
						compKey[item] += orderOffHeap&1;
						orderOffHeap = orderOffHeap >> 1;						
					}
					
					//while (stack.empty() == false)
					//{
					//	compKey[word] += stack.top();
					//	stack.pop();
					//}
					
					maxHeap.pop();

				}



				for (auto row : buffer)
				{
					for (auto ch : row)
					{
						string wordToReplace = "";

						if (ch < 91 && ch >64 || ch > 96 && ch < 123)
						{
							wordToReplace += ch;
						}
						else if (ch == NEWLINE)
						{

						}
						else
						{
							if (wordToReplace.size() > 0)
							{
								for(auto ch : compKey[word])
								{

									buffer[vector_y].insert(buffer[vector_y].begin() + vector_x, int(ch));
									curs_x++;
									printBuffer(main_window, buffer, output_tedge, output_bedge, output_ledge, output_redge);
								}

							}
							wordToReplace = "";
						}
					}
				}

			






**/




				break;




			default:


				row = buffer.begin();

				col = buffer[vector_y].begin();

				//if cursor is at the right edge and there is more to display, the character is inserted and the text moves leftward and the cursor doesn't move.
				if (curs_x == win_cols - 1 && vector_x < buffer[vector_y].size())
				{

					buffer[vector_y].insert(col + vector_x, input);
					output_redge++;
					output_ledge++;
					break;
				}
				//if the cursor is at the right edge and there is nothing more to display, the new character gets added to the end of the vector and the text shifts leftward.
				else if (curs_x == win_cols - 1 && vector_x == buffer[vector_y].size())
				{
					buffer[vector_y].push_back(input);
					output_redge++;
					output_ledge++;
					break;
				}
				//in all other cases, the input goes onto the screen and the cursor moves rightward.
				else if (curs_x < win_cols - 1 && vector_x < buffer[vector_y].size())
				{
					buffer[vector_y].insert(col + vector_x, input);
					curs_x++;
					break;
				}
				else if (curs_x < win_cols && vector_x == buffer[vector_y].size())
				{
					buffer[vector_y].push_back(input);
					curs_x++;
					break;
				}



			}



			printBuffer(main_window, buffer, output_tedge, output_bedge, output_ledge, output_redge);


			wmove(main_window, curs_y, curs_x);

			wrefresh(main_window);

			refresh();
		}



		//changes color for prompt
		attron(COLOR_PAIR(PROMPTCOLORS));


		//any key to exit prompt
		mvprintw(term_rows - 1, term_cols - 50, "Press any key to escape");


		attroff(COLOR_PAIR(PROMPTCOLORS)); //CHANGES COLOR BACK TO WINCOLORS

		wrefresh(main_window);
		wgetch(main_window);
		endwin();
}






bool confirm(void) {
	char answer = getch();
	switch (answer) {

	case 'y':
		return true;
		break;
	case 'Y':
		return true;
		break;

	case 'n':
		return false;
		break;

	case 'N':
		return false;
		break;

	default:
		confirm();
		break;
	}

}

void windowResize(WINDOW* win, int rows, int cols) {

	resize_window(win, (rows - 4), (cols - 4));

}

void printBuffer(WINDOW* win, vector<vector <int> > buff, int frow, int lrow, int fcol, int lcol)
{
	wclear(win);
	
	int scrn_y = 0;

	int scrn_x = 0;
	
	for (int i = frow; i < buff.size() && i < lrow; i++)
	{
		if (buff[i].size() > 0)
		{
			for (int j = fcol; j < buff[i].size() && j < lcol; j++)
			{
				mvwaddch(win, scrn_y, scrn_x, char(buff[i][j]));
				scrn_x++;
			}

		}
		
		scrn_x = 0;
		scrn_y++;
	}
	
}

string intToBinary(int some_number)
{
	//below code doesn't work for zero
	if (some_number == 0)
	{
		return "0";
	}

	stack<uint8_t> bits{};
	while (some_number > 0)
	{
		bits.push(some_number & 1);
		some_number = some_number >> 1;
	}
	ostringstream result;
	while (bits.empty() == false)
	{
		result << (short)bits.top();
		bits.pop();
	}
	return result.str();
}


