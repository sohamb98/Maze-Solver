#define s0 A6                 //Define sensor pins s0=>Leftmost
#define s1 A5                  //left_turn()
#define s2 A4                   //right_turn()
#define s3 A3                  //get_sesor_data() 
#define s4 A2                  //sensor_output()
#define s5 A1
#define s6 A0

#define rm1 22
#define rm2 24
#define lm1 25
#define lm2 23
#define renenable 2
#define lenenable 3

#define threshold 160
#define slow_speed 160
#define delay_straight 220
#define delay_turn 200

#define Kp 0.05

int error = 0; int prev_error = 0; int e=0;
int renenable_value = 160;
int lenenable_value = 160;

int sensor[] = {s0,s1,s2,s3,s4,s5,s6};
int sensor_value[] = {0,0,0,0,0,0,0};
int sensor_weight[] = {-13,-5,-1,0,1,5,13};

void setup(){
  Serial.begin(9600);
  for(int i = 0; i<7; i++){
    pinMode(sensor[i], INPUT);
  }
  pinMode(rm1, OUTPUT);
  pinMode(rm2, OUTPUT);
  pinMode(lm1, OUTPUT);
  pinMode(lm2, OUTPUT);
  pinMode(renenable, OUTPUT);
  pinMode(lenenable, OUTPUT);
  
  Serial.println("In setup");
  
}


void get_sensor_data(){
  for(int i = 0; i<7; i++)
    sensor_value[i] = analogRead(sensor[i]);
}

void error_calculation(){
  error = 0;
  for(int i = 0; i<7; i++){
    error = error + sensor_value[i]*sensor_weight[i];
  }
}



void right_90(){
  slow_down();
  delay(delay_straight);
  do{
    digitalWrite(rm1, LOW);
    digitalWrite(rm2, HIGH);
    digitalWrite(lm1, HIGH);
    digitalWrite(lm2, LOW);
    analogWrite(renenable, slow_speed);
    analogWrite(lenenable, slow_speed);
    //delay(delay_turn); 
  }
  while(analogRead(sensor[3]) > 600 );
}


void left_90(){
  slow_down();
  delay(delay_straight);
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  digitalWrite(lm1, LOW);
  digitalWrite(lm2, HIGH);
  analogWrite(renenable, slow_speed);
  analogWrite(lenenable, slow_speed);
  delay(delay_turn);
  do{
    digitalWrite(rm1, HIGH);
    digitalWrite(rm2, LOW);
    digitalWrite(lm1, LOW);
    digitalWrite(lm2, HIGH);
    analogWrite(renenable, slow_speed);
    analogWrite(lenenable, slow_speed);
    //delay(delay_turn);
  }
  while(analogRead(sensor[3]) > 600 );
}


void slow_down(){
  analogWrite(renenable, slow_speed);
  analogWrite(lenenable, slow_speed);
}

void smooth_turn(int renenable_value, int lenenable_value){
  analogWrite(renenable, renenable_value);
  analogWrite(lenenable, lenenable_value);
}

void control_block(){
  e = error*Kp;
  
  if(error>10000)                                    //Sure left
    left_90();
  else if(error<-10000)                                   //Sure right
    right_90();
  else if(analogRead(sensor[1])<600 && analogRead(sensor[5]<600)){
    
    left_90(); // Replace this with right_90() if right preferance algorithm is applied
  }
  
  else if(analogRead(sensor[0])>600 && analogRead(sensor[1])>600 && analogRead(sensor[2])>600 && analogRead(sensor[3])>600 && analogRead(sensor[4])>600 && analogRead(sensor[5])>600 && analogRead(sensor[6])>600 ){
    left_90();
  }
  else{
    renenable_value = threshold + e;
    lenenable_value = threshold - e;
   
  if(renenable_value>255)
    renenable_value = 255;
  else if(renenable_value<0)
    renenable_value = 0;
  if(lenenable_value>255)
    lenenable_value = 255;
  else if(renenable_value<0)
    lenenable_value = 0;
     
  smooth_turn(renenable_value, lenenable_value);
 }
}


void loop(){
  digitalWrite(rm1, HIGH);
  digitalWrite(rm2, LOW);
  
  digitalWrite(lm1, HIGH);
  digitalWrite(lm2, LOW);
  
  get_sensor_data();
  error_calculation();
  control_block();
  
  //print_sensor_data();
}


