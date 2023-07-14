#include<bits/stdc++.h>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <algorithm>

using namespace std;

const string RED_COLOR = "\033[31m";
const string GREEN_COLOR = "\033[32m";
const string CYAN_COLOR = "\033[36m";
const string BG_BLUE = "\033[44m";

const string RESET_COLOR = "\033[0m";
const string BG_CYAN = "\033[46m";
const string FG_RED = "\033[31m";
const string FG_GREEN = "\033[32m";
const string FG_YELLOW = "\033[33m";
const string FG_BLUE = "\033[34m";
const string FG_MAGENTA = "\033[35m";
const string FG_CYAN = "\033[36m";

struct Criminal {
    string id;
    string name;
    int age;
    string crime;
    string sentence;
    string dateOfCrime;
    string address;

    Criminal(const string& id, const string& name, int age, const string& crime,
             const string& sentence, const string& dateOfCrime, const string& address)
        : id(id), name(name), age(age), crime(crime),
          sentence(sentence), dateOfCrime(dateOfCrime), address(address) {}
};

class CriminalManagementSystem {
private:
    vector<list<Criminal>> hashTable;
    const string filename;
    const int size;

public:
    explicit CriminalManagementSystem(const string& filename, int size)
        : filename(filename), size(size) {
        hashTable.resize(size);
    }

    void loadCriminals() {
        ifstream file(filename);
        if (file.is_open()) {
            string id, name, crime, sentence, dateOfCrime, address;
            int age;
            while (file >> id >> name >> age >> crime >> sentence >> dateOfCrime >> address) {
                int hashValue = hashFunction(id);
                hashTable[hashValue].emplace_back(id, name, age, crime, sentence, dateOfCrime, address);
            }
            file.close();
            cout << GREEN_COLOR << "Criminals loaded successfully." << RESET_COLOR << endl;
        } else {
            cout << RED_COLOR << "Error: Failed to open file " << filename << "." << RESET_COLOR << endl;
        }
    }

    void saveCriminals() {
        ofstream file(filename);
        if (file.is_open()) {
            for (const auto& bucket : hashTable) {
                for (const auto& criminal : bucket) {
                    file << criminal.id << " " << criminal.name << " " << criminal.age << " "
                         << criminal.crime << " " << criminal.sentence << " "
                         << criminal.dateOfCrime << " " << criminal.address << endl;
                }
            }
            file.close();
            cout << GREEN_COLOR << "Criminals saved successfully." << RESET_COLOR << endl;
        } else {
            cout << RED_COLOR << "Error: Failed to open file " << filename << " for writing." << RESET_COLOR << endl;
        }
    }

    void addCriminal() {
        string id, name, crime, sentence, dateOfCrime, address;
        int age;
        cout << "Enter the ID: ";
        cin.ignore();
        getline(cin, id);

        // Check if the ID already exists in the file
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            string existingId;
            istringstream iss(line);
            if (iss >> existingId) {
                if (existingId == id) {
                    cout << RED_COLOR << "Error: Criminal with ID " << id << " already exists." << RESET_COLOR << endl;
                    file.close();
                    return;
                }
            }
        }
        file.close();

        int hashValue = hashFunction(id);
        list<Criminal>& bucket = hashTable[hashValue];

        cout << "Enter the Name: ";
        getline(cin, name);
        cout << "Enter the Age: ";
        cin >> age;
        cout << "Enter the Crime: ";
        cin.ignore();
        getline(cin, crime);
        cout << "Enter the Sentence: ";
        getline(cin, sentence);
        cout << "Enter the Date of Crime: ";
        getline(cin, dateOfCrime);
        cout << "Enter the Address: ";
        getline(cin, address);

        bucket.emplace_back(id, name, age, crime, sentence, dateOfCrime, address);

        cout << GREEN_COLOR << "Criminal added successfully." << RESET_COLOR << endl;
    }


