//
//  time.c
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
    system("clear");
    // initialize variables

    time_t timer;
    int choice;
    
    struct tm *tm_info;
    char new_std_regnbr[9];
    char year_extract[5],aca_year[5],t_aca_year[5];
    char std_first_name[10], std_last_name[10];
    int ctr_current_year,ctr_next_year;
    int std_regnbr_pos;
    int std_master_flag,class_master_flag;
    int tot_students;
    int class_of_admission,t_class_of_admission;
    char int_day[3];
    char week_day[10];
    char int_month[3];
    char month[10];
    char zero[4];
    char num_str[5];
    char int_curr_year[5], int_next_year[5];
    int try_ctr;
    FILE *std_master_rec, *class_master_rec;
    
    time(&timer);
    tm_info = localtime(&timer);
    strftime(int_day, 3, "%d", tm_info);
    strftime(week_day, 10, "%A", tm_info);
    strftime(int_month, 3, "%m", tm_info);
    strftime(month, 10, "%B", tm_info);
    strftime(int_curr_year, 5, "%Y", tm_info);
    sprintf(int_next_year,"%d",atoi(int_curr_year)+1);
    
    printf("Choose start year - 1 for %s and 2 for %s: ", int_curr_year, int_next_year);
    scanf("%d",&choice);
    if (choice!=1 && choice!=2)
        choice=1;
    switch(choice)
    {
        case 1:
            strcpy(aca_year,int_curr_year);
            break;
        case 2:
            strcpy(aca_year,int_next_year);
            break;
    }
    try_ctr=3;
get_class:
    printf("6 - Class 06\t7 - Class 07\t8 - Class 08\t9 - Class 09\t10 - Class 10\n");
    printf("Choose class of admission: ");
    scanf("%d",&class_of_admission);
    
    if(class_of_admission<6 || class_of_admission>10)
    {
        try_ctr--;
        if(try_ctr<1)
        {
            printf("You have exhausted all your chances for a correct entry, bye bye!\n\n");
            goto end_appl;
        }
        else
        {
            printf("Invalid class of admission ... you have %d chances left!\n",try_ctr);
            goto get_class;
        }
    }
    
    std_master_rec=fopen("std_master.dat","r");
    class_master_rec=fopen("class_master.dat","r");
    
    if(class_master_rec!=NULL)
    {
        tot_students=0;
        while(fscanf(class_master_rec,"%d %s %s %d\n",&t_class_of_admission,t_aca_year,new_std_regnbr,&class_master_flag)!=EOF)
        {
            if(class_of_admission==t_class_of_admission && strcmp(aca_year,t_aca_year)==0)
                tot_students++;
        }
    }
    
    if(class_master_rec==NULL || tot_students<10)
    {
        printf("Enter first name : ");
        scanf("%s",std_first_name);
        printf("Enter last name : ");
        scanf("%s",std_last_name);
    
        if(std_master_rec==NULL)
        {
            switch(choice)
            {
                case 1:
                    sprintf(new_std_regnbr,"%s",int_curr_year);
                    break;
                case 2:
                    sprintf(new_std_regnbr,"%s",int_next_year);
                    break;
            }
            strcat(new_std_regnbr,"0001");
        }
        else
        {
            ctr_current_year=0;
            ctr_next_year=0;
            while (fscanf(std_master_rec,"%s\n",new_std_regnbr)!=EOF)
            {
                for(std_regnbr_pos=0;std_regnbr_pos<4;std_regnbr_pos++)
                    year_extract[std_regnbr_pos]=new_std_regnbr[std_regnbr_pos];
                if(atoi(year_extract)>=atoi(int_curr_year))
                {
                    if(strcmp(year_extract,int_curr_year)==0)
                        ctr_current_year++;
                    if(strcmp(year_extract,int_next_year)==0)
                        ctr_next_year++;
                }
            }
            switch(choice)
            {
                    case 1:
                        ctr_current_year++;
                        sprintf(new_std_regnbr,"%s",int_curr_year);
                        if(ctr_current_year<1000)
                        {
                            if(ctr_current_year<10)
                                sprintf(zero,"%s","000");
                            if(ctr_current_year>=10 && ctr_current_year<100)
                                sprintf(zero,"%s","00");
                            if(ctr_current_year>=100 && ctr_current_year<1000)
                                sprintf(zero,"%s","0");
                            strcat(new_std_regnbr,zero);
                        }
                        sprintf(num_str,"%d",ctr_current_year);
                        strcat(new_std_regnbr,num_str);
                        break;
                    
                    case 2:
                        ctr_next_year++;
                        sprintf(new_std_regnbr,"%s",int_next_year);
                        if(ctr_next_year<1000)
                        {
                            if(ctr_next_year<10)
                                sprintf(zero,"%s","000");
                            if(ctr_next_year>=10 && ctr_next_year<100)
                                sprintf(zero,"%s","00");
                            if(ctr_next_year>=100 && ctr_next_year<1000)
                                sprintf(zero,"%s","0");
                            strcat(new_std_regnbr,zero);
                        }
                        sprintf(num_str,"%d",ctr_next_year);
                        strcat(new_std_regnbr,num_str);
                        break;
                }
        }
        printf("Congrats, your registration has been accepted and your registration number is: %s\n",new_std_regnbr);
        fclose(std_master_rec);
        fclose(class_master_rec);
        std_master_rec=fopen("std_master.dat","a+");
        fprintf(std_master_rec,"%s %s %s\n",new_std_regnbr,std_first_name,std_last_name);
        class_master_rec=fopen("class_master.dat","a+");
        fprintf(class_master_rec,"%d %s %s %d\n",class_of_admission,aca_year,new_std_regnbr,1);
    }
    else
        printf("Sorry, no more place in class %d for academic year %s\n", class_of_admission, aca_year);
    fclose(std_master_rec);
    fclose(class_master_rec);
end_appl:
    fflush(stdin);
    return 0;
}
