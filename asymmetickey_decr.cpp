
// Test.cpp : Defines the entry point for the console application.
//

#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <bitset>

using namespace std;

void Build_array(string number, vector<int>& large);
vector<int> multiply_by_self(vector<int> large);
vector<int> divide_by_two(vector<int> large);
vector<int> raise_to(vector<int> large, vector<int> power);
vector<int> multiply(vector<int> large, vector<int> large2);
vector<int> mod(vector<int> large, vector<int> mod, vector<int> k, vector<int> add1);
int check_greater(vector<int> large, vector<int> mod);
vector<int> sub(vector<int> large, vector<int> mod);
vector<int> add(vector<int> large, vector<int> mod);
vector<int> string_int(string myString);
string int_string(vector<int> message);

int main()
{
	string line;
	vector<int> public_e;
	vector<int> modulus;
	string filename;
	cout << "Please enter key file name: ";
		cin >> filename;
	ifstream myfile (filename.c_str());
	int count = 0;
  	if (myfile.is_open()){
    		while ( getline (myfile,line,',') ){
			if(count == 0){
      				Build_array(line, public_e);
				count++;
			}else{
				Build_array(line, modulus);
			}
    		}
    		myfile.close();
  	}
	cout << "Please enter file name to encrypt: ";
		cin >> filename;
	ifstream ifs(filename.c_str());
	vector<int> message_int;
	char c;
  	while (ifs.get(c)) message_int.push_back(c-'0');
	
	vector<int> raised = raise_to(message_int, public_e);
	cout<<"raising"<<endl;

 
	int k_start[] = {0};
  	std::vector<int> k (k_start, k_start+ sizeof(k_start) / sizeof(int) );
	int add_help[] = {1};
  	std::vector<int> add1 (add_help, add_help+ sizeof(add_help) / sizeof(int) );
  	return 0;

	vector<int> crypt = mod(raised, modulus, k, add1);
	cout<<"moding"<<endl;

	string result = int_string(crypt);
	ofstream outputFile;
	outputFile.open("alan-nash.plaintext");
        	outputFile << result;

}


string int_string(vector<int> message){
	string result;		
	for(int i = 0; i<message.size();i++){
		int val = message[i];
		while(val<20){
			val*=10;
			i++;
			val+=message[i];
		}
		bitset<8> temp = val;
		result.push_back(char(temp.to_ulong()));
	}
	return result;
}


vector<int> string_int(string myString){
	vector<bitset<8> > message;
  	for (std::size_t i = 0; i < myString.size(); ++i)
  	{
      		message.push_back(bitset<8>(myString.c_str()[i]));
  	}
	vector<int> message_int;
	for(int i = 0; i<message.size();i++){
		int convert = (int)message[i].to_ulong();
		if(convert>999){
			int i1, i2, i3, i4;
			i4 = convert % 10;
			i3 = (convert/10) %10;
			i2 = (convert/100) %10;
			i1 = (convert/1000) %10;
			message_int.push_back(i1);
			message_int.push_back(i2);
			message_int.push_back(i3);
			message_int.push_back(i4);
		}else if(convert>99){
			int i2, i3, i4;
			i4 = convert % 10;
			i3 = (convert/10) %10;
			i2 = (convert/100) %10;
			message_int.push_back(i2);
			message_int.push_back(i3);
			message_int.push_back(i4);
		}else if(convert>9){
			int  i3, i4;
			i4 = convert % 10;
			i3 = (convert/10) %10;
			message_int.push_back(i3);
			message_int.push_back(i4);
		}else{
			int  i4;
			i4 = convert % 10;
			message_int.push_back(i4);
		
		}
		
	}

	return message_int;


}

int check_greater(vector<int> large, vector<int> mod){
	if(mod.size()<large.size()) return 1;
	else if(mod.size()>large.size()) return 0;
	else{
		for(int i = 0; i<(int)large.size(); i++){
			if(large[i]<mod[i]) return 0;
			if(large[i]>mod[i]) return 1;
		}
	}
	return 1;
}

vector<int> sub(vector<int> large, vector<int> mod){
	vector<int> result;
		int loc = large.size()-1;
		for(int i = (int)mod.size()-1; i>=0; i--){
			if(mod[i]>large[loc]){
				large[loc-1]-=1;
				large[loc]+=10;
				result.insert(result.begin(), large[loc]-mod[i]);
			}else{
				result.insert(result.begin(), large[loc]-mod[i]);
			}
			loc--;
		}
		for(int i = loc; i>=0;i--){
			result.insert(result.begin(), large[i]);
		}
		if(result[0] == 0) result.erase (result.begin());
	return result;
}

vector<int> add(vector<int> large, vector<int> mod){
	vector<int> result;
		int loc = large.size()-1;
		int rem = 0;
		for(int i = (int)mod.size()-1; i>=0; i--){
			result.insert(result.begin(), large[loc]+mod[i]+rem);
			rem = 0;
			if(result[i]>9){
				if(i==0) result.insert(result.begin(), rem);
				rem = result[i]/10;
				result[i] = result[i]%10;
			}
			loc--;
		}
		for(int i = loc; i>=0;i--){
			result.insert(result.begin(), large[i]);
		}
	while(result[0] == 0) result.erase(result.begin());
	return result;
}

