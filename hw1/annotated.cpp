#include "stdafx.h"
#include "stdio.h"
#include "stdlib.h"

#undef __analysis_assume
#include <CodeAnalysis\SourceAnnotations.h>

#define BUF_SIZE 100
#define STR_SIZE 200

void zeroing();

__out_bcount(size) char *my_alloc(_In_ size_t size) {
	char *ch = (char *)malloc(size);
	//*ch = NULL;																		// FIXED
	if (ch && size > 0)
		ch[size - 1] = NULL;  // null terminate here too, to be safe							
	return ch;
}

[returnvalue:SA_Pre(Tainted = SA_Yes)] HRESULT input([SA_Post(Tainted = SA_Yes)] __inout_bcount(len) char *buf, _In_ int len) {														// FIXED
	return (fgets(buf, len, stdin) != NULL) ? SEVERITY_SUCCESS : SEVERITY_ERROR;				// FIXED
}

[returnvalue:SA_Post(Tainted = SA_Yes)] __out_bcount(STR_SIZE) char *do_read() {
	char *buf = my_alloc(STR_SIZE);
	printf("Allocated a string at %p", buf);											// FIXED
	if (FAILED(input(buf, STR_SIZE))) {													// FIXED
		printf("error!");
		exit(-1);
	}

	return buf;
}

[returnvalue:SA_Post(Tainted = SA_Yes)] void copy_data(__in_bcount(STR_SIZE) char *buf1,
	_In_ int len1,
	__out_bcount(STR_SIZE) char *buf2,
	_In_ int len2) {
	memcpy_s(buf2, len2, buf1, len1);
	buf2[STR_SIZE - 1] = NULL; // null terminate, just in case
}

void swap(__in_bcount(STR_SIZE) char *buf1,
	__out_bcount(STR_SIZE) char *buf2) {
	char *x = buf1;
	buf2 = buf1;
	buf1 = x;
}

int execute([SA_Pre(Tainted = SA_No)] char *buf) {
	return system(buf); // pass buf as command to be executed by the OS
}

void validate([SA_Pre(Tainted = SA_Yes)][SA_Post(Tainted = SA_No)] char *buf) {

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


int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow) {
	char *buf1 = do_read();
	char *buf2 = my_alloc(BUF_SIZE);
	if (buf2 == NULL)
		exit(-1);
	zeroing();
	if (SUCCEEDED(test_ready())) {									// FIXED
		validate(buf1);
		execute(buf1);
	}
	char* buf3 = do_read();
	validate(buf3);
	validate(buf2);
	copy_data(buf3, STR_SIZE, buf2, BUF_SIZE);					// FIXED
																// now buf2 should be 
	validate(buf2);
	execute(buf2);

	char *buf4 = my_alloc(STR_SIZE);
	char *buf5 = do_read();
	swap(buf4, buf5);
	validate(buf4);
	validate(buf5);
	execute(buf4);
}

// *****************************************************************

void zero(__out_bcount(len) int *buf, _In_ int len)
{
	int i;
	for (i = 0; i <= len - 1; i++)															// FIXED 
		buf[i] = 0;
}


void zeroboth(__out_bcount(len) int *buf, _In_ int len,
	__out_bcount(len3) int *buf3, _In_ int len3)
{
	int *buf2 = buf;
	int len2 = len;
	zero(buf2, len2);
	zero(buf3, len3);
}

void zeroboth2(__out_bcount(len) int *buf, _In_ int len,
	__out_bcount(len3) int *buf3, _In_ int len3)
{
	zeroboth(buf, len3, buf3, len);
}

void zeroing()
{
	int elements[200] = { 0 };													// FIXED
	int oelements[100] = { 0 };													// FIXED
	zeroboth2(elements, 200, oelements, 100);
}
