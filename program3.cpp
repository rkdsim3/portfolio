//
//  program3.cpp
//  OS_assignment2_multi_process_thread
//
//  Created by Dongha Kang on 2020/04/22.
//  Copyright © 2020 Dongha Kang. All rights reserved.
//
#include <iostream>
//#include <pthread.h>
#include <stdio.h>
#include <chrono>

using namespace std;

typedef struct Shared_data
{
    int * filter; //[N][3][x][y]
    int * input; // [3][X+2][Y+2] padded
    int * result; //[N][X-x+3][Y-y+3]
    int num_threads;
    int N, X, Y, x, y;
} Shared_data;

Shared_data given_file;

void *conv_relu(void *arg) //이 프로젝트의 경우 thread function의 argument나 return 타입이 굳이 포인터일 필요가없다고 느껴질수 있다 왜냐면 수정하지않기 때문이다. 그러나 수정하는 경우도 가능하게하기위해서 보이드 포인터를 이용한다.
{
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//    clock_t start= clock();
    
    //convolution
    int quotient = given_file.N/given_file.num_threads;
    int remainder = given_file.N%given_file.num_threads;
    int temp = (long)(arg)+1;
    if(temp<=remainder){
        for(int i=(quotient+1)*(temp-1);i<(quotient+1)*(temp);i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<given_file.X-given_file.x+3;k++){
                    for(int l=0;l<given_file.Y-given_file.y+3;l++){
                        for (int m=0;m<given_file.x;m++){
                            for (int n=0;n<given_file.y;n++){
                                
                                given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+k*(given_file.Y-given_file.y+3)+l]+=given_file.input[j*(given_file.X+2)*(given_file.Y+2)+(k+m)*(given_file.Y+2)+(l+n)]*given_file.filter[(i*3*given_file.x*given_file.y)+j*(given_file.x*given_file.y)+m*given_file.y+n];
                                                            }
                        }
                    }
                }
            }
        }
    }else{
        for(int i= (quotient+1)*remainder+(temp-remainder-1)*quotient;i<remainder*(quotient+1)+(temp-remainder)*quotient;i++){
            for(int j=0;j<3;j++){
                for(int k=0;k<given_file.X-given_file.x+3;k++){
                    for(int l=0;l<given_file.Y-given_file.y+3;l++){
                        for (int m=0;m<given_file.x;m++){
                            for (int n=0;n<given_file.y;n++){
                                given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+k*(given_file.Y-given_file.y+3)+l]+=given_file.input[j*(given_file.X+2)*(given_file.Y+2)+(k+m)*(given_file.Y+2)+(l+n)]*given_file.filter[(i*3*given_file.x*given_file.y)+j*(given_file.x*given_file.y)+m*given_file.y+n];
                                
                            }
                        }
                    }
                }
            }
        }
    }
    
    
    
    //RELU
    if(temp<=remainder){
        for(int i=(quotient+1)*(temp-1);i<(quotient+1)*(temp);i++){
            for(int j=0;j<given_file.X-given_file.x+3;j++){
                for(int k=0;k<given_file.Y-given_file.y+3;k++){
                    given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k]=max(given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k],0);
                }
            }
        }
    }else{
        for(int i= (quotient+1)*remainder+(temp-remainder-1)*quotient;i<remainder*(quotient+1)+(temp-remainder)*quotient;i++){
            for(int j=0;j<given_file.X-given_file.x+3;j++){
                for(int k=0;k<given_file.Y-given_file.y+3;k++){
                    given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k]=max(given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k],0);
                }
            }
        }
    }
    //%%%%%%%%% below code is wrong, because there are overlapp between thread
//    for(int i=0;i<given_file.N;i++){
//        for(int j=0;j<given_file.X-given_file.x+3;j++){
//            for(int k=0;k<given_file.Y-given_file.y+3;k++){
//                given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k]=max(given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k],0);
//            }
//        }
//    }
    
    
    
    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    pthread_exit((void*)(long)dif.count());
//        clock_t end = clock();
//    double time_taken = (double)(end - start)/CLOCKS_PER_SEC;
//    long time_takenn=(int)(time_taken*double(1000)+0.5);//
//
//    pthread_exit((void*) time_takenn);
//
}

