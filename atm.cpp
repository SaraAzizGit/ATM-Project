/* ATM Interface:
The ATMs in our cities are built on C/C++, as we have all seen them. It is a console-based
application with five different classes. In order to use the system, the user must enter his or her
user ID and pin when it starts. Once the details are entered successfully, ATM functionality is
unlocked. As a result of the project, the following operations can be performed:
TRANSACTIONS HISTORY
WITHDRAW 
DEPOSIT 
TRANSFER 
QUIT */

# include <iostream>
# include <vector>
# include <string>
# include <iomanip>
# include <cmath>

using namespace std;

// class to represent a user's account
class Account {
    private:
        string userName;
        int userID;
        int pin;
        double balance;
        vector<string> transactionHistory; // just like arrays but resize themselves as we push elements into them

    public:
        Account() {}

        Account(string UserName, int UserId, int Pin) {
            userName = UserName;
            userID = UserId;
            pin = Pin;
            balance = 0.0;
        }

        string getUserName() {
            return userName;
        }

        int getUserID() {
            return userID;
        }

        bool validatePin(int Pin) {
            return pin == Pin;
        }

        double getBalance() {
            return balance;
        }

        void getTransactionHistory() {
            cout << "* Transactions History *" << endl << endl;
            cout << fixed << setprecision(2);
            
            for (string action: transactionHistory) { // syntax: for (vectorType elementVariable: vectorName)
                // for converting e.g. 30.000000 to 30.00
                int position = action.find("."); // finding "." in the string (of Rs.)
                string amountString = action.substr(position + 1); // holding the substring after "." (of Rs.)
                int amountInt = stoi(amountString); // convert it to int
                int roundAmount = round((amountInt * 100)) / 100; // round up to two decimal places

                cout << "* " << action.substr(0, position + 2) << roundAmount << ".00" << endl;
            }
        }

        bool withdraw(double amount) {
            cout << fixed << setprecision(2);

            if (amount > 0 && amount <= balance) {
                balance -= amount;
                transactionHistory.push_back("Withdraw: Rs. " + to_string(amount)); // method to push element at the end of vector
                return true;
            } else if (amount > balance) {
                cout << "Your account doesn't have enough balance" << endl;
                return false;
            } else {
                cout << "Please enter a valid amount to withdraw" << endl;
                return false;
            }
        }

        bool deposit(double amount) {
            cout << fixed << setprecision(2);

            if (amount > 0) {
                balance += amount;
                transactionHistory.push_back("Deposit: Rs. " + to_string(amount));
                return true;
            } else {
                cout << "Please enter a valid amount to deposit" << endl;
                return false;
            }
        }

        void transfer(Account& targetAccount, double amount) { // passing by reference so changes are recorded
            cout << fixed << setprecision(2);

            if (amount > 0 && amount <= balance) {
                balance -= amount;
                targetAccount.deposit(amount); // calling deposit method on target account

                cout << "* Transfer Successful! *" << endl << endl << "You have made a funds transfer of Rs. " << amount << " to " << targetAccount.userName << endl;

                transactionHistory.push_back("Transfer to " + targetAccount.userName + ": Rs. " + to_string(amount));
            } else if (amount > balance) {
                cout << "Your account doesn't have enough balance" << endl;
                cout << "* Transfer Unsuccessful! *" << endl;
            } else {
                cout << "Please enter a valid amount to transfer" << endl;
                cout << "* Transfer Unsuccessful! *" << endl;
            }
        }
};

// class to represent the ATM
class ATM {
    private:
        vector<Account> accounts; // contains all bank accounts made

    public:
        void addUser(string userName, int userID, int pin) {
            accounts.push_back(Account(userName, userID, pin));
        }

        Account* findAccount(int userID) { // method to find an account and return its address
            for (Account& account: accounts) { // iterate through accounts vector
                if (account.getUserID() == userID) {
                    return &account;
                }
            }

            cout << "* Account not found *" << endl;
            return nullptr; // represents a null pointer value
        }
};

