#include <iostream>
#include <cstdio>
#include <string>
#include <ctype.h>

#define NUM 256
#define OP  257
#define NO_OP -1
#define SUM 1
#define MINUS 2
using namespace std;

struct token{
  int t;
  int a;
};
struct attrib {
  char op;
  int value;
};

void error(string);
void print(char);
int calc(int, int, int);
attrib expr();
attrib term();
attrib rest();
void match(string);
string nextToken();


string input; // = "9-5+2";
//<9> <-> <5> <+> <2>
// --- ANALISADOR L�XICO
int count = 0;

string nextToken(){
  if (count == input.length())
    return to_string(EOF);

  while(input[count] == ' '){
      count++;
  }
  char x = input[count++];
  string token = string(1, x);

  while(isdigit(x) && isdigit(input[count]) ){
      token += string(1, input[count++ ]);
  }

  cout << "nextToken:" << token << endl;
  return token;
}
// --- ANALISADOR SINT�TICO
string lookahead;

attrib expr(){
  attrib e, t, r;
  t = term();
  r = rest();
  e.value = calc(t.value, r.op, r.value);
  return e;
}

attrib rest(){
  attrib e, t, r;
  e.op = NO_OP;

  if (lookahead == "-"){
     //-term rest
     match("-");
     e.op = MINUS;
     t = term();
     r = rest();

  } else if (lookahead == "+"){
    //+term rest
    match("+");
    e.op = SUM;
    t = term();
    r = rest();
  } else if (lookahead != to_string(EOF)) {
    //vazio
    //syntax error?
    error("Sem tempo irm�o");
  }
  e.value = calc(t.value, r.op, r.value);
  return e;
}

attrib term(){
  attrib r;
  r.op = NO_OP;
  /*switch (lookahead) {
    case "0": match("0"); r.value = 0; break;
    case "1": match("1"); r.value = 1; break;
    case "2": match("2"); r.value = 2; break;
    case "3": match("3"); r.value = 3; break;
    case "4": match("4"); r.value = 4; break;
    case "5": match("5"); r.value = 5; break;
    case "6": match("6"); r.value = 6; break;
    case "7": match("7"); r.value = 7; break;
    case "8": match("8"); r.value = 8; break;
    case "9": match("9"); r.value = 9; break;
    default: error("Number expected.");
  }*/
  string token = lookahead;
  match(token);
  std::string::size_type sz;
  r.value = stoi(token,&sz);
  return r;
}

void match(string c){
  if (lookahead == c) {
    lookahead = nextToken();
  } else {
    string s = "Found " + lookahead + " but expected " + c;
    error(s);
  }
}

int calc(int v1, int op, int v2){
  switch (op){
    case NO_OP : return v1;
    case SUM : return v1+v2;
    case MINUS : return v1-v2;
  }
  return 0;//error
}

void error(string msg){
  cout << endl << "Syntax Error: "<< msg << endl;
  cout << input << endl;
  for (int i = 0 ; i < count-1; i++)
     cout << " ";
     cout << "^";
   cout << endl;

}

void print(char c){
  cout << c;
}
// --- ENTRYPOINT
int main(){
  cout << "$ ";
  getline(cin, input);
  while (!input.empty()){
    lookahead = nextToken();
    attrib e = expr();
    cout << e.value << endl << "$ ";
    getline(cin, input);
    count = 0;
  }
}
