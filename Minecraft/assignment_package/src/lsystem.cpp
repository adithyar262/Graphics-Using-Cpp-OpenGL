#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class LSystem {
private:
    std::string axiom;
    std::unordered_map<char, std::string> rules;
    int iterations;

public:
    LSystem(const std::string& axiom, const std::unordered_map<char, std::string>& rules, int iterations)
        : axiom(axiom), rules(rules), iterations(iterations) {}

    std::string generate() {
        std::string current = axiom;
        for (int i = 0; i < iterations; ++i) {
            std::string next;
            for (char c : current) {
                if (rules.find(c) != rules.end()) {
                    next += rules[c];
                } else {
                    next += c;
                }
            }
            current = next;
        }
        return current;
    }
};
