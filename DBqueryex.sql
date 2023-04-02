CREATE TABLE Faculty (
        instructor_name TEXT(100),
        department_name TEXT(100),
        office TEXT(100));

CREATE TABLE Course (\
        code TEXT(20),\
        section INT(10),\
        semester INT(10),\
        year INT(10),\
        title TEXT(40),\
        classroom TEXT(20),\
        instructor_name TEXT(20),
        PRIMARY KEY(code));

INSERT INTO Faculty VALUES (%s,%s,%s);
INSERT INTO Course VALUES (%s,%s,%s,%s,%s,%s);


