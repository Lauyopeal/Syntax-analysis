#include <iostream>
#include <string>
#include <set>
#include <vector>
#include <map>
#include <cstdlib>
#include <cstring>
#include <stack>

using namespace std;
bool used[100];

class CSS
{
    public:
        string left;
        set<string> right;

        CSS( const string& temp )//构造函数参数为左值
        {
            left = temp;
            right.clear();
        }
        void print ( )//打印此非终结符对应的产生式
        {
            printf ( "%s->" , left.c_str() );
            set<string>::iterator it = right.begin();
            printf ( "%s" , it->c_str());
            it++;
            for ( ; it!= right.end() ; it++ )
                printf ( "|%s" , it->c_str() );
            puts("");
        }
        void insert ( const string& temp )//右值插入
        {
            right.insert(temp);
        }
};
vector<CSS> WF;//产生式集合
map<string,int> VN;//非终结符集合 int 对应第几条产生式
map<string,set<char> > first;//
map<string,set<char> > follow;//
vector<map<char,string> > predict_table;//
string start;

//旁组长
void print (void);//
void store_string(string input);//
//我
void rm_jianjie(void );//
void rm_zhijie(void );//
void dfs ( int x );
void jianhua(void);
void xiaochu(void);

//流质强
void mmm( int x );//
void make_first ( );//
void make_follow ( );//
//历程
bool check_first ( const string& text , char ch );//
bool check_follow ( const string& text , char ch );//
void make_table ();//
//去浪
void print1  ( int steps , stack<string> stk , string src , string wf , int x );
void analyse ( const string& src );
/*
S->Aa|Bb
A->a|Aa
B->Bb|b
*/
void tiqu( void )
{

    int flag=0;
    int i=0;
    string strtemp;

    while(flag < WF.size())//遍历每一条产生式
    {
        cout<<"****"<<flag<<"****"<<WF.size()<<endl;
        getchar();
        cout<<"-----1------\n";
       strtemp.clear();
       int i=0;
       set<string>::iterator it1=WF[flag].right.begin();
       set<string>::iterator it2=WF[flag].right.begin();
       set<string>::iterator it4=WF[flag].right.begin();
       strtemp+=it2->at(0);
       char ch=it2->at(0);

       for(it2 = WF[flag].right.begin(); it2!= WF[flag].right.end(); it2++)
       { cout<<"-----2------\n";

           if(it2->at(0)!=ch)
           {
               break;
           }
       }

       if(it2== WF[flag].right.end())
       {
           flag++;
           continue;
       }

       i++;
       it1=WF[flag].right.begin();

       while(it1 != WF[flag].right.end())
       {
            cout<<"-----3------\n";
            char ch=it1->at(i);
            for(it1 = it4,it4++; it1!= it2; it1++)
            {
                 cout<<"-----4------\n";
               if(it1->at(i)!=ch)
               {
                   string tt=WF[flag].left+"\"";
                   WF[flag].insert(strtemp+tt);
                    WF.push_back( CSS(tt));//加入新的非终结符
                   VN[tt] = WF.size();
                   int x = VN[tt]-1;

                   vector<string> cont;
                   set<string>& ss = WF[x].right;
                   for(set<string>::iterator it3=it4; it3!=it2; it3++)
                   {
                       ss.insert(it3->substr(i));
                       WF[flag].right.erase(it3);
                   }
                   break;
               }

            }
            if(it1 == it2)
            {
               strtemp+=it1->at(i);
               i++;
               it1=it4;
            }
            else
            {
               i=0;
               it1=it2;
               it4=it2;
               strtemp.clear();
               strtemp+=it2->at(0);
               char ch=it2->at(0);
               it1=it4;
               for(it2 = it1; it2!= WF[flag].right.end(); it2++)
               {
                    if(it2->at(0)!=ch)
                    {
                      break;
                    }
               }
               i++;
            }

        }
       flag++;
    }
}

