#include <iostream>
#include <list>
#include <map>
#include <string>
#include<iomanip>


using namespace std;

class course {
public:
    string name;//CIS554
    int section;//1
    int credits;//3
    string grade;//A-
    course() {}

    course(string n, int s, int c, string g) {
        name = n;
        section = s;
        credits = c;
        grade = g;
    }

    bool operator <(const course &c) const;

    bool operator==(const course &c) const;

    float num_grade();

};

float course::num_grade() {
    map<string, float> M{
            {"A", 4.0f},
            {"A-", 3.667f},
            {"B+", 3.333f},
            {"B", 3.0f},
            {"B-", 2.667f},
            {"C+", 2.333f},
            {"C", 2.0f},
            {"C-", 1.667f},
            {"D", 1.0f},
            {"D-", 0.667f},
            {"F", 0.0f}
    };
    return M[grade];
}

bool course::operator<(const course &c) const {
    return (name < c.name);
}

bool course::operator==(const course &c) const {
    return (name == c.name);
}


/*
* Semester numbers:  Spring 2019: 20191; Fall 2019: 20192, etc.
Implement the following functions.
When adding a student, if the student is already in DB, then ignore the operation.
When adding a course, if the course is already in DB (even if it is in
a different semester), then ignore the operation.
All courses in a semester should be sorted according to name (increasing order)

When dropping a course, if the course does not exist, then ignore the operation.
When removing a student, if the student does not exist, then ignore the operation.
All courses in a semester need to be sorted.
When dropping or adding a course, overall GPA, semester GPA, overall credits and semester credits all need to be updated.
If after drop_course, the list becomes empty, you don't need to remove the list.


*/
//Implement the following functions.
void add_student(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int id);

void remove_student(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int id);

void add_course(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int semester,
                int id, course c); //20171 Spring semester of 2017; 20172: Fall semester of 2017
void
drop_course(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int semester,
            int id, course c);

void print_student_semester_courses(
        map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int semester,
        int id);

void print_student_all_courses(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB,
                          int id);

ostream &operator<<(std::ostream &outputStream,
                    const map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB);
//Implement additional functions such that you can do
//cout << DB << endl;



int main() {

    map<int, pair < pair<int, float> *, map<int, tuple<int, float, list<course*> > * > * >  >  DB;

    add_student(DB, 11111);
    add_student(DB, 11111);
    add_student(DB, 11111);
    course C1("CIS554", 1, 3, "A-"), C2("CSE674", 1, 3, "B+"), C3("MAT296", 8, 4, "A"), C4("WRT205", 5, 3, "A");

    add_course(DB, 20171, 11111, C1);
    add_course(DB, 20171, 11111, C4);
    add_course(DB, 20171, 11111, C3);
    add_course(DB, 20171, 11111, C2);
    print_student_semester_courses(DB, 20171, 11111);

    drop_course(DB, 20171, 11111, C1);
    print_student_semester_courses(DB, 20171, 11111); //sorted according to course name

    course C5("CIS351", 2, 3, "A-"), C6("PSY205", 5, 3, "B+"), C7("MAT331", 2, 3, "A"), C8("ECN203", 4, 3, "A");
    add_course(DB, 20172, 11111, C5);
    add_course(DB, 20172, 11111, C6);
    add_course(DB, 20172, 11111, C7);
    add_course(DB, 20172, 11111, C8);
    add_course(DB, 20172, 11111, C3);
    print_student_all_courses(DB, 11111);//ID GPA

    add_student(DB, 11112);
    add_course(DB, 20171, 11112, C2);
    add_course(DB, 20171, 11112, C5);
    add_course(DB, 20171, 11112, C7);
    add_course(DB, 20171, 11112, C4);
    print_student_semester_courses(DB, 20171, 11112);

    add_course(DB, 20172, 11112, C8);
    add_course(DB, 20172, 11112, C3);
    add_course(DB, 20172, 11112, C5);
    add_course(DB, 20172, 11112, C1);
    print_student_semester_courses(DB, 20172, 11112);

    print_student_all_courses(DB, 11112);

    //Overload operator<< to allow the following cout statements.
    cout << DB << endl;
    remove_student(DB, 11111);
    cout << DB << endl;
    return 0;
}


