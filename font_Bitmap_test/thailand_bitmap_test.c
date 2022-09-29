#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// typedef unsigned char uint8;
// typedef unsigned int uint16;
// typedef unsigned long uint32;

// typedef char int8;
// typedef int int16;
// typedef long int32;
// typedef unsigned char BYTE;
// typedef char sint8;

const char* filename = "GUI_Resource_Full.bin";

int show_word(int type ,short unicode_addr)
{
    FILE *file;
    char *buffer;
    int i, j;
    int font_addr, font_addr_start, font_size, font_width; //unicode_addr
	file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s!!\n", filename);
		//system("pause");
		return 0;
	}
    if(type==0)
    {
		font_addr_start = 0x0000503E;
		font_size = 76;
		font_width = 3;
		printf("You select M font size, font start address is 0x%.04lX\n\n", font_addr_start);        
    }
    else if(type==1)
    {
		font_addr_start = 0x004C513E;
		font_size = 137;
		font_width = 4;
		printf("You select L font size, font start address is 0x%.04lX\n\n", font_addr_start);        
    }
    else
	{
		printf("No define.\n");
		return 0;
	}
	buffer=(char *)malloc(font_size);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!!\n");
		fclose(file);
		return 0;
	}
	//while(1)
	{
		font_addr = font_addr_start+256;  // 256 is font header.
		
		// printf("Please input the unicode (hex): ");
		// scanf("%x", &unicode_addr);
		
		// get the full size unicode address.
		font_addr += unicode_addr*font_size;
		
		fseek(file, font_addr, SEEK_SET);
		fread(buffer, font_size, 1, file);
		
		printf("unicode: 0x%.04lX, font_addr: 0x%.04lX, font_width: %d\n\n", unicode_addr, font_addr, *buffer);
		for(i=1; i<font_size; i++)
		{
			for(j=7; j>-1; j--)
			{
				if((*(buffer + i)>>j) & 0x01)
					printf("O");
				else
					printf(".");
			}
		
			if(i%font_width == 0)
			printf("\n");
		}
		printf("\n");
	}

	fclose(file);
	free(buffer);    
    return 0;
}

int main(void)
{
	// FILE *file;
	// char *buffer;
	char c;
	int i, j;
	// int font_addr, font_addr_start, font_size, font_width, unicode_addr;
    char show_string[64]={"\x31\x0e\x32\x0e\x00\x00"};
    int type=0;
    short *ptr=NULL;
	// Open GUI_Resource_Full.bin font file.
	// file = fopen(filename, "rb");
	// if (!file)
	// {
		// fprintf(stderr, "Unable to open file %s!!\n", filename);
		//system("pause");
		// return 0;
	// }

	printf("Please select font size M or L: ");
 	scanf("%c", &c);
	if(c=='M' || c=='m')
	{
		// font_addr_start = 0x0000503E;
		// font_size = 76;
		// font_width = 3;
		// printf("You select M font size, font start address is 0x%.04lX\n\n", font_addr_start);
        type=0;
	}
	else if(c=='L' || c=='l')
	{
		// font_addr_start = 0x004C513E;
		// font_size = 137;
		// font_width = 4;
		// printf("You select L font size, font start address is 0x%.04lX\n\n", font_addr_start);
        type=1;
	}
	else
	{
		printf("No define.\n");
		return 0;
	}
    
    printf("strlen %d size %d \n",strlen(show_string),sizeof(show_string));
    
    for(i=0 ;i<strlen(show_string);i=i+2)
    {
        ptr=&show_string[i];
        show_word(type,*ptr);
    }    


	return 0;
}

