#include <stdio.h>
#include <string.h>

/*------------------------------------------------------------------------------------*/
/*------------------------------ VARIABLE DECLARATIONS (START) -----------------------*/
#define UP_KEY 72
#define DOWN_KEY 80
#define LEFT_KEY 75 
#define RIGHT_KEY 77
#define ENTER_KEY 13
#define ESC_KEY 27

#define STUDENT_MENU_STATE 1
#define CLASS_MENU_STATE 2
#define EXIT -1

int SELECTOR = 0,
    ID = 0,
    classID = 0,
    CURRENT_STUDENT = 0,
    CURRENT_CLASS = 0,
    STATE = CLASS_MENU_STATE;

char *STUDENT_MENU[80] = {"View all students", "Find a student", "Add a student", "Menu"};
char *SELECT_STUDENT_OPTIONS[80] = {"View", "Edit", "Delete", "Cancel"};
char *EDIT_STUDENT_OPTIONS[80] = {"Firstname", "Lastname", "Quiz", "Homework/Seatwork", "Attendance", "Final exam", "Cancel"};
char *CLASS_MENU[80] = {"View all class", "Add a class", "Exit"};

typedef struct student{
     int id;
     char firstname[80];
     char lastname[80];
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
}StudentNode;

typedef struct class{
     int id;
     char name[80];
     StudentNode *students;
}Class;

typedef struct cNode{
     Class data;
     struct cNode *next;
     struct cNode *prev;
}ClassNode;

StudentNode *students, *searchResults;
ClassNode *classes;
/*------------------------------- VARIABLE DECLARATIONS (END) ------------------------*/
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/*------------------------------ FUNCTION DECLARATIONS (START) -----------------------*/
//helpers: ------------------------------------------------
char *getSelector(int id);
char *getSelectorAndStudentID(int id, int student_id);
char *getSelectorAndClassID(int id, int class_id);
int cycleInput(int size);
void displayOptions(char *options[], int start, int end);
int isNumber(char str[]);
int getNumber(char *msg);

//student module: -----------------------------------------
void	showStudentOptions(int size);
void handleStudentMenuInput();
void printStudent(Student s);
void printStudentForTable(Student s, int select_id);
Student createStudent();
int computeGrade(Student s);
void handleAddStudent();
void handleFindStudent();
void handleViewAllStudents();
void showSelectedStudentOptions();
void handleDeleteStudent();
void handleEditStudent();

//class module:--------------------------------------------
void printClassForTable(Class c, int select_id);
void handleViewAllClass();
void printClassList(ClassNode *pointer);
void handleAddClass();
void	showClassOptions(int size);
void handleClassMenuInput();

//doubly linked list module:-------------------------------
StudentNode *createStudentNode();
void addStudent(StudentNode *pointer, Student data); 
void addSearchResult(StudentNode *pointer, Student data);
void findStudent(StudentNode *pointer, char key[]); 
void deleteStudent(StudentNode *pointer, int id); 
int getSize(StudentNode *pointer);
Student getStudent(StudentNode *pointer,int id);
void editStudent(StudentNode *pointer, int id, void* newData, int selector);

ClassNode *createClassNode();
void addClass(ClassNode *pointer, Class data);
int getClassSize(ClassNode *pointer);

//state module: -------------------------------------------
void handleState();
/*------------------------------- FUNCTION DECLARATIONS (END) ------------------------*/
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/*-------------------------------- HELPERS FUNCTONS (START) --------------------------*/
char *getSelector(int id){ return ( (SELECTOR == id) ? ">>" : "  "); }
char *getSelectorAndStudentID(int id, int student_id){
     if(SELECTOR == id){
          CURRENT_STUDENT = student_id;
          return ">>";
     }else{
          return "  ";
     }
}
char *getSelectorAndClassID(int id, int class_id){
     if(SELECTOR == id){
          CURRENT_CLASS = class_id;
          return ">>";
     }else{
          return "  ";
     }
}

int cycleInput(int size){
	int INPUT = getch();	
     if(size > 0){
          if(INPUT == UP_KEY)		SELECTOR = (SELECTOR <= 0) ? (size-1) : (SELECTOR - 1); 
          else if(INPUT == DOWN_KEY)	SELECTOR = (SELECTOR + 1) % size; 
     } else if(INPUT == ESC_KEY) return INPUT;
	return (INPUT == ENTER_KEY) ? ENTER_KEY : -1;
}

void displayOptions(char *options[], int start, int end){
     int i = 0;
     for(i = start; i < end; i++){
          printf("\t\t\t   %s %s\n", getSelector(i), options[i]);
     }
}

