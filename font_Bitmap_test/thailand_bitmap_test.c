#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FONT_L 1
#define FONT_M 0

struct font_info
{
	int start_addr;
	int font_size;
	int fidth_width;
};

const char* filename = "GUI_Resource_Full.bin";
int offset;

struct font_info FONT_INFO[2]=
{
	{0x0000503E,76,3},
	{0x004C513E,137,4},
};

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
	{
		font_addr = font_addr_start+256;  // 256 is font header.
	
		// get the full size unicode address.
		font_addr += unicode_addr*font_size;
		
		fseek(file, font_addr, SEEK_SET);
		fread(buffer, font_size, 1, file);
		memcpy(bitmap, buffer ,font_size);
	}

	fclose(file);
	//free(buffer);  
    return buffer;
    
}

void endian_swap(unsigned short* x)
{
	*x = (*x>>8)|(*x<<8);
}

void combine_bitmap(char *dst_bitmap , char *src_bitmap , int size , int type)
{
    int i=0,j=0;
    unsigned short *ptr=NULL;
    unsigned short tmp=0;
    printf("combine_bitmap \n");
    if( type==1 ) //move up  /shift left
    {
       // j=(size-1)/4;
        # if 1
        for( i=1;i<size;i=i+4)
        {
			tmp=(unsigned char)src_bitmap[i]; 
			tmp=tmp << 8 |(unsigned char)src_bitmap[i+1];
			tmp =tmp << 1;
			src_bitmap[i]=tmp>>8;
			src_bitmap[i+1]=tmp;			
        }
        #endif        
        
    }    
    else if( type==2 ) //move down shift right
    {
        //j=(size-1)/4;
        #if 1
        for( i=3;i<size;i=i+4)
        {
            // ptr=&src_bitmap[i];
            // tmp=*ptr;
			tmp=(unsigned char)src_bitmap[i]; 
			tmp=tmp << 8 |(unsigned char)src_bitmap[i+1];
			//printf("bef shift right %x \n" ,tmp);
            //printf("SRC shift right %x %x \n" ,(unsigned char)src_bitmap[i],(unsigned char)src_bitmap[i+1]);//(unsigned char)src_bitmap[i+2],(unsigned char)src_bitmap[i+3]
			// endian_swap(&tmp);
			// printf("endian change shift right %x \n" ,tmp);
            tmp =tmp >> 1;
			// printf("change shift right %x \n" ,tmp);
			// endian_swap(&tmp);
			//printf("after shift right %x \n" ,tmp);
			src_bitmap[i]=tmp>>8;
			src_bitmap[i+1]=tmp;
            // *ptr=(short)tmp;
			//printf("after SRC shift right %x %x \n" ,(unsigned char)src_bitmap[i],(unsigned char)src_bitmap[i+1]);						
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
    // memset(bitmap, 0, sizeof(137));
    // combine_bitmap(bitmap,tmp_bitmap,137,1);
	// show_font(bitmap,type);
	// memset(bitmap, 0, 137);
    // combine_bitmap(bitmap,tmp_bitmap,137,2);    
	// show_font(bitmap,type);
	// eshow_font(tmp_bitmap,type);   
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
//早安：อรณุ สวัสดิ์ （發音：阿倫莎瓦）\u0e2d\u0e23\u0e13\u0e38\u0020\u0e2a\u0e27\u0e31\u0e2a\u0e14\u0e34\u0e4c

char* show_string[16][32]=
{
        {"\x01\x0e\x00\x00"},
        {"\x02\x0e\x2d\x0e\x1a\x0e\x04\x0e\x13\x0e\x38\x0e\x04\x0e\x23\x0e\x31\x0e\x1A\x0e\x00\x00"},
        {"\x13\x0e\x38\x0e\x00\x00"},//\x13\x0e
        {"\x14\x0e\x34\x0e\x4c\x0e\x00\x00"}, 
		{"\x2d\x0e\x23\x0e\x13\x0e\x38\x0e\x20\x00\x2a\x0e\x27\x0e\x31\x0e\x2a\x0e\x14\x0e\x34\x0e\x4c\x0e\x00\x00"}, 
};

int main(void)
{
	char c;
    char *bitmap=NULL;
	int i, j;
    int type=0;
    short *ptr=NULL;

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

    ptr=(short *)show_string[3][0];    

#if 1
    while(*ptr!=0)     
    {
        offset = 0;
        get_bitmap(type,bitmap,ptr);
        
        show_font(bitmap,type);
        memset(bitmap, 0, sizeof(bitmap));
        ptr+=offset;
    }    
#endif

	return 0;
}

