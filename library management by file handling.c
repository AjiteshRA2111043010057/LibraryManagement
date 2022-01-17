#include <stdio.h>
#include <conio.h>
#include <string.h>
#include <stdlib.h>
typedef struct
{
    int yyyy;
    int mm;
    int dd;
} Date;
typedef struct
{
    char username[30];
    char password[20];
} sFileHeader;
typedef struct
{
    int books_id;
    char bookName[50];
    char authorName[50];
    char studentName[50];
    char studentAddr[300];
    Date bookIssueDate;
} s_BooksInfo;
void printMessageCenter(const char* message)
{
    int len =0;
    int pos = 0;
    len = (78 - strlen(message))/2;
    printf("\t\t\t");
    for(pos =0 ; pos < len ; pos++)
    {
        printf(" ");
    }
    printf("%s",message);
}
void headMessage(const char *message)
{
    system("cls");
    printf  ("\t\t\t***************************************************************************");
    printf("\n\t\t\t-=-=-=-=-=-=      Library management System Project in C       =-=-=-=-=-=-");
    printf("\n\t\t\t***************************************************************************");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
    printMessageCenter(message);
    printf("\n\t\t\t----------------------------------------------------------------------------");
}
int isNameValid(const char *name)
{
    int vN=1;
    int l=0;
    int i=0;
    l= strlen(name);
    for(i=0;i<l;i++)
    {
        if(!(isalpha(name[i])) && (name[i]!='\n') && (name[i]!=' '))
        {
            vN=0;
            break;
        }
    }
    return vN;
}
int  IsLeapYear(int year)
{
    return (((year % 4 == 0) &&
             (year % 100 != 0)) ||
            (year % 400 == 0));
}
int isValidDate(Date *validDate)
{
    if (validDate->yyyy > 9999 ||
            validDate->yyyy < 1900)
        return 0;
    if (validDate->mm < 1 || validDate->mm > 12)
        return 0;
    if (validDate->dd < 1 || validDate->dd > 31)
        return 0;
    if (validDate->mm == 2)
    {
        if (IsLeapYear(validDate->yyyy))
            return (validDate->dd <= 29);
        else
            return (validDate->dd <= 28);
    }
    if (validDate->mm == 4 || validDate->mm == 6 ||
            validDate->mm == 9 || validDate->mm == 11)
        return (validDate->dd <= 30);
    return 1;
}