int isNumber(char str[]){
     int i = 0;
     do{
          if(!isdigit(str[i++])){
               return 0;
          }
     }while(i < strlen(str));
     return 1;
}

int getNumber(char *msg){
     char input[] = "0";
     do{
          if(!isNumber(input)) printf("(Incorrect input! Please try again)\n");
          printf("%s", msg);
          gets(input);
     }while(!isNumber(input));
     return (atoi(input));     
}
/*--------------------------------- HELPERS FUNCTIONS (END) --------------------------*/
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/*---------------------------- DOUBLY LINKEDLIST FUNCTIONS (START) -------------------*/

StudentNode *createStudentNode(){
     StudentNode *head, *tail;
     head = (StudentNode *)malloc(sizeof(StudentNode)); 
     tail = head;
     tail->next = NULL;
     tail->prev = NULL;
     return head;
}
void addStudent(StudentNode *pointer, Student data) {
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
     }
     pointer->next = (StudentNode *)malloc(sizeof(StudentNode));
     (pointer->next)->prev = pointer;
     pointer = pointer->next;
     data.id = ID++;
     data.grade = computeGrade(data);
     pointer->data = data;
     pointer->next = NULL;
}
void addSearchResult(StudentNode *pointer, Student data) {
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
     }
     pointer->next = (StudentNode *)malloc(sizeof(StudentNode));
     (pointer->next)->prev = pointer;
     pointer = pointer->next;
     pointer->data = data;
     pointer->next = NULL;
}
void findStudent(StudentNode *pointer, char key[]) {
     pointer =  pointer -> next; 
     searchResults = createStudentNode();
     while(pointer!=NULL) {
          if((strcasecmp(pointer->data.firstname, key) == 0) || (strcasecmp(pointer->data.lastname, key) == 0)){ 
               addSearchResult(searchResults, pointer->data);
          }
          pointer = pointer -> next;
     }
}
Student getStudent(StudentNode *pointer, int id){
     Student temp = {-1, "", "", 1, 1, 1, 1, 1};
     pointer =  pointer -> next; 
     while(pointer!=NULL) {
          if((pointer->data.id == id)){ 
               return pointer->data;
          }
          pointer = pointer -> next;
     }
     return temp;
}
void deleteStudent(StudentNode *pointer, int id) {
     while(pointer->next!=NULL && (pointer->next)->data.id != id) {
          pointer = pointer -> next;
     }
     if(pointer->next==NULL) {
          printf("Student is not present in the list\n");
          return;
     }
     StudentNode *temp;
     temp = pointer -> next;
     pointer->next = temp->next;
     temp->prev =  pointer;
     free(temp);
     return;
}
void editStudent(StudentNode *pointer, int id, void* newData, int selector){
     pointer =  pointer -> next; 
     while(pointer!=NULL) {
          if(pointer->data.id == id){ 
               switch(selector){
                    case 0: strcpy(pointer->data.firstname, (char *)newData); break;
                    case 1: strcpy(pointer->data.lastname, (char *)newData); break;
                    case 2: pointer->data.quiz = newData; break;
                    case 3: pointer->data.hw = newData; break;
                    case 4: pointer->data.attendance = newData; break;
                    case 5: pointer->data.final_exam = newData; break;
               }
               pointer->data.grade = computeGrade(pointer->data);
               break;
          }
          pointer = pointer -> next;
     }
}
int getSize(StudentNode *pointer){
     int count = 0;
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
          count++;
     }
     return count;
}

ClassNode *createClassNode(){
     ClassNode *head, *tail;
     head = (ClassNode *)malloc(sizeof(ClassNode)); 
     tail = head;
     tail->next = NULL;
     tail->prev = NULL;
     return head;
}
void addClass(ClassNode *pointer, Class data) {
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
     }
     pointer->next = (ClassNode *)malloc(sizeof(StudentNode));
     (pointer->next)->prev = pointer;
     pointer = pointer->next;
     data.id = classID++;
     pointer->data = data;
     pointer->next = NULL;
}
int getClassSize(ClassNode *pointer){
     int count = 0;
     while(pointer->next!=NULL) {
          pointer = pointer -> next;
          count++;
     }
     return count;
}

/*----------------------------- DOUBLY LINKEDLIST FUNCTIONS (END) --------------------*/
/*------------------------------------------------------------------------------------*/
/*------------------------------------------------------------------------------------*/
/*------------------------------- STUDENT MODULE (START) -----------------------------*/

