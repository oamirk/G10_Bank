#include <iostream>
#include <cmath>
#include <string>

using namespace std;

enum customer_type {Individual, Business};
//Current account number start from 100, savings start from 200
int  curr_acc_no = 100, sav_acc_no = 200;
//variables for keeping track of customers
int customer_no{}, current_customer{};

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
    Customer(string name){
        this->name = name;
    }
    void set_name(string name){
        this->name = name;
    }

    void set_name(){
        cout << "Enter customer first name: "; cin >> fname;
        cout << "Enter customer last name: "; cin >> lname;
    }

};

class Debt{
    Customer debtee;
    double principal, time_elapsed, interest_rate;
    string collateral;

    double value = principal* pow((1+ interest_rate/100), time_elapsed);

};

class Account{
    public:
    Customer owner;
    int account_no, daily_transactions{};
    double balance{};

    virtual bool withdraw(double amount) = 0;

    bool deposit(){
        double amount;
        cout << "Enter Amount to deposit: " << endl;
        cin >> amount;
        if (amount > 0){
            balance += amount;
            cout << "Deposited " << amount << " NGN to " << owner.name << "." << endl;
            cout << "Acct no: " << account_no << endl;
            cout << "Balance: " << balance << " NGN" << endl;
            return true;
        }
        else{
            cout << "Deposit failed! Invalid amount" << endl;
            return false;
        }
    }
    
    bool deposit(double amount){
        if (amount > 0){
            balance += amount;
            cout << "Deposited " << amount << " NGN to " << owner.name;
            cout << ". Acct no: " << account_no << endl;
            cout << "Balance: " << balance << " NGN" << endl;
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
    Current_account(){ //empty default constructor
        this->owner = owner;
        this->balance = 0;
    }
    Current_account(Customer owner){
        this->owner = owner;
    }

    Current_account(double amount){
        balance = amount;
    }

    Current_account(Customer owner, double amount){
        this->owner = owner;
        balance = amount;
    }
    
    bool withdraw(double amount) override{
        if (amount < 200){
            cout << "Withdrawals must have a value of 200 NGN or higher" << endl;
            return false;
        }
        if (balance >= amount){
            balance -= amount;
            cout << "Withdrawn " << amount << " NGN from " << owner.name << "." << endl;
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
            cout << "Withdrawn " << amount << " NGN from " << owner.name << "." << endl;
            daily_transactions += 1;
            return true;
        }

        else if (balance >= amount && daily_transactions <= 3){
            balance -= amount + 50; //transaction fee over daily limit for savings account
            cout << "Withdrawn " << amount << " NGN from " << owner.name << "." << endl;
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
double get_worth(int customer_id);
void new_account_menu(int customer_id);
void starting_deposit(int customer_id, Account *accounts);
void new_customer();
void manage_customer();
void transaction_menu();
bool transfer(Account &giver, Account &receiver, double amount);


//arrays containing relevant objects
Customer customers[100];
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
        
        case 2:{
            manage_customer();
            break;
        }

        case 3:{
            transaction_menu();
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
    return id-1; //0 indexed
}

int get_acc_no(){
    int id;

    cout << "Enter Acct No:" << endl;
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
            curr_acc_no += 1; //increment the number of current accounts
            current_accounts[curr_acc_no-101].account_no = curr_acc_no; //assign latest account number
            current_accounts[curr_acc_no-101].owner = customers[customer_id]; //assign owner
            starting_deposit(curr_acc_no-101, current_accounts);//0 index
            break;
        }

        case 2:{
            sav_acc_no += 1; //increment the number of savings accounts
            savings_accounts[sav_acc_no-201].account_no = sav_acc_no; //assign account number
            savings_accounts[sav_acc_no-201].owner = customers[customer_id]; //assign owner
            starting_deposit(sav_acc_no-201, savings_accounts);
        }
    }
}

void starting_deposit(int acc_no, Account *account){
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
                (account+acc_no)->balance = start_balance;
                cout << "Opened a new account for " << (account+acc_no)->owner.name;
                cout << " with starting deposit of " << (account+acc_no)->balance << " NGN." << endl;
                cout << "Acct No: " << (account+acc_no)->account_no << endl;
                main_menu(); //return to the main menu
                break;
            }

            else{
                cout << "Invalid starting deposit. Setting starting deposit as 0 NGN." << endl;
            }//intentional fallthrough
        }

        case 2:{
            (account+acc_no)->balance = 0;
            cout << "Opened a new account for " << (account+acc_no)->owner.name;
            cout << " with starting deposit of " << (account+acc_no)->balance << " NGN." << endl;
            cout << "Acct No: " << (account+acc_no)->account_no << endl;
            main_menu(); //return to the main menu
            break;
        }
                
        default:{
            cout << "Setting starting deposit as 0 NGN" << endl;
            (account+acc_no)->balance = 0;
            cout << "Opened a new account for " << (account+acc_no)->owner.name;
            cout << " with starting deposit of " << (account+acc_no)->balance << " NGN." << endl;
            cout << "Acct No: " << (account+acc_no)->account_no << endl;
            main_menu(); //return to the main menu
            break;
        };
    }
}

