/*************************************************
 *Author*        :wayne
 *Description*   :http模块
 *Created Time*  : Fri 24 Apr 2020 03:58:11 PM CST

**************************************************/

#include <errno.h>
#include "http.h"

static const char* get_file_type(const char *type);
static void parse_uri(char *uri, int length, char *filename, char *query);
static void do_error(int fd, char *cause, char *err_num, char *short_msg, char *long_msg);
//static void serve_static(int fd, char *filename, size_t filesize, http_out *out);

static char *ROOT = NULL;

mime_type_t mime[] = 
{
		{".html", "text/html"},
		{".xml", "text/xml"},
		{".xhtml", "application/xhtml+xml"},
		{".txt", "text/plain"},
		{".rtf", "application/rtf"},
		{".pdf", "application/pdf"},
		{".word", "application/msword"},
		{".png", "image/png"},
		{".gif", "image/gif"},
		{".jpg", "image/jpeg"},
		{".jpeg", "image/jpeg"},
		{".au", "audio/basic"},
		{".mpeg", "video/mpeg"},
		{".mpg", "video/mpeg"},
		{".avi", "video/x-msvideo"},
		{".gz", "application/x-gzip"},
		{".tar", "application/x-tar"},
		{".css", "text/css"},
		{NULL , "text/plain"},
};

const char* get_file_type(const char *type)
{
		//将type和索引表中后缀比较，返回类型用于填充Content-type字段
		for (int i = 0; mime[i].type != NULL; ++i)
		{
				if (strcmp(type, mime[i].type) == 0)
						return mime[i].value;
		}
		//未识别则直接返回"text/plain"
		return "text/plain";
}

static void parse_uri(char *uri_start, int uri_length, char *filename, char *query)
{
		uri_start[uri_length] = '\0';
		//找到'?'位置界定非参部分
		char *delim_pos = strchr(uri_start, '?');
		int filename_length = (delim_pos != NULL) ? ((int)(delim_pos - uri_start)) : uri_length;
		strcpy(filename, ROOT);
		//将uri中属于'?'之前部分内容追加到filename
		strncat(filename, uri_start, filename_length);
		//在请求中找到最后一个'/'位置界定文件位置
		char *last_comp = strrchr(filename, '/');
		//在文件名中找到最后一个'.'界定文件类型
		char *last_dot = strrchr(last_comp, '.');
		//请求文件时末尾加'/'
		if ((last_dot == NULL) && (filename[strlen(filename) - 1] != '/'))
		{
				strcat(filename, "/");
		}
		//默认请求index.html
		if (filename[strlen(filename) - 1] == '/')
		{
				strcat(filename, "index.html");
		}

		return;
}

void do_error(int fd, char *cause, char *err_num, char *short_msg, char *long_msg)
{
		char header[MAXLINE];
		char body[MAXLINE];

    // 用log_msg和cause字符串填充错误响应体
    sprintf(body, "<html><title>fasver Error<title>");
    sprintf(body, "%s<body bgcolor=""ffffff"">\n", body);
    sprintf(body, "%s%s : %s\n", body, err_num, short_msg);
    sprintf(body, "%s<p>%s : %s\n</p>", body, long_msg, cause);
    sprintf(body, "%s<hr><em>fasver web server</em>\n</body></html>", body);

    // 返回错误码，组织错误响应头
    sprintf(header, "HTTP/1.1 %s %s\r\n", err_num, short_msg);
    sprintf(header, "%sServer: fasver\r\n", header);
    sprintf(header, "%sContent-type: text/html\r\n", header);
    sprintf(header, "%sConnection: close\r\n", header);
    sprintf(header, "%sContent-length: %d\r\n\r\n", header, (int)strlen(body));

		//Add 404 Page

		//发送错误信息
		rio_writen(fd, header, strlen(header));
		rio_writen(fd, body, strlen(body));

		return;
}

