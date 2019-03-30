//
//  name.c
//  
//
//  Created by Victor Mitra on 04/03/19.
//

#include <stdio.h>
#include <stdlib.h>

int main()
{
    char fname[20],lname[10];
    int age;
    system("clear");
    printf("Enter first name: ");
    gets(fname);
    printf("Enter last name: ");
    scanf("%s",lname);
    printf("Enter age: ");
    scanf("%d",&age);
    
    printf("%s %s is aged %d years\n",fname,lname,age);
    return 0;
}
