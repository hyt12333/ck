#include<stdio.h>
#include<stdlib.h>

#define False 0
#define True 1

/********主要数据结构********/
char NAME[100]={0};//资源的名称
int Max[100][100]={0};//最大需求矩阵
int Allocation[100][100]={0};//系统已分配矩阵
int Need[100][100]={0};//还需要资源矩阵
int Available[100]={0};//可用资源矩阵
int Request[100]={0};//请求资源向量	
int Work[100]={0};//存放系统可提供资源量 
int Finish[100]={0}; //标记系统是否有足够的资源分配给各个进程 
int Security[100]={0};//存放安全序列

int M=100;//进程的最大数
int N=100;//资源的最大数

/********初始化数据：输入进程数量、资源种类、各种资源可利用数量、
  各进程对资源最大需求量、各进程的资源已分配数量等。********/
void init()
{
    /* m为进程个数，即矩阵行数，n为资源种类，即矩阵列数。*/
    int i,j,m,n;
    int number,flag;
    char name;//输入资源名称
    int temp[100]={0};//统计已经分配的资源
    //输入系统资源数目及各资源初试个数 
    printf("系统可用资源种类为:");
    scanf("%d",&n);
    N=n;
    for(i=0;i<n;i++)
    {
        printf("资源%d的名称:",i);
        fflush(stdin);  //清空输入流缓冲区的字符，注意必须引入#include<stdlib.h>头文件
        scanf("%c",&name);
        NAME[i]=name;
        printf("资源%c的初始个数为:",name);	
        scanf("%d",&number);
        Available[i]=number;
    }

    //输入进程数及各进程的最大需求矩阵 
    printf("\n请输入进程的数量:");	
    scanf("%d",&m);
    M=m;
    printf("请输入各进程的最大需求矩阵的值[Max]:\n");
    do{
        flag = False;
        for(i=0;i<M;i++)
            for(j=0;j<N;j++)
            {
                scanf("%d",&Max[i][j]);
                if(Max[i][j]>Available[j])
                    flag = True;				
            }
        if(flag)
            printf("资源最大需求量大于系统中资源最大量，请重新输入!\n");								
    } while(flag);


    //输入各进程已经分配的资源量，并求得还需要的资源量 
    do{
        flag=False;
        printf("请输入各进程已经分配的资源量[Allocation]:\n");
        for(i=0;i<M;i++)
        {
            for(j=0;j<N;j++)
            {
                scanf("%d",&Allocation[i][j]);
                if(Allocation[i][j]>Max[i][j])  
                    flag=True;				
                Need[i][j]=Max[i][j]-Allocation[i][j];
                temp[j]+=Allocation[i][j];//统计已经分配给进程的资源数目
            }
        }
        if(flag)
            printf("分配的资源大于最大量，请重新输入!\n");		
    }while(flag);

    //求得系统中可利用的资源量 
    for(j=0;j<N;j++)
        Available[j]=Available[j]-temp[j];
}

/********显示资源分配矩阵********/
void showdata()
{
    int i,j;
    printf("*************************************************************\n");
    printf("系统目前可用的资源[Available]:\n");
    for(i=0;i<N;i++)
        printf("%c  ",NAME[i]);
    printf("\n");
    for(j=0;j<N;j++)
        printf("%d  ",Available[j]);
    printf("\n");
    printf("系统当前的资源分配情况如下：\n");
    printf("            Max   	 Allocation    Need\n");
    printf("进程名     ");
    //输出与进程名同行的资源名，Max、Allocation、Need下分别对应 
    for(j=0;j<3;j++){
        for(i=0;i<N;i++)
            printf("%c  ",NAME[i]);
        printf("     ");
    }
    printf("\n");
    //输出每个进程的Max、Allocation、Need 
    for(i=0;i<M;i++){
        printf(" P%d        ",i);
        for(j=0;j<N;j++)
            printf("%d  ",Max[i][j]);
        printf("     "); 
        for(j=0;j<N;j++)
            printf("%d  ",Allocation[i][j]);
        printf("     "); 
        for(j=0;j<N;j++)
            printf("%d  ",Need[i][j]);
        printf("\n");
    }
}

/********尝试分配资源********/
int test(int i) //试探性的将资源分配给第i个进程 
{ 
    for(int j=0;j<N;j++)
    {
        Available[j]=Available[j]-Request[j];
        Allocation[i][j]=Allocation[i][j]+Request[j];
        Need[i][j]=Need[i][j]-Request[j];
    }
    return True;
}

