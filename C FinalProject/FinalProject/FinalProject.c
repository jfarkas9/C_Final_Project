//*****************************
//Final Project
//Josh's Aquarium Chemistry Tracker
// Written by: Josh Farkas
//Date: Dec 01, 2013
//Purpose: Input, record, and output aquarium water test results.
//***************************** 
// ascii images borrowed from http://www.chris.com/ascii/index.php?art=animals/fish
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#define pause system("pause")
#define cls system("cls")
#define flush fflush(stdin)
#define line printf("\n")
#define STATNUM 36 //How many results to store, currently three years

typedef struct{ //Struct for date data
	int day;
	int month;
	int year;	
} DATE;

typedef struct{ //Struct for water test variables
	DATE date;
	double temp;
	double pH;
	double ammonia;
	double nitrites;
	double nitrates;
	int ifNull; //tells if there is any data within current position of struct array. null=0, not null = 1
} STATS;

void scanStats(STATS[], int*); //scans data from bin file into struct array stats
void displayMenu(STATS[], int*); //displays main menu
void newTestResult(STATS[], int*); //recieves results from user
void clearData(STATS[], int*); //deletes data within stat array
void displayResults(STATS[], int*); //displays results. uses viewchange and graphresults
void viewChange(STATS[], int*); //finds highest, lowest, average across all variables
void graphResults(STATS[], int*); //visual representation of data

main(){
	FILE *file; 
	int c, counter=0; //counter is referenced later as *c
	STATS stats[STATNUM]; //struct array for stats
	scanStats(stats, &counter); //scans data from bin file into struct array stats
	//Splash Screen
		file = fopen("splash.txt", "r");
		if (file) {  //pulls ascii image from file
			while ((c = getc(file)) != EOF)
				putchar(c);
		fclose(file);
		}//end if
		if(file== NULL) //if no file, display text
			printf("Welcome to Josh's Aquarium Chemistry Tracker");
		line;
		pause;
	//end splash screen
	displayMenu(stats, &counter); //displays main menu
} //end of main

void displayMenu(STATS stats[], int *c){ //displays main menu
	FILE *file;
	int x, choice;
	do{
		cls;
		//Splash Screen
			file = fopen("menu.txt", "r");
			if (file) { //yes, another wonderful ascii image. I have more for your enjoyment
				while ((x = getc(file)) != EOF)
					putchar(x);
				fclose(file);
			}//end if
			if(file== NULL) 
				printf("Menu:\n");
		//end splash screen
		printf("1. Enter New Results\n");
		printf("2. View Results\n");
		printf("3. Clear Stored Results\n");
		printf("4. Quit\n");
		printf("Enter Choice: ");
		scanf("%i", &choice);

		switch(choice){
			case 1: //enter new result
				newTestResult(stats, c); 
			break;
			
			case 2: //view results
				displayResults(stats, c);
			break;
			
			case 3: //clear stored results
				clearData(stats, c);
			break;
		}//end switch
	}while (choice != 4); 
}//end displayMenu

