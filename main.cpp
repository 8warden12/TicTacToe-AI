#include <stdio.h>
#include "defs.h"
#include <cstdlib>
#include <vector>
using namespace std;
int games = 0;
int streak = 0;
int gl = 0;
int gw = 0;
int draws = 0;
char tboard[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' ',};

struct position{
	int value = 0;
	char view[9] = {' ',' ',' ',' ',' ',' ',' ',' ',' '};
};

vector<position*> allPositions;
vector<position*> database;
vector<position*> candidates;
vector<position*> currentGame;

position *newPosition()
{
	position *p = new position;
	allPositions.push_back(p);
	return p;
}



void garbageCollection()
{		
	for(int i = 0; i<allPositions.size(); i++)
	{
		free(allPositions[i]);
	}
	allPositions.clear();
}

void storeNodes()
{
	FILE *f = fopen("database.txt","w");
	for(int i = 0; i<database.size(); i++)
	{
		fprintf(f,"%d",database[i]->value);
		for(int x = 0; x<9; x++)
		{
			if(database[i]->view[x] == ' ')
			{
				fprintf(f,"-");
			}else{
				fprintf(f,"%c",database[i]->view[x]);
			}
			
		}
		fprintf(f,"\n");
	}
	
/*	for(int i = 0; i<database.size(); i++)
	{
		for(int x = 0; x<9; x++)
		{
			if(database[i]->view[x] == ' ')
			{
				printf("-");
			}else{
				printf("%c",database[i]->view[x]);
			}
			
		}
		printf(" %d\n",database[i]->value);
	}*/
	fprintf(f,"#\n");
	fprintf(f,"%d %d %d %d %d",games,gl,gw,draws,streak);
	fclose(f);
}

char tchar = ' ';
int tint;
void loadNodes()
{
	FILE *f = fopen("database.txt","r");
	
	for(;;)
	{	
		position *cepos = new position;
		fscanf(f,"%d",&tint);
		//printf("tint is: %d\n",tint);
		cepos->value = tint;
		for(int x = 0; x<9; x++)
		{
			fscanf(f,"%c",&tchar);
		//	printf("scanning a %c\n",tchar);
			if(tchar == '-')
			{
				tchar = ' ';
			}else if(tchar == '#')
			{
				goto dd;
			}
			
			cepos->view[x] = tchar;
		}
		
		database.push_back(cepos);
		allPositions.push_back(cepos);
		fseek(f,1,SEEK_CUR);
	}
	dd:
	fscanf(f,"%d %d %d %d %d",&games,&gl,&gw,&draws,&streak);
	fclose(f);

	
}

int sv;
bool subtract(char a[9])
{
	sv = 0;
	for(int i = 0; i<9; i++)
	{
		if(a[i] != board[i])
		{
			if(board[i] == ' ' && a[i] == 'X')
			{
				sv++;
			}else{
				return false;
			}
		}
	}
	if(sv == 1)
	{
		return true;
	}else{
		return false;
	}
	
}

bool cnb0()
{
	for(int i = 0; i<candidates.size(); i++)
	{
		if(candidates[i]->value > -1)
		{
			return false;
		}
	}
	return true;
}

void Xadd(char a[9])
{
	for(int i = 0; i<9; i++)
	{
		if(tboard[i] == ' ' && a[i] == 'X')
		{
			tboard[i] = a[i];
		}
	}
}

bool filled(char a[9])
{
	for(int i = 0; i<9; i++)
	{
		if(a[i] == ' ')
		{
			return false;
		}
	}
	
	return true;
}


void process()
{	
	hc = 0;
	highest = 0;
	found = false;
	candidates.clear();
	for(int i = 0; i<database.size(); i++)
	{
		if(subtract(database[i]->view))
		{
			candidates.push_back(database[i]);
		}
	}
	
	if(candidates.empty())
	{	
		printf("Candidates are empty.\n");
		for(int i = 0; i<9; i++)
		{
			if(board[i] == ' ')
			{

				board[i] = 'X';
				break;
			}
		}
		
		position *p = newPosition();
		for(int i = 0; i<9; i++)
		{			
			p->view[i] = board[i];
		}
		currentGame.push_back(p);
		database.push_back(p);
		printf("PROCESS: Creating new position p and assigning it to CURRENT\n");
		
	}else if(cnb0()){
		
			printf("in cnb0\n");
			
			for(int i = 0; i<9; i++)
			{
				tboard[i] = ' ';
			}
			
			
			for(int i = 0; i<candidates.size(); i++)
			{
				Xadd(candidates[i]->view);
			}
		
			
			if(filled(tboard))
			{
				tboard:
				printf("PROCESS: tboard is filled.\n");
				highest = 0;
				for(int i = 0; i<candidates.size(); i++)
				{
					if(candidates[i]->value < highest)
					{
						highest = candidates[i]->value;
					}
				}
				
				highest--;
				
				for(int i = 0; i<candidates.size(); i++)
				{
					if(candidates[i]->value > highest)
					{
						highest = candidates[i]->value;
						hc = i;
					}
				}
				
				for(int i = 0; i<9; i++)
				{
					board[i] = candidates[hc]->view[i];
				}
				
				currentGame.push_back(candidates[hc]);
				
				
			}else{
				
				found = false;
			printf("PROCESS: tboard is not filled up. Going in next unoccupied square.\n");
				for(int i = 0; i<9; i++)
				{
					printf("sProcess: tboard %d is '%c'\n",i,tboard[i]);
					if(tboard[i] == ' ' && board[i] == ' ')
					{
						found = true;
						board[i] = 'X';
						position *p = newPosition();
						for(int x = 0; x<9; x++)
						{
							p->view[x] = board[x];
						}
						
						database.push_back(p);
						
						currentGame.push_back(p);
						
						
						break;
					}
				}
				if(!found)
				{
					goto tboard;
				}
				
			}
		
	
		
		
		
	}else{
		
		printf("PROCESS: A POSITION HAS A VALUE HIGHER THAN 0!!!1\n");
		candidates.clear();
		highest = -1;
		hc = 0;
		
		for(int i = 0; i<database.size(); i++)
		{
			if(subtract(database[i]->view))
			{
				candidates.push_back(database[i]);
			}
		}
		
		for(int x = 0; x<candidates.size(); x++)
		{
			if(candidates[x]->value > highest)
			{
				found = true;
				highest = candidates[x]->value;
				hc = x;
			}
		}
					
			
			
	
			
			
			for(int i = 0; i<9; i++)
			{
				board[i] = candidates[hc]->view[i];
			}
			
			currentGame.push_back(candidates[hc]);
		}
	}
	
	
