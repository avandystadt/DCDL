#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
#define MAX_NB 6
#define BUF_LEN 50

unsigned _stklen = 60000U ;


typedef struct {
    int num[MAX_NB];
    char text[MAX_NB][50];
    int goal;
    int nbOperation;
    char solutionTable[50][50];
    int nbOperationTable[50];
    int nbSolutions;
    int bestDiff;
} gameData;

/*typedef struct {
    int num1;
    int num2;
    int operand;
} operation;*/

int gameLoop(gameData * game)
{
    int i,j,k,kTemp;
    int numRemaining = MAX_NB-game->nbOperation;
    for (i=0;i<numRemaining;++i){
        if(game->num[i]==game->goal){
            printf("%s\n",game->text[0]);
            return 1;
        }
        for(j=i+1;j<numRemaining;++j){
            // try with all operations
            // recursion with a new gameData struct
            // new number will be put in first position as it's likely to be the one we're looking for, therefore the one we should compare first
            gameData gameNew;
            kTemp=1;
            for(k=1;k<numRemaining;++k){
                if(k!=i&&k!=j){
                    gameNew.num[kTemp]=game->num[k];
                    strcpy(gameNew.text[kTemp],game->text[k]);
                    ++kTemp;
                }
            }
            gameNew.goal=game->goal;
            gameNew.nbOperation=game->nbOperation+1;

            // addition
            gameNew.num[0]=game->num[i]+game->num[j];
            sprintf(gameNew.text[0],"(%s + %s)",game->text[i],game->text[j]);

            if(gameLoop(&gameNew)){
                return 1;
            }

            // multiplication
            if(game->num[i]!=1 && game->num[j]!=1){
                gameNew.num[0]=game->num[i]*game->num[j];
                sprintf(gameNew.text[0],"%s * %s",game->text[i],game->text[j]);

                if(gameLoop(&gameNew)){
                    return 1;
                }
            }

            // at this point, we are doing non-commutative operation and want the first element to be the bigger one
            int high,low;
            char highText[50],lowText[50];
            if(game->num[i]<game->num[j]){
                high=game->num[j];
                low=game->num[i];
                strcpy(highText,game->text[j]);
                strcpy(lowText,game->text[i]);

            }
            else{
                high=game->num[i];
                low=game->num[j];
                strcpy(highText,game->text[i]);
                strcpy(lowText,game->text[j]);
            }

            // substraction
            gameNew.num[0]=high-low;
            if(gameNew.num[0]!=0){
                sprintf(gameNew.text[0],"(%s - %s)",highText,lowText);
                if(gameLoop(&gameNew))
                    return 1;
            }
            // division
            if(!((high%low!=0) || (low*low == high) || low==1)){
                gameNew.num[0]=high/low;
                sprintf(gameNew.text[0],"(%s / %s)",highText,lowText);
                if(gameLoop(&gameNew)){
                    return 1;
                }
            }


        }
    } //_____first for loop
    return 0;
}

gameData dataPrep(){
    gameData game;
    memset(&game,0,sizeof(gameData));
    int r=0;
    for(int i=0;i<MAX_NB;++i){
        while(r==0){
            r = 1 + (rand() % 24);
            for(int j=0;j<i;++j){ // avoid duplicates
                if(r==game.num[j])
                    r=0;
            }
            game.num[i]=r;
        }
        r=0;
    }
    for(int i=0;i<MAX_NB;++i){
        if(game.num[i]>10 && game.num[i]<=20)
            game.num[i]-=10;
        if(game.num[i]==21)
            game.num[i]=25;
        if(game.num[i]==22)
            game.num[i]=50;
        if(game.num[i]==23)
            game.num[i]=75;
        if(game.num[i]==24)
            game.num[i]=100;
        sprintf(game.text[i],"%d",game.num[i]);
        //printf("%s\t",game.text[i]);
    }

    game.goal=100+(rand()%900);
    //printf("\nGoal: %d\n\n",game.goal);
    return game;

}

