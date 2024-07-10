//Music Festival Ticket Management System
//Assumption: Tickets are non-refundable, that is, cancellation services are not offered.

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

//Defining functions
int main();
void create(char nam[25], char gen[10], int id, int age, bool checkedIn);
void book_ticket();
void search_ticket();
void show_details(int ticket_id);
void last_used_id();

int max_limit = 500;  //Predefined number of tickets
struct customer*head = NULL;  //Initializing head of linked list
int ticket=0;  //Ticket counter
int last_id=0;  //To keep track of booked tickets
int verification = 0;  //To track number of people entering

void saving_data(struct customer*);

//Defining structure
struct customer{
    char name[20];
    char gender[10];
    int id;
    int age;
    bool checkedIn;
    struct customer*link;
};

//Function to create node for each customer
void create(char nam[25], char gen[10], int id, int a, bool checkedIn)
{
    struct customer *newptr = NULL, *ptr;
    newptr = (struct customer*)malloc(sizeof(struct customer));

    strcpy(newptr->name, nam);
    strcpy(newptr->gender, gen);
    newptr->id = id;
    newptr->age = a;
    newptr->checkedIn  = checkedIn;
    newptr->link = NULL;

    if (head == NULL)
        head = newptr;
    else
    {
        ptr = head;
        while (ptr->link != NULL)
            ptr = ptr->link;
        ptr->link = newptr;
    }
    //updating last id used
    if(id>last_id)
    last_id=id;
}

//Function to book ticket
void book_ticket()
{
    int n;
    printf("\tEvent:Music Festival\n");
    printf("\tDate:13 May 2023\n");
    printf("\tPrice:200\n");
    printf("\nEnter number of tickets:");
    scanf("%d", &n);

    char nam[25], gen[10];
    int a, id;
    for(int i=1; i<=n; i++)
    {
        //incrementing value of ticket to generate unique ticket id
        ticket+=1;

        //checking for availability of tickets
        if(ticket>max_limit)
        {
            printf("\nSOLD OUT\n");
        }
        //taking customer details if tickets are available
        else
        {
        id=ticket;
        printf("\nEnter Name:");
        scanf("%s", nam);
        printf("\nEnter Gender:");
        scanf("%s", gen);
        printf("\nEnter Age:");
        scanf("%d", &a);
        create(nam, gen, id, a, 0);
        }
    }
    saving_data(head);
    printf("\nBooking Successful!\nThank you for booking tickets!\n\n");
}

//Function to display details of tickets
void show_details(int t)
{
    struct customer* ptr = head;
    bool notFound=1;  //To keep track if ticket id is not found
    while (ptr != NULL)
    {
        if (ptr->id == t)
        {
            notFound=0;
            printf("\nTicket ID: %d\n", ptr->id);
            printf("Name: %s\n", ptr->name);
            printf("Gender: %s\n", ptr->gender);
            printf("Age: %d\n", ptr->age);
            printf("Checked in: %d\n", ptr->checkedIn);
        }
        ptr = ptr->link;
    }
    if(notFound)
    printf("Ticket not found\n");
}

//Function to search for ticket
void search_ticket()
{
    int t;
    printf("Enter Ticket ID:");
    scanf("%d", &t);
    struct customer* ptr = head;
    bool foundUser = 0;   //To keep track if ticket id is not found
    printf("Total checked in users: %d \n", verification);
    while(ptr!=NULL)
    {
        if(ptr->id==t)
        {
          foundUser = 1;
          printf("Valid Ticket id\n");
          if (ptr->checkedIn == 0)
          {
            printf("Checking in user\n");
            ptr->checkedIn = 1;
            verification += 1;
            saving_data(head);
          }
          else
          {
            printf("User already checked in\n");
          }
          show_details(t);
        }
        ptr = ptr->link;
    }
    if(foundUser == 0) { 
      printf("\nInvalid ID\n");
    }
}

//Function to store details in txt file
void saving_data(struct customer* head)
{
    FILE *fptr;
    fptr = fopen("Customer_details.txt", "w");

    while(head!=NULL)
    {
        fprintf(fptr, "%d\t", head->id);
        fprintf(fptr, "%s\t", head->name);
        fprintf(fptr, "%s\t", head->gender);
        fprintf(fptr, "%d\t", head->age);
        fprintf(fptr, "%d\n", head->checkedIn);
        head = head->link;
    }
    fclose(fptr);
}

//Function to read text file and check the last ticket id used
void last_used_id()
{
    head=NULL;
    verification = 0;
    FILE *fptr;
    fptr = fopen("Customer_details.txt", "r");

    if (fptr == NULL) {
        return;
    }

    int id;
    char name[25], gender[10];
    int age;
    int checkedIn;

    while(fscanf(fptr, "%d\t%s\t%s\t%d\t%d\n", &id, name, gender, &age, &checkedIn) == 5)  //fscanf is reading 5 values from the file
    {
        if (id > last_id)
            last_id = id;
        create(name, gender, id, age, checkedIn);
        if (checkedIn == 1)
          verification += 1;
    }

    fclose(fptr);
}

//Driver Code
int main()
{
    last_used_id();
    ticket = last_id;
    int choice;
    printf("*****\n");
    printf("\tMUSIC FESTIVAL TICKET BOOKING\t\n");
    printf("*****\n");
    printf("\tMAIN MENU\n");
    printf("\t1.Book Ticket\n");
    printf("\t2.Search Ticket\n");
    printf("\t3.Exit\n");
    printf("Enter Choice:");
    scanf("%d", &choice);

    switch(choice)
    {
        case 1:
        book_ticket();
        break;

        case 2:
        search_ticket();
        break;

        case 3:
        printf("Thank you using our ticket booking system!\n");
        break;

        default:
        printf("Invalid Input\n");
    }
    if(choice!=3)
    main();
    return 0;
}
