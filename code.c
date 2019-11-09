#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <ctype.h>


struct booking
{
    char guest[20];
    char roomtype[20];
    char numadults[2];
    char numkids[2];
    char checkin[20];
    char checkout[20];
} ubk;


struct user
{
    char usid[20];
    char upwd[20];
    char unme[20];
    char uage[2];
    char gender[2];
} uzr;


struct room
{
    char roomnumber[20];
    char roomtype[20];
    char roomcapacity[20];
} rom;


char option[20];
FILE *fp;
FILE *fpc;
FILE *fpt;
char validator[20];
char userid[20];
char pass[20];
char filename[100], c;
int size;

int mainmenu(void);
int adminmenu(void);
int registeration(void);
int viewuserbooking(void);
int editbook(void);
int deletebook(void);
int userbooking(void);
int welcome(void);
int addroom(void);

int clear(void)
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
    system("clear");
#endif

#if defined(_WIN32) || defined(_WIN64)
    system("cls");
#endif

    return 0;
}

int profile(void)
{
    fp = fopen("users", "r");

    while (fread(&uzr, sizeof(uzr), 1, fp))
    {
        if (strcmp(userid, uzr.usid) == 0)
        {
            clear();
            printf("\nYour Profile:\n----------------------\n\nName      : %s\nusername  : %s\nAge       : %s\nGender    : %s", uzr.unme, uzr.usid, uzr.uage, uzr.gender);
        }
    }
    fclose(fp);

    printf("\n\n[1] Back to Main Menu\n");
    printf("> ");
    scanf("%s", option);
    if (strcmp(option, "1") == 0)
    {
        clear();
        mainmenu();
    }

    return 0;
}

int editdelete(void)
{
    viewuserbooking();
    printf("\n[1] Edit Your Booking.\n[2] Cancel Your Booking.\n[3] Go to Main Menu.\n> ");
    scanf("%s", option);
    if (strcmp(option, "1") == 0)
    {
        clear();
        editbook();
    }
    if (strcmp(option, "2") == 0)
    {
        clear();
        deletebook();
    }
    if (strcmp(option, "3") == 0)
    {
        clear();
        mainmenu();
    }
    else
    {
        clear();
        printf("Wrong Input");
        editdelete();
    }

    return 0;
}

int deletebook()
{
    fp = fopen("bookings", "r");
    fpt = fopen("tempfile", "w");
    fpc = fopen("cancelled", "w");

    while (fread(&ubk, sizeof(ubk), 1, fp))
    {
        if (strcmp(userid, ubk.guest) == 1)
        {
            fwrite(&ubk, sizeof(ubk), 1, fpt);
        }
        else if (strcmp(userid, ubk.guest) == 0)
        {
            fwrite(&ubk, sizeof(ubk), 1, fpc);
        }
    }
    fclose(fp);
    fclose(fpt);
    fclose(fpc);

    fp = fopen("bookings", "w");
    fpt = fopen("tempfile", "r");

    while (fread(&ubk, sizeof(ubk), 1, fpt))
    {
        fwrite(&ubk, sizeof(ubk), 1, fp);
    }
    fclose(fp);
    fclose(fpt);
    clear();
    printf("\nYour Booking is canceled successfully.\n\n");
    mainmenu();

    return 0;
}

int editbook(void)
{
    fp = fopen("bookings", "r");
    fpt = fopen("tempfile", "w");

    while (fread(&ubk, sizeof(ubk), 1, fp))
    {
        if (strcmp(userid, ubk.guest) == 1)
        {
            fwrite(&ubk, sizeof(ubk), 1, fpt);
        }
    }
    fclose(fp);
    fclose(fpt);

    fp = fopen("bookings", "w");
    fpt = fopen("tempfile", "r");

    while (fread(&ubk, sizeof(ubk), 1, fpt))
    {
        fwrite(&ubk, sizeof(ubk), 1, fp);
    }
    fclose(fp);
    fclose(fpt);

    userbooking();
    clear();

    return 0;
}

