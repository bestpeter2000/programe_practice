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
int offset;

#if 0
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

#endif

char* get_data_from_resource(int type,char *bitmap ,short unicode_addr)
{
    FILE *file;
    char *buffer;
    int i, j;
    int font_addr, font_addr_start, font_size, font_width; //unicode_addr
    if(type==0)
    {
		font_addr_start = 0x0000503E;
		font_size = 76;
		font_width = 3;
		printf("You select M font size, font start address is 0x%.04X\n\n", font_addr_start);        
    }
    else if(type==1)
    {
		font_addr_start = 0x004C513E;
		font_size = 137;
		font_width = 4;
		printf("You select L font size, font start address is 0x%.04X\n\n", font_addr_start);        
    }
    else
	{
		printf("No define.\n");
		return NULL;
	}
	buffer=(char *)malloc(font_size);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!!\n");
		fclose(file);
		return NULL;
	}
    
	file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s!!\n", filename);
		//system("pause");
		return NULL;
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
		memcpy(bitmap, buffer ,font_size);
		// printf("unicode: 0x%.04lX, font_addr: 0x%.04lX, font_width: %d\n\n", unicode_addr, font_addr, *buffer);
		// for(i=1; i<font_size; i++)
		// {
			// for(j=7; j>-1; j--)
			// {
				// if((*(buffer + i)>>j) & 0x01)
					// printf("O");
				// else
					// printf(".");
			// }
		
			// if(i%font_width == 0)
			// printf("\n");
		// }
		// printf("\n");
	}

	fclose(file);
	//free(buffer);  
    return buffer;
    
}

void combine_bitmap(char *dst_bitmap , char *src_bitmap , int size , int type)
{
    int i=0,j=0;
    unsigned int *ptr=NULL;
    unsigned int tmp=0;
    printf("combine_bitmap \n");
    if( type==1 ) //move up /  shift left
    {
       // j=(size-1)/4;
        # if 0
        for( i=1;i<64;i=i+4)
        {
            ptr=&src_bitmap[i];
            printf("bef shift right %x \n" ,(unsigned int)*ptr);
            printf("SRC shift right %x %x %x %x \n" ,(unsigned char)src_bitmap[i],(unsigned char)src_bitmap[i+1],(unsigned char)src_bitmap[i+2],(unsigned char)src_bitmap[i+3]);
            tmp =(unsigned int)*ptr << 1;
             printf("after shift right %x \n" ,tmp);
            *ptr=tmp;
        }
        #endif        
        
    }    
    else if( type==2 ) //move down shift right
    {
        //j=(size-1)/4;
        #if 0
        for( i=1;i<size;i=i+4)
        {
            ptr=&src_bitmap[i];
            // printf("bef shift right %d \n" ,*ptr);
            tmp =*ptr >> 1;
            *ptr=tmp;
            // printf("aft shift right %d \n" ,*ptr);
        }
            #endif
    }        
    for( i=0 ; i< size ; ++i)
    {
        dst_bitmap[i]=dst_bitmap[i] | src_bitmap[i];
    }    
}


void show_font(char *bitmap , int type)
{
    int i=0 , j=0;
    int font_addr, font_addr_start, font_size, font_width; //unicode_addr   
    if(type==0)
    {
		font_size = 76;
		font_width = 3;
		printf("You select M font size \n");        
    }
    else if(type==1)
    {
		font_size = 137;
		font_width = 4;
		printf("You select L font size \n");        
    }
    else
        printf("unknow \n"); 
    
    for(i=1; i<(font_size); i++)
    {
        for(j=7; j>-1; j--)
        {
            if((*(bitmap + i)>>j) & 0x01)
                printf("O");
            else
                printf(".");
        }
    
        if(i%font_width == 0)
        printf("\n");
    }
    printf("\n");
    
}


void get_bitmap(int type, char *bitmap , short *unicode)
{
    char *tmp_bitmap=(char *)malloc(137);
    get_data_from_resource(type ,tmp_bitmap,*unicode);
    memcpy(bitmap, tmp_bitmap ,137);
    printf(" get unicode %x %x\n", *unicode,*(unicode+1));
    offset++;
    // show_font(bitmap,type);
    // memset(bitmap, 0, sizeof(tmp_bitmap));
    // combine_bitmap(bitmap,tmp_bitmap,137,1);
    // show_font(bitmap,type);
    // memset(bitmap, 0, sizeof(tmp_bitmap));
    // combine_bitmap(bitmap,tmp_bitmap,137,2);    
    // show_font(bitmap,type);   
    #if 1
    if(*(unicode+1) >= 0x0e38 && *(unicode+1) <= 0x0e39) //shoe
    {
        get_bitmap(type,tmp_bitmap,unicode+1);
        combine_bitmap(bitmap,tmp_bitmap,137,2);
         printf(" print shoe \n");
    }
    else if( (*(unicode+1) >= 0x0e33 && *(unicode+1) <= 0x0e37)  || (*(unicode+1) >= 0x0e4c && *(unicode+1) <= 0x0e4e)
            || (*(unicode+1) == 0x0e31 ) ||   (*(unicode+1) == 0x0e47 )   //hat
           ) 
    {
        get_bitmap(type,tmp_bitmap,unicode+1);
        combine_bitmap(bitmap,tmp_bitmap,137,1); 
        printf(" print hat \n");    
    }
    else if(*(unicode+1) >= 0x0e48 && *(unicode+1) <= 0x0e4B) //voice
    {
        get_bitmap(type,tmp_bitmap,unicode+1);
        combine_bitmap(bitmap,tmp_bitmap,137,0);   
        printf(" print voice \n");        
    }
    else
    {    
        return ;
    }    
    #endif
}


char* show_string[16][32]=
{
    //{
        {"\x01\x0e\x00\x00"},
        {"\x02\x0e\x2d\x0e\x1a\x0e\x04\x0e\x13\x0e\x38\x0e\x04\x0e\x23\x0e\x31\x0e\x1A\x0e\x00\x00"},
        {"\x13\x0e\x38\x0e\x00\x00"},
        {"\x31\x00\x00\x00"},
   // },    
};

int main(void)
{
	char c;
    char *bitmap=NULL;
	int i, j;
	// int font_addr, font_addr_start, font_size, font_width, unicode_addr;
    //char show_string[64]={"\x44\x0e\x02\x0e\x48\x0e\x44\x0e\x01\x0e\x48\x0e\x00\x00"};
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
	bitmap=(char *)malloc(137);
	if (!bitmap)
	{
		fprintf(stderr, "Memory error!!\n");
		return 0;
	}    
    printf("strlen %ld size %ld \n",strlen(show_string[2][0]),sizeof(show_string));
    memset(bitmap, 0, sizeof(bitmap));
    //for(i=0 ;i<strlen(show_string);i=i+2)
    ptr=(short *)show_string[1][0];    
   // printf(" get unicode %x %x\n", *ptr,*(ptr+1));
#if 1
    while(*ptr!=0)     
    {
        offset = 0;
        //ptr=&show_string[i];
        //show_word(type,*ptr);
        get_bitmap(type,bitmap,ptr);
        
        show_font(bitmap,type);
        memset(bitmap, 0, sizeof(bitmap));
        ptr+=offset;
    }    
#endif

	return 0;
}

