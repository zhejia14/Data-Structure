#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
int node_num, global_c=0;
int path[1000];
bool visit[1000];
typedef struct node{//struct 存放node id 與 x y座標
    int id;
    double x;
    double y;
}node;
long double distance(double x1, double y1, double x2, double y2){//座標計算距離函數
    long double dis=0.0;
    x1=(x1-x2)*(x1-x2);
    y1=(y1-y2)*(y1-y2);
    dis=x1+y1;
    dis=sqrt(dis);
    return dis;
}
void DFS(int i, bool graph[node_num][node_num]){//DFS拜訪spanning tree上所有node
    visit[i]=true;
    path[global_c]=i;//拜訪順序加入path當中形成一條路徑
    global_c++;
    for(int count=0;count<node_num;count++){
        if(graph[i][count] && !visit[count])
            DFS(count, graph);
    }
}
int main() {
    double B;
    scanf("%d %lf", &node_num, &B);
    node array[node_num];
    long double map[node_num][node_num];
    for(int count=0;count<node_num;count++)
        scanf("%d %lf %lf", &array[count].id, &array[count].x, &array[count].y);//input
    for(int count=0;count<node_num;count++)map[count][count]=0.0;//initial map
    for(int i=0;i<node_num-1;i++){//建立map-->存放所有node之間的距離
        for(int j=i+1;j<node_num;j++){
            map[i][j]=distance(array[i].x, array[i].y, array[j].x, array[j].y);
            map[j][i]=map[i][j];
        }
    }
    //Prim's Algorithm
    int paraent[node_num];//存取每個node的父節點
    long double dis[node_num];
    for(int count=0;count<node_num;count++){//initial
        dis[count]=99999;
        visit[count]=false;
    }
    dis[0]=0.0;
    paraent[0]=-1;
    for(int i=0;i<node_num;i++){
        int temp_a=-1, temp_b, min=99999;
        for(int j=0;j<node_num;j++){//尋找不在tree上，且離tree最近的點
            if(!visit[j] && dis[j]<min){
                temp_a=j;//一直記錄最近的點
                min=dis[j];
            }
        }
        if(temp_a==-1)break;//從root開始可連通的點都找完就跳出
        visit[temp_a]=true;
        for(temp_b=0;temp_b<node_num;temp_b++){
            if(!visit[temp_b] && map[temp_a][temp_b]<dis[temp_b]){
                dis[temp_b]=map[temp_a][temp_b];
                paraent[temp_b]=temp_a;//記錄父節點
            }
        }
    }
    bool graph[node_num][node_num];//DFS graph
    for(int i=0;i<node_num;i++){
        visit[i]=false;//預先初始化visit
        for(int j=0;j<node_num;j++){//根據spanning tree建立graph 有連通Ｔ否則為Ｆ
            if(paraent[j]==i && i!=j){
                graph[i][j]=graph[j][i]=true;
            }
            else graph[i][j]=false;
        }
    }
    int uav=0, counter=0;
    int result[node_num][node_num];
    int mark=0;
    double sumdis=0;
    memset(result, -1, sizeof(result));
    for(int count=0;count<node_num;count++){//DFS拜訪spanning tree上所有node
        int C=count;
        do{                                 //將所有的node都設為起點建立不同的拜放順序
            if(!visit[C])DFS(C, graph);
            C++;
            if(C==node_num)C=0;
        }while(C!=count);
        int tmp_uav=0;
        while (mark<=node_num-1) {//讓不同的拜放順序都嘗試走過並找出需要最小uav的順序
            sumdis+=map[path[mark]][path[mark+1]];//利用path，根據map紀錄的距離查表
            if(sumdis>=(B/2)){//距離當大於等於B的一半需要一台uav
                sumdis=0;
                tmp_uav++;
                if(mark+1>=node_num)break;
            }
            if(sumdis<=(B/2)&&mark+1>=node_num){//若是距離不到（B/2）但已經無下一節點直接記錄
                tmp_uav++;
                break;
            }
            mark++;
        }
        if(tmp_uav<uav){//記錄所需最小的uav跟此路徑的起點
            uav=tmp_uav;
            counter=count;
        }
        for(int i=0;i<node_num;i++){//初始化visit ＆ path
            visit[i]=false;
            path[i]=0;
        }
        global_c=0;
    }
    int C=counter;//建立最小的uav跟此起點的路徑（再做一次DFS）
    do{
        if(!visit[C])DFS(C, graph);
        C++;
        if(C==node_num)C=0;
    }while(C!=counter);
    uav=0;counter=0;mark=0;sumdis=0.0;
    while (mark<=node_num-1) {//利用path，根據map紀錄的距離查表
        sumdis+=map[path[mark]][path[mark+1]];
        result[uav][counter]=path[mark];//記錄路徑在result
        counter++;
        if(sumdis>=(B/2)){//距離當大於等於B的一半需要一台uav
            sumdis=0;
            uav++;
            counter=0;
            if(mark+1>=node_num)break;
        }
        if(sumdis<=(B/2)&&mark+1>=node_num){//若是距離不到（B/2）但已經無下一節點直接記錄
            uav++;
            counter=0;
            result[uav][counter]=path[mark];
            break;
        }
        mark++;
    }
    printf("%d\n", uav);//output
    for(int i=0;i<uav;i++){
        int j=0;
        printf("%d ", i);
        while (result[i][j]!=-1) {
            printf("%d ", result[i][j]);
            j++;
        }
        printf("\n");
    }
    return 0;
}
