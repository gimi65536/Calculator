#include "BigNumber.h"
#ifdef _BIG_NUMBER_
#ifndef _VARIABLE_ADT_
#define _VARIABLE_ADT_

const string variable_namespace = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_$";
const string varinit_regex = "A-Za-z_\\$";
const string varcontent_regex = "\\w\\$";
const string varinitonly_regex = "";
const string varcontonlt_regex = "\\d";
const string vartotal_regex = "\\w\\$";

class Node{
friend class Variable_ADT;
private:
	string name;
	Node* next;
	Node(){
		name = "\0";
		num = 0;
		next = nullptr;
	}
	Node(string nama){
		name = nama;
		num = 0;
		next = nullptr;
	}
	Node(string nama, BigNumber n){
		name = nama;
		num = n;
		next = nullptr;
	}
	~Node(){
		delete next;
	}
public:
	BigNumber num;
	//set con/destructor private so that user cannot create Node casually
	//but set Var_ADT friend so that it can create Node objects
	//because Var_ADT is friend class, so we don't have to prepare getter or setter
};

class Variable_ADT{
private:
	Node* head;
	BigNumber count;
	Node Error_node;
public:
	Variable_ADT(){
		head = nullptr;
		count = 0;
	}
	Variable_ADT(bool temp){
		head = nullptr;
		count = 0;
		if(temp){
			BigNumber zero = 0;
			for(int i = 0;i < 52;i++){
				string nama;
				nama += variable_namespace[i];
				new_var(nama, zero);
			}
		}
	}
	~Variable_ADT(){
		delete head;
	}
	BigNumber& operator [] (BigNumber i){
		if(i < 0 || i >= count){
			return Error_node.num;
		}
		Node* tmp = head;
		for(BigNumber j = 0;j < count;j += 1){
			if(j == i){
				return tmp -> num;
			}
			tmp = tmp -> next;
		}
	}
	bool new_var(string nama, const BigNumber& n){
		if(count == 0){
			head = new Node(nama, n);
			count += 1;
			return true;
		}
		Node* tmp = head;
		while(tmp -> next != nullptr){
			tmp = tmp -> next;
		}
		tmp -> next = new Node(nama, n);
		count += 1;
		return true;
	}
	bool check_name(string nama){
		Node* tmp = head;
		while(tmp != nullptr){
			if(tmp -> name == nama){
				return true;
			}
			tmp = tmp -> next;
		}
		return false;
	}
	BigNumber getIndex(string nama){
		Node* tmp = head;
		for(BigNumber i = 0;i < count;i += 1){
			if(tmp -> name == nama){
				return i;
			}
			tmp = tmp -> next;
		}
		return -1;
	}
	const BigNumber& getCount(){
		return count;
	}
};

#ifndef _AUTO_VAR_SPACE_
#define _AUTO_VAR_SPACE_ 1
#endif
#if _AUTO_VAR_SPACE_ == 0
Variable_ADT variable(false);
#else
Variable_ADT variable(true);
#endif

#endif
#endif