#include<bits/stdc++.h>
using namespace std;
int main(){
    int n = 0; cin>>n;
    int arr[n][n];
    memset(arr,0,sizeof(arr));
    arr[0][n/2] = 1;

    int x = 0; int y = n/2;

    for(int k = 2;k<=n * n;++k){
        if(x == 0&&y != n-1){
            x = n-1; ++y;
        }
        else if(x != 0&&y == n-1){
            --x; y = 0;
        }
        else if(x == 0&&y == n-1){
            ++x;
        }
        else if(x != 0&&y != n-1){
            if(arr[x-1][y+1] == 0){
                --x;++y;
            }
            else ++x;
        }

        arr[x][y] = k;
    }


    for(int i = 0;i<n;++i){
        for(int j = 0;j<n;++j) cout<<arr[i][j]<<" ";
        cout<<"\n";
    }

    return 0;
}