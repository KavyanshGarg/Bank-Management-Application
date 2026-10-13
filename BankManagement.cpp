#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <string>
#include <iomanip>

using namespace std;

const string FILE_NAME = "accounts.txt";
const char SEPARATOR = '|';

// ===================== Bank Account Class =====================

class BankAccount
{
private:
    int accountNumber;
    string customerName;
    string accountType;
    double balance;

public:

    // Constructor
    BankAccount()
    {
        accountNumber = 0;
        balance = 0.0;
    }

    // Set account details
    void createAccount()
    {
        cout << "\n========== CREATE ACCOUNT ==========\n";

        cout << "Enter Account Number: ";
        cin >> accountNumber;
        cin.ignore();

        cout << "Enter Customer Name: ";
        getline(cin, customerName);

        cout << "Enter Account Type (Savings/Current): ";
        getline(cin, accountType);

        do
        {
            cout << "Enter Initial Deposit: ";
            cin >> balance;

            if (balance < 0)
            {
                cout << "Initial deposit cannot be negative.\n";
            }

        } while (balance < 0);

        cout << "\nAccount created successfully.\n";
    }

    // Display account
    void display() const
    {
        cout << "\n----------------------------------------\n";
        cout << "Account Number : " << accountNumber << '\n';
        cout << "Customer Name  : " << customerName << '\n';
        cout << "Account Type   : " << accountType << '\n';
        cout << fixed << setprecision(2);
        cout << "Balance        : " << balance << '\n';
        cout << "----------------------------------------\n";
    }

    // Deposit money
    bool deposit(double amount)
    {
        if (amount <= 0)
        {
            return false;
        }

        balance += amount;
        return true;
    }

    // Withdraw money
    bool withdraw(double amount)
    {
        if (amount <= 0 || amount > balance)
        {
            return false;
        }

        balance -= amount;
        return true;
    }

    // Get account number
    int getAccountNumber() const
    {
        return accountNumber;
    }

    // Convert object into file format
    string toFileString() const
    {
        return to_string(accountNumber) + SEPARATOR +
               customerName + SEPARATOR +
               accountType + SEPARATOR +
               to_string(balance);
    }

    // Create object from file data
    bool fromFileString(const string& line)
    {
        stringstream data(line);
        string value;

        if (!getline(data, value, SEPARATOR))
            return false;

        accountNumber = stoi(value);

        if (!getline(data, customerName, SEPARATOR))
            return false;

        if (!getline(data, accountType, SEPARATOR))
            return false;

        if (!getline(data, value, SEPARATOR))
            return false;

        balance = stod(value);

        return true;
    }
};

// ===================== File Functions =====================

vector<BankAccount> loadAccounts()
{
    vector<BankAccount> accounts;

    ifstream file(FILE_NAME);

    if (!file)
    {
        return accounts;
    }

    string line;

    while (getline(file, line))
    {
        BankAccount account;

        if (account.fromFileString(line))
        {
            accounts.push_back(account);
        }
    }

    file.close();

    return accounts;
}

void saveAccounts(const vector<BankAccount>& accounts)
{
    ofstream file(FILE_NAME);

    for (const BankAccount& account : accounts)
    {
        file << account.toFileString() << '\n';
    }

    file.close();
}

// ===================== Search Function =====================

int findAccount(const vector<BankAccount>& accounts, int accountNumber)
{
    for (int i = 0; i < static_cast<int>(accounts.size()); i++)
    {
        if (accounts[i].getAccountNumber() == accountNumber)
        {
            return i;
        }
    }

    return -1;
}

// ===================== Create New Account =====================

void addAccount()
{
    vector<BankAccount> accounts = loadAccounts();

    BankAccount newAccount;
    newAccount.createAccount();

    if (findAccount(accounts, newAccount.getAccountNumber()) != -1)
    {
        cout << "\nAccount number already exists.\n";
        return;
    }

    accounts.push_back(newAccount);

    saveAccounts(accounts);

    cout << "Account saved successfully.\n";
}

// ===================== Display All Accounts =====================

void showAllAccounts()
{
    vector<BankAccount> accounts = loadAccounts();

    if (accounts.empty())
    {
        cout << "\nNo bank accounts found.\n";
        return;
    }

    cout << "\n========== ALL ACCOUNTS ==========\n";

    for (int i = 0; i < static_cast<int>(accounts.size()); i++)
    {
        cout << "\nAccount " << i + 1;
        accounts[i].display();
    }
}