int main()
{
    WF.clear();
    int n;
    int i=0;
    string input;
    cout<<"input the number of CSS\n";cin>>n;
    cout<<"input the CSS:\n";
    while(n--)
    {
        cin>>input;
        store_string(input);//文法存储
    }
    //rm_jianjie();
    rm_zhijie();
    //jianhua();
    //tiqu();
    print();
    make_first();
    make_follow();
    make_table();
    string in = "i*i+i";
    analyse( in );

   // print();

}
/*E->T|E+T
T->F|T*F
F->i|(E)*/

void store_string(string input)
{
    int i=input.find('-');
    string templ(input,0,i);
    //cout<<str<<endl;
    //CSS temp(str);

    if ( !VN[templ] )
    {
        WF.push_back (CSS(templ));
        VN[templ] = WF.size();
    }
    if(WF.size()==1)
    {start=templ;}
    int x = VN[templ]-1;
    templ.clear();
    string tempr(input,i+2,input.size());
    //cout <<"the right :  " << tempr << endl;
    string r_str;
    r_str.clear();
    for(int i=0;i<tempr.size();i++)
    {
       // cout<<"i:"<<i<<endl;
       //cout<<tempr.at(i);
        if(tempr.at(i)=='|')
        {
           // cout<<"--"<<r_str<<" ";
            WF[x].insert(r_str);
            r_str.clear();
            continue;
        }
        r_str+=tempr.at(i);
    }
    WF[x].insert(r_str);

}


void  print ()
{
    puts("**********print********");
    for ( int i = 0 ; i < WF.size() ; i++ )
        WF[i].print();
}

void rm_jianjie( )
{
    /*从最后一个产生式开始，逐步向前替换其他产生式右值*/
    for ( int i = (WF.size()-1) ; i>=0 ; i-- )
        for ( int j = (WF.size()-1) ; j > i ; j-- )
        {
            vector<string> cont;
            set<string>& right1 = WF[i].right;
            set<string>& right2 = WF[j].right;
            char ch = WF[j].left[0];
            set<string>::iterator it1 = right1.begin();
            set<string>::iterator it2;
            for ( ; it1 != right1.end() ; it1++ )
                if ( it1->at(0) == ch )
                    for ( it2 = right2.begin() ; it2 != right2.end() ; it2++ )
                        if(*it2 !="~")
                          cont.push_back ( *it2 + it1->substr(1) );
                        else
                          cont.push_back (it1->substr(1) );
            int nn = right1.size();
            while ( nn-- )
            {
                if ( right1.begin()->at(0) == ch )
                    right1.erase ( right1.begin() );
                else
                {
                    cont.push_back ( *(right1.begin()) );
                    right1.erase ( right1.begin() );
                }
            }
            for ( int i = 0 ; i < cont.size() ; i++ )
                right1.insert ( cont[i] );
        }
}
/*
E->E+T|T
T->T*F|F
F->P^F|P
P->(E)|I*/
void rm_zhijie ( )
{
    for ( int i = 0 ; i < WF.size() ; i++ )//遍历每一条产生式
    {
        char ch = WF[i].left[0];
       // set<string>& temp = WF[i].right;//此条产生式
        string tt = WF[i].left.substr(0,1)+"\'";//如果出现左递归使用的符号S S'
        bool flag = true;

        for ( set<string>::iterator it = WF[i].right.begin(); it != WF[i].right.end() ; it++ )
            if ( it->at(0) == ch )//出现左递归
            {
            //	cout<<"YES\n";
                WF.push_back( CSS(tt));//加入新的非终结符
                VN[tt] = WF.size();
              //  cout<<"WF.size():  "<<WF.size()<<endl;
                flag = false;
                break;
            }
        int x = VN[tt]-1;
        if ( flag ) continue;//此产生式不存在左递归

        vector<string> cont;
        set<string>& ss = WF[x].right;//新的
        WF[x].right.insert ( "~" );

        while (!WF[i].right.empty())
        {
            if ( WF[i].right.begin()->at(0) == ch )
            {
			    WF[x].right.insert(WF[i].right.begin()->substr(1)+tt);//p'
               // cout<<WF[i].right.begin()->substr(1)+tt<<endl;
			}
            else
            {
                // cout << "YES : " << WF[i].right.begin()->substr(0)+tt;
                //cout<<WF[i].right.begin()->substr(0)+tt<<endl;
                cont.push_back (WF[i].right.begin()->substr(0)+tt);//p
            }
            WF[i].right.erase(WF[i].right.begin());//delete
        }

        for ( int j = 0 ; j < cont.size() ; j++ )
        {
            WF[i].right.insert (cont[j]);
        }
    }

}

