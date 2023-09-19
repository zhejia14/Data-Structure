#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
int strNum,strMax,data,Err=0;//字串數、字串MAX長度、錯誤個數
typedef struct node *NodePointer;
typedef struct node{//node num=hash number arr：作為makr 以及對照array index用
    int arr;
    unsigned long num;
}node;
unsigned long MurmurOAAT32(char* key){//hashing function
    unsigned long h=3323198485ul;
    for(;*key;++key){
        h ^= *key;
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}
void Djudge(node tree[][data],char array[][strMax+1],char result[][strMax+1],int posX,int posY,int l){//發出query比對
    unsigned long J;
    printf("1 %d %d\n", posX, posY);//query
    fflush(NULL);
    scanf("%lu",&J);//get maseter num
    if(tree[posX][posY].num!=J){//若是不相同(child內或本身是錯誤字串)
        if(posX==l-1){//本身是錯誤字串 記錄到result
            strcpy(result[Err], array[tree[posX][posY].arr]);
            strcat(result[Err],"\0");
            Err++;
            return;
        }
        Djudge(tree, array, result, posX+1, posY*2, l);//leftchild內有錯誤字串繼續往下層尋找
        if(tree[posX+1][(posY*2)+1].arr<strNum && tree[posX+1][(posY*2)+1].arr>-10){//rightchild避開copy node
            Djudge(tree, array, result, posX+1, (posY*2)+1, l);
        }
    }
    return;
}
int main() {
    scanf("%d %d", &strNum, &strMax);//input
    char array[strNum][strMax+1];memset(array, 0, sizeof(array));
    char result[strNum][strMax+1];memset(result, 0, sizeof(result));
    if(strNum%2!=0 && strNum>1)data=strNum+1;
    else data=strNum;
    int t=0,l=0;
    while(t<data){
        t=pow(2,l);
        l++;
    }//計算tree會有幾層
    node tree[l][data];
    for(int i=0;i<l-1;i++){//initial tree
        for(int j=0;j<data;j++){
            tree[i][j].num=0;
            tree[i][j].arr=-1;
            tree[l-1][j].arr=j;//將最底層的arr index填入
        }
    }
    for(int i=0;i<strNum;i++){//字串輸入與hashing
        scanf("%s", array[i]);
        strcat(array[i], "\0");
        tree[l-1][i].num=MurmurOAAT32(array[i]);
        tree[l-1][i].arr=i;
    }
    if(data!=strNum)tree[l-1][data-1].num=tree[l-1][data-2].num;//當字串數為奇數個copy最後一個node
    int i=l-1,j=0,k=0,len=data;
    while(i>0){//hashing
        while(k<len){
            unsigned long add=tree[i][k].num+tree[i][k+1].num;
            char str[1000];
            int counter=0;
            while(add!=0){//transform hashinh num to char
                str[counter]=(char)(add%10+48);
                add/=10;
                counter++;
            }
            char inverse[counter+1];
            for(int l=0,k=counter-1;l<counter;l++,k--)inverse[l]=str[k];
            inverse[counter]='\0';
            tree[i-1][k/2].num=MurmurOAAT32(inverse);
            k+=2;
            j++;
        }
        if(j%2!=0 && j!=1){//當此層node數為奇數個copy最後一個node
            tree[i-1][j].num=tree[i-1][j-1].num;
            tree[i-1][j].arr=-10;
            len=j+1;
        }else len=j;
        i--;
        k=0;
        j=0;
    }
    Djudge(tree, array, result, 0, 0, l);
    printf("2 %d\n", Err);//output
    for(int i=0;i<Err;i++)printf("%s\n", result[i]);
    return 0;
}
