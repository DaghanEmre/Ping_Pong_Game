#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct game_Info
{
    int position_A;
    int position_B;
    int ball_xPosition;
    int ball_yPosition;
    int ball_xSpeed;
    int ball_ySpeed;
    int mode;
    int score_A;
    int score_B;
    struct game_Info *next;
}game_Info;


void checking_FileExist(FILE* fileName);
void parse_Line(FILE *input,FILE *output,game_Info **game_Info_Head,game_Info **stack_Head);
void goback(game_Info **game_Info_Head,game_Info **stack_Head);
void move_Mode(game_Info **game_Info_Head, FILE *output,int PlayerA_Move,int PlayerB_Move,game_Info **stack_Head);
void stack_initilize(struct game_Info* head);
void stack_push(struct game_Info* head, int position_A, int position_B,int ball_xPosition,int ball_yPosition,int ball_xSpeed,int ball_ySpeed,int mode,int score_A,int score_B);
void stack_pop(struct game_Info* head, int position_A, int position_B,int ball_xPosition,int ball_yPosition,int ball_xSpeed,int ball_ySpeed,int mode,int score_A,int score_B);
int isEmpty(struct game_Info* head);

/*Main Function*/
int main()
{
    FILE *input;
    FILE *output;

    output = fopen("Result.txt","w");
    checking_FileExist(output);

    input = fopen("Input.txt","r");
    checking_FileExist(input);

    game_Info *game_Info_Head = NULL; /*Head of game_Info linked list*/
    game_Info* stack_Head = NULL;    /*Head of stack*/

    parse_Line(input,output,&game_Info_Head,&stack_Head);

    return 0;
}

void parse_Line(FILE *input,FILE *output,game_Info **game_Info_Head,game_Info **stack_Head)
{
    *game_Info_Head=(game_Info*)malloc(sizeof(game_Info));
    char* command;
    char readLine[100];
    while(fgets(readLine,100,input)!=NULL)
    {
        command = strtok(readLine," ");

        /*that function runs the set operation*/
        if (strcmp(command,"set") == 0)
        {
            /*Parsing Set Commend*/

            (*game_Info_Head)->position_A = atoi(strtok(NULL,","));

            (*game_Info_Head)->position_B = atoi(strtok(NULL,","));

            (*game_Info_Head)->ball_xPosition = atoi(strtok(NULL,","));

            (*game_Info_Head)->ball_yPosition = atoi(strtok(NULL,","));

            (*game_Info_Head)->ball_xSpeed = atoi(strtok(NULL,","));

            (*game_Info_Head)->ball_ySpeed = atoi(strtok(NULL,","));

            (*game_Info_Head)->mode = atoi(strtok(NULL,","));

            (*game_Info_Head)->score_A = 0;
            (*game_Info_Head)->score_B = 0;

            printf("position_A: %d\n",(*game_Info_Head)->position_A);
            printf("position_B: %d\n",(*game_Info_Head)->position_B);
            printf("Ball x position: %d\n",(*game_Info_Head)->ball_xPosition);
            printf("Ball y position: %d\n",(*game_Info_Head)->ball_yPosition);
            printf("Ball x speed: %d\n",(*game_Info_Head)->ball_xSpeed);
            printf("Ball y speed: %d\n",(*game_Info_Head)->ball_ySpeed);
            printf("Mode: %d\n",(*game_Info_Head)->mode);
            printf("Score A: %d\n",(*game_Info_Head)->score_A);
            printf("Score B: %d\n",(*game_Info_Head)->score_B);

        }

        /*that function runs the move operation*/
        else if(strcmp(command,"move") == 0)
        {
            int PlayerA_Move, PlayerB_Move;

            PlayerA_Move = atoi(strtok(NULL,","));

            PlayerB_Move = atoi(strtok(NULL,","));

            move_Mode(game_Info_Head,output,PlayerA_Move,PlayerB_Move,stack_Head);

        }

        /*that function runs the changemode operation*/
        else if(strcmp(command,"changemode") == 0)
        {
            (*game_Info_Head)->mode = atoi(strtok(NULL,","));
        }

        /*that function runs the changespeed operation*/
        else if(strcmp(command,"changespeed") == 0)
        {
            (*game_Info_Head)->ball_xSpeed = atoi(strtok(NULL,","));
            (*game_Info_Head)->ball_ySpeed = atoi(strtok(NULL,","));
        }

        /*that function runs the goback operation*/
        else if(strcmp(command,"goback") == 0)
        {
            int i;
            int score = atoi(strtok(NULL,","));
            for(i = 0;i < score -1;i++){
                stack_pop(stack_Head,(*game_Info_Head)->position_A,(*game_Info_Head)->position_B,(*game_Info_Head)->ball_xPosition,(*game_Info_Head)->ball_yPosition,
                        (*game_Info_Head)->ball_xSpeed,(*game_Info_Head)->ball_ySpeed,(*game_Info_Head)->mode,(*game_Info_Head)->score_A,(*game_Info_Head)->score_B);
            }
        }
        else
            printf("invalid command!\n");
    }
    fprintf(output,"%d,%d,%d,%d,%d,%d",(*game_Info_Head)->position_A,(*game_Info_Head)->position_B,(*game_Info_Head)->ball_xPosition,(*game_Info_Head)->ball_yPosition,(*game_Info_Head)->score_A,(*game_Info_Head)->score_B);
    fclose(input);
    fclose(output);
}


