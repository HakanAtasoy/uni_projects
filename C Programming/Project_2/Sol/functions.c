#include <stdlib.h>
#include <stdio.h>

#define n_sector 3



/* Fill in the functions and define helper functions if you need to */

float*** create_sector_times(unsigned int* n_drivers, unsigned int* n_laps){
    int i, k, l;
    
    scanf("%u %u", n_drivers, n_laps) ;

    float *** arr = malloc(sizeof(float **) * (*n_drivers)) ;
    
    for(i = 0; i < *n_drivers; i++)
    {
        arr[i] = malloc(sizeof(float *) * (*n_laps));
    } 

    for(i = 0; i < *n_drivers; i++)
    {
        for(k = 0; k < *n_laps; k++)
        {
            arr[i][k] = malloc(sizeof(float) * 3);
        }
    }

    for(i = 0; i < *n_drivers; i++)
    {
        for(k = 0; k < *n_laps; k++)
        {
            for(l = 0; l < 3; l++)
            {
                scanf("%f", &arr[i][k][l]);
            }      
        }
    }

    return arr;
}


unsigned int** create_positions(unsigned int* p_drivers, unsigned int* n_races){
    
    int a, b;
    scanf("%u %u", p_drivers, n_races) ;
    
    unsigned int** finish = malloc(sizeof(unsigned int*) * (*p_drivers) ) ;

    for(a = 0; a < *p_drivers; a++)
    {
        finish[a] = malloc(sizeof(unsigned int) * (*n_races)) ;
    }
    
    for(a = 0; a < *p_drivers; a++)
    {
        for(b = 0; b < *n_races; b++)
        {
            scanf("%u", &finish[a][b]) ; 
        }
    }
    
    return finish;
}


float** calculate_lap_times(float*** sector_times, unsigned int n_drivers, unsigned int n_laps){

    int x, y, z;
    
    float ** lap_times = malloc(sizeof(float*) * n_drivers) ;

    for(x = 0; x < n_drivers; x++)
    {
        lap_times[x] = malloc(sizeof(float) * n_laps) ;
    }

    for(x = 0; x < n_drivers; x++)
    {
        for(y = 0; y < n_laps; y++)
        {
            lap_times[x][y] = 0 ;
        }
    }

    for(x = 0; x < n_drivers; x++)
    {
        for(y = 0; y < n_laps; y++)
        {
            for(z = 0; z < 3; z++)
            {
                lap_times[x][y] += sector_times[x][y][z] ;
            }
        }
    }

    return lap_times;
}


unsigned int find_fastest_lap(float** lap_times, unsigned int n_drivers, unsigned int n_laps){
    
    unsigned int x, y ; 
    float new_time, epsilon = 0.0001, fastest_lap = lap_times[0][0];
    unsigned int fastest_id = 0 ;

    
    for(x = 0; x < n_drivers; x++)
    {
        for(y = 0; y < n_laps; y++)
        {
            new_time = lap_times[x][y] ;
            if ( (fastest_lap - epsilon < new_time) && (fastest_lap + epsilon > new_time) )  ;
            else if( fastest_lap > new_time ) 
            {   
                fastest_lap = new_time; 
                fastest_id = x;   
            }
        }
    }    

    return fastest_id;
}


unsigned int find_driver_fastest_lap(float** sector_times_of_driver, unsigned int n_laps){
    
    unsigned int e, q;
    unsigned int lap_number = 0 ;
    float epsilon = 0.0001 ;
    
    float * driver = malloc( sizeof(float) * n_laps ) ;
    
    for(e = 0; e < n_laps; e++)
    {
        driver[e] = 0 ;
        for(q = 0; q < 3; q++)
        {
            driver[e] += sector_times_of_driver[e][q] ;
        }
    }

    for(e = 0; e < n_laps; e++)
    {
        if( (driver[e] - epsilon < driver[lap_number]) && (driver[e] + epsilon > driver[lap_number]) )  ;
        else if( driver[e] < driver[lap_number] ) lap_number = e;
    }

    
    return lap_number;
}


