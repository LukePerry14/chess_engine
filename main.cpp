#include <iostream>
#include <array>
#include <vector>
#include <utility>
#include <sstream>
#include <variant>
#include <type_traits>
#include "board.h"

using namespace std;

uint16_t encode_numbers_to_16bit(int num1, int num2) {
    
    uint16_t encoded_value = (static_cast<uint16_t>(num2) << 8) | static_cast<uint16_t>(num1);
    
    return encoded_value;
}

int main(int argc, char *argv[]) {
    int maxDepth = 10;
    if (argc != 1){
        if (argc != 2) {
            cout << "Invalid number of arguments" << endl;
            cout << "Exiting..." << endl;
            return 0;
        } else {
            try {
                maxDepth = std::stoi(argv[1]);
            } catch (const std::invalid_argument& ia) {
                cerr << "Invalid argument type" << endl;
                cout << "Exiting..." << endl;
                return 1;
            } catch (const std::out_of_range& oor) {
                cerr << "Argument out of range" << endl;
                cout << "Exiting..." << endl;
                return 1;
            }
        }
    }
    Board board(maxDepth);
    bool whiteTurn = true;

    while (true) {
        board.print_board();
        auto values = board.move_recommendation(whiteTurn);

        if (!(values.first)) {
            break;
        }
        string colour = whiteTurn? "white" : "black";

        int num1 = -1;
        int num2 = -1;
        uint16_t move;

        while (num1 < 0 || num1 > 63 || num2 < 0 || num2 > 63) {
            cout << colour << " move: ";
            string input;
            getline(cin, input);
            stringstream ss(input);
            ss >> num1 >> num2;
        }

        try {
            move = encode_numbers_to_16bit(num1, num2);
        } catch (const invalid_argument& e) {
            cerr << "Error: " << e.what() << endl;
        }   

        board.make_move(move);
        whiteTurn = not whiteTurn;
    }
    return 0;
}
