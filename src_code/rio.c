#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "rio.h"

ssize_t rio_readn(int fd, void *usrbuf, size_t n)
{
		size_t nleft = n;
		ssize_t nread;
		char *bufp = (char *)usrbuf;
		while (nleft > 0)
		{
				if ((nread = read(fd, bufp, nleft)) < 0)
				{
						if (errno == EINTR)
								nread = 0;
						else 
								return -1;
				}
				else if (nread == 0)
						break;
				nleft -= nread;
				bufp += nread;
		}
		return (n - nleft);
}

ssize_t rio_writen(int fd, void *usrbuf, size_t n)
{
		size_t nleft = n;
		ssize_t nwrite;
		char *bufp = (char *)usrbuf;
		while (nleft > 0)
		{
				if ((nwrite = write(fd, bufp, nleft)) <= 0)
				{
						if (errno == EINTR)
								nwrite = 0;
						else 
								return -1;
				}
				nleft -= nwrite;
				bufp += nwrite;
		}
		return n;
}
