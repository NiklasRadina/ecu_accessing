unsigned char memory[2][2] = {{0x1, 0x2}, {0x3, 0x4}};
unsigned char input[4];
unsigned char response[3];
bool sending = false;
unsigned char rom_id[3] = {0x4, 0x2, 0x0};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(1953);
  
}

void loop() {
  if (Serial.available() >= 4){
    Serial.readBytes(input, 4);
    switch (input[0]){
      case 0x12: // stop
        sending = false;
        break;
      case 0x78: // read data
        sending = true;
        response[0] = input [1];
        response[1] = input [2];
        response[2] = memory[input[1]][input[2]];
        break;
      case 0xAA:
        if (input[1] == response[0] && input[2] == response[1] && sending){
          memory[input[1]][input[2]] = input[3];
          response[2] = input[3];
        }
        break;
      case 0x00:
        if (input[1] == 0x46 && input[2] == 0x48 && input[3] == 0x49 && sending){
          for (char i = 0; i<3; i++){
            response[i] = rom_id[i];
          }
        }
      default:
        break;  
    }
  }
  if(sending){
    Serial.write(response, 3);
  }
  
}
