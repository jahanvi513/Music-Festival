//Music Festival Ticket Management System
//Assumption: Tickets are non-refundable, that is, cancellation services are not offered.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TABLE_SIZE 500

// Defining functions
int main();
void create(char nam[25], char gen[10], int id, int age, bool checkedIn);
void book_ticket();
void search_ticket();
void show_details(int ticket_id);
void last_used_id();
unsigned int hash(int id);

struct customer* hash_table[TABLE_SIZE];  // Hash table
int ticket = 0;  // Ticket counter
int last_id = 0;  // To keep track of booked tickets
int verification = 0;  // To track number of people entering

void saving_data();

// Defining structure
struct customer {
    char name[20];
    char gender[10];
    int id;
    int age;
    bool checkedIn;
    struct customer* next;
};

// Hash function
unsigned int hash(int id) {
    return id % TABLE_SIZE;
}

// Function to create a new customer entry
void create(char nam[25], char gen[10], int id, int age, bool checkedIn) {
    struct customer* new_customer = (struct customer*)malloc(sizeof(struct customer));
    strcpy(new_customer->name, nam);
    strcpy(new_customer->gender, gen);
    new_customer->id = id;
    new_customer->age = age;
    new_customer->checkedIn = checkedIn;
    new_customer->next = NULL;

    int index = hash(id);
    if (hash_table[index] == NULL) {
        hash_table[index] = new_customer;
    } else {
        struct customer* temp = hash_table[index];
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = new_customer;
    }

    // Updating last id used
    if (id > last_id) {
        last_id = id;
    }
}

// Function to book tickets
void book_ticket() {
    int n;
    printf("\tEvent: Music Festival\n");
    printf("\tDate: 13 May 2023\n");
    printf("\tPrice: 200\n");
    printf("\nEnter number of tickets: ");
    scanf("%d", &n);

    char nam[25], gen[10];
    int age, id;
    for (int i = 1; i <= n; i++) {
        // Incrementing value of ticket to generate unique ticket id
        ticket += 1;

        // Checking for availability of tickets
        if (ticket > TABLE_SIZE) {
            printf("\nSOLD OUT\n");
            return;
        }

        // Taking customer details if tickets are available
        id = ticket;
        printf("\nEnter Name: ");
        scanf("%s", nam);
        printf("\nEnter Gender: ");
        scanf("%s", gen);
        printf("\nEnter Age: ");
        scanf("%d", &age);
        create(nam, gen, id, age, false);
    }
    saving_data();
    printf("\nBooking Successful!\nThank you for booking tickets!\n\n");
}

// Function to display details of tickets
void show_details(int t) {
    int index = hash(t);
    struct customer* temp = hash_table[index];
    bool notFound = true;  // To keep track if ticket id is not found

    while (temp != NULL) {
        if (temp->id == t) {
            notFound = false;
            printf("\nTicket ID: %d\n", temp->id);
            printf("Name: %s\n", temp->name);
            printf("Gender: %s\n", temp->gender);
            printf("Age: %d\n", temp->age);
            printf("Checked in: %d\n", temp->checkedIn);
        }
        temp = temp->next;
    }

    if (notFound) {
        printf("Ticket not found\n");
    }
}

// Function to search for ticket
void search_ticket() {
    int t;
    printf("Enter Ticket ID: ");
    scanf("%d", &t);

    int index = hash(t);
    struct customer* temp = hash_table[index];
    bool foundUser = false;   // To keep track if ticket id is not found

    printf("Total checked in users: %d\n", verification);
    while (temp != NULL) {
        if (temp->id == t) {
            foundUser = true;
            printf("Valid Ticket ID\n");
            if (!temp->checkedIn) {
                printf("Checking in user\n");
                temp->checkedIn = true;
                verification += 1;
                saving_data();
            } else {
                printf("User already checked in\n");
            }
            show_details(t);
            break;
        }
        temp = temp->next;
    }

    if (!foundUser) {
        printf("\nInvalid ID\n");
    }
}

// Function to store details in txt file
void saving_data() {
    FILE *fptr;
    fptr = fopen("Customer_details.txt", "w");

    for (int i = 0; i < TABLE_SIZE; i++) {
        struct customer* temp = hash_table[i];
        while (temp != NULL) {
            fprintf(fptr, "%d\t%s\t%s\t%d\t%d\n", temp->id, temp->name, temp->gender, temp->age, temp->checkedIn);
            temp = temp->next;
        }
    }

    fclose(fptr);
}

// Function to read text file and check the last ticket id used
void last_used_id() {
    FILE *fptr;
    fptr = fopen("Customer_details.txt", "r");

    if (fptr == NULL) {
        return;
    }

    int id;
    char name[25], gender[10];
    int age;
    int checkedIn;

    while (fscanf(fptr, "%d\t%s\t%s\t%d\t%d\n", &id, name, gender, &age, &checkedIn) == 5) {
        if (id > last_id) {
            last_id = id;
        }
        create(name, gender, id, age, checkedIn);
        if (checkedIn) {
            verification += 1;
        }
    }

    fclose(fptr);
}

// Driver code
int main() {
    last_used_id();
    ticket = last_id;
    int choice;
    printf("*****\n");
    printf("\tMUSIC FESTIVAL TICKET BOOKING\n");
    printf("*****\n");
    printf("\tMAIN MENU\n");
    printf("\t1. Book Ticket\n");
    printf("\t2. Search Ticket\n");
    printf("\t3. Exit\n");
    printf("Enter Choice: ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            book_ticket();
            break;
        case 2:
            search_ticket();
            break;
        case 3:
            printf("Thank you for using our ticket booking system!\n");
            break;
        default:
            printf("Invalid Input\n");
    }

    if (choice != 3) {
        main();
    }

    return 0;
}

