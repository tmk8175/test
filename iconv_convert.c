#include <iconv.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

int code_convert(char *from_charset, char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
	{
		return -1;
	}
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
	{
		return -1;
	}
	iconv_close(cd);
	*pout = '\0';

	return 0;
}

int utf2unicode(char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{
	return code_convert("UTF-8", "UCS-2BE", inbuf, inlen, outbuf, outlen);
}

int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{
	return code_convert("UTF-8", "gb2312", inbuf, inlen, outbuf, outlen);
}

int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen) 
{
	return code_convert("gb2312", "UTF-8", inbuf, inlen, outbuf, outlen);
}

int main(void) 
{
	char *s = "中国";
	int fd = open("test.txt", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
	char buf[10];
	u2g(s, strlen(s), buf, sizeof(buf));
	write(fd, buf, strlen(buf));
	close(fd);

	fd = open("test.txt2", O_RDWR|O_CREAT, S_IRUSR | S_IWUSR);
	char buf2[10];
	g2u(buf, strlen(buf), buf2, sizeof(buf2));
	write(fd, buf2, strlen(buf2));
	close(fd);
	return 1;
}
//iconv -l
//iconv -f utf-8 -t gb2312 ./software_.txt > ./software_asserts.txt