void newTestResult(STATS stats[], int *c){//recieves results from user
	FILE *fP;
	FILE *file;	
	int x, day, month, year;
	double pH, temp, ammonia, nitri, nitra;
	cls;
	//Splash Screen
		file = fopen("newresults.txt", "r");
		if (file) {
			while ((x = getc(file)) != EOF)
				putchar(x);
		fclose(file);
		}//end if
		if(file== NULL) 
			printf("Enter Results:\n");
		line; 
	//end splash screen
	printf("Please enter the");
	printf("\n\nDay:"); //this would be a great place for an input check
	scanf("%i", &day);
	printf("Month: ");
	scanf("%i", &month);
	printf("Year(xx): ");
	scanf("%i", &year);
	printf("Temperature: ");
	scanf("%lf", &temp);
	printf("pH: ");
	scanf("%lf", &pH);
	printf("Ammonia level: ");
	scanf("%lf", &ammonia);
	printf("Nitrites level: ");
	scanf("%lf", &nitri);
	printf("Nitrates level: ");
	scanf("%lf", &nitra);

	//add new stats to array
	stats[(*c)].date.day+=day;
	stats[(*c)].date.month+=month;
	stats[(*c)].date.year+=year;
	stats[(*c)].temp+=temp;
	stats[(*c)].pH+=pH;
	stats[(*c)].ammonia+=ammonia;
	stats[(*c)].nitrites+=nitri;
	stats[(*c)].nitrates+=nitra;
	stats[(*c)].ifNull = 1; //file no longer null
	
	fP=fopen("file.bin", "wb");
	fwrite(stats, sizeof(STATS), STATNUM, fP); //write array to bin file

	if(stats[1].ifNull==1){ //if this is the second entry or later, the previus stats can be compaired
		cls;
		printf("Results:    This  Previus Change\n"); //outputs results just entered, previus results, then percentage difference between stats
		printf("Temperature %.2lf  %.2lf    ", temp, stats[(*c)-1].temp);
			if(temp*100/stats[(*c)-1].temp >100){printf("Increase %.2lf%%\n", temp*100/stats[(*c)-1].temp-100); } //display an increase of percentage
			if(temp*100/stats[(*c)-1].temp <100){printf("Decrease %.2lf%%\n", temp*100/stats[(*c)-1].temp); } //display a decrease of percentage
			if(temp*100/stats[(*c)-1].temp==100){printf("None\n");} //no change between results
		printf("pH          %.2lf  %.2lf    ", pH, stats[(*c)-1].pH);
			if(pH*100/stats[(*c)-1].pH >100){printf("Increase %.2lf%%\n", pH*100/stats[(*c)-1].pH-100);}
			if(pH*100/stats[(*c)-1].pH <100){printf("Decrease %.2lf%%\n", pH*100/stats[(*c)-1].pH); }
			if(pH*100/stats[(*c)-1].pH==100){printf("None\n");}
		printf("Ammonia     %.2lf  %.2lf    ", ammonia, stats[(*c)-1].ammonia);
			if(ammonia*100/stats[(*c)-1].ammonia >100){printf("Increase %.2lf%%\n", ammonia*100/stats[(*c)-1].ammonia-100);}
			if(ammonia*100/stats[(*c)-1].ammonia <100){printf("Decrease %.2lf%%\n", ammonia*100/stats[(*c)-1].ammonia); }
			if(ammonia*100/stats[(*c)-1].ammonia==100){printf("None\n");}
		printf("Nitrites    %.2lf  %.2lf    ", nitri, stats[(*c)-1].nitrites);
			if(nitri*100/stats[(*c)-1].nitrites >100){printf("Increase %.2lf%%\n", nitri*100/stats[(*c)-1].nitrites-100);}
			if(nitri*100/stats[(*c)-1].nitrites <100){printf("Decrease %.2lf%%\n", nitri*100/stats[(*c)-1].nitrites); }
			if(nitri*100/stats[(*c)-1].nitrites==100){printf("None\n");}
		printf("Nitrates    %.2lf  %.2lf    ", nitra, stats[(*c)-1].nitrates);
			if(nitra*100/stats[(*c)-1].nitrates >100){printf("Increase %.2lf%%\n", nitra*100/stats[(*c)-1].nitrates-100);}
			if(nitra*100/stats[(*c)-1].nitrates <100){printf("Decrease %.2lf%%\n", nitra*100/stats[(*c)-1].nitrates); }
			if(nitra*100/stats[(*c)-1].nitrates==100){printf("None\n");}
		pause;
	}//end if
	(*c)++;//incruments counter
}//end newTestResult

void displayResults(STATS stats[], int *c){
	int i, choice;
	cls;
	if(stats[0].ifNull==1){ //if there are no stats logged into the array, the results menu will not display
		printf("Choose from the following:\n");
		printf("1. View List\n");
		printf("2. View Change\n");
		printf("3. View Graph\n");
		printf("4. Previus Menu\n");
		printf("Enter Choice: ");
		scanf("%i", &choice);

		switch(choice){
			case 1: //list results
				cls;
				printf("Date      Temp   pH     Ammonia   Nitrites Nitrates\n");
					for(i=0; i<STATNUM; i++){ //statnum or counter
						if(stats[i].ifNull==1)
							printf("%02i/%02i/%02i  %.2lf   %.2lf   %.2lf      %.2lf     %.2lf \n", stats[i].date.day, stats[i].date.month, stats[i].date.year, stats[i].temp, stats[i].pH, stats[i].ammonia, stats[i].nitrites, stats[i].nitrates );
					}//end for
				pause;
			break;
			
			case 2: //view change
				viewChange(stats, c);
			break;
			
			case 3: //graph results
				graphResults(stats, c);
			break;

		}//end switch
	}else{
		printf("No Results Found\n"); //displays that there are no results logged
		pause;
		}//end if/else 
}//end displayResults

