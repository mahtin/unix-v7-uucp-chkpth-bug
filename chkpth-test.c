
/*
 * Testing v7 uucp chkpth() function
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* https://github.com/v7unix/v7unix/blob/master/v7/usr/src/cmd/uucp/uucp.h */
/* Line 43-44 */
#define FAIL -1
#define SUCCESS 0


int
/* https://github.com/v7unix/v7unix/blob/master/v7/usr/src/cmd/uucp/prefix.c */
/* Line 8-20 */
prefix(s1, s2)
char *s1, *s2;
{
	char c;

	while ((c = *s1++) == *s2++)
		if (c == '\0')
			return(1);
	return(c == '\0');
}

int
/* https://github.com/v7unix/v7unix/blob/master/v7/usr/src/cmd/uucp/chkpth.c */
/* Line 31-32 */
chkpth(logname, mchname, path)
char *path, *logname, *mchname;
{
	char *s;

	/* https://github.com/v7unix/v7unix/blob/master/v7/usr/src/cmd/uucp/chkpth.c */
	/* Line 64-68 */
	/*  check for /../ in path name  */
	for (s = path; *s != '\0'; s++) {
		if (*s == '/' && prefix("../", (++s)))
			return(FAIL);
	}

	return(SUCCESS);
}

int main()
{
	int result;
	char buffer[255+1];

	while (fgets(buffer, 255, stdin)) {
		buffer[strcspn(buffer, "\n")] = '\0';
		if (strlen(buffer) == 0) {
			printf("\n");
			continue;
		}
		printf("%-50s\t", buffer);
		fflush(stdout);
		result = chkpth(0, 0, buffer);
		if (result == FAIL) {
			printf("%s\n", "FAIL");
		} else {
			printf("%s\n", "SUCCESS");
		}
		fflush(stdout);
	}

}
