//
// Created by vladimir on 08.08.2015.
//

#ifndef SOCKET_DFA_H
#define SOCKET_DFA_H

#include <map>
#include <utility>

template <class S, class T> class DFA {
protected:
  std::map<std::pair<S, T>, S> _table;
  std::size_t _it = 0;
  S _currentState;

  S _begin, _end;

public:
  const S& currentState() const { return _currentState; }
  const S& finalState() const { return _end; }
  DFA() {}
  DFA(const S& beginState, const S& endState)
      : _begin(beginState), _end(endState) {
    _currentState = beginState;
  }
  DFA(const DFA&) = default;
  DFA(DFA&&) = default;
  virtual DFA& operator=(const DFA&) = default;
  virtual ~DFA() = default;

  void add(const std::pair<S, T>& pair, const S& state) {
    _table.insert(std::make_pair(pair, state)); // = state;
  }
  void transition(const T& token) {
    auto nextState = _table.find(std::make_pair(_currentState, token));
    if (nextState == _table.end())
      throw std::runtime_error("transition does not exist");
    _currentState = nextState->second;
  }

};

#endif // SOCKET_DFA_H
