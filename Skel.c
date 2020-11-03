/* Program to do "calculations" on numeric CSV data files.

   Skeleton program written by Alistair Moffat, ammoffat@unimelb.edu.au,
   September 2020, with the intention that it be modified by students
   to add functionality, as required by the assignment specification.

   Student Authorship Declaration:

   (1) I certify that except for the code provided in the initial skeleton
   file, the  program contained in this submission is completely my own
   individual work, except where explicitly noted by further comments that
   provide details otherwise.  I understand that work that has been developed
   by another student, or by me in collaboration with other students, or by
   non-students as a result of request, solicitation, or payment, may not be
   submitted for assessment in this subject.  I understand that submitting for
   assessment work developed by or in collaboration with other students or
   non-students constitutes Academic Misconduct, and may be penalized by mark
   deductions, or by other penalties determined via the University of
   Melbourne Academic Honesty Policy, as described at
   https://academicintegrity.unimelb.edu.au.

   (2) I also certify that I have not provided a copy of this work in either
   softcopy or hardcopy or any other form to any other student, and nor will I
   do so until after the marks are released. I understand that providing my
   work to other students, regardless of my intention or any undertakings made
   to me by that other student, is also Academic Misconduct.

   (3) I further understand that providing a copy of the assignment
   specification to any form of code authoring or assignment tutoring service,
   or drawing the attention of others to such services and code that may have
   been made available via such a service, may be regarded as Student General
   Misconduct (interfering with the teaching activities of the University
   and/or inciting others to commit Academic Misconduct).  I understand that
   an allegation of Student General Misconduct may arise regardless of whether
   or not I personally make use of such solutions or sought benefit from such
   actions.

   Signed by: [Enter your full name and student number here before submission]
   Dated:     [Enter the date that you "signed" the declaration]

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

/* these #defines provided as part of the initial skeleton */

#define MAXCOLS	20	/* maximum number of columns to be handled */
#define MAXROWS	999	/* maximum number of rows to be handled */
#define LABLEN  20	/* maximum length of each column header */
#define LINELEN 100	/* maximum length of command lines */

#define ERROR	(-1)	/* error return value from some functions */

#define O_NOC	'-'	/* the "do nothing" command */
#define O_IND	'i'	/* the "index" command */
#define O_ANA 	'a'	/* the "analyze" command */
#define O_DPY 	'd'	/* the "display" command */
#define O_PLT 	'p'	/* the "plot" command */
#define O_SRT 	's'	/* the "sort" command */

#define CH_COMMA ','	/* comma character */
#define CH_CR    '\r'	/* pesky CR character in DOS-format files */
#define CH_NL    '\n'	/* newline character */

/* if you wish to add further #defines, put them below this comment */

//This is for plot function, since our we need to divide our interval into 10 parts
#define INTERVAL_RANGE 10

/* and then, here are some types for you to work with */
typedef char head_t[LABLEN+1];

typedef double csv_t[MAXROWS][MAXCOLS];


/****************************************************************/

/* function prototypes */

void get_csv_data(csv_t D, head_t H[],  int *dr,  int *dc, int argc,
	char *argv[]);
void error_and_exit(char *msg);
void print_prompt(void);
int  get_command(int dc, int *command, int ccols[], int *nccols);
void handle_command(int command, int ccols[], int nccols,
			csv_t D, head_t H[], int dr, int dc);
void do_index(csv_t D, head_t H[], int dr, int dc,
		 int ccols[], int nccols);
/* add further function prototypes below here */

void do_analyze(csv_t D, head_t H[], int dr, int dc,
		 int ccols[], int nccols);
void do_display(csv_t D, head_t H[], int dr, int dc,
		 int ccols[], int nccols);
void do_sort(csv_t D, head_t H[], int dr, int dc,
		 int ccols[], int nccols);
void do_plot(csv_t D, head_t H[], int dr, int dc,
		 int ccols[], int nccols);		 
