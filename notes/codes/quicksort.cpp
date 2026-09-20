#include<bits/stdc++.h>
using namespace std;
int partition(int arr[],int l,int r){
    int i = l; int j = r;

    int pavot = arr[l];
    while(i<j){
        while(i<j&&arr[j]>=pavot) j--;
        if(i<j) arr[i++] = arr[j];
        while(i<j&&arr[i]<=pavot) i++;
        if(i<j) arr[j--] = arr[i];
    }
    arr[i] = pavot;
    return i;
}

void qsort(int arr[],int l ,int r){
    if(l>=r) return;
    int k = partition(arr,l,r);
    qsort(arr,l,k-1);
    qsort(arr,k + 1,r);
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; cin>>n;

    int arr[n]; memset(arr,0,n);

    for(int i = 0;i<n;i++) cin>>arr[i];

    qsort(arr,0,n-1);

    for(int i = 0;i<n;++i) cout<<arr[i]<<" ";

    return 0;
}