int editroom()
{
    printf("Please, Choose Which room: ");
    scanf("%s", option);

    fp = fopen("rooms", "r");
    fpt = fopen("tempfile", "w");

    while (fread(&rom, sizeof(rom), 1, fp))
    {
        if (strcmp(option, rom.roomnumber) == 1)
        {
            fwrite(&rom, sizeof(rom), 1, fpt);
        }
    }
    fclose(fp);
    fclose(fpt);

    fp = fopen("bookings", "w");
    fpt = fopen("tempfile", "r");

    while (fread(&ubk, sizeof(ubk), 1, fpt))
    {
        fwrite(&ubk, sizeof(ubk), 1, fp);
    }
    fclose(fp);
    fclose(fpt);

    addroom();
    clear();

    return 0;
}

int checker()
{
    size = ftell(fp);

    if ((fp = fopen("sample.txt", "r")) != NULL)
    {
        if (0 == size)
        {
            userbooking();
        }
        else
        {
            while (fread(&ubk, sizeof(ubk), 1, fp))
            {
                if (strcmp(userid, ubk.guest) == 0)
                {
                    clear();
                    printf("\nSorry. You can't create two bookings.\nDelete/Edit Your Current Booking.\n\n");
                    editdelete();
                }
                else
                {
                    userbooking();
                }
            }
        }
    }
    else
    {
        fp = fopen("bookings", "w");
        userbooking();
        fclose(fp);
    }
}

int userbooking(void)
{
    printf("Let's find the best room for you!\n");
    printf("\nRoom Type (Single/Double): ");
    scanf("%s", ubk.roomtype);
    printf("Number of Adults : ");
    scanf("%s", ubk.numadults);
    printf("Number of Kids (Type 0 if None) : ");
    scanf(" %s", ubk.numkids);
    printf("Date of Checkin :  ");
    scanf(" %s", ubk.checkin);
    printf("Date of Checkout :  ");
    scanf(" %s", ubk.checkout);
    fp = fopen("bookings", "a");
    strcpy(ubk.guest, userid);
    fwrite(&ubk, sizeof(ubk), 1, fp);
    fclose(fp);
    clear();
    printf("\nDone. Room Registered Successfully.\n");
    mainmenu();
    return 0;
}

int viewuserbooking(void)
{
    int marker = 0;
    clear();
    fp = fopen("bookings", "r");
    while (fread(&ubk, sizeof(ubk), 1, fp))
    {
        if (strcmp(userid, ubk.guest) == 0)
        {
            printf("You have one active booking!\n\n");
            printf("Room\t\tNumber \t\tNumber \t\tCheck \t\tCheck \n");
            printf("Type\t\tof Adults\tof Kids\t\tIn Date\t\tOut Date\n");
            printf("------------------------------------------------------------------------------\n");
            printf("%s\t\t%s\t\t%s\t\t%s\t%s\n\n\n", ubk.roomtype, ubk.numadults, ubk.numkids, ubk.checkin, ubk.checkout);
            marker = 1;
        }
    }

    if (marker == 0)
    {
        printf("You don't have one active booking!\n");
    }
    fclose(fp);
    return 0;
}

int viewbooking(void)
{
    clear();
    fp = fopen("bookings", "r");
    while (fread(&ubk, sizeof(ubk), 1, fp))
    {
        if (fp != NULL)
        {
            printf("\n");
            printf("User  \tRoom\t\tNumber \t\tNumber \t\tCheck \t\tCheck \n");
            printf("      \tType\t\tof Adults\tof Kids\t\tIn Date\t\tOut Date\n");
            printf("------------------------------------------------------------------------------------\n");
            printf("%s\t%s\t\t%s\t\t%s\t\t%s\t%s\n", ubk.guest, ubk.roomtype, ubk.numadults, ubk.numkids, ubk.checkin, ubk.checkout);
        }
        else
        {
            printf("No Booking avilable.\n");
        }
    }
    fclose(fp);
    printf("\n\n=======================");
    printf("\n[1] Go to main menu.\n");
    printf("\n> ");
    scanf(" %s", option);

    if (strcmp(option, "1") == 0)
    {
        clear();
        adminmenu();
    }

    clear();

    return 0;
}

