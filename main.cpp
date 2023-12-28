#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <windows.h>


using namespace std;

struct User {
    int id = 0;
    string userName,  userPassword;
};

struct Person {
    int id = 0;
    int userId = 0;
    string name = "", surname = "", phoneNumber = "", email = "", address = "";
};

string readLine() {

    string input;
    cin.sync();
    getline(cin, input);
    return input;
}

char getSymbol() {
    string input = "";
    char symbol = {0};

    while(true) {
        getline(cin, input);

        if(input.length() == 1) {
            symbol = input[0];
            break;
        }
    }
    return symbol;
}

int getInt() {

    int number = 0;
    string input;

    while (true) {

        input = readLine();

        stringstream myStream(input);
        if(myStream >> number) break;
    }
    return number;
}

bool doesUserExist(vector<User>& currentUser, int userId) {
    for (const User& user : currentUser) {
        if (user.id == userId) {
            return true;
        }
    }
    return false;
}

bool isUserNameTaken(vector<User> &currentUser, string &userName) {
    for (const User &user : currentUser) {
        if (user.userName == userName) {
            cout << "Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika: " << endl;
            cout << endl;
            return true;
        }
    }
    return false;
}

bool shouldRemovePerson(const Person &person, int idToRemove, int loggedUserId) {
    return person.id == idToRemove && person.userId == loggedUserId;
}

void handleFileSwap (string oldFileName, string tempFileName) {

    if (remove("Ksiazka adresowa.txt") != 0) {
        cout<< "Nie mozna usunac pliku" << endl;
        return;
    }

    if (rename("Adresaci_tymczasowy.txt", "Ksiazka adresowa.txt") != 0) {
        cout << "Nie mozna zamienic pliku." << endl;
        return;
    }

}

void deleteLineFromAddressBook(int loggedUserId) {

    string line, field, oldFileName = "Ksiazka adresowa.txt", tempFileName = "Adresaci_tymczasowy.txt";
    ifstream file(oldFileName);
    ofstream tempFile(tempFileName);

    while (getline(file, line)) {

        istringstream iss(line);

        getline (iss, field, '|');

        if (stoi(field) != loggedUserId) {
            tempFile << line << endl;
        }

    }

    file.close();
    tempFile.close();

    handleFileSwap(oldFileName, tempFileName);

}

void writePersonToFile(const Person &person, ofstream &file) {
    file << person.id << '|';
    file << person.userId << '|';
    file << person.name << '|';
    file << person.surname << '|';
    file << person.phoneNumber << '|';
    file << person.email << '|';
    file << person.address << '|' << endl;
}

void writeUserToFile(const User &user, ofstream &file) {
    file << user.id << '|';
    file << user.userName << '|';
    file << user.userPassword << '|'<< endl;
}

void displaySingleContactDetails(const Person& person) {
    cout << "ID Adresata: " << person.id << endl;
    cout << "ID Uzytkownika: " << person.userId << endl;
    cout << "Imie: " << person.name << endl;
    cout << "Nazwisko: " << person.surname << endl;
    cout << "Telefon: " << person.phoneNumber << endl;
    cout << "Email: " << person.email << endl;
    cout << "Adres: " << person.address << endl;
    cout << endl;
}

void showContactsAddressData(vector <Person> &postalAddress, int loggedUserId) {

    for (Person &person : postalAddress) {
        if (person.userId == loggedUserId) {
            displaySingleContactDetails(person);
        }
    }
}

void displayAllContactsFromAddressBook(vector<Person> &postalAddress, int loggedUserId) {

    if (postalAddress.empty()) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        system("pause");
    } else {
        system("cls");
        showContactsAddressData(postalAddress, loggedUserId);
        system("pause");
    }
}

int loadSavedContactsFromAddressBook(vector<Person> &postalAddress) {

    string line{};
    fstream file;

    file.open("Ksiazka adresowa.txt", ios::in);

    if (!file.is_open()) {
        return 0;
    }

    while (getline(file, line)) {
        istringstream iss(line);
        string personLineOfData;
        int personNumber = 1;

        Person person;

        while (getline(iss, personLineOfData, '|')) {
            switch (personNumber) {
            case 1:
                person.id = stoi(personLineOfData);
                break;
            case 2:
                person.userId = stoi(personLineOfData);
                break;
            case 3:
                person.name = personLineOfData;
                break;
            case 4:
                person.surname = personLineOfData;
                break;
            case 5:
                person.phoneNumber = personLineOfData;
                break;
            case 6:
                person.email = personLineOfData;
                break;
            case 7:
                person.address = personLineOfData;
                break;
            }
            personNumber++;
        }

        postalAddress.push_back(person);
    }

    file.close();

    return postalAddress.back().id;
}

