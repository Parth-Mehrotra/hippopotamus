#include<stdio.h>
#include<stdlib.h>

#include "mymalloc.h"
#include "myfree.h"

int main(int argc, char** argv)
{
	void* thing = malloc(50);
	void* thing2 = malloc(150);
	void* thing3 = malloc(100);
	return 0;
}
