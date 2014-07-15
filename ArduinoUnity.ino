#include <Wire.h>

uint8_t outbuf[6];   
int cnt=0;

void setup(){
  //Serial.begin(19200);
  Serial.begin(9600);
  Wire.begin();
  Wire.beginTransmission(0x52);
  Wire.write(0x40);        
  Wire.write(0x00);
  Wire.endTransmission();
}

void loop(){
  Wire.requestFrom (0x52, 6);

  while(Wire.available()){
    outbuf[cnt] = nunchuk_decode_byte (Wire.read()); 
    cnt++;
  }    

  if(cnt >= 5) send_data();

  cnt=0;
  send_zero();

  Serial.flush();   
  delay(30);  
}

void send_data(){
  int joy_x_axis = outbuf[0];
  int joy_y_axis = outbuf[1];
  int accel_x_axis = outbuf[2] * 2 * 2;
  int accel_y_axis = outbuf[3] * 2 * 2;
  int accel_z_axis = outbuf[4] * 2 * 2;
  int z_button = 0;
  int c_button = 0;

  if((outbuf[5] >> 0) & 1) z_button = 1;
  if((outbuf[5] >> 1) & 1) c_button = 1;
  if((outbuf[5] >> 2) & 1) accel_x_axis += 2;
  if((outbuf[5] >> 3) & 1) accel_x_axis += 1;
  if((outbuf[5] >> 4) & 1) accel_y_axis += 2;
  if((outbuf[5] >> 5) & 1) accel_y_axis += 1;
  if((outbuf[5] >> 6) & 1) accel_z_axis += 2;
  if((outbuf[5] >> 7) & 1) accel_z_axis += 1;

  Serial.print(joy_x_axis, DEC);
  Serial.print("#");
  Serial.print(joy_y_axis, DEC);
  Serial.print("#");
  Serial.print(accel_x_axis, DEC);
  Serial.print("#");
  Serial.print(accel_y_axis, DEC);
  Serial.print("#");
  Serial.print(accel_z_axis, DEC);
  Serial.print("#");
  Serial.print(z_button, DEC);
  Serial.print("#");
  Serial.print(c_button, DEC);
  Serial.println();
}  


void send_zero(){
  Wire.beginTransmission (0x52);
  Wire.write(0x00);
  Wire.endTransmission ();
}

char nunchuk_decode_byte (char x){
  x = (x ^ 0x17) + 0x17;
  return x;
}

