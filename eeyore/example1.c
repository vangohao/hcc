int putint(int b);
int getint();
int a;
int func(int a[],int b)
{
    int i;
    i = 1;
    int s;
    s = 0;
	a[1] = 0;
    while (s<25)
    {
        s = s + i ;
        if(i == 5) i = 0;
        if(i >6 && (i>7 || i>8))
        {
            a[i] = i*s;
        }
        else i  = a[i] + 1;
    }
	return s;
}
int b;
int main()
{
    int aa[5];   /*fuck you */
    int b;/*
	b = 1;*/
	int i;
	i = 0;
	while(i<5)
	{
	aa[i] = getint();
i = i + 1;
	}
    b = func(aa,b);
	i = putint(b);
    return 0;
}
