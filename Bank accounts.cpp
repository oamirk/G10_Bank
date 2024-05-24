#include <iostream>
#include <cmath>
#include <string>

using namespace std;

class Customer{
    private:
    string fname{}, lname{};
    customer_type entity{};

    void set_entity_type(customer_type entity){
        this->entity = entity;
    }

    public:
    string name;
    int customer_id;

    Customer(){
        this->entity = customer_type::Individual;
    }

    void set_name(string name){
        this->name = name;
        cout << name << endl;
    }
    /*
    void check_info(){
        cout << "Customer\t: " << customers[customer_counter].name << endl;
        cout << "Customer id\t:" << customers[customer_counter].customer_id << endl;
    }
    */
    
};

class Debt{
    Customer debtee;
    double principal, time_elapsed, interest_rate;
    string collateral;

    double value = principal* pow((1+ interest_rate/100), time_elapsed);

};

class Account: public Customer{
    public:
    Customer owner;
    int account_no, daily_transactions{};
    double balance{};

    virtual bool withdraw(double amount) = 0;
    
    bool deposit(double amount){
        if (amount > 0){
            balance += amount;
            cout << "Deposited " << amount << " NGN to " << owner.name << "." << endl;
            return true;
        }
        else{
            cout << "Deposit failed! Invalid amount" << endl;
            return false;
        }
    }

    void balance_check(){
        cout << "Balance for " << owner.name << ": " << balance << "NGN" << endl;
    }

    friend bool transfer(Account &giver, Account &receiver, double amount);
};

class Current_account: public Account{
    public:
    Current_account(){} //empty default constructor
    
    Current_account(Customer owner){
        this->owner = owner;
        customer_no += 1;
    }

    Current_account(double amount){
        balance = amount;
    }

    Current_account(Customer owner, double amount){
        this->owner = owner;
        balance = amount;
    }

    void set_name(string account_name){}
    
    bool withdraw(double amount) override{
        if (amount < 200){
            cout << "Withdrawals must have a value of 200 NGN or higher" << endl;
            return false;
        }
        if (balance >= amount){
            balance -= amount;
            cout << "Withdrawn " << amount << " NGN from " << name << "." << endl;
            daily_transactions += 1;
            return true;
        }
        else{
            cout << "Insufficient balance" << endl;
            return false;
        }
    }
};

class Savings_account: public Account{
    public:
    int transaction_limit{};

    bool withdraw(double amount) override{
        if (amount < 200){
            cout << "Withdrawals must have a value of 200 NGN or higher" << endl;
            return false;
        }
        if (balance >= amount && daily_transactions <= transaction_limit){
            balance -= amount;
            cout << "Withdrawn " << amount << " NGN from " << name << "." << endl;
            daily_transactions += 1;
            return true;
        }

        else if (balance >= amount && daily_transactions <= 3){
            balance -= amount + 50; //transaction fee over daily limit for savings account
            cout << "Withdrawn " << amount << " NGN from " << name << "." << endl;
            daily_transactions += 1;
            return true;
        }

        else{
            cout << "Insufficient balance" << endl;
            return false;
        }
    }
};

void main_menu();
void new_vs_existing();
int get_id();
void new_account_menu(int customer_id);
void starting_deposit(int customer_id, Account *accounts);
void new_customer();
bool transfer(Account &giver, Account &receiver, double amount);

enum customer_type {Individual, Business};
int customer_no{}, curr_acc_no = 100, sav_acc_no = 200, current_customer{};
//Current account number start from 100, savings start from 200
int customer_counter{};
Customer customers[200];

Savings_account savings_accounts[50];
Current_account current_accounts[50]; 

Debt debts[50];

int main(){
    main_menu();
    return 0;
}


void main_menu(){
    int selection;
    cout << "Welcome to G10 Bank! Please select an option" << endl;
    cout << "\t\t(1)Open Account \t(2) Manage Customer Info \t (3) Manage Transaction" << endl;
    cin >> selection;

    switch(selection){
        case 1:{
            new_vs_existing();
            break;
        }
    };
}