void scanStats(STATS stats[], int *c){ //scans data from bin file into struct array stats
	FILE *fP;
	int i;

	for(i=0; i<STATNUM; i++){ //clears junk from array....... probably would have been best to just clear the ifnull
		stats[i].date.day = 0;
		stats[i].date.month = 0;
		stats[i].date.year = 0;
		stats[i].temp = 0;
		stats[i].pH = 0;
		stats[i].ammonia = 0;
		stats[i].nitrites = 0;
		stats[i].nitrates = 0;
		stats[i].ifNull = 0;
	}//end for
	
	fP=fopen("file.bin", "rb");
	if(fP == NULL){ //if file is unavailable
		fopen("file.bin", "wb");
		fwrite(stats, sizeof(STATS), STATNUM, fP); //creates file, enters array into bin file
	}
	fread(stats, sizeof(STATS), STATNUM, fP); //reads array from file
	fclose(fP);

	for(i=0; i<STATNUM; i++){
		if(stats[i].ifNull != 0){
			(*c)++; //incruments counter
		}//end if
	}
}//end scanStats

void clearData(STATS stats[], int *c){  //deletes data within stat array
	FILE *fP;
	int i;
	char choice;
	cls;
	printf("Are you sure you want to clear the log? (Y/N): "); //confirms user choice
	scanf(" %c", &choice);
	choice=toupper(choice);
		if(choice=='Y'){
			(*c)=0;
			for(i=0; i<STATNUM; i++){ //clears array
				stats[i].date.day = 0;
				stats[i].date.month = 0;
				stats[i].date.year = 0;
				stats[i].temp = 0;
				stats[i].pH = 0;
				stats[i].ammonia = 0;
				stats[i].nitrites = 0;
				stats[i].nitrates = 0;
				stats[i].ifNull = 0;
			}//end for
			fP=fopen("file.bin", "wb");
			fwrite(stats, sizeof(STATS), STATNUM, fP); //writes array to log
			printf("\nLog cleared\n\n");
			pause;
		}else{
			printf("\nLog not cleared\n\n"); //anything other than a "y" entered will not clear the log
			pause;
		}//end if/else
}//end clear data

void viewChange(STATS stats[], int *c){ //finds highest, lowest, average across all variables
	int i, pass;
	double temp, tempH, tempL, tempA=0, pHH, pHL, pHA=0, ammonH, ammonL, ammonA=0, nitriH, nitriL, nitriA=0, nitraH, nitraL, nitraA=0; //there has got to be a simpler way to do this
	double tempAr[STATNUM]={0}; //temporary array for numbers
	cls;
				//Temp
					for (i=0; i<(*c); i++){ //coppies data over to temp array
						tempAr[i]=stats[i].temp;
						tempA+=tempAr[i];
					}//end for

					for(pass=1; pass <= (*c)-1; pass++){ //bubble sorts data
							for(i=0; i<(*c)-1; i++){
								if(tempAr[i] > tempAr[i+1]){
								temp=tempAr[i];
								tempAr[i]=tempAr[i+1];
								tempAr[i+1]= temp;
								}//end for
							}//end for
						}//end for pass
					tempH=tempAr[((*c)-1)]; //high of data
					tempL=tempAr[0]; //low of data
					tempA=tempA/(*c); //average of data

				//pH
					for (i=0; i<(*c); i++){
						tempAr[i]=stats[i].pH;
						pHA+=tempAr[i];
					}//end for

					for(pass=1; pass <= (*c)-1; pass++){
							for(i=0; i<(*c)-1; i++){
								if(tempAr[i] > tempAr[i+1]){
								temp=tempAr[i];
								tempAr[i]=tempAr[i+1];
								tempAr[i+1]= temp;
								}//end for
							}//end for
						}//end for pass
					pHH=tempAr[(*c)-1];
					pHL=tempAr[0];
					pHA=pHA/(*c);

				//Ammonia
					for (i=0; i<(*c); i++){
						tempAr[i]=stats[i].ammonia;
						ammonA+=tempAr[i];
					}//end for

					for(pass=1; pass <= (*c)-1; pass++){
							for(i=0; i<(*c)-1; i++){
								if(tempAr[i] > tempAr[i+1]){
								temp=tempAr[i];
								tempAr[i]=tempAr[i+1];
								tempAr[i+1]= temp;
								}//end for
							}//end for
						}//end for pass
					ammonH=tempAr[(*c)-1];
					ammonL=tempAr[0];
					ammonA=ammonA/(*c);

				//nitrites
					for (i=0; i<(*c); i++){
						tempAr[i]=stats[i].nitrites;
						nitriA+=tempAr[i];
					}//end for

					for(pass=1; pass <= (*c)-1; pass++){
							for(i=0; i<(*c)-1; i++){
								if(tempAr[i] > tempAr[i+1]){
								temp=tempAr[i];
								tempAr[i]=tempAr[i+1];
								tempAr[i+1]= temp;
								}//end for
							}//end for
						}//end for pass
					nitriH=tempAr[(*c)-1];
					nitriL=tempAr[0];
					nitriA=nitriA/(*c);

				//Nitrates
					for (i=0; i<(*c); i++){
						tempAr[i]=stats[i].nitrates;
						nitraA+=tempAr[i];
					}//end for

					for(pass=1; pass <= (*c)-1; pass++){
							for(i=0; i<(*c)-1; i++){
								if(tempAr[i] > tempAr[i+1]){
								temp=tempAr[i];
								tempAr[i]=tempAr[i+1];
								tempAr[i+1]= temp;
								}//end for
							}//end for
						}//end for pass
					nitraH=tempAr[(*c)-1];
					nitraL=tempAr[0];
					nitraA=nitraA/(*c);

				printf("Value         Highest Lowest Average\n");
				printf("--------------------------------------\n");
				printf("Temperature   %02.2lf   %02.2lf  %02.2lf\n", tempH, tempL, tempA);
				printf("pH            %02.2lf   %02.2lf  %02.2lf\n", pHH, pHL, pHA);
				printf("Ammonia       %02.2lf   %02.2lf  %02.2lf\n", ammonH, ammonL, ammonA);
				printf("Nitrites      %02.2lf   %02.2lf  %02.2lf\n", nitriH, nitriL, nitriA);
				printf("Nitrates      %02.2lf   %02.2lf  %02.2lf\n", nitraH, nitraL, nitraA);
				
				pause;
}//end viewChange