void getUserMove()
{		
	hc = 0;
	highest = 0;
	found = false;

		woo:
		printf("\n012\n345\n678\nChoose:");
		fseek(stdin,0,SEEK_END);
		scanf("%d",&choice);
		
		
		if(board[choice] == ' ')
		{
			board[choice] = 'O';	
		
		}else{
			printf("asdasd");
			goto woo;
		}
		
	
	
}

void display()
{	
		printf("Total games: %d\nGames lost: %d\nGames won: %d\nGames Drawn: %d\nStreak: %d\n",games,gl,gw,draws,streak);
		for(int i = 0; i<9; i++)
		{
			if(i == 3 || i == 6)
			{
				printf("\n");
			}
			printf("%c",board[i]);
		}
		
		printf("\n-----------------\n");	
		

}

bool draw()
{
	for(int i = 0; i<9; i++)
	{
		if(board[i] == ' ')
		{
			return false;
		}
	}
	return true;
}

void detectWin()//1 = pc V 2 = h V
{
	if(equ(board,wp1) || equ(board,wp2) || equ(board,wp3) || equ(board,wp4) || equ(board,wp5) || equ(board,wp6) || equ(board,wp7) || equ(board,wp8))	
	{
		gameOutcome = 1;
	}else if(equ(board,wp21) || equ(board,wp22) || equ(board,wp23) || equ(board,wp24) || equ(board,wp25) || equ(board,wp26) || equ(board,wp27) || equ(board,wp28))	
	{
		gameOutcome = 2;
	}else if(draw())
	{

		gameOutcome = 3;
		
	}
	
	
	

}


int main()
{

	loadNodes();
	reset:
	while(true)
	{
		//system("cls");
		display();
		process();
		detectWin();
		if(gameOutcome == 1 || gameOutcome == 2 || gameOutcome == 3)
		{
			running = false;
			break;
		}
	
		display();
		getUserMove();
		detectWin();
		if(gameOutcome == 2 || gameOutcome == 1 || gameOutcome == 3)
		{
			running = false;
			break;
		}
	
	}
	
	games++;
	display();

		
	
		

		
		
		
		
		if(gameOutcome == 2)
		{
			streak = 0;
			gw++;
			FILE *f = fopen("record.txt","a");
			
			fprintf(f,"2");
			fclose(f);
			f = NULL;
			
			for(int i = 0; i<currentGame.size(); i++)
			{
				currentGame[i]->value = currentGame[i]->value - 1;
				printf("%d",currentGame[i]->value);
			} 
			printf("you won. Play again?\n");
			scanf(" %c",&ac);
			
		//	printf("you won. Play again?\n");
			
		}else if(gameOutcome == 1 || gameOutcome == 3)	
		{	
			
			streak++;
			if(gameOutcome == 3)
			{
				FILE *f = fopen("record.txt","a");
				fprintf(f,"3");
				fclose(f);
				f = NULL;
				draws++;
				for(int i = 0; i<currentGame.size(); i++)
				{
					currentGame[i]->value = currentGame[i]->value + 4;
					
				}
				printf("draw. Play again?\n");
				scanf(" %c",&ac);
				
				
				//	printf("draw. Play again?\n");
			}
			
			if(gameOutcome == 1)
			{
				gl++;
				for(int i = 0; i<currentGame.size(); i++)
				{
					currentGame[i]->value = currentGame[i]->value + 4;
				}
				
				printf("you lost. Play again?\n");
				scanf(" %c",&ac);
				FILE *f = fopen("record.txt","a");
				fprintf(f,"1");
				fclose(f);
				f = NULL;

			//	printf("you lost. Play again?\n");
			}
		}
		currentGame.clear();
		if(streak > 89)
		{
			system("cls");
			printf("The program has successfully reached the rank of proficient, after %d games.\n",games);
			goto br;
		}
		gameOutcome = 20398;
		for(int i = 0; i<9; i++)
		{
			board[i] = ' ';
		}
		
		if(ac != 'y')
		{
			goto br;
		}
		
		goto reset;

	br:
		storeNodes();
	garbageCollection();
	
}