void add_student(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int id) {

    auto it = DB.find(id);

    if (it != DB.end()) {
        return;
    }


    // Credits,GPA
    pair<int, float> *studentScoreData{new pair<int, float>(0, 0.0f)};

    // Semester Data
    map<int, tuple<int, float, list<course *>> *> *studentSemesterData{
            new map<int, tuple<int, float, list<course *>> *>};


    //Add into DB. Or use DB.insert()
    DB[id] = make_pair(studentScoreData, studentSemesterData);


}


void remove_student(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int id) {

    auto it = DB.find(id);

    if (it == DB.end()) {
        return;
    }

    auto data = DB[id];
    auto &studentOverallDetails = data.first;
    auto &semesterMap = data.second;

    //Delete courses in each semester and then delete
    for (auto &row: *semesterMap) {
        for (auto &i: get<2>(*row.second)) {
            delete i;
        }

        //Delete the tuple
        delete row.second;

    }
    delete semesterMap;

    delete studentOverallDetails;

    DB.erase(id);


}

void add_course(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int semester,
                int id, course c) {

    // Check if the student exists in the database
    auto studentEntry = DB.find(id);
    if (studentEntry == DB.end()) {
        // Student not found
        return;
    }

    //* because its a pointer to map
    auto &studentSemesterMap{*DB[id].second}; //studentSemesterMap is map


    //Check if course already exists in any semester for student. If exists, return
    for (auto semesterDetails: studentSemesterMap) {
        auto courses = get<2>(*semesterDetails.second);
        for (auto course: courses) {
            if (*course == c) {
                return;
            }
        }
    }


    auto it2{studentSemesterMap.find(semester)};

    //If semester doesn't exists, add semester
    if (it2 == studentSemesterMap.end()) {
        studentSemesterMap[semester] = new tuple<int, float, list<course *> >{0, 0.0f, {}};
    }

    //Semester Data currently before adding
    tuple<int, float, list<course *> > &semesterData = *studentSemesterMap[semester];


    auto &coursesInSemester = get<2>(semesterData);

    //UPDATE Courses : Add course to the list in its right place(sort)
    auto it1 = coursesInSemester.begin();
    while(it1!= coursesInSemester.end() && **it1 < c){
        it1++;
    }
    coursesInSemester.insert(it1,new course(c));



    //UPDATE semester GPA,credits and overall GPA,credits
    auto &semesterGPA = get<1>(semesterData);
    auto &overallGPA = DB[id].first->second;
    auto &semesterCredits = get<0>(semesterData);
    auto &overallCredits = DB[id].first->first;

    overallGPA = (overallGPA * overallCredits + c.num_grade() * c.credits) / (overallCredits + c.credits);
    semesterGPA = (semesterGPA * semesterCredits + c.num_grade() * c.credits) / (semesterCredits + c.credits);
    semesterCredits += c.credits;
    overallCredits += c.credits;

}

