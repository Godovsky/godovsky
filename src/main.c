#include <stdio.h>
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct Node
{
        char str[64];
        uint64_t value;
        struct Node * next;
} L_Line;

int main (int argc, char * argv[])
{
	L_Line * Line = calloc (1, sizeof (L_Line));
	L_Line * Begin = Line;
	
	FILE * fd = NULL;
	fd = fopen ("/proc/meminfo", "r");
	if (NULL == fd)
	{
			printf ("Unsuccessful file open (1)!\n\n");
			return 1;
	}

	fscanf (fd, "%[^\n]\n", Line->str);
	sscanf (Line->str, "%*[^:]:%*[^0123456789]%lu", &Line->value);
	if (strlen (Line->str) != 0)
	{
			for (int i = 0; Line->str[i] != '\0'; i++)
					if (Line->str[i] == ':') Line->str[i] = ' ';
			while (!feof (fd))
			{
					Line->next = calloc (1, sizeof (L_Line));
					Line = Line->next;
					Line->next = NULL;
					fscanf (fd, "%[^\n]\n", Line->str);
					sscanf (Line->str, "%*[^:]:%*[^0123456789]%lu", &Line->value);
					for (int i = 0; Line->str[i] != '\0'; i++)
							if (Line->str[i] == ':') Line->str[i] = ' ';
			}
			Line = Begin;
	}
	fclose (fd);
 	while (1)
	{
		if (Line->value < Line->next->value)
		{
			uint64_t tmp = Line->value;
			Line->value = Line->next->value;
			Line->next->value = tmp;
			char tmpStr[100] = {0};
			strcpy (tmpStr, Line->str);
			strcpy (Line->str, Line->next->str);
			strcpy (Line->next->str, tmpStr);
			Line = Begin;
			continue;
		}
		Line = Line->next;
		if (NULL == Line->next) break;
	}
	Line = Begin;
	fd = fopen ("newmeminfo", "w");
	if (NULL == fd)
	{
			printf ("Unsuccessful file open (2)!\n\n");
			return 1;
	}
	while (1)
	{
		fprintf (fd, "%s", Line->str);
		L_Line * tmp = Line;
		Line = Line->next;
		free (tmp);
		if (NULL == Line) break;
		else fprintf (fd, "\n");
	}
	return 0;
}
