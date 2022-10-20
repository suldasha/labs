#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cstdio>

using namespace std;

//Класс строка
class String{
	char* line;
	int len;
public:
	int Len();
	String (){
		len = 0;
		line = new char [1];
	    line[0] = '\0';
	}; //String a;
	String (int l){
		len = 0;
		line = new char [l+1];
	    line[0] = '\0';
	};
	String (const char* s){
		line = new char [strlen(s)+1];
	    for (len = 0; line[len] = s[len]; len++);
	};
	String (const String& s){
		line = new char [s.len + 1];
	    for (len = 0; len < s.len; len++)
			line[len] = s.line[len];
	};
	~String(){
		delete [] line;
	};
	char& operator [] (int);
	bool operator == (String&);
	bool operator != (String&);
	String& operator = (String&);
	String& operator += (String&);
	String operator + (String&);
	friend ostream& operator << (ostream&, String&);
	int BM_poisk (String&);
	int KMP_poisk (String&);
	String operator () (int, int);
};
inline int String:: Len(){
	return len;
};
inline char& String:: operator [] (int x){
	if (x < 0 || x >= len){
		cout << "Error" << endl;
		exit (0);
	};
	return line[x];
}
inline bool String:: operator == (String& s){
	if (strcmp(line, s.line))
		return false;
	return true;
}
inline bool String:: operator != (String& s){
	if (strcmp(line, s.line))
		return true;
	return false;
}
inline String& String:: operator = (String& s){
	if (this != &s){
		delete [] line;
		line = new char [(len=s.Len())+1];
		strcpy(line, s.line);
	};
	return *this;
}
inline String& String:: operator += (String& s){
	char *t = new char [len+1];
	strcpy(t, line);
	delete [] line;
	len = len+s.Len();
	line = new char [len+1];
	strcpy(line, t);
	strcat(line, s.line);
	delete [] t;
	return *this;
}
inline String String:: operator + (String& s){
	String result(len+s.len+1);
	strcpy(result.line, line);
	strcat(result.line, s.line);
	result.len = strlen(result.line);
	return result;
}
inline ostream& operator << (ostream& out, String& s){
    out << s.line;
	return out;
}
inline int String:: BM_poisk (String& s){
	int result = -1;
	if (len < s.Len()) {
		return result;
	};
	int T [256] = {};	
	int i;
    for (i = 0; i < 256; i++)
	    T[i] = s.Len();
	for (i = 0; i < s.Len(); i++)
	    T[s[i]] = s.Len()-1-i; 
	i = s.len - 1;
	int j = s.Len() - 1;
	int k;
	int flag;
	while (i<len && j>=0){
		k = i;
		j = s.Len()-1;
		flag = 1;
		while (j >= 0 && flag == 1){
			if (line[k] == s[j]){
				j--;
				k--;
		    } else {
				i += T[line[i]];
				j = s.Len() - 1; 
				flag = 0;
			};
		};
	};
	if (j < 0)
		result = i + 1 - s.Len();
	return result;
}
inline int String:: KMP_poisk (String& s){
	int result = -1;
	int *NewJ = new int [s.Len()];
	int j = 0;
	int k = -1;
	int flag;
	NewJ[0] = -1;
	while (j < s.Len() - 1){
		flag = 1;
		while (k >= 0 && flag == 1){
			if (s[j] != s[k])
				k = NewJ[k];
			else 
				flag = 0;
		};
		k++;
		j++;
		if (s[j] == s[k])
			NewJ[j] = NewJ[k];
		else
			NewJ[j] = k;
	};
	int i = 0;
	j = 0;
	while (j < s.Len() && i < len){
		flag = 1;
		while (j>=0 && flag == 1){
			if (line[i] != s[j])
				j = NewJ[j];
			else
				flag = 0;
		};
		i++;
		j++;
	};
	if (i>=0)
		result = i - s.Len();
	delete [] NewJ;
	return result;
}
inline String String:: operator () (int i, int j){
	if (i<0 || i>=len || j<0 || j>=len)
		exit (0);
	if (i > j){
		int t = i;
		i = j;
		j = t;
	};
	String result (j-i+1);
	result.len = j-i+1;
	int c = 0;
	for (int k = i; k<=j; k++){
		result[c] = line[k];
		c++;
	};
	result.line[c] = '\0';
	return result;
}	
int main (){
	String s ("Red lorry, yellow lorry.");
	String p ("yellow");
	int bm = s.BM_poisk(p);
	if (bm == -1){
		cout << "Not found" << endl;
		return 0;
	};
	cout << "bm = " << bm << endl;
	int kmp = s.KMP_poisk(p);
	if (kmp == -1){
		cout << "Not found" << endl;
		return 0;
	};
	cout << "kmp = " << kmp << endl;
	String a = s(0,bm-1);
	String b = s(bm+p.Len(), s.Len()-1);
	String str = a + b;
	cout << str << endl;
	return 0;
}
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	