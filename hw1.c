#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#include "sqlite3.h"

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

void select_sql_db(sqlite3 *db, char * sql){

    sqlite3_stmt * stmt;

    printf("Performing query... : %s\n", sql);
	sqlite3_prepare_v2(db, sql,-1, &stmt, NULL);
	
	printf("Got results:\n");
	while (sqlite3_step(stmt) != SQLITE_DONE) {
		int i;
		int num_cols = sqlite3_column_count(stmt);
		
		for (i = 0; i < num_cols; i++)
		{
			switch (sqlite3_column_type(stmt, i))
			{
			case (SQLITE3_TEXT):
				printf("%s|", sqlite3_column_text(stmt, i));
				break;
			case (SQLITE_INTEGER):
				printf("%d|", sqlite3_column_int(stmt, i));
				break;
			default:
                printf("ERROR: %s|", sqlite3_column_text(stmt, i));    
				break;
			}
		}
		printf("\n");

	}

	sqlite3_finalize(stmt);
}
void create_faculty_table(sqlite3 *db){
    char * sql = "CREATE TABLE Faculty (\
        instructor_name TEXT(100),\
        department_name TEXT(100),\
        office TEXT(100));";
    sqlite3_exec(db, sql, NULL,NULL,NULL);

}

void insert_faculty_table(sqlite3 *db ,char * instructor_name, char * department_name, char * office){
    char sql[200];
    sprintf(sql, "INSERT INTO Faculty VALUES ('%s','%s','%s');", instructor_name, department_name,office);
    printf("insert sql : %s \n",sql);
    sqlite3_exec(db, sql, NULL,NULL,NULL);
}
void insert_course_table(sqlite3 *db ,char * code, char * section, char * semester, char * year, char * title, char * classroom, char * instructor_name){
    char sql[300];
    sprintf(sql, "INSERT INTO Course VALUES ('%s',%s,%s,%s,'%s','%s','%s');", code, section, semester, year,title,classroom,instructor_name);
    printf("insert sql : %s \n",sql);
    sqlite3_exec(db, sql, NULL,NULL,NULL);
}
void create_course_table(sqlite3 *db){
    char * errmsg;
    char * sql = "CREATE TABLE Course (\
        code TEXT(20),\
        section INT(10),\
        semester INT(10),\
        year INT(10),\
        title TEXT(40),\
        classroom TEXT(20),\
        instructor_name TEXT(20),\
        primary key(code,section,semester,year));";
    sqlite3_exec(db, sql, NULL,NULL,&errmsg);
    printf("%s",errmsg);

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
        //insert_faculty_table(faculty_db,name,dept_name,office);

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
        
        //insert_course_table(faculty_db,code,section,semester,year,title,classroom,instructor_name);

    }
    //show all
    select_sql_db(faculty_db, "SELECT * FROM Course;");
    select_sql_db(faculty_db, "SELECT * FROM Faculty;");
    //Q2
    select_sql_db(faculty_db, "SELECT title FROM Course WHERE instructor_name LIKE 'Wookhee_Kim';");
    //Q3
    select_sql_db(faculty_db, "SELECT title FROM Course WHERE code NOT LIKE 'BBAB%';");
    //Q4
    select_sql_db(faculty_db, "SELECT c.code, c.section, c.semester, c.year, c.title, c.classroom, c.instructor_name, f.department_name, f.office\
FROM Course c\
JOIN Faculty f ON c.instructor_name = f.instructor_name\
WHERE f.department_name = 'CSE';");
    fclose(in);
    sqlite3_close(faculty_db);


    return 0;

}