void move_Mode(game_Info **game_Info_Head, FILE *output,int PlayerA_Move,int PlayerB_Move,game_Info **stack_Head)
{
    /*That part of function makes move the player bars*/
    int summationA = (*game_Info_Head)->position_A + PlayerA_Move;
    (*game_Info_Head)->position_A = summationA;

    int summationB = (*game_Info_Head)->position_B + PlayerB_Move;
    (*game_Info_Head)->position_B = summationB;

    /*That part makes positive integer the ball speed*/
    int destination;
    destination = abs((*game_Info_Head)->ball_xSpeed);

    int i;
    int score_Flag = 0;
    for(i = 0; i < destination; i++){
        /*Operation for ball's movement*/
        if((*game_Info_Head)->ball_xSpeed > 0){
            if((*game_Info_Head)->ball_xPosition == 79){
                (*game_Info_Head)->ball_xSpeed *= -1;
                (*game_Info_Head)->ball_xPosition--;
            } else{
                (*game_Info_Head)->ball_xPosition++;
            }
        }
        else if ((*game_Info_Head)->ball_xSpeed < 0){
            if((*game_Info_Head)->ball_xPosition == 0){
                (*game_Info_Head)->ball_xSpeed *= -1;
                (*game_Info_Head)->ball_xPosition++;
            } else{
                (*game_Info_Head)->ball_xPosition--;
            }
        } else {
        }
        if((*game_Info_Head)->ball_ySpeed > 0){
            if((*game_Info_Head)->ball_yPosition == 24){
                if((*game_Info_Head)->mode == 1)
                {
                    (*game_Info_Head)->ball_ySpeed *= -1;
                    (*game_Info_Head)->ball_yPosition--;
                }
                else if((*game_Info_Head)->mode == 2)
                {
                    (*game_Info_Head)->ball_yPosition = 0;
                }else{
                    printf("Wrong mode! Fatal error!");
                    exit(-1);
                }
            } else{
                (*game_Info_Head)->ball_yPosition++;
            }
        } else if ((*game_Info_Head)->ball_ySpeed < 0){
            if((*game_Info_Head)->ball_yPosition == 0){
                if((*game_Info_Head)->mode == 1){
                    (*game_Info_Head)->ball_ySpeed *= -1;
                    (*game_Info_Head)->ball_yPosition++;
                }
                else if((*game_Info_Head)->mode == 2)
                {
                    (*game_Info_Head)->ball_yPosition = 24;
                }else{
                    printf("Wrong mode! Fatal error!");
                    exit(-1);
                }
            } else{
                (*game_Info_Head)->ball_yPosition--;
            }
        } else {
        }

        /*operation for x axis*/
        if((*game_Info_Head)->ball_xPosition == 0)
        {
            if(((*game_Info_Head)->position_A > (*game_Info_Head)->ball_yPosition) || ((*game_Info_Head)->position_A < (*game_Info_Head)->ball_yPosition))
            {
                (*game_Info_Head)->score_B++;
                score_Flag = 1;
            }
        }
        if((*game_Info_Head)->ball_xPosition == 79)
        {
            if(((*game_Info_Head)->position_B > (*game_Info_Head)->ball_yPosition) || ((*game_Info_Head)->position_B < (*game_Info_Head)->ball_yPosition))
            {
                (*game_Info_Head)->score_A++;
                score_Flag = 1;
            }
        }
    }
    if(score_Flag == 1)
    {
        stack_push(stack_Head,(*game_Info_Head)->position_A,(*game_Info_Head)->score_B,(*game_Info_Head)->ball_xPosition,(*game_Info_Head)->ball_yPosition,
                   (*game_Info_Head)->ball_xSpeed,(*game_Info_Head)->ball_ySpeed,(*game_Info_Head)->mode,(*game_Info_Head)->score_A,(*game_Info_Head)->score_B);
    }

    printf("ball_posX: %d ball_posY: %d scoreA: %d scoreB: %d\n",(*game_Info_Head)->ball_xPosition,(*game_Info_Head)->ball_yPosition,
                                (*game_Info_Head)->score_A,(*game_Info_Head)->score_B);
    printf("position A : %d\n", (*game_Info_Head)->position_A);
    printf("position B : %d\n", (*game_Info_Head)->position_B);
}

