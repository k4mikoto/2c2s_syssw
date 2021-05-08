#include <stack>
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <math.h>
#include <map>

int priority(char op){
	switch(op){
		case '+':
		case '-': return 2;
		case '*':
		case '/': return 3;
		case '>':
		case '<': return 1;
	}
	return 0;
}

double value(double a, double b, char op){
	switch(op){
		case '+': return a + b;
		case '-': return a - b;
		case '*': return a * b;
		case '/': return a / b;
		case '>': return a > b;
		case '<': return a < b;
	}
	return 0;
}

std::map<std::string, double> variables;

double solve(std::string message){
	size_t i,k;
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
			for(k = i; !leave; ++k){
				fun_args.push_back(message[k]);
				if(message[k]=='(') layer++;
				if(message[k]==')') layer--;
				if(layer==0) leave = true;
			}
			i = k - 1;
			fun_args.erase(fun_args.length()-1);
			fun_args.erase(0,1);
			std::string tmp;
			tmp+=fun_args[0];
			fun_args_individual.push_back(tmp);
			arg_num = 0;
			for(k = 1; k < fun_args.length(); ++k){
				if(fun_args[k]=='(') layer++;
				if(fun_args[k]==')') layer--;
				if((fun_args[k]==',' || fun_args[k]==';') && layer==0){
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
			else if (fun_name=="cos"){
				nums.push(cos(solve(fun_args_individual[0])));
			}
			else if (fun_name=="tan"){
				nums.push(tan(solve(fun_args_individual[0])));
			}
			else if (fun_name=="atan"){
				nums.push(atan(solve(fun_args_individual[0])));
			}
			else if (fun_name=="asin"){
				nums.push(asin(solve(fun_args_individual[0])));
			}
			else if (fun_name=="acos"){
				nums.push(acos(solve(fun_args_individual[0])));
			}
			else if (fun_name=="log"){
				nums.push(log(solve(fun_args_individual[0])));
			}
			else if (fun_name=="sqrt"){
				nums.push(sqrt(solve(fun_args_individual[0])));
			}
			else if (fun_name=="exp"){
				nums.push(exp(solve(fun_args_individual[0])));
			}
			else if (fun_name=="pow"){
				nums.push(pow(solve(fun_args_individual[0]), solve(fun_args_individual[1])));
			}
			else if(fun_name == "pi"){
				nums.push(3.1415);
			}
			else if(fun_name == "println"){
				nums.push((double)printf(">>%f\n",solve(fun_args_individual[0])));
			}
			else if(fun_name == "if"){
				if(!(fun_args.find("if")+1 || fun_args.find("while")+1 || fun_args.find("for")+1)){
					i = message.find('{',i);
					if(solve(fun_args_individual[0])){
						std::string str;
						layer = 0;
						leave = false;
						for(k = i; !leave; ++k){
							str.push_back(message[k]);
							if(message[k]=='{') layer++;
							if(message[k]=='}') layer--;
							if(layer==0) leave = true;
						}
						str.erase(str.length()-1);
						str.erase(0,1);
						i = k - 1;
						solve(str);
					}
				} else throw std::logic_error("'if' should'nt contain 'if', 'for', or 'while' inside of round brackets");
			}
			else if(fun_name == "while"){
				if(!(fun_args.find("if")+1 || fun_args.find("while")+1 || fun_args.find("for")+1)){
					i = message.find('{',i);
					std::string str;
					layer = 0;
					leave = false;
					for(k = i; !leave; ++k){
						str.push_back(message[k]);
						if(message[k]=='{') layer++;
						if(message[k]=='}') layer--;
						if(layer==0) leave = true;
					}
					str.erase(str.length()-1);
					str.erase(0,1);
					i = k - 1;
					while(solve(fun_args_individual[0]))
						solve(str);
				} else throw std::logic_error("'while' should'nt contain 'if', 'for', or 'while' inside of round brackets");
			}
			else if(fun_name == "for"){
				if(!(fun_args.find("if")+1 || fun_args.find("while")+1 || fun_args.find("for")+1)){
					i = message.find('{',i);
					std::string str;
					layer = 0;
					leave = false;
					for(k = i; !leave; ++k){
						str.push_back(message[k]);
						if(message[k]=='{') layer++;
						if(message[k]=='}') layer--;
						if(layer==0) leave = true;
					}
					str.erase(str.length()-1);
					str.erase(0,1);
					i = k - 1;
					for(solve(fun_args_individual[0]); solve(fun_args_individual[1]); solve(fun_args_individual[2]))
						solve(str);
				} else throw std::logic_error("'for' should'nt contain 'if', 'for', or 'while' inside of round brackets");
			}
			else if(fun_name == "var"){
				switch(fun_args_individual.size()){
				case 1:{
					nums.push(variables[fun_args_individual[0]]);
					break;
				}
				case 2:{
					variables[fun_args_individual[0]] = solve(fun_args_individual[1]);
					break;
				}
				default: throw std::logic_error("Not a valid 'var' usage");
				}
			}
			fun_args.erase();
			fun_args_individual.clear();
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
//	std::cout << solve("8+9-2*4") << "\n"; 			//  9
//	std::cout << solve("(2-6)*3+(3+4)") << "\n"; 	// -5
//	std::cout << solve("5*2+1-7/3-2") << "\n";	 	//  6.666
//	std::cout << solve("((2+1+3)+6)*4") << "\n";	// 48
//	std::cout << solve("((2))-1/2") << "\n";		//  1.5
	solve("println(1+1/4+sin(pi())+pow(2+1,2-3+3)) if(2>1){ println(123) } "); //10.25
	solve("var(nigga,4+7) var(nigga,var(nigga)+1) println(var(nigga)) if(var(nigga)>11){ println(var(nigga)+10) }  ");
	system("pause");
	return 0;
}
