#include <stdio.h>

#include <string.h>
#include <stdlib.h>
#include <sys/stat.h> 
#include <uci.h>
#include <unistd.h>
#include <syslog.h>
#include <stdarg.h>
#include <time.h>

int debug = 0;

void xprintf(const char *fmt, ...)
{
    if( debug==0 ) 
    {
        return;
    }

    va_list args;
    char buf[2048]={0};
    int  len = 0;
    memset((void *)buf, 0, sizeof(buf));
#ifdef _WIN32
    time_t tnow = time(NULL);
    struct tm* ptm = localtime(&tnow);
    len = sprintf(buf, "--%d-%.2d-%.2d %.2d:%.2d:%.2d--", ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
#else
    time_t tnow;
    time(&tnow);
    struct tm *ptm = localtime(&tnow);//gmtime(&tnow);
    len = sprintf(buf, "[%d-%02d-%02d %02d:%02d:%02d]", (1900+ptm->tm_year),(1+ptm->tm_mon),ptm->tm_mday, ptm->tm_hour, ptm->tm_min, ptm->tm_sec);
#endif
    va_start(args, fmt);
    vsnprintf(&buf[len], (sizeof(buf) - len), fmt, args);
    va_end(args);

#ifdef _ANDROID
    LOGD("%s\n", buf);
#else
    printf("%s\n", buf);
#endif
}

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
				printf("\tprintf(\"line %d\\n\", __LINE__);\n");
				return flag;
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
	if (argc != 3)
	{
		printf("use:%s source_file_name.c dist_file_name.c\n", argv[0]);
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