int main() {
    ATM atm;
    int loginOrQuit;

    atm.addUser("Sara", 180903, 1234);
    atm.addUser("Sarim", 171102, 4321);

    do {
        cout << endl << "                               A T M                               " << endl;
        cout << "*******************************************************************" << endl;
        cout << "1. Login" << endl;
        cout << "2. Quit" << endl;
        cout << endl << "Enter 1 or 2: ";
        cin >> loginOrQuit;

        if (loginOrQuit == 2) {
            break;
        }

        int enteredUserID, enteredPin;
        cout << endl << "*** Login ***" << endl << "Enter User ID: ";
        cin >> enteredUserID;
        cout << "Enter Pin: ";
        cin >> enteredPin;
        cout << endl;

        cin.ignore(); // to clear the input stream (used here because of endl)

        Account* userAccount = atm.findAccount(enteredUserID);

        if (userAccount != nullptr && userAccount->validatePin(enteredPin)) {
            // User is authenticated
            cout << "Welcome, " << userAccount->getUserName() << "!" << endl;

            int operation;
            char choice;
            double amount;

            do {
                cout << endl << "Which operation do you want to perform?" << endl
                    << "1. View Transactions History" << endl
                    << "2. Withdraw" << endl
                    << "3. Deposit" << endl
                    << "4. Transfer" << endl
                    << "5. Show Current Balance" << endl
                    << "6. Quit" << endl;

                cout << endl << "Enter choice (1-6): ";
                cin >> operation;
                cout << endl;

                if (operation == 6) {
                    break; // control returns to outerloop 
                }

                switch (operation)
                {
                    case 1: {
                        cout << "Show Transactions History? (Enter Y/N): ";
                        cin >> choice;
                        cout << endl;

                        if (choice == 'y' || choice == 'Y') {
                            userAccount->getTransactionHistory();
                        }

                        break;
                    }

                    case 2: {
                        cout << "Enter amount to Withdraw: Rs. ";
                        cin >> amount;

                        cout << endl << "Withdraw Rs. " << amount << "? (Enter Y/N): ";
                        cin >> choice;
                        cout << endl;

                        if (choice == 'y' || choice == 'Y') {
                            if (userAccount->withdraw(amount) == true) {
                                cout << "* Withdraw Successful! *" << endl << endl << "Your account has been debited with Rs. " << amount << endl;
                            } else {
                                cout << "* Withdraw Unsuccessful! *" << endl;
                            }
                        }

                        break;
                    }

                    case 3: {
                        cout << "Enter amount to Deposit: Rs. ";
                        cin >> amount;

                        cout << endl << "Deposit Rs. " << amount << "? (Enter Y/N): ";
                        cin >> choice;
                        cout << endl;

                        if (choice == 'y' || choice == 'Y') {
                            if (userAccount->deposit(amount) == true) {
                                cout << "* Deposit Successful! *" << endl << endl << "Your account has been credited with Rs. " << amount << endl;
                            } else {
                                cout << "* Deposit Unsuccessful! *" << endl;
                            }
                        }

                        break;
                    }

                    case 4: {
                        int targetUserID;
                        cout << "Enter User ID of Target Account: ";
                        cin >> targetUserID;

                        Account* targetAccount = atm.findAccount(targetUserID);

                        if (targetAccount != nullptr) {
                            cout << "Enter amount to Transfer: Rs. ";
                            cin >> amount;

                            cout << endl << "Transfer Rs. " << amount << " to " << targetAccount->getUserName() << "? (Enter Y/N): ";
                            cin >> choice;
                            cout << endl;
                            
                            if (choice == 'y' || choice == 'Y') {
                                userAccount->transfer(*targetAccount, amount);
                            }
                        }

                        break;
                    }

                    case 5: {
                        cout << "Show Current Balance? (Enter Y/N): ";
                        cin >> choice;
                        cout << endl;

                        cout << fixed << setprecision(2) << "* Current Balance: Rs. " << userAccount->getBalance() << " *" << endl;

                        break;
                    }

                    default: {
                        break;
                    }
                }
            } while (operation != 6);

        } else {
            cout << "Access Denied - Invalid UserID or PIN" << endl;
        }

    } while (loginOrQuit == 1);
    
    return 0;
}