#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FONT_L 1
#define FONT_M 0

#define SHIFT_LOWPART       0x10
#define MOVE_NONE 	        0x00
#define MOVE_UP             0x01
#define MOVE_DOWN           0x02
#define MOVE_LEFT           0x04
#define MOVE_RIGHT          0x08

struct font_info
{
	int start_addr;
	int font_size;
	int font_width;
};

const char* filename = "GUI_Resource_Full.bin";
int offset;

struct font_info FONT_INFO[2]=
{
	{0x0000503E,76,3}, // M size
	{0x004C513E,137,4}, // L size
};

void get_data_from_resource(int type,char *bitmap ,short unicode_addr)
{
    FILE *file;
    char *buffer;
    int i, j;
    int font_addr; //unicode_addr

	if(type > 1 )
	{
		printf("No define.\n");
		return ;		
	}
	printf("You select FONT font size %d, font start address is 0x%.04X\n\n",FONT_INFO[type].font_size, FONT_INFO[type].start_addr); 	

	buffer=(char *)malloc(FONT_INFO[type].font_size);
	if (!buffer)
	{
		fprintf(stderr, "Memory error!!\n");
		fclose(file);
		return ;
	}
    
	file = fopen(filename, "rb");
	if (!file)
	{
		fprintf(stderr, "Unable to open file %s!!\n", filename);
		//system("pause");
		return ;
	}    
	{
		font_addr = FONT_INFO[type].start_addr+256;  // 256 is font header.
	
		// get the full size unicode address.
		font_addr += unicode_addr*FONT_INFO[type].font_size;
		
		fseek(file, font_addr, SEEK_SET);
		fread(buffer, FONT_INFO[type].font_size, 1, file);
		memcpy(bitmap, buffer ,FONT_INFO[type].font_size);
	}

	fclose(file);
    return ;
    
}


void combine_bitmap(char *dst_bitmap , char *src_bitmap , int font_index , int type ,char shift_size)
{
    int i=0,j=0;
    unsigned short tmp=0;
    //unsigned int lowpart_start;
    unsigned int move_start;
    
    if(type & SHIFT_LOWPART)
        move_start = FONT_INFO[font_index].font_width - 2 + 1;  
    else
        move_start = 1;    

    printf("combine_bitmap action %x start %d\n",type, move_start);
    // high part start start on index 1
    // low  part
    if( type & MOVE_UP ) //move up, shift left //only hat need move up, hat is on the high part
    {
        for( i=move_start;i<FONT_INFO[font_index].font_size;i=i+FONT_INFO[font_index].font_width)
        {
			tmp=(unsigned char)src_bitmap[i]; 
			tmp=tmp << 8 |(unsigned char)src_bitmap[i+1];
			tmp =tmp << (shift_size & 0x0F);
			src_bitmap[i]=tmp>>8;
			src_bitmap[i+1]=tmp;			
        }    
    }    
    //else 
    if( type & MOVE_DOWN ) //move down, shift right //only shoe need move down, hat is on the high part
    {
        for( i=move_start;i<FONT_INFO[font_index].font_size;i=i+FONT_INFO[font_index].font_width)
        {

			tmp=(unsigned char)src_bitmap[i]; 
			tmp=tmp << 8 |(unsigned char)src_bitmap[i+1];
			//printf("bef shift right %x \n" ,tmp);
            //printf("SRC shift right %x %x \n" ,(unsigned char)src_bitmap[i],(unsigned char)src_bitmap[i+1]);
            tmp =tmp >> (shift_size & 0x0F);
			//printf("after shift right %x \n" ,tmp);
			src_bitmap[i]=tmp>>8;
			src_bitmap[i+1]=tmp;
        }
    }
    //else 
    if( type & MOVE_LEFT ) //move left, only shoe,hat need move left // shift all byte x = x+(font_width)
    {
        src_bitmap[0] = (src_bitmap[0]>(shift_size>>4))?(src_bitmap[0]-(shift_size>>4)):src_bitmap[0]; 
        printf("bef shift left %d \n" ,(shift_size>>4));
        for( i=1;i<FONT_INFO[font_index].font_size;i++)
        {
            if(i < (FONT_INFO[font_index].font_size - (FONT_INFO[font_index].font_width)*(shift_size>>4)))
            {
			    src_bitmap[i]=src_bitmap[i+(FONT_INFO[font_index].font_width*(shift_size>>4))];
			    
            }
            else
                src_bitmap[i]=0;
        }
    }

    if( type & MOVE_RIGHT ) //move left, only shoe,hat need move left // shift all byte x = x+(font_width)
    {
        puts("\rmove right");
        src_bitmap[0] = src_bitmap[0]+ (shift_size>>4);           
        for( i=FONT_INFO[font_index].font_size;i>=1;i--)
        {
            
            if(i <= (FONT_INFO[font_index].font_width*(shift_size>>4)))
            {
			    src_bitmap[i]=0;			    
            }
            else
            {
                src_bitmap[i]=src_bitmap[i-FONT_INFO[font_index].font_width*(shift_size>>4)];
            }    
        }        
    }  
    
    dst_bitmap[0] = (dst_bitmap[0] > src_bitmap[0])?dst_bitmap[0]:src_bitmap[0];
    for( i=1 ; i< FONT_INFO[font_index].font_size ; ++i)
    {
        dst_bitmap[i]=dst_bitmap[i] | src_bitmap[i];
    }    
}