void dfs ( int x )
{
    if ( used[x] ) return;
    used[x] = 1;
    set<string>::iterator it = WF[x].right.begin();

    for ( ; it != WF[x].right.end(); it++ )
        for ( int i = 0 ; i < it->length() ; i++ )
            if ( isupper(it->at(i)) )
            {
                if ( it->length() > i+1 && it->at(i+1) == '\'' )
                    dfs ( VN[it->substr(i,2)]-1 );
                else
                    dfs ( VN[it->substr(i,1)]-1 );
            }
}

//化简
void jianhua(void)
{

    memset ( used , 0 , sizeof ( used ) );
    int x = VN[start]-1;
    dfs (x);
    cout<<"qqqqq\n";
    vector<CSS> res;
    for( int i = 0 ; i < WF.size() ; i++ )
        if( used[i] )
            res.push_back ( WF[i] );
    WF.clear();
    WF = res;
}


//检查一个字符是否属于一个字符串的FIRST集合
bool check_first ( const string& text , char ch )
{
    for ( int i = 0 ; i < text.length() ; i++ )
    {
        bool hasEmpty = false;
        if ( !isupper(text[i]) && text[i] != '\'' )
        {
            if ( text[i] != ch ) return false;
            else return true;
        }
        else if ( isupper(text[i] ) )
        {
            string temp;
            if ( i != text.length()-1 && text[i+1] == '\'' )
                temp = text.substr(i,2);
            else
                temp = text.substr(i,1);
            set<char>& dic = first[temp];
            set<char>::iterator it = dic.begin();
            for ( ; it != dic.end() ; it++ )
            {
                if ( *it == '~' ) hasEmpty = true;
                if ( *it == ch ) return true;
            }
            if ( !hasEmpty) break;
        }
        else continue;
    }
    return false;
}

//检查一个字符是否属于一个字符串的FOLLOW集合
bool check_follow ( const string& text , char ch )
{
    set<char>& dic = follow[text];
    set<char>::iterator it = dic.begin();
    for ( ; it != dic.end() ; it++ )
        if ( *it == ch ) return true;
    return false;
}

void make_table ()
{
    map<char,string> temp;
    vector<char> letter;
    bool vis[500];
    memset ( vis , 0 , sizeof ( vis ) );
    for ( int i = 0 ; i < WF.size() ; i++ )
    {
        set<string>& right = WF[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < it->length() ; j++ )
                if ( !isupper(it->at(j)) && it->at(j) != '\'' )
                {
                    if ( vis[it->at(j)] ) continue;
                    vis[it->at(j)] = true;
                    letter.push_back ( it->at(j) );
                }
    }
    for ( int i = 0 ; i < WF.size() ; i++ )
    {
        temp.clear();
        string& left = WF[i].left;
        set<string>& right = WF[i].right;
        set<string>::iterator it = right.begin();
        for ( ; it != right.end() ; it++ )
            for ( int j = 0 ; j < letter.size() ; j++ )
            {
                //cout << *it << " " <<  letter[j] << endl;
                if ( check_first ( *it , letter[j] ) )
                {
                    //cout << "YES" << endl;
                    temp[letter[j]] = *it;
                }
                if ( it->at(0) == '~' && check_follow ( left, letter[j] ))
                    temp[letter[j]] = *it;
            }
        predict_table.push_back ( temp );
    }

    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        printf ( "-" );
    puts ("");
    printf ( "|%9s" , "|" );
    for ( int i = 0 ; i < letter.size() ; i++ )
        printf ( "%5c%5s" , letter[i] , "|" );
    puts("");
    for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
        printf ( "-" );
    puts("");
    for ( int i = 0 ; i < WF.size(); i++ )
    {
        printf ( "|%5s%4s" , WF[i].left.c_str() , "|" );
        for ( int j = 0 ; j < letter.size() ; j ++ )
            if ( predict_table[i].count(letter[j] ) )
                printf ( "%7s%3s" , predict_table[i][letter[j]].c_str() , "|" );
            else printf ( "%10s" , "|" );
        puts("");
        for ( int i = 0 ; i <= (letter.size()+1)*10 ; i++ )
            printf ( "-" );
        puts ("");
    }
}