void loadSavedUsersFromUserFile(vector<User> &currentUser) {

    string line{};
    fstream file;

    file.open("Uzytkownicy.txt", ios::in | ios::app);

    User user;

    int userNumber = 1;

    while (getline(file, line)) {
        istringstream iss(line);
        string userLineOfData;

        while (getline(iss, userLineOfData, '|')) {
            switch (userNumber) {
            case 1:
                user.id = stoi(userLineOfData);
                break;
            case 2:
                user.userName = userLineOfData;
                break;
            case 3:
                user.userPassword = userLineOfData;
                break;
            }
            userNumber++;
        }

        currentUser.push_back(user);

        userNumber = 1;
    }

    file.close();
}

void rewriteVectorToFile(vector<Person> &postalAddress) {
    ofstream file;
    string lineWithPersonData;

    file.open("Ksiazka adresowa.txt", ios::out | ios::trunc);
    if (file.good() == true) {
        for (Person &person : postalAddress) {
            writePersonToFile(person, file);
            file << lineWithPersonData;
        }

        file.close();
        cout << "Dane zostaly zapisane." << endl;
        system ("pause");
    } else {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system ("pause");
    }
}

void rewriteVectorToUserFile(vector<User> &currentUser) {
    ofstream file("Uzytkownicy.txt");

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system ("pause");
        return;
    }

    for (const User &user : currentUser) {
        writeUserToFile(user, file);
    }
    file.close();
}

void addNewPersonToTheFile(const Person& newPerson) {
    ofstream file("Ksiazka adresowa.txt", ios::app);

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system ("pause");
        return;
    }

    writePersonToFile(newPerson, file);

    file.close();
}

void addNewUserToTheFile(const User &newUser) {
    ofstream file("Uzytkownicy.txt", ios::app);

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        system ("pause");
        return;
    }

    writeUserToFile(newUser, file);

    file.close();
}

void addNewPersonToAddressBook(vector<Person> &postalAddress, int loggedUserId, int personId) {

    string name, surname, phoneNumber, email, address;

    cout << "Podaj imie: ";
    name = readLine();
    cout << "Podaj nazwisko: ";
    surname = readLine();
    cout << "Podaj numer telefonu: ";
    phoneNumber = readLine();
    cout << "Podaj email: ";
    email = readLine();
    cout << "Podaj adres: ";
    address = readLine();

    Person newPerson;
    newPerson.id = personId + 1;
    newPerson.userId = loggedUserId;
    newPerson.name = name;
    newPerson.surname = surname;
    newPerson.phoneNumber = phoneNumber;
    newPerson.email = email;
    newPerson.address = address;

    postalAddress.push_back(newPerson);

    addNewPersonToTheFile (newPerson);

    cout << endl << "Adresat zostal dodany do ksiazki adresowej." << endl;
    system ("pause");
}

void userRegistration(vector<User> &currentUser) {

    string userName, userPassword;

    do {
        cout << "Podaj nazwe uzytkownika: ";
        userName = readLine();
    }

    while (isUserNameTaken(currentUser, userName));

    cout << "Podaj haslo: ";
    userPassword = readLine();

    User newUser;
    newUser.userName = userName;
    newUser.userPassword = userPassword;
    newUser.id = currentUser.empty() ? 1 : currentUser.back().id + 1;

    currentUser.push_back(newUser);

    addNewUserToTheFile(newUser);

    cout << "Konto zostalo zalozone" << endl;
    system("pause");

}