//Check if the specific column is sorted or not
//Used in do_analyze function if it returns 1
//that means we have to print median too else we do not
int is_sorted_col(csv_t D, int col, int dr);
/*Check if all ccols of two rows contain same data or not
If they contain same data it returns 1 
else it returns 0 
It is used in display function until a column
is found which have different data than previous column*/
/* This function whether previous two rows contains same data or 
not for specific number of columns, this function will be used in sort
functions to check whether the previos user mentioned columns
are in sorted position or not and then we change the value
of this column*/
int two_row_same(csv_t D, int ccols[], int first_row ,
 int second_row , int nccols);
 
/*Swap the data of first row with second row , both row index is
passed as argument to this function */
void change_data(csv_t D,int ccols[], int first_row ,
 int second_row , int nccols) ;
 
void assign(double assigner[] , double assigned[] , int dc); 

/****************************************************************/

/* main program controls all the action
*/
int
main(int argc, char *argv[]) {

	head_t H[MAXCOLS];	/* labels from the first row in csv file */
	csv_t D;		/* the csv data stored in a 2d matrix */
	int dr=0, dc=0;		/* number of rows and columns in csv file */
	int ccols[MAXCOLS];
	int nccols;
	int command;

	/* this next is a bit of magic code that you can ignore for
	   now, it reads csv data from a file named on the
	   commandline and saves it to D, H, dr, and dc
	   */
	get_csv_data(D, H, &dr, &dc, argc, argv);
	
	/* ok, all the input data has been read, safe now to start
	   processing commands against it */

	print_prompt();
	while (get_command(dc, &command, ccols, &nccols) != EOF) {
		handle_command(command, ccols, nccols,
			D, H, dr, dc);
		print_prompt();
	}

	/* all done, so pack up bat and ball and head home */
	printf("\nTa daa!!!\n");
	return 0;
}

/****************************************************************/

/* prints the prompt indicating ready for input
*/
void
print_prompt(void) {
	printf("> ");
}

/****************************************************************/

/* read a line of input into the array passed as argument
   returns false if there is no input available
   all whitespace characters are removed
   all arguments are checked for validity
   if no argumnets, the numbers 0..dc-1 are put into the array
*/
int get_command(int dc, int *command, int columns[], int *nccols) 
{
	int i=0, c, col=0;
	char line[LINELEN];
	/* comand is in first character position */
	if ((*command=getchar()) == EOF) {
		return EOF;
	}
	/* and now collect the rest of the line, integer by integer,
	   sometimes in C you just have to do things the hard way */
	while (((c=getchar())!=EOF) && (c!='\n')) {
		if (isdigit(c)) {
			/* digit contributes to a number */
			line[i++] = c;
		} else if (i!=0)  {
			/* reached end of a number */
			line[i] = '\0';
			columns[col++] = atoi(line);
			/* reset, to collect next number */
			i = 0;
		} else {
			/* just discard it */
		}
	}
	if (i>0) {
		/* reached end of the final number in input line */
		line[i] = '\0';
		columns[col++] = atoi(line);
	}

	if (col==0) 
	{
		/* no column numbers were provided, so generate them */
		for (i=0; i<dc; i++) {
			columns[i] = i;
		}
		*nccols = dc;
		return !EOF;
	}

	/* otherwise, check the one sthat were typed against dc,
	   the number of cols in the CSV data that was read */
	for (i=0; i<col; i++) {
		if (columns[i]<0 || columns[i]>=dc) {
			printf("%d is not between 0 and %d\n",
				columns[i], dc);
			/* and change to "do nothing" command */
			*command = O_NOC;
		}
	}
	/* all good */
	*nccols = col;
	return !EOF;
}

/****************************************************************/

