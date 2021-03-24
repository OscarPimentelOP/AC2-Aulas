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
        print(cnt1++);
        delay(20000000); //1 hz
        print(cnt5++);
        delay(4000000); //5hz 20M/5
        print(cnt10++);
        delay(2000000); //10hz 20M/10       
    }
}