void deleteCriminal() {
    string id;
    cout << "Enter the ID of the criminal you want to delete: ";
    cin.ignore();
    getline(cin, id);

    bool found = false;

    // Open the input file and create a temporary output file
    ifstream inputFile(filename);
    ofstream tempFile("temp.txt");

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string currentId;
            if (iss >> currentId) {
                if (currentId == id) {
                    // Skip the line if the ID matches
                    found = true;
                    continue;
                }
            }
            // Copy the line as it is to the temporary file
            tempFile << line << endl;
        }

        inputFile.close();
        tempFile.close();

        // Rename the temporary file to replace the original file
        if (found) {
            if (remove(filename.c_str()) == 0) {
                if (rename("temp.txt", filename.c_str()) == 0) {
                    cout << GREEN_COLOR << "Criminal deleted successfully." << RESET_COLOR << endl;
                } else {
                    cout << RED_COLOR << "Error: Failed to rename the temporary file." << RESET_COLOR << endl;
                }
            } else {
                cout << RED_COLOR << "Error: Failed to remove the original file." << RESET_COLOR << endl;
            }
        } else {
            cout << RED_COLOR << "Error: Criminal with ID " << id << " not found." << RESET_COLOR << endl;
            // Remove the temporary file
            remove("temp.txt");
        }
    } else {
        cout << RED_COLOR << "Error: Failed to open file " << filename << "." << RESET_COLOR << endl;
    }
}


 void modifyCriminal() {
    string id;
    cout << "Enter the ID of the criminal you want to modify: ";
    cin.ignore();
    getline(cin, id);

    bool found = false;

    // Open the input file and create a temporary output file
    ifstream inputFile(filename);
    ofstream tempFile("temp.txt");

    if (inputFile.is_open() && tempFile.is_open()) {
        string line;
        while (getline(inputFile, line)) {
            istringstream iss(line);
            string currentId;
            if (iss >> currentId) {
                if (currentId == id) {
                    string name, crime, sentence, dateOfCrime, address;
                    int age;
                    iss >> name >> age >> crime >> sentence >> dateOfCrime >> address;

                    cout << "Enter the new Name: ";
                    getline(cin, name);
                    cout << "Enter the new Age: ";
                    cin >> age;
                    cout << "Enter the new Crime: ";
                    cin.ignore();
                    getline(cin, crime);
                    cout << "Enter the new Sentence: ";
                    getline(cin, sentence);
                    cout << "Enter the new Date of Crime: ";
                    getline(cin, dateOfCrime);
                    cout << "Enter the new Address: ";
                    getline(cin, address);

                    // Update the details with the new values
                    tempFile << currentId << " " << name << " " << age << " " << crime << " " << sentence << " "
                             << dateOfCrime << " " << address << endl;

                    found = true;
                } else {
                    // Copy the line as it is to the temporary file
                    tempFile << line << endl;
                }
            }
        }

        inputFile.close();
        tempFile.close();

        // Rename the temporary file to replace the original file
        if (found) {
            if (remove(filename.c_str()) == 0) {
                if (rename("temp.txt", filename.c_str()) == 0) {
                    cout << GREEN_COLOR << "Criminal modified successfully." << RESET_COLOR << endl;
                } else {
                    cout << RED_COLOR << "Error: Failed to rename the temporary file." << RESET_COLOR << endl;
                }
            } else {
                cout << RED_COLOR << "Error: Failed to remove the original file." << RESET_COLOR << endl;
            }
        } else {
            cout << RED_COLOR << "Error: Criminal with ID " << id << " not found." << RESET_COLOR << endl;
            // Remove the temporary file
            remove("temp.txt");
        }
    } else {
        cout << RED_COLOR << "Error: Failed to open file " << filename << "." << RESET_COLOR << endl;
    }
}


    void searchCriminalById() {
    string id;
    cout << "Enter the ID of the criminal you want to search: ";
    cin.ignore();
    getline(cin, id);

    ifstream file(filename);
    string line;
    bool found = false;

    while (getline(file, line)) {
        string existingId;
        istringstream iss(line);
        if (iss >> existingId) {
            if (existingId == id) {
                string name, crime, sentence, dateOfCrime, address;
                int age;
                iss >> name >> age >> crime >> sentence >> dateOfCrime >> address;
                displayCriminalDetails(Criminal(existingId, name, age, crime, sentence, dateOfCrime, address));
                found = true;
                break;
            }
        }
    }

    file.close();

    if (!found) {
        cout << RED_COLOR << "Error: Criminal with ID " << id << " not found." << RESET_COLOR << endl;
    }
}