/* this next is a bit of magic code that you can ignore for now
   and that will be covered later in the semester; it reads the
   input csv data from a file named on the commandline and saves
   it into an array of character strings (first line), and into a
   matrix of doubles (all other lines), using the types defined
   at the top of the program.  If you really do want to understand
   what is happening, you need to look at:
   -- The end of Chapter 7 for use of argc and argv
   -- Chapter 11 for file operations fopen(), and etc
*/
void
get_csv_data(csv_t D, head_t H[],  int *dr,  int *dc, int argc,
		char *argv[]) {
	FILE *fp;
	int rows=0, cols=0, c, len;
	double num;

	if (argc<2) {
		/* no filename specified */
		error_and_exit("no CSV file named on commandline");
	}
	if (argc>2) {
		/* confusion on command line */
		error_and_exit("too many arguments supplied");
	}
	if ((fp=fopen(argv[1], "r")) == NULL) {
		error_and_exit("cannot open CSV file");
	}

	/* ok, file exists and can be read, next up, first input
	   line will be all the headings, need to read them as
	   characters and build up the corresponding strings */
	len = 0;
	while ((c=fgetc(fp))!=EOF && (c!=CH_CR) && (c!=CH_NL)) {
		/* process one input character at a time */
		if (c==CH_COMMA) {
			/* previous heading is ended, close it off */
			H[cols][len] = '\0';
			/* and start a new heading */
			cols += 1;
			len = 0;
		} else {
			/* store the character */
			if (len==LABLEN) {
				error_and_exit("a csv heading is too long");
			}
			H[cols][len] = c;
			len++;
		}
	}
	/* and don't forget to close off the last string */
	H[cols][len] = '\0';
	*dc = cols+1;

	/* now to read all of the numbers in, assumption is that the input
	   data is properly formatted and error-free, and that every row
	   of data has a numeric value provided for every column */
	rows = cols = 0;
	while (fscanf(fp, "%lf", &num) == 1) {
		/* read a number, put it into the matrix */
		if (cols==*dc) {
			/* but first need to start a new row */
			cols = 0;
			rows += 1;
		}
		/* now ok to do the actual assignment... */
		D[rows][cols] = num;
		cols++;
		/* and consume the comma (or newline) that comes straight
		   after the number that was just read */
		fgetc(fp);
	}
	/* should be at last column of a row */
	if (cols != *dc) {
		error_and_exit("missing values in input");
	}
	/* and that's it, just a bit of tidying up required now  */
	*dr = rows+1;
	fclose(fp);
	printf("    csv data loaded from %s", argv[1]);
	printf(" (%d rows by %d cols)\n", *dr, *dc);
	return;
}
 
/****************************************************************/

void
error_and_exit(char *msg) {
	printf("Error: %s\n", msg);
	exit(EXIT_FAILURE);
}

/****************************************************************/

/* the 'i' index command
*/			
void
do_index(csv_t D, head_t H[], int dr, int dc,
		int ccols[], int nccols) {
	int i, c;
	printf("\n");
	for (i=0; i<nccols; i++) 
	{
		c = ccols[i];
		
		printf("    column %d: %s\n", c, H[c]);
	}
}
		 
/*****************************************************************
******************************************************************

Below here is where you do most of your work, and it shouldn't be
necessary for you to make any major changes above this point (except
for function prototypes, and perhaps some new #defines).

Below this point you need to write new functions that provide the
required functionality, and modify function handle_command() as you
write (and test!) each one.

Tackle the stages one by one and you'll get there.

Have Fun!!!

******************************************************************
*****************************************************************/
//This check whether the specific column is sorted or not
//The array , column which is to be sorted , and total num rows
// are passed as parameter to this function
//it traverses all the row for that column , if any previous row
//have greater vcalue than next row that means our column is not
//sorted so we return 0 else if loop completes successfully we 
//return 1 meaning the column is sorted
int is_sorted_col(csv_t D, int col, int dr)
{
	for ( int i = 0 ; i < dr - 1 ; i++ )
	{
		//If the previous row value is greater
		//than the next row value that mean data is not sorted
		if ( D[i][col] > D[i+1][col] )
		{
			return 0;
		}
	}
	//Loop complete without igniting the if condition that means
	// column is sorted
	return 1;
}
/*This function is most important one
It check whether all the columns value for two rows are same
or not, it traverse each column for both rows comparing the values
of column of both rows with each other, if they are not equal 
it instantly return 0 indicating that these two rows are not equal*/
int two_row_same(csv_t D, int ccols[], int first_row ,
 int second_row , int nccols)
{
	int c;
	for ( int i = 0 ; i < nccols ; i++ )
	{
		c = ccols[i];
		//It checks whether the specific is same for both row or not
		if ( D[first_row][c] != D[second_row][c] )
		{
			return 0;
		}	
	}
	return 1;
}
/*This function  swap the data of first row with second row
, all the column of both rows are swapped*/
void change_data(csv_t D,int ccols[], int first_row ,
 int second_row , int nccols) 
 {
 	int c;
 	for ( int i = 0 ; i < nccols ; i++ )
 	{
 		c = ccols[i];
 		//temp stores value of specific column for first row
 		double temp = D[first_row][c];
 		//first row is assigned value of second row
 		D[first_row][c] = D[second_row][c];
 		//first row value was saved in temp earlier
 		D[second_row][c] = temp;
	 }
 }
