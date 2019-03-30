//
//  age.c
//  
//
//  Created by Victor Mitra on 27/02/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

int main()
{
    time_t rawtime;
    struct tm * timeinfo;
    system("clear");
    
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    printf ( "Current local time and date: %s\n", asctime (timeinfo) );
    
    
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
    printf("%s/%s/%s\n\n", month, day, year);
    int n=atoi(year);
    printf("%d\n\n",n+1);
    
    return 0;
}