int main(int argc, const char * argv[]){
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    
//    clock_t start = clock();
//
    char * pos = NULL;
    given_file.num_threads = (int)strtol(argv[1],&pos,10);
    cin >> given_file.N >> given_file.x >> given_file.y;
    
    int sudothread=0;
    if(given_file.num_threads>given_file.N){
        sudothread=given_file.num_threads-given_file.N;
        given_file.num_threads=given_file.N;
    }
    pthread_t threads[given_file.num_threads];
   
    
    given_file.filter = (int*) malloc (given_file.N*given_file.x*given_file.y*3*sizeof(int));

    //reading filters
    for(int i=0;i<given_file.N;i++){
        for(int j=0;j<3;j++){
            for(int k=0;k<given_file.x;k++){
                for(int l=0;l<given_file.y;l++){
                    cin>>given_file.filter[l+k*given_file.y+j*given_file.x*given_file.y+i*given_file.x*given_file.y*3];
                }
            }
        }
    }
    
  
                                       
    cin >> given_file.X >> given_file.Y;
    given_file.input =(int*) malloc ((given_file.X+2)*(given_file.Y+2)*3*sizeof(int));
    given_file.result= (int*) malloc((given_file.N)*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)*sizeof(int));


    //initialize(inlcude padding)
    for(int i=0;i<(given_file.X+2)*(given_file.Y+2)*3;i++){
        given_file.input[i]=0;
    }
    //reading input
    for(int i=0;i<3;i++){
        for(int j=1;j<given_file.X+1;j++){
            for(int k=1;k<given_file.Y+1;k++){
                cin>>given_file.input[i*(given_file.X+2)*(given_file.Y+2)+j*(given_file.Y+2)+k];
            }
        }
    }
    

  

    //initialize result
    for(int i=0;i<given_file.N;i++){
        for(int j=0;j<given_file.X-given_file.x+3;j++){
            for(int k=0;k<given_file.Y-given_file.y+3;k++){
                given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k]=0;
            }
        }
    }

//    clock_t mid= clock(); // debug
////
    int rc;
    void *thread_return;
    for (long i=0;i<given_file.num_threads;i++){//make threads
        rc=pthread_create(&threads[i],NULL,conv_relu,(void *) i);
        if(rc){
            cout<<"ERROR; return code from pthread_create() is" <<rc<<"\n"<<endl;
            exit(-1);
        }
    }

    long times_threads[given_file.num_threads];

    for(long i=0; i<given_file.num_threads; i++) {//joining all threads
        rc = pthread_join(threads[i], &thread_return);
        if (rc) {
            cout<<"ERROR; return code from pthread_join() is "<<rc<<endl;
            exit(-1);
        }
        times_threads[i]=(long)(thread_return);
    }
//
//    clock_t mid2 = clock();//debug

    //print result
    for(int i=0;i<given_file.N;i++){
            for(int j=0;j<given_file.X-given_file.x+3;j++){
                for(int k=0;k<given_file.Y-given_file.y+3;k++){
                    if(k!=given_file.Y-given_file.y+2){
                    cout<< given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k]<<" ";
                    }else{
                        cout<< given_file.result[i*(given_file.X-given_file.x+3)*(given_file.Y-given_file.y+3)+j*(given_file.Y-given_file.y+3)+k];
                    }
                }
                cout<<endl;
            }
            cout<<endl;
        }

    if(sudothread!=0){

        for(int i=0;i<given_file.num_threads;i++){

            cout<<times_threads[i]<<" ";
        }
        for(int i=0;i<sudothread;i++){
            if(i!=sudothread-1){
                cout<<0<<" ";
            }else{
                cout<<0;
            }
        }
    }else{

        for(int i=0;i<given_file.num_threads;i++){
            if(i!=given_file.num_threads-1){
                cout<<times_threads[i]<<" ";
            }else{
                cout<<times_threads[i];
            }
        }
    }



//    clock_t end = clock();
//      double time_taken = (double)(end - start)/CLOCKS_PER_SEC;
////
//      cout<< endl<< (int)(time_taken*double(1000)+0.5)<<endl;
//    cout<<endl<<(int)(((double)(mid-start))/(CLOCKS_PER_SEC)*double(1000)+0.5)<<endl;
//    cout<<endl<<(int)(((double)(mid2-start))/(CLOCKS_PER_SEC)*double(1000)+0.5)<<endl;


    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout<< endl<<dif.count()<<" "<<endl;
    
    }
