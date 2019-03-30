/*

This is an application developed in C by Victor Mitra (a project executed between 1st Feb and 8th Mar 2019)
This application is about a student management system, for a school which has students from year 6 to 10.

The main functionalities of this application are:
1) New student registration (for current year and following year)
2) Student performance entry (for term 1 and term 2 for all the classes in a current year)
3) View student performance, current year and the past

The C program makes uses of the following:

1) modular approach by using functions
2) structures, arrays and pointers
3) file handling to store and retrieve data
4) cross referencing between files to simulate a functionality of RDBMS
5) efficient data entry by making auto generation of codes and allowing step-by-step (e.g. year => class => student registration)

The following fixed named files are used for persistent storage

1) std_master.dat => to store student master data with registration number, first and last name
2) class_master.dat => to store information about current and past student class inscriptions of the students
3) marks_master.dat => to store student performance records
4) year_master.dat => to store unique calendar years for which student data is available

*/

// inclusion of standard header files

#include <stdio.h>
#include <stdlib.h>
#include <string.h> 
#include <time.h> 

//Declaring all the functions that will be used to build the program

int getpassword(); // get password and validate it
void menu(); // displays the main menu
void new_std_reg(); // New student registration (for current year and following year)
void add_std_marks(); // Student performance entry
void view_std_result(); // View student performance
void exit_stdmgtsys(); // Exit the student management system application
int val_num_entry(); // validate the numerical entries
void show_warning(); // display a standard warning message
void cls(); // an own function written with the help of the system("clear") function in the interest of efficiency

// Starting point of the application

int main()
{
    cls();
    int code=getpassword();
    if(code==1)
    {
        printf("Success !\n");
        menu();
    }
    else
    {
        printf("Wrong password.\n");
        exit_stdmgtsys();
    }
    return 0;
}

/************ End of function main() *****************/

/*** Function to enter the password and grant/deny access to the system ***/

int getpassword() 
{
    char passwd[10]; 
    printf("Enter password for the system: ");
    scanf("%s",passwd); 
    if(strcmp(passwd,"victor")==0) //comparing the entered password with "victor"
        return 1;
    else
        return 0;
}

/************ End of function getpassword() *****************/

/* 

Function to display a menu and accept choices
The user is offered a choice between 0 and 3 (both inclusive)
Entering any other choice apart from these will make the user exit out of the system

*/

void menu() 
{
    int choice; 
    repeat:
    cls();
    printf("\n\n\t\t\t\t****** Welcome to the Student Management System ******\n\n"); 
    printf("\n\n\t1. New student registration.\n");
    printf("\n\t2. Enter student performance record\n");
    printf("\n\t3. View student performance\n");
    printf("\n\t0. Exit system.\n");
    printf("\n\nEnter your choice: ");
    scanf("%d",&choice); 
    if (choice >=0 && choice <=3) 
    {
        switch(choice)
        {
            case 1:new_std_reg();
                break;
            case 2:add_std_marks();
                break;
            case 3:view_std_result();
                break;
            case 0:exit_stdmgtsys();
                break;
        }
        if (choice!=0) 
        {
            printf("\n\nEnter 0 to exit, any other number to continue: "); 
            scanf("%d",&choice);
            if (choice==0)
                exit_stdmgtsys();
            else
                goto repeat;
        }
    }
    else
    {
        printf("Invalid choice ... exiting system.\n"); 
        exit_stdmgtsys();
    }
}

/************ End of function menu() *****************/

/* 

Function to add a new student => new_std_reg()

This function generates a unique student ID of 8 chararcters and based on the following:
a) calendar year (yyyy) in which the student is registered
b) the class of admission
c) a serial number formed with a number and required zeros

*/

