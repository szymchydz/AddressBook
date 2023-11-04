#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <windows.h>


using namespace std;

struct Person {
    int id = 0;
    int userId = 0;
    string name = "", surname = "", phoneNumber = "", email = "", address = "";
};

struct User {
    int id = 0;
    string userName,  userPassword;
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

void writePersonToFile(const Person &person, ofstream &file) {
    file << person.id << "|";
    file << person.name << "|";
    file << person.surname << "|";
    file << person.phoneNumber << "|";
    file << person.email << "|";
    file << person.address << "|" << endl;
}

void writeUserToFile(const User &user, ofstream &file) {
    file << user.id << "|";
    file << user.userName << "|";
    file << user.userPassword << "|"<< endl;
}

void displaySingleContactDetails(const Person& person) {
    cout << "ID: " << person.id << endl;
    cout << person.name << " " << person.surname << endl;
    cout << "Telefon: " << person.phoneNumber << endl;
    cout << "Email: " << person.email << endl;
    cout << "Adres: " << person.address << endl;
    cout << endl;
}

void showContactsAddressData(vector <Person> &postalAddress) {
    for (Person &person : postalAddress) {
        displaySingleContactDetails(person);
    }
}

void displayAllContactsFromAddressBook(vector<Person> &postalAddress) {

    if (postalAddress.empty()) {
        cout << "Ksiazka adresowa jest pusta." << endl;
        system("pause");
    } else {
        system("cls");
        showContactsAddressData(postalAddress);
        system("pause");
    }
}

void loadSavedContactsFromAddressBook(vector<Person> &postalAddress) {

    string line{};
    fstream file;

    file.open("Ksiazka adresowa.txt", ios::in);

    if (!file.is_open()) {
        cout << "Plik nie istnieje." << endl;
        return;
    }

    Person currentPerson;

    int personNumber = 1;

    while (getline(file, line)) {
        istringstream iss(line);
        string personLineOfData;

        while (getline(iss, personLineOfData, '|')) {
            switch (personNumber) {
            case 1:
                currentPerson.id = stoi(personLineOfData);
                break;
            case 2:
                currentPerson.name = personLineOfData;
                break;
            case 3:
                currentPerson.surname = personLineOfData;
                break;
            case 4:
                currentPerson.phoneNumber = personLineOfData;
                break;
            case 5:
                currentPerson.email = personLineOfData;
                break;
            case 6:
                currentPerson.address = personLineOfData;
                break;
            }
            personNumber++;
        }

        postalAddress.push_back(currentPerson);

        personNumber = 1;
    }

    file.close();
}

void loadSavedUsersFromUserFile(vector<User> &currentUser) {

    string line{};
    fstream file;

    file.open("Uzytkownicy.txt", ios::in);

    if (!file.is_open()) {
        cout << "Plik nie istnieje." << endl;
        return;
    }

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
    ofstream file("Ksiazka adresowa.txt");

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        return;
    }

    for (const Person &person : postalAddress) {
        writePersonToFile(person, file);
    }
    file.close();
}

void rewriteVectorToUserFile(vector<User> &currentUser) {
    ofstream file("Uzytkownicy.txt");

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        return;
    }

    for (const User &user : currentUser) {
        writeUserToFile(user, file);
    }
    file.close();
}

void addNewPersonToTheFile(const Person &newPerson) {
    ofstream file("Ksiazka adresowa.txt", ios::app);

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        return;
    }

    writePersonToFile(newPerson, file);

    file.close();
}

void addNewUserToTheFile(const User &newUser) {
    ofstream file("Uzytkownicy.txt", ios::app);

    if (!file.is_open()) {
        cout << "Nie udalo sie otworzyc pliku i zapisac do niego danych." << endl;
        return;
    }

    writeUserToFile(newUser, file);

    file.close();
}

void addNewPersonToAddressBook(vector<Person> &postalAddress) {

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
    newPerson.id = postalAddress.empty() ? 1 : postalAddress.back().id + 1;
    newPerson.name = name;
    newPerson.surname = surname;
    newPerson.phoneNumber = phoneNumber;
    newPerson.email = email;
    newPerson.address = address;

    postalAddress.push_back(newPerson);

    addNewPersonToTheFile (newPerson);

}

bool isUserNameTaken(vector<User> &currentUser, string &userName) {
    for (const User &user : currentUser) {
        if (user.userName == userName) {
            cout << "Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika: ";
            return true;
        }
    }
    return false;
}

int userRegistration(vector<User> &currentUser, int currentUsersCount) {
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
    currentUsersCount++;

    addNewUserToTheFile(newUser);

    cout << "Konto zostalo zalozone";

    return currentUsersCount;
}