void print1 ( int steps , stack<string> stk , string src , string wf , int x )
{
    printf ( "%-10d" , steps );
    string out = "";
    while ( !stk.empty() )
    {
        out = stk.top()+out;
        stk.pop();
    }
    printf ( "#%-9s" , out.c_str() );
    out ="";
    for ( int i = x ; i < src.length() ; i++ )
        out += src[i];
    printf ( "%-10s" , (out+"#").c_str() );
    printf ( "%-10s\n" , wf.c_str() );
}

void analyse ( const string& src )
{
    stack<string> stk;
    stk.push ( "E" );
    int steps = 0;
    int idx = 0;
    printf ( "%-10s%-10s%-10s%-10s\n" , "步骤","符号栈","输入串","所用产生式" );
    string tmp="";
    print1 ( steps++,stk,src,tmp,idx );
    while ( !stk.empty() )
    {
        string u = stk.top();
        tmp.clear();

        stk.pop();
        if ( !isupper(u[0]) )
        {
            if ( idx == src.length() && u[0] == '~' );
            else
                if ( src[idx] == u[0] )
                idx++;
        }
        else
        {
            int x = VN[u]-1;
            tmp = predict_table[x][src[idx]];
            for ( int i = tmp.length()-1 ; i >= 0 ; i-- )
            {
                if ( tmp[i] == '\'' )
                {
                    string v = tmp.substr(i-1,2);
                    stk.push ( v );
                    i--;
                }
                else
                {
                    string v = tmp.substr(i,1);
                    stk.push( v );
                }
            }
            tmp = u+"->"+tmp;
        }
        print1 ( steps++,stk,src,tmp,idx );
    }
}
/*
E->E+T|T
T->T*F|F
F->P^F|P
P->(E)|*/
void mmm( int x )
{
    if ( used[x] ) return;
    used[x] = 1;
    string& left = WF[x].left;
    set<string>& right = WF[x].right;
    //cout<<"!!!:"<<*(right.begin())<<endl;
    set<string>::iterator it = right.begin();
    for ( ; it!= right.end() ; it++ )
        for ( int i = 0 ; i < it->length() ; i++ )
        {
            if ( !isupper( it->at(i) ) && it->at(i) != '\'' )
            {
                cout<<"Vt:"<<it->at(i)<<endl;
                first[left].insert ( it->at(i) );
                break;
            }
            if ( isupper( it->at(i) ) )
            {
                int y;
                if ( i != it->length()-1 && it->at(i+1) == '\'' )
                    y = VN[it->substr(i,2)]-1;
                else y = VN[it->substr(i,1)]-1;
                string& tleft = WF[y].left;
                mmm(y);
                set<char>& temp = first[tleft];
                set<char>::iterator it1 = temp.begin();
                bool flag = true;
                for ( ; it1 != temp.end() ; it1++ )
                {
                    if ( *it1 == '~' ) flag = false;
                    first[left].insert( *it1 );
                }

               /* for(int j=i;j<it->length();j++)
                {
                     if ( !isupper( it->at(j) ) && it->at(j) != '\'' )
                    {
                        cout<<"Vt:"<<it->at(j)<<endl;
                        first[left].insert ( it->at(j) );
                        break;
                    }
                }*/

                if ( flag ) break;
            }
            else continue;
        }
}
void make_first ( )
{
    memset (used,0,sizeof(used));
    for ( int i=0 ; i<WF.size(); i++)
        mmm(i);

    map<string,set<char> >::iterator it = first.begin();
    for ( ; it != first.end() ; it++ )
    {
        printf ( "FIRST(%s)={" , it->first.c_str() );
        set<char> & temp = it->second;
        set<char>::iterator it1 = it->second.begin();
        bool flag = false;
        for ( ; it1 != it->second.end() ; it1++ )
        {
            if ( flag ) printf ( "," );
            printf ( "%c" , *it1 );
            flag = true;
        }
        puts ("}");
    }

}

