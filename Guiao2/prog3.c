void delay(int ms){
    for(; ms > 0; ms--){
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
} 

int cnt1 = 0;
int cnt5 = 0;
int cnt10 = 0;

void main(){

    while(1){
        printInt(cnt1++);
        delay(20000000); //1 hz
        printInt(cnt5++);
        delay(4000000); //5hz 20M/5
        printInt(cnt10++);
        delay(2000000); //10hz 20M/10       
    }
}