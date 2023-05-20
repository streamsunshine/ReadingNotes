#include	"unpipc.h"

int
main(int argc, char **argv)
{
	size_t	n;
	char	buff[MAXLINE], command[MAXLINE];
	FILE	*fp;

		/* 4read pathname */
	fgets(buff, MAXLINE, stdin);
	n = strlen(buff);		/* fgets() guarantees null byte at end */
	if (buff[n-1] == '\n')
		n--;				/* delete newline from fgets() */

	snprintf(command, sizeof(command), "cat %s", buff);
	fp = popen(command, "r");

		/* 4copy from pipe to standard output */
	while (fgets(buff, MAXLINE, fp) != NULL)
		fputs(buff, stdout);

	pclose(fp);
	exit(0);
}
