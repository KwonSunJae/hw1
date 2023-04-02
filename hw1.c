#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "sqlite3.h"
void create_faculty_table(sqlite3 *db){
    
}

static int callback_function(void *notUsed, int argc, char **argv, char **azColName){
    int i=0;
	for(i=0; i<argc; i++){
		if(argv[i]!=NULL)
            printf("%s = %s ",azColName[i],argv[i]);
		else
            printf("%s = %p ",azColName[i],NULL);
	}
	return 0;
}


int main(){
    char *name, *dept_name, *office, *p;
    int i,fd,rc;
    char line[1024];
    char *pLine;
    sqlite3 *faculty_db, *course_db;

    char sql_buf[1024]; 
    rc = sqlite3_open("./faculty.db", &faculty_db);
    create_faculty_table(faculty_db);

    FILE *in = fopen("faculty.txt", "r");
    while (!feof(in)) {
        pLine = fgets(line, 1024, in);
        if(pLine==NULL)
            break;
        name = strtok(pLine, " ");
        dept_name = strtok(NULL, " ");
        office = strtok(NULL, " ");

    }
    fclose(in);
    sqlite3_close(faculty_db);


    return 0;

}