void	showStudentOptions(int size){
     int  start = 0,
          end = size;
	system("cls");
     printf("\n\t\t\t\tClass Record\n\n");
	printf("\t   (Use the arrow keys to navigate and press enter to select)\n\n");
     displayOptions(STUDENT_MENU, start, end);
}
/* Print a student, select_id is for the id of the student in the list */
void printStudentForTable(Student s, int select_id){
     printf("\n\t %s %s %s", getSelectorAndStudentID(select_id, s.id), s.firstname, s.lastname, s.grade);
}
void printStudent(Student s){
     system("cls");
     printf("\n\n\t\t\t\tStudent: %s %s\n", s.firstname, s.lastname);
     printf("\n\tFirstname: %s", s.firstname);
     printf("\n\tLastname %s\n", s.lastname);
     printf("\n\t[Scores]\n\tQuizzes: %d", s.quiz);
     printf("\n\tHomework/Seatwork:  %d", s.hw);
     printf("\n\tAttendance: %d", s.attendance);
     printf("\n\tFinal exam:  %d\n", s.final_exam);
     printf("\n\tGrade: %d\n\n", s.grade);
}
/* Create a student based from the input of the user. Trap if input is not a number */
Student createStudent(){
     char input[80];
     Student s; 
     system("cls");
     printf("\n\t\t\t\tCreate a new student: \n\n");
     printf("Enter firstname: ");
     gets(s.firstname);
     printf("Enter lastname: ");
     gets(s.lastname);

     s.quiz = getNumber("Enter score in quizzes: ");
     s.attendance = getNumber("Enter attendance: ");
     s.hw = getNumber("Enter score Homework/Seatwork: ");
     s.final_exam = getNumber("Enter score in final exam: ");
     return s;     
}
void printStudentList(StudentNode *pointer){
     int size = getSize(students); 
     int i = 0;
     printf("\n\n\t\t\t\tList of students:");
     if(size == 0){
          printf("\n\t\t\t\tThe list is empty\n");
          printf("\n\t\t\t    (Press enter to continue)\n");
     }else{
          while(pointer->next!=NULL) {
               pointer = pointer -> next;
               printStudentForTable(pointer->data, i++);
          }
     }
}
void handleAddStudent(){
     Student temp = createStudent(); 
     addStudent(students, temp);
     printf("\nStudent %s was added to the list. (Press any key to continue)\n", temp.firstname);
}
void handleFindStudent(){
     char input[80];
     system("cls");
     printf("\n\n\t\t\t\tSearch a student\n");
     printf("\nEnter firstname or lastname: ");
     gets(input);
     findStudent(students, input) ;

     system("cls");
     printf("\n\n\t\t\t\tSearch results\n");
     printf("\nStudents found: %d\n", (getSize(searchResults) > 0 ? getSize(searchResults) : 0));
     printStudentList(searchResults);
}
void showSelectedStudentOptions(){
     Student s = getStudent(students, CURRENT_STUDENT);
     do{
          system("cls");
          printf("\n\n\t\t\tStudent: %s %s", s.firstname, s.lastname);
          printf("\n\n\t\t\tWhat would you like to do?\n\n");
          displayOptions(SELECT_STUDENT_OPTIONS, 0, 4);
     }while(cycleInput(4) != ENTER_KEY);
}
void handleDeleteStudent(){
     char in[20];
     printf("\nAre you sure you want to delete this student? [Y/N]: ");
     gets(in);
     if(strcmp(in, "Y") == 0 || strcmp(in, "y") == 0) {
          deleteStudent(students, CURRENT_STUDENT); 
          printf("\n\nStudent was removed. (Press any key to continue)");
          getch();
     }
     STATE = STUDENT_MENU_STATE;
}
void handleEditStudent(){
     Student s = getStudent(students, CURRENT_STUDENT);
     int size = 7;
     char input[80];
     SELECTOR = 0;
     do{
          system("cls");
          printf("\n\n\t\t\tStudent: %s %s", s.firstname, s.lastname);
          printf("\n\n\t\t\tWhat data would you like to edit?\n\n");
          displayOptions(EDIT_STUDENT_OPTIONS, 0, size);
     }while(cycleInput(size) != ENTER_KEY);
     
     switch(SELECTOR){
          case 0: printf("Edit firstname: "); gets(input); editStudent(students, CURRENT_STUDENT, input, SELECTOR); break;
          case 1: printf("Edit lastname: "); gets(input); editStudent(students, CURRENT_STUDENT, input, SELECTOR); break;
          case 2: editStudent(students, CURRENT_STUDENT, getNumber("Edit quiz score: "), SELECTOR);break;
          case 3: editStudent(students, CURRENT_STUDENT, getNumber("Edit homework score: "), SELECTOR);break;
          case 4: editStudent(students, CURRENT_STUDENT, getNumber("Edit attendance: "), SELECTOR);break;
          case 5: editStudent(students, CURRENT_STUDENT, getNumber("Edit final exam score: "), SELECTOR);break;
          case 6: STATE = STUDENT_MENU_STATE; break;
     }
}
void handleViewAllStudents(){
     /* show the list of students and allow user to select them */
     do{  system("cls"); printStudentList(students); }while(cycleInput(getSize(students)) != ENTER_KEY);

     SELECTOR = 0;

     if(getSize(students) > 0){
          /* show options if user selects a student */
          showSelectedStudentOptions();

          /* proccess the option the user selected */
          switch(SELECTOR){
               case 0: printStudent(getStudent(students, CURRENT_STUDENT)); getch();break;
               case 1: handleEditStudent(); break;
               case 2: handleDeleteStudent(); break;
               case 3: STATE = STUDENT_MENU_STATE; break;
          }
     }
}
void handleStudentMenuInput(){
     int size  = 4;
     /* show the student options until the user press enter */
     do{ showStudentOptions(size); }while(cycleInput(size)!= ENTER_KEY);
     
     /* process the selected option of the user */
     switch(SELECTOR){
          case 0: handleViewAllStudents(); break;
          case 1: handleFindStudent(); getch(); break;
          case 2: handleAddStudent(); getch(); break;
          case 3: STATE = CLASS_MENU_STATE; break;
     }
     SELECTOR = 0;
}
int computeGrade(Student s){ return (s.quiz + s.attendance + s.hw + s.final_exam); }
/*-------------------------------- STUDENT MODULE (END) ------------------------------*/
/*------------------------------------------------------------------------------------*/
/*-------------------------------- CLASS MODULE (START) ------------------------------*/