float* selection_sort(float* arr, unsigned int len, char ord){
    
    int o, p, min_index, max_index;
    float epsilon = 0.0001, min , max ;

    float* copy = malloc( sizeof(float) * len ) ;

    for(o = 0; o < len; o++)
    {
        copy[o] = arr[o] ;
    }
    
    if(ord == 'A')
    {
        for(p = 0; p < len; p++)
        {
            min = copy[p] ;
            min_index = p ;
            for(o = p; o < len; o++)
            {
                if( (min - epsilon < copy[o]) && (min + epsilon > copy[o]) ) ;
                else if(copy[o] < min) 
                { 
                    min = copy[o] ; 
                    min_index = o ; 
                } 
            }
            if( min_index == p ) continue ;
            copy[min_index] = copy[p] ;
            copy[p] = min ;
        }

    }

 
    if(ord == 'D')
    {
        for(p = 0; p < len; p++)
        {
            max = copy[p] ;
            max_index = p ;
            for(o = p; o < len; o++)
            {
                if( (max - epsilon < copy[o]) && (max + epsilon > copy[o]) ) ;
                else if(copy[o] > max) 
                { 
                    max = copy[o] ; 
                    max_index = o ; 
                } 
            }
            if( max_index == p) continue ;
            copy[max_index] = copy[p] ;
            copy[p] = max ;
        }

    } 

    return copy;
}


unsigned int* find_finishing_positions(float** lap_times, unsigned int n_drivers, unsigned int n_laps){
    
    unsigned int o, p, min_index ;
    float epsilon = 0.0001, min;
    unsigned int* fin_pos = malloc( sizeof(unsigned int) * n_drivers ) ;
    unsigned int* lap_totals = malloc( sizeof(unsigned int) * n_drivers ) ;    
    

    for(o = 0; o < n_drivers; o++)
    {
        lap_totals[o] = 0;
        for(p = 0; p < n_laps; p++)
        {
            lap_totals[o] += lap_times[o][p] ;
        }
    }

    for(o = 0; o < n_drivers; o++)
    {
        fin_pos[o] = o ;
    }
    
    for(p = 0; p < n_drivers; p++)
    {
        min = lap_totals[p] ;
        min_index = p;
        for(o = p; o < n_drivers; o++)
        {
            if( (min - epsilon < lap_totals[o]) && (min + epsilon > lap_totals[o]) ) ;
            else if(lap_totals[o] < min) 
            { 
                min = lap_totals[o] ; 
                min_index = o ; 
            } 
        }
        if( min_index == p ) continue;
        lap_totals[min_index] = lap_totals[p] ;
        fin_pos[min_index] = p ;
        lap_totals[p] = min ;
        fin_pos[p] = min_index ;
    }

    return fin_pos;
}


float* find_time_diff(float** lap_times, unsigned int n_drivers, unsigned int n_laps, unsigned int driver1, unsigned int driver2){

    int d ;
    float * time_diff = malloc( sizeof(float) * n_laps ) ;


    for(d = 0; d < n_laps; d++)
    {
        if(d != 0)
        {
            time_diff[d] = time_diff[d - 1] + lap_times[driver1][d] - lap_times[driver2][d] ;  
        }
        else time_diff[d] = lap_times[driver1][d] - lap_times[driver2][d] ;
    }
    
    return time_diff;
}


unsigned int* calculate_total_points(unsigned int** positions, unsigned int p_drivers, unsigned int n_races){
    
    int b, f ;
    unsigned int * total_points = malloc( sizeof(unsigned int) * p_drivers ) ;
    unsigned int * placement = malloc ( sizeof(int) * p_drivers ) ;

    for(b = 0; b < p_drivers; b++)
    {
        total_points[b] = 0;
        scanf("%u", &placement[b]) ;
    }
    
    for(b = 0; b < p_drivers; b++)
    {
        for(f = 0; f < n_races; f++)
        {
            total_points[b] +=  placement[positions[b][f] - 1] ;
        }
    }
    return total_points;
}


unsigned int find_season_ranking(unsigned int* total_points, unsigned int p_drivers, unsigned int id){
    
    unsigned int t, w = 1 ;
    float racer = total_points[id], epsilon = 0.0001 ;
    

    for(t = 0; t < p_drivers; t++)
    {
        if ( t == id ) continue ;
        else if ( (total_points[t] - epsilon < racer) && (total_points[t] + epsilon > racer) && ( t < id) ) w++ ;
        else if ( total_points[t] >  racer ) w++ ;
    }


    return w;
}