#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<math.h>
#include<pthread.h>


double globalSqrtSum = 0;
pthread_mutex_t mutex;

struct threadData{
    long long int  start;
    long long int  end;
    long long int  range;
};

int isHaveAnyError(int argc,char const *argv[]);
void divideIntoAnyPart(struct  threadData *ptr[32] ,long long int  start,long long int  end,int numberOfParts);
void createPThreadStruct(struct threadData *ptr[32] ,int numberOfStructs);

void * threadFunction(void * threadPtr){

    //printf("thread fun is called\n");

    //typecasting 
    struct threadData * ptr = (struct threadData *) threadPtr;

    long long int  start = ptr->start;
    long long int  end = ptr->end;
    
    printf("start : %lld end : %lld \n",start,end);

    // calculating globalSqrtSum 
    long long int x;
    for(x = start;x<=end;x++){
        globalSqrtSum += sqrt(x);
    }

    //printf("sum of all sqrt(x) : %f \n",globalSqrtSum);

    return 0;
}
void * threadFunction2(void * threadPtr){

    printf("thread fun is called\n");
    struct threadData * ptr = (struct threadData *) threadPtr;

    long long int  start = ptr->start;
    long long int  end = ptr->end;
    long long int x;
    
    printf("start : %lld end : %lld \n",start,end);

    for(x = start;x<=end;x++){
        //printf("x; %lld \n",x);
        pthread_mutex_lock(&mutex);
        globalSqrtSum += sqrt(x);
        pthread_mutex_unlock(&mutex);
    }
   // printf("sum of all sqrt(x) : %f \n",globalSqrtSum);

    return 0;
}

int main(int argc, char const *argv[])
{
    int x , d ; 
    printf("chose a method(1,2,3) : ");
    scanf("%d",&d);
    printf("enter a numberofrange : ");
    scanf("%d",&x);

    /*if(isHaveAnyError(argc,argv) == 1 ){
        printf("error in main \n");
        return 1;
    }*/

    //printf("there is no error 1 \n");
    // convert char to number 
    /*long long int a = atoll(argv[1]); // start number 
    long long int b = atoll(argv[2]); // end number 
    int c = atoi(argv[3]);// number of threads 
    int d = atoi(argv[4]);*/
    //printf("there is no after conversion\n");
    
    pthread_t myThreads[x];
    


    // creating a pointer array to store all sturucts 
    struct  threadData *ptr[32] = {NULL};
    
    /*long long int  start = a;
    long long int  range =((b-a) / x); 
    long long int  end = b + range;*/

    long long int start = 880130203012;
    long long end = 922823372203;
    createPThreadStruct(ptr,x);
    divideIntoAnyPart(ptr,start,end,x);
    printf("start : %lld end : %lld \n",start,end);

   // printf("there is no after conversion222 \n");
    //printf("myData->start : % lld  myData->end : %lld range : %lld \n",ptr[0]->start, ptr[0]->end ,ptr[0]->range );
   // printf("before creting multiple thread\n");


    if(d == 1){
        // creating multple thread 
        for(int i = 0;i<x;i++){
            if(pthread_create(&myThreads[i],NULL,&threadFunction,ptr[i]) != 0){
                printf("error in create\n");
                return 1;
            }      
        
        } 
    }else if( d == 2){
        // creating multple thread 

        pthread_mutex_init(&mutex,NULL);
        for(int i = 0;i<x;i++){
            
            if(pthread_create(&myThreads[i],NULL,&threadFunction2,ptr[i]) != 0){
                printf("error in create\n");
                return 1;
            } 
        } 
    }

    for(int i = 0;i<x;i++){
        // error checkhing while joining 
         if(pthread_join(myThreads[i],NULL) != 0){
            printf("error in join \n");
            return 1;
        }   
    } 
    pthread_mutex_destroy(&mutex);
    printf("sum of all sqrt(x) : %f \n",globalSqrtSum);
    //printf("after creting multiple thread\n");

}

void divideIntoAnyPart(struct  threadData *ptr[32] ,long long int start,long long int end,int numberOfParts){

    // if number of parts equal 1  
    if(numberOfParts == 1){
        ptr[0]->start = start;
        ptr[0]->end = end;
        return ;
    }


    // TO DO bu kisimda araliklarin birbirlerine daha yakin olmalarini saglamaya calismalayiz
    long long int intervalSize = ((end - start) / numberOfParts );
    long long int divisionPoint = 0;
    long long int startOfInterval = start;

    for(int i = 1 ; i<numberOfParts;i++){

        divisionPoint = start + i * intervalSize;

        ptr[i-1]->start = startOfInterval;
        ptr[i-1]->end = divisionPoint; // it can be divisionpont - 1 

        //printf("divisionpoint %d : %lld \n", i, divisionPoint);
       // printf("Interval %d : ",i);
        /*for(long long int j = startOfInterval;j<=divisionPoint;j++){
            printf("%lld ",j);
            
        }*/
       // printf("\n");
       // printf("ptr[%d]->start : %lld ptr[%d]->end : %lld \n",i-1,ptr[i-1]->start,i-1,ptr[i-1]->end);
        startOfInterval = divisionPoint+1;
       // printf("\n");

    }

    ptr[numberOfParts-1]->start = startOfInterval;
    ptr[numberOfParts-1]->end = end;

    //printf("ptr[%d]->start : %lld ptr[%d]->end : %lld \n",numberOfParts-1,ptr[numberOfParts-1]->start,
    //numberOfParts-1,ptr[numberOfParts-1]->end);

   // printf("Interval %d : ",numberOfParts);
    
    /*for(int i = (divisionPoint+1);i<=end;i++){
        printf("%d ",i);

    }
    printf("\n");*/

}
void createPThreadStruct(struct threadData *ptr[32] ,int numberOfStructs){

    // creating number of struct and then assing all structs to pointer 
    for(int i = 0;i<numberOfStructs;i++){
        ptr[i] =(struct threadData*) malloc(sizeof(struct threadData));
    }

}


// checking there is an any error or not and return 1 or 0 
int isHaveAnyError(int argc,char const *argv[]){
    if (argc != 5) {
        printf("you entered missing arguments\n");
        return 1;
    } 
    // Convert command line arguments to appropriate types
    long long int a = atoll(argv[1]);
    long long int b = atoll(argv[2]);
    int c = atoi(argv[3]);// number of threads 
    int d = atoi(argv[4]);
    // checking arguments according to be numeric or not 
    if(a==0 || b==0 || c==0 || d == 0 ){
        printf("enter numeric input\n");
        return 1;
    }

    // checking commandline arguments condition
    if (a > b) {
        printf("Error: (a) must be less than or equal to (b)\n");
        return 1;
    }

    if (c  < 1) {
        printf("Error: Number of threads (c) must be at least 1\n");
        return 1;
    }
    if (d < 1 || d > 3) {
        printf("Error: Invalid method for (d) d must be 1 or 2 or 3 \n");
        return 1;
    }

    printf("\na = %lld, b = %lld, c = %d, d = %d\n", a, b, c, d);

    return 0; // there is no error
}