int userSignIn (vector <User> &currentUser) {
    string userName, userPassword;

    ifstream file("Uzytkownicy.txt");

    if (!file.is_open()) {
        cout << "Plik nie istnieje: Uzytkownicy.txt" << endl;
        system("pause");
        return 0;
    }

    cout << "Podaj nazwe uzytkownika: ";
    userName = readLine();
    unsigned int i = 0;

    while (i < currentUser.size()) {
        if (currentUser[i].userName == userName) {
            for (int attempts = 0; attempts < 3; attempts++) {
                cout << "Podaj haslo uzytkownika.Pozostalo prob " << 3 - attempts <<": ";
                userPassword = readLine();
                if (currentUser[i].userPassword == userPassword) {
                    cout << "Zalogowales sie." << endl;
                    system("pause");
                    return currentUser[i].id;
                }
            }
            cout << "Bledne logowanie." << endl;
            cout << "Odczekaj 3 sekundy na odblokowanie." << endl;
            for (int countdown = 3; countdown > 0; countdown--) {
                cout << countdown << "s" << endl;
                Sleep(1000);
            }
            return 0;
        }
        i++;
    }
    cout << "Nie ma uzytkownika o takiej nazwie" << endl;
    system("pause");

    return 0;
}

void userPasswordChange(vector<User>& currentUser, int loggedUserId) {
    string userNewPassword;

    cout << "Podaj nowe haslo: ";
    cin >> userNewPassword;

    for (User& user : currentUser) {
        if (user.id == loggedUserId) {
            user.userPassword = userNewPassword;
            rewriteVectorToUserFile(currentUser);
            cout <<"Haslo zostalo zmienione." << endl;
            system("pause");
            return;
        }
    }
}

void searchPersonByName(vector<Person> &postalAddress, int loggedUserId) {

    if (postalAddress.empty()) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        system("pause");
        return;
    }

    system("cls");
    string nameToSearch;
    cout << "Wpisz imie adresata: ";
    nameToSearch = readLine();

    bool found = false;

    system("cls");
    for (const Person &person : postalAddress) {
        if (person.name == nameToSearch && person.userId == loggedUserId) {
            displaySingleContactDetails(person);
            found = true;
        }
    }

    if (!found) {
        cout << "Brak adresata o tym imieniu w ksiazce adresowej." << endl;
    }

    system("pause");
}

void searchPersonBySurname( vector<Person> &postalAddress, int loggedUserId) {

    if (postalAddress.empty()) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        system("pause");
        return;
    }

    system("cls");
    string surnameToSearch;
    cout << "Wpisz nazwisko adresata: ";
    surnameToSearch = readLine();
    bool found = false;

    for (const Person &person : postalAddress) {
        if (person.surname == surnameToSearch && person.userId == loggedUserId) {
            displaySingleContactDetails(person);
            found = true;
        }
    }

    if (!found) {
        cout << "Brak adresata o tym nazwisku w ksiazce adresowej." << endl;
    }
    system("pause");
}

void removePersonFromAddressBook(vector<Person> &postalAddress, int loggedUserId) {

    if (postalAddress.empty()) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        system("pause");
        return;
    }

    int idOfPersonToRemove;
    char choice;
    bool found = false;


    cout << "Podaj id adresata do usuniecia: ";
    idOfPersonToRemove = getInt();

    auto it = postalAddress.begin();

    while (it != postalAddress.end()) {
        while (it != postalAddress.end()) {
            if (shouldRemovePerson(*it, idOfPersonToRemove, loggedUserId)) {
                found = true;

                cout << "Usuwasz Osobe:" << endl;
                displaySingleContactDetails(*it);
                cout << endl << "Czy chcesz usunac tego adresata? t/n: ";
                choice = getSymbol();

                if (choice == 't') {
                    it = postalAddress.erase(it);
                    cout << "Adresata usunieto" << endl;
                    Sleep(1500);
                    deleteLineFromAddressBook(loggedUserId);
                    rewriteVectorToFile(postalAddress);

                } else {
                    cout << "Bledne potwierdzenie. Nie usunieto adresata" << endl;
                    system("pause");
                    return;
                }
            } else {
                ++it;
            }
        }

        if (!found) {
            cout << "Brak adresata o tym ID w ksiazce adresowej" << endl;
            system("pause");
        }
    }
}

