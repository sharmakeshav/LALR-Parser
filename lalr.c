#include<stdio.h>
#include<string.h>
#include <ctype.h>

void check(int);
int add(int ,char ,int,char,int);
int present(int ,char);
void print();
void newState();
void isFinish();
void calculateFirst(char ,int );
void updateLookAhead(int,int,char,int);
void checkDuplicate();

char str[10][50],closureI[40][20][50],productionIncluded[40][40],first[10][10],lookAhead[40][20][50];
int cnt,numberOfProductionsIncluded[20],state=0,item=0,nowProcessing=0,t=0;

int main()
{
    FILE *fp;
    int i = 0,j,x;
    for(i=0; i<10; i++)
    {
        for(j=0; j<10; j++)
        {
            first[i][j]=';';
        }
    }
    i=0;
    j=0;
    fp = fopen("test.txt","r");

    while(!feof(fp))
    {
        fgets(str[i],50,fp);
        i++;
    }
    fclose(fp);
    cnt = i-1;
    for(x=0; x<i-1; x++)
    {
        calculateFirst(str[x][0],x);
    }
    /* for(j=0;j<i-1;j++){
         printf("%s\n",first[j]);
         exit(0);
     }*/
    i = 0;
    j = 0;
    while(str[0][i]!='\n')
    {
        closureI[0][0][j] = str[0][i];
        if(str[0][i]=='>')
        {
            closureI[0][0][++j] = '.';
        }
        i++;
        j++;
    }
    closureI[0][0][j]='\n';
    lookAhead[0][0][0] = '$';
    lookAhead[0][0][1] = ';';
    check(nowProcessing);
    return 0;
}

void calculateFirst(char c,int x)
{
    int j,temp,n=1;
    temp = cnt;
    for(j=0; j<temp; j++)
    {
        if(str[j][0]==c)
        {
            if(islower(str[j][3]))
                first[x][n++]=str[j][3];
            else
            {
                calculateFirst(str[j][3],x);
            }
        }
    }
    first[x][0]=c;
    cnt = temp;
}

void check(int s)
{
    char c,d,e;
    int i=0,j=0,m=0;
    while(m<=j)
    {
        i=0;
        while(closureI[s][m][i]!='\n')
        {
            if(closureI[s][m][i]=='.'&&closureI[s][m][i+1]!='\n')
            {
                c = closureI[s][m][i+1];
                d = closureI[s][m][i+2];
                //printf("%c kljf\n",d);
                /*printf("%c",d);
                exit(0);*/
                if(isupper(c))
                {
                    if(!present(s,c))
                        j = add(s,c,j,d,m);
                }
                break;
            }
            i++;
        }

        m++;
    }
    isFinish();
    if(state>0)
    {
        //printf("\n%d\n",state);
        checkDuplicate();
    }
    /*if(state==13)
    {
        printf("%d %d\n",nowProcessing,item);
        print();
        exit(0);
    }*/
    newState();
}

int add(int s,char c,int total,char d,int m)
{
    int i=0,k=0,l=0,temp;
    temp = cnt;
    while(i<temp)
    {
        k=0,l=0;
        if(str[i][0] == c)
        {
            total++;
            while(str[i][k]!='\n')
            {
                closureI[s][total][l] = str[i][k];
                if(str[i][k]=='>')
                    closureI[s][total][++l] ='.';
                k++;
                l++;
            }
            closureI[s][total][l] = '\n';
            closureI[s][total+1][0] = '#';
            updateLookAhead(s,total,d,m);
        }
        i++;
    }
    productionIncluded[s][numberOfProductionsIncluded[s]++]=c;
    cnt = temp;
    return total;

}