void searchCriminalByCrime() {
    string crime;
    cout << "Enter the crime you want to search: ";
    cin.ignore();
    getline(cin, crime);

    bool found = false;

    ifstream file(filename);
    string line;

    while (getline(file, line)) {
        istringstream iss(line);
        string currentId, name, currentCrime, sentence, dateOfCrime, address;
        int age;
        if (iss >> currentId >> name >> age >> currentCrime >> sentence >> dateOfCrime >> address) {
            if (currentCrime == crime) {
                displayCriminalDetails(Criminal(currentId, name, age, currentCrime, sentence, dateOfCrime, address));
                found = true;
            }
        }
    }

    file.close();

    if (!found) {
        cout << RED_COLOR << "No criminals found with the given crime." << RESET_COLOR << endl;
    }
}



    void displayCriminals() {
        cout << "--------------------------------------" << endl;
        cout << "Criminal Records" << endl;
        cout << "--------------------------------------" << endl;
        for (const auto& bucket : hashTable) {
            for (const auto& criminal : bucket) {
                displayCriminalDetails(criminal);
            }
        }
        cout << "--------------------------------------" << endl;
    }

private:
    int hashFunction(const string& id) {
        int sum = 0;
        for (char ch : id) {
            sum += ch;
        }
        return sum % size;
    }

    void displayCriminalDetails(const Criminal& criminal) {
        cout << "ID: " << criminal.id << endl;
        cout << "Name: " << criminal.name << endl;
        cout << "Age: " << criminal.age << endl;
        cout << "Crime: " << criminal.crime << endl;
        cout << "Sentence: " << criminal.sentence << endl;
        cout << "Date of Crime: " << criminal.dateOfCrime << endl;
        cout << "Address: " << criminal.address << endl;
        cout << "--------------------------------------" << endl;
    }
};

int main() {
    const string filename = "criminals.txt";
    const int size = 100;
    CriminalManagementSystem cms(filename, size);
    cms.loadCriminals();

    int choice;
    do {
        system("color F0");
        cout << BG_CYAN << "------------------------" << endl;
        cout << "| Criminal Record Management |" << endl;
        cout << "------------------------" << RESET_COLOR << endl;
        cout << FG_CYAN << "1. Add Criminal" << endl;
        cout << FG_RED << "2. Delete Criminal" << endl;
        cout << FG_YELLOW << "3. Modify Criminal" << endl;
        cout << FG_BLUE << "4. Search Criminal by ID" << endl;
        cout << FG_MAGENTA << "5. Search Criminal by Crime" << endl;
        cout << FG_GREEN << "6. Display All Criminals" << endl;
        cout << FG_YELLOW << "7. Save and Exit" << RESET_COLOR << endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                cout << FG_CYAN << "Adding Criminal..." << RESET_COLOR << endl;
                cms.addCriminal();
                break;
            case 2:
                cout << FG_RED << "Deleting Criminal..." << RESET_COLOR << endl;
                cms.deleteCriminal();
                break;
            case 3:
                cout << FG_YELLOW << "Modifying Criminal..." << RESET_COLOR << endl;
                cms.modifyCriminal();
                break;
            case 4:
                cout << FG_BLUE << "Searching Criminal by ID..." << RESET_COLOR << endl;
                cms.searchCriminalById();
                break;
            case 5:
                cout << FG_MAGENTA << "Searching Criminal by Crime..." << RESET_COLOR << endl;
                cms.searchCriminalByCrime();
                break;
            case 6:
                cout << FG_GREEN << "Displaying All Criminals..." << RESET_COLOR << endl;
                cms.displayCriminals();
                break;
            case 7:
                cout << FG_YELLOW << "Save and Exit..." << RESET_COLOR << endl;
                break;
            default:
                cout << FG_RED << "Invalid choice. Please try again." << RESET_COLOR << endl;
                break;
        }
    } while (choice != 7);

    cms.saveCriminals();

    return 0;
}
