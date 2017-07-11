UDP udp;
FuelGauge battery;
int counter = 0;
const int maxCount = 20;
int lightOn = 0;

//OSC compliant buffer to be populated with values
unsigned char buffer[] = {'/','r','g','h','t','\0','\0','\0',',','i','i','i','\0','\0','\0','\0','?','?','?','?','?','?','?','?','?','?','?','?'};

int irVals[maxCount];
int RTFSR = 0; //Right Toe FSR current value
int RHFSR = 0; //Right Heel FSR current value
int RIRPR = 0; //Right IR sensor current value


void setup() {
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);

    udp.begin(9001);
}

/* This method contains the actions performed in a constant loop.
*/
void loop() {
    RTFSR = analogRead(A0);
    RHFSR = analogRead(A1); 
    irVals[counter] = analogRead(A2);
    
    populate(RTFSR, 0); 
    populate(RHFSR, 4); 
    populate(av(irVals), 8);
    
    //Send OSC buffer to offboard computer
    udp.beginPacket(IPAddress(172,20,10,2), 9001);
    udp.write(buffer, sizeof(buffer));
    udp.endPacket();
    
    counter = (counter+1)%maxCount;
    
    /*
    .getSoC() *****NOTE***** This library function always returns zero currently, must investigate 
    if(battery.getSoC() <= 10 && counter == 0){
        if(lightOn == 1){
            digitalWrite(D7, LOW);
            lightOn = 0;
        } else{
            digitalWrite(D7, HIGH);
            lightOn = 1;
        }
    }*/

    delay(5);
}

/* This method populates the section of the buffer starting from 16 + offset
*  with the given 32-bit integer, breaking the integer up into the 4 bytes
*  that comprise it.
*/
void populate(int val, int offset){
    buffer[16+offset] = (val>>24) & 0xFF;
    buffer[17+offset] = (val>>16) & 0xFF;
    buffer[18+offset] = (val>>8) & 0xFF;
    buffer[19+offset] = val & 0xFF;
}

/* This method averages the recent IR values to smooth the otherwise
*  very noisy data */
int av(int* addr){
    int total = 0;
    for(int i = 0; i < maxCount; i++){
        total = total + *(addr + i);
    }
    return total/maxCount;
}