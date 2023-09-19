#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int srcY, srcX, reqnum, accreq=0, accreq2=0, accreq3=0, accreq4=0;
typedef  struct req{//requst 的結構 smallbase存形狀中x佔較小的
  int id;
  int shape1[2];
  int shape2[2];
  int shape3[2];
  int shape4[2];
  int shape5[2];
  int smallbase[2];
}req;
int compare(const void *req1, const void *req2){//qsort的比較function讓requsts由x佔較小的排到大
  int ret=((req*)req1)->smallbase[1]-((req*)req2)->smallbase[1];
  if(ret>0)return 1;
  else if (ret<0)return -1;
  return 0;
}
void case1(int result[reqnum][5], req array[reqnum]){//baseline
  int posY=0, posX=0;
  int far=srcX;//剩下的x量
  int count=0;
  while(far>0){//一直放入requst直到我的x用到沒有或是所有的requsts跑完
    if(array[count].shape1[1]<=far && array[count].shape1[0]<=srcY){
      result[accreq][0]=array[count].id;
      result[accreq][1]=array[count].shape1[0];
      result[accreq][2]=array[count].shape1[1];
      result[accreq][3]=posY;
      result[accreq][4]=posX;
      posX=posX+array[count].shape1[1];
      accreq++;
      far=far-array[count].shape1[1];
    }//if end
    count++;
    if(far>0 && count>=reqnum)break;//所有的requsts跑完就跳出
  }//while end
}
void case2(int result[reqnum][5], req array[reqnum],int casenum){//基於baseline的優化有case 2 / case 3
  int posY=0, posX=0;
  int far=srcX;//剩下的x量
  int count=0;
  while(far>0 && casenum==2){//放入的requst都是這項requst中x佔最少的依據input的順序
    if(array[count].smallbase[1]<=far && array[count].smallbase[0]<=srcY){
      result[accreq2][0]=array[count].id;
      result[accreq2][1]=array[count].smallbase[0];
      result[accreq2][2]=array[count].smallbase[1];
      result[accreq2][3]=posY;
      result[accreq2][4]=posX;
      posX=posX+array[count].smallbase[1];
      accreq2++;
      far=far-array[count].smallbase[1];
    }
    count++;
    if(far>0 && count>=reqnum)break;
  }
  while(far>0 && casenum==3){//放入的requst都是這項requst中x佔最少的依據qsort先對requst進行排列讓x佔少的先排
    if(array[count].smallbase[1]<=far && array[count].smallbase[0]<=srcY){
      result[accreq3][0]=array[count].id;
      result[accreq3][1]=array[count].smallbase[0];
      result[accreq3][2]=array[count].smallbase[1];
      result[accreq3][3]=posY;
      result[accreq3][4]=posX;
      posX=posX+array[count].smallbase[1];
      accreq3++;
      far=far-array[count].smallbase[1];
    }
    count++;
    if(far>0 && count>=reqnum)break;
  }
}
void output(int result[reqnum][5], int accept){//印出結果
  printf("%d\n", accept);
      for(int i=0;i<accept;i++){
        printf("%d %dx%d %d %d\n", result[i][0], result[i][1], result[i][2],result[i][3], result[i][4]);
      }
}
void inputresult(req in, int posY, int posX,int result[reqnum][5]){//給 case3填入結果的function
  result[accreq4][0]=in.id;
  result[accreq4][1]=in.smallbase[0];
  result[accreq4][2]=in.smallbase[1];
  result[accreq4][3]=posY;
  result[accreq4][4]=posX;
}
void case3(int result[reqnum][5], req array[reqnum]){//Y的量大於等於16的狀況，將不同的形狀都向上疊
  if(srcY<16)return;
  int posY=0, posX=0;
  int far=srcX;
  int count=0;
  int mark=0;
  while(far>0){//一直放入requst直到x的量沒有
    if(array[count].smallbase[1]==1){//16x1的形狀不向上疊一根一根排列
      inputresult(array[count], posY, posX, result);
      posX=posX+array[count].smallbase[1];
      accreq4++;
      far=far-array[count].smallbase[1];
    }
    if(array[count].smallbase[1]==2 && far>=2){//8x2的形狀可以疊兩層
      mark=2;
      if(posY==0){inputresult(array[count], posY, posX, result);posY=8;accreq4++;}//第一層
      else if(posY==8){//第二層 之後就會向下一個x前進不再往上
        inputresult(array[count], posY, posX, result);
        posY=0;
        posX=posX+array[count].smallbase[1];
        accreq4++;
        far=far-array[count].smallbase[1];
      }
    }//smallbase 2
    if(array[count].smallbase[1]==4 && far>=4){//4x4的形狀可以疊4層
      if(mark!=4 && posY>0 && mark!=0){//如果上一個形狀並不是4x4會直接往下一個x前進不再理會上面的剩下空間
        posY=0;
        posX=posX+array[count-1].smallbase[1];
        far=far-array[count-1].smallbase[1];
        if(far<4)return;
      }
      mark=4;
      if(posY==0){inputresult(array[count], posY, posX, result);posY=4;accreq4++;}//第一層
      else if(posY==4){inputresult(array[count], posY, posX, result);posY=8;accreq4++;}//第2層
      else if(posY==8){inputresult(array[count], posY, posX, result);posY=12;accreq4++;}//第3層
      else if(posY==12){//第4層 之後就會向下一個x前進不再往上
        inputresult(array[count], posY, posX, result);
        posY=0;
        posX=posX+array[count].smallbase[1];
        accreq4++;
        far=far-array[count].smallbase[1];
      }
    }//smallbase 4
    if(array[count].smallbase[1]==8 && far>=8){//2x8的形狀可以疊8層
      if(mark!=8 && posY>0 && mark!=0){//如果上一個形狀並不是2x8會直接往下一個x前進不再理會上面的剩下空間
        posY=0;
        posX=posX+array[count-1].smallbase[1];
        far=far-array[count-1].smallbase[1];
        if(far<8)return;
      }
      mark=8;
      if(posY==0){inputresult(array[count], posY, posX, result);posY=2;accreq4++;}//第1層
      else if(posY==2){inputresult(array[count], posY, posX, result);posY=4;accreq4++;}//2
      else if(posY==4){inputresult(array[count], posY, posX, result);posY=6;accreq4++;}//3
      else if(posY==6){inputresult(array[count], posY, posX, result);posY=8;accreq4++;}//4
      else if(posY==8){inputresult(array[count], posY, posX, result);posY=10;accreq4++;}//5
      else if(posY==10){inputresult(array[count], posY, posX, result);posY=12;accreq4++;}//6
      else if(posY==12){inputresult(array[count], posY, posX, result);posY=14;accreq4++;}//7
      else if(posY==14){//第8層 之後就會向下一個x前進不再往上
        inputresult(array[count], posY, posX, result);
        posY=0;
        posX=posX+array[count].smallbase[1];
        accreq4++;
        far=far-array[count].smallbase[1];
      }
    }//smallbase 8
    if(array[count].smallbase[1]==16 && far>=16){//1x16的形狀可以疊16層
      if(mark!=16 && posY>0 && mark!=0){//如果上一個形狀並不是1x16會直接往下一個x前進不再理會上面的剩下空間
        posY=0;
        posX=posX+array[count-1].smallbase[1];
        far=far-array[count-1].smallbase[1];
        if(far<16)return;
      }
      mark=16;
      if(posY==0){inputresult(array[count], posY, posX, result);posY=1;accreq4++;}//第1層
      else if(posY==1){inputresult(array[count], posY, posX, result);posY=2;accreq4++;}//2
      else if(posY==2){inputresult(array[count], posY, posX, result);posY=3;accreq4++;}//3
      else if(posY==3){inputresult(array[count], posY, posX, result);posY=4;accreq4++;}//4
      else if(posY==4){inputresult(array[count], posY, posX, result);posY=5;accreq4++;}//5
      else if(posY==5){inputresult(array[count], posY, posX, result);posY=6;accreq4++;}//6
      else if(posY==6){inputresult(array[count], posY, posX, result);posY=7;accreq4++;}//7
      else if(posY==7){inputresult(array[count], posY, posX, result);posY=8;accreq4++;}//8
      else if(posY==8){inputresult(array[count], posY, posX, result);posY=9;accreq4++;}//9
      else if(posY==9){inputresult(array[count], posY, posX, result);posY=10;accreq4++;}//10
      else if(posY==10){inputresult(array[count], posY, posX, result);posY=11;accreq4++;}//11
      else if(posY==11){inputresult(array[count], posY, posX, result);posY=12;accreq4++;}//12
      else if(posY==12){inputresult(array[count], posY, posX, result);posY=13;accreq4++;}//13
      else if(posY==13){inputresult(array[count], posY, posX, result);posY=14;accreq4++;}//14
      else if(posY==14){inputresult(array[count], posY, posX, result);posY=15;accreq4++;}//15
      else if(posY==15){//第16層 之後就會向下一個x前進不再往上
        inputresult(array[count], posY, posX, result);
        posY=0;
        posX=posX+array[count].smallbase[1];
        accreq4++;
        far=far-array[count].smallbase[1];
      }
    }//smallbase 16
    count++;
    if(far>0 && count>=reqnum)break;
  }
}
int main() {
  scanf("%d %d %d", &srcY, &srcX, &reqnum);
  int result[reqnum][5];//共有4種算法4種result
  int result2[reqnum][5];
  int result3[reqnum][5];
  int result4[reqnum][5];
  req array[reqnum];
  for(int count=0;count<reqnum;count++){//input 會將x佔最少的紀錄在smallbase中
    char x,end;
    scanf("%d", &array[count].id);
    scanf("%d%c%d%c", &array[count].shape1[0], &x, &array[count].shape1[1], &end);
    array[count].smallbase[0]=array[count].shape1[0];
    array[count].smallbase[1]=array[count].shape1[1];
        
    if(end!='\n'){
      scanf("%d%c%d%c", &array[count].shape2[0], &x, &array[count].shape2[1], &end);
      if(array[count].shape2[1]<array[count].smallbase[1]){
        array[count].smallbase[0]=array[count].shape2[0];
        array[count].smallbase[1]=array[count].shape2[1];
      }
    }
    if(end!='\n'){
      scanf("%d%c%d%c", &array[count].shape3[0], &x, &array[count].shape3[1], &end);
      if(array[count].shape3[1]<array[count].smallbase[1]){
        array[count].smallbase[0]=array[count].shape3[0];
        array[count].smallbase[1]=array[count].shape3[1];
      }
    }
    if(end!='\n'){
      scanf("%d%c%d%c", &array[count].shape4[0], &x, &array[count].shape4[1], &end);
      if(array[count].shape4[1]<array[count].smallbase[1]){
        array[count].smallbase[0]=array[count].shape4[0];
        array[count].smallbase[1]=array[count].shape4[1];
      }
    }
    if(end!='\n'){
      scanf("%d%c%d%c", &array[count].shape5[0], &x, &array[count].shape5[1], &end);
      if(array[count].shape5[1]<array[count].smallbase[1]){
        array[count].smallbase[0]=array[count].shape5[0];
        array[count].smallbase[1]=array[count].shape5[1];
      }
    }
  }
  case1(result, array);//base line
  case2(result2, array, 2);//基於base line 的優化
  qsort(array, reqnum, sizeof(req), compare);//讓requsts由x佔較小的排到大
  case2(result3, array, 3);//基於base line 的優化 使用排序過的input
  case3(result4,array);//基於 srcY>＝16的疊到16的疊法
  if(accreq>=accreq2 && accreq>=accreq3 && accreq>=accreq4)output(result, accreq);//爭取最好的結果輸出
  else if(accreq2>=accreq3 && accreq2>=accreq4) output(result2, accreq2);
  else if(accreq3>=accreq4)output(result3, accreq3);
  else output(result4, accreq4);
  return 0;
}

