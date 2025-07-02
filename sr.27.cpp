#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

class CommunityMember {
protected:
    string name;
    int id;

public:
    CommunityMember(const string& n, int i) : name(n), id(i) {}
    virtual ~CommunityMember() {}

    virtual void display() const {
        cout << "Имя: " << name << ", ID: " << id;
    }

    string getName() const { return name; }
    int getId() const { return id; }
};


class Discipline {
private:
    string name;
    string code;

public:
    Discipline(const string& n, const string& c) : name(n), code(c) {}

    string getName() const { return name; }
    string getCode() const { return code; }

    void display() const {
        cout << "Дисциплина: " << name << " (" << code << ")";
    }
};


class Student : public CommunityMember {
private:
    vector<pair<Discipline, CommunityMember*>> disciplines; // дисциплины и преподаватели

public:
    Student(const string& n, int i) : CommunityMember(n, i) {}

    void addDiscipline(const Discipline& disc, CommunityMember* teacher) {
        disciplines.emplace_back(disc, teacher);
    }

    void display() const override {
        CommunityMember::display();
        cout << "\nТип: Студент\nДисциплины:\n";
        for (const auto& pair : disciplines) {
            pair.first.display();
            cout << ", Преподаватель: " << pair.second->getName() << endl;
        }
    }

    const vector<pair<Discipline, CommunityMember*>>& getDisciplines() const {
        return disciplines;
    }

    void updateTeacherForDiscipline(const Discipline& disc, CommunityMember* newTeacher) {
        for (auto& pair : disciplines) {
            if (pair.first.getName() == disc.getName() && pair.first.getCode() == disc.getCode()) {
                pair.second = newTeacher;
            }
        }
    }
};


class GraduateStudent : public Student {
private:
    CommunityMember* supervisor;

public:
    GraduateStudent(const string& n, int i) : Student(n, i), supervisor(nullptr) {}

    void setSupervisor(CommunityMember* sup) {
        supervisor = sup;
    }

    void display() const override {
        Student::display();
        cout << "Научный руководитель: ";
        if (supervisor) {
            cout << supervisor->getName();
        } else {
            cout << "не назначен";
        }
        cout << endl;
    }
};


class Teacher : public CommunityMember {
private:
    vector<pair<Discipline, vector<Student*>>> teachingGroups; // дисциплины и группы студентов

public:
    Teacher(const string& n, int i) : CommunityMember(n, i) {}

    void addTeachingGroup(const Discipline& disc, const vector<Student*>& group) {
        teachingGroups.emplace_back(disc, group);
    }

    void display() const override {
        CommunityMember::display();
        cout << "\nТип: Преподаватель\nПреподает:\n";
        for (const auto& pair : teachingGroups) {
            pair.first.display();
            cout << "\nСтуденты: ";
            for (const auto* student : pair.second) {
                cout << student->getName() << " ";
            }
            cout << endl;
        }
    }

    void assignGroupToDiscipline(const Discipline& disc, const vector<Student*>& group) {

        teachingGroups.emplace_back(disc, group);

        for (auto* student : group) {
            student->updateTeacherForDiscipline(disc, this);
        }
    }
};


class Researcher : public CommunityMember {
private:
    string researchArea;

public:
    Researcher(const string& n, int i, const string& area)
        : CommunityMember(n, i), researchArea(area) {}

    void display() const override {
        CommunityMember::display();
        cout << "\nТип: Научный работник\nОбласть исследований: " << researchArea << endl;
    }
};

void saveToFile(const string& filename, const vector<CommunityMember*>& members) {
    ofstream out(filename);
    if (!out) {
        cerr << "Не удалось открыть файл для записи" << endl;
        return;
    }

    for (const auto* member : members) {
        if (dynamic_cast<const Student*>(member)) {
            out << "Student " << member->getName() << " " << member->getId() << endl;
        } else if (dynamic_cast<const GraduateStudent*>(member)) {
            out << "GraduateStudent " << member->getName() << " " << member->getId() << endl;
        } else if (dynamic_cast<const Teacher*>(member)) {
            out << "Teacher " << member->getName() << " " << member->getId() << endl;
        } else if (dynamic_cast<const Researcher*>(member)) {
            out << "Researcher " << member->getName() << " " << member->getId() << " "
                << dynamic_cast<const Researcher*>(member)->display() << endl;
        }
    }
    out.close();
}


vector<CommunityMember*> loadFromFile(const string& filename) {
    vector<CommunityMember*> members;
    ifstream in(filename);
    if (!in) {
        cerr << "Не удалось открыть файл для чтения" << endl;
        return members;
    }

    string type, name;
    int id;
    while (in >> type >> name >> id) {
        if (type == "Student") {
            members.push_back(new Student(name, id));
        } else if (type == "GraduateStudent") {
            members.push_back(new GraduateStudent(name, id));
        } else if (type == "Teacher") {
            members.push_back(new Teacher(name, id));
        } else if (type == "Researcher") {
            string area;
            getline(in, area);
            members.push_back(new Researcher(name, id, area));
        }
    }
    in.close();
    return members;
}

int main() {
    Discipline math("Математика", "MATH101");
    Discipline physics("Физика", "PHYS201");
    Discipline programming("Программирование", "PROG301");

    Student student1("Иван Иванов", 1001);
    Student student2("Петр Петров", 1002);
    GraduateStudent gradStudent("Алексей Сидоров", 2001);
    Teacher teacher("Дмитрий Николаев", 3001);
    Researcher researcher("Сергей Павлов", 4001, "Квантовая физика");


    student1.addDiscipline(math, &teacher);
    student1.addDiscipline(physics, &teacher);
    student2.addDiscipline(programming, &teacher);
    gradStudent.addDiscipline(math, &teacher);
    gradStudent.addDiscipline(physics, &researcher);


    gradStudent.setSupervisor(&researcher);


    vector<Student*> group1 = {&student1, &gradStudent};
    vector<Student*> group2 = {&student2};


    teacher.assignGroupToDiscipline(math, group1);
    teacher.assignGroupToDiscipline(programming, group2);


    vector<CommunityMember*> members = {&student1, &student2, &gradStudent, &teacher, &researcher};

    cout << "=== Университетское сообщество ===\n";
    for (const auto* member : members) {
        member->display();
        cout << "----------------------------\n";
    }


    saveToFile("university.txt", members);
    auto loadedMembers = loadFromFile("university.txt");

    cout << "\n=== Данные, загруженные из файла ===\n";
    for (const auto* member : loadedMembers) {
        member->display();
        cout << "----------------------------\n";
    }


    for (auto* member : loadedMembers) {
        delete member;
    }

    return 0;
}
