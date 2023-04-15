#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "sqlite3.h"




void create_faculty_table(sqlite3 *db){
    char * sql = "CREATE TABLE Faculty (instructor_name TEXT,department_name TEXT,office TEXT);";
    sqlite3_exec(db, sql, NULL,NULL,NULL);

}

void create_course_table(sqlite3 *db){

    char * sql = "CREATE TABLE Course (code TEXT,section INT,semester INT,year INT,title TEXT,classroom TEXT,instructor_name TEXT,primary key(code,section,semester,year));";
    sqlite3_exec(db, sql, NULL,NULL,NULL);
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

void query_db(sqlite3 *db ){
    char * sql = "SELECT * FROM Course;";
    sqlite3_stmt * stmt;

	sqlite3_prepare_v2(db, sql,-1, &stmt, NULL);
	
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		int num_cols = sqlite3_column_count(stmt);
		
		for (i = 0; i < num_cols; i++)
		{
			switch (sqlite3_column_type(stmt, i))
			{
			case (SQLITE3_TEXT):
				printf("%s ", sqlite3_column_text(stmt, i));
				break;
			case (SQLITE_INTEGER):
				printf("%d ", sqlite3_column_int(stmt, i));
				break;
			default: 
				break;
			}
		}
		printf("\n");

	}

	sqlite3_finalize(stmt);
}

int main(){
    char *name, *dept_name, *office, *p;
    char *code, *section, *semester, *year,*title,*classroom,*instructor_name;
    int i,fd,rc;
    char line[1024];
    char *pLine;
    sqlite3 *faculty_db, *course_db;

    char sql_buf[1024]; 
    rc = sqlite3_open("./faculty.db", &faculty_db);
    create_faculty_table(faculty_db);
    create_course_table(faculty_db);

    FILE *in = fopen("faculty.txt", "r");
    while (!feof(in)) {
        pLine = fgets(line, 1024, in);
        if(pLine==NULL)
            break;
        name = strtok(pLine, " ");
        dept_name = strtok(NULL, " ");
        office = strtok(NULL, "\r");
        char sql[1024];
        sprintf(sql, "INSERT INTO Faculty VALUES ('%s','%s','%s');", name, dept_name,office);
        sqlite3_exec(faculty_db, sql, NULL,NULL,NULL);
    }

    in = fopen("course.txt", "r");
    while (!feof(in)) {
        pLine = fgets(line, 1024, in);
        if(pLine==NULL)
            break;
        code = strtok(pLine, " ");
        section = strtok(NULL, " ");
        semester = strtok(NULL, " ");
        year = strtok(NULL, " ");
        title = strtok(NULL, " ");
        classroom = strtok(NULL, " ");
        instructor_name =  strtok(NULL, "\r");
        char sql[1024];
        sprintf(sql, "INSERT INTO Course VALUES ('%s',%s,%s,%s,'%s','%s','%s');", code, section, semester, year,title,classroom,instructor_name);
        sqlite3_exec(faculty_db, sql, NULL,NULL,NULL);
    }

    query_db(faculty_db);
    
    
    fclose(in);
    sqlite3_close(faculty_db);


    return 0;

}
