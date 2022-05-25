#include "des.h"

int main (){
	char input[] = "02468aceeca86420";
	char key[] = "0f1571c947d9e859";
	char output[16];
	encrypt(input,output,key,16);
	printf("%.16s\n",output);
}
