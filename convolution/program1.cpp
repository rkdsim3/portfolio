//
//  main.cpp
//  MultiprocessingAndMultithreading
//
//  Created by Dongha Kang on 2020/04/15.
//  Copyright © 2020 Dongha Kang. All rights reserved.
//

#include <iostream>
#include <chrono>

using namespace std;

int main(int argc, const char * argv[]) {
    std::chrono::high_resolution_clock::time_point start = std::chrono::high_resolution_clock::now();
//     clock_t start = clock();
    
    int N; //The Number of the filters
    int X, Y; //X=The number of rows of the input, Y= The number of cols of the input
    int x, y; //x=The number of rows of the filters, y= The number of cols of the filters
    cin >> N >> x >> y;
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
    int input[3][X+2][Y+2]; //+2 for padding
    for(int k=0;k<3;k++){
        for (int i=0;i<X+2;i++){//initialize elements as 0
            for(int j=0;j<Y+2;j++){
                input[k][i][j]=0;
            }
        }
    }
    for (int k=0;k<3;k++){
        for (int i=1;i<X+1;i++){
            for(int j=1;j<Y+1;j++){
                cin >> input[k][i][j];
            }
        }
    }
    

    int result[N][X-x+3][Y-y+3];
    //initialize
    for (int i=0;i<N;i++){
        for (int j=0;j<X-x+3;j++){
            for(int k=0;k<Y-y+3;k++){
                result[i][j][k]=0;
            }
        }
    }
    
    


    //convolution
    for (int m=0;m<N;m++){//iterate over filters
        for (int n=0;n<3;n++){//iterate over rgb channels
            for(int i=0;i<X-x+3;i++){
                for(int j=0;j<Y-y+3;j++){
                    for (int k=0;k<x;k++){
                        for (int l=0;l<y;l++){
                            result[m][i][j]+=input[n][i+k][j+l]*elementsOfFilters[m][n][k][l];
                        }
                    }
                }
            }
        }
    }

    //applying relu function (f(x)=max(0,x))
    for (int i=0;i<N;i++){
        for (int j=0;j<X-x+3;j++){
            for (int k=0;k<Y-y+3;k++){
                result[i][j][k]=max(0,result[i][j][k]);
            }
        }
    }



//print result
    for (int i=0;i<N;i++){
        for (int j=0;j<X-x+3;j++){
            for (int k=0;k<Y-y+3;k++){
                if (k==Y-y+3-1){
                    cout << result[i][j][k];
                }else{
                    cout << result[i][j][k] << " ";
                }

            }
            cout << endl;
        }
        cout<< endl;
    }

//    time

    std::chrono::high_resolution_clock::time_point end = std::chrono::high_resolution_clock::now();
    std::chrono::milliseconds dif = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

    cout<< dif.count()<<" "<<endl;
//    clock_t end = clock();
//       double time_taken = (double)(end - start)/CLOCKS_PER_SEC;
//
//       cout<< (int)(time_taken*double(1000)+0.5)<<" "<<endl;




    return(0);
}
