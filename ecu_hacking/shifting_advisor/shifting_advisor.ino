const unsigned char get_rpm_message[4] = {0x78, 0x13, 0x38, 0x00};
const unsigned char stop_car[4] = {0x12, 0, 0, 0};

volatile unsigned int raw_rpm_value = 0;
volatile bool rpm_changed = false;
bool advise_to_shift = false;
volatile unsigned char last_recieved_byte = 0; 

const long flashing_interval = 80;
unsigned long last_time_flashed = 0; 
byte flashing_leds_state = HIGH;



void flash_red_leds(){
  unsigned long current_time = millis();
  if(current_time - last_time_flashed >= flashing_interval){
    last_time_flashed = current_time;
    flashing_leds_state = !flashing_leds_state;
    digitalWrite(46, flashing_leds_state);
    digitalWrite(44, flashing_leds_state);
    digitalWrite(42, flashing_leds_state);
  } 
}

void update_rpm_display(unsigned char new_raw_value){ 
  unsigned int rpm = new_raw_value * 25;
  advise_to_shift = false;
  
  switch(rpm){

    case 1000 ... 1999:
      digitalWrite(50, LOW);
      digitalWrite(48, LOW);
      digitalWrite(46, LOW);
      digitalWrite(44, LOW);
      digitalWrite(42, LOW);
      digitalWrite(40, LOW);
      digitalWrite(38, LOW);
      
      digitalWrite(52, HIGH);
      digitalWrite(36, HIGH);
      break;
      
    case 2000 ... 2999:
      digitalWrite(48, LOW);
      digitalWrite(46, LOW);
      digitalWrite(44, LOW);
      digitalWrite(42, LOW);
      digitalWrite(40, LOW);
          
      digitalWrite(50, HIGH);
      digitalWrite(38, HIGH);
      break;
  
    case 3000 ... 3999:
      digitalWrite(46, LOW);
      digitalWrite(44, LOW);
      digitalWrite(42, LOW);
      
      digitalWrite(48, HIGH);
      digitalWrite(40, HIGH);
      break;
  
    case 4000 ... 4999:
      digitalWrite(44, LOW);
      
      digitalWrite(46, HIGH);
      digitalWrite(42, HIGH);
      break;
  
    case 5000 ... 5999:
      digitalWrite(44, HIGH);
      break;
      
    case 6000 ... 7200:
      advise_to_shift = true;
      break;
  
    default:
      for(int i =36; i<=52; i=i+2){
        digitalWrite(i, LOW);
      }
      break;
  }

}

void serialEvent3(){
  unsigned char incoming_byte = Serial3.read();
  switch(incoming_byte){
    case 0x13:
      last_recieved_byte = incoming_byte;
      break;
    case 0x38:
      if(last_recieved_byte == 0x13){
        last_recieved_byte = incoming_byte;
      }
      break;
    default:
      if(last_recieved_byte == 0x38){
        raw_rpm_value = incoming_byte;
        rpm_changed = true;
      } 
  }
}

void setup() {
  Serial3.begin(1954, SERIAL_8E1);

  pinMode(36, OUTPUT);
  pinMode(38, OUTPUT);
  pinMode(40, OUTPUT);
  pinMode(42, OUTPUT);
  pinMode(44, OUTPUT);
  pinMode(46, OUTPUT);
  pinMode(48, OUTPUT);
  pinMode(50, OUTPUT);
  pinMode(52, OUTPUT);

  Serial3.write(get_rpm_message, 4);
}

void loop() {
  if(rpm_changed){
    rpm_changed = false;
    update_rpm_display(raw_rpm_value);
  }
  if(advise_to_shift){
    flash_red_leds();
  }
}
