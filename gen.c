//
//  gen.c
//  
//
//  Created by Victor Mitra on 03/03/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    system("clear");
    time_t timer;
    struct tm* tm_info;
    char day[3];
    char month[3];
    char year[5];
    time(&timer);
    tm_info = localtime(&timer);
    strftime(day, 3, "%d", tm_info);
    strftime(month, 3, "%m", tm_info);
    strftime(year, 5, "%Y", tm_info);
    printf("\n\nCurrent date (dd/mm/yyyy) is: %s/%s/%s\n\n", day, month, year);
   
    return 0;
}
    
