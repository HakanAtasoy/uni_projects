#include <stdio.h>
#include <stdlib.h>
#include "the3.h"


int helper_name_check(char* x, char* y)
{
    while (*x == *y)
    {
        if((*x == 0) && (*y == 0))
        {
            return 0;
        }
        x++;
        y++;
    }
    return 1;
}



Apartment* add_apartment(Apartment* head, int index, char* apartment_name, int max_bandwidth){

    int i ;
    Apartment* new, *copy;

    if (head == NULL)
    {
        head = malloc(sizeof(Apartment)) ; 
        head -> name = apartment_name ;
        head -> max_bandwidth = max_bandwidth ;
        head -> flat_list = NULL ;
        head -> next = head ; 
        return head ;
    }

    copy = head ;

    if (index == 0)
    {
        new = malloc(sizeof(Apartment)) ;
        new -> name = apartment_name ;
        new -> max_bandwidth = max_bandwidth ;
        new -> flat_list = NULL ;
        new -> next = head ;
        while(copy -> next != head)
        {
            copy = copy -> next ;
        }
        copy -> next = new ;
        head = new ;
        return head ;
    }

    for(i = 0; i < index - 1; i++)
    {
       copy = copy -> next ; 
    }

    new = malloc(sizeof(Apartment)) ;
    new -> next = copy -> next ;
    copy -> next = new ;
    new -> name = apartment_name ;
    new -> max_bandwidth = max_bandwidth ;
    new -> flat_list = NULL ;

    return head ;

}

void add_flat(Apartment* head, char* apartment_name, int index, int flat_id, int initial_bandwidth){

    Flat* new, *place ;
    Apartment* ap = head ;
    int max_pos_band, e ;

    while(helper_name_check(ap -> name, apartment_name))
    {
        ap = ap -> next ;
    }

    max_pos_band = ap -> max_bandwidth ;
    place = ap -> flat_list ;

    while(place)
    {
        max_pos_band -= place -> initial_bandwidth ;
        place = place -> next ;
    }

    place = ap -> flat_list ;
    
    if (place == NULL)
    {
        ap -> flat_list = malloc(sizeof(Flat)) ;
        place = ap -> flat_list ;
        place -> id = flat_id ;
        place -> initial_bandwidth = initial_bandwidth <= max_pos_band ? initial_bandwidth : max_pos_band ;
        place -> is_empty = 0 ;
        place -> next = NULL ;
        place -> prev = NULL ;
    }

    else if (index == 0)
    {
        new = malloc(sizeof(Flat)) ;
        new -> id = flat_id ;
        new -> initial_bandwidth = initial_bandwidth <= max_pos_band ? initial_bandwidth : max_pos_band ;
        new -> is_empty = 0 ;
        new -> next = place ;
        new -> prev = NULL ;
        place -> prev = new ;
        ap -> flat_list = new ;
    
    }

    else
    {
        new = malloc(sizeof(Flat)) ;
        new -> id = flat_id ;
        new -> initial_bandwidth = initial_bandwidth <= max_pos_band ? initial_bandwidth : max_pos_band ;
        new -> is_empty = 0 ;

        for (e = 0; e < index - 1; e++)
        {
            place = place -> next ;
        }

        new -> next = place -> next ;
        new -> prev = place ;
        place -> next = new ;
        if (new -> next)
        {
            new -> next -> prev = new ;
        }
    } 

}

Apartment* remove_apartment(Apartment* head, char* apartment_name){

    Flat* tracker ;
    Apartment* ap = head, *rm = head;
    
    while(helper_name_check(ap -> name, apartment_name))
    {
        ap = ap -> next ;
    }

    if ((ap == head) && (ap -> next == NULL))
    {
        tracker = ap -> flat_list ;
        free(ap) ;
        while(tracker)
        {
            if(tracker -> prev) free(tracker -> prev) ;
            if(!tracker -> next) { free(tracker) ; break ; }
            tracker = tracker -> next ;
        }
        head = NULL ;
        return head ;
    }

    else
    {
        if (ap == head) head = ap -> next ;
        tracker = ap -> flat_list ;
        while(rm -> next != ap)
        {
            rm = rm -> next ;
        }
        rm -> next = ap -> next ;
        free(ap) ;
        while(tracker)
        {
            if(tracker -> prev) free(tracker -> prev) ;
            if(!tracker -> next)  { free(tracker) ; break ; }
            tracker = tracker -> next ;
        }
        ap -> flat_list = NULL ;
        return head ;
    }

}

