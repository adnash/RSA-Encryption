/*
* Created on: 3/24/16
* Author: Alan Nash
*/
 
#include<iostream>
#include <iterator>
#include<string>
#include <bitset>
#include<map>
#include <vector>
#include <iostream>
#include <vector>
#include <algorithm>
#include <fstream>
#include <sstream>
#include <ctype.h>
#include <string>
#include<math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;
 
//define function protoTypes
void removeDuplicate(vector<char> &str);
void getKeys(vector<char> &key1, vector<char> &key2);
void xor_Logic(vector<bitset<8> > &Message, vector<bitset<8> > &key);
void convert_binary(vector<char>& key,vector<bitset<8> >& keyb);
void transportation(vector<bitset<8> >& message, vector<bitset<8> >& key);
void read_binary_file(vector<bitset<8> >& key1, vector<bitset<8> >& key2);
//main program

int main() {
	vector<char> key1;
	vector<bitset<8> > key1b;
	vector<char> key2;
	vector<bitset<8> > key2b;
	getKeys(key1, key2);
	convert_binary(key1, key1b);
	convert_binary(key2, key2b);
	read_binary_file(key1b, key2b);
	//xor_Logic(key2b,key1b);
	return 0;
 
}

void convert_binary(vector<char>& key,vector<bitset<8> >& keyb){

  std::string temp;
	keyb.clear();
  for (std::size_t i = 0; i < key.size(); ++i)
  {
  	bitset<8> b(key[i]);
	keyb.push_back(b);
  }
	
}

void read_binary_file(vector<bitset<8> >& key1, vector<bitset<8> >& key2){
	vector<bitset<8> > message;
	vector<bitset<8> > temp;
	string filename = "binary_dump";
	cout << "Please enter file name to decrypt: ";
		cin >> filename;
	std::ifstream file (filename.c_str(), std::ios::in | std::ios::binary);
	char c;
	vector<char> read;
	while(file.get(c)){
		read.push_back(c);
	}
	convert_binary(read, temp);
	copy(temp.begin(), temp.end(), back_inserter(message));
	

	transportation(message, key2);
	transportation(message, key1);	
	xor_Logic(message, key1);


	ofstream outbin( "alan-nash-decrypted-str", ios::binary );
	for(int i = 0; i<(int)message.size();i++){
		unsigned long f = message[i].to_ulong(); 
		unsigned char c = static_cast<unsigned char>( f );	
		outbin.write( reinterpret_cast <const char*> (&c), sizeof(c) );
	}
	outbin.close();

	
			
}

void transportation(vector<bitset<8> >& message, vector<bitset<8> >& key){
	std::vector<std::vector<bitset<8> > > trans ;
	vector<bitset<8> > myRow;
	for (int i = 0; i < (int)key.size(); i++) {
		myRow.push_back(key[i]);
	}
	trans.push_back(myRow);
	for (int i = 0; i < ((int)message.size()/10)+1; i++) {
		trans.push_back(myRow);
	}
	myRow.clear();
	int count=0;
	char old_min = 'A';
	for(int i = 0; i < (int)key.size(); i++){
		char min = 'z';
		int loca = 0;
		unsigned char c1;
		for(int m = 0; m < (int)key.size(); m++){
			unsigned long i1 = trans[0][m].to_ulong(); 
			c1 = static_cast<unsigned char>( i1 );
			if(c1<=min && c1>old_min){
				min = c1;
				loca = i;
			}
		
		}
		old_min = min;
		for (int t = 1; t < ((int)trans.size()); t++) {
			trans[t][loca] = message[count];
			count++;
		}
		
	}



		vector<bitset<8> > final;
		for (int j = 1; j < (int)trans.size(); j++) {
			for(int m = 0; m<(int)trans[j].size(); m++){
				final.push_back(trans[j][m]);
			}
		}

		message = final;
}

void xor_Logic(vector<bitset<8> > &Message, vector<bitset<8> > &key){
	vector<bitset<8> > cipher_temp;
	int size_key = key.size();
	int size = Message.size();
	for (int i = 0; i < size; ++i){
		cipher_temp.push_back(Message[i]^key[i%size_key]);
	}
	Message = cipher_temp;
}

void getKeys(vector<char> &key1, vector<char> &key2){
	string input1;
	string input2;
	//get key 1 and check greater then 10 char
	while(key1.size()<10){
		key1.clear();
		cout << "Please enter key1: ";
		cin >> input1;
		copy(input1.begin(), input1.end(), back_inserter(key1));
		removeDuplicate(key1);
		if(key1.size()<10){
			cout<< "Key1 is to short please enter a new one."<<endl;
		}
	}
	while(key1.size()>10){
		key1.pop_back();
	}
	//get key 2 and check greater then 10 char
	while(key2.size()<10){
		key2.clear();
		cout << "Please enter key2: ";
		cin >> input2;
		copy(input2.begin(), input2.end(), back_inserter(key2));
		removeDuplicate(key2);
		if(key2.size()<10){
			cout<< "Key2 is to short please enter a new one."<<endl;
		}
	}
	while(key2.size()>10){
		key2.pop_back();
	}

}
 
void removeDuplicate(vector<char> &str) {
 
	//get the length of string
	int stringLength = str.size() - 1;
	 
	for (int i = 0; i < stringLength; i++) {
		for (int j = i + 1; j <= stringLength;) {
			if (str[i] == str[j]) { //if two conjugative char are same then replace the first char with last char of string and reduce the length of string
				str.erase(str.begin()+j);
				stringLength = str.size() - 1;
			} else {   //or increase j	
				j++;
			}
		}
			 
	}
 
}