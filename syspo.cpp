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
	size_t i,v;
	double a,b,val;
	char op;
	std::stack<double> nums;
	std::stack<char> acts;

	std::string fun_name;
	std::string fun_args;
	std::vector<std::string> fun_args_individual;

	bool type = false;
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
//			if (fun_name == "println") {
//				printf("%s\n",fun_args.c_str());
//				system("pause");
//			}
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
//				printf("faiif at %d\n",!(fun_args_individual[0].find("if")+1));
				if(!(fun_args_individual[0].find("if")+1)){
					if(solve(fun_args_individual[0])){
						std::string str;
						v = message.find('{',i);
						layer = 0;
						leave = false;
						for(auto k = v; !leave; ++k){
							str.push_back(message[k]);
							if(message[k]=='{') layer++;
							if(message[k]=='}') layer--;
							if(layer==0) leave = true;
						}
						printf("\ninside if %s\n",str.c_str());
						solve(str);
					}
				}
				type = true;
			}
			else if(fun_name == "var"){
				switch(fun_args_individual.size()){
				case 1:{
					nums.push(variables[fun_args_individual[0]]);
					break;
				}
				case 2:{
					variables[fun_args_individual[0]] = solve(fun_args_individual[1]);
//					printf("%s set %f\n",fun_args_individual[0].c_str(),variables[fun_args_individual[0]]);
					break;
				}
				default: throw std::logic_error("Not a valid 'var' usage");
				}
			}
			fun_args.erase();
			fun_args_individual.clear();
			if(type){

			} else{
				i = message.find(')',i);
			}
			type = false;
			printf("%c",message[i]);
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
	solve("println(1+1/4+sin(pi())+pow(2+1,2-3+3)) if(2>1){ println(123) } "); //10.25
	solve("var(nigga,4+7) var(nigga,var(nigga)+1) println(var(nigga)) if(var(nigga)>11){ println(var(nigga)+10) }  ");
	system("pause");
	return 0;
}