// ===================== Deposit =====================

void depositMoney()
{
    vector<BankAccount> accounts = loadAccounts();

    if (accounts.empty())
    {
        cout << "\nNo accounts available.\n";
        return;
    }

    int accountNumber;

    cout << "\n========== DEPOSIT MONEY ==========\n";
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1)
    {
        cout << "\nAccount not found.\n";
        return;
    }

    double amount;

    cout << "Enter Deposit Amount: ";
    cin >> amount;

    if (accounts[index].deposit(amount))
    {
        saveAccounts(accounts);
        cout << "\nDeposit successful.\n";
    }
    else
    {
        cout << "\nInvalid deposit amount.\n";
    }
}

// ===================== Withdraw =====================

void withdrawMoney()
{
    vector<BankAccount> accounts = loadAccounts();

    if (accounts.empty())
    {
        cout << "\nNo accounts available.\n";
        return;
    }

    int accountNumber;

    cout << "\n========== WITHDRAW MONEY ==========\n";
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1)
    {
        cout << "\nAccount not found.\n";
        return;
    }

    double amount;

    cout << "Enter Withdrawal Amount: ";
    cin >> amount;

    if (accounts[index].withdraw(amount))
    {
        saveAccounts(accounts);
        cout << "\nWithdrawal successful.\n";
    }
    else
    {
        cout << "\nInvalid amount or insufficient balance.\n";
    }
}

// ===================== Balance Inquiry =====================

void checkBalance()
{
    vector<BankAccount> accounts = loadAccounts();

    if (accounts.empty())
    {
        cout << "\nNo accounts available.\n";
        return;
    }

    int accountNumber;

    cout << "\n========== BALANCE INQUIRY ==========\n";
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1)
    {
        cout << "\nAccount not found.\n";
        return;
    }

    accounts[index].display();
}

// ===================== Search Account =====================

void searchAccount()
{
    vector<BankAccount> accounts = loadAccounts();

    if (accounts.empty())
    {
        cout << "\nNo accounts available.\n";
        return;
    }

    int accountNumber;

    cout << "\n========== SEARCH ACCOUNT ==========\n";
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1)
    {
        cout << "\nAccount not found.\n";
        return;
    }

    accounts[index].display();
}

// ===================== Delete Account =====================

void deleteAccount()
{
    vector<BankAccount> accounts = loadAccounts();

    if (accounts.empty())
    {
        cout << "\nNo accounts available.\n";
        return;
    }

    int accountNumber;

    cout << "\n========== DELETE ACCOUNT ==========\n";
    cout << "Enter Account Number: ";
    cin >> accountNumber;

    int index = findAccount(accounts, accountNumber);

    if (index == -1)
    {
        cout << "\nAccount not found.\n";
        return;
    }

    char choice;

    cout << "\nAre you sure you want to delete this account? (Y/N): ";
    cin >> choice;

    if (choice != 'Y' && choice != 'y')
    {
        cout << "\nDeletion cancelled.\n";
        return;
    }

    accounts.erase(accounts.begin() + index);

    saveAccounts(accounts);

    cout << "\nAccount deleted successfully.\n";
}

// ===================== Main Menu =====================

void showMenu()
{
    cout << "\n\n";
    cout << "============================================\n";
    cout << "          BANK MANAGEMENT SYSTEM\n";
    cout << "============================================\n";
    cout << "1. Create Account\n";
    cout << "2. Display All Accounts\n";
    cout << "3. Deposit Money\n";
    cout << "4. Withdraw Money\n";
    cout << "5. Check Balance\n";
    cout << "6. Search Account\n";
    cout << "7. Delete Account\n";
    cout << "8. Exit\n";
    cout << "============================================\n";
}

// ===================== Main Function =====================

int main()
{
    int choice;

    do
    {
        showMenu();

        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                addAccount();
                break;

            case 2:
                showAllAccounts();
                break;

            case 3:
                depositMoney();
                break;

            case 4:
                withdrawMoney();
                break;

            case 5:
                checkBalance();
                break;

            case 6:
                searchAccount();
                break;

            case 7:
                deleteAccount();
                break;

            case 8:
                cout << "\nThank you for using the Bank Management System.\n";
                break;

            default:
                cout << "\nInvalid choice. Please select 1-8.\n";
        }

    } while (choice != 8);

    return 0;
}