updateLookAhead(int s,int total,char d,int m)
{
    int i=0,j=0,temp,k=1;
    temp = cnt;
    if(islower(d)||d=='+'||d=='*')
    {
        lookAhead[s][total][i] = d;
        lookAhead[s][total][i+1] = ';';
    }
    else if(isupper(d))
    {
        while(j<temp)
        {
            if(d==first[j][0])
            {
                break;
            }
            j++;
        }
        while(first[j][k]!=';')
        {
            lookAhead[s][total][i++] = first[j][k];
            k++;
        }
        lookAhead[s][total][i] = ';';
    }
    else
    {
        while(lookAhead[s][m][j]!=';')
        {
            lookAhead[s][total][i++] = lookAhead[s][m][j];
            j++;
        }
        lookAhead[s][total][i] = ';';
    }
    lookAhead[s][total+1][0]='#';
}

void newState()
{
    int j = 0,k = 0,i = 0,p=0,q=0;
    //printf("%d",nowProcessing);
    while(closureI[nowProcessing][item][j]!='\n')
    {
        if(closureI[nowProcessing][item][j]=='.'&&closureI[nowProcessing][item][j+1]!='\n')
        {
            state++;
            i = 0;
            k = 0;
            while(closureI[nowProcessing][item][i]!='\n')
            {

                if(closureI[nowProcessing][item][i]=='.')
                {
                    closureI[state][0][k++]=closureI[nowProcessing][item][++i];
                    closureI[state][0][k++]='.';
                    i++;
                    continue;
                }

                closureI[state][0][k] = closureI[nowProcessing][item][i];
                i++;
                k++;
            }
            closureI[state][0][k] = '\n';
            closureI[state][1][0] = '#';
            p=0;
            q=0;
            while(lookAhead[nowProcessing][item][p]!=';')
            {
                lookAhead[state][0][q++]=lookAhead[nowProcessing][item][p];
                p++;
            }
            lookAhead[state][0][q]=';';
            lookAhead[state][1][0]='#';
            /*if(nowProcessing==0&&item==1){
                printf("%s",lookAhead[2][0]);
            }*/
            break;
        }
        j++;
    }
    item++;
    /*if(closureI[nowProcessing][item][0]=='#'){
        isFinish();
    }*/
    check(state);
}

int present(int s,char c)
{
    int i;
    for(i=0; i<numberOfProductionsIncluded[s]; i++)
    {
        if(productionIncluded[s][i]==c)
        {
            return 1;
        }
    }
    return 0;
}

void isFinish()
{
    int x,y;
    x = nowProcessing;
    y = item;
    while(closureI[x][y][0]=='#')
    {
        x++;
        y=0;
        if(x>state)
        {
            print();
            exit(0);
        }
    }
    nowProcessing = x;
    item = y;
}

void checkDuplicate()
{
    int i,j,p=0,q=0,x=0,y=0,flag=0,a=0,b=0;
    i = state-1;
    j = state;
    while(i>=0)
    {
        if(closureI[i][0][0]=='#')
        {
            i--;
            continue;
        }
        while(closureI[j][q][0]!='#')
        {
            q++;
        }
        b = q;
        y = q-1;
        while(closureI[i][p][0]!='#')
        {
            p++;
        }
        x = p-1;
        if(p==q)
        {
            a=0;
            p=0;
            q=0;
            flag=0;
            while(closureI[j][q][0]!='#')
            {
                p = 0;
                while(closureI[i][p][0]!='#')
                {
                    if(!stricmp(closureI[i][p],closureI[j][q]))
                    {
                        if(!stricmp(lookAhead[i][p],lookAhead[j][q])){
                            flag++;
                        }
                    }
                    p++;
                }
                q++;
            }
            if(flag==b)
            {
                closureI[j][0][0]='#';
                break;
            }
        }
        i--;
        p=0;
        q=0;
    }
}

void print()
{
    int i=0,s=0,temp=0,t=0;
    while(s<state)
    {
        if(closureI[s][0][0]=='#')
        {
            s++;
            continue;
        }
        i=0;
        printf("State %d\n",s);
        while(closureI[s][i][0]!='#')
        {
            temp = strlen(closureI[s][i]);
            temp--;
            t = strlen(lookAhead[s][i]);
            t--;
            printf("%.*s,%.*s\n",temp,closureI[s][i],t,lookAhead[s][i]);
            i++;
        }
        s++;
        printf("\n\n\n");
    }
}
