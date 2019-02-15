//sample class implementation file
#include<stdio.h>
#include"sampleclass.h"

//default construcutor
SampleClass::SampleClass()
{
	counter = -1;
}
//constructor
SampleClass::SampleClass(int count)
{
	counter = count;
}

//class method
void SampleClass::addone()
{
	counter++;
	printf("counter = %d\n", counter);
}