UDP udp;
FuelGauge battery;
int counter = 0;
int maxCount = 30;
int lightOn = 0;

//OSC compliant buffer to be populated with values
unsigned char buffer[] = {'/','l','e','f','t','\0','\0','\0',',','i','i','i','\0','\0','\0','\0','?','?','?','?','?','?','?','?','?','?','?','?'};

int LTFSR = 0; //Left Toe FSR current value
int LHFSR = 0; //Left Heel FSR current value
int LFLEX = 0; //Left Flex Sensor current value

void setup() {
    pinMode(D7, OUTPUT);
    digitalWrite(D7, LOW);

    udp.begin(9000);
}

void loop() {
    LTFSR = analogRead(A0);
    LHFSR = analogRead(A1);
    LFLEX = analogRead(A2);
    
    populate(LTFSR, 0);
    populate(LHFSR, 4);
    populate(LFLEX, 8);
    
    udp.beginPacket(IPAddress(172,20,10,2), 9000);
    udp.write(buffer, sizeof(buffer));
    udp.endPacket();
    
    counter = (counter+1)%maxCount;
    
    /*
    .getSoC() always returning zero??
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