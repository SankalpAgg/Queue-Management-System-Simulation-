#include "RegisterList.h"

#include <iostream>

#include "Register.h"
//#include "main.cpp"
using namespace std;

RegisterList::RegisterList() {
  head = nullptr;
  size = 0;
}

RegisterList::~RegisterList() {
  // Delete all registers in the list
}

Register* RegisterList::get_head() { 
  return head;
 }

int RegisterList::get_size() { 
  // return number of registers 
  Register* temp = head;
  int numRegs = 0;
  if(temp != NULL){
    while(temp != NULL){
      numRegs += 1;
      temp->set_next(temp->get_next());
    }
  }
  return numRegs;
}


Register* RegisterList::get_min_items_register(){
  // loop all registers to find the register with least number of items
  Register* temp = head;
  Register* min;
   min = temp;
  if(temp != NULL){
    while(temp != NULL){
     if(min->get_queue_list()->get_items() > temp->get_queue_list()->get_items()){
      min = temp;
     }
    temp = temp->get_next();
  }
}
return min;
}

Register* RegisterList::get_free_register(){
  // return the register with no customers
  // if all registers are occupied, return nullptr
  Register* temp = head;
  if(temp != NULL){
    while(temp != NULL){
      if(temp->get_queue_list()->get_head() == NULL){
        return temp;
      }
      temp = temp->get_next();
    }
  }
  
  return NULL;
}

void RegisterList::enqueue(Register* newRegister) {
  // a register is placed at the end of the queue
  // if the register's list is empty, the register becomes the head
  // Assume the next of the newRegister is set to null
  // You will have to increment size 
  Register* temp = head;
  if(temp != NULL){
    while(temp->get_next() != NULL){
       temp = temp->get_next();
    }
    temp->set_next(newRegister);
  }else{
    head = newRegister;
    head->set_next(NULL);
  }
  size++;
}

bool RegisterList::foundRegister(int ID) {
  // look for a register with the given ID
  // return true if found, false otherwise
Register* temp = head;
if(temp != NULL){
  while(temp != NULL){
    if(temp->get_ID() == ID){
      return true;
    }
    temp = temp->get_next();
  }
  return false;
}
return false; // check once
}

Register* RegisterList::dequeue(int ID) {
  // dequeue the register with given ID

  // return the dequeued register
  // return nullptr if register was not found
  Register* temp = head;
  Register* prev;
  Register* store;
  if(temp != NULL){
   if(temp->get_ID() == ID){
        store = temp;
        head = temp->get_next();
        store->set_next(NULL);
        size--;
        return store;
    }
  }

  prev = head;
  temp = prev->get_next();
  if(temp != NULL){
    while(temp != NULL){
      if(temp->get_ID() == ID){
        store = temp;
        prev->set_next(temp->get_next());
        store->set_next(NULL);
        size--;
        return store;
      }
      temp = temp->get_next();
    }
  }
  
  return NULL;
}

Register* RegisterList::calculateMinDepartTimeRegister(double expTimeElapsed) {
  // return the register with minimum time of departure of its customer
  // if all registers are free, return nullptr
   Register* temp = head;
   Register* min = head;
   
   //loop for checking queue's head 
   //return type is register, if i return earlier then that, won't send to doneList
   int flag = 0;
   if(temp != NULL){
    while(temp != NULL){
       if(temp->calculateDepartTime() == -1){
        temp = temp->get_next();
        }else{
          min = temp;
          flag = 1;
          break;
        }
    }
    if(flag == 0){
      return NULL;
    }
  }

   temp = head;
   if(temp != NULL){
      while(temp != NULL){
      if(temp->calculateDepartTime() != -1 && min->calculateDepartTime() > temp->calculateDepartTime()){
      min = temp;
}
    temp = temp->get_next();
    }
    double minDepartTime = min->calculateDepartTime();
    
      if(minDepartTime <= expTimeElapsed){
        cout << "Departed a customer at register ID " << min->get_ID() << " at " <<  minDepartTime << endl;
       
        return min;
      }else{
        return NULL;
      }
    }
    return NULL;
}

void RegisterList::print() {
  Register* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}


//If the register available time is 
//earlier than or equal to customer arrival time then it should allow a customer to placed in that register.
