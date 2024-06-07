#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>

using namespace std;
class person //abtract class
{
protected:
    string first_name;
    string last_name;
    char gender;
    int age;
    string phone;
    string address;
public:
    person() : first_name(""), last_name(""), gender('M'), age(0), phone(""), address("") {}
    person(const string& first, const string& last, const char& gen, int ag, const string& ph, const string& addr) :
        first_name(first), last_name(last), gender(gen), age(ag), phone(ph), address(addr) {}

    virtual void display() const = 0; /// pure virtual function
    /// Handle error input /////
    bool isValidName(const string& name) {
        if (name.empty() || !(name[0] >= 'A' && name[0] <= 'Z')) {
            return false;
        }
        for (char ch : name) {
            if (!((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z'))) {
                return false;
            }
        }
        return true;
    }

    bool isValidNumber(const string& input) {
        if (input.empty()) {
            return false;
        }
        for (char ch : input) {
            if (!(ch >= '0' && ch <= '9')) {
                return false;
            }
        }
        return true;
    }

    bool isValidPhoneNumber(const string& phone) {
        if (phone.length() != 10) {
            return false;
        }
        for (char ch : phone) {
            if (!(ch >= '0' && ch <= '9')) {
                return false;
            }
        }
        return true;
    }

    bool isValidAddress(const string& address) {
        if (address.empty()) {
            return false;
        }
        bool foundDigit = false;
        bool foundSpace = false;
        for (char ch : address) {
            if (ch >= '0' && ch <= '9') {
                foundDigit = true;
            }
            else if (ch == ' ' && foundDigit) {
                foundSpace = true;
            }
        }
        return foundDigit && foundSpace;
    }
    bool containsDigit(const string& str) {
        for (char ch : str) {
            if (ch >= '0' && ch <= '9') {
                return true;
            }
        }
        return false;
    }
};
class Account {
private:
    string user;
    int password;
public:
    string type;
    int id;
    Account* next;
    Account(string user, int password, int id, string type) : user(user), password(password), id(id), type(type), next(nullptr) {}
    static int checkLogin(Account* listAccount, const string& username, int password, const string& type, int& id) {
        while (listAccount != nullptr) {
            if (listAccount->type == type && listAccount->user == username && listAccount->password == password) {
                id = listAccount->id;
                return 1;
            }
            listAccount = listAccount->next;
        }
        return 0;
    }

    static void getDataFromFileAccount(Account* head) {
        ifstream file("account.csv");
        if (file.is_open()) {
            string line;
            bool firstLine = true;
            while (getline(file, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string tempString;
                int password, id;
                string user, type;
                getline(ss, user, ',');
                getline(ss, tempString, ',');
                password = stoi(tempString);
                getline(ss, tempString, ',');
                id = stoi(tempString);
                getline(ss, type, ',');

                Account* newAccount = new Account(user, password, id, type);
                Account* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newAccount;
            }
            file.close();
        }
        else {
            cout << "Unable to open file: account.csv" << endl;
        }
    }

    static void addNewAccount(Account* head, const string& username, int password, int id, const string& type) {
        Account* newAccount = new Account(username, password, id, type);
        newAccount->next = head->next;
        head->next = newAccount;
    }
    static void createAccount(string name, int id, string type, Account* head) {
        string user = name + to_string(id);
        Account* newAccount = new Account(user, 1, id, type);
        Account* tmp = head;
        if (head == nullptr) {
            head = newAccount;
            newAccount->next = nullptr;
        }
        else {
            while (tmp->next != nullptr)
            {
                tmp = tmp->next;
            }
            tmp->next = newAccount;
        }
    }
    static void printlistAccount(Account* head) {
        Account* temp = head;
        cout << left << setw(15) << "User"
            << setw(15) << "Password"
            << setw(15) << "ID"
            << setw(15) << "Type" << endl;
        while (temp != nullptr) {

            cout << left << setw(15) << temp->user
                << setw(15) << temp->password
                << setw(15) << temp->id
                << setw(15) << temp->type << endl;
            temp = temp->next;
        }
    }
    static void updatefileaccount(Account* head) {
        ofstream file("account.csv", ios::trunc);
        if (file.is_open()) {
            file << "User,Password,ID,Type\n";

            Account* tmp = head->next;
            while (tmp != nullptr) {
                file << tmp->user << ","
                    << tmp->password << ","
                    << tmp->id << ","
                    << tmp->type << "," << "\n";
                tmp = tmp->next;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file account.csv";
        }
    }
};

class patient : public person {
private:
    int height;
    int weight;
public:
    patient* next;
    int patient_id;

    patient() : person(), height(0), weight(0), next(nullptr), patient_id(0) {}
    patient(int id, const string& first, const string& last, const char& gen, int ag, const string& ph, const string& addr, int ht, int wt) :
        person(first, last, gen, ag, ph, addr), height(ht), weight(wt), next(nullptr), patient_id(id) {}

    void display() const override {
        cout << left << setw(5) << patient_id
            << setw(15) << first_name
            << setw(15) << last_name
            << setw(10) << gender
            << setw(10) << age
            << setw(15) << phone
            << setw(30) << address
            << setw(10) << height
            << weight << endl;
    }

    /*Add patient*/
    int generatePatientID(patient* head) {
        int max_id = 0;
        patient* temp = head;
        while (temp != nullptr) {
            if (temp->patient_id > max_id) {
                max_id = temp->patient_id;
            }
            temp = temp->next;
        }
        return max_id + 1;
    }
    void addpatient(patient* head, Account* head1) {
        patient* temp = head;
        patient* newPatient = new patient();
        newPatient->patient_id = generatePatientID(head);

        cout << "Please enter information of patient below\n\n";

        // Input and validation for first name
        do {
            cout << "First name: ";
            getline(cin >> ws, newPatient->first_name);
            if (!isValidName(newPatient->first_name)) {
                cout << "Invalid input. The first name must start with an uppercase letter and contain no digits. Please try again.\n";
            }
        } while (!isValidName(newPatient->first_name));

        // Input and validation for last name
        do {
            cout << "\nLast name: ";
            getline(cin, newPatient->last_name);
            if (!isValidName(newPatient->last_name)) {
                cout << "Invalid input. The last name must start with an uppercase letter and contain no digits. Please try again.\n";
            }
        } while (!isValidName(newPatient->last_name));

        // Input and validation for age
        string ageInput;
        do {
            cout << "\nEnter age: ";
            cin >> ageInput;
            if (!isValidNumber(ageInput) || stoi(ageInput) <= 0) { /// age must not be 0
                cout << "Invalid input. Age must be a positive number. Please enter again: ";
            }
        } while (!isValidNumber(ageInput) || stoi(ageInput) <= 0);
        newPatient->age = stoi(ageInput);

        // Input and validation for gender
        cout << "\nGender (M = Male || F = Female): ";
        cin >> newPatient->gender;
        while (newPatient->gender != 'M' && newPatient->gender != 'F') {
            cout << "Invalid input. Please enter 'M' for Male or 'F' for Female: ";
            cin >> newPatient->gender;
        }

        // Input and validation for mobile number
        do {
            cout << "\nEnter mobile number: ";
            getline(cin >> ws, newPatient->phone);
            if (!isValidPhoneNumber(newPatient->phone)) {
                cout << "Invalid phone number. Please enter a 10-digit number without any letters or special characters.\n";
            }
        } while (!isValidPhoneNumber(newPatient->phone));

        // Input and validation for address: house number
        string houseNumber;
        do {
            cout << "\nEnter house number: ";
            getline(cin >> ws, houseNumber);
            if (!isValidNumber(houseNumber)) {
                cout << "Invalid input. House number must be a number. Please enter again: ";
            }
        } while (!isValidNumber(houseNumber));

        // Input and validation for address: street name
        string streetName;
        do {
            cout << "\nEnter street name: ";
            getline(cin >> ws, streetName);
            if (containsDigit(streetName)) {
                cout << "Invalid input. Street name must not contain numbers. Please enter again: ";
            }
        } while (containsDigit(streetName));

        // Input and validation for address: city
        string cityName;
        do {
            cout << "\nEnter city name: ";
            getline(cin >> ws, cityName);
            if (containsDigit(cityName)) {
                cout << "Invalid input. City name must not contain numbers. Please enter again: ";
            }
        } while (containsDigit(cityName));

        // Concatenate address parts
        newPatient->address = houseNumber + " " + streetName + " " + cityName;

        // Input and validation for height
        string heightInput;
        do {
            cout << "\nEnter the height of the patient (in cm): ";
            cin >> heightInput;
            if (!isValidNumber(heightInput) || stoi(heightInput) <= 0) {
                cout << "Invalid input. Please enter a valid height in cm: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!isValidNumber(heightInput) || stoi(heightInput) <= 0);
        newPatient->height = stoi(heightInput);

        // Input and validation for weight
        string weightInput;
        do {
            cout << "\nEnter the weight of the patient (in kg): ";
            cin >> weightInput;
            if (!isValidNumber(weightInput) || stoi(weightInput) <= 0) {
                cout << "Invalid input. Please enter a valid weight in kg: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
        } while (!isValidNumber(weightInput) || stoi(weightInput) <= 0);
        newPatient->weight = stoi(weightInput);

        cout << "\nRegister new patient successfully!\n";

        if (head == nullptr) {
            head = newPatient;
            newPatient->next = nullptr;
        }
        else {
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newPatient;
            newPatient->next = nullptr;
        }

        Account::createAccount(newPatient->first_name, newPatient->patient_id, "1", head1);
    }

    void updatepatient(int id, patient* head) {
        patient* tmp = head;
        string ageInput, houseNumber, streetName, cityName, heightInput, weightInput;
        while (tmp != nullptr && tmp->patient_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Patient with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << setw(10) << "Height"
            << "Weight" << endl;
        printpatientdetail(id, head);
        bool exit = false;
        while (!exit) {
            int purpose = 0;
            cout << "\nSelect options you want to fix:\n";
            cout << "[01] : First Name\n";
            cout << "[02] : Last Name\n";
            cout << "[03] : Gender\n";
            cout << "[04] : Age\n";
            cout << "[05] : Phone\n";
            cout << "[06] : Address\n";
            cout << "[07] : Height\n";
            cout << "[08] : Weight\n";
            cout << "[00] : Save\n";
            cout << "\nEnter your choice: ";
            cin >> purpose;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (purpose) {
            case 1:
                do {
                    cout << "Enter new patient's first name: ";
                    getline(cin >> ws, tmp->first_name);
                    if (!isValidName(tmp->first_name)) {
                        cout << "Invalid input. The first name must start with an uppercase letter and contain no digits. Please try again.\n";
                    }
                } while (!isValidName(tmp->first_name));
                break;
            case 2:
                do {
                    cout << "\nEnter new patient's last name: ";
                    getline(cin, tmp->last_name);
                    if (!isValidName(tmp->last_name)) {
                        cout << "Invalid input. The last name must start with an uppercase letter and contain no digits. Please try again.\n";
                    }
                } while (!isValidName(tmp->last_name));
                break;
            case 3:
                /*cout << "Enter new patient's gender: ";
                cin >> tmp->gender;*/
                cout << "\nEnter new patient's gender (M = Male || F = Female): ";
                cin >> tmp->gender;
                while (tmp->gender != 'M' && tmp->gender != 'F') {
                    cout << "Invalid input. Please enter 'M' for Male or 'F' for Female: ";
                    cin >> tmp->gender;
                }
                break;
            case 4:
                do {
                    cout << "\nEnter new patient's age: ";
                    cin >> ageInput;
                    if (!isValidNumber(ageInput) || stoi(ageInput) <= 0) {
                        cout << "Invalid input. Age must be a positive number. Please enter again: ";
                    }
                } while (!isValidNumber(ageInput) || stoi(ageInput) <= 0);
                tmp->age = stoi(ageInput);
                break;
            case 5:
                do {
                    cout << "\nEnter new patient's mobile number: ";
                    getline(cin >> ws, tmp->phone);
                    if (!isValidPhoneNumber(tmp->phone)) {
                        cout << "Invalid phone number. Please enter a 10-digit number without any letters or special characters.\n";
                    }
                } while (!isValidPhoneNumber(tmp->phone));
                break;
            case 6:

                do {
                    cout << "\nEnter new patient's house number: ";
                    getline(cin >> ws, houseNumber);
                    if (!isValidNumber(houseNumber)) {
                        cout << "Invalid input. House number must be a number. Please enter again: ";
                    }
                } while (!isValidNumber(houseNumber));

                // Input and validation for address: street name

                do {
                    cout << "\nEnter new patient's street name: ";
                    getline(cin >> ws, streetName);
                    if (containsDigit(streetName)) {
                        cout << "Invalid input. Street name must not contain numbers. Please enter again: ";
                    }
                } while (containsDigit(streetName));

                // Input and validation for address: city

                do {
                    cout << "\nEnter new patient's city name: ";
                    getline(cin >> ws, cityName);
                    if (containsDigit(cityName)) {
                        cout << "Invalid input. City name must not contain numbers. Please enter again: ";
                    }
                } while (containsDigit(cityName));

                // Concatenate address parts
                tmp->address = houseNumber + " " + streetName + " " + cityName;
                break;
            case 7:

                do {
                    cout << "\nEnter new patient's height (in cm): ";
                    cin >> heightInput;
                    if (!isValidNumber(heightInput) || stoi(heightInput) <= 0) {
                        cout << "Invalid input. Please enter a valid height in cm: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (!isValidNumber(heightInput) || stoi(heightInput) <= 0);
                tmp->height = stoi(heightInput);
                break;
            case 8:
                do {
                    cout << "\nEnter new patient's weight (in kg): ";
                    cin >> weightInput;
                    if (!isValidNumber(weightInput) || stoi(weightInput) <= 0) {
                        cout << "Invalid input. Please enter a valid weight in kg: ";
                        cin.clear();
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                    }
                } while (!isValidNumber(weightInput) || stoi(weightInput) <= 0);
                tmp->weight = stoi(weightInput);
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << "New information of patient:\n";
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << setw(10) << "Height"
            << "Weight" << endl;
        printpatientdetail(id, head);

    };
    static void printpatientdetail(int id, patient* head) {
        patient* tmp = head;
        while (tmp != nullptr && tmp->patient_id != id) {
            tmp = tmp->next;
        }
        tmp->display();
    };
    void searchpatient(patient* head) {
        int id;
        cout << "Enter patient ID to search: ";
        cin >> id;
        patient* tmp = head;

        while (tmp != nullptr && tmp->patient_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "\nPatient with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(5) << "\nID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << setw(10) << "Height"
            << "Weight" << endl;
        printpatientdetail(id, head);
    };
    static void getDataFromFilePatient(patient* head) {
        ifstream file("patient.csv");
        if (file.is_open()) {
            string line;
            bool firstLine = true;
            while (getline(file, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string tempString;
                int patient_id, age;
                char gender;
                string first_name, last_name, phone, address;
                float height, weight;
                getline(ss, tempString, ',');
                patient_id = stoi(tempString);
                getline(ss, first_name, ',');
                getline(ss, last_name, ',');
                getline(ss, tempString, ',');
                gender = tempString[0];
                getline(ss, tempString, ',');
                age = stoi(tempString);
                getline(ss, phone, ',');
                getline(ss, address, ',');
                getline(ss, tempString, ',');
                height = stoi(tempString);
                getline(ss, tempString, ',');
                weight = stoi(tempString);
                patient* newPatient = new patient(patient_id, first_name, last_name, gender, age, phone, address, height, weight);

                patient* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newPatient;
            }
            file.close();
        }
        else {
            cout << "Unable to open file: patient.csv" << endl;
        }
    }
    void updatefilepatient(patient* head) {
        ofstream file("patient.csv", ios::trunc);
        if (file.is_open()) {
            file << "ID,First Name,Last Name,Gender,Age,Phone,Address,Height,Weight\n";

            patient* tmp = head->next;
            while (tmp != nullptr) {
                file << tmp->patient_id << ","
                    << tmp->first_name << ","
                    << tmp->last_name << ","
                    << tmp->gender << ","
                    << tmp->age << ","
                    << tmp->phone << ","
                    << tmp->address << ","
                    << tmp->height << ","
                    << tmp->weight << "\n";
                tmp = tmp->next;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file: patient.csv" << endl;
        }
    }
};
class doctor : public person {
private:
public:
    string type;
    doctor* next;
    int doctor_id;

    doctor() : person(), type(""), next(nullptr), doctor_id(0) {}
    doctor(int id, const string& first, const string& last, const char& gen, int ag, const string& ph, const string& addr, const string& typ) :
        person(first, last, gen, ag, ph, addr), type(typ), next(nullptr), doctor_id(id) {}

    void display() const override {
        cout << left << setw(5) << doctor_id
            << setw(15) << first_name
            << setw(15) << last_name
            << setw(10) << gender
            << setw(10) << age
            << setw(15) << phone
            << setw(30) << address
            << type << endl;
    }

    /* Create unique ID for new Doctor */
    int generateDoctorID(doctor* head) {
        int max_id = 0;
        doctor* temp = head;
        while (temp != nullptr) {
            if (temp->doctor_id > max_id) {
                max_id = temp->doctor_id;
            }
            temp = temp->next;
        }
        return max_id + 1;
    }
    void adddoctor(doctor* head, Account* head1) {
        doctor* temp = head;
        doctor* newDoctor = new doctor();
        newDoctor->doctor_id = generateDoctorID(head);
        cout << "Please enter information of patient below\n\n";
        // Input and validation for first name
        do {
            cout << "First name: ";
            getline(cin >> ws, newDoctor->first_name);
            if (!isValidName(newDoctor->first_name)) {
                cout << "Invalid input. The first name must start with an uppercase letter and contain only alphabetic characters. Please try again.\n";
            }
        } while (!isValidName(newDoctor->first_name));

        // Input and validation for last name
        do {
            cout << "\nLast name: ";
            getline(cin, newDoctor->last_name);
            if (!isValidName(newDoctor->last_name)) {
                cout << "Invalid input. The last name must start with an uppercase letter and contain only alphabetic characters. Please try again.\n";
            }
        } while (!isValidName(newDoctor->last_name));

        // Input and validation for age
        string ageInput;
        do {
            cout << "\nEnter age (must be between 18 and 65): ";
            cin >> ageInput;
            if (!isValidNumber(ageInput) || stoi(ageInput) < 18 || stoi(ageInput) > 65) {
                cout << "Invalid input. Age must be a number between 18 and 65. Please enter again: ";
            }
        } while (!isValidNumber(ageInput) || stoi(ageInput) < 18 || stoi(ageInput) > 65);
        newDoctor->age = stoi(ageInput);

        // Input and validation for gender
        do {
            cout << "\nGender (M = Male || F = Female): ";
            cin >> newDoctor->gender;
            if (newDoctor->gender != 'M' && newDoctor->gender != 'F') {
                cout << "Invalid input. Please enter 'M' for Male or 'F' for Female: ";
            }
        } while (newDoctor->gender != 'M' && newDoctor->gender != 'F');


        // Input and validation for mobile number
        do {
            cout << "\nEnter mobile phone number : ";
            getline(cin >> ws, newDoctor->phone);
            if (!isValidPhoneNumber(newDoctor->phone)) {
                cout << "Invalid phone number. Please enter a 10-digit number without any letters or special characters.\n";
            }
        } while (!isValidPhoneNumber(newDoctor->phone));

        // Input and validation for address: house number
        string houseNumber;
        do {
            cout << "\nEnter house number: ";
            getline(cin >> ws, houseNumber);
            if (!isValidNumber(houseNumber)) {
                cout << "Invalid input. House number must be a number. Please enter again: ";
            }
        } while (!isValidNumber(houseNumber));

        // Input and validation for address: street name
        string streetName;
        do {
            cout << "\nEnter street name: ";
            getline(cin >> ws, streetName);
            if (containsDigit(streetName)) {
                cout << "Invalid input. Street name must not contain numbers. Please enter again: ";
            }
        } while (containsDigit(streetName));

        // Input and validation for address: city
        string cityName;
        do {
            cout << "\nEnter city name: ";
            getline(cin >> ws, cityName);
            if (containsDigit(cityName)) {
                cout << "Invalid input. City name must not contain numbers. Please enter again: ";
            }
        } while (containsDigit(cityName));

        // Concatenate address parts
        newDoctor->address = houseNumber + " " + streetName + " " + cityName;

        // Input and validation for type
        do {
            cout << "Type: ";
            getline(cin >> ws, newDoctor->type);
            if (!isValidName(newDoctor->type)) {
                cout << "Invalid input. The type must contain only alphabetic characters. Please try again.\n";
            }
        } while (!isValidName(newDoctor->type));

        cout << "Register new doctor successfully!\n";
        if (head == nullptr) {
            head = newDoctor;
            newDoctor->next = nullptr;
        }
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newDoctor;
        Account::createAccount(newDoctor->first_name, newDoctor->doctor_id, "2", head1);

    }

    void searchdoctor(doctor* head) {
        int id;
        cout << "Enter doctor ID to search: ";
        cin >> id;
        doctor* tmp = head;
        while (tmp != nullptr && tmp->doctor_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "\nDoctor with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(5) << "\nID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << "Type" << endl;
        printdoctordetail(id, head);
    }
    /*Update doctor information*/
    void updatedoctor(int id, doctor* head) {
        doctor* tmp = head;
        string ageInput, houseNumber, streetName, cityName;
        while (tmp != nullptr && tmp->doctor_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Doctor with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << "Type" << endl;
        printdoctordetail(id, head);
        bool exit = false;
        while (!exit) {
            int purpose = 0;
            cout << "\nSelect options you want to fix:\n";
            cout << "[01] : First Name\n";
            cout << "[02] : Last Name\n";
            cout << "[03] : Gender\n";
            cout << "[04] : Age\n";
            cout << "[05] : Phone\n";
            cout << "[06] : Address\n";
            cout << "[07] : Type\n";
            cout << "[00] : Save\n";
            cout << "\nEnter your choice: ";
            cin >> purpose;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            switch (purpose) {
            case 1:
                do {
                    cout << "Enter new doctor's first name: ";
                    getline(cin >> ws, tmp->first_name);
                    if (!isValidName(tmp->first_name)) {
                        cout << "Invalid input. The first name must start with an uppercase letter and contain only alphabetic characters. Please try again.\n";
                    }
                } while (!isValidName(tmp->first_name));
                break;
            case 2:
                do {
                    cout << "\nEnter new doctor's last name: ";
                    getline(cin, tmp->last_name);
                    if (!isValidName(tmp->last_name)) {
                        cout << "Invalid input. The last name must start with an uppercase letter and contain only alphabetic characters. Please try again.\n";
                    }
                } while (!isValidName(tmp->last_name));
                break;
            case 3:
                do {
                    cout << "\nGender (M = Male || F = Female): ";
                    cin >> tmp->gender;
                    if (tmp->gender != 'M' && tmp->gender != 'F') {
                        cout << "Invalid input. Please enter 'M' for Male or 'F' for Female: ";
                    }
                } while (tmp->gender != 'M' && tmp->gender != 'F');
                break;
            case 4:
                do {
                    cout << "\nEnter new doctor's age (must be between 18 and 65): ";
                    cin >> ageInput;
                    if (!isValidNumber(ageInput) || stoi(ageInput) < 18 || stoi(ageInput) > 65) {
                        cout << "Invalid input. Age must be a number between 18 and 65. Please enter again: ";
                    }
                } while (!isValidNumber(ageInput) || stoi(ageInput) < 18 || stoi(ageInput) > 65);
                tmp->age = stoi(ageInput);
                break;
            case 5:
                do {
                    cout << "\nEnter new doctor's mobile phone number : ";
                    getline(cin >> ws, tmp->phone);
                    if (!isValidPhoneNumber(tmp->phone)) {
                        cout << "Invalid phone number. Please enter a 10-digit number without any letters or special characters.\n";
                    }
                } while (!isValidPhoneNumber(tmp->phone));
                break;
            case 6:
                do {
                    cout << "\nEnter new doctor's house number: ";
                    getline(cin >> ws, houseNumber);
                    if (!isValidNumber(houseNumber)) {
                        cout << "Invalid input. House number must be a number. Please enter again: ";
                    }
                } while (!isValidNumber(houseNumber));

                // Input and validation for address: street name
                do {
                    cout << "\nEnter new doctor's street name: ";
                    getline(cin >> ws, streetName);
                    if (containsDigit(streetName)) {
                        cout << "Invalid input. Street name must not contain numbers. Please enter again: ";
                    }
                } while (containsDigit(streetName));

                // Input and validation for address: city
                do {
                    cout << "\nEnter new doctor's city name: ";
                    getline(cin >> ws, cityName);
                    if (containsDigit(cityName)) {
                        cout << "Invalid input. City name must not contain numbers. Please enter again: ";
                    }
                } while (containsDigit(cityName));

                // Concatenate address parts
                tmp->address = houseNumber + " " + streetName + " " + cityName;
                break;
            case 7:
                do {
                    cout << "Type: ";
                    getline(cin >> ws, tmp->type);
                    if (!isValidName(tmp->type)) {
                        cout << "Invalid input. The type must contain only alphabetic characters. Please try again.\n";
                    }
                } while (!isValidName(tmp->type));
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << "New information of doctor:\n";
        printdoctordetail(id, head);
    }

    static void printdoctordetail(int id, doctor* head) {
        doctor* tmp = head;
        while (tmp != nullptr && tmp->doctor_id != id) {
            tmp = tmp->next;
        }
        tmp->display();
    }
    void updatefiledoctor(doctor* head) {
        ofstream file("doctor.csv", ios::trunc);
        if (file.is_open()) {
            file << "ID,First Name,Last Name,Gender,Age,Phone,Address,Type\n";

            doctor* tmp = head->next;
            while (tmp != nullptr) {
                file << tmp->doctor_id << ","
                    << tmp->first_name << ","
                    << tmp->last_name << ","
                    << tmp->gender << ","
                    << tmp->age << ","
                    << tmp->phone << ","
                    << tmp->address << ","
                    << tmp->type << "\n";
                tmp = tmp->next;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file";
        }
    }


    static void getDataFromFileDoctor(doctor* head) {
        ifstream file("doctor.csv");
        if (file.is_open()) {
            string line;
            bool firstLine = true;
            while (getline(file, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string tempString;
                int doctor_id, age;
                char gender;
                string first_name, last_name, phone, address, type;

                getline(ss, tempString, ',');
                doctor_id = stoi(tempString);
                getline(ss, first_name, ',');
                getline(ss, last_name, ',');
                getline(ss, tempString, ',');
                gender = tempString[0];
                getline(ss, tempString, ',');
                age = stoi(tempString);
                getline(ss, phone, ',');
                getline(ss, address, ',');
                getline(ss, type, ',');

                doctor* newDoctor = new doctor(doctor_id, first_name, last_name, gender, age, phone, address, type);

                doctor* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newDoctor;
            }
            file.close();
        }
        else {
            cout << "Unable to open file: doctor.csv" << endl;
        }
    }

};
class appointment {
private:
    string date;
    string status;
    string location;
public:
    int doctor_id;
    int patient_id;
    appointment* next;
    int appointment_id;
    appointment() : next(nullptr) {}

    appointment(int a_id, int d_id, int p_id, const string& date, const string& status, const string& location) :
        appointment_id(a_id), doctor_id(d_id), patient_id(p_id), date(date), status(status), location(location), next(nullptr) {}
    static int generateAppointmentID(appointment* head) {
        int max_id = 0;
        appointment* temp = head;
        while (temp != nullptr) {
            if (temp->appointment_id > max_id) {
                max_id = temp->appointment_id;
            }
            temp = temp->next;
        }
        return max_id + 1;
    }
    static void addapointment(appointment* newapp, appointment* head)
    {
        if (head == nullptr)
        {
            head = newapp;
        }
        else
        {
            appointment* temp = head;
            while (temp->next != nullptr)
            {
                temp = temp->next;
            }
            temp->next = newapp;
        }
    };

    void removeappointment(appointment* head) {
        int id;
        char confirm;
        cout << "Enter ID: ";
        cin >> id;
        if (head == nullptr) return;
        appointment* tmp = head;
        appointment* prev = nullptr;

        while (tmp != nullptr && tmp->appointment_id != id) {
            prev = tmp;
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Appointment not found.\n";
            return;
        }
        cout << left << setw(20) << "Appointment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Date"
            << setw(15) << "Status"
            << setw(20) << "Location" << endl;
        printappointment(id, head);
        cout << "Do you want to delete this appointment?(Y/N): ";
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled.\n";
            return;
        }
        if (prev == nullptr) {
            head = head->next;
        }
        else {
            prev->next = tmp->next;
        }
        delete tmp;
        cout << "Remove doctor successfully!!\n";
    }
    void searchappointment(appointment* head) {
        int id;
        cout << "Enter appointment ID to search: ";
        cin >> id;
        appointment* tmp = head;
        while (tmp != nullptr && tmp->appointment_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "\nAppointment with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(20) << "Appointment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Date"
            << setw(15) << "Status"
            << setw(20) << "Location" << endl;
        printappointment(id, head);
    };
    void updateappointmentforadmin(int id, appointment* head) {
        appointment* tmp = head;
        int new_appointment_id;
        bool id_exists;
        while (tmp != nullptr && tmp->appointment_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Appointment with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(20) << "Appointment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Date"
            << setw(15) << "Status"
            << setw(20) << "Location" << endl;
        printappointment(id, head);
        bool exit = false;
        while (!exit) {
            int purpose = 0;
            cout << "\nSelect options you want to fix:\n";
            cout << "[01] : Appointment ID\n";
            cout << "[02] : Doctor ID\n";
            cout << "[03] : Patient ID\n";
            cout << "[04] : Date\n";
            cout << "[05] : Status\n";
            cout << "[06] : Location\n";
            cout << "[00] : Save\n";
            cout << "\nEnter your choice: ";
            cin >> purpose;

            switch (purpose) {
            case 1:
                do {
                    cout << "Enter new appointment id: ";
                    cin >> new_appointment_id;
                    id_exists = false;
                    // Ki?m tra xem id m?i có t?n t?i trong danh sách không
                    appointment* check = head;
                    while (check != nullptr) {
                        if (check->appointment_id == new_appointment_id) {
                            id_exists = true;
                            break;
                        }
                        check = check->next;
                    }
                    if (id_exists) {
                        cout << "Appointment ID already exists. Please choose a different one." << endl;
                    }
                } while (id_exists);
                tmp->appointment_id = new_appointment_id;
                break;
            case 2:
                cout << "Enter new doctor id: ";
                cin >> tmp->doctor_id;
                break;
            case 3:
                cout << "Enter new patient id: ";
                cin >> tmp->patient_id;
                break;
            case 4:
                cout << "Enter new date: ";
                cin >> tmp->date;
                break;
            case 5:
                cout << "Enter new status: ";
                cin >> tmp->status;
                break;
            case 6:
                cout << "Enter new location: ";
                cin >> tmp->location;
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << "New information of appointment: \n";
        cout << left << setw(20) << "Appointment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Date"
            << setw(15) << "Status"
            << setw(20) << "Location" << endl;
        printappointment(tmp->appointment_id, head);
    }
    void updateappointment(int id, appointment* head, int doctor_id = 0) {
        appointment* tmp = head;
        while (tmp != nullptr) {
            if (tmp->appointment_id == id && tmp->doctor_id == doctor_id) {
                break;
            }
            else
            {
                tmp = tmp->next;
            }

        }
        if (tmp == nullptr) {
            cout << "Appointment with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(20) << "Appointment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Date"
            << setw(15) << "Status"
            << setw(20) << "Location" << endl;
        printappointment(id, head);
        bool exit = false;
        while (!exit) {
            int purpose = 0;
            cout << "\nSelect options you want to fix:\n";
            cout << "[01] : Status\n";
            cout << "[02] : Location\n";
            cout << "[00] : Save\n";
            cout << "\nEnter your choice: ";
            cin >> purpose;

            switch (purpose) {
            case 1:
                cout << "Enter new status(Y/N): ";
                cin >> tmp->status;
                if (tmp->status == "Y" || tmp->status == "y")
                {
                    tmp->status = "Confirmed";
                }
                else if (tmp->status == "N" || tmp->status == "n")
                {
                    tmp->status = "Rejected";
                }
                else {
                    cout << "Invalid choice";
                    break;
                }
                break;
            case 2:
                cout << "Enter new location: ";
                cin >> tmp->location;
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << "New information of appointment: \n";
        cout << left << setw(20) << "Appointment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Date"
            << setw(15) << "Status"
            << setw(20) << "Location" << endl;
        printappointment(id, head);
    }


    static void book_Appointment(int id, doctor* head, appointment* appointmenthead)
    {
        int type;
        int index;
        std::string list[10] = {
            "",
            "Cardiologist",
            "Neurologist",
            "Dentist",
            "Dermatologist",
            "Pediatrician",
            "Oncologist",
            "Orthopedist",
            "Surgeon",
            "Gynecologist",
        };
        while (true) {
            std::cout << "[1]: Cardiologist\n";
            std::cout << "[2]: Neurologist\n";
            std::cout << "[3]: Dentist\n";
            std::cout << "[4]: Dermatologist\n";
            std::cout << "[5]: Pediatrician\n";
            std::cout << "[6]: Oncologist\n";
            std::cout << "[7]: Orthopedist\n";
            std::cout << "[8]: Surgeon\n";
            std::cout << "[9]: Gynecologist\n";
            std::cout << "Enter kind of doctor: ";
            std::cin >> type;

            if (std::cin.fail() || type < 1 || type > 9) {
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                std::cout << "Invalid input. Please enter a number between 1 and 9.\n";
            }
            else {
                break;
            }
        }
        doctor* temp = head; // doctor
        int id_doctor[1000];
        int count = 0;
        while (temp != nullptr) {
            if (temp->type == list[type]) {
                if (count == 0) {
                    std::cout << std::left << std::setw(5) << "STT"
                        << setw(5) << "ID"
                        << setw(15) << "First Name"
                        << setw(15) << "Last Name"
                        << setw(10) << "Gender"
                        << setw(10) << "Age"
                        << setw(15) << "Phone"
                        << setw(30) << "Address"
                        << "Type" << std::endl;
                }
                std::cout << "[" << count << "] : ";
                doctor::printdoctordetail(temp->doctor_id, head);
                id_doctor[count] = temp->doctor_id;
                count++;
            }
            temp = temp->next;
        }

        if (count == 0) {
            cout << "No doctors available for the selected type.\n";
            return;
        }
        //// X? lí n?u nh?p l?i
        while (true) {
            cout << "Enter your choice : ";
            cin >> index;

            if (cin.fail() || index < 0 || index >= count) {
                cout << "Invalid input. Please enter a valid index.\n";
            }
            else {
                break;
            }
        }
        // NH?P NGÀY THÁNG N?M
        int day = 0;
        int month = 0;
        int year = 0;
        while (true) {
            cout << "Enter day : ";
            cin >> day;
            if (std::cin.fail() || day < 1 || day > 31) {
                cout << "Invalid day. Please enter a valid day (1-31).\n";
            }
            else {
                break;
            }
        }
        while (true) {
            cout << "Enter month : ";
            cin >> month;
            if (std::cin.fail() || month < 1 || month > 12) {
                std::cout << "Invalid month. Please enter a valid month (1-12).\n";
            }
            else {
                break;
            }
        }
        cout << "Enter year: ";
        cin >> year;
        int app_id = generateAppointmentID(appointmenthead);
        string date = to_string(day) + "/" + to_string(month) + "/" + to_string(year);
        cout << "Book appoitment successfully. Waiting for doctor's confirmation.\n";
        appointment* newapp = new appointment(app_id, id_doctor[index], id, date, "Pending", "Pending");
        addapointment(newapp, appointmenthead);


    }
    void printappointment(int id, appointment* head)
    {
        appointment* tmp = head;
        while (tmp != nullptr && tmp->appointment_id != id) {
            tmp = tmp->next;
        }

        cout << left << setw(20) << tmp->appointment_id
            << setw(15) << tmp->doctor_id
            << setw(15) << tmp->patient_id
            << setw(15) << tmp->date
            << setw(15) << tmp->status
            << setw(20) << tmp->location << endl;
    }
    void printallappointment(int id, int type, appointment* head, doctor* doctor_head, patient* patient_head) {
        // type = 0 admin , type = 2 doctor, type = 1 patient
        if (type == 0)
        {
            cout << left << setw(20) << "Appointment ID"
                << setw(15) << "Doctor ID"
                << setw(15) << "Patient ID"
                << setw(15) << "Date"
                << setw(15) << "Start"
                << setw(20) << "Location" << endl;
            appointment* temp = head->next;
            while (temp != nullptr)
            {
                printappointment(temp->appointment_id, head);
                temp = temp->next;
            }
        }
        else if (type == 2)
        {
            cout << left << setw(20) << "Appointment ID"
                << setw(15) << "Doctor ID"
                << setw(15) << "Patient ID"
                << setw(15) << "Date"
                << setw(15) << "Status"
                << setw(20) << "Location" << endl;
            appointment* temp = head;
            while (temp != nullptr)
            {
                if (temp->doctor_id == id)
                {
                    printappointment(temp->appointment_id, head);
                }
                temp = temp->next;
            }
        }
        else if (type == 1)
        {
            cout << left << setw(20) << "Appointment ID"
                << setw(15) << "Doctor ID"
                << setw(15) << "Patient ID"
                << setw(15) << "Date"
                << setw(15) << "Status"
                << setw(20) << "Location" << endl;
            appointment* temp = head;
            while (temp != nullptr)
            {
                if (temp->patient_id == id)
                {
                    printappointment(temp->appointment_id, head);
                }
                temp = temp->next;
            }
        }
    }
    static void getdatafromappointmentfile(appointment* head) {
        ifstream file("appointment.csv");
        if (file.is_open()) {
            string line;
            bool firstLine = true;
            while (getline(file, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string tempString;
                int doctor_id, appointment_id, patient_id;
                string date, status, location;

                getline(ss, tempString, ',');
                appointment_id = stoi(tempString);
                getline(ss, tempString, ',');
                doctor_id = stoi(tempString);
                getline(ss, tempString, ',');
                patient_id = stoi(tempString);
                getline(ss, date, ',');
                getline(ss, status, ',');
                getline(ss, location, ',');

                appointment* newAppointment = new appointment(appointment_id, doctor_id, patient_id, date, status, location);
                /*if (head == nullptr)
                {
                    head = newAppointment;
                }*/
                appointment* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newAppointment;
            }
            file.close();
        }
        else {
            cout << "Unable to open file: doctor.csv" << endl;
        }
    }
    static void updatefileappointment(appointment* head) {
        ofstream file("appointment.csv", ios::trunc);
        if (file.is_open()) {
            file << "Appointment ID,Doctor ID,Patient ID,Date,Status,Location\n";

            appointment* tmp = head->next;
            while (tmp != nullptr) {
                file << tmp->appointment_id << ","
                    << tmp->doctor_id << ","
                    << tmp->patient_id << ","
                    << tmp->date << ","
                    << tmp->status << ","
                    << tmp->location << "\n";
                tmp = tmp->next;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file appointment.csv";
        }
    }
};
class treatment {
private:
    string start;
    string end;
    string description;
public:
    int patient_id;
    int treatment_id;
    int doctor_id;
    treatment* next;
    treatment() : next(nullptr) {}
    treatment(int t_id, int d_id, int p_id, const string& s, const string& e, const string& desc)
        : treatment_id(t_id), doctor_id(d_id), patient_id(p_id), start(s), end(e), description(desc) {}
    static int generateTreatmentID(treatment* head) {
        int max_id = 0;
        treatment* temp = head;
        while (temp != nullptr) {
            if (temp->treatment_id > max_id) {
                max_id = temp->treatment_id;
            }
            temp = temp->next;
        }
        return max_id + 1;
    }
    void addtreatment(int id, treatment* head) {
        treatment* temp = head;
        treatment* newTreatment = new treatment();
        newTreatment->treatment_id = generateTreatmentID(head);
        cout << "Please enter information of treatment below\n\n";
        cout << "Enter Patient ID: ";
        cin >> newTreatment->patient_id;
        cout << "Enter start date: ";
        cin >> newTreatment->start;
        cout << "Enter end date: ";
        cin >> newTreatment->end;
        cout << "Enter desciption: ";
        getline(cin >> ws, newTreatment->description);
        if (head == nullptr) {
            head = newTreatment;
            newTreatment->next = nullptr;
        }
        while (temp->next != nullptr)
        {
            temp = temp->next;
        }
        temp->next = newTreatment;
        cout << "Add treatment successfully!!!\n";

    };
    void updatetreatmentdetailforadmin(int id, treatment* head) {
        treatment* tmp = head;

        while (tmp != nullptr && tmp->treatment_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Treatment with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(20) << "\nTreatment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Start"
            << setw(15) << "End"
            << setw(30) << "Description" << endl;
        printtreatmentdetail(id, head);
        int new_treatment_id;
        bool id_exists;
        bool exit = false;
        while (!exit) {
            int purpose = 0;
            cout << "\nSelect options you want to fix:\n";
            cout << "[01] : Treatment ID\n";
            cout << "[02] : Doctor ID\n";
            cout << "[03] : Patient ID\n";
            cout << "[04] : Start date\n";
            cout << "[05] : End date\n";
            cout << "[06] : Desciption\n";
            cout << "[00] : Save\n";
            cout << "\nEnter your choice: ";
            cin >> purpose;

            switch (purpose) {
            case 1:
                do {
                    cout << "Enter new treatment id: ";
                    cin >> new_treatment_id;
                    id_exists = false;
                    // Ki?m tra xem id m?i có t?n t?i trong danh sách không
                    treatment* check = head;
                    while (check != nullptr) {
                        if (check->treatment_id == new_treatment_id) {
                            id_exists = true;
                            break;
                        }
                        check = check->next;
                    }
                    if (id_exists) {
                        cout << "Treatment ID already exists. Please choose a different one." << endl;
                    }
                } while (id_exists);
                tmp->treatment_id = new_treatment_id;
                break;
            case 2:
                cout << "Enter new doctor id: ";
                cin >> tmp->doctor_id;
                break;
            case 3:
                cout << "Enter new patient id: ";
                cin >> tmp->patient_id;
                break;
            case 4:
                cout << "Enter new start date: ";
                cin >> tmp->start;
                break;
            case 5:
                cout << "Enter new end date: ";
                cin >> tmp->end;
                break;
            case 6:
                cout << "Enter new desciption: ";
                cin >> tmp->description;
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << "New information of treatment: \n";
        cout << left << setw(20) << "\nTreatment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Start"
            << setw(15) << "End"
            << setw(30) << "Description" << endl;
        printtreatmentdetail(tmp->treatment_id, head);
    }
    void updatetreatmentdetail(int id, treatment* head, int doctor_id = 0) {
        treatment* tmp = head;
        while (tmp != nullptr) {
            if (tmp->treatment_id == id && tmp->doctor_id == doctor_id) {
                break;
            }
            else
            {
                tmp = tmp->next;
            }


        }
        if (tmp == nullptr) {
            cout << "Treatment with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(20) << "\nTreatment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Start"
            << setw(15) << "End"
            << setw(30) << "Description" << endl;
        printtreatmentdetail(id, head);
        bool exit = false;
        while (!exit) {
            int purpose = 0;
            cout << "\nSelect options you want to fix:\n";
            cout << "[01] : Start date\n";
            cout << "[02] : End date\n";
            cout << "[03] : Desciption\n";
            cout << "[00] : Save\n";
            cout << "\nEnter your choice: ";
            cin >> purpose;

            switch (purpose) {
            case 1:
                cout << "Enter new start date: ";
                cin >> tmp->start;
                break;
            case 2:
                cout << "Enter new end date: ";
                cin >> tmp->end;
                break;
            case 3:
                cout << "Enter new desciption: ";
                cin >> tmp->description;
                break;
            case 0:
                exit = true;
                break;
            default:
                cout << "Invalid choice. Please try again." << endl;
                break;
            }
        }
        cout << "New information of treatment: \n";
        cout << left << setw(20) << "\nTreatment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Start"
            << setw(15) << "End"
            << setw(30) << "Description" << endl;
        printtreatmentdetail(id, head);
    }
    void removetreatment(treatment* head) {
        int id;
        char confirm;
        cout << "Enter ID: ";
        cin >> id;

        if (head == nullptr) return;

        treatment* tmp = head;
        treatment* prev = nullptr;

        while (tmp != nullptr && tmp->treatment_id != id) {
            prev = tmp;
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Treatment not found.\n";
            return;
        }
        cout << left << setw(20) << "Treatment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Start"
            << setw(15) << "End"
            << setw(30) << "Description" << endl;
        printtreatmentdetail(id, head);
        cout << "Do you want to delete this treatment?(Y/N): ";
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled.\n";
            return;
        }
        if (prev == nullptr) {
            head = head->next;
        }
        else {
            prev->next = tmp->next;
        }
        delete tmp;
        cout << "Remove doctor successfully!!\n";

    };
    void searchtreatment(treatment* head) {
        int id;
        cout << "Enter treatment ID to search: ";
        cin >> id;
        treatment* tmp = head;
        while (tmp != nullptr && tmp->treatment_id != id) {
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "\nTreatment with ID " << id << " not found." << endl;
            return;
        }
        cout << left << setw(20) << "\nTreatment ID"
            << setw(15) << "Doctor ID"
            << setw(15) << "Patient ID"
            << setw(15) << "Start"
            << setw(15) << "End"
            << setw(30) << "Description" << endl;
        printtreatmentdetail(id, head);
    };
    void printtreatmentdetail(int id, treatment* head)
    {
        treatment* tmp = head;
        while (tmp != nullptr && tmp->treatment_id != id) {
            tmp = tmp->next;
        }

        cout << left << setw(20) << tmp->treatment_id
            << setw(15) << tmp->doctor_id
            << setw(15) << tmp->patient_id
            << setw(15) << tmp->start
            << setw(15) << tmp->end
            << setw(30) << tmp->description << endl;
    };
    void printalltreatment(int id, int type, treatment* head, doctor* doctor_head, patient* patient_head) {
        // type = 0 admin , type = 2 doctor, type = 1 patient
        if (type == 0)
        {
            cout << left << setw(20) << "Treatment ID"
                << setw(15) << "Doctor ID"
                << setw(15) << "Patient ID"
                << setw(15) << "Start"
                << setw(15) << "End"
                << setw(30) << "Description" << endl;
            treatment* temp = head->next;
            while (temp != nullptr)
            {
                printtreatmentdetail(temp->treatment_id, head);
                temp = temp->next;
            }
        }
        else if (type == 2)
        {
            cout << left << setw(20) << "Treatment ID"
                << setw(15) << "Doctor ID"
                << setw(15) << "Patient ID"
                << setw(15) << "Start"
                << setw(15) << "End"
                << setw(30) << "Description" << endl;
            treatment* temp = head;
            while (temp != nullptr)
            {
                if (temp->doctor_id == id)
                {
                    printtreatmentdetail(temp->treatment_id, head);
                }
                temp = temp->next;
            }
        }
        else if (type == 1)
        {
            cout << left << setw(20) << "Treatment ID"
                << setw(15) << "Doctor ID"
                << setw(15) << "Patient ID"
                << setw(15) << "Start"
                << setw(15) << "End"
                << setw(30) << "Desciption" << endl;
            treatment* temp = head;
            while (temp != nullptr)
            {
                if (temp->patient_id == id)
                {
                    printtreatmentdetail(temp->treatment_id, head);
                }
                temp = temp->next;
            }
        }
    }
    static void getdatafromfiletreatment(treatment* head) {
        ifstream file("treatment.csv");
        if (file.is_open()) {
            string line;
            bool firstLine = true;
            while (getline(file, line)) {
                if (firstLine) {
                    firstLine = false;
                    continue;
                }
                stringstream ss(line);
                string tempString;
                int doctor_id, treatment_id, patient_id;
                string start, end, description;

                getline(ss, tempString, ',');
                treatment_id = stoi(tempString);
                getline(ss, tempString, ',');
                doctor_id = stoi(tempString);
                getline(ss, tempString, ',');
                patient_id = stoi(tempString);
                getline(ss, start, ',');
                getline(ss, end, ',');
                getline(ss, description, ',');

                treatment* newTreatment = new treatment(treatment_id, doctor_id, patient_id, start, end, description);
                treatment* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = newTreatment;
            }
            file.close();
        }
        else {
            cout << "Unable to open file: treatment.csv" << endl;
        }
    };
    static void updatefiletreatment(treatment* head) {
        ofstream file("treatment.csv", ios::trunc);
        if (file.is_open()) {
            file << "Treatment ID,Doctor ID,Patient ID,Start,End,Description\n";

            treatment* tmp = head->next;
            while (tmp != nullptr) {
                file << tmp->treatment_id << ","
                    << tmp->doctor_id << ","
                    << tmp->patient_id << ","
                    << tmp->start << ","
                    << tmp->end << ","
                    << tmp->description << "\n";
                tmp = tmp->next;
            }
            file.close();
        }
        else {
            cerr << "Unable to open file";
        }
    }
};
class admin {
public:
    static void printalldoctor(doctor* head) {
        doctor* temp = head->next;
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << "Type" << endl;
        while (temp != nullptr)
        {
            doctor::printdoctordetail(temp->doctor_id, head->next);
            temp = temp->next;
        }
    }
    static void printallpatient(patient* head) {
        patient* temp = head->next;
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << setw(10) << "Height"
            << "Weight" << endl;
        while (temp != nullptr)
        {
            patient::printpatientdetail(temp->patient_id, head->next);
            temp = temp->next;
        }
    }

    static void patienttreatment(int doctor_id, treatment* head, patient* head1) {
        treatment* tmp = head;
        while (tmp != nullptr) {
            if (tmp->doctor_id == doctor_id) {
                cout << left << setw(5) << "ID"
                    << setw(15) << "First Name"
                    << setw(15) << "Last Name"
                    << setw(10) << "Gender"
                    << setw(10) << "Age"
                    << setw(15) << "Phone"
                    << setw(30) << "Address"
                    << setw(10) << "Height"
                    << "Weight" << endl;
                patient::printpatientdetail(tmp->patient_id, head1);
            }
            tmp = tmp->next;
        }
    }
    static void removepatient(patient* head, appointment* head2, Account* head3) {
        int id;
        char confirm;
        cout << "Enter ID: ";
        cin >> id;

        if (head == nullptr) return;
        patient* tmp = head;
        patient* prev = nullptr;

        while (tmp != nullptr && tmp->patient_id != id) {
            prev = tmp;
            tmp = tmp->next;
        }

        if (tmp == nullptr) {
            cout << "Patient not found.\n";
            return;
        }
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << setw(10) << "Height"
            << "Weight" << endl;
        patient::printpatientdetail(id, head);
        cout << "Do you want to delete this patient?(Y/N)";
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled.\n";
            return;
        }
        if (prev == nullptr) {
            head = head->next;
        }
        else {
            prev->next = tmp->next;
        }
        delete tmp;
        // Xóa t?t c? các cu?c h?n c?a b?nh nhân
        appointment* currentAppointment = head2;
        appointment* prevAppointment = nullptr;
        while (currentAppointment != nullptr) {
            if (currentAppointment->patient_id == id) {
                if (prevAppointment == nullptr) {
                    head2 = head2->next;
                }
                else {
                    prevAppointment->next = currentAppointment->next;
                }
                appointment* temp = currentAppointment;
                currentAppointment = currentAppointment->next;
                delete temp;
            }
            else {
                prevAppointment = currentAppointment;
                currentAppointment = currentAppointment->next;
            }
        }
        // Delete the account of the patient
        Account* currentAccount = head3;
        Account* prevAccount = nullptr;
        while (currentAccount != nullptr) {
            if (currentAccount->id == id && currentAccount->type == "1") {
                if (prevAccount == nullptr) {
                    head3 = head3->next;
                }
                else {
                    prevAccount->next = currentAccount->next;
                }
                Account* temp = currentAccount;
                currentAccount = currentAccount->next;
                delete temp;
            }
            else {
                prevAccount = currentAccount;
                currentAccount = currentAccount->next;
            }
        }

        cout << "Delete patient successfully!!!\n";
    };
    static void removedoctor(doctor* head, appointment* head2, Account* head3) {
        int id;
        char confirm;
        cout << "Enter ID: ";
        cin >> id;

        if (head == nullptr) return;

        doctor* tmp = head;
        doctor* prev = nullptr;

        while (tmp != nullptr && tmp->doctor_id != id) {
            prev = tmp;
            tmp = tmp->next;
        }
        if (tmp == nullptr) {
            cout << "Doctor not found.\n";
            return;
        }
        cout << left << setw(5) << "ID"
            << setw(15) << "First Name"
            << setw(15) << "Last Name"
            << setw(10) << "Gender"
            << setw(10) << "Age"
            << setw(15) << "Phone"
            << setw(30) << "Address"
            << "Type" << endl;
        doctor::printdoctordetail(id, head);
        cout << "Do you want to delete this doctor?(Y/N): ";
        cin >> confirm;
        if (confirm != 'y' && confirm != 'Y') {
            cout << "Deletion cancelled.\n";
            return;
        }
        if (prev == nullptr) {
            head = head->next;
        }
        else {
            prev->next = tmp->next;
        }
        delete tmp;
        // Xóa t?t c? các cu?c h?n c?a bác s?
        appointment* currentAppointment = head2;
        appointment* prevAppointment = nullptr;
        while (currentAppointment != nullptr) {
            if (currentAppointment->doctor_id == id) {
                if (prevAppointment == nullptr) {
                    head2 = head2->next;
                }
                else {
                    prevAppointment->next = currentAppointment->next;
                }
                appointment* temp = currentAppointment;
                currentAppointment = currentAppointment->next;
                delete temp;
            }
            else {
                prevAppointment = currentAppointment;
                currentAppointment = currentAppointment->next;
            }
        }
        // Delete the account of the doctor
        Account* currentAccount = head3;
        Account* prevAccount = nullptr;
        while (currentAccount != nullptr) {
            if (currentAccount->id == id && currentAccount->type == "2") {
                if (prevAccount == nullptr) {
                    head3 = head3->next;
                }
                else {
                    prevAccount->next = currentAccount->next;
                }
                Account* temp = currentAccount;
                currentAccount = currentAccount->next;
                delete temp;
            }
            else {
                prevAccount = currentAccount;
                currentAccount = currentAccount->next;
            }
        }
        cout << "Remove doctor successfully!!\n";
    }

};

int main() {
    string user;
    int password;
    string type;
    int id;
    Account* listHeader = new Account("admin", 1, 0, "3");
    doctor* doctorheader = new doctor();
    doctor::getDataFromFileDoctor(doctorheader);
    patient* patientheader = new patient();
    patient::getDataFromFilePatient(patientheader);
    Account::getDataFromFileAccount(listHeader);
    appointment* appointmentheader = new appointment();
    appointment::getdatafromappointmentfile(appointmentheader);
    appointment* temp = appointmentheader;
    treatment* treatmentheader = new treatment();
    treatment::getdatafromfiletreatment(treatmentheader);
    treatment* temp2 = treatmentheader;
    /*while (true && temp != nullptr)
    {
        appointment p;
        p.printappointment(temp->appointment_id, appointmentheader);
        temp = temp->next;
    }*/
    string title = "WELCOME TO HOSPITAL MANAGEMENT";
    int width = 120;
    int padding = (width - title.length()) / 2;
    string border(width, '*');
    cout << border << endl;
    cout << setw(padding) << "" << title << endl;
    cout << border << endl;
    while (true) {
        cout << "Enter user: ";
        cin >> user;
        cout << "Enter password: ";
        cin >> password;
        cout << "Choose kind of account: " << endl;
        cout << "[1]: Patient" << endl;
        cout << "[2]: Doctor" << endl;
        cout << "[3]: Admin" << endl;
        cin >> type;

        if (Account::checkLogin(listHeader, user, password, type, id) == 1) {
            cout << "Login successfully!";
            break;
        }
        else {
            system("cls");
            cout << "Invalid login credentials!\n";
        }
    }
    while (true) {
        /*Patient menu*/
        if (type == "1") {
            system("cls");
            bool exit = false;
            while (!exit)
            {
                int purpose = 0;
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "\nSelect an option:\n\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "./HOME/PATIENTS\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "[01] : Get patient details\n";
                cout << "[02] : Book appointment\n";
                cout << "[03] : Check appointment\n";
                cout << "[04] : Get treatment details\n";
                cout << "[00] : Back\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
                cout << "Enter your choice: ";
                cin >> purpose;

                if (purpose == 1)
                {
                    cout << left << setw(5) << "ID"
                        << setw(15) << "First Name"
                        << setw(15) << "Last Name"
                        << setw(10) << "Gender"
                        << setw(10) << "Age"
                        << setw(15) << "Phone"
                        << setw(30) << "Address"
                        << setw(10) << "Height"
                        << "Weight" << endl;
                    patient p;
                    p.printpatientdetail(id, patientheader);
                }
                else if (purpose == 2)
                {
                    appointment::book_Appointment(id, doctorheader, appointmentheader);
                    appointment::updatefileappointment(appointmentheader);
                }
                else if (purpose == 3)
                {
                    appointment a;
                    a.printallappointment(id, 1, appointmentheader, doctorheader, patientheader);
                }
                else if (purpose == 4)
                {
                    treatment t;
                    t.printalltreatment(id, 1, treatmentheader, doctorheader, patientheader);
                }
                else if (purpose == 0)
                {
                    exit = true;
                    break;
                }
                else
                {
                    cout << "Invalid choice!\n";
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

                cout << endl;
                cout << "\nPress ENTER to continue...\n";
                cout << endl;

                getchar();
            };
        }
        /*Doctor menu*/
        else if (type == "2") {
            system("cls");
            bool exit = false;
            while (!exit)
            {
                int purpose = 0;
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "\nSelect an option:\n\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "./HOME/DOCTORS\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "[01] : Get doctor details\n";
                cout << "[02] : Get patient details\n";
                cout << "[03] : Check appointment\n";
                cout << "[04] : Get treatment details\n";
                cout << "[05] : Response the appointment\n";
                cout << "[06] : Add treatment\n";
                cout << "[07] : Update treatment detail\n";
                cout << "[00] : Back\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
                cout << "Enter your choice: ";
                cin >> purpose;

                if (purpose == 1)
                {
                    doctor d;
                    cout << left << setw(5) << "ID"
                        << setw(15) << "First Name"
                        << setw(15) << "Last Name"
                        << setw(10) << "Gender"
                        << setw(10) << "Age"
                        << setw(15) << "Phone"
                        << setw(30) << "Address"
                        << "Type" << endl;
                    d.printdoctordetail(id, doctorheader);
                }
                else if (purpose == 2)
                {
                    admin::patienttreatment(id, treatmentheader, patientheader);
                }
                else if (purpose == 3)
                {
                    appointment a;
                    a.printallappointment(id, 2, appointmentheader, doctorheader, patientheader);
                }
                else if (purpose == 4)
                {
                    treatment t;
                    t.printalltreatment(id, 2, treatmentheader, doctorheader, patientheader);
                }
                else if (purpose == 5)
                {
                    int ida;
                    cout << "Enter appointment id:";
                    cin >> ida;
                    appointment a;
                    a.updateappointment(ida, appointmentheader, id);
                    a.updatefileappointment(appointmentheader);
                }
                else if (purpose == 6)
                {
                    treatment t;
                    t.addtreatment(id, treatmentheader);
                    t.updatefiletreatment(treatmentheader);
                }
                else if (purpose == 7)
                {
                    int idd;
                    cout << "Enter appointment id:";
                    cin >> idd;
                    treatment t;
                    t.updatetreatmentdetail(idd, treatmentheader, id);
                    t.updatefiletreatment(treatmentheader);
                }
                else if (purpose == 0)
                {
                    exit = true;
                    break;
                }
                else
                {
                    cout << "Invalid choice!\n";
                }

                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

                cout << endl;
                cout << "\nPress ENTER to continue...\n";
                cout << endl;

                getchar();
            }
        }
        /*Admin menu*/
        else if (type == "3") {
            system("cls");
            bool exit = false;
            while (!exit) {
                int purpose = 0;
                cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "\nSelect an option:\n\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "./HOME/ADMIN\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                cout << "[01] : Add\n";
                cout << "[02] : Remove\n";
                cout << "[03] : Search\n";
                cout << "[04] : Update information\n";
                cout << "[05] : Print all information\n";
                cout << "[00] : Back\n";
                cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
                cout << "Enter your choice: ";
                cin >> purpose;
                if (purpose == 1) {
                    system("cls");
                    bool exit1 = false;
                    while (!exit1) {
                        int purpose1 = 0;
                        cout << "Select an option:\n\n";
                        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
                        cout << "[01] : Register new patient\n";
                        cout << "[02] : Register new doctor\n";
                        cout << "[03] : Register new appointment\n";
                        cout << "[04] : Register new treatment\n";
                        cout << "[00] : Back\n";
                        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~\n\n";
                        cout << "Enter your choice: ";
                        cin >> purpose1;
                        if (purpose1 == 1) {
                            patient p;
                            p.addpatient(patientheader, listHeader);
                            p.updatefilepatient(patientheader);
                            Account::updatefileaccount(listHeader);
                        }
                        else if (purpose1 == 2) {
                            doctor d;
                            d.adddoctor(doctorheader, listHeader);
                            d.updatefiledoctor(doctorheader);
                            Account::updatefileaccount(listHeader);
                        }
                        else if (purpose1 == 3) {

                            string date, status, location;
                            int doctor_id, patient_id;
                            int app_id = appointment::generateAppointmentID(appointmentheader);
                            cout << "Enter ID doctor : ";
                            cin >> doctor_id;
                            cout << "Enter ID patient : ";
                            cin >> patient_id;
                            cout << "Enter date : ";
                            cin >> date;    // EX: "20/12/2003"
                            cout << "Enter status : ";
                            cin >> status;
                            cout << "Enter location : ";
                            cin >> location;
                            appointment* a = new appointment(app_id, doctor_id, patient_id, date, status, location);
                            appointment::addapointment(a, appointmentheader);
                            cout << "Register new appointment successfully!!!\n";
                            appointment::updatefileappointment(appointmentheader);
                        }
                        else if (purpose1 == 4) {
                            treatment t;
                            t.addtreatment(id, treatmentheader);
                            t.updatefiletreatment(treatmentheader);
                        }
                        else if (purpose1 == 0) {
                            system("cls");
                            exit1 = true;
                            break;
                        }
                    }
                }
                else if (purpose == 2) {
                    system("cls");
                    bool exit2 = false;
                    while (!exit2)
                    {
                        int purpose2 = 0;
                        cout << "Select an option:\n\n";
                        cout << "[01] : Remove a patient\n";
                        cout << "[02] : Remove a doctor\n";
                        cout << "[03] : Remove an appointment\n";
                        cout << "[04] : Remove a treatment\n";
                        cout << "[00] : Back\n";
                        cout << "Enter your choice: ";
                        cin >> purpose2;
                        if (purpose2 == 1) {
                            patient p;
                            appointment a;
                            admin::removepatient(patientheader, appointmentheader, listHeader);
                            p.updatefilepatient(patientheader);
                            a.updatefileappointment(appointmentheader);
                            Account::updatefileaccount(listHeader);
                        }
                        if (purpose2 == 2) {
                            doctor d;
                            appointment a;
                            admin::removedoctor(doctorheader, appointmentheader, listHeader);
                            d.updatefiledoctor(doctorheader);
                            a.updatefileappointment(appointmentheader);
                            Account::updatefileaccount(listHeader);
                        }
                        if (purpose2 == 3)
                        {
                            appointment a;
                            a.removeappointment(appointmentheader);
                            a.updatefileappointment(appointmentheader);
                        }
                        if (purpose2 == 4) {
                            treatment t;
                            t.removetreatment(treatmentheader);
                            t.updatefiletreatment(treatmentheader);
                        }
                        if (purpose2 == 0) {
                            system("cls");
                            exit2 = true;
                            break;
                        }
                    }
                }
                else if (purpose == 3) {
                    system("cls");
                    bool exit3 = false;
                    while (!exit3) {
                        int purpose3 = 0;
                        cout << "\nSelect an option:\n\n";
                        cout << "[01] : Search patient information\n";
                        cout << "[02] : Search doctor information\n";
                        cout << "[03] : Search appointment information\n";
                        cout << "[04] : Search treatment information\n";
                        cout << "[00] : Back\n";
                        cout << "Enter your choice: ";
                        cin >> purpose3;
                        if (purpose3 == 1) {
                            patient p;
                            p.searchpatient(patientheader);
                        }
                        if (purpose3 == 2) {
                            doctor d;
                            d.searchdoctor(doctorheader);
                        }
                        if (purpose3 == 3) {
                            appointment a;
                            a.searchappointment(appointmentheader);
                        }
                        if (purpose3 == 4) {
                            treatment t;
                            t.searchtreatment(treatmentheader);
                        }
                        if (purpose3 == 0) {
                            system("cls");
                            break;
                            exit = true;
                        }
                    }

                }
                else if (purpose == 4) {
                    system("cls");
                    bool exit4 = false;
                    while (!exit4) {
                        int purpose4 = 0;
                        cout << "\nSelect an option:\n\n";
                        cout << "[01] : Update patient information\n";
                        cout << "[02] : Update doctor information\n";
                        cout << "[03] : Update appointment information\n";
                        cout << "[04] : Update treatment information\n";
                        cout << "[00] : Back\n";
                        cout << "Enter your choice: ";
                        cin >> purpose4;
                        if (purpose4 == 1) {
                            int idupdate1;
                            cout << "Enter ID: ";
                            cin >> idupdate1;
                            patient p;
                            p.updatepatient(idupdate1, patientheader);
                            p.updatefilepatient(patientheader);
                        }
                        if (purpose4 == 2) {
                            int idupdate;
                            cout << "Enter ID: ";
                            cin >> idupdate;
                            doctor d;
                            d.updatedoctor(idupdate, doctorheader);
                            d.updatefiledoctor(doctorheader);
                        }
                        if (purpose4 == 3) {
                            int ida;
                            cout << "Enter appointment id:";
                            cin >> ida;
                            appointment a;
                            a.updateappointmentforadmin(ida, appointmentheader);
                            a.updatefileappointment(appointmentheader);
                        }
                        if (purpose4 == 4) {
                            int idd;
                            cout << "Enter treatment id: ";
                            cin >> idd;
                            treatment t;
                            t.updatetreatmentdetailforadmin(idd, treatmentheader);
                            t.updatefiletreatment(treatmentheader);
                        }
                        if (purpose4 == 0) {
                            system("cls");
                            break;
                            exit4 = true;
                        }
                    }

                }
                else if (purpose == 5) {
                    system("cls");
                    bool exit5 = false;
                    while (!exit5) {
                        int purpose5 = 0;
                        cout << "\nSelect an option:\n\n";
                        cout << "[01] : Print information of all patient\n";
                        cout << "[02] : Print information of all doctor\n";
                        cout << "[03] : Print information of all appointment\n";
                        cout << "[04] : Print information of all treatment\n";
                        cout << "[05] : Print information of all account\n";
                        cout << "[00] : Back\n";
                        cout << "Enter your choice: ";
                        cin >> purpose5;
                        if (purpose5 == 1) {
                            /*system("cls");*/
                            admin::printallpatient(patientheader);
                        }
                        if (purpose5 == 2) {
                            /*system("cls");*/
                            admin::printalldoctor(doctorheader);
                        }
                        if (purpose5 == 3) {
                            appointment a;
                            a.printallappointment(id, 0, appointmentheader, doctorheader, patientheader);
                        }
                        if (purpose5 == 4) {
                            treatment t;
                            t.printalltreatment(id, 0, treatmentheader, doctorheader, patientheader);
                        }
                        if (purpose5 == 5) {
                            Account::printlistAccount(listHeader);
                        }
                        if (purpose5 == 0) {
                            system("cls");
                            break;
                            exit5 = true;
                        }
                    }
                }
                else if (purpose == 0) {
                    exit = true;
                    break;
                }
                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // clearing cin buffer;

                cout << endl;
                cout << "\nPress ENTER to continue...\n";
                cout << endl;

                getchar();
            };
        }

        cout << "Do you want to log out? (Y/N): ";
        char choice;
        cin >> choice;
        if (choice == 'Y' || choice == 'y') {
            system("cls");
            main();
        }
        else if (choice == 'N' || choice == 'n') {
            system("cls");
            continue;
        }
        else {
            cout << "Invalid choice!" << endl;
        }
    }
    return 0;
}
