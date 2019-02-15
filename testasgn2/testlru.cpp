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
class tblock
{
	public:
		//members
		int bnum =-1;
		string selfkey;
		//constructor
		tblock()
		{
			//default constructor
		}
		tblock(int number, string conkey)
		{
			bnum = number;
			selfkey = conkey;
		}
		//no methods
		bool operator==(const tblock& other) const
		{
			if(selfkey == other.selfkey)
			{
				return true;
			}
			return false;
		}
};


class tcache
{
	public:
		//members
		int csize;
		int cmax;
		std::map<string, tblock*> mymap; //hash table(map)
		std::map<string, tblock*>::iterator mitr; //map cursor pointer
		std::list<tblock>::iterator litr; //list cursor pointer
		std::list<tblock> mylist; //linked list
		//constructor
		tcache(int max)
		{
			cmax = max;
			csize = 0;
		}
		//methods
		void put(string key, int val)
		{ 
			mitr = mymap.find(key);
			if(mitr == mymap.end())
			{
				//if here key not in map
				if(csize < cmax)
				{
					//if here room in cache
					++csize;
					insertBlock(key, val);
				}
				else
				{
				//if here cache is full
					poplru();
					insertBlock(key, val);
				}
				return;
			}
			//if here key exists
			//update and move to head of list
			mylist.remove(*(mitr->second));
			mitr->second->bnum = val;
			mylist.push_front(*(mitr->second));
		}

		void insertBlock(string key, int val)
		{
			//create block that holds value
			tblock* block = new tblock(val, key);
			//put block at head of list
			mylist.push_front(*block);
			//insert into map
			mymap.insert(make_pair(key, block));
		}

		void get(string key)
		{
			//look for blockkey
			mitr = mymap.find(key);
			if(mitr != mymap.end())
			{
				//if blockkey exists
				tblock tempblock = *(mitr->second);
				//move block to head of list
				mylist.remove(tempblock);
				mylist.push_front(tempblock);
				//return block contents
				//cout<<"get["<<key<<"] = "<<tempblock.bnum<<endl;
			}
			//return not found
		}

		void poplru()
		{
			--csize;
			//get key of tail node
			string keytodel = mylist.back().selfkey;
			//delete memory
			mitr = mymap.find(keytodel);
			delete mitr->second;
			//delete block from linked list
			mylist.pop_back();
			//delete item from hm
			mymap.erase(keytodel);	
		}
		void getmapsize()
		{
			cout <<"map = "<< mymap.size() << endl;
		}
		void getlistsize()
		{
			cout <<"list = "<< mylist.size() << endl;
		}
		void showlist()
		{
			for(litr = mylist.begin(); litr!=mylist.end(); ++litr)
			{
				cout << " | " << litr->selfkey<<"="<<litr->bnum;
			}
			cout << endl;
		}
};

int main(int argc, char** argv)
{
	
	tcache cob(3);
	string key1 = "A";
	string key2 = "B";
	string key3 = "C";
	string key4 = "D";
	
	cob.put(key1, 1);
	cob.put(key2, 2);
	cob.put(key3, 3);
	cob.showlist();
	cob.getmapsize();
	cob.getlistsize();
	cob.put(key2, 4);
	cob.showlist();
	cob.getmapsize();
	cob.getlistsize();


	//cob.poplru();
	//cob.poplru();
	//cob.poplru();



	return 0;
}

