#include  
#define __DEBUG__  
#ifdef __DEBUG__  
#define DEBUG(format,...) printf("File: "__FILE__", Line: %05d: "format"/n", __LINE__, ##__VA_ARGS__)  
#else  
#define DEBUG(format,...)  
#endif  
int main() {  
    char str[]="Hello World";  
    DEBUG("A ha, check me: %s",str);  
    return 0;  
}  