//
//  main.cpp
//  Multiprocessing
//
//  Created by Dongha Kang on 2020/04/19.
//  Copyright © 2020 Dongha Kang. All rights reserved.
//

#include <iostream>
#include <chrono>
#include <sys/wait.h> //linux need it
#include <fcntl.h>
#include <unistd.h>
using namespace std;

int main(int argc, const char * argv[]) {
    
//    time_t starttime, endtime;//debug
//    starttime = time(NULL);//debug
//
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
    
//    cout<< "starttime="<<starttime<<"start="<<start<<endl;//debug
//
    char * pos = NULL;
    int total_process_num = (int)strtol(argv[1],&pos,10);
    int N; //The Number of the filters
    int X, Y; //X=The number of rows of the input, Y= The number of cols of the input
    int x, y; //x=The number of rows of the filters, y= The number of cols of the filters
    cin >> N >> x >> y;
    double processtime[N];//time consumed by each process
    int elementsOfFilters[N][3][x][y];//[#of the filter][0(R),1(G),2(B) channel][row][col]
    for (int i=0;i<N;i++){
        for (int j=0;j<3;j++){
            for (int k=0;k<x;k++){
                for (int l=0;l<y;l++){
                    cin >> elementsOfFilters[i][j][k][l];
                
                }
            }
        }
    }
    
    cin >> X >> Y;
    int input[3][X][Y];
    
    for (int i=0;i<3;i++){//initialize
        for (int j=0;j<X;j++){
            for(int k=0;k<Y;k++){
                input[i][j][k]=0;
            }
        }
    }

    for (int i=0;i<3;i++){
        for (int j=0;j<X;j++){
            for(int k=0;k<Y;k++){
                cin >> input[i][j][k];
            }
        }
    }
    
//    for (int i=0;i<N;i++){
//          for (int j=0;j<3;j++){
//              for (int k=0;k<x;k++){
//                  for (int l=0;l<y;l++){
//                      cout << elementsOfFilters[i][j][k][l]<<" ";
//
//                  }
//                  cout<< endl;
//              }
//              cout<<endl;
//          }
//      }
//
//
//    for (int i=0;i<3;i++){
//          for (int j=0;j<X;j++){
//              for(int k=0;k<Y;k++){
//                  cout <<input[i][j][k]<<" ";
//              }
//              cout<< endl;
//          }
//      }



    int sudoprocess_num=0;

    if(total_process_num>N){
        sudoprocess_num=total_process_num-N;
        total_process_num=N;

    }

    int quotient = N/total_process_num;
    int remainder = N%total_process_num;
    int temp = remainder;



    //dividing filters for and make sub test files
    int stdoutcopy=dup(1);//make a duplication for stdout to roll back later
    int saved_filters_num=0;
    for(int m=0;m<total_process_num;m++){
        char buf[256];
        sprintf(buf,"test%d.txt",m+1);
        int in = open(buf, O_TRUNC | O_RDWR | O_CREAT,S_IRWXU);

        dup2(in,1);
        if(temp-->0){
            cout<<quotient+1<<" "<<x<<" "<<y<<endl;
            for(int i=saved_filters_num;i<saved_filters_num+quotient+1;i++){
                for(int j=0;j<3;j++){
                    for(int k=0;k<x;k++){
                        for (int l=0;l<y;l++){
                            cout << elementsOfFilters[i][j][k][l]<<" ";
                        }
                        cout<<endl;
                    }
                }
            }
            saved_filters_num+=quotient+1;

        }else{
            cout<<quotient<<" "<<x<<" "<<y<<endl;
            for(int i=saved_filters_num;i<saved_filters_num+quotient;i++){
                for(int j=0;j<3;j++){
                    for(int k=0;k<x;k++){
                        for (int l=0;l<y;l++){
                            cout << elementsOfFilters[i][j][k][l]<<" ";
                        }
                        cout<<endl;
                    }
                }
            }
            saved_filters_num+=quotient;

        }
        //duplicated
        cout<<X<<" "<<Y<<endl;
        for(int i=0;i<3;i++){
            for(int j=0;j<X;j++){
                for(int k=0;k<Y;k++){
                    cout<<input[i][j][k]<<" ";
                }
                cout<<endl;
            }
        }
        close(in);
    }



    for(int i=0;i<total_process_num;i++){//covolution each process and save the result as "result%d.txt",i+1
        if(fork()==0){
            char buf[256];
            char buff[256];
            char * args[] = { (char*)("./program1") , NULL };//covert the string "./program1" to char*
            sprintf(buf,"test%d.txt",i+1);
            sprintf(buff,"result%d.txt",i+1);
            int in = open(buf,O_RDONLY,S_IRWXU);
            int out = open(buff, O_TRUNC | O_RDWR | O_CREAT,S_IRWXU);
            dup2(in,0);
            dup2(out,1);//after dup2, execlp's stdout will redirect to the out file
            execvp("./program1", args);// exec inherit the properties such as dup2 setting and pid and ppid to new exected process
            close(in);
            close(out);
            exit(0);
        }
    }



    int result[N][X-x+3][Y-y+3];
//    for(int i=0;i<N;i++){
//        for(int j=0;j<X-x+3;j++){
//            for(int k=0;k<Y-y+3;k++){
//                result[i][j][k]=0;
//            }
//        }
//    }

    for(int i=0;i<total_process_num;i++){// wait for all of the children
        wait(NULL);//wait(null) wait for one of the children, success on macOS but fail on linux #include <sys/wait.h> //linux need it
    }

    //reading result#
    temp=remainder;
    saved_filters_num=0;
    for(int m=0;m<total_process_num;m++){
        char buf[256];
        sprintf(buf,"result%d.txt",m+1);
        int in = open(buf,O_RDONLY,S_IRWXU);
        dup2(in,0);
        if(temp-->0){
            for(int i=saved_filters_num;i<saved_filters_num+quotient+1;i++){
                for(int j=0;j<X-x+3;j++){
                    for (int k=0;k<Y-y+3;k++){
                        cin >> result[i][j][k];
                    }
                }
            }

            saved_filters_num+=quotient+1;
        }else{
            for(int i=saved_filters_num;i<saved_filters_num+quotient;i++){
                for(int j=0;j<X-x+3;j++){
                    for (int k=0;k<Y-y+3;k++){
                        cin >> result[i][j][k];
                    }
                }
            }
            saved_filters_num+=quotient;
        }
        cin >> processtime[m];
    }

    dup2(stdoutcopy,1);// dup2(out,1)했던걸 다시 standardoutput으로 바꿔준다.
    for(int i=0;i<N;i++){
        for(int j=0;j<X-x+3;j++){
            for(int k=0;k<Y-y+3;k++){
                cout<< result[i][j][k]<<" ";
            }
            cout<<endl;
        }
        cout<<endl;
    }

    if(sudoprocess_num==0)
    {
        for(int i=0;i<total_process_num;i++){
            if(i==total_process_num-1){
                cout<<processtime[i];
            }else{
                cout<<processtime[i]<<" ";
            }
        }
    }else{
        for(int i=0;i<total_process_num;i++){
            cout<<processtime[i]<<" ";
        }
        for(int i=0;i<sudoprocess_num;i++){
            if(i==sudoprocess_num-1){
                cout<<0;
            }else{
                cout<<0<<" ";
            }
        }
    }

    //delete tmp files
    for(int i=0;i<total_process_num;i++){
            char buf[256];
            char buff[256];
            sprintf(buf,"test%d.txt",i+1);
            sprintf(buff,"result%d.txt",i+1);
            remove(buf);
            remove(buff);
    }




    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout<<endl<< dif.count()<<" "<<endl;

    return 0;
}

