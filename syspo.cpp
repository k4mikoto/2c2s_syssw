#include <stack>
#include <vector>
#include <string>
#include <iostream>

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
	int i;
	double a,b,val;
	char op;
	std::stack<double> nums;
	std::stack<char> acts;
	std::string fun_name;
	std::string fun_args;
	std::vector<std::string> fun_args_individual;
	for(i = 0; i < message.length(); ++i){
		if(message[i] == ' ')
			continue;
		else if(isalpha(message[i])){
			fun_name = message.substr(i,message.find('(',i)-i);
			fun_args=message.substr(message.find('(',i)+1,message.find(')',i)-message.find('(',i)-1);
			if(fun_name=="sin"){

			}
			else if (fun_name=="pow"){

			}
			else if(fun_name == "pi"){

			}
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
	std::cout << solve("(2-6)*3+sin(3+4)") << "\n"; 	// -5
	std::cout << solve("5*2+1-7/3-2") << "\n";	 	//  6.666
	std::cout << solve("((2+1+3)+6)*4") << "\n";	// 48
	std::cout << solve("((2))-1/2") << "\n";		//  1.5
	system("pause");
	return 0;
}