/********试探性分配资源作废********/
int Retest(int i) //与test操作相反 
{ 
    for(int j=0; j<N; j++)
    {
        Available[j] = Available[j] + Request[j];
        Allocation[i][j] = Allocation[i][j] - Request[j];
        Need[i][j] = Need[i][j] + Request[j];
    }
    return True;
}

/********安全性算法********/
int safe()
{
    int i,j,k=0,m,apply;
    //初始化work 
    for(j=0;j<N;j++)
        Work[j] = Available[j];
    //初始化Finish 
    for(i=0;i<M;i++) 
        Finish[i] = False;
    //求安全序列 
    for(i=0;i<M;i++){ 
        apply=0;
        for(j=0;j<N;j++){
            if(Finish[i]==False && Need[i][j]<=Work[j])
            {   
                apply++;
                //直到每类资源尚需数都小于系统可利用资源数才可分配
                if(apply==N)
                {  
                    for(m=0;m<N;m++)
                        Work[m]=Work[m]+Allocation[i][m];//更改当前可分配资源
                    Finish[i]=True;
                    Security[k++]=i;
                    i=-1; //保证每次查询均从第一个进程开始		
                }
            }
        }
    }

    for(i=0;i<M;i++){
        if(Finish[i]==False){
            printf("系统不安全\n");//不成功系统不安全
            return False;
        }
    }
    printf("系统是安全的!\n");//如果安全，输出成功
    printf("存在一个安全序列:");
    for(i=0;i<M;i++){//输出运行进程数组
        printf("P%d",Security[i]);
        if(i<M-1) 
            printf("->");
    }
    printf("\n");
    return True;
}

/********利用银行家算法对申请资源进行试分********/
void bank()
{
    int flag = True;//标志变量，判断能否进入银行家算法的下一步 
    int i,j;

    printf("请输入请求分配资源的进程号(0-%d):",M-1); 
    scanf("%d",&i);//输入须申请资源的进程号

    printf("请输入进程P%d要申请的资源个数:\n",i);
    for(j=0;j<N;j++)
    {
        printf("%c:",NAME[j]);
        scanf("%d",&Request[j]);//输入需要申请的资源
    }

    //判断银行家算法的前两条件是否成立 
    for (j=0;j<N;j++)
    {
        if(Request[j]>Need[i][j])//判断申请是否大于需求，若大于则出错
        { 
            printf("进程P%d申请的资源大于它需要的资源",i);
            printf("分配不合理，不予分配！\n");
            flag = False;
            break;
        }
        else 
        {
            if(Request[j]>Available[j])//判断申请是否大于当前可分配资源，若大于则出错
            {                         
                printf("进程%d申请的资源大于系统现在可利用的资源",i);
                printf("\n");
                printf("系统尚无足够资源，不予分配!\n");
                flag = False;
                break;
            }
        }
    }
    //前两个条件成立，试分配资源，寻找安全序列 
    if(flag) {
        test(i); //根据进程需求量,试分配资源 
        showdata(); //根据进程需求量，显示试分配后的资源量 
        if(!safe()) //寻找安全序列
        {
            Retest(i);
            showdata();
        }
    }
}


int main()//主函数
{	
    char choice;
    printf("\t---------------------------------------------------\n");
    printf("\t||                                               ||\n");
    printf("\t||               银行家算法的实现                ||\n");
    printf("\t||                                               ||\n");
    printf("\t||                                               ||\n");
    printf("\t||                     在此输入个人姓名：******  ||\n");
    printf("\t||                                               ||\n");
    printf("\t---------------------------------------------------\n");
    init();//初始化数据
    showdata();//显示各种资源
    //用银行家算法判定系统当前时刻是否安全，不安全就不再继续分配资源 
    if(!safe()) exit(0);

    do{
        printf("*************************************************************\n");
        printf("\n");
        printf("\n");
        printf("\t-------------------银行家算法演示------------------\n");
        printf("                     R(r):请求分配   \n");	
        printf("                     E(e):退出       \n");
        printf("\t---------------------------------------------------\n");
        printf("请选择：");
        fflush(stdin);  //清空输入流缓冲区的字符，注意必须引入#include<stdlib.h>头文件
        scanf("%c",&choice);
        switch(choice)
        {
            case 'r':
            case 'R':
                bank();break;			
            case 'e':
            case 'E':
                exit(0);
            default: printf("请正确选择!\n");break;
        }
    } while(choice);
}
