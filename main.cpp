

#include "RegularExpressions.h"
#include <iostream>
#include <fstream>

using namespace std;
using namespace RegularExpressions;

void CreateTestings(){
	string s;
	//s = "\\Baaaa\\b $"; // \Ba\B	a[a]a a[a]a		\ba  [a]aaaa
	//s = "($name1:(tyowuei|rwyoeitu|t{name2}){name1}) ($name2:(X|X{name1}|{name3})) ($name3:(X|{name1}))";
	//s+= "($name5:(ttt|w?|($name7:W{name7})){1,}X{name5})";
	// (($name2:X)Y{name3})?
	//s = "a(?:bc|(?:\\(|ok)|x)cc(1)\\Q+-()*/\\E";
	//s = "[^:.0-5--_]";
	//s = "[^\\S\\s\\D\\d\\W\\w \\f\\n\\r\\t\\v 1-7 \\--\\\\ A-]";
	//s = "AB: (ON(UU)XX|OFF(FF)WW)(?(1)A|B)(?:A|B)(?>A)\\Q+-()*/\\E (?smx-i) (?-i:tv) .\\S\\nW !\\x20! [^123A-B](A|B){10,}+X?";
	//s = "\\Q+-()*/\\E";
	/*
	RowRE*R = RowRE::parse(s);
	if(R){
		cout<<R->toString()<<endl;
		BaseFinder*BF = BaseFinder::ConvertRowRE(R);
		if(BF)cout<<BF->toString()<<endl;
		}
	*/

	string text;

	s = "e(v+?v+)+vaaaa";
	text = " evvvaaaabbcc+xx aaa";


	s = ".+?==(e|x)";
	text = "a== e==x utoieutieo==ex";

	s = "(?>x*?)xOK";
	text = "xxxxxxxxOK";

	s = "OK(?!x|c)";
	text = "OKQ";

	s = "(?<=xa|y |-|(Z+))OK";
	text = "xaOK y OK -OK  ZZZZZZZZOK";

	s = "(?(x)a)OK";
	text = "pOK";

	s = "B?\\1";
	text = "ABBA";

	s = "M?(?(((1)))a|b)";
	text = "Mab";

	s = "(?(?<=A)M|P)";
	text = ",PAP"; // "MAM,PAP";
	s = "X(A)?(?(2)M|P)";
	text = "XPMAM,PAP XAM";
	
	s = "<([A-Z])+>OK<(2)>";
	text = "<WO>OK<WO> <BAM>OK<BAM>";


	s = "OK$";
	text = "OK OK OK";

	s = "\\Baa\\B";
	text = "aaaaaa aaa";

	s = "(?-i)(?i:tv)set";
	text = "TVset TVSET tvset";

	s = "(?m)^AA$";
	text = "AA\nAA";


	s = "($name1:\\(({name1}?|A)\\)){name1}";
	text = "(((((((A)))))))";


	FindersMahine FM(s.c_str());
	FM.Build();
	cout<<FM.toString()<<endl;
	FM.RunParser(text.c_str());
}




string Load(const char*name){
	ifstream f(name);
	if(!f){
		cout<<"File "<<name<<" not found."<<endl;
		return NULL;
		}
	int length;
	f.seekg(0, ios::end);    // go to the end
	length = f.tellg();           // report location (this is the length)
	f.seekg(0, ios::beg);    // go back to the beginning
	char* text = new char[length+1];    // allocate memory for a buffer of appropriate dimension
	memset(text,0,length+1);
	f.read(text, length);       // read the whole file into the buffer
	f.close();                    // close file handle
	string R(text);
	delete[] text;
	return R;
}




void SuperExpressionTest(){
	string SuperExpression = Load("SuperExpression.txt");
	FindersMahine FM(SuperExpression.c_str());
	FM.Build();

	cout<<FM.toString()<<endl;

	RowRE* RRE = FM.RunParser(SuperExpression.c_str());
	if(!RRE)return;
	cout<<RRE->toString()<<endl;
	delete RRE;
}




void FullTesting(){
	string Expressions = Load("Expressions.txt");
	string Data = Load("Data.txt");
	string Expression = "[\\n\\r]*//-+[\\n\\r]*";
	FindersMahine FM(Expression.c_str());
	FM.Build();
	RowRE* RRE = FM.RunParser(Expressions.c_str());
	if(!RRE)return;
	int i,size = RRE->Row.size();
	for(i=0;i<size;++i){
		RowRE::OneTermRE* X = RRE->Row[i];
		RowRE*R = X->one.pr;
		bool isText = (*R->Row[0]->one.ps=="Text");
		if(!isText)continue;
		string expression = *R->Row[1]->one.ps;
		if(expression.empty())continue;
		if(expression[0]=='/')continue;
		cout<<"//========================================="<<endl;
		cout<<"Expression: "<<expression<<endl;
		FindersMahine FM(expression.c_str());
		FM.Build();
		RowRE* RR = FM.RunParser(Data.c_str());
		if(!RR)continue;
		int ii,size2 = RR->Row.size();
		for(ii=0;ii<size2;++ii){
			RowRE::OneTermRE* X = RR->Row[ii];
			RowRE*R = X->one.pr;
			bool isExpression = (*R->Row[0]->one.ps=="Expression");
			if(!isExpression)continue;
			R = R->Row[1]->one.pr;
			cout<<"//-----------------------------------------"<<endl;
			cout<<R->toString()<<endl;
			}
		delete RR;
		}
	delete RRE;
}




//-------------------------------------------------------------------------------------------------
void main(){
	//CreateTestings();
	//SuperExpressionTest();
	FullTesting();
}




