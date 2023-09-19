#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stdbool.h>
int nodes=0, links=0, req=0, accreq=0, accreq2=0;//nodes:node數量 links：link數量 req:requst數量 accreq＆accreq2一個counter記下有多少個可以走的路徑
typedef struct require{//一個結構存儲requst的頭尾及id
  int id;
  int src;
  int dst;
}require;

void creategraph(int map[nodes][nodes], int graph[nodes][nodes]){//利用map去創造一個權重都是1或0的graph給dijkstra
  for(int i=0;i<nodes;i++){
    for(int j=0;j<nodes;j++){
      if(map[i][j]>0)graph[i][j]=1;
    }
  }
}
void changemap(int map[nodes][nodes], int route[],int num, int nodeQM[nodes]){//用來更新link跟node剩下的channel還有QuatumMemories
  for(int i=num,j=num-1;j>=0;i--,j--){//有走過的link其channel都會-1
    map[route[i]][route[j]]=map[route[i]][route[j]]-1;
    map[route[j]][route[i]]=map[route[j]][route[i]]-1;
  }
  for(int i=num;i>=0;i--){//在路徑中如果是頭跟尾-1  中間的因為兩邊都會接所以-2
    if(i==num || i==0)nodeQM[route[i]]=nodeQM[route[i]]-1;
    else nodeQM[route[i]]=nodeQM[route[i]]-2;
  }
  for(int i=0;i<nodes;i++){//如果node的QuatumMemories低於0則此node變為死路連結此node的link也都不能使用channel變為0
    if(nodeQM[i]<=0){
      for(int j=0;j<nodes;j++){
        map[i][j]=0;
        map[j][i]=0;
      }
    }
  }
}
void dijkstra(int graph[nodes][nodes], int src, int dst, int map[nodes][nodes], int nodeQM[nodes], int result[req][nodes+1], int reqid, int mapid){
  //尋求路徑
  int dist[nodes];//記錄起始點到其餘各點的路徑長度
  int parent[nodes];//記錄各點的parent
  bool visit[nodes];//記錄各個點是否在路徑當中
  for (int i = 0; i < nodes; i++){//初始化
    dist[i] = INT_MAX;
    visit[i] = false;
    parent[i]=-1;
  }
  //起始點設定
  dist[src] = 0;
  parent[src]=src;
  for (int count = 0; count < nodes; count++){
    int a=-1, min=INT_MAX;
    for (int i=0; i<nodes;i++){
      if(!visit[i] && dist[i]<min){
        a=i;
        min=dist[i];
      }
    }
    
    if(a==-1)break;//起始點所連通的路徑都已找完則跳出
    visit[a]=true;

    for(int b=0;b<nodes;b++){
      if( b != src && b != dst && nodeQM[b] > 1 && map[a][b]>0 && map[b][a]>0){//並非頭尾  是路徑過程中的節點其QuatumMemories需要大於1否則此路徑為非法
        if(graph[a][b]!=0 && graph[b][a] != 0 && !visit[b] && dist[a]+graph[a][b] < dist[b]){//尋求較短度路徑並且加入路徑
          dist[b] = dist[a] + graph[a][b];
          parent[b]=a;
        }
        else if(graph[a][b]!=0 && graph[b][a] != 0 && !visit[b] && dist[a]+graph[a][b] == dist[b] && a < parent[b]){//若是出現相同長度路徑選擇node id較小的
          dist[b] = dist[a] + graph[a][b];
          parent[b]=a;
        }
      }
      else if( ( b == src || b == dst) && nodeQM[b] > 0 && map[a][b]>0 && map[b][a]>0){//頭尾節點QuatumMemories需要大於0
        if(graph[a][b]!=0 && graph[b][a] != 0 && !visit[b] && dist[a]+graph[a][b] < dist[b]){//尋求較短度路徑並且加入路徑
          dist[b] = dist[a] + graph[a][b];
          parent[b]=a;
        }
        else if(graph[a][b]!=0 && graph[b][a] != 0 && !visit[b] && dist[a]+graph[a][b] == dist[b] && a < parent[b]){//若是出現相同長度路徑選擇node id較小的
          dist[b] = dist[a] + graph[a][b];
          parent[b]=a;
        }
      }
    }
  }
    
  if(parent[dst]!= -1 && mapid==1){//尋求路徑成功
    accreq++;
    result[accreq-1][0]=reqid;  //用來記錄那個req的id 因為不一定所有的req都會成功輸出只要輸成功的
    int route[nodes];
    for(int i=0;i<nodes;i++)route[i] = 0;
    route[0]=dst;
    int i,j;
    for(i=dst,j=1;i!=src;j++){//記下路徑
      route[j]=parent[i];
      i=parent[i];
    }
    for(int k=j-1, m=1;k>=0;k--, m++){
      result[accreq-1][m]=route[k];//存到之後輸出的結果陣列
     }
    changemap(map, route, j-1, nodeQM);
  }
  else if (parent[dst]!=-1 && mapid==2){//尋求路徑成功
    accreq2++;
    result[accreq2-1][0]=reqid;  //用來記錄那個req的id 因為不一定所有的req都會成功輸出只要輸成功的
    int route[nodes];
    for(int i=0;i<nodes;i++)route[i] = 0;
    route[0]=dst;
    int i,j;
    for(i=dst,j=1;i!=src;j++){//記下路徑
      route[j]=parent[i];
      i=parent[i];
    }
    for(int k=j-1, m=1;k>=0;k--, m++){
      result[accreq2-1][m]=route[k];//存到之後輸出的結果陣列
     }
    changemap(map, route, j-1, nodeQM);
  }
}
int main() {
  scanf("%d %d %d", &nodes, &links, &req);
  int nodeQM[nodes];
  int nodeQM2[nodes];
  int map[nodes][nodes];
  int map2[nodes][nodes];
  int graph[nodes][nodes];
  int result[req][nodes+1];
  int result2[req][nodes+1];
  memset(map, 0, sizeof(map));
  memset(map2, 0, sizeof(map2));
  memset(graph, 0, sizeof(graph));
  for(int i=0;i<nodes;i++){//輸入node資料
    int nodeid,QM;
    scanf("%d %d", &nodeid, &QM);
    nodeQM[nodeid]=QM;
    nodeQM2[nodeid]=QM;
  }
  
  for(int i=0;i<links;i++){//輸入link資料
    int linkid, link1ed, link2ed, Channel;
    scanf("%d %d %d %d", &linkid, &link1ed, &link2ed, &Channel);
    map[link1ed][link2ed]=Channel;
    map[link2ed][link1ed]=Channel;
    map2[link1ed][link2ed]=Channel;
    map2[link2ed][link1ed]=Channel;
  }
  require array[req];
  for(int i=0;i<req;i++){//輸入requst資料
    int reqid, reqsrc, reqdst;
    scanf("%d %d %d", &reqid, &reqsrc, &reqdst);
    array[i].id=reqid;
    array[i].src=reqsrc;
    array[i].dst=reqdst;
    for(int j=0;j<nodes+1;j++){
      result[i][j]=-1;
      result2[i][j]=-1;
    }
  }
  for(int i=0;i<req;i++){//將原始的requst開始尋求路徑
    memset(graph, 0, sizeof(graph));
    creategraph(map, graph);
    dijkstra(graph, array[i].src, array[i].dst, map, nodeQM, result, array[i].id, 1);
  }
  
  require sort[req];
  for(int i=0,j=req-1,k=0;i<req;i++){//將輸入的requst進行排序由頭尾直接相連的排在前
    if(map[array[i].src][array[i].dst]>0){
      sort[k]=array[i];
      k++;
    }
    else{
      sort[j]=array[i];
      j--;
    }
  }
  
  for(int i=0;i<req;i++){//將排序過後的requst開始尋求路徑
    memset(graph, 0, sizeof(graph));
    creategraph(map2, graph);
    dijkstra(graph, sort[i].src, sort[i].dst, map2, nodeQM2, result2, sort[i].id, 2);
  }
  
  //輸出的部分  比較判斷原始與排序後誰的accept requst數量較多輸出較多的
  if(accreq>=accreq2){
    printf("%d\n", accreq);
    for(int i=0;i<accreq;i++){
      printf("%d ", result[i][0]);
      for(int j=1;j<nodes+1;j++){
        if(j+1<nodes+1 && result[i][j+1]==-1){
          printf("%d ", result[i][j]);
          break;
        }
        printf("%d ", result[i][j]);
      }
      printf("\n");
    }
  }
  else if (accreq2>accreq){
    printf("%d\n", accreq2);
    for(int i=0;i<accreq2;i++){
      printf("%d ", result2[i][0]);
      for(int j=1;j<nodes+1;j++){
        if(j+1<nodes+1 && result2[i][j+1]==-1){
          printf("%d ", result2[i][j]);
          break;
        }
        printf("%d ", result2[i][j]);
      }
      printf("\n");
    }
  }
  return 0;
}