int viewdeletedbooking(void)
{

    size = ftell(fp);

    if ((fp = fopen("cancelled", "r")) != NULL)
    {
        if (0 == size)
        {
            clear();
            printf("No Deleted Bookings Found");
            printf("\n\n=======================");
            printf("\n[1] Go to main menu.\n");
            printf("\n> ");
            scanf(" %s", option);

            if (strcmp(option, "1") == 0)
            {
                clear();
                adminmenu();
            }
        }
        else
        {
            while (fread(&ubk, sizeof(ubk), 1, fp))
            {
                if (fp != NULL)
                {
                    printf("\n");
                    printf("User  \tRoom\t\tNumber \t\tNumber \t\tCheck \t\tCheck \n");
                    printf("      \tType\t\tof Adults\tof Kids\t\tIn Date\t\tOut Date\n");
                    printf("------------------------------------------------------------------------------------\n");
                    printf("%s\t%s\t\t%s\t\t%s\t\t%s\t%s\n", ubk.guest, ubk.roomtype, ubk.numadults, ubk.numkids, ubk.checkin, ubk.checkout);
                }
                else
                {
                    printf("No Booking avilable.\n");
                }
            }

            fclose(fp);
            printf("\n\n=======================");
            printf("\n[1] Go to main menu.\n");
            printf("\n> ");
            scanf(" %s", option);

            if (strcmp(option, "1") == 0)
            {
                clear();
                adminmenu();
            }
        }
    }
    else
    {
        clear();
        printf("No Deleted Bookings Found");
        printf("\n\n=======================");
        printf("\n[1] Go to main menu.\n");
        printf("\n> ");
        scanf(" %s", option);

        if (strcmp(option, "1") == 0)
        {
            clear();
            adminmenu();
        }
    }

    return 0;
}

int addroom(void)
{
    clear();
    printf("Room Number :  ");
    scanf(" %s", rom.roomnumber);
    printf("\nRoom Type (Single/Double): ");
    scanf("%s", rom.roomtype);
    printf("Room Capacity : ");
    scanf("%s", rom.roomcapacity);

    fp = fopen("rooms", "a");
    fwrite(&rom, sizeof(rom), 1, fp);
    fclose(fp);
    clear();

    printf("\nDone. Room Added Successfully.\n");
    adminmenu();
    return 0;
}

int mainmenu(void)
{
    printf("Welcome!\n\n");
    printf("[1] Book a New Room.\n");
    printf("[2] Edit/Cancel Your Booking.\n");
    printf("[3] View Your Profile.\n");
    printf("[4] Logout.\n");
    printf("\n> ");
    scanf(" %s", option);

    if (strcmp(option, "1") == 0)
    {
        clear();
        checker();
    }
    else if (strcmp(option, "2") == 0)
    {
        clear();
        editdelete();
    }
    else if (strcmp(option, "3") == 0)
    {
        profile();
    }

    else if (strcmp(option, "4") == 0)
    {
        clear();
        welcome();
    }
    else
    {
        clear();
        printf("Wrong Input\n\n");
        mainmenu();
    }

    return 0;
}

int adminmenu(void)
{
    printf("\n\n");
    printf("[1] Add New Room.\n");
    printf("[2] Modify a Room.\n");
    printf("[3] View Reservations.\n");
    printf("[4] View Cancelations.\n");
    printf("[5] Logout.\n");
    printf("\n> ");
    scanf(" %s", option);

    if (strcmp(option, "1") == 0)
    {
        clear();
        addroom();
    }
    else if (strcmp(option, "2") == 0)
    {
        clear();
        editroom();
    }
    else if (strcmp(option, "3") == 0)
    {
        viewbooking();
    }

    else if (strcmp(option, "4") == 0)
    {
        clear();
        viewdeletedbooking();
    }
    else if (strcmp(option, "5") == 0)
    {
        clear();
        welcome();
    }
    else
    {
        clear();
        printf("Wrong Input\n\n");
        mainmenu();
    }

    return 0;
}