void new_std_reg() 
{
    cls();
    time_t timer;

    /* 
    The time_t datatype is a data type in the ISO C library defined for storing system time values. 
    Such values are returned from the standard time() library function. This type is a typedef defined
    in the standard header. ISO C defines time_t as an arithmetic type, but does not specify any particular 
    type, range, resolution, or encoding for it. Also unspecified are the meanings of arithmetic operations 
    applied to time values.

    */
    int choice;
    
    struct tm *tm_info;

    /* 

    The tm structure provides a namespace to have the time information filled in. Following is the tm structure information −

        struct tm {
        int tm_sec;         // seconds,  range 0 to 59          
        int tm_min;         // minutes, range 0 to 59           
        int tm_hour;        // hours, range 0 to 23             
        int tm_mday;        // day of the month, range 1 to 31  
        int tm_mon;         // month, range 0 to 11             
        int tm_year;        // The number of years since 1900  
        int tm_wday;        // day of the week, range 0 to 6    
        int tm_yday;        // day in the year, range 0 to 365  
        int tm_isdst;       // daylight saving time             	
        };
    
    */

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
    int year_ctr;

    // Declaring pointers for the persistent storage

    FILE *std_master_rec, *class_master_rec, *year_master_rec;
    
    time(&timer); // returns the time since the Epoch (00:00:00 UTC, January 1, 1970), measured in seconds.

    /* 
    The localtime function uses the time pointed by timer to fill a tm structure with the values that represent 
    the corresponding local time. The value of timer is broken up into the structure tm and expressed in the local time zone.
    */

    tm_info = localtime(&timer);

    /*
        strftime() is a function in C which is used to format date and time. 

        Some format specifiers for strftime() that is chosen are as follows :
        %A = Full weekday name
        %B = Full month name March
        %d = Day of the month (01-31)
        %m = Month as a decimal number (01-12)
        %Y = Returns year from the time in yyyy format

    */
    strftime(int_day, 3, "%d", tm_info);
    strftime(week_day, 10, "%A", tm_info);
    strftime(int_month, 3, "%m", tm_info);
    strftime(month, 10, "%B", tm_info);
    strftime(int_curr_year, 5, "%Y", tm_info); //Captures the current year - the year the program is being executed
    sprintf(int_next_year,"%d",atoi(int_curr_year)+1); //Captures the year after the current year
    
    printf("Choose start year - 1 for %s and 2 for %s: ", int_curr_year, int_next_year); //Displays a message to choose 1 for current year or 2 for next year
    scanf("%d",&choice); //Captures the value 1 for current year or 2 for next year
    if (choice!=1 && choice!=2) //If the user enters any value other 1 and 2, they program will interpret that the user has entered 1
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
            printf("You have exhausted all your chances for a correct entry, bye bye!\n\n"); //Displaying a message, after the user has exhausted all the attempts to enter a valid class
            goto end_appl;
        }
        else
        {
            printf("Invalid class of admission ... you have %d chances left!\n",try_ctr); //Displaying the message after every failed attempt. %d is the number of attempts left for entering a valid class
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
}

/************ End of function new_std_reg() *****************/

/* 

Function to add student performance record:

1) the function allows marks entry for the students in the current academic year
2) it references the class_master.dat and the corresponding first and last name from the std_master.dat
3) the valid marks allowed to enter are between 0 and 40 (both inclusive). To keep development efforts optimal, 
   no second chance is allowed (a warning is displayed for that purpose)

*/