void show_font(char *bitmap , int type)
{
    int i=0 , j=0;
    int print_size=0;
	if(type > 1 )
	{
		printf("No define.\n");
		return ;		
	}
    print_size = ((bitmap[0] * FONT_INFO[type].font_width))  ; //& 0xFFFFFFF8
	printf("You select FONT size =  %d %d \n",bitmap[0],print_size); 


    for(i=1; i<(print_size+1); i++)
    {
        for(j=7; j>-1; j--)
        {
            if((*(bitmap + i)>>j) & 0x01)
                printf("O");
            else
                printf(".");
        }
    
        if(i%(FONT_INFO[type].font_width) == 0)
        	printf("\n");
    }
    printf("\n");
    
}


void get_bitmap(int font_type, char *bitmap, short main_font, short *unicode, int action, char shift_size)
{
    char merge_shift = 0 ;
    char *tmp_bitmap=(char *)malloc(FONT_INFO[font_type].font_size);
    memset(tmp_bitmap, 0, FONT_INFO[font_type].font_size);
    get_data_from_resource(font_type ,tmp_bitmap,*unicode);
    //memcpy(bitmap, tmp_bitmap ,FONT_INFO[type].font_size);
    combine_bitmap(bitmap,tmp_bitmap,font_type,action, shift_size);
   
    offset++;

    // show_font(bitmap,type);
    // memset(bitmap, 0, sizeof(137));
    // combine_bitmap(bitmap,tmp_bitmap,137,3);
	// show_font(bitmap,type);
	// memset(bitmap, 0, 137);
    // combine_bitmap(bitmap,tmp_bitmap,137,2);    
	// show_font(bitmap,type);
	// show_font(tmp_bitmap,type);   
    free(tmp_bitmap);

    #if 1
    //printf(" get unicode %x %x\n", *unicode,*(unicode+1));
    if(*(unicode+1) >= 0x0e38 && *(unicode+1) <= 0x0e39) // shoe
    {
        printf(" merge shoe \n");
        if(main_font == 0x0e23)
            merge_shift = 2;
        merge_shift = (merge_shift<<4) | 1;
        get_bitmap(font_type,bitmap,main_font,unicode+1,SHIFT_LOWPART|MOVE_LEFT|MOVE_DOWN, merge_shift);
        //get_bitmap(font_type,bitmap, main_font,unicode+1,MOVE_DOWN|MOVE_LEFT);
        
    }
    else if( (*(unicode+1) >= 0x0e33 && *(unicode+1) <= 0x0e37)  || (*(unicode+1) >= 0x0e4c && *(unicode+1) <= 0x0e4e)
            || (*(unicode+1) == 0x0e31 ) ||   (*(unicode+1) == 0x0e47 )   //hat
           ) 
    {
        printf(" merge hat \n"); 
        if(main_font == 0x0e1b || main_font == 0x0e1d || main_font == 0x0e1f)// main body ,   
        {    
            merge_shift = 2 << 4;
            get_bitmap(font_type,bitmap,main_font,unicode+1,MOVE_LEFT, merge_shift); 
        }
        else if((*(unicode+1) == 0x0e33 ))
        {
            merge_shift = 6 << 4;
            get_bitmap(font_type,bitmap,main_font,unicode+1,MOVE_RIGHT, merge_shift); 
        }        
        else        
            get_bitmap(font_type,bitmap,main_font,unicode+1,MOVE_NONE, merge_shift);    
            //get_bitmap(font_type,bitmap,main_font,unicode+1,MOVE_UP);   
    }
    else if(*(unicode+1) >= 0x0e48 && *(unicode+1) <= 0x0e4B) //voice
    {
        printf(" merge voice \n");  
        get_bitmap(font_type,bitmap,main_font,unicode+1,MOVE_NONE, merge_shift);      
    }
    else
    {    
        return ;
    }

    #endif
}