int login(void)
{
    int marker = 0;
    printf("Please, Enter your Username :  ");
    scanf(" %s", userid);

    printf("Please, Enter your Password :  ");
    scanf(" %s", pass);

    fp = fopen("users", "r");

    while (fread(&uzr, sizeof(uzr), 1, fp))
    {
        if (strcmp(userid, uzr.usid) == 0 && strcmp(pass, uzr.upwd) == 0)
        {
            clear();
            mainmenu();
            marker = 1;
        }
    }

    if (marker == 0)
    {
        clear();
        printf("\nSorry, Wrong credentials.\n");
        login();
    }

    fclose(fp);
    return 0;
}

int welcome(void)
{
    printf("\nWelcome!\n");
    printf("\n- Type LOGIN to login");
    printf("\n- Press REGISTER to register\n\n> ");
    scanf("%s", option);

    if (strcmp(option, "login") == 0)
    {
        clear();
        login();
    }

    else if (strcmp(option, "register") == 0)
    {
        clear();
        printf("\nWe are happy to see you here.\n\nFirst, choose your Username: ");
        registeration();
    }

    else if (strcmp(option, "admin") == 0)
    {
        clear();
        printf("\nFirst, choose your username: ");
        scanf("%s", userid);
        printf("First, choose your password: ");
        scanf("%s", pass);

        if (strcmp(userid, "admin") == 0 && strcmp(pass, "pass") == 0)
        {
            clear();
            printf("\nWelcome to Admin Dashboard.\n");
            adminmenu();
        }
    }
    else
    {
        clear();
        printf("\nWrong Input.\n");
        welcome();
    }

    return 0;
}

int registeration(void)
{
    scanf("%s", validator);

    if (access("users", F_OK) != -1)
    {
        fp = fopen("users", "r");
        while (fread(&uzr, sizeof(uzr), 1, fp))
        {
            if (strcmp(validator, uzr.usid) == 0)
            {
                clear();
                printf("\nUsername is taken.\n\nUse another username.\n");
                registeration();
            }

            else
            {
                fp = fopen("users", "a");
                strcpy(uzr.usid, validator);
                printf("Please, Enter your password:  ");
                scanf("%s", uzr.upwd);
                printf("Please, Enter your Name:  ");
                scanf("%s", uzr.unme);
                printf("Please, Enter your Age:  ");
                scanf(" %s", uzr.uage);
                printf("Please, Enter your Gender:  ");
                scanf(" %s", uzr.gender);

                fp = fopen("users", "a");
                fwrite(&uzr, sizeof(uzr), 1, fp);
                fclose(fp);
                clear();
            }
        }
    }
    else
    {
        fp = fopen("users", "a");
        strcpy(uzr.usid, validator);
        printf("Please, Enter your password:  ");
        scanf("%s", uzr.upwd);
        printf("Please, Enter your Name:  ");
        scanf("%s", uzr.unme);
        printf("Please, Enter your Age:  ");
        scanf(" %s", uzr.uage);
        printf("Please, Enter your Gender:  ");
        scanf(" %s", uzr.gender);

        fp = fopen("users", "a");
        fwrite(&uzr, sizeof(uzr), 1, fp);
        fclose(fp);
        clear();
    }

    printf("\nDone. User Registered Successfully.\n");
    login();
    printf("\nNow, Login First.\n");
    return 0;
}

int files(void)
{
    fp = fopen("users", "r");
    if (fp == NULL)
    {
        fp = fopen("users", "w");
    }
    fclose(fp);
    return 0;
}

int main(void)
{
    clear();
    welcome();
}