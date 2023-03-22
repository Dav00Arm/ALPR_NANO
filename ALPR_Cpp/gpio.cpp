// Handles input and output
#include <iostream>
// For delay function
#include <chrono> 
// Handles threads of program execution
#include <thread>
// Signal handling
#include <signal.h>
#include <JetsonGPIO.h>
 
// Pin Definitions


// Flag to determine when user wants to end program
bool done = false;
 
// Function called by Interrupt
void signalHandler (int s){
  done = true;
}

void openGate(int pin){
  GPIO::output(pin, GPIO::HIGH);
}

void closeGate(int pin){
  GPIO::output(pin, GPIO::LOW);
}

int main() {
  std::vector<int> pins = {7,11,13,23};
  GPIO::setmode(GPIO::BOARD);

  signal(SIGINT, signalHandler);

  GPIO::setup(pins[0], GPIO::OUT, GPIO::LOW);  
  GPIO::setup(pins[1], GPIO::OUT, GPIO::LOW);  
  GPIO::setup(pins[2], GPIO::OUT, GPIO::LOW);  
  GPIO::setup(pins[3], GPIO::OUT, GPIO::LOW);  

  // Pin Definitions 
  while(true){
    int value;
    std::cout << "Enter a number: ";
    std::cin >> value;
    openGate(pins[value]);
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    closeGate(pins[value]);
    if(value == 5){
      break;
    }
  }
  GPIO::cleanup();

  return 0;
}