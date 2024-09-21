#include <cmath>
#include <cstdlib>
#include <iostream>
#include <sstream>

#include "Customer.h"
#include "QueueList.h"
#include "Register.h"
#include "RegisterList.h"

using namespace std;

// Function Declarations:

// Set mode of the simulation
string getMode();

// Register
void parseRegisterAction(stringstream &lineStream, string mode);
void openRegister(
    stringstream &lineStream,
    string mode);  // register opens (it is upto customers to join)
void closeRegister(stringstream &lineStream,
                   string mode);  // register closes 

// Customer
void addCustomer(stringstream &lineStream,
                 string mode);  // customer wants to join


// Helper functions
bool getInt(stringstream &lineStream, int &iValue);
bool getDouble(stringstream &lineStream, double &dValue);
bool foundMoreArgs(stringstream &lineStream);

// Global variables
RegisterList *registerList; // holding the list of registers
QueueList *doneList; // holding the list of customers served
QueueList *singleQueue; // holding customers in a single virtual queue
double expTimeElapsed; // time elapsed since the beginning of the simulation

// List of commands:
// To open a register
// register open <ID> <secPerItem> <setupTime> <timeElapsed>
// To close register
// register close <ID> <timeElapsed>
// To add a customer
// customer <items> <timeElapsed>

// do statistics
int main() {
  registerList = new RegisterList();
  doneList = new QueueList();
  singleQueue = new QueueList();
  expTimeElapsed = 0;

  // Set mode by the user
  string mode = getMode();

  string line;
  string command;

  cout << "> ";  // Prompt for input
  getline(cin, line);

  while (!cin.eof()) {
    stringstream lineStream(line);
    lineStream >> command;
    if (command == "register") {
      parseRegisterAction(lineStream, mode);
    } else if (command == "customer") {
      addCustomer(lineStream, mode);
    } else {
      cout << "Invalid operation" << endl;
    }
    cout << "> ";  // Prompt for input
    getline(cin, line);
  }

  
    cout << "Finished at time " << expTimeElapsed << endl;
    cout << "Statistics:" << endl;
    Customer* temp = doneList->get_head();
    Customer* max = doneList->get_head();

if(temp == NULL){
   cout << "Maximum wait time: " << "0" << endl;
   }

    if(temp != NULL){
      while(temp != NULL){
        if((max->get_departureTime()-max->get_arrivalTime()) < (temp->get_departureTime()-temp->get_arrivalTime())){
          max = temp;
          }
        temp = temp->get_next();
      }
    }

    if(max != NULL ){
    cout << "Maximum wait time: " << max->get_departureTime()-max->get_arrivalTime() << endl;
    temp = doneList->get_head();
    }
    

    double avgWaitTime = 0;
    double sum = 0;
    if(temp != NULL){
      while(temp != NULL){
        sum = sum + (temp->get_departureTime()-temp->get_arrivalTime());
        temp = temp->get_next();
      }
     }

  temp = doneList->get_head();
  int count = 0;
  if(temp != NULL){
      while(temp != NULL){
        count++;
        temp = temp->get_next();
      }
      }
  avgWaitTime = sum/count;
  cout << "Average wait time: " << avgWaitTime << endl;

temp = doneList->get_head();
double stdDev = 0;
double sum1 = 0;
if(temp != NULL){
      while(temp != NULL){
      sum1 += ((temp->get_departureTime()-temp->get_arrivalTime())-avgWaitTime) * ((temp->get_departureTime()-temp->get_arrivalTime())-avgWaitTime);
      temp = temp->get_next();
      } 
}

stdDev = sqrt((sum1/count));
cout << "Standard Deviation of wait time: " << stdDev << endl;


Register* tempDelReg = registerList->get_head();
if(tempDelReg != NULL){
  while(tempDelReg != NULL){
    tempDelReg->get_queue_list()->empty_queue();
    Register* nextDelReg = tempDelReg->get_next();
    delete tempDelReg;
    tempDelReg = nextDelReg;
  }
 }
delete registerList;
doneList->empty_queue();
delete doneList;
singleQueue->empty_queue();
delete singleQueue;



  // You have to make sure all dynamically allocated memory is freed 
  // before return 0
  return 0;
}

string getMode() {
  string mode;
  cout << "Welcome to ECE 244 Grocery Store Queue Simulation!" << endl;
  cout << "Enter \"single\" if you want to simulate a single queue or "
          "\"multiple\" to "
          "simulate multiple queues: \n> ";
  getline(cin, mode);

  if (mode == "single") {
    cout << "Simulating a single queue ..." << endl;
  } else if (mode == "multiple") {
    cout << "Simulating multiple queues ..." << endl;
  }

  return mode;
}

void single_add(){
  Register* free_reg = registerList->get_free_register();
  if(free_reg != NULL){
     free_reg->get_queue_list()->enqueue(singleQueue->dequeue());
     cout << "Queued a customer with free register " << free_reg->get_ID() << endl;
  }else{
    cout << "No free registers" << endl;
  }
}

