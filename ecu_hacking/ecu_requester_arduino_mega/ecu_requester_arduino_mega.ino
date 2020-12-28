unsigned char message_car[4];
unsigned char response_car[4];
unsigned char stop_car[4] = {0x12, 0, 0, 0};
bool got_message = true;

void setup() {
  Serial.begin(9600);
  Serial3.begin(1953);
}

void loop() {
  got_message = false;
  delay(100);
  char len = Serial.available();
  if (len > 0){
    unsigned char *input;
    input = (unsigned char*) malloc(10);
    len = Serial.readBytesUntil(10, input, 10);
    if(len != 8){
      Serial.println("error: invalid input");
    }else{
      for(unsigned char i = 0; i<4; i++){
        if(!isHexadecimalDigit(*(input + 2*i))){
          Serial.println("error: invalid input");
          break;
        }
        if(isDigit(*(input+ 2*i))){
          message_car[i] = 16 * (*(input + 2*i) - 48); // convert hex number in ascii format to number
        }else{
          message_car[i] = 16 * (*(input + 2*i) - 65 + 10); // convert hex number in ascii format to number
        }
        
        if(!isHexadecimalDigit(*(input + 2*i + 1))){
          Serial.println("error: invalid input");
          break;
        }
        if(isDigit(*(input + 2*i + 1))){
          message_car[i] = message_car[i] + (*(input + 2*i + 1) - 48);
        }else{
          message_car[i] = message_car[i] + (*(input + 2*i + 1) - 65 + 10);
        }
        got_message = true;
      }
    }
  }
  if(got_message){
    Serial3.write(message_car, 4);
//    delay(100);
//    Serial3.write(stop_car, 4);
  }
  if(Serial3.available() > 0){
//    Serial3.readBytes(response_car, 4);
//    for(unsigned char i = 0; i < 4; i++){
//      Serial.print(response_car[i], HEX);
//    }
    Serial.println(Serial3.read());
  }

}