void new_customer(){
    string fname;
    string lname;

    customer_no += 1; //increment the number of active customers in the bank
    customers[customer_no-1].customer_id = customer_no; //set the customer id as latest customer
    //set the customer for actions to be taken on, (0 indexing in customer array)
    current_customer = customers[customer_no-1].customer_id - 1; 

    cout << "Enter customer first name: "; cin >> fname;
    cout << "Enter customer last name: "; cin >> lname;

    customers[current_customer].set_name(fname + " " + lname); //set customer name
    
    //Display new customer info
    cout << "Customer\t: " << customers[current_customer].name << endl;
    cout << "Customer id\t: " << customers[current_customer].customer_id << endl;

    new_account_menu(current_customer); //create account for customer
}

void manage_customer(){
    int selection, id = get_id();
    cout << "\t\t(1)Check Total Balance \t(2)Rename Account \t(3)" << endl;

    cin >> selection;

    switch(selection){
        case 1:{
            cout << customers[id].name << " has a total balance of ";
            cout << get_worth(id) << " NGN." << endl;
            main_menu();
            break;
        }
    }
};

double get_worth(int customer_id){
    double worth{};
    for(Current_account acc: current_accounts){
        if(acc.owner.customer_id == customer_id+1){// 0 index correction
            worth += acc.balance;
        }
        else{
            continue;
        }
    }
    for(Savings_account acc: savings_accounts){
        if(acc.owner.customer_id == customer_id+1){
            worth += acc.balance;
        }
        else{
            continue;
        }
    }
    return worth;
}

void transaction_menu(){
    int selection;
    cout << "Select transaction type:" << endl;
    cout << "\t\t(1) Deposit \t(2) Withdraw \t(3) Transfer\t (4) Main menu" << endl;

    cin >> selection;
    switch(selection){
        case 1:{
            int acc_no = get_acc_no();
            //current accounts
            for(int i = 0; i < 50; i++){
                if(acc_no = (current_accounts+i)->account_no){
                    (current_accounts+i)->deposit();
                    main_menu();
                    break;
                }
                else{
                    continue;
                }
            }
            //savings accounts
            for(int i = 0; i < 50; i++){
                if(acc_no = (savings_accounts+i)->account_no){
                    (savings_accounts+i)->deposit();
                    main_menu();
                    break;
                }
                else{
                    continue;
                }
            }
            cout << "Invalid account number" << endl; //doesn't find account
            main_menu();
            break;
        }

        case 2:{
            int acc_no = get_acc_no();
            //current accounts
            for(int i = 0; i < 50; i++){
                if(acc_no = (current_accounts+i)->account_no){
                    double amount; 
                    cout<< "Enter amount to withdraw: ";
                    cin >> amount;
                    (current_accounts+i)->withdraw(amount);
                    main_menu();
                    break;
                }
                else{
                    continue;
                }
            }
            //savings accounts
            for(int i = 0; i < 50; i++){
                if(acc_no = (savings_accounts+i)->account_no){
                    double amount; 
                    cout<< "Enter amount to withdraw: ";
                    cin >> amount;
                    (current_accounts+i)->withdraw(amount);
                    main_menu();
                    break;
                }
                else{
                    continue;
                }
            }
            cout << "Invalid account number" << endl; //doesn't find account
            main_menu();
            break;
        }
        /*
        case 3:
        case 4:
        default:{}*/
    }
}

bool transfer(Account &giver, Account &receiver, double amount){
    if (giver.withdraw(amount)){
        receiver.deposit(amount);
        cout << "Transferred " << amount << " NGN from ";
        cout << giver.owner.name << " to " << receiver.owner.name << endl;
        giver.balance_check();
        receiver.balance_check();

        return true;
    }
    else{
        return false;
    }
}