void
drop_course(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int semester,
            int id, course c) {

    // Check if the student exists in the database
    auto studentEntry = DB.find(id);
    if (studentEntry == DB.end()) {
        // Student not found
        return;
    }

    //* because its a pointer to map
    auto &studentSemesterMap{*DB[id].second}; //studentSemesterMap is map
    auto it1{studentSemesterMap.find(semester)};

    //If semester doesn't exists, return
    if (it1 == studentSemesterMap.end()) {
        return;
    }

    //Semester Data currently before adding
    tuple<int, float, list<course *> > &semesterData = *studentSemesterMap[semester];


    auto &coursesInSemester = get<2>(semesterData);


    //If course doesn't exist in semester, return
    bool found = false;
    for (auto it2 = coursesInSemester.begin(); it2 != coursesInSemester.end(); ++it2) {
        if (**it2 == c) {
            found = true;
        }
    }
    if (found == false) {
        return;
    }

    //Remove the course
    //Tip : List.erase() needs iterator as input
    for (auto it3 = coursesInSemester.begin(); it3 != coursesInSemester.end(); ++it3) {
        if (**it3 == c) {
            //Delete the address it3
            delete *it3;
            coursesInSemester.erase(it3);
            break;
        }
    }


    //UPDATE semester GPA,credits and overall GPA,credits
    auto &semesterGPA = get<1>(semesterData);
    auto &overallGPA = DB[id].first->second;
    auto &semesterCredits = get<0>(semesterData);
    auto &overallCredits = DB[id].first->first;

    if(overallCredits - c.credits == 0){
        overallGPA = 0.0f;
    }
    else {
        overallGPA = (overallGPA * overallCredits - c.num_grade() * c.credits) / (overallCredits - c.credits);
    }

    if(semesterCredits - c.credits == 0){
        semesterGPA = 0.0f;
    }
    else{
        semesterGPA = (semesterGPA * semesterCredits - c.num_grade() * c.credits) / (semesterCredits - c.credits);
    }

    semesterCredits -= c.credits;
    overallCredits -= c.credits;

}

void print_student_semester_courses(
        map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB, int semester,
        int id) {
    // Check if the student exists in the database
    auto studentEntry = DB.find(id);
    if (studentEntry == DB.end()) {
        // Student not found
        return;
    }

    //Check if the semester exists in the database
    auto studentSemesterEntry = DB[id].second->find(semester);
    if (studentSemesterEntry == DB[id].second->end()) {
        // Semester not found
        return;
    }


    //Get the list of courses for the semester
    auto list = get<2>(*studentSemesterEntry->second);

    cout << "ID: " << id << endl;
    cout << "Semester: " << semester << endl;
    cout << "GPA: " << fixed << setprecision(2) << get<1>(*studentSemesterEntry->second) << endl;
    cout << "Credits: " << get<0>(*studentSemesterEntry->second) << endl;
    for (auto course: list) {
        cout << "(" << course->name << " " << course->section << " " << course->credits << " " << course->grade << ") ";
    }

    cout << endl;

}

void
print_student_all_courses(map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB,
                          int id) {
    // Check if the student exists in the database
    auto studentEntry = DB.find(id);
    if (studentEntry == DB.end()) {
        // Student not found
        return;
    }

    auto &overallGPA = DB[id].first->second;
    auto &overallCredits = DB[id].first->first;

    cout << "ID: " << id << endl;
    cout << "Overall GPA: " << overallGPA << endl;
    cout << "Overall Credits: " << overallCredits << endl;

    for (auto &semesterEntry : *DB[id].second) {
        cout << "Semester: " << semesterEntry.first << endl;
        cout << "GPA: " << get<1>(*semesterEntry.second) << endl;
        cout << "Credits: " << get<0>(*semesterEntry.second) << endl;
        for (auto course: get<2>(*semesterEntry.second)) {
            cout << "(" << course->name << " " << course->section << " " << course->credits << " " << course->grade
                 << ") ";
        }
        cout << endl;
    }

    cout << endl;
}


ostream &operator<<(ostream &outputStream,
                    const map<int, pair<pair<int, float> *, map<int, tuple<int, float, list<course *> > *> *> > &DB) {

    cout << "DB: " << endl;
    for (const auto &student: DB) {
        outputStream << "ID: " << student.first << endl;
        outputStream << "Overall GPA: " << student.second.first->second << endl;
        outputStream << "Overall Credits: " << student.second.first->first << endl;

        for (const auto &semester: *student.second.second) {
            outputStream << "Semester: " << semester.first << endl;
            outputStream << "GPA: " << get<1>(*semester.second) << endl;
            outputStream << "Credits: " << get<0>(*semester.second) << endl;

            for (const auto &course: get<2>(*semester.second)) {
                outputStream << "(" << course->name << " " << course->section << " " << course->credits << " "
                             << course->grade << ") ";
            }

            outputStream << endl;
        }

        outputStream << endl;
    }

    return outputStream;
}

