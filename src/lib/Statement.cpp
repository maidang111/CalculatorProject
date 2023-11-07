#include <iostream>
#include "Statement.h"
#include "InfixParser.h"
using namespace std;

//Creating a statment block that takes a vector and sperates it into a condidtion and body vector for easily evaluation
Statement::Statement(){}

Statement::~Statement(){
    for(size_t i = 0; i < condition.size(); i++){
        delete condition.at(i);
    }
    // cout << body.size() << endl;
    for(size_t i = 0; i < body.size(); i++){
        // cout << body.at(i)->print();
        delete body.at(i);
    }
    // cout << "delete this";
    // delete this;
}
void Statement::deleteStatement(){
    // cout << "here" << endl;
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}

// void deleteFuc(Statement* head){
//     if(head == nullptr){
//         return;
//     }

//     for(size_t i = 0; i < head->condition.size(); i++){
//         delete head->condition.at(i);
//     }
//     for (size_t i = 0; i< head->body.size(); i++){
//         deleteFuc(head->body.at(i));
//     }
//     delete head;
// }

void Statement::print(){
    cout << "not a vaild function for type" << endl;
    return;
}

void If::print(){
    cout << "if "; 

    InfixParser infixParser(condition);
    infixParser.print_val = false;
    infixParser.print_endl = false;
    infixParser.build_AST();

    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        cout << "    ";
        body.at(i)->print();
        cout << endl;
    }
    cout << "}" << endl;
}
void If::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}

void Else::print(){
    cout << "else "; 
    
    InfixParser infixParser(condition);
    infixParser.print_val = false;
    infixParser.print_endl = false;
    infixParser.build_AST();    
    
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        cout << "    ";
        body.at(i)->print();
        cout << endl;
    }
    cout << "}" << endl;
}
void Else::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}

void While::print(){
    cout << "while "; 
    // cout << condition.size();
    InfixParser infixParser(condition);
    infixParser.print_val = false;
    infixParser.print_endl = false;
    infixParser.build_AST();
    
    cout << " {" << endl;
    for(size_t i = 0; i < body.size(); i++){
        cout << "    ";
        body.at(i)->print();
    }
    cout << "}" << endl;
}
void While::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}

void Print::print(){
    cout << "print ";
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->print();
    }
}

void Print::deleteStatement(){
    for(size_t i = 0; i < body.size(); i++){
        body.at(i)->deleteStatement();
    }
    delete this;
}

void Expression::print(){
    InfixParser infixParser(body);
    infixParser.print_val = false;
    infixParser.print_endl = true;
    infixParser.build_AST();
}

void Expression::deleteStatement(){
    // cout << body.size();
    // for(size_t i = 0; i < body.size(); i++){
    //     delete body.at(i);
    // }
    delete this;
}