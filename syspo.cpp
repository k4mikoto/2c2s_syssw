#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>

int priority(char op){
	switch(op){
		case '+':
		case '-': return 1;
		case '*':
		case '/': return 2;
	}
	return 0;
}

double value(double a, double b, char op){
	switch(op){
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
	}
	return 0;
}

double solve(std::string message){
	size_t i;
	double a,b,val;
	char op;

	std::stack<double> nums;
	std::stack<char> acts;

	std::string fun_name;
	std::string fun_args;
	std::vector<std::string> fun_args_individual;

	bool leave = false;
	size_t arg_num = 0;
	size_t layer = 0;
	for(i = 0; i < message.length(); ++i){
		if(message[i] == ' ')
			continue;
		else if(isalpha(message[i])){
			leave = false;
			fun_name = message.substr(i,message.find('(',i)-i);
			i = message.find('(',i);
			for(auto k = i; !leave; ++k){
				fun_args.push_back(message[k]);
				if(message[k]=='(') layer++;
				if(message[k]==')') layer--;
				if(layer==0) leave = true;
			}
			fun_args.erase(fun_args.length()-1);
			fun_args.erase(0,1);
			std::string tmp;
			tmp+=fun_args[0];
			fun_args_individual.push_back(tmp);
			arg_num = 0;
			for(size_t k = 1; k < fun_args.length(); ++k){
				if(fun_args[k]=='(') layer++;
				if(fun_args[k]==')') layer--;
				if((fun_args[k]==',') && layer==0){
					std::string str;
					k++;
					arg_num++;
					str = fun_args[k];
					fun_args_individual.push_back(str);
					continue;
				}
				fun_args_individual[arg_num].push_back(fun_args[k]);
			}
			if(fun_name=="sin"){
				nums.push(sin(solve(fun_args_individual[0])));
			}
			else if (fun_name=="pow"){
				nums.push(pow(solve(fun_args_individual[0]),solve(fun_args_individual[1])));
			}
			else if(fun_name == "pi"){
				nums.push(3.1415);
			}
			fun_args.erase();
			fun_args_individual.clear();
			i=message.find(')',i);
			if(i==message.length()){
				break;
			}

		}
		else if(message[i] == '('){
			acts.push(message[i]);
		}
		else if(isdigit(message[i])){
			val = 0;
			while(i < message.length() && isdigit(message[i])){
				val = (val*10) + (message[i]-'0');
				i++;
			}
			nums.push(val);
			i--;
		}
		else if(message[i] == ')'){
			while(!acts.empty() && (acts.top() != '(') ){
				b = nums.top();
				nums.pop();
				a = nums.top();
				nums.pop();
				op = acts.top();
				acts.pop();
				nums.push(value(a, b, op));
			}
			if(!acts.empty())
				acts.pop();
		}
		else{
			while(!acts.empty() && (priority(acts.top()) >= priority(message[i])) ){
				b = nums.top();
				nums.pop();
				a = nums.top();
				nums.pop();
				op = acts.top();
				acts.pop();
				nums.push(value(a, b, op));
			}
			acts.push(message[i]);
		}
	}
	while(!acts.empty()){
		b = nums.top();
		nums.pop();
		a = nums.top();
		nums.pop();
		op = acts.top();
		acts.pop();
		nums.push(value(a, b, op));
	}
	return nums.top();
}

int main() {
	std::cout << solve("8+9-2*4") << "\n"; 			//  9
	std::cout << solve("(2-6)*3+(3+4)") << "\n"; 	// -5
	std::cout << solve("5*2+1-7/3-2") << "\n";	 	//  6.666
	std::cout << solve("((2+1+3)+6)*4") << "\n";	// 48
	std::cout << solve("((2))-1/2") << "\n";		//  1.5
	std::cout << solve("1+1/4+sin(pi())+pow(2+1,2-3+3)") << "\n"; //10.25
	system("pause");
	return 0;
}
