#include <iostream>
#include <pthread.h>
#include <string>
#include <fstream>
#include <unistd.h>
#include <queue>
#include <thread>
#include <chrono>


using namespace std;


struct BankAccount{
  string company_name; // name of the company
  int balance; // current balance in the account
  pthread_mutex_t mutex; // pthread_mutex_t type to represent the mutex
};

struct TicketVendingMachine {
  pthread_t thread; // pthread_t type to represent the thread
  pthread_mutex_t mutex; // pthread_mutex_t type to represent the mutex
  int in_process; // whether the machine is in use or not
  int customer_id; // ID of the customer using the machine
  int id; // ID of the ticket vending machine
};

struct Customer {
  pthread_t thread; // pthread_t type to represent the thread
  int id; // ID of the customer
  int sleep_time; // duration to sleep after arrival
  int tvm_id;  // ID of the ticket vending machine
  int bank_id;  // ID of bank account
  int amount; // amount of the prepayment
};

pthread_mutex_t output_mutex; 

const int NUM_COMPANIES = 5;
const string COMPANY_NAMES[NUM_COMPANIES] = {"Kevin", "Bob", "Stuart", "Otto", "Dave"};
BankAccount accounts[NUM_COMPANIES];
const int NUM_VMACHINES = 10;
TicketVendingMachine vmachines[NUM_VMACHINES];
Customer *customers;
int finish = 1;
int finish_out = 1;
string outfile;
string output = "";



void *output_func(void *param) {
    ofstream log_file(outfile);

    while (finish_out > 0) {
        if (output != "") {
            log_file << output << endl;
            output = "";
            pthread_mutex_unlock(&output_mutex);
        }
    }

    log_file.close();
    pthread_exit(NULL);
}

void *vmachine_func(void *param) {

    int vmachine_id = *(int *)param;
    int bank;
    int amount;

    while(finish > 0 | vmachines[vmachine_id].in_process == 1){
       
        if (vmachines[vmachine_id].in_process == 1){
            
            int cust_id = vmachines[vmachine_id].customer_id;
            
            Customer customer = customers[cust_id];
            bank = customer.bank_id;
            amount = customer.amount;

            
            pthread_mutex_lock(&accounts[bank].mutex);
            accounts[bank].balance += amount;
          
            pthread_mutex_unlock(&accounts[bank].mutex);

            vmachines[vmachine_id].in_process = 0;;
            
            pthread_mutex_lock(&output_mutex);

            output = "Customer" + to_string(cust_id + 1) + "," + to_string(amount) + "TL," + accounts[bank].company_name;

            //cout << "Customer" << cust_id + 1 << "," << amount << "TL," << accounts[bank].company_name << endl;

            pthread_mutex_unlock(&vmachines[vmachine_id].mutex);

        }
    }    
    pthread_exit(NULL);
}

void *customer_func(void *param) {

    int customer_id = *(int *)param;

    

    int milliseconds = customers[customer_id].sleep_time ;
    
    this_thread::sleep_for(chrono::milliseconds(milliseconds) );

    int vmachine= customers[customer_id].tvm_id -1;

    pthread_mutex_lock(&vmachines[vmachine].mutex);
    
    vmachines[vmachine].customer_id = customer_id;
    vmachines[vmachine].in_process = 1;

    pthread_exit(NULL);
}



int main(int argc, char** argv){

    string filename = argv[1];
    fstream file(filename);
    string line;

    getline(file, line);

    int num_customer = stoi(line);

    outfile = filename.substr(0, filename.length()-4) + "_log.txt";

    customers = new Customer[num_customer];

    // create an array of bank accounts

    for (int i = 0; i < NUM_COMPANIES; i++) {
        accounts[i].company_name = COMPANY_NAMES[i];
        accounts[i].balance = 0;
        pthread_mutex_init(&accounts[i].mutex, NULL);
    }

    pthread_t output_log;
    pthread_create(&output_log, 0, output_func, 0);

    for (int i = 0; i < NUM_VMACHINES; i++) {
        pthread_mutex_init(&vmachines[i].mutex, NULL);
        vmachines[i].in_process = 0;
        vmachines[i].id = i;
        pthread_create(&vmachines[i].thread, NULL, vmachine_func, &vmachines[i].id);
    }

    sleep(1);

    size_t pos;
    string delimiter = ",";
    string token;
    
    int sleep_time;
    int vmachine;
    string company;
    int amount;

    int j;

    for (int i = 0; i < num_customer; i++){
        getline(file, line);
        customers[i].id = i;
        j = 0;
        while ((pos = line.find(delimiter)) != std::string::npos) {
            token = line.substr(0, pos);
            
            if (j == 0) {customers[i].sleep_time = stoi(token);}
            else if (j == 1) {customers[i].tvm_id = stoi(token);}
            else if (j == 2) {company = token;}
            line.erase(0, pos + delimiter.length());
            j++;
        }
        for (int j = 0; j < NUM_COMPANIES; j++) {
            if (company == COMPANY_NAMES[j]) {
                customers[i].bank_id = j;
                break;
            }
        }
        customers[i].amount = stoi(line);
    }
    
    for(int i = 0; i < num_customer; i++ ) {
  	    pthread_create(&customers[i].thread,NULL,customer_func, &customers[i].id);
    }

    sleep(1);

    for(int i = 0; i < num_customer; i++ ) {
  	    pthread_join(customers[i].thread,NULL);
    }

    finish = 0;

    for(int i = 0; i < NUM_VMACHINES; i++ ) {
  	    pthread_join(vmachines[i].thread,NULL);
    }

    pthread_mutex_lock(&output_mutex);

    output = "All payments are completed";

    for(int i = 0;i < NUM_COMPANIES; i++){
        //cout << COMPANY_NAMES[i] << ": " << accounts[i].balance << endl;
        pthread_mutex_lock(&output_mutex);
        output = COMPANY_NAMES[i] + ": " + to_string(accounts[i].balance);
    }

    sleep(0.5);
    
    finish_out = 0;
    pthread_join(output_log,NULL);
}
