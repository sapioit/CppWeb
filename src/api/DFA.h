//
// Created by vladimir on 08.08.2015.
//

#ifndef SOCKET_DFA_H
#define SOCKET_DFA_H

//#include <string>
//#include <map>
//#include <utility>
//
//namespace Http {
//    template<class S, class M>
//    class DFA {
//        std::map<std::pair<S, M>, S> _table;
//        std::size_t _it = 0;
//        S _currentState;
//
//        S _begin, _end;
//    public:
//
//        DFA(const S& beginState, const S& endState) {
//
//        }
//
//        void AddState(const std::pair<S, M>& pair, const S& state) {
//            _table[pair] = state;
//        }
//        S nextState(const M& token) {
//            auto nextState = _table.find(std::make_pair(_currentState, token));
//        }
//
//    };
//
//    enum State {
//        Begin,
//        End,
//        EOL,
//        Method,
//        Method_S,
//        URI,
//        URI_s,
//        Version,
//        Version_S,
//    };
//    constexpr auto space = " ";
//    constexpr auto CRLF = "\r\n";
//
//    void bla(std::string http) {
//        DFA<State, std::string> dfa(State::Begin, State::End);
//
//        dfa.AddState(std::make_pair(State::Begin, ""), State::Method);
//
//        dfa.AddState(std::make_pair(State::Method, space), State::Method_S);
//        dfa.AddState(std::make_pair(State::Method_S, space), State::Method_S);
//        dfa.AddState(std::make_pair(State::Method_S, 'h'), State::URI);
//
//        dfa.AddState(std::make_pair(State::URI, space), State::URI_s);
//        dfa.AddState(std::make_pair(State::URI_s, space), State::URI_s);
//        dfa.AddState(std::make_pair(State::URI_s, "H"), State::Version);
//
//        dfa.AddState(std::make_pair(State::Version, space), State::Version_S);
//        dfa.AddState(std::make_pair(State::Version_S, space), State::Version_S);
//        dfa.AddState(std::make_pair(State::Version_S, CRLF), State::EOL);
//
//        dfa.AddState(std::make_pair(State::EOL, CRLF), State::End);
//
//
//        State current = State::Begin;
//        std::size_t it = 0;
//
//        while(current != State::End) {
//            current = dfa.nextState(std::string(http.begin() + it, 1));
//            switch(current) {
//                case State::Method:
//                {
//                    auto space = http.find(space, it);
//                    std::string method(http.begin() + it, http.begin() + space -1);
//                    it = space;
//                }
//            }
//        }
//
//    }
//}


#endif //SOCKET_DFA_H
