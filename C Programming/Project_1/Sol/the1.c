# include <stdio.h>
int i, k, j, l, coordinates[25][25], row, column, color, x, y, old_color, counter ;
int cp_1r, cp_1c, cp_2r, cp_2c, p_1r, p_1c, p_2r, p_2c ;
int start_row, start_column, finish_row, finish_column ;
int p_start_row, p_start_column, p_finish_row, p_finish_column ;
int t_l_r, t_l_c, b_r_r, b_r_c, p_t_l_r, p_t_l_c, p_b_r_r, p_b_r_c, angle, copied[625], copy_counter, paste_counter ;
char operation, rotation ;

/* Recersive function for the first operation*/
void filler (int r, int c)
{
    coordinates[r][c] = color; 

    if ((0 <= r-1 <= 24) && (coordinates[r-1][c] == old_color)) filler(r-1, c);
    if ((0 <= r+1 <= 24) && (coordinates[r+1][c] == old_color)) filler(r+1, c);
    if ((0 <= c-1 <= 24) && (coordinates[r][c-1] == old_color)) filler(r, c-1);
    if ((0 <= c+1 <= 24) && (coordinates[r][c+1] == old_color)) filler(r, c+1);
    if ((0 <= r-1 <= 24) && (0 <= c-1 <= 24) && (coordinates[r-1][c-1] == old_color)) filler(r-1, c-1);
    if ((0 <= r+1 <= 24) && (0 <= c+1 <= 24) && (coordinates[r+1][c+1] == old_color)) filler(r+1, c+1);
    if ((0 <= r+1 <= 24) && (0 <= c-1 <= 24) && (coordinates[r+1][c-1] == old_color)) filler(r+1, c-1);
    if ((0 <= r-1 <= 24) && (0 <= c+1 <= 24) && (coordinates[r-1][c+1] == old_color)) filler(r-1, c+1);        

}


int main(void){

for(i = 0; i < 25; i++)
{ 
    for (k = 0; k<25; k++) 
    { 
        scanf("%d", &coordinates[i][k]) ; 
    }
}

scanf(" %c", &operation) ;

if (operation == 'F'){

scanf("%d %d %d %d",&y, &x, &row, &column);

/* if else ladder for finding the color */
if ((0 <= x) && (x <= 24) && (0 <= y) && (y <= 24)) color = coordinates[y][x] ;
else if ((x < 0) && (y < 0)) color = 0;
else if ((0 <= x) && (x  <= 24) && (y < 0)) color = 1;
else if ((x > 24) && (y < 0)) color = 2;
else if ((x > 24) && (0 <= y) && (y <= 24)) color = 3;
else if ((x > 24) && (y > 24)) color = 4;
else if ((0 <= x) && (x <= 24) && (y > 24)) color = 5;
else if ((x < 0) && (y > 24)) color = 6;
else if ((x < 0) && (0 <= y) && (y <= 24)) color = 7;

old_color = coordinates[row][column] ;


filler(row, column) ;

for (i = 0; i < 25; i++)
{
    for (k=0; k < 25; k++)
    {
        if (k == 24) printf("%d \n", coordinates[i][k]) ;
        else printf("%d ", coordinates[i][k]) ;
    }

}

}

if (operation == 'P' ){

scanf("%d %d %d %d %d %d %d %d", &cp_1r, &cp_1c, &cp_2r, &cp_2c, &p_1r, &p_1c, &p_2r, &p_2c) ;

/*Finding the top left and bottom right corners of the areas that will be copied and pasted*/
start_row = (cp_1r <= cp_2r) ? cp_1r : cp_2r ;
start_column = (cp_1c <= cp_2c) ? cp_1c : cp_2c ;
finish_row = (cp_1r <= cp_2r) ? cp_2r : cp_1r ;
finish_column = (cp_1c <= cp_2c) ? cp_2c : cp_1c ;

p_start_row = (p_1r <= p_2r) ? p_1r : p_2r ;
p_start_column = (p_1c <= p_2c) ? p_1c : p_2c ;
p_finish_row = (p_1r <= p_2r) ? p_2r : p_1r ;
p_finish_column = (p_1c <= p_2c) ? p_2c : p_1c ;

/*Storing the copied values in a array to paste them later*/
for(i = start_row; i <= finish_row; i++)
{
    for(k = start_column; k<= finish_column; k++)
    {
        copied[copy_counter] = coordinates[i][k] ;
        copy_counter++;
    }

}

/*Pasting the copied values and counting the changed pixels*/
for(i = p_start_row; i <= p_finish_row; i++)
{
    for(k = p_start_column; k<= p_finish_column; k++)
    {
        old_color = coordinates[i][k] ;
        coordinates[i][k] = copied[paste_counter];
        paste_counter++;
        if (old_color != coordinates[i][k]) counter++;
    }

}

for (i = 0; i < 25; i++)
{
    for (k=0; k < 25; k++)
    {
        if (k == 24) printf("%d \n", coordinates[i][k]) ;
        else printf("%d ", coordinates[i][k]) ;
    }

}

printf("%d\n", counter) ;

}

if (operation == 'R') {

scanf(" %c %d", &rotation, &angle) ;
scanf("%d %d %d %d %d %d %d %d", &t_l_r, &t_l_c, &b_r_r, &b_r_c, &p_t_l_r, &p_t_l_c, &p_b_r_r, &p_b_r_c) ;

/*Since rotating right 90 degrees and rotating left 270 degrees is the same thing handling rotations in couples.*/
/*Storing the values in the order they will be pasted*/
if ( (rotation == 'R' && angle == 0) || (rotation == 'L' && angle == 0 ))
{

    for(i = t_l_r; i <= b_r_r; i++)
    {
        for(k = t_l_c; k <= b_r_c; k++)
        {
            copied[copy_counter] = coordinates[i][k] ; 
            copy_counter++ ;
        }
    }

}


if ( (rotation == 'R' && angle == 90) || (rotation == 'L' && angle == 270 ))
{

    for(i = t_l_c; i <= b_r_c; i++)
    {
        for(k = b_r_r; k >= t_l_r; k--)
        {
            copied[copy_counter] = coordinates[k][i] ; 
            copy_counter++ ;
        }
    }

}

if ( (rotation == 'R' && angle == 180) || (rotation == 'L' && angle == 180 ))
{

    for(i = b_r_r; i >= t_l_r; i--)
    {
        for(k = b_r_c; k >= t_l_c; k--)
        {
            copied[copy_counter] = coordinates[i][k] ; 
            copy_counter++ ;
        }
    }

}

if ( (rotation == 'R' && angle == 270) || (rotation == 'L' && angle == 90 ))
{

    for(i = b_r_c; i >= t_l_c; i--)
    {
        for(k = t_l_r; k <= b_r_r; k++)
        {
            copied[copy_counter] = coordinates[k][i] ; 
            copy_counter++ ;
        }
    }

}

/*Pasting the values and counting the changes*/
for (i = p_t_l_r; i <= p_b_r_r; i++)
{
    for(k = p_t_l_c; k <= p_b_r_c; k++)
    {
        old_color = coordinates[i][k] ;
        coordinates[i][k] = copied[paste_counter] ;
        paste_counter++ ;
        if (old_color != coordinates[i][k]) counter++ ;
    }
}

for (i = 0; i < 25; i++)
{
    for (k=0; k < 25; k++)
    {
        if (k == 24) printf("%d \n", coordinates[i][k]) ;
        else printf("%d ", coordinates[i][k]) ;
    }

}

printf("%d\n", counter) ;

}

}
