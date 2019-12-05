#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <grp.h>
#include <pwd.h>
#include <time.h>

char root[4] =  {'\0' };

char print_file_type(mode_t mode){
	if(mode & S_IFREG)//regular file
		return '-';
	else if(mode & S_IFDIR)
		return 'd';
	else if(mode & S_IFIFO)
		return 'p';
	else if(mode & S_IFSOCK)
		return 's';
	else if(mode & S_IFLNK)
		return 'l';
	else if(mode & S_IFBLK)
		return 'b';
	else 
		return 'R';
}

void print_roots(mode_t mode){

	printf((mode & S_IRUSR) ? "r" : "-");
	printf((mode & S_IWUSR) ? "w" : "-");
	printf((mode & S_IXUSR) ? "x" : "-");
	printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
}

void print_time(struct stat *mystat){
	char date[50];
	strftime(date, 50, "%b  %d  %H:%M\t", localtime(&(mystat->st_ctime)));
    printf("%s", date);
}
int main(){
	
	//taking dir name
	DIR *dir = opendir(".");

	if(!dir){
		printf("Can't open dir\n");
		exit(-1);
	}

	struct dirent *mydir = NULL;
	struct stat *mystat = calloc(1, sizeof(struct stat));
	struct passwd *myuser = calloc(1, sizeof(struct passwd));	
	struct group *mygroup  = calloc(1, sizeof(struct group));

	while((mydir = readdir(dir)) != NULL){
		fflush(stdout); 
		stat(mydir->d_name, mystat);
		myuser = getpwuid(mystat->st_uid);
		mygroup = getgrgid(mystat->st_gid);

		printf("%c", print_file_type(mystat->st_mode));
		print_roots(mystat->st_mode);
		printf("%4lu ", mystat->st_nlink);
		printf("%s ", myuser->pw_name);
		printf("%s ", mygroup->gr_name);
		printf("%5lu ", mystat->st_size);
		print_time(mystat);
		printf("%s\n", mydir->d_name);
	}

	closedir(dir);

	return 0;	
}
