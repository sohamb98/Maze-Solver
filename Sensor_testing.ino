#define s0 A6                 //Define sensor pins s0=>Leftmost
#define s1 A5                  //left_turn()
#define s2 A4                   //right_turn()
#define s3 A3                  //get_sesor_data() 
#define s4 A2                  //sensor_output()
#define s5 A1
#define s6 A0

int sensor[] = {s0,s1,s2,s3,s4,s5,s6};
int sensor_value[] = {0,0,0,0,0,0,0};
int sensor_weight[] = {-13,-5,-1,0,1,5 ,13};
int e;

void setup(){
  Serial.begin(9600);
  for(int i = 0; i<7; i++){
    pinMode(sensor[i], INPUT);
  }
}


void get_sensor_data(){
  for(int i = 0; i<7; i++)
    sensor_value[i] = analogRead(sensor[i]);
}

void sensor_output(){
  e = 0;
  for(int i = 0; i<7; i++){
    e = e + sensor_value[i]*sensor_weight[i];
  }
}

void print_sensor_data(){
  for(int i = 0; i<7; i++){
    Serial.print(sensor_value[i]); Serial.print("   ");
  }
  Serial.print("           ");Serial.print(e);
  Serial.println("");
} 

void loop(){
  get_sensor_data();
  sensor_output();
  print_sensor_data();
  delay(50);
}


