#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int pre(FILE* input, FILE* output)
{
    char q=0;
    char ch;
    bool str=false;
    bool com=false;
    bool ou=false;
    while((ch=fgetc(input))!=EOF)
    {
        if(ch=='/' && com==false && str==false)
        {

        }
        else if(ch=='*' && q =='/' && com==false && str==false)
        {
            com = true;
        }
        else if(ch!='*' && q=='/' && com==false && str==false)
        {

            if(ou==false)
            fputc(q,output);
            else ou=true;
            fputc(ch,output);
        }
        else if(ch=='*' && com==true)
        {

        }
        else if(ch=='/' && q=='*' && com==true)
        {
            com=false;
            ou=true;
        }
        else if(ch=='"' &&q!='\\' && com==false && str==false)
        {
            str=true;
            fputc(ch,output);
        }
        else if(ch=='"' || ch=='\n' &&q!='\\' && com==false && str==true)
        {
            str=false;
            fputc(ch,output);
        }
        else if(com==false){
            fputc(ch,output);
        }
        q = ch;
    }
    return 0;
}