void graphResults(STATS stats[], int *c){ //visual representation of data
	int i, choice, pass;
	char string[5][20]; //string array
		//ooooooh looky, the string requirement for the assignment
	strcpy(string[0], "Temperature");
	strcpy(string[1], "pH");
	strcpy(string[2], "Ammonia");
	strcpy(string[3], "Nitrites");
	strcpy(string[4], "Nitrates");
	cls;
	printf("Choose from the following to graph:\n");
	for(i=0; i<5; i++)
		printf("%i.%s\n", i+1, string[i]); //prints variables 
	printf("Enter Choice: ");
	scanf("%i", &choice);
		
		switch(choice){
			case 1: //temperature
				cls;
				printf("%s:\n", string[0]);
				for(pass=0; pass<(*c); pass++){ //passes through array
					printf("%02i/%02i/%02i ", stats[pass].date.day, stats[pass].date.month, stats[pass].date.year); //prints date for entry
					for(i=0; i<(stats[pass].temp); i++){ //prints line of asterisks for the number
						printf("*");
					}
					line; //next line, next entry
					}
				line;
				pause;
			break;

			case 2: //pH
				cls;
				printf("%s:\n", string[1]);
				for(pass=0; pass<(*c); pass++){
					printf("%02i/%02i/%02i ", stats[pass].date.day, stats[pass].date.month, stats[pass].date.year);
					for(i=0; i<(stats[pass].pH); i++){
						printf("*");
					}
					line;
					}
				line;
				pause;
			break;

			case 3: //ammonia
				cls;
				printf("%s:\n", string[2]);
				for(pass=0; pass<(*c); pass++){
					printf("%02i/%02i/%02i ", stats[pass].date.day, stats[pass].date.month, stats[pass].date.year);
					for(i=0; i<(stats[pass].ammonia); i++){
						printf("*");
					}
					line;
					}
				line;
				pause;
			break;

			case 4: //nitrites
				cls;
				printf("%s:\n", string[3]);
				for(pass=0; pass<(*c); pass++){
					printf("%02i/%02i/%02i ", stats[pass].date.day, stats[pass].date.month, stats[pass].date.year);
					for(i=0; i<(stats[pass].nitrites); i++){
						printf("*");
					}
					line;
					}
				line;
				pause;
			break;

			case 5: //nitrates
				cls;
				printf("%s:\n", string[4]);
				for(pass=0; pass<(*c); pass++){
					printf("%02i/%02i/%02i ", stats[pass].date.day, stats[pass].date.month, stats[pass].date.year);
					for(i=0; i<(stats[pass].nitrates); i++){
						printf("*");
					}
					line;
					}
				line;
				pause;
			break;

			}//end switch
}//end graphresults
//Thanks for sitting through my mess of a program