void printClassForTable(Class c, int select_id){
     printf("\n\t %s %s", getSelectorAndClassID(select_id, c.id), c.name);
}
void handleViewAllClass(){
     do{  system("cls"); printClassList(classes); }while(cycleInput(getClassSize(classes)) != ENTER_KEY);
     printf("current class: %d", CURRENT_CLASS);getch();
}

void printClassList(ClassNode *pointer){
     int size = getClassSize(classes); 
     int i = 0;
     printf("\n\n\t\t\t\tList of class:");
     if(size == 0){
          printf("\n\t\t\t\tThe list is empty\n");
          printf("\n\t\t\t    (Press enter to continue)\n");
     }else{
          while(pointer->next!=NULL) {
               pointer = pointer -> next;
               printClassForTable(pointer->data, i++);
          }
     }
}
void handleAddClass(){
     char input[80];
     Class c;

     system("cls");
     printf("\n\n\t\t\t\tAdd Class\n\n");
     printf("Classname: ");
     gets(c.name);
     addClass(classes, c);
     
     printf("\nClass %s was added. (Press enter to continue)", c.name);
     getch();
}
void	showClassOptions(int size){
     int  start = 0,
          end = size;
	system("cls");
     printf("\n\t\t\t\tClass Record\n\n");
	printf("\t   (Use the arrow keys to navigate and press enter to select)\n\n");
     displayOptions(CLASS_MENU, start, end);
}
void handleClassMenuInput(){
     int size = 3;
     do{ showClassOptions(size); }while(cycleInput(size)!= ENTER_KEY);

     /* process the selected option of the user */
     switch(SELECTOR){
          case 0: handleViewAllClass(); break;
          case 1: handleAddClass(); break;
          case 2: STATE = EXIT;  break;
     }
     SELECTOR = 0;
}

/*-------------------------------- CLASS MODULE (END) ------------------------------*/
/*------------------------------------------------------------------------------------*/

/* State handler */
void handleState(){
     do{
          switch(STATE){
               case STUDENT_MENU_STATE: handleStudentMenuInput(); break;
               case CLASS_MENU_STATE: handleClassMenuInput(); break;
               case EXIT: exit(0); break;
          }
     }while(STATE != EXIT);
}

/* The main function */
int main(){
     students = createStudentNode();
     searchResults = createStudentNode();
     classes = createClassNode();
     //load students from file..
     //get last id based from the list of all students loaded
     //set id to last id
     //...code here..
     handleState();

     free(students);
     free(searchResults);
	return 0;
}
