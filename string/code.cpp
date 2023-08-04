#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using namespace std;


int MoreThanHalfNum_Solution(vector<int>& numbers) {
        if(numbers.size()==1){
            return numbers[0];
        }
        sort(numbers.begin(),numbers.end());
        int count=1;
        int standrad=(numbers.size()%2==1)?numbers.size()/2+1:numbers.size()/2;
        for(size_t i=1;i<numbers.size();i+=count){
            if(numbers[i]==numbers[i-1]){
                count++;
                int tmp=numbers[i];
                for(size_t j=i+1;j<numbers.size();++j){
                    if(count==standrad){
                        return tmp;
                    }
                    if(numbers[j]==tmp){
                        count++;
                    }
                    else{
                        break;
                    }
                }
            }
            count=1;
        }
        return -1;
    }
int main(){
    vector<int> arr={3,3,3,3,3,2,2,2,2};
    MoreThanHalfNum_Solution(arr);
    cout<<sizeof()<<endl;
    return 0;
}