int parseCommand(int * num, char * playerInput){
    int num1=0, num2=0, operand=0, currentNum=1,result=0;
    int i,j;
    char cur;
//    printf("You typed: %s\n",playerInput);
    for(int i=0;i<BUF_LEN;++i){
        cur=playerInput[i]; //make it easier to read but we could just have worked on playerInput directly
        if(cur>='0' && cur<='9'){
            if(currentNum==1){
                num1*=10;
                num1+=cur-48;
            }
            else if(currentNum==2){
                num2*=10;
                num2+=cur-48;
            }
        }
        else if(cur=='+' && currentNum==1){
            operand=1;
            ++currentNum;
        }
        else if(cur=='-' && currentNum==1){
            operand=2;
            ++currentNum;
        }
        else if(cur=='*' && currentNum==1){
            operand=3;
            ++currentNum;
        }
        else if(cur=='/' && currentNum==1){
            operand=4;
            ++currentNum;
        }
        else if(cur==' ') // ignore whitespace
            continue;
        else
            break;
    }
    // printf("Your two numbers: %d %d operation %d\n",num1,num2,operand);
    // now check if they actually exist
    if(operand<1 || operand>4)
        return 0;
    for(i=0;i<MAX_NB;++i){
        if(num1==num[i])
            break;
    }

    for(j=0;j<MAX_NB;++j){
        if(num2==num[j])
            break;
    }

    if(i==MAX_NB || j==MAX_NB)
        return 0;


    if(operand==2 && num1<num2){
        printf("Negative numbers aren't allowed\n");
        return 0;
    }
    if(operand==4 && (num1%num2)){
        printf("Divisions have to result in integers\n");
        return 0;
    }
    switch(operand){
        case 1:
            printf("%d + %d = %d\n",num1,num2,result=num1+num2);
            break;
        case 2:
            printf("%d - %d = %d\n",num1,num2,result=num1-num2);
            break;
        case 3:
            printf("%d * %d = %d\n",num1,num2,result=num1*num2);
            break;
        case 4:
            printf("%d / %d = %d\n",num1,num2,result=num1/num2);
            break;
    }

    num[i]=result; // place new result in the array in place of the first number used
    for(int k=j;k<MAX_NB;++k)
        num[k]=num[k+1]; // move everything to the left starting from second number

    return 1;
}

int solutionReached(int * num, int goal){
    for(int i=0;i<MAX_NB;++i){
        if(num[i]==goal)
            return i+1;
    }
    return 0; // no solution found
}

void testData(gameData * game){
    game->num[0] = 3;
    game->num[1] = 75;
    game->num[2] = 2;
    game->num[3] = 4;
    game->num[4] = 1;
    game->num[5] = 1;
    game->num[6] = 0;
    game->goal = 888;
    game->nbOperation=0;

}

int playerLoop(gameData * game){
    char playerInput[BUF_LEN];
    int num[MAX_NB][MAX_NB]; // for backups
    int nbOperation=0;

    for(int i=0;i<MAX_NB;++i){
        num[nbOperation][i]=game->num[i];
    }
    while(nbOperation<5 && (!solutionReached(num[nbOperation],game->goal))){

        for(int i=0;i<MAX_NB-nbOperation;++i)
            printf("%d\t",num[nbOperation][i]);
        printf("\nGoal: %d\n",game->goal);

        printf("\nPlease type your operation (one at a time), or 'h' for a list of commands:\n");
        fgets(playerInput, 100, stdin);
        if(playerInput[0]=='c'){
            printf("Resetting...\n\n");
            nbOperation=0;
            continue;
        }
        else if(playerInput[0]=='u'){
            printf("Undoing last operation\n\n");
            if(nbOperation){ //security in case user does it the first time
                --nbOperation;
            }
            continue;
        }
        else if(playerInput[0]=='s'){
            printf("Stopping the game\n\n");
            nbOperation==MAX_NB;
            break;
        }
        else if(playerInput[0]=='h'){
            printf("u: undo the previous operation\n"
               "c: clear all operations\n"
               "s: stop, or post your closest approximation\n");
            continue;
        }

        ++nbOperation;
        for(int i=0;i<MAX_NB;++i){
            num[nbOperation][i]=num[nbOperation-1][i];
        }

        if(!parseCommand(num[nbOperation],playerInput)){
            printf("T'as encore fait de la merde\n");
            --nbOperation;
        }
    }

    if(solutionReached(num[nbOperation],game->goal))
        printf("Congratulations!\n");
/*    else if(solutionClose((num[nbOperation],game->closest)))
        printf("You found the closest possible value\n");*/
    else
        printf("You lost\n");
}

int main()
{
    srand(time(NULL));
    gameData game;
    game = dataPrep();
    char gameMode=0;
    char buf[50];
    char c;

//    testData(&game); //temp
//    printf("%d %d %d %d %d %d %d",game.num[0],game.num[1],game.num[2],game.num[3],game.num[4],game.num[5],game.goal);

    printf("Choose your game mode:\n"
           "1-Enter the solution in the console\n"
           "2-Calculate mentally and check a solution\n"
           "Type 1 or 2: ");


    while(gameMode!=1 && gameMode !=2){
        gameMode=getchar()-48;
        while ((c = getchar()) != '\n' && c != EOF) { } //empty stdin buffer
        if(gameMode!=1 && gameMode !=2){
            printf("Error, please type 1 or 2.\n\n");
            continue;
        }
    }
    if(gameMode==2){
        for(int i=0;i<MAX_NB;++i)
            printf("%s\t",game.text[i]);
        printf("\nGoal: %d\n\n",game.goal);

        printf("Please type Enter when you want to check if there was a solution");
        getchar();
        printf("Solution: ");
        gameLoop(&game);
/*        if(gameLoop(&game))
            printf("Solution has been found:\n");
        else
            printf("No solution, closest approximation:\n");*/
    }
    else if(gameMode==1){
        playerLoop(&game);
    }
}

// todo: best approximation
/*                 for(l=0;l<numRemaining ;++l){
                    printf("%d\t",game->num[l]);
                }
                    printf("\n");*/
// bug: seems to reuse some numbers (seems to happen for or ones at the end?)

