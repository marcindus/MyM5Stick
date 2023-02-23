#pragma once

#include <iostream>
#include <memory>
#include <functional>
#include <map>
#include <algorithm>

using namespace std;


enum class STATES { ON, OFF};

using Funktor = function<void(STATES)>;

struct State {

  State(Funktor change_state_functor) : change_state_functor(change_state_functor) {}

  virtual void on() = 0 ;
  virtual void off() = 0;
  virtual string toString() {return string{"EMPTY"};}

  Funktor change_state_functor = nullptr;

  void change_state(STATES target_state) {
    change_state_functor(target_state);
  }


};


struct Working : State{
  Working(Funktor function_to_change_state) :
    State(function_to_change_state) {}

  void on() override {
//    cout << __PRETTY_FUNCTION__ <<  " already on\n";
  }

  void off() override {
    cout << __PRETTY_FUNCTION__ <<  " stopping\n";
    change_state(STATES::OFF);
  }

string toString() override {return string{"ON"};}
};


struct Stopped : State{
  Stopped(Funktor function_to_change_state) :
    State(function_to_change_state) {}

  void off() override {
    cout <<  __PRETTY_FUNCTION__ <<  " already off\n";
  }

  void on() override {
    cout<< __TIMESTAMP__ << ": " << __PRETTY_FUNCTION__ <<  " starting\n";
    change_state(STATES::ON);
  }
string toString() override {return string{"OFF"};}
};


struct Machine {

  Machine() {
    all_states[STATES::ON] = make_shared<Working>(change_state);
    all_states[STATES::OFF] = make_shared<Stopped>(change_state);
    current_state = all_states[STATES::OFF];

  }

  void on() { current_state->on();}
  void off() { current_state->off();}

  shared_ptr<State> current_state = nullptr;
  function<void(STATES)> change_state = [this](STATES state)
  {
    cout << __PRETTY_FUNCTION__ << " state change from " << current_state->toString() 
    << " to " << all_states[state]->toString() << "\n";
      this->current_state = all_states[state];
  };

  map<STATES, shared_ptr<State>> all_states;

};