void multiple_add(Customer* &newCustomer){
Register* free_reg = registerList->get_free_register();
Register* least_item_reg = registerList->get_min_items_register();
if(free_reg != NULL){
  free_reg->get_queue_list()->enqueue(newCustomer);
cout << "Queued a customer with quickest register " << free_reg->get_ID() << endl;
}else{
 least_item_reg->get_queue_list()->enqueue(newCustomer);
 cout << "Queued a customer with quickest register " << least_item_reg->get_ID() << endl;
}
}

void addCustomer(stringstream &lineStream, string mode) {
  int items;
  double timeElapsed;
  if (!getInt(lineStream, items) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments." << endl;
    return;
  }
  // Depending on the mode of the simulation (single or multiple),
  // add the customer to the single queue or to the register with
  // fewest items
  expTimeElapsed = expTimeElapsed + timeElapsed;
  
  while(1){
  Register *addDone = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  //cout << "Queued a customer with free register " << registerList->get_free_register()->get_ID() << endl;
  
  if(addDone != NULL){
    addDone->departCustomer(doneList);
    if(mode == "single"){
      if(singleQueue->get_head() != NULL){
    single_add();
    }
    
  }
  }
  else{
    break;
  }
  }

  Customer* newCustomer = new Customer(expTimeElapsed,items);
  cout << "A customer entered" << endl;
  if(mode == "single"){
    singleQueue->enqueue(newCustomer);
    single_add();
  }else if(mode == "multiple"){
    multiple_add(newCustomer);
  }
  
}


void parseRegisterAction(stringstream &lineStream, string mode) {
  string operation;
  lineStream >> operation;
  if (operation == "open") {
    openRegister(lineStream, mode);
  } else if (operation == "close") {
    closeRegister(lineStream, mode);
  } else {
    cout << "Invalid operation" << endl;
  }
}

void openRegister(stringstream &lineStream, string mode) {
  int ID;
  double secPerItem, setupTime, timeElapsed;
  // convert strings to int and double
  if (!getInt(lineStream, ID) || !getDouble(lineStream, secPerItem) ||
      !getDouble(lineStream, setupTime) ||
      !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  
  // Check if the register is already open
  // If it's open, print an error message
  // Otherwise, open the register
  // If we were simulating a single queue, 
  // and there were customers in line, then 
  // assign a customer to the new register
  
  if(registerList->foundRegister(ID) == true){
    cout << "Error: register " << ID << " is already open" << endl;
  }else{
    expTimeElapsed = expTimeElapsed + timeElapsed;
  //cout << expTimeElapsed << " " << timeElapsed << endl;
  while(1){
  Register *addDone = registerList->calculateMinDepartTimeRegister(expTimeElapsed);

  if(addDone != NULL){
    addDone->departCustomer(doneList);
   if(mode == "single"){
      if(singleQueue->get_head() != NULL){
    single_add();
    }
  }
  }else{
    break;
  }
  }
  
    Register* newReg = new Register(ID,secPerItem,setupTime,expTimeElapsed);
    cout << "Opened register " << ID << endl;
    registerList->enqueue(newReg);
    if(mode == "single"){
      if(singleQueue->get_head() != NULL){
    single_add();
    }
  }
  }
  
}

void closeRegister(stringstream &lineStream, string mode) {
  int ID;
  double timeElapsed;
  // convert string to int
  if (!getInt(lineStream, ID) || !getDouble(lineStream, timeElapsed)) {
    cout << "Error: too few arguments." << endl;
    return;
  }
  if (foundMoreArgs(lineStream)) {
    cout << "Error: too many arguments" << endl;
    return;
  }
  // Check if the register is open
  // If it is open dequeue it and free it's memory
  // Otherwise, print an error message 
  expTimeElapsed = expTimeElapsed + timeElapsed;
  //cout << expTimeElapsed << " " << timeElapsed << endl;
  while(1){
  Register *addDone = registerList->calculateMinDepartTimeRegister(expTimeElapsed);
  
  if(addDone != NULL){
    addDone->departCustomer(doneList);
    
    if(mode == "single"){
      if(singleQueue->get_head() != NULL){
    single_add();
    }
  }
  }
  else{
    break;
  }
  }

  if(registerList->foundRegister(ID) == true){
    Register* regDel = registerList->dequeue(ID);
    cout << "Closed register" << " " << ID << endl;
    delete regDel;
    regDel = NULL;
  }else{
    cout << "Error: register " << ID << " is not open" << endl;
  }
}

bool getInt(stringstream &lineStream, int &iValue) {
  // Reads an int from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  iValue = stoi(command);
  return true;
}

bool getDouble(stringstream &lineStream, double &dvalue) {
  // Reads a double from the command line
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  }
  dvalue = stod(command);
  return true;
}

bool foundMoreArgs(stringstream &lineStream) {
  string command;
  lineStream >> command;
  if (lineStream.fail()) {
    return false;
  } else {
    return true;
  }
}