void add_std_marks()
{
    cls();
    time_t timer;
    int choice;
    
    struct tm *tm_info; 
    
    char std_regnbr[9],t_std_regnbr[9];
    char year_extract[5],t_aca_year[5];
    char std_first_name[10],std_last_name[10];
    int ctr_current_year,ctr_next_year;
    int std_regnbr_pos;
    int std_master_flag,class_master_flag;
    int tot_students;
    int class_choice;
    int class_of_admission, t_class_of_admission;
    char int_day[3];
    char week_day[10];
    char int_month[3];
    char month[10];
    char zero[4];
    char num_str[5];
    char int_curr_year[5], int_next_year[5];
    int try_ctr,choice_ctr,std_choice,x,year_ctr,match_ctr;
    char m_reg_nbr[9],m_aca_year[5];
    int m_class,m_term,m_sub1,m_sub2,m_sub3,m_sub4,m_sub5,m_flag;
    FILE *std_master_rec, *class_master_rec, *marks_master_rec,*year_master_rec;
    
    struct std_marks 
    {
        int srl_num;
        char std_regnbr[9];
        char std_first_name[10];
        char std_last_name[10];
        char aca_year[5];
        int term;
        int current_class;
        int math_marks;
        int sci_marks;
        int eng_marks;
        int II_lang_marks;
        int soc_marks;
    }mrk_all[10]; 
    
    time(&timer); 
    tm_info = localtime(&timer);
    strftime(int_day, 3, "%d", tm_info);
    strftime(week_day, 10, "%A", tm_info);
    strftime(int_month, 3, "%m", tm_info);
    strftime(month, 10, "%B", tm_info);
    strftime(int_curr_year, 5, "%Y", tm_info);
    sprintf(int_next_year,"%d",atoi(int_curr_year)+1);
    
    std_master_rec=fopen("std_master.dat","r"); 
    class_master_rec=fopen("class_master.dat","r"); 
    
    if(std_master_rec==NULL) 
    {
        printf("No students exist on records to enter marks"); 
        goto end_appl;
    }
    else
    {
        try_ctr=3; 
    get_class:
        printf("For which class of year %s do you want to enter results ?\n", int_curr_year);
        printf("6 - Class 06\t7 - Class 07\t8 - Class 08\t9 - Class 09\t10 - Class 10\n");
        printf("Choose class: ");
        scanf("%d",&class_choice);
        
        if(class_choice<6 || class_choice>10) 
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
        choice_ctr=0;
        while(fscanf(class_master_rec,"%d %s %s %d\n",&t_class_of_admission,t_aca_year,t_std_regnbr,&class_master_flag)!=EOF)
        {
            if(class_choice==t_class_of_admission && strcmp(int_curr_year,t_aca_year)==0 && class_master_flag==1) //Comparing the existing string data with the data that are entered by the user
            {
                while(fscanf(std_master_rec,"%s %s %s\n",std_regnbr,std_first_name,std_last_name)!=EOF)
                {
                    if(strcmp(t_std_regnbr,std_regnbr)==0) //Comparing the existing registration number with the registration that is entered by the user
                    {
                        if(choice_ctr==0)
                            printf("\nFollowing students are currently there in class %d of current year (%s).\n\n",class_choice,int_curr_year); 
                        printf("%d %s %s %s\n",choice_ctr+1,std_regnbr,std_first_name,std_last_name);
                        mrk_all[choice_ctr].srl_num=choice_ctr+1; //???
                        strcpy(mrk_all[choice_ctr].std_regnbr,std_regnbr);
                        strcpy(mrk_all[choice_ctr].std_first_name,std_first_name);
                        strcpy(mrk_all[choice_ctr].std_last_name,std_last_name);
                        strcpy(mrk_all[choice_ctr].aca_year,t_aca_year);
                        mrk_all[choice_ctr].current_class=class_choice;
                        choice_ctr++;
                    }
                }
                fseek(std_master_rec, 0, SEEK_SET);
            }
        }
        if(choice_ctr==0)
        {
            printf("No students in class %d of current year (%s).\n",class_choice,int_curr_year);
            goto end_appl;
        }
        try_ctr=3;
        printf("\nFor whom do you like to enter the marks ?\n");
    get_choice:
        printf("Enter your choice (a number from %d to %d): ",1,choice_ctr);
        scanf("%d",&std_choice);
        if(std_choice<1 || std_choice>choice_ctr)
        {
            try_ctr--;
            if(try_ctr<1)
            {
                printf("Sorry, you have exhausted all your chances for a correct entry!\n\n");
                goto end_appl;
            }
            else
            {
                printf("Invalid choice ... you have %d chances left!\n",try_ctr);
                goto get_choice;
            }
        }
        printf("\nEntering exam marks for %s %s\n",mrk_all[std_choice-1].std_first_name,mrk_all[std_choice-1].std_last_name);
        show_warning();
        printf("Enter term: (1 or 2)");
        scanf("%d",&mrk_all[std_choice-1].term);
        // passing 2nd parameter as 1 to validate term
        if (val_num_entry(mrk_all[std_choice-1].term,1)==0)
            goto end_appl;
        
        marks_master_rec=fopen("marks_master.dat","r");
        match_ctr=0;
        while(fscanf(marks_master_rec,"%s %s %d %d %d %d %d %d %d %d\n",m_reg_nbr,m_aca_year,&m_class,&m_term,&m_sub1,&m_sub2,&m_sub3,&m_sub4,&m_sub5,&m_flag)!=EOF) //Checking if a data already exists that the user has entered
        {
            if(strcmp(mrk_all[std_choice-1].std_regnbr,m_reg_nbr)==0 && strcmp(mrk_all[choice_ctr-1].aca_year,m_aca_year)==0 && class_choice==m_class && mrk_all[std_choice-1].term==m_term)
            {
                match_ctr=1;
                fseek(marks_master_rec, 0, SEEK_END);
            }
        }
        fclose(marks_master_rec);
        if(match_ctr==0)
        {
            printf("Marks in maths (from 1 to 40): ");
            scanf("%d",&mrk_all[std_choice-1].math_marks);
            // passing 2nd parameter as 2 to validate marks
            if (val_num_entry(mrk_all[std_choice-1].math_marks,2)==0)
                goto end_appl;
            printf("Marks in science (from 1 to 40): ");
            if (val_num_entry(mrk_all[std_choice-1].math_marks,2)==0)
                goto end_appl;
            scanf("%d",&mrk_all[std_choice-1].sci_marks);
            if (val_num_entry(mrk_all[std_choice-1].sci_marks,2)==0)
                goto end_appl;
            printf("Marks in English: (from 1 to 40): ");
            scanf("%d",&mrk_all[std_choice-1].eng_marks);
            if (val_num_entry(mrk_all[std_choice-1].eng_marks,2)==0)
                goto end_appl;
            printf("Marks in 2nd language: (from 1 to 40): ");
            scanf("%d",&mrk_all[std_choice-1].II_lang_marks);
            if (val_num_entry(mrk_all[std_choice-1].II_lang_marks,2)==0)
                goto end_appl;
            printf("Marks in social studies: (from 1 to 40): ");
            scanf("%d",&mrk_all[std_choice-1].soc_marks);
            if (val_num_entry(mrk_all[std_choice-1].soc_marks,2)==0)
                goto end_appl;
            marks_master_rec=fopen("marks_master.dat","a");
            fprintf(marks_master_rec,"%s %s %d %d %d %d %d %d %d %d\n",mrk_all[std_choice-1].std_regnbr,mrk_all[std_choice-1].aca_year,mrk_all[std_choice-1].current_class,mrk_all[std_choice-1].term,mrk_all[std_choice-1].math_marks,mrk_all[std_choice-1].sci_marks,mrk_all[std_choice-1].eng_marks,mrk_all[std_choice-1].II_lang_marks,mrk_all[std_choice-1].soc_marks,1);
            printf("Thank you, marks entered are registered in the system.\n");
            year_master_rec=fopen("year_master.dat","r");
            year_ctr=0;
            if(year_master_rec!=NULL)
            {
                while(fscanf(year_master_rec,"%s\n",t_aca_year)!=EOF) 
                {
                    if(strcmp(mrk_all[std_choice-1].aca_year,t_aca_year)==0)
                    {
                        year_ctr=1;
                        fseek(year_master_rec, 0, SEEK_END);
                    }
                }
            }
            fclose(year_master_rec);
            if(year_ctr==0)
            {
                year_master_rec=fopen("year_master.dat","a+");
                fprintf(year_master_rec,"%s\n",mrk_all[std_choice-1].aca_year);
                fclose(year_master_rec);
            }
        }
        else
        {
            printf("Marks for student %s %s for term %d of academic year %s already exists.\n\n",mrk_all[std_choice-1].std_first_name,mrk_all[std_choice-1].std_last_name,mrk_all[std_choice-1].term,mrk_all[std_choice-1].aca_year);
            goto end_appl;
        }
    }
    end_appl:
    fclose(std_master_rec); //Close the file std_master.dat
    fclose(class_master_rec); //Close the file class_master.dat
    fclose(marks_master_rec); //Close the file marks_master.dat
    fflush(stdin);
}