void make_flat_empty(Apartment* head, char* apartment_name, int flat_id){

    Flat* vacation ;
    Apartment* ap = head ;

    while(helper_name_check(ap -> name, apartment_name))
    {
        ap = ap -> next ;
    } 

    vacation = ap -> flat_list ;

    while(vacation)
    {
        if(vacation -> id == flat_id)
        {
            vacation -> is_empty = 1 ;
            vacation -> initial_bandwidth = 0 ;
            break ;
        }
        vacation = vacation -> next ;
    }
}

int find_sum_of_max_bandwidths(Apartment* head){

    Apartment* ap ;
    int sum = 0 ;

    if (head == NULL) return 0 ;

    ap = head -> next ;

    sum += head -> max_bandwidth ;

    while(ap != head)
    {
        sum += ap -> max_bandwidth ;
        ap = ap -> next ;
    }

    return sum ;

}

Apartment* merge_two_apartments(Apartment* head, char* apartment_name_1, char* apartment_name_2){

    Apartment* ap1 = head, *ap2 = head, *changer = head ;
    Flat* fl1, *fl2, *tracker ;

    while(helper_name_check(ap1 -> name, apartment_name_1))
    {
        ap1 = ap1 -> next ;
    } 

    while(helper_name_check(ap2 -> name, apartment_name_2))
    {
        ap2 = ap2 -> next ;
    } 

    if (ap2 == head) head = ap2 -> next ;

    fl1 = ap1 -> flat_list ;
    fl2 = ap2 -> flat_list ;

    while (changer -> next != ap2)
    {
        changer = changer -> next ;
    }

    changer -> next = ap2 -> next ;

    ap1 -> max_bandwidth += ap2 -> max_bandwidth ;

    free(ap2) ;

    
    if (fl2 == NULL) ;
    else if (fl1 == NULL) fl1 = fl2 ;
    else 
    {
        tracker = fl1 ;
        while(tracker -> next)
        {
            tracker = tracker -> next ;
        }

        tracker -> next = fl2 ;
        fl2 -> prev = tracker ;
    }

    return head ;
}

void relocate_flats_to_same_apartment(Apartment* head, char* new_apartment_name, int flat_id_to_shift, int* flat_id_list){

    Apartment* new = head, *hunter;
    Flat* dog, *dog2 ;
    int k, searched, found ;

    while(helper_name_check(new -> name, new_apartment_name))
    {
        new = new -> next ;
    } 

    dog2 = new -> flat_list ;
    while (dog2)
    {
        if (dog2 -> id == flat_id_to_shift) break ;
        dog2 = dog2 -> next ;
    }

    for(k = 0; flat_id_list[k]; k++)
    {
        searched = flat_id_list[k] ;
        hunter = head ;
        do
        {
            found = 0 ;
            dog = hunter -> flat_list ;
            while(dog)
            {
                if (dog -> id == searched) {found = 1 ; break ; }
                dog = dog -> next ;
            }
            if (found == 1)
            {
                hunter -> max_bandwidth -= dog -> initial_bandwidth ;
                new -> max_bandwidth += dog -> initial_bandwidth ;
                if(dog -> prev) dog -> prev -> next = dog -> next ;
                else hunter -> flat_list = dog -> next ;
                if(dog -> next) dog -> next -> prev = dog -> prev ;
                dog -> next = dog2 ;
                dog -> prev = dog2 -> prev ;
                dog2 -> prev = dog ;
                if(dog -> prev) dog -> prev -> next = dog ;
                else new -> flat_list = dog ; 
                break ;
            }
            hunter = hunter -> next ;

        } while (hunter != head);
         
    }

}