void append ( const string& str1 , const string& str2 )
{
    set<char>& from = follow[str1];
    set<char>& to = follow[str2];
    set<char>::iterator it = from.begin();
    for ( ; it != from.end() ; it++ )
        to.insert ( *it );
}

void make_follow ( )
{
    while ( true )
    {
        bool goon = false;
        for ( int i = 0 ; i < WF.size() ; i++ )
        {
            string& left = WF[i].left;
            set<string>& right = WF[i].right;
            set<string>::iterator it = right.begin();
            for ( ; it!= right.end() ; it++ )
            {
                bool flag = true;
                const string& str = *it;
                for ( int j = it->length()-1 ; j >= 0 ; j-- )
                {
                    if ( str[j] == '\'' )
                    {
                        int x = VN[it->substr(j-1,2)]-1;
                        if ( flag )
                        {
                            int tt = follow[it->substr(j-1,2)].size();
                            append ( left , it->substr(j-1,2) );
                            int tt1 = follow[it->substr(j-1,2)].size();
                            if ( tt1 > tt ) goon = true;
                            if ( !WF[x].right.count("~" ) )
                                flag = false;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper(str[k]) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j-1,2)];
                                int tt = to.size();
                                set<char>::iterator it1 = from.begin();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert ( *it1 );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !WF[VN[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j-1,2)].size();
                                follow[it->substr(j-1,2)].insert ( str[k] );
                                int tt1 = follow[it->substr(j-1,2)].size();
                                if ( tt1 > tt )
                                    goon = true;
                                break;
                            }
                            else continue;
                        }
                        j--;
                    }
                    else if ( isupper(str[j] ) )
                    {
                        int x = VN[it->substr(j,1)]-1;
                        if ( flag )
                        {
                            int tt = follow[it->substr(j,1)].size();
                            append ( left , it->substr(j,1) );
                            if ( !WF[x].right.count("~") )
                                flag = false;
                            int tt1 = follow[it->substr(j,1)].size();
                            if ( tt1 > tt ) goon = true;
                        }
                        for ( int k = j+1 ; k < it->length() ; k++ )
                        {
                            if ( isupper( str[k] ) )
                            {
                                string id;
                                if ( k != it->length()-1 && str[k+1] == '\'' )
                                    id = it->substr(k,2);
                                else id = it->substr(k,1);
                                set<char>& from = first[id];
                                set<char>& to = follow[it->substr(j,1)];
                                set<char>::iterator it1 = from.begin();
                                int tt = follow[it->substr(j,1)].size();
                                for ( ; it1 != from.end() ; it1++ )
                                    if ( *it1 != '~' )
                                        to.insert( *it1 );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                if ( !WF[VN[id]-1].right.count("~") )
                                    break;
                            }
                            else if ( str[k] != '\'' )
                            {
                                int tt = follow[it->substr(j,1)].size();
                                follow[it->substr(j,1)].insert ( str[k] );
                                int tt1 = follow[it->substr(j,1)].size();
                                if ( tt1 > tt ) goon = true;
                                break;
                            }
                            else continue;
                        }
                    }
                    else flag = false;
                }
            }
        }
        if ( !goon ) break;
    }
#define DEBUG
#ifdef DEBUG
    puts ("****************FOLLOW集**********************" );
    map<string,set<char> >::iterator it = follow.begin();
    for ( ; it != follow.end() ; it++ )
    {
        printf ( "FOLLOW(%s)={" , it->first.c_str() );
        set<char> & temp = it->second;
        temp.insert('#');
        set<char>::iterator it1 = temp.begin();
        bool flag = false;
        for ( ; it1 != temp.end() ; it1++ )
        {
            if ( flag ) printf ( "," );
            printf ( "%c" , *it1 );
            flag = true;
        }
        puts ("}");
    }
#endif
}


