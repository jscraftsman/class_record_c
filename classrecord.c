#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------------------*/
/* START VARIABLE DECLARATIONS -------------------------------------------------------*/
#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75 
#define RIGHT_KEY 77
#define ENTER_KEY 13

#define STUDENT_MENU_STATE 1
#define EXIT -1

int SELECTOR = 0,
    ID = 0,
    STATE = STUDENT_MENU_STATE;

char *STUDENT_MENU[80] = {"View all students", "Add a student", "Edit a student", "Delete a student", "Exit"};

typedef struct student{
     int id;
     char first_name[80];
     char last_name[80];
     int quiz;
     int hw;
     int attendance;
     int final_exam;
     int grade;
}Student;

typedef struct node{
     Student data;
     struct node *next;
     struct node *prev;
}Node;

Node *students;

/* END VARIABLE DECLARATIONS ---------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/* START FUNCTION DECLARATIONS -------------------------------------------------------*/
//helpers: ------------------------------------------------
char *getSelector(int id);
int cycleInput(int size);
void displayOptions(char *options[], int start, int end);

//student module: -----------------------------------------
void	showStudentOptions(int size);
void handleStudentMenuInput();
void printStudent(Student s);
Student createStudent();

//doubly linked list module:-------------------------------
void initNode(Node *head, Node *tail);
void addStudent(Node *pointer, Student data); 
Student findStudent(Node *pointer, int key); 
void deleteStudent(Node *pointer, int id); 
int getSize(Node *pointer);
/* END FUNCTION DECLARATIONS ---------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/* START HELPERS FUNCTONS ------------------------------------------------------------*/
char *getSelector(int id){ return ( (SELECTOR == id) ? ">>" : "  "); }

int cycleInput(int size){
	int INPUT = getch();	
	if(INPUT == UP_KEY)		SELECTOR = (SELECTOR <= 0) ? (size-1) : (SELECTOR - 1); 
	else if(INPUT == DOWN_KEY)	SELECTOR = (SELECTOR + 1) % size; 

	return (INPUT == ENTER_KEY) ? ENTER_KEY : -1;
}

void displayOptions(char *options[], int start, int end){
     int i = 0;
     for(i = start; i < end; i++){
          printf("\t\t\t   %s %s\n", getSelector(i), options[i]);
     }
}
/* END HELPERS FUNCTIONS -------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/* START DOUBLY LINKEDLIST FUNCTIONS -------------------------------------------------*/
void initNode(Node *student, Node *tail){
     student = (Node *)malloc(sizeof(Node)); 
     tail = student;
     tail->next = NULL;
     tail->prev = NULL;
     Student s = {-1, "", "", 0, 0, 0, 0, 0};
     student->data = s;
}
void addStudent(Node *pointer, Student data) {
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
     }
     pointer->next = (Node *)malloc(sizeof(Node));
     (pointer->next)->prev = pointer;
     pointer = pointer->next;
     pointer->data = data;
     pointer->next = NULL;
}

Student findStudent(Node *pointer, int key) {
     Student temp;
     temp.id = -1;
     
     pointer =  pointer -> next; 
     while(pointer!=NULL) {
          if(pointer->data.id == key){ 
               return pointer->data;
          }
          pointer = pointer -> next;
     }
     return temp;
}
void deleteStudent(Node *pointer, int id) {
     while(pointer->next!=NULL && (pointer->next)->data.id != id) {
          pointer = pointer -> next;
     }
     if(pointer->next==NULL) {
          printf("Student is not present in the list\n");
          return;
     }
     Node *temp;
     temp = pointer -> next;
     pointer->next = temp->next;
     temp->prev =  pointer;
     free(temp);
     return;
}

int getSize(Node *pointer){
     int count = 0;
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
          count++;
     }
     return count;
}
/* END DOUBLY LINKEDLIST FUNCTIONS ---------------------------------------------------*/
/*------------------------------------------------------------------------------------*/

/* Menu (Students) */
void	showStudentOptions(int size){
     int  start = 0,
          end = size;
	system("cls");
     printf("\n\t\t\t\tClass Record\n\n");
	printf("\t   (Use the arrow keys to navigate and press enter to select)\n\n");
     displayOptions(STUDENT_MENU, start, end);
}

void printStudent(Student s){
     printf("ID: %d", s.id);
     printf("\nFirstname: %s", s.first_name);
     printf("\nLastname: %s", s.last_name);
     printf("\nQuiz: %d", s.quiz);
     printf("\nHW: %d", s.hw);
     printf("\nAttendance: %d", s.attendance);
     printf("\nFinal exam: %d", s.final_exam);
     printf("\nGrade: %d\n", s.grade);
}

Student createStudent(){
     char input[80];
     Student s; 
     gets(input);
     puts(input);
     return s;     
}

void printStudentList(Node *pointer){
     int size = getSize(students); 
     printf("Size of list: %d\n", size);
     if(size == 0){
          printf("\nThe list is empty.\n");
     }else{
          printf("\nThe list is not empty\n");
     }

}
/* Input handler (Students) */
void handleStudentMenuInput(){
     int size  = 5;
     Student temp = {5, "ab", "cd", 1, 2, 3, 4, 5};
     do{ showStudentOptions(size); }while(cycleInput(size)!= ENTER_KEY);

     switch(SELECTOR){
          case 0: system("cls");
                  printStudentList(students);
                  getch();
                  break;
          case 1:
                  addStudent(students, temp);
                  break;
          case 4: printf("\nGoodbye..\n"); STATE = EXIT; break;
     }
}
/* State handler */
void handleState(){
     do{
          switch(STATE){
               case STUDENT_MENU_STATE: handleStudentMenuInput(); break;
               case EXIT: exit(0); break;
          }
     }while(STATE != EXIT);
}

int main(){
     Node *tail;
     Student dummy = {-1, "empty", "empty", 0, 0, 0, 0, 0};
     //initNode(students, tail); 
     students = (Node *)malloc(sizeof(Node)); 
     tail = students;
     tail->next = NULL;
     tail->prev = NULL;
     tail->data = dummy;

     handleState();
	getch();
	return 0;
}