/*REFERENCE: http://www.zentut.com/c-tutorial/c-stack-using-pointers/   */
void stack_push(struct game_Info* head, int position_A, int position_B,
            int ball_xPosition,int ball_yPosition,int ball_xSpeed,
            int ball_ySpeed,int mode,int score_A,int score_B)
{
    struct game_Info* temp = (struct game_Info*)malloc(sizeof(struct game_Info));
    if(temp == NULL)
    {
        exit(0);
    }
    temp->position_A = position_A;
    temp->position_B = position_B;
    temp->ball_xPosition = ball_xPosition;
    temp->ball_yPosition = ball_yPosition;
    temp->ball_xSpeed = ball_xSpeed;
    temp->ball_ySpeed = ball_ySpeed;
    temp->mode = mode;
    temp->score_A = score_A;
    temp->score_B = score_B;
    temp->next = head;
    head = temp;

}

/*REFERENCE: http://www.zentut.com/c-tutorial/c-stack-using-pointers/   */
void stack_pop(struct game_Info* head, int position_A, int position_B,int ball_xPosition,int ball_yPosition,int ball_xSpeed,int ball_ySpeed,int mode,int score_A,int score_B)
{
    struct game_Info* temp;
    temp=(game_Info*)malloc(sizeof(game_Info));
    temp = head;
    position_A = head->position_A;
    position_B = head->position_B;
    ball_xPosition = head->ball_xPosition;
    ball_yPosition = head->ball_yPosition;
    ball_xSpeed = head->ball_xSpeed;
    ball_ySpeed = head->ball_ySpeed;
    mode = head->mode;
    score_A = head->score_A;
    score_B = head->score_B;
    head = head->next;
    free(temp);
}

/*That function returns 1 when the stack is empty, otherwise it returns 0*/
int isEmpty(struct game_Info* head){
    return head == NULL ? 1 : 0;
}

void checking_FileExist(FILE* fileName){
    if (fileName == NULL)
        {
            printf("Error! Could not open file\n");
            exit(-1);
        }

}



