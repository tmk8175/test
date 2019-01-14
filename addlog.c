#include <stdio.h>

int checkchar(char *p)
{
	static int flag = 0;
	printf("%s", p);
    while(*p)
    {
        if(*p != NULL && *p != '\r' && *p != '\n')
		{
			if (*p == ')')
			{
				flag = 1;
			}
			if (flag == 1 && *p == '{')
			{
				flag = 2;
				printf("\tprintf(\"line %%d\\n\", __LINE__);\n");
			}
			if (flag == 1 && *p != ' ' && *p != '\r' && *p != '\n' && *p != '\t' && *p != ')' && *p != '{')
			{
				flag = 0;
			}
			p++;
			continue;
		}
        p++;
    }
	if (*p == '\0')
	{
		return 0;
	}
	if (flag == 1 && *p != ' ' && *p != '\r' && *p != '\n' && *p != '\t' && *p != ')' && *p != '{')
	{
		flag = 0;
	}
	else
	{
		flag = 0;
	}
	return flag;
}

int main(int argc, char** argv)
{
	if (argc != 2)
	{
		printf("use:%s source_file_name.c dist_file_name.c\n");
		return 0;
	}
	FILE *fp1,*fp2;
	char str[128];
	if ((fp1=fopen(argv[1], "r")) == NULL)
	{
		printf("cannot open %s file/n", argv[1]);
		exit(0);
	}
	if((fp2=fopen(argv[2],"w"))==NULL)
	{
		printf("cannot open %s file/n", argv[2]);
		exit(0);
	}
	while (fgets(str,128,fp1) > 0)
	{
		fputs(str,fp2 );
		int ret = checkchar(str);
		if (ret == 2)
		{
			fputs("\tprintf(\"line %%d\\n\", __LINE__);\n", fp2 );
		}
	}
	fclose(fp1);
	fclose(fp2);
	return 0;
}