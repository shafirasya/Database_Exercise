#include <iostream>
#include <string>
#include <fstream> // for saving to files
#include <sstream> // to parse from string to object to be splitted/parsed

using namespace std; // for easier accepting inputs/outputs

string stringify(string name, string number, string major)
{
    return name + " " + number + " " + major + "\n";
}

// Declare class for objects [not a function, don't use ()] !!
class Student
{

public:
    string name;
    string number;
    string major;

    Student(string name, string number, string major) : name(name), number(number), major(major) {}

    string stringifyStudent()
    {
        return stringify(name, number, major);
    }
};

// Save in BE, specify first where to store
class DBase
{

public:
    ifstream in;     // input
    ofstream out;    // output
    string fileName; // file name

    // why const char* ? prob so we can remember the address
    DBase(const char *fileName) : fileName(fileName) {}

    // helper functions
    void replace(const string &newFileName)
    {
        remove(DBase::fileName.c_str());                      // remove file with the input's filename (must use character array)
        rename(newFileName.c_str(), DBase::fileName.c_str()); // rename temporary file to our DBase's filename (must be in character array)
    }

    bool extractLine(const string &line, string &name, string &number, string &major, string inputName = "")
    {

        // Create stringstream object that creates object with information based on string w/ its delimiter (default = " " aka space)
        stringstream received(line);
        received >> name >> number >> major;

        return name == inputName;
    }

    // storing function
    void save(Student data)
    {
        cout << "Student's name is " << data.name << endl;

        // 1. Open/Create file to store the object based on the filename
        DBase::out.open(DBase::fileName, ios::app);

        // 2. Save data into file by converting from object to acceptable type such as STRING
        DBase::out << data.stringifyStudent();

        // Whenever we save, the out function will still keep the previous records

        // 3. Close the file
        DBase::out.close();
    }

    void showAll()
    {
        // For inputting to the program

        // 1. Open the file
        DBase::in.open(DBase::fileName, ios::in);

        // 2. Retrieve the data from the file
        string line;
        string name, number, major;
        int index = 1;

        // Retrieve all until end of file
        while (getline(DBase::in, line))
        {
            DBase::extractLine(line, name, number, major);

            cout << index++ << ".\t";
            cout << name << "\t" << number << "\t" << major << endl;
        }

        // 3. Close the file
        DBase::in.close();
    }

    void retrieve(string inputName)
    {

        // 1. Open the file
        DBase::in.open(DBase::fileName, ios::in);

        // 2. Retrieve the data from the file
        string line;
        string name, number, major;
        bool found = false;

        while (getline(DBase::in, line) and !found)
        {
            found = DBase::extractLine(line, name, number, major, inputName);
        }

        // 3. Close the file
        DBase::in.close();

        // 4. Show Output

        if (found)
        {
            cout << "Student is found! Details: \n"
                 << name << "\t" << number << "\t" << major << endl;
        }
        else
        {
            cout << "Student is not found!" << endl;
        }
    }

    void deleteData(string inputName)
    {

        // 1. Open file to the program
        DBase::in.open(DBase::fileName, ios::in);

        // 2. Create temporary file to store all records except the one to be deleted
        ofstream temp;
        temp.open("NewRecord.txt", ios::app);

        // 3. Iterate through all records in the DBase's file and keep track if specified record is found or not
        string line;                // each string in the database file
        string name, number, major; // to store record
        bool found = false;

        while (getline(DBase::in, line)) // getline(ifstream, line) -> to retrieve each line in the database file
        {
            if (DBase::extractLine(line, name, number, major, inputName))
            {
                found = true;
                continue;
            }

            temp << stringify(name, number, major);
        }

        // 4. Close both files
        DBase::in.close();
        temp.close();

        // 5. Checking entry, remove & rename if needed
        if (found)
        {
            DBase::replace("NewRecord.txt");
            cout << "Student is deleted!" << endl;
        }
        else
        {
            remove("NewRecord.txt");
            cout << "Student not found! No change in record!" << endl;
        }
    }

    void modify(string inputName)
    {

        // 1. Open file to the program
        DBase::in.open(DBase::fileName, ios::in);

        // 2. Create temporary file to store all records except the one to be deleted
        ofstream temp;
        temp.open("NewRecord.txt", ios::app);

        // 3. Iterate through all records in the DBase's file and keep track if specified record is found or not
        string line;                // each string in the database file
        string name, number, major; // to store record
        bool found = false;

        while (getline(DBase::in, line)) // getline(ifstream, line) -> to retrieve each line in the database file
        {
            if (DBase::extractLine(line, name, number, major, inputName))
            { // if record/entry found, modify the data by asking for user input (so we will include the updated one on the new file)
                found = true;
                cout << "Student found! Please enter new student's information: " << endl;

                // Take user's input one by one
                cout << "Enter new student's name: ";
                cin >> name;
                cout << "Enter new student's number: ";
                cin >> number;
                cout << "Enter new student's major: ";
                cin >> major;
            }
            temp << stringify(name, number, major);
        }

        // 4. Close both files
        DBase::in.close();
        temp.close();

        // 5. Checking entry, remove & rename if needed
        if (found)
        {
            DBase::replace("NewRecord.txt");
            cout << "Student is modified!" << endl;
        }
        else
        {
            remove("NewRecord.txt");
            cout << "Student not found! No change in record!" << endl;
        }
    }
};

void insertion(DBase &database)
{

    string name, number, major;

    cout << "Enter student's name: ";
    cin >> name;
    cout << "Enter student's number: ";
    cin >> number;
    cout << "Enter student's major: ";
    cin >> major;

    Student newStudent(name, number, major);
    database.save(newStudent);
}

int main()
{
    // Initialize database
    DBase A_db("data.txt");

    cout << "Hello Guys! Welcome to our simple student database." << endl;
    char menu = 'Y';

    while (menu == 'Y' or menu == 'y')
    {

        int feature;
        cout << "\nOur features: \n"
             << "1. Insertion \n2. Retrieval \n3. Deletion \n4. Modification \n5. Show All \n"
             << "\nInsert feature's number: " << endl;
        cin >> feature;

        if (feature == 1)
        { // Insertion
            insertion(A_db);
        }
        else if (feature == 5)
        { // Show All
            cout << "\nAll students information: " << endl;
            A_db.showAll();
        }
        else
        {
            string inputName;
            cout << "\nPlease enter student's name: " << endl;
            cin >> inputName;

            switch (feature)
            {
            case 2: // Retrieval
                A_db.retrieve(inputName);
                break;
            case 3: // Deletion
                A_db.deleteData(inputName);
                break;
            case 4: // Modification
                A_db.modify(inputName);
                break;
            default:
                cout << "\nOperation invalid!" << endl;
                break;
            };
        }

        cout << "\nOperation is done! Enter Y or y to continue explore our database. To exit, simply click any other key!" << endl;
        cin >> menu;
    }

    return 0;
}