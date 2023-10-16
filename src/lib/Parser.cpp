#include "Parser.h"
#include "AST.h"
#include "Node.h"
#include "Operator.h"
#include "Number.h"
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <cstdlib>

using namespace std;

Parser::Parser() {
    root = nullptr;
}

void Parser::read_tokens(vector<Token*> tokens_list) {
    // checking if it has end token
    // check it has correct end sign

    Token* end_token = nullptr;
    if (!tokens_list.empty()) {
        end_token = tokens_list.at(tokens_list.size() - 1);
        if (end_token->value != "END") {
            cout << "Unexpected token at line " << end_token->row << " column " << end_token->column 
                << ": END" << endl;
            exit(2);
        }
        //ex: Unexpected token at line 1 column 20: END
    }
    else {
        return;
    }

    // checking if it ends with )
    if (tokens_list.size() > 2) {
        end_token = tokens_list.at(tokens_list.size() - 2);
        if (end_token->value != ")") {
            cout << "Unexpected token at line " << end_token->row << " column " << end_token->column 
                << endl;
            exit(2);
        }
    }

    Node* operator_mark = nullptr;
    Node* add_operator = nullptr;
    Node* add_number = nullptr;
    bool left_parenthesis = false;
    bool right_parenthesis = false;
    Token* current_token = nullptr;
    int parenthesis_switch = 0;
    set<string> operator_check = {"+", "-", "*", "/"};
    // string opperators[] = {"+", "-", "*", "/"}; 
    // set<string> opperator_check(opperators, opperators + 5);

    // reading token until last )

    for (unsigned i = 0; i < tokens_list.size()- 1; ++i) {
        // ith token
        current_token = tokens_list.at(i);

        // checking if it is "("

        // if it is true, the next value would be child of current node
        if (current_token->value == "(") {
            left_parenthesis = true;
            parenthesis_switch += 1;
        } 
        // if last Token one was (, add the value as mark_token's child
        else if (left_parenthesis) {
            // checking if it is not a number
            // after (, there always should be operator, no parenthesis and no number
            if (operator_check.find((tokens_list.at(i))->value) == operator_check.end()) {
                cout << "Unexpected token at line " << current_token->row << " column " 
                    << current_token->column << endl;
                exit(2);
            }
            // checking if it is an 
            // in the case where root node does not exist
            if (root == nullptr) {
                root = new Operator(nullptr, current_token);
                operator_mark = root;
            }
            // otherwise
            else {
                add_operator = new Operator(operator_mark, current_token);
                operator_mark->add_child(add_operator);
                operator_mark = add_operator;
            }
            left_parenthesis = false;
        }
        // if the last token was ), return to the parent node
        if (right_parenthesis) {
            operator_mark = operator_mark->switch_to_parent();
            right_parenthesis = false;
        }        
        // if ), finished with the current )
        if (current_token->value == ")") {
            right_parenthesis = true;
            parenthesis_switch -= 1;
            if (parenthesis_switch < 0) {
                cout << "Unexpected token at line " << current_token->row << " column " 
                    << current_token->column << endl;
                exit(2);
            }

        }
        // in the  case of numbers
        else {
            add_number = new Number(operator_mark,current_token);
            operator_mark->add_child(add_number);
        }

    }
    
    Token* check_end_parenthesis = tokens_list.at(tokens_list.size() - 2);
    if (parenthesis_switch != 0 || operator_mark != nullptr) {

        cout << "Unexpected token at line " << check_end_parenthesis->row 
             << " column " << check_end_parenthesis->column << endl;
        exit(2);
    }
}

double Parser::calculate() const {
    if (root == nullptr) {
        return 0;
    }
    Node* current_node = root;
    return calculate_help(current_node);
}

double Parser::calculate_help(Node* operator_node) const {
    // check if the node is operator, if not, return the value;
    if (!operator_node->node_type()) {
        return operator_node->get_number();
    }


    // go through the children nodes
    string operator_sign = operator_node->check_operator();
    vector<Node*>& list_children = operator_node->children;
    double division_check = 0;

    double result = 0;
    Node* current_node = nullptr;
    for (unsigned int i = 0; i < list_children.size(); ++i) {
        current_node = list_children.at(i);
        if (operator_sign == "+") {
            result += calculate_help(current_node);
        }
        else if (operator_sign == "-") {
            result -= calculate_help(current_node);
        }
        else if (operator_sign == "*") {
            result *= calculate_help(current_node);
        }
        else if (operator_sign == "/") {
            division_check = calculate_help(current_node);
            if (i != 0 && division_check == 0) {
                cout << "Runtime error: division by zero." << endl;
                exit(3);
            }
            else {
                result /= division_check;
            }
        }
    }
    return result;
}


void Parser::print() const {
    cout << print_help(root) << endl;
    cout << calculate() << endl;
}

string Parser::print_help(Node* in_node) const {
    if (!in_node->node_type()) {
        return to_string(in_node->get_number());
    }

    string print_expression;
    string expression;
    print_expression += "(";
    expression = in_node->check_operator();

    vector<Node*>& list_children = in_node->children;

    for (unsigned int i = 0; i < list_children.size(); ++i) {
        print_expression += print_help(list_children.at(i));
        if (i != list_children.size() - 1) {
            print_expression += ((list_children.at(i))->check_operator());

        }
    }
    print_expression += ")";
    return print_expression;
}

Parser::~Parser() {

    delete_help(root);
}

void Parser::delete_help(Node* current_node) {
    if (!current_node->node_type()) {
        delete current_node;

        return;
    }

    vector<Node*>& list_children = current_node->children;
    for (unsigned int i = 0; i < list_children.size(); ++i) {
        delete_help(list_children.at(i));
    }
    delete current_node;
}