void addbook()
{
    int days;
    s_BooksInfo addBookInfoInDataBase={0};
    FILE *fr = NULL;
    int status = 0;
    fr = fopen("file.bin","ab+");
    if(fr == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    headMessage("ADD NEW BOOKS");
    printf("\n\n\t\t\tENTER YOUR DETAILS BELOW:");
    printf("\n\t\t\t---------------------------------------------------------------------------\n");
    printf("\n\t\t\tBook ID NO  = ");
    fflush(stdin);
    scanf("%u",&addBookInfoInDataBase.books_id);
    do
    {
        printf("\n\t\t\tBook Name  = ");
        fflush(stdin);
        fgets(addBookInfoInDataBase.bookName,50,stdin);
        status = isNameValid(addBookInfoInDataBase.bookName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        printf("\n\t\t\tAuthor Name  = ");
        fflush(stdin);
        fgets(addBookInfoInDataBase.authorName,50,stdin);
        status = isNameValid(addBookInfoInDataBase.authorName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        printf("\n\t\t\tStudent Name  = ");
        fflush(stdin);
        fgets(addBookInfoInDataBase.studentName,50,stdin);
        status = isNameValid(addBookInfoInDataBase.studentName);
        if (!status)
        {
            printf("\n\t\t\tName contain invalid character. Please enter again.");
        }
    }
    while(!status);
    do
    {
        printf("\n\t\t\tEnter date in format (day/month/year): ");
        scanf("%d/%d/%d",&addBookInfoInDataBase.bookIssueDate.dd,&addBookInfoInDataBase.bookIssueDate.mm,&addBookInfoInDataBase.bookIssueDate.yyyy);
        status = isValidDate(&addBookInfoInDataBase.bookIssueDate);
        if (!status)
        {
            printf("\n\t\t\tPlease enter a valid date.\n");
        }
    }
    while(!status);
    fwrite(&addBookInfoInDataBase,sizeof(addBookInfoInDataBase), 1, fr);
    fclose(fr);
}
void searchbook()
{
    int found = 0;
    char bookName[50] = {0};
    s_BooksInfo addBookInfoInDataBase = {0};
    FILE *fr = NULL;
    int status = 0;
    fr=fopen("file.bin","rb");
    if(fr==NULL)
    {
        printf("\n\t\t\tFile is not opened\n");
        exit(1);
    }
    headMessage("SEARCH BOOKS");
    if (fseek(fr,sizeof(sFileHeader),SEEK_SET) != 0)
    {
        fclose(fr);
        printf("\n\t\t\tFacing issue while reading file\n");
        exit(1);
    }
    printf("\n\n\t\t\tEnter Book Name to search:");
    fflush(stdin);
    fgets(bookName,50,stdin);
    while (fread (&addBookInfoInDataBase, sizeof(addBookInfoInDataBase), 1, fr))
    {
        if(!strcmp(addBookInfoInDataBase.bookName, bookName))
        {
            found = 1;
            break;
        }
    }
    if(found)
    {
        printf("\n\t\t\tBook id = %u\n",addBookInfoInDataBase.books_id);
        printf("\t\t\tBook name = %s",addBookInfoInDataBase.bookName);
        printf("\t\t\tBook authorName = %s",addBookInfoInDataBase.authorName);
        printf("\t\t\tBook issue date(day/month/year) =  (%d/%d/%d)",addBookInfoInDataBase.bookIssueDate.dd,
               addBookInfoInDataBase.bookIssueDate.mm, addBookInfoInDataBase.bookIssueDate.yyyy);
    }
    else
    {
        printf("\n\t\t\tNo Record");
    }
    fclose(fr);
    printf("\n\n\n\t\t\tPress any key to go to main menu.....");
    getchar();
}
void viewbook()
{
    int found = 0;
    char bookName[50] = {0};
    s_BooksInfo addBookInfoInDataBase = {0};
    FILE *fr = NULL;
    int status = 0;
    int countBook = 1;
    headMessage("VIEW BOOKS DETAILS");
    fr = fopen("file.bin","rb");
    if(fr == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    if (fseek(fr,sizeof(sFileHeader),SEEK_SET) != 0)
    {
        fclose(fr);
        printf("Facing issue while reading file\n");
        exit(1);
    }
    while (fread (&addBookInfoInDataBase, sizeof(addBookInfoInDataBase), 1, fr))
    {
        printf("\n\t\t\tBook Count = %d\n\n",countBook);
        printf("\t\t\tBook id = %u",addBookInfoInDataBase.books_id);
        printf("\n\t\t\tBook name = %s",addBookInfoInDataBase.bookName);
        printf("\t\t\tBook authorName = %s",addBookInfoInDataBase.authorName);
        printf("\t\t\tBook issue date(day/month/year) =  (%d/%d/%d)\n\n",addBookInfoInDataBase.bookIssueDate.dd,
               addBookInfoInDataBase.bookIssueDate.mm, addBookInfoInDataBase.bookIssueDate.yyyy);
        found = 1;
        ++countBook;
    }
    fclose(fr);
    if(!found)
    {
        printf("\n\t\t\tNo Record");
    }
    printf("\n\n\t\t\tPress any key to go to main menu.....");
    fflush(stdin);
    getchar();
}
void deletebook()
{
    int found = 0;
    int bookDelete = 0;
    sFileHeader fileHeaderInfo = {0};
    char bookName[50] = {0};
    s_BooksInfo addBookInfoInDataBase = {0};
    FILE *fr = NULL;
    FILE *tmpFp = NULL;
    int status = 0;
    headMessage("Delete Books Details");
    fr = fopen("file.bin","rb");
    if(fr == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    tmpFp = fopen("tmp.bin","wb");
    if(tmpFp == NULL)
    {
        fclose(fr);
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,sizeof(sFileHeader), 1, fr);
    fwrite(&fileHeaderInfo,sizeof(sFileHeader), 1, tmpFp);
    printf("\n\t\t\tEnter Book ID NO. for delete:");
    scanf("%d",&bookDelete);
    while (fread (&addBookInfoInDataBase, sizeof(addBookInfoInDataBase), 1, fr))
    {
        if(addBookInfoInDataBase.books_id != bookDelete)
        {
            fwrite(&addBookInfoInDataBase,sizeof(addBookInfoInDataBase), 1, tmpFp);
        }
        else
        {
            found = 1;
        }
    }
    (found)? printf("\n\t\t\tRecord deleted successfully....."):printf("\n\t\t\tRecord not found");
    fclose(fr);
    fclose(tmpFp);
    remove("file.bin");
    rename("tmp.bin","file.bin");
    printf ("\n\n\n\t\t\tPlease enter to continue.....");
    getch();
}
void updatebook()
{
    sFileHeader fileHeaderInfo = {0};
    FILE *fr = NULL;
    char userName[50] = {0};
    char password[50] = {0};
    headMessage("Update Credential");
    fr = fopen("file.bin","rb+");
    if(fr == NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,sizeof(sFileHeader), 1, fr);
    if (fseek(fr,0,SEEK_SET) != 0)
    {
        fclose(fr);
        printf("\n\t\t\tFacing issue while updating password\n");
        exit(1);
    }
    printf("\n\n\t\t\tNew Username:");
    fflush(stdin);
    fgets(userName,30,stdin);
    printf("\n\n\t\t\tNew Password:");
    fflush(stdin);
    fgets(password,20,stdin);
    strncpy(fileHeaderInfo.username,userName,sizeof(userName));
    strncpy(fileHeaderInfo.password,password,sizeof(password));
    fwrite(&fileHeaderInfo,sizeof(sFileHeader), 1, fr);
    fclose(fr);
    printf("\n\t\t\tYour Password has been changed successfully");
    printf("\n\t\t\ttLogin Again:");
    fflush(stdin);
    getchar();
    exit(1);
}
void menu()
{
    int choice = 0;
    do
    {
        headMessage("MAIN MENU");
        printf("\n\n\n\t\t\t1.Add Books");
        printf("\n\t\t\t2.Search Books");
        printf("\n\t\t\t3.View Books");
        printf("\n\t\t\t4.Delete Book");
        printf("\n\t\t\t5.Update Password");
        printf("\n\t\t\t0.Exit");
        printf("\n\n\n\t\t\tEnter choice => ");
        scanf("%d",&choice);
        switch(choice)
        {
        case 1:
            addbook();
            break;
        case 2:
            searchbook();
            break;
        case 3:
            viewbook();
            break;
        case 4:
            deletebook();
            break;
        case 5:
            updatebook();
            break;
        case 0:
            printf("\n\n\n\t\t\t\tThank you!!!\n\n\n\n\n");
            exit(1);
            break;
        default:
            printf("\n\n\n\t\t\tINVALID INPUT!!! Try again...");
        }
    }
    while(choice!=0);
}

void login()
{
    char userName[30]={0};
    char password[20]={0};
    int L=0;
    sFileHeader fileHeaderInfo={0};
    FILE *fr=NULL;
    headMessage("Login");
    fr=fopen("file.bin","rb");
    if(fr==NULL)
    {
        printf("File is not opened\n");
        exit(1);
    }
    fread (&fileHeaderInfo,sizeof(sFileHeader),1, fr);
    fclose(fr);
    do
    {
        printf("\n\n\n\t\t\t\tUsername:");
        fgets(userName,30,stdin);
        printf("\n\t\t\t\tPassword:");
        fgets(password,20,stdin);
        if((!strcmp(userName,fileHeaderInfo.username)) && (!strcmp(password,fileHeaderInfo.password)))
        {
            menu();
        }
        else
        {
            printf("\t\t\t\tLogin Failed Enter Again UserName & Password\n\n");
            L++;
        }
    }
    while(L<=3);
    if(L>3)
    {
        headMessage("Login Failed");
        printf("\t\t\t\tSorry,Unknown User.");
        getch();
        system("cls");
    }
}
int isFileExists(const char *path)
{
    FILE *fr=fopen(path,"rb");
    int status=0;
    if (fr != NULL)
    {
        status = 1;
        fclose(fr);
    }
    return status;
}
void init()
{
    FILE *fr = NULL;
    int status = 0;
    const char defaultUsername[] ="Ajitesh\n";
    const char defaultPassword[] ="admin\n";
    sFileHeader fileHeaderInfo={0};
    status = isFileExists("file.bin");
    if(!status)
    {
        fr = fopen("file.bin","wb");
        if(fr!=NULL)
        {
            strncpy(fileHeaderInfo.password,defaultPassword,sizeof(defaultPassword));
            strncpy(fileHeaderInfo.username,defaultUsername,sizeof(defaultUsername));
            fwrite(&fileHeaderInfo,sizeof(sFileHeader), 1, fr);
            fclose(fr);
        }
    }
}
int main()
{
    init();
    login();
    return 0;
}