void editPersonDataInAddressBook (vector <Person> &postalAddress, int loggedUserId) {
    int idOfPersonToEdit;
    char choice;
    bool found = false;

    if (postalAddress.empty()) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        system("pause");
        return;
    }

    system("cls");

    string name, surname, phoneNumber, email, address;

    cout << "*****EDYCJA ADRESATA*****" << endl;
    cout << endl;
    cout << "Podaj Id adresata do edycji : ";
    idOfPersonToEdit = getInt();
    system("cls");

    for (Person &person : postalAddress) {
        if (person.id == idOfPersonToEdit && person.userId == loggedUserId) {
            found = true;

            cout << "Dane do edycji" << endl;
            cout << "1. Imie" << endl;
            cout << "2. Nazwisko" << endl;
            cout << "3. Numer telefonu" << endl;
            cout << "4. Adres mail" << endl;
            cout << "5. Adres zamieszkania" << endl;
            cout << "6. Powrot do menu glownego" << endl;
            cout << "Twoj wybor: " << endl;
            choice = getSymbol();

            switch (choice) {
            case '1':
                cout << "Edytuj imie" << endl;
                person.name = readLine();

                break;
            case '2':
                cout << "Edytuj nazwisko" << endl;
                person.surname = readLine();

                break;
            case '3':
                cout << "Edytuj numer telefon" << endl;
                person.phoneNumber = readLine();
                break;
            case '4':
                cout << "Edytuj adres email" << endl;
                person.email = readLine();
                break;
            case '5':
                cout << "Edytuj adres zamieszkania" << endl;
                person.address = readLine();
                break;
            case '6':
                return;
            }
        }
    }

    if(!found) {
        cout << "Brak adresata o tym ID w ksiazce adresowej" << endl;
        system("pause");
        return;
    }

    rewriteVectorToFile(postalAddress);
}

char chooseOptionFromMainMenu() {
    system("cls");
    cout << "   >>> MENU GLOWNE <<< " << endl;
    cout << "........................" << endl;
    cout << "1. Rejestracja" << endl;
    cout << "2. Logowanie" << endl;
    cout << "9. Zakoncz program" << endl;
    cout << "........................" << endl;
    cout << "Twoj wybor: ";
    char choice;
    choice = getSymbol();

    return choice;
}

char chooseOptionFromUserMenu () {

    system("cls");
    cout << "***** KSIAZKA ADRESOWA *****" << endl;
    cout << endl;
    cout << "1. Dodaj adresata" << endl;
    cout << "2. Wyszukaj adresata po imieniu" << endl;
    cout << "3. Wyszukaj adresata po nazwisku" << endl;
    cout << "4. Wyswietl wszystkich adresatow" << endl;
    cout << "5. Usun adresata" << endl;
    cout << "6. Edytuj adresata" << endl;
    cout << "7. Zmien haslo" << endl;
    cout << "9. Wyloguj sie" << endl;
    cout << endl;
    cout << "Twoj wybor: ";
    char choice = getSymbol();

    return choice;
}

int main() {
    vector<Person> postalAddress;
    vector<User> currentUser;

    int loggedUserId = 0;

    loadSavedUsersFromUserFile(currentUser);

    char choice;

    while (true) {
        if (loggedUserId == 0) {
            choice = chooseOptionFromMainMenu();

            switch (choice) {
            case '1':
                userRegistration(currentUser);
                break;
            case '2':
                loggedUserId = userSignIn(currentUser);
                break;
            case '9':
                exit(0);
            default:
                cout << endl << "Nie ma takiej opcji w menu" << endl;
                system("pause");
                break;
            }
        } else {

            if (!doesUserExist(currentUser, loggedUserId)) {
                cout << "Uzytkownik o podanym ID nie istnieje." << endl;
                loggedUserId = 0;
            } else {

                postalAddress.clear();
                int lastPersonId = loadSavedContactsFromAddressBook(postalAddress);

                choice = chooseOptionFromUserMenu();

                switch (choice) {
                case '1':
                    addNewPersonToAddressBook(postalAddress, loggedUserId, lastPersonId);
                    break;
                case '2':
                    searchPersonByName(postalAddress, loggedUserId);
                    break;
                case '3':
                    searchPersonBySurname(postalAddress, loggedUserId);
                    break;
                case '4':
                    displayAllContactsFromAddressBook(postalAddress, loggedUserId);
                    break;
                case '5':
                    removePersonFromAddressBook(postalAddress, loggedUserId);
                    break;
                case '6':
                    editPersonDataInAddressBook(postalAddress, loggedUserId);
                    break;
                case '7':
                    userPasswordChange(currentUser, loggedUserId);
                    break;
                case '9':
                    loggedUserId = 0;
                    break;
                }
            }
        }
    }
    return 0;
}

