#include<curses.h>
#include<time.h>
#include<stdlib.h>
#include<unistd.h>

//compile with -lncurses

int main(int argc, char *argv[])
{
	char* givenPhrase;
	char* border;
	char currentGuess;
	int   currentIndex;
	int   numOfChars = 0;
	int   row, col;
	struct timespec startTime, curTime;


	initscr();				//start curses mode

	getmaxyx(stdscr, row, col);

	clock_gettime(CLOCK_REALTIME, &startTime);		//record start time

	//count the characters
	for(int i = 1; i < argc; i++)
	{
		for(int j = 0; argv[i][j] != 0; j++)
		{
			numOfChars++;
		}
		numOfChars++;
	}


	//make the givenPhrase string and concatenate all the args into it
	givenPhrase = malloc(numOfChars * sizeof(char));
	currentIndex = 0;

	for(int i = 1; i < argc; i++)
	{
		for(int j = 0; argv[i][j] != 0; j++)
		{
			givenPhrase[currentIndex] = argv[i][j];
			currentIndex++;
		}

		if(i != argc - 1)
		{
			givenPhrase[currentIndex] = ' ';
			currentIndex++;
		}
		else
			givenPhrase[currentIndex] = 0;
	}

	//print border
	start_color();

	border = malloc((numOfChars + 10) * sizeof(char));
	border[0] = '+';
	border[numOfChars + 9] = 0;
	border[numOfChars + 8] = '+';

	for(int i = 1; i < numOfChars+8; i++)
		border[i] = '-';

	init_pair(3,COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(3));
	mvprintw((row/2) - 4, (col/2)-(numOfChars/2 + 5), "%s", border);
	mvprintw((row/2) + 4, (col/2)-(numOfChars/2 + 5), "%s", border);

	for(int i = 0; i < 7; i++)
	{
		mvprintw((row/2) - 3 + i, (col/2)-(numOfChars/2 + 5), "|");
		mvprintw((row/2) - 3 + i, (col/2)+(numOfChars/2 + 4), "|");
	}
	attroff(COLOR_PAIR(3));

	//print the givenPhrase to screen
	init_pair(4, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(4));
	mvprintw((row/2) - 2, ((col/2)-(numOfChars/2)), "%s", givenPhrase);
	attroff(COLOR_PAIR(4));

	//seed random
	srand(time(0));

	//start guessing

	init_pair(1,COLOR_RED, COLOR_BLACK);
	init_pair(2,COLOR_GREEN, COLOR_BLACK);
	for(int i = 0; givenPhrase[i] != 0; i++)
	{
		//guess one letter at a time
		while(currentGuess != givenPhrase[i])
		{
			currentGuess = (' ' + (rand() % (127 - ' ')));
			usleep(1000);
			attron(COLOR_PAIR(1));
			mvprintw(row/2, (col/2) - (numOfChars/2) + i, "%c", currentGuess);
			attroff(COLOR_PAIR(1));
			clock_gettime(CLOCK_REALTIME,&curTime);
			mvprintw((row/2) + 2 , (col/2) - 2, "%-6.2f", (curTime.tv_nsec - startTime.tv_nsec)/1000000000.00 + curTime.tv_sec - startTime.tv_sec);
			refresh();
		}
			attron(COLOR_PAIR(2));
			mvprintw(row/2, (col/2) - (numOfChars/2) + i, "%c", currentGuess);
			attroff(COLOR_PAIR(2));
	}

	mvprintw(row/2, (col/2) - (numOfChars/2) + numOfChars - 1, " ");

	free(givenPhrase);

	//time(NULL) - startTime;

	getch();
	endwin();						//end curses mode

	return 0;
}
