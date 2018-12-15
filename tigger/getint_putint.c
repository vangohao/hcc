#include<unistd.h>
int getchar()
{
	char ch=0;
	read(0,&ch,1);
	return ch;
}

int putint(int a)
{
	char buf[40];
	int i = 0;
	int negative = 0;
	if(a<0)
	{
		negative =1;
		a = -a;
	}
	do
	{
		buf[35-i] = a%10 + '0';
		a/=10;
		i++;
	}
	while(a);
	if(negative) {buf[35-i] = '-'; i++;}
	write(1,buf+(36-i),i);
	if(negative) a= -a;
	return a;
}
int getint()
{
	int ch; int negative = 0;
	int result=0;
	while((ch=getchar()) != -1 && 
		(ch == ' ' || ch == '\n' || ch=='\t'));
	if(ch ==  -1 ) return 0;
	do
	{
		if(ch =='-' && negative ==0 ) negative = 1;
		else if(ch =='-') return 0;
		else
		{
			result = result * 10 + ch - '0';
			negative = 2;
		}
		ch = getchar();
	}
	while(ch>'9' || ch <'0');
	if(negative)   return -result;
	else return result;
}