void 
do_analyze(csv_t D, head_t H[], int dr, int dc,
		int ccols[], int nccols) {
	
	printf("\n");
	int c;
	for ( int i = 0 ; i < nccols ; i++ )
	{
		c = ccols[i];
		
		//9 spaces for max , min names
		printf("         %8s ",H[c]);
		
		//Checks if col is sorted or not
		int sort = is_sorted_col(D,c,dr);
		//if col is sorted then we print sorted after column name
		if ( sort == 1 )
		{
			printf("(sorted)\n");
		}
		else
		{
			printf("\n");
		}
		
		int j = 0;
		double max = D[j][c];
		double min = D[j][c];
		double avg = D[j][c];
		for ( j = 1 ; j < dr ; j++ )
		{
			//Checks for max value of that column
			if ( D[j][c] > max )
			{
				max = D[j][c];
			}
			else if ( D[j][c] < min )
			{
				min = D[j][c];
			}
			//Calculating sum to later divide by total rows
			//to get average
			avg += D[j][c];
		}
		avg = avg / dr;	
		
		printf("    max = %7.1f\n",max);
		printf("    min = %7.1f\n",min);
		printf("    avg = %7.1f\n",avg);
		
		//If the col is sorted, then we print median too
		//of that particular column
		if ( sort == 1 )
		{
			double med;
			//Odd number of rows
			//simply print the midst value
			if ( dr % 2 != 0 )
			{
				med = D[dr/2][c];
			}
			//Even number of rows 
			//Add both middle value and divide them by 2
			else
			{
				med = ( D[dr/2][c] + D[(dr+1)/2][c] ) / 2;
			}
			printf("    med = %7.1f\n",med);
		}
		printf("\n");
	}
}
void 
do_display(csv_t D, head_t H[], int dr, int dc,
		int ccols[], int nccols)
{
	//Headings display 
	//Each heading have will be given at most 8 spaces
	int spaces = nccols;
	int c;
	printf("\n");
	//Since the right most column is to be printed first
	//so we multiply spaces by 8 to print most right column name
	//first then we subtact 1 from space to print 2nd column name
	//In %*s, * represnts the total space and s represnts the string
	for ( int i = nccols - 1 ; i >= 0 ; i-- )
	{
		c = ccols[i];	
		printf("%*s\n",spaces * 8, H[c]);
		spaces--;
	}
	
	//Now we have to display all the rows
	//Only those rows which are unique
	//and after each unique row we have to print the time
	//that row was found
	//so we need instance variable to store time the row duplicate was found
	int instances , i;
	for ( i = 0 ; i < dr - 1 ; i++ )
	{
		//instance is initialized to 1 
		instances = 1;
		
		//Until a unique row is found
		// we increment the i and instances
		while ( two_row_same(D,ccols,i,i+1,nccols) )
		{
			instances++;
			i++;
		}
		//Print the unique row data
		for ( int j = 0 ; j < nccols ; j++ )
		{
			c = ccols[j];
			//7.1 one means print 7 digits before decimal
			// and 1 digit after decimal
			printf(" %7.1f",D[i][c]);
		}
		printf("    (%2d instance",instances);
		//plural instances
		if ( instances > 1 )
		{
			printf("s");
		}
		printf(")\n");
	}
	//Check if i haven't reached the limit then print the data
	if ( i != dr )
	{
		//Since this is the last row , we check if the second
		//last row was same to last row or not
		//beacuse instances value was set to 1 when it goes inside
		//loop but for last iteration it didn't went inside loop
		// as condition went false
		//so we check here whether the last two rows were same or
		//not if not then set the value of instance to 1
		if ( ! two_row_same(D,ccols,i,i-1,nccols) )
		{
			instances = 1;
		}
		//Print the last data
		for ( int j = 0 ; j < nccols ; j++ )
		{
			c = ccols[j];
			printf(" %7.1f",D[i][c]);
		}
		printf("    (%2d instance",instances);
		if ( instances > 1 )
		{
			printf("s");
		}
		printf(")\n");
	}
}
void assign(double assigner[] , double assigned[] , int dc)
{
	for ( int z = 0 ; z < dc ; z++ )
	{
		assigned[z] = assigner[z];
	}
}
//Hardest function to write
//We have to sort based on multiple column
//s 3 0 1 means first sort 3rd column
//then sort value based 0th column where the values of 3rd column are same
//and then sort 1st column where 0th and 3rd coumn values are same
void 
do_sort(csv_t D, head_t H[], int dr, int dc,
		int ccols[], int nccols)
{
	//This array store the whole row
	//To store the current element in insertion sort
	double saver[MAXCOLS];
	int c;
	printf("\n");
	for ( int k = 0 ; k < nccols ; k++ )
	{
		//printf("Current column being sorted is : %d\n",ccols[k]);
		c = ccols[k];	
		for ( int i = 0 ; i < dr ; i++ )
		{
			//printf("Current element is : %lf\n",D[i][c]);
			double curr_element = D[i][c];
			//This saves whole row of current element
			/*for ( int m = 0 ; m < dc ; m++ )
			{
				saver[m] = D[i][m];
			}*/
			assign(D[i] , saver , dc );
			//j will move i - 1 to 0 to find any values 
			//greater than current element
			int j = i - 1;
			
			//Two row same function check whether the previous
			//column in sort function are same or not
			//k is the count of previous column
			//j and j + 1 are rows
			//ccols contain the col number 
			//if the value of current column and row is
			//greater than curr element we need to swap
			while ( j >= 0 &&
			 D[j][c] > curr_element &&
			  two_row_same(D,ccols,j+1,j,k))
			{
				//printf("Greater element is : %lf\n",D[j][c]);
				//Assigning the value of previous row to next row
				// we don't need swap function here since we are 
				//overwriting and not changing
			 	/*for ( int z = 0 ; z < dc ; z++ )
			 	{
			 		D[j+1][z] = D[j][z];
				}*/
				assign(D[j] , D[j+1] , dc );
				j--;
			}
			/*for ( int z = 0 ; z < dc ; z++ )
			{
			 	D[j+1][z] = saver[z];
			}*/
			assign(saver , D[j+1] , dc );
		}
	}
	//Printing the name of column on whose basis we sorted the data
	printf("    sorted by: ");
	for ( int i = 0 ; i < nccols ; i++ )
	{
		c = ccols[i];
		printf("%s",H[c]);
		if ( i + 1 != nccols )
		{
			printf(", ");
		}
	}
	printf("\n");	
}
void do_plot(csv_t D, head_t H[], int dr, int dc,
		 int ccols[], int nccols)
{
	//We have to plot data 
	/*if(fabs(d1-d2) < 0.0000001)
    	{
    	}*/
    	printf("\n");
	double min = D[0][ccols[0]];
	double max = D[0][ccols[0]];
	double interval_increment;
	//We will store bars array and then determine the scale
	int bars[INTERVAL_RANGE][nccols];
	for ( int i = 0 ; i < INTERVAL_RANGE ; i++ )
	{
		for ( int j = 0 ; j < nccols ; j++ )
		{
			bars[i][j] = 0;
		}
	}
	//Initially the scale is 1
	int scale = 1;
	int c;
	//Getting maximum and minmum in select column
	//We first need to find maximum and minmum of all specific column
	//then subtract and divide by 10 to get size of each interval
	for ( int i = 0 ; i < nccols ; i++ )
	{
		c = ccols[i];
		for ( int j = 0 ; j < dr ; j++ )
		{
			if ( D[j][c] > max )
			{
				max = D[j][c];
			}
			else if ( D[j][c] < min )
			{
				min = D[j][c];
			}
		}
	}
	
	//All column have same values we can't plot
	if ( max == min )
	{
		printf("all selected elements are %.1f\n",min);
		return;
	}
	//Size of interval
	interval_increment = ( max - min ) / INTERVAL_RANGE;
	
	//Printing the plot for each interval
	int j = 0;
	for ( double i = min ; j <= 10 ; i += interval_increment , j++ )
	{
		//All 10 intervals are completed so we break
		if ( j == 10 )
		{
			break;
		}
		//Bars for each interval
		for ( int k = 0 ; k < nccols ; k++ )
		{
			c = ccols[k];
			for ( int m = 0 ; m < dr ; m++ )
			{
				double upper = i + interval_increment;
				if ( (D[m][c] > i || 
				fabs(D[m][c] - i ) < 0.000001 ) &&
				 D[m][c] < upper )
				{
					bars[j][k] = bars[j][k] + 1;
				}
				//For last plot, we need to check <= for upper bound
				//for previous we have to do < upper bound only
				else if ( j == 9 )
				{
					
					if ( (D[m][c] > i || 
					fabs(D[m][c] - i ) < 0.000001 ) &&
					 ( D[m][c] < upper ||
					fabs(D[m][c] - upper ) < 0.000001 ))
					{
						bars[j][k] = bars[j][k] + 1;
					}
				}
			}
		}	
	}
	//Find max bar to determine scale range
	int max1 = -1;
	for ( int i = 0 ; i < INTERVAL_RANGE ; i++ )
	{
		for ( int j = 0 ; j < nccols ; j++ )
		{
			if ( bars[i][j] > max1 )
			{
				max1 = bars[i][j];
			}
		}
	}
	//Bring down to less than 60 bars
	while ( max1 > 60 )
	{
		scale++;
		max1 -= 60;
	}
	//Printing the bars based in bars array
	j = 0;
	for ( double i = min ; j <= 10 ; i += interval_increment , j++ )
	{
		//For zero value
		if ( i < 0.0000001 && i > -0.0000001 )
		{
			//I don't know why i am doing this and
			//why we have to add subtract before 0
			printf("     -%1.1f +\n", i);
		}
		else
		{
			printf("    %5.1f +\n", i);
		}
		
		//All 10 intervals are completed so we break
		if ( j == 10 )
		{
			break;
		}
		//Plotting for each interval
		for ( int k = 0 ; k < nccols ; k++ )
		{
			c = ccols[k];
			printf("    %5d |",c);
			int m = bars[j][k] / scale;
			//That means number is greater than integer value and we have to round up the number
			if ( bars[j][k] / scale > 0 &&
			 bars[j][k] % scale != 0)
			{
				m++;
			}
			for (  ; m > 0 ; m-- )
			{
				printf("]");
			}
			printf("\n");
		}	
	}
	
	printf("    scale = %d\n",scale);
}

/* this function examines each incoming command and decides what
   to do with it, kind of traffic control, deciding what gets
   called for each command, and which of the arguments it gets
*/
void
handle_command(int command, int ccols[], int nccols,
			csv_t D, head_t H[], int dr, int dc) { 
	if (command==O_NOC) {
		/* the null command, just do nothing */
	} 
	else if (command==O_IND) 
	{
		do_index(D, H, dr, dc, ccols, nccols);
	} 
	else if (command==O_ANA) 
	{
		do_analyze(D, H, dr, dc, ccols, nccols);
	} 
	else if (command==O_DPY) 
	{
		do_display(D, H, dr, dc, ccols, nccols);
	}
	else if ( command==O_SRT )
	{
		do_sort(D, H, dr, dc, ccols, nccols);
	}
	else if ( command==O_PLT )
	{
		do_plot(D, H, dr, dc, ccols, nccols);
	}
	else {
		printf("command '%c' is not recognized"
			" or not implemented yet\n", command);
	}
	return;
}

//Algorithms are fun