void new_vs_existing(){
    int selection;
    cout << "Is this account for a new or existing customer?" << endl;
    cout << "\t\t(1) New Customer\t (2) Existing Customer" << endl;
    cin >> selection;

    switch(selection){
        case 1:{
            new_customer();
            break;
        }
        
        case 2:{
            new_account_menu(get_id());
        }
    }
}

int get_id(){
    int id;

    cout << "Enter customer ID" << endl;
    cin >> id;

    return id;
}

void new_account_menu(int customer_id){
    int selection;
    cout << "Select account type:\n" << endl;
    cout << "\t\t(1) Current Account\t (2) Savings Account\t (3) Main Menu" << endl;
    cin >> selection;
    
    switch(selection){
        case 1:{
            curr_acc_no += 1;
            current_accounts[customer_id-1].account_no = curr_acc_no; //assign account number
            current_accounts[customer_id-1].owner = customers[current_customer]; //assign owner
            starting_deposit(customer_id, current_accounts);
            break;
        }

        case 2:{
            sav_acc_no += 1;
            current_accounts[customer_id-1].account_no = sav_acc_no; //assign account number
            current_accounts[customer_id-1].owner = customers[current_customer]; //assign owner
            starting_deposit(customer_id, current_accounts);
        }
    }
}

void starting_deposit(int customer_id, Account *account){
    int selection;
    cout << "Would you like to put in a starting deposit? (min value: 1000 NGN)\n" << endl;
    cout << "\t\t(1) Yes\t (2) No" << endl;
    cin >> selection;
                
    switch (selection){
        case 1:{
            double start_balance;
            cout << "Input starting deposit:" << endl;
            cin >> start_balance;

            if (start_balance >= 1000){
                (account+customer_id-1)->balance = start_balance;
                cout << "Opened a new account for " << (account+customer_id-1)->owner.name;
                cout << " with starting deposit of " << (account+customer_id-1)->balance << " NGN." << endl;
                cout << "Acct No: " << (account+customer_id-1)->account_no << endl;
                main_menu();
                break;
            }

            else{
                cout << "Invalid starting deposit. Setting starting deposit as 0 NGN." << endl;
            }
        }

        case 2:{
            (account+customer_id)->balance = 0;
            cout << "Opened a new account for " << (account+customer_id-1)->owner.name;
            cout << " with starting deposit of " << (account+customer_id-1)->balance << " NGN." << endl;
            break;
        }
                
        default:{
            cout << "Setting starting deposit as 0 NGN" << endl;
            (account+customer_id)->balance = 0;
            cout << "Opened a new account for " << (account+customer_id-1)->owner.name;
            cout << " with starting deposit of " << (account+customer_id-1)->balance << " NGN." << endl;
        };
    }
}

void new_customer(){
    string fname;
    string lname;

    customer_counter += 1; //increment the number of active customers in the bank
    customers[customer_counter].customer_id = customer_counter; //set the customer id as latest customer
    current_customer = customers[customer_counter].customer_id; //set the customer for actions to be taken on

    cout << "Enter customer first name: "; cin >> fname;
    cout << "Enter customer last name: "; cin >> lname;

    customers[current_customer].set_name(fname + " " + lname); //set customer name

    cout << "Customer\t: " << customers[customer_counter].name << endl;
    cout << "Customer id\t: " << customers[customer_counter].customer_id << endl;
    //Display new customer info

    new_account_menu(current_customer); //create account for customer
}

bool transfer(Account &giver, Account &receiver, double amount){
    if (giver.withdraw(amount)){
        receiver.deposit(amount);
        cout << "Transferred " << amount << " NGN from ";
        cout << giver.owner.name << " to " << receiver.owner.name << endl;
        giver.balance_check();

        return true;
    }
    else{
        return false;
    }
}
