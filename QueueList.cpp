#include "QueueList.h"

#include "Customer.h"

QueueList::QueueList() { head = nullptr; }

QueueList::QueueList(Customer* customer) { head = customer; }

QueueList::~QueueList(){
}

Customer* QueueList::get_head(){
  return head;
}

void QueueList::enqueue(Customer* customer) {
  // a customer is placed at the end of the queue
  // if the queue is empty, the customer becomes the head
  Customer* temp = head;
  if(temp != NULL){
    while(temp->get_next() != NULL){
       temp = temp->get_next();
      }
   temp->set_next(customer);
   customer->set_next(NULL);
  }else{
    head = customer;
    head->set_next(NULL);
  }
  //std::cout << "inside enqueue";
  //this->print();
  
}

Customer* QueueList::dequeue() {
  // remove a customer from the head of the queue 
  // and return a pointer to it
  //std::cout << "inside dequeue";
  //this->print();
  Customer* temp = head;
  if(temp != NULL){
    head = head->get_next();
    //check for memory leak
    //delete temp;
    temp->set_next(NULL);
    //std::cout << "customer items" << head->get_numOfItems() << std::endl;
    return temp;
  }
  return NULL;
}

int QueueList::get_items() {
  // count total number of items each customer in the queue has
  Customer* temp = head;
  int numItems = 0;
  while(temp != NULL){
    numItems = numItems + temp->get_numOfItems();
    temp = temp->get_next(); 
  }
  // std::cout << "num of items " << numItems << std::endl;
  return numItems;
}

bool QueueList::empty_queue() {
  // if the queue is empty, return false
  // if the queue is not empty, delete all customers
  // and return true
  Customer* temp = head;
  if(temp != NULL){
    while(temp != NULL){
    Customer* next = temp->get_next();
    delete temp;
    temp = next;
  }
  return true;
  }
  return false;
}

void QueueList::print() {
  // print customers in a queue
  Customer* temp = head;
  while (temp != nullptr) {
    temp->print();
    temp = temp->get_next();
  }
}