//早安：อรุณ สวัสดิ์     \u0e2d\u0e23\u0e38\u0e13\u0020\u0e2a\u0e27\u0e31\u0e2a\u0e14\u0e34\u0e4c
//沒關係：ไม่เป็นไรค่ะ \x0e\x44\x0e\x21\x0e\x48\x0e\x40\x0e\x1b\x0e\x47\x0e\x19\x0e\x44\x0e\x23\x0e\x04\x0e\x48\x0e\x30
char* show_string[16][64]=
{
        {"\x01\x0e\x00\x00"},
        {"\x02\x0e\x2d\x0e\x1a\x0e\x04\x0e\x13\x0e\x38\x0e\x04\x0e\x23\x0e\x31\x0e\x1A\x0e\x00\x00"},
        {"\x21\x0e\x38\x0e\x48\x0e\x20\x00\x1e\x0e\x34\x0e\x48\x0e\x20\x00\x17\x0e\x31\x0e\x49\x0e\x00\x00"},//word test
        {"\x2a\x0e\x33\x0e\x20\x00\x1b\x0e\x35\x0e\x20\x00\x1b\x0e\x47\x0e\x20\x00\x23\x0e\x38\x0e\x20\x00\x23\x0e\x39\x0e\x49\x0e\x00\x00"}, //word test| สำ  ปี  ป็  รุ  รู้ 
		{"\x2d\x0e\x23\x0e\x38\x0e\x13\x0e\x20\x00\x2a\x0e\x27\x0e\x31\x0e\x2a\x0e\x14\x0e\x34\x0e\x4c\x0e\x00\x00"}, //早安
        {"\x2a\x0e\x33\x0e\x44\x0e\x21\x0e\x48\x0e\x40\x0e\x1b\x0e\x47\x0e\x19\x0e\x44\x0e\x23\x0e\x04\x0e\x48\x0e\x30\x0e\x00\x00"}, //沒關係
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
        type=0;
	}
	else if(c=='L' || c=='l')
	{
        type=1;
	}
	else
	{
		printf("No define.\n");
		return 0;
	}

	bitmap=(char *)malloc(FONT_INFO[type].font_size);
	if (!bitmap)
	{
		fprintf(stderr, "Memory error!!\n");
		return 0;
	}    
    printf("strlen %ld size %ld \n",strlen(show_string[2][0]),sizeof(show_string));
    memset(bitmap, 0, FONT_INFO[type].font_size);

    printf("Please select display array index :");
 	scanf("%d", &i);
    if(i >= 0 && i <= 9)
    {    
        printf("index = %d \n",i);
        ptr=(short *)show_string[i][0];    
    }
	else
	{
		printf("No define. %d \n",i);
		return 0;
	}   

#if 1
    while(*ptr!=0)     
    {
        offset = 0;
        memset(bitmap, 0, FONT_INFO[type].font_size);
        get_bitmap(type,bitmap,*ptr,ptr,MOVE_NONE,0);    
        show_font(bitmap,type);
        
        ptr+=offset;
    }    
#endif

	return 0;
}

