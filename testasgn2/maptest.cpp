//#include<bits/stdc++.h>
//NOTE
//object declaration without new on stack
//object declaration with new on heap


//c 
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
//c++
#include<iostream>
#include<map>
#include<iterator>
#include<list>
using namespace std;



class tcache
{
	public:
		//members
		int csize;
		int cmax;
		std::map<string, int> mymap; //hash table(map)
		std::map<string, int>::iterator mitr; //map cursor pointer
		
		tcache(int max)
		{
			cmax = max;
			csize = 0;
		}
		//methods
		void put(string key, int val)
		{
			if(csize < cmax)
			{
				mitr = mymap.find(key);
				if(mitr == mymap.end())
				{
					mymap.insert(make_pair(key, val));
					cout<<"insert:key = "<<key<<" val = "<<val<<endl;
					++csize;
				}
				else
				{
					//update value for key
					mitr->second = val;
					cout<<"update:key = "<<key<<" val = "<<val<<endl;
				}
				return;
			}
			cout << "cache full did not put "<< val << endl;
		}
		void get(string key)
		{
			mitr = mymap.find(key);
			if(mitr != mymap.end())
			{
				cout << key << " = " << mitr->second << endl;
				return; 
			}
			cout << key << " not found" << endl;
		}
};

int main(int argc, char** argv)
{
	tcache tob(3);
	
	string key1 = "kevin";
	tob.put(key1, 1);
	tob.get(key1);
	tob.put(key1, 5);
	tob.get(key1);

	cout << "mapsize = " << tob.mymap.size() << endl;
	return 0;
}