int userSignIn (vector <User> &currentUser, int currentUsersCount) {
    string userName, userPassword;

    cout << "Podaj nazwe uzytkownika: ";
    userName = readLine();
    int i = 0;

    while (i < currentUsersCount) {
        if (currentUser[i].userName == userName) {
            for (int attempts = 0; attempts < 3; attempts++) {
                cout << "Podaj haslo uzytkownika.Pozostalo prob " << 3 - attempts <<": ";
                userPassword = readLine();
                if (currentUser[i].userPassword == userPassword) {
                    cout << "Zalogowales sie.";
                    Sleep(1000);
                    return currentUser[i].id;
                }
            }
            cout << "Odczekaj 3 sekundy na odblokowanie." << endl;
            cout << "3s" << endl;
            Sleep(1000);
            cout << "2s" << endl;
            Sleep(1000);
            cout << "1s" << endl;
            Sleep(1000);
            return 0;
        }
        i++;
    }
    cout << "Nie ma uzytkownika o takiej nazwie" << endl;
    Sleep(1500);
    return 0;
}

void searchPersonByName(vector<Person> &postalAddress) {

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

    for (Person &person : postalAddress) {
        if (person.name == nameToSearch) {
            displaySingleContactDetails(person);
            found = true;
            system("pause");
        }
    }

    if (!found) {
        cout << "Brak adresata o tym imieniu w ksiazce adresowej." << endl;
        system("pause");
    }
}

void searchPersonBySurname( vector<Person> &postalAddress) {

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

    for (Person &person : postalAddress) {
        if (person.surname == surnameToSearch) {
            displaySingleContactDetails(person);
            found = true;
            system("pause");
        }
    }

    if (!found) {
        cout << "Brak adresata o tym nazwisku w ksiazce adresowej." << endl;
        system("pause");
    }
}

void removePersonFromAddressBook(vector<Person> &postalAddress) {

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
        if (idOfPersonToRemove == it -> id) {
            found = true;

            cout << "Usuwasz Osobe:" << endl;
            cout << "ID:                    " << it->id << endl;
            cout << "Imie:                  " << it->name << endl;
            cout << "Nazwisko:              " << it->surname << endl;
            cout << "Numer telefonu:        " << it->phoneNumber << endl;
            cout << "Adres email:           " << it->email << endl;
            cout << "Adres zamieszkania:    " << it->address << endl;
            cout << endl << "Czy chcesz usunac tego adresata? t/n: ";
            choice = getSymbol();

            if (choice == 't') {
                it = postalAddress.erase(it);
                cout << "Adresata usunieto" << endl;
                rewriteVectorToFile(postalAddress);
                system("pause");


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

void editPersonDataInAddressBook (vector <Person> &postalAddress) {
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
        if (person.id == idOfPersonToEdit) {
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
    }
    rewriteVectorToFile(postalAddress);
}

void passwordChange (vector <User> &currentUser, int currentUsersCount, int loggedUserId) {

    string userNewPassword;

    cout << "Podaj nowe userPassword: ";
    cin >> userNewPassword;

    for ( User &user : currentUser) {
        if (user.id == loggedUserId) {
            user.userPassword = userNewPassword;
            rewriteVectorToUserFile(currentUser);
            cout << "Haslo zostalo zmienione.";
            Sleep(1500);
            return;
        }
    }
}

int chooseOptionFromMainMenu () {

    system ("cls");
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

int chooseOptionFromUserMenu () {

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

    loadSavedUsersFromUserFile(currentUser);

    int loggedUserId = 0;
    int currentUsersCount = currentUser.size();
    char choice;

    while (true) {

        if (loggedUserId == 0) {

            choice = chooseOptionFromMainMenu ();

            switch (choice) {
            case '1':
                currentUsersCount = userRegistration (currentUser,currentUsersCount);
                break;
            case '2':
                loggedUserId = userSignIn (currentUser, currentUsersCount);
                break;
            case '9':
                exit(0);
            default :
                cout << endl << "Nie ma takiej opcji w menu" << endl;
                system ("pause");
                break;
            }
        } else {
            loadSavedContactsFromAddressBook(postalAddress);

            choice = chooseOptionFromUserMenu();

            switch (choice) {
            case '1':
                addNewPersonToAddressBook(postalAddress);
                break;
            case '2':
                searchPersonByName(postalAddress);
                break;
            case '3':
                searchPersonBySurname(postalAddress);
                break;
            case '4':
                displayAllContactsFromAddressBook(postalAddress);
                break;
            case '5':
                removePersonFromAddressBook(postalAddress);
                break;
            case '6':
                editPersonDataInAddressBook (postalAddress);
                break;
            case '7':
                passwordChange (currentUser, currentUsersCount, loggedUserId);
                break;
            case '9':
                loggedUserId = 0;
                break;
            }
        }
    }
    return 0;
}



