#include "stdafx.h"
#include "stdio.h"
#undef __analysis_assume
#include <CodeAnalysis\SourceAnnotations.h>

#define BUF_SIZE 100
#define STR_SIZE 200

void zeroing();

char *my_alloc(size_t size) {
	char *ch  = (char *)malloc(size);
	*ch = NULL;
	ch[size] = NULL;  // null terminate here too, to be safe
	return ch;
}

HRESULT input(char *buf, int len) {
	return (fgets(buf,len,stdin) != NULL)?SEVERITY_SUCCESS:SEVERITY_ERROR;
}

char *do_read() {
	char *buf = my_alloc(STR_SIZE);
	printf("Allocated a string at %x", buf);
	if (!input(buf, STR_SIZE)) {
		printf("error!");
		exit(-1);
	}
	if (*buf = NULL)
		printf("empty string");
	return buf;
}

void copy_data(char *buf1,
               int len1,
               char *buf2,
               int len2) {
	if(len1 >= len2-1)
        return;
    memcpy_s(buf2,len2,buf1,len1);
	buf2[STR_SIZE-1] = NULL; // null terminate, just in case
}

void swap(char *buf1,
	  char *buf2) {
	char *x = buf1;
	buf2 = buf1;
	buf1 = x;
}

int execute(char *buf) {
	return system(buf); // pass buf as command to be executed by the OS
}

void validate([SA_Pre(Tainted=SA_Yes)][SA_Post(Tainted=SA_No)] char *buf) {

    // This is a magical validation method, which turns tainted data
    // into untainted data, for which the code not shown.
    //
    // A real implementation might for example use a whitelist to filter
    // the string.

}

_Check_return_ int test_ready() {
	// code not shown
	return 1;
}

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
	char *buf1 = do_read();
	char *buf2 = my_alloc(BUF_SIZE);
	if (buf2 == NULL)
		exit(-1);
	zeroing();
	test_ready();
	execute(buf1);
        
        char* buf3 = do_read();
	copy_data(buf3, STR_SIZE, buf2, BUF_SIZE);
	execute(buf2);

	char *buf4 = my_alloc(STR_SIZE);
        char *buf5 = do_read();
        swap(buf4, buf5);
        execute(buf4);

}

// *****************************************************************

void zero(int *buf, int len)
{
    int i;
    for(i = 0; i <= len; i++)
        buf[i] = 0;
}

void zeroboth(int *buf, int len, 
              int *buf3, int len3)
{
    int *buf2 = buf;
    int len2 = len;
    zero(buf2, len2);
    zero(buf3, len3);
}

void zeroboth2(int *buf, int len, 
	       int *buf3, int len3)
{
	zeroboth(buf, len3, buf3, len);
}

void zeroing()
{
    int elements[200];
    int oelements[100];
    zeroboth2(elements, 200, oelements, 100);
}