vector<int> mod(vector<int> large, vector<int> mod, vector<int> k, vector<int> add1){
	k = add(add1,k);
	vector<int> check = multiply(k,mod);
	vector<int> subtract = sub(large, check);
	while(check_greater(mod,subtract)!=1){
		k = add(add1,k);
		check = multiply(k,mod);
		subtract = sub(large, check);
		while(subtract[0] == 0) subtract.erase(subtract.begin());
	}
	check = multiply(k,mod);
	subtract = sub(large, check);
	while(subtract[0] == 0) subtract.erase(subtract.begin());
	return subtract;

}
vector<int> raise_to(vector<int> large, vector<int> power){
	int mult_at_end = 0;
	if((power[power.size()-1]%2)!=0){
		mult_at_end += 1;
		power[power.size()-1]-=1;
	}
	vector<int> result = large;
	while(power.size()!=1 || power[0]>1){
		if((power[power.size()-1]%2)!=0){
			if(power.size()!=1 || power[0]>1){
				power[power.size()-1]-=1;
				vector<int> temp;
				temp = result;
				result = multiply_by_self(result);
				power = divide_by_two(power);
				result = multiply(result, temp);
				while(result[0] == 0) result.erase(result.begin());

			}
		}else{
				result = multiply_by_self(result);
				power = divide_by_two(power);
				while(result[0] == 0) result.erase(result.begin());
		}
	}
	if(mult_at_end == 1) result = multiply(result, large);
	return result;
	
}

vector<int> divide_by_two(vector<int> large){
	vector<int> result;
	int track = 0;
	for(int j = 0; j <(int)large.size(); j++){
		track*=10;
		track+=large[j];
		if(2<=track){
			int mult = track/2;
			result.push_back(mult);
			int sub = 2*mult;
			track -=sub;
		}else{
			result.push_back(0);
		}
	}
	if(result[0] == 0) result.erase (result.begin());

	return result;

}

vector<int> multiply_by_self(vector<int> large){
	vector<int> result;
	result.insert(result.begin(), 0);
	int val;
	int count = 0;
	int pos = 0;
	for(int j = large.size()-1; j >= 0; j--){
		pos = result.size()-1-count;
                for(int k = large.size()-1; k >= 0; k--){
			val = large[j] * large[k];
			int rem = val/10;
			int add = val%10;
			if(pos<=0){
				result.insert(result.begin(), rem);
				result[1] +=add;
				if(result[1]>9){
					int add_ov = result[1]/10;
					result[0] +=add_ov;
					result[1] = result[1]%10;
				}
				pos = 0;
			}else{
				result[pos-1] +=rem;				
				result[pos] +=add;
				if(result[pos]>9){
					int add_ov = result[pos]/10;
					result[pos-1] +=add_ov;
					result[pos]%=10;
				}
				if(result[pos-1]>9){
					if(pos-1==0){
						int add_ov = result[pos-1]/10;
						result.insert(result.begin(), add_ov);
						result[1]%=10;
						pos++;
					}else{
						int add_ov = result[pos]/10;
						result[pos-1] +=add_ov;
						result[pos]%=10;
					}
				}
				pos--;
			}			
                }
		count++;
	}
	while(result[0] == 0) result.erase(result.begin());
        return result;

}

vector<int> multiply(vector<int> large, vector<int> large2){
	vector<int> result;
	result.insert(result.begin(), 0);
	int val;
	int count = 0;
	int pos = 0;
	for(int j = large.size()-1; j >= 0; j--){
		pos = result.size()-1-count;
                for(int k = large2.size()-1; k >= 0; k--){
			val = large[j] * large2[k];
			int rem = val/10;
			int add = val%10;
			if(pos<=0){
				result.insert(result.begin(), rem);
				result[1] +=add;
				if(result[1]>9){
					int add_ov = result[1]/10;
					result[0] +=add_ov;
					result[1] = result[1]%10;
				}
				pos = 0;
			}else{
				result[pos-1] +=rem;				
				result[pos] +=add;
				if(result[pos]>9){
					int add_ov = result[pos]/10;
					result[pos-1] +=add_ov;
					result[pos]%=10;
				}
				if(result[pos-1]>9){
					if(pos-1==0){
						int add_ov = result[pos-1]/10;
						result.insert(result.begin(), add_ov);
						result[1]%=10;
						pos++;
					}else{
						int add_ov = result[pos]/10;
						result[pos-1] +=add_ov;
						result[pos]%=10;
					}
				}
				pos--;
			}			
                }
		count++;
	}
	while(result[0] == 0) result.erase(result.begin());
	while(result[0]>9){
		int add_ov = result[0]/10;
		result.insert(result.begin(), add_ov);
		result[1]%=10;
	}
        return result;

}


void Build_array(string number, vector<int>& large){
	for(std::string::size_type i = 0; i < number.size(); ++i) {
		if(number[i] != ' '){
			large.push_back(number[i]-'0');
		}
	}
}

