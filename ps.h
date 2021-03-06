#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>

#define MAX_LEN 20
struct ps_info *trav_dir(char dir[]);
int read_info(char d_name[], struct ps_info *p1);
void print_ps(struct ps_info *head);
int is_num(char *);
void uid_to_name();

typedef struct ps_info
{
	char pname[MAX_LEN];
	char user[MAX_LEN];
	int pid;
	int ppid;
	char state;
	struct ps_info *next;
} mps;

mps *trav_dir(char dir[])
{
	DIR *dir_ptr;
	mps *head, *p1, *p2;
	struct dirent *direntp;
	struct stat infobuf;

	if ((dir_ptr = opendir(dir)) == NULL)
		fprintf(stderr, "dir error %s\n", dir);
	else
	{
		head = p1 = p2 = (struct ps_info *)malloc(sizeof(struct ps_info)); // Create a linked list
		while ((direntp = readdir(dir_ptr)) != NULL)					   // Traverse all process directories in the/proc directory
		{
			if ((is_num(direntp->d_name)) == 0) // Determine whether the directory name is a pure number
			{
				if (p1 == NULL)
				{
					printf("malloc error!\n");
					exit(0);
				}

				if (read_info(direntp->d_name, p1) != 0) // Get process information
				{
					printf("read_info error\n");
					exit(0);
				}
				p2->next = p1; // Insert a new node
				p2 = p1;
				p1 = (struct ps_info *)malloc(sizeof(struct ps_info));
			}
		}
	}
	p2->next = NULL;
	return head;
}

int read_info(char d_name[], struct ps_info *p1)
{
	FILE *fd;
	char dir[20];
	struct stat infobuf;

	sprintf(dir, "%s/%s", "/proc/", d_name);
	chdir("/proc");					  // Switch to/proc directory, otherwise stat returns -1
	if (stat(d_name, &infobuf) == -1) // get process USER
		fprintf(stderr, "stat error %s\n", d_name);
	else
		// p1->user=uid_to_name(infobuf.st_uid);
		uid_to_name(infobuf.st_uid, p1);

	chdir(dir); // Switch to/proc/pid directory
	if ((fd = fopen("stat", "r")) < 0)
	{
		printf("open the file is error!\n");
		exit(0);
	}
	while (4 == fscanf(fd, "%d%s%c%d\n", &(p1->pid), p1->pname, &(p1->state), &(p1->ppid))) // Read the contents of the first four fields of the file and store them in the relevant linked list members
	{
		break;
	}
	fclose(fd);
	return 0;
}

void uid_to_name(uid_t uid, struct ps_info *p1) // The process uid gets the process owner user
{
	struct passwd *getpwuid(), *pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL)
	{
		sprintf(numstr, "%d", uid);
		strcpy(p1->user, numstr);
	}
	else
		strcpy(p1->user, pw_ptr->pw_name);
}

int is_num(char p_name[])
{
	int i, len;
	len = strlen(p_name);
	if (len == 0)
		return -1;
	for (i = 0; i < len; i++)
		if (p_name[i] < '0' || p_name[i] > '9')
			return -1;
	return 0;
}

void print_ps(struct ps_info *head)
{
	mps *list;
	printf("USER\t\tPID\tPPID\tSTATE\tPNAME\n");
	for (list = head; list != NULL; list = list->next)
	{
		printf("%s\t\t%d\t%d\t%c\t%s\n", list->user, list->pid, list->ppid, list->state, list->pname);
	}
}
