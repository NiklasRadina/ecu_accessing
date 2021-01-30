unsigned char message_car[4] ;
unsigned char response_car[4];
unsigned char stop_car[4] = {0x12, 0, 0, 0};
bool got_message = true;

void setup() {
  Serial.begin(9600);
  Serial3.begin(1954, SERIAL_8E1);
//  while (Serial3.available()){
//    Serial3.read();
//  }
}

void print_message(unsigned char *message){
  for(char i=0; i<4; i++){
    Serial.print(*(message + i), HEX);
  }
  Serial.println();
}

void loop() {
  got_message = false;
//  delay(100);
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
    free(input);
  }
  
  if(got_message){
    Serial3.write(message_car, 4);
    Serial.println("message sent: ");
    print_message(message_car);
//    while(true){
//      if (Serial3.available() > 2){
//        Serial3.write(stop_car, 4);
//        break;
//      }
//      delay(10);
//    }
  }
  if(Serial3.available() > 2){
    Serial3.readBytes(response_car, 3);
    Serial.println("response: ");
//    for(unsigned char i = 0; i < 3; i++){
//      Serial.print(response_car[i], HEX);
//    }
    Serial.println();
    Serial.println(response_car[2] -50);
//    Serial.println(Serial3.read());

  }

}