/************ End of function add_std_marks() *****************/

/* 

Function to view student performance record(s):
1) At first it checks for which years records are available and provides a list of those years (if no records available, the function stops)
2) For each available year, the function lists down for which classes in the year marks are available
3) After choosing the class, the (unique) list of students are displayed
4) After choosing a particular student, the marks (of all available terms) are displayed

*/

void view_std_result()
{
    cls(); 

    // to hold the unique years for which marks exist

    struct year_chain // to create a run-time buffer to hold the contents of the file year_master.dat
    {
        int srl_nbr;
        char aca_year[5];
    }ychn[10];

    // buffer structure to hold the student records from file

    struct std_marks 
    {
        char std_regnbr[9];
        int current_class;
        int term;
        int math_marks;
        int sci_marks;
        int eng_marks;
        int II_lang_marks;
        int soc_marks;
        int flag;
    }mrk_all[400],mrk_foraclass[100];

    struct unique_std_reg
    {
        int srl_nbr;
        char std_regnbr[9];
        char std_first_name[10];
        char std_last_name[10];
    }ureg_nbr[50];

    char t_std_regnbr[9];
    char t_std_first_name[10];
    char t_std_last_name[10];
    char t_aca_year[5];
    int t_term;
    int t_current_class;
    int t_math_marks;
    int t_sci_marks;
    int t_eng_marks;
    int t_II_lang_marks;
    int t_soc_marks;
    int t_flag;

    int class_6_ctr=0;
    int class_7_ctr=0;
    int class_8_ctr=0;
    int class_9_ctr=0;
    int class_10_ctr=0;

    struct class_choice
    {
        int option;
        int class_nbr;
    }cl_choice[5];

    int class_srl_no=0;
    int to_show;
    int show_ctr;
    int num_recs_to_show;
    int mrk_foraclass_ctr;
    int v_mrk_ctr;

    int year_srl_nbr,ctr_year_disp,mrk_ctr,t_mrk_ctr,choice_year,choice_class,ureg_nbr_ctr,curr_pos_uregnbr,last_pos_uregnbr,ureg_nbr_found_ctr;

    FILE *marks_master_rec,*year_master_rec,*std_master_rec;
	marks_master_rec=fopen("marks_master.dat","r");
	if(marks_master_rec!=NULL)

/*
The function year_master.dat is opened in the read mode

1) The function reads the data that are stored in the file marks_master.dat
2) After reading the file, it checks the years, for which the student performance records are available

*/
   
   
   {
        year_master_rec=fopen("year_master.dat","r");
        year_srl_nbr=0;
        while(fscanf(year_master_rec,"%s\n",ychn[year_srl_nbr].aca_year)!=EOF)
        {
            ychn[year_srl_nbr].srl_nbr=year_srl_nbr+1;
            year_srl_nbr++;
        }
        printf("Records for the following years are available:\n\n");
        for(ctr_year_disp=0;ctr_year_disp<year_srl_nbr;ctr_year_disp++)
            printf("%d. %s\n",ychn[ctr_year_disp].srl_nbr,ychn[ctr_year_disp].aca_year);
        show_warning();
        printf("\nSelect serial number of the year for which you like to view records (from %d to %d): ",1,ctr_year_disp);
        scanf("%d",&choice_year);

        mrk_ctr=0;
        last_pos_uregnbr=0;

         // now loading all the records of the chosen year into a structure array
        while(fscanf(marks_master_rec,"%s %s %d %d %d %d %d %d %d %d\n",t_std_regnbr,t_aca_year,&t_current_class,&t_term,&t_math_marks,&t_sci_marks,&t_eng_marks,&t_II_lang_marks,&t_soc_marks,&t_flag)!=EOF)
        {
            if(strcmp(t_aca_year,ychn[choice_year-1].aca_year)==0)
            {
                switch(t_current_class)
                {
                    case 6:
                        class_6_ctr++;
                        break;
                    case 7:
                        class_7_ctr++;
                        break;
                    case 8:
                        class_8_ctr++;
                        break;
                    case 9:
                        class_9_ctr++;
                        break;
                    case 10:
                        class_10_ctr++;
                        break;
                }
                strcpy(mrk_all[mrk_ctr].std_regnbr,t_std_regnbr);
                mrk_all[mrk_ctr].current_class=t_current_class;
                mrk_all[mrk_ctr].term=t_term;
                mrk_all[mrk_ctr].math_marks=t_math_marks;
                mrk_all[mrk_ctr].sci_marks=t_sci_marks;
                mrk_all[mrk_ctr].eng_marks=t_eng_marks;
                mrk_all[mrk_ctr].II_lang_marks=t_II_lang_marks;
                mrk_all[mrk_ctr].soc_marks=t_soc_marks;
                mrk_ctr++;
            }
        }
        
        printf("For the year %s, marks for the following classes are available:\n\n",ychn[choice_year-1].aca_year);
        if(class_6_ctr>0)
        {
            printf("%d Class 6.\n",class_srl_no+1);
            cl_choice[class_srl_no].option=class_srl_no+1;
            cl_choice[class_srl_no].class_nbr=6;
            class_srl_no++;
        }
        if(class_7_ctr>0)
        {
            printf("%d Class 7.\n",class_srl_no+1);
            cl_choice[class_srl_no].option=class_srl_no+1;
            cl_choice[class_srl_no].class_nbr=7;
            class_srl_no++;
        }
        if(class_8_ctr>0)
        {
            printf("%d Class 8.\n",class_srl_no+1);
            cl_choice[class_srl_no].option=class_srl_no+1;
            cl_choice[class_srl_no].class_nbr=8;
            class_srl_no++;
        }
        if(class_9_ctr>0)
        {
            printf("%d Class 9.\n",class_srl_no+1);
            cl_choice[class_srl_no].option=class_srl_no+1;
            cl_choice[class_srl_no].class_nbr=9;
            class_srl_no++;
        }
        if(class_10_ctr>0)
        {
            printf("%d Class 10.\n",class_srl_no+1);
            cl_choice[class_srl_no].option=class_srl_no+1;
            cl_choice[class_srl_no].class_nbr=10;
            class_srl_no++;
        }
        printf("\nNow enter the serial nbr of the class from which you like to see the marks: ");
        scanf("%d",&choice_class);
        to_show=cl_choice[choice_class-1].class_nbr;
        
        // for the chosen class
        num_recs_to_show=0;
        mrk_foraclass_ctr=0;
        for(t_mrk_ctr=0;t_mrk_ctr<mrk_ctr;t_mrk_ctr++)
        {
            if(mrk_all[t_mrk_ctr].current_class==to_show)
            {
                // loads the smaller array with records for the chosen class
                num_recs_to_show++;
                strcpy(mrk_foraclass[mrk_foraclass_ctr].std_regnbr,mrk_all[t_mrk_ctr].std_regnbr);
                mrk_foraclass[mrk_foraclass_ctr].current_class=mrk_all[t_mrk_ctr].current_class;
                mrk_foraclass[mrk_foraclass_ctr].term=mrk_all[t_mrk_ctr].term;
                mrk_foraclass[mrk_foraclass_ctr].math_marks=mrk_all[t_mrk_ctr].math_marks;
                mrk_foraclass[mrk_foraclass_ctr].sci_marks=mrk_all[t_mrk_ctr].sci_marks;
                mrk_foraclass[mrk_foraclass_ctr].eng_marks=mrk_all[t_mrk_ctr].eng_marks;
                mrk_foraclass[mrk_foraclass_ctr].II_lang_marks=mrk_all[t_mrk_ctr].II_lang_marks;
                mrk_foraclass[mrk_foraclass_ctr].soc_marks=mrk_all[t_mrk_ctr].soc_marks;

                // unique records structur array to display

                ureg_nbr_found_ctr=0;
                for(curr_pos_uregnbr=0;curr_pos_uregnbr<50;curr_pos_uregnbr++)
                {
                    if (strcmp(ureg_nbr[curr_pos_uregnbr].std_regnbr,mrk_foraclass[mrk_foraclass_ctr].std_regnbr)==0)
                        ureg_nbr_found_ctr++;
                }
                if (ureg_nbr_found_ctr==0)
                {
                    ureg_nbr[last_pos_uregnbr].srl_nbr=last_pos_uregnbr+1;
                    strcpy(ureg_nbr[last_pos_uregnbr].std_regnbr,mrk_foraclass[mrk_foraclass_ctr].std_regnbr);

                    //referencing std_master.dat for the corresponding first name and last name of student

                    std_master_rec=fopen("std_master.dat","r");
                    while(fscanf(std_master_rec,"%s %s %s\n",t_std_regnbr,t_std_first_name,t_std_last_name)!=EOF)
                    {
                        if(strcmp(ureg_nbr[last_pos_uregnbr].std_regnbr,t_std_regnbr)==0)
                        {
                            strcpy(ureg_nbr[last_pos_uregnbr].std_first_name,t_std_first_name);
                            strcpy(ureg_nbr[last_pos_uregnbr].std_last_name,t_std_last_name);
                            fseek(std_master_rec, 0, SEEK_END); 
                        }
                    }
                    fclose(std_master_rec);
                    last_pos_uregnbr++;
                    mrk_foraclass_ctr++;
                }
            }
        }
        if(num_recs_to_show>0)
        {
            printf("\nRecords of the following students from class %d of academic year %s are available:\n\n",to_show,ychn[choice_year-1].aca_year);
            for(t_mrk_ctr=0;t_mrk_ctr<last_pos_uregnbr;t_mrk_ctr++)
            printf("\n%d %s %s %s",t_mrk_ctr+1,ureg_nbr[t_mrk_ctr].std_regnbr,ureg_nbr[t_mrk_ctr].std_first_name,ureg_nbr[t_mrk_ctr].std_last_name);
            printf("\n\nEnter the serial nbr of the student whose marks you like to view: ");
            scanf("%d",&v_mrk_ctr);
            printf("\nNow displaying the marks of %s %s with registration number: %s\n",ureg_nbr[v_mrk_ctr-1].std_first_name,ureg_nbr[v_mrk_ctr-1].std_last_name,ureg_nbr[v_mrk_ctr-1].std_regnbr);
            for(show_ctr=0;show_ctr<num_recs_to_show;show_ctr++)
            {
                if(strcmp(mrk_foraclass[show_ctr].std_regnbr,ureg_nbr[v_mrk_ctr-1].std_regnbr)==0)
                {
                    printf("\nTerm: %d.\n",mrk_foraclass[show_ctr].term);
                    printf("Math marks: %d.\n",mrk_foraclass[show_ctr].math_marks);
                    printf("Science marks: %d.\n",mrk_foraclass[show_ctr].sci_marks);
                    printf("English marks: %d.\n",mrk_foraclass[show_ctr].eng_marks);
                    printf("2nd language marks: %d.\n",mrk_foraclass[show_ctr].II_lang_marks);
                    printf("Social studies marks: %d.\n",mrk_foraclass[show_ctr].soc_marks);
                }
            }
        }
    }
	else
        printf("No student marks available to show.\n");

    fclose(marks_master_rec);
    fclose(year_master_rec);
    fflush(stdin);
}

void exit_stdmgtsys()
{
    printf("Bye ... have a nice day!\n\n\n");
}

void cls()
{
    system("clear");
}

int val_num_entry(int x, int cat)
{
    int ret_code=1;
    switch(cat)
    {
        case 1:
            if (x<0 || x>2)
            {
                printf("Invalid term entered.\n\n");
                ret_code=0;
            }
            break;
        case 2:
            if (x<0 || x>40)
            {
                printf("Invalid marks entered.\n\n");
                ret_code=0;
            }
            break;
    }
    return ret_code;
}

void show_warning()
{
    printf("**************************** Please be careful ***********************************\n\n");
    printf("An invalid entry will get you out of the system without giving you a 2nd chance !!\n\n");
    printf("**********************************************************************************\n\n");
}
