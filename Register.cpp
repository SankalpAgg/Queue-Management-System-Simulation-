
// Created by Salma Emara on 2023-06-02.
#include "Register.h"

#include <iostream>
using namespace std;
Register::Register(int id, double timePerItem, double overhead,
                   double entryTime) {
  ID = id;
  secPerItem = timePerItem;
  overheadPerCustomer = overhead;
  availableTime = entryTime;
  next = nullptr;           // no other registers yet
  queue = new QueueList();  // no customers in line yet, but has to initialize a
                            // queue
}

Register::~Register(){
  delete queue;
}

QueueList* Register::get_queue_list(){
  return this->queue;
}

Register* Register::get_next(){
  return this->next;
}

int Register::get_ID(){
  return ID;
}

double Register::get_secPerItem(){
  return secPerItem;
}

double Register::get_overheadPerCustomer(){
  return overheadPerCustomer;
}

double Register::get_availableTime(){
  return availableTime;
}

void Register::set_next(Register* nextRegister){
  this->next = nextRegister;
}

void Register::set_availableTime(double availableSince){
  availableTime = availableSince;
}

double Register::calculateDepartTime() {
  // Get the departure time of the first customer in the queue
  // returns -1 if no customer is in the queue
  if(queue->get_head() != NULL){
  if(queue->get_head()->get_arrivalTime() >= availableTime){
    //cout << secPerItem * queue->get_head()->get_numOfItems() + overheadPerCustomer + queue->get_head()->get_arrivalTime() << endl;
    return secPerItem * queue->get_head()->get_numOfItems() + overheadPerCustomer + queue->get_head()->get_arrivalTime();
  }else{
    return secPerItem * queue->get_head()->get_numOfItems() + overheadPerCustomer + availableTime;
  }
  }else{
    return -1;
  }
  }

void Register::departCustomer(QueueList* doneList) {
  // dequeue the head, set last dequeue time, add to doneList,
  // update availableTime of the register
  Register* temp = this;
  Customer* tempCustomer = queue->get_head();
  tempCustomer->set_departureTime(temp->calculateDepartTime());
  queue->dequeue();
  doneList->enqueue(tempCustomer);
  temp->set_availableTime(tempCustomer->get_departureTime());
}

void Register::print() {
  std::cout << "Register ID: " << ID << std::endl;
  std::cout << "Time per item: " << secPerItem << std::endl;
  std::cout << "Overhead per customer: " << overheadPerCustomer << std::endl;
  if (queue->get_head() != nullptr) {
    std::cout << "Queue has customers: \n";
    queue->print();
  }
}
