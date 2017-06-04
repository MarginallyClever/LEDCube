//------------------------------------------------------------------------------
// LED Cube 4x4x4
// dan@marginallyclever.com
// based on work by http://www.instructables.com/id/4x4x4-LED-Cube-Arduino-Uno/
// pins 0-13,A0,A1 are cathodes (short leg) vertical columns.  Legs in a column are solered together.
// pins A2-A5 are layers.  Every layer is soldered together.
//
// By setting cathode pins HIGH I can block power flowing into the pin.
// so if I choose one anode HIGH and only one cathode LOW I can make power flow
// through a single light.
//
// To make my life a little easier I wrote in a buffer system:
// - a method that can display a WIDTH*LENGTH*HEIGHT buffer on the LEDs.
// - a setLight(x,y,z,state) method that finds the right buffer index for me.
// - a showImage(ms) that displays the buffer for ms milliseconds.
//------------------------------------------------------------------------------


//------------------------------------------------------------------------------
// DEFINES
//------------------------------------------------------------------------------


#define WIDTH  4  // x axis
#define LENGTH 4  // y axis
#define HEIGHT 4  // z axis
#define CUBE_CENTER (((float)WIDTH-1.0)*0.5)  // halfway between row 1 and row 2

//------------------------------------------------------------------------------
// GLOBALS
//------------------------------------------------------------------------------

// pins
int layer[HEIGHT] = {A2,A3,A4,A5};
int grid[WIDTH*LENGTH] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,A0,A1};

// buffer system
int image[WIDTH*LENGTH*HEIGHT];


//------------------------------------------------------------------------------
// METHODS
//------------------------------------------------------------------------------


void setup() {
  int i;
  for(i=0;i<WIDTH*LENGTH;++i) {
    pinMode(grid[i],OUTPUT);
  }
  for(i=0;i<HEIGHT;++i) {
    pinMode(layer[i],OUTPUT);
  }
}


void loop() {
  oneAtATime();
  allOn();
  showImage(200);
  allOff();
  showImage(200);
  flickerOn();
  flickerOff();
  sweepZ();
  sweepY();
  sweepX();
  aroundEdgeDown();
  randomFlicker();
  spiralInAndOut();
  randomRain();
  intersectingArbitraryPlane();
}


// light all the LEDs in the image buffer set to 1.
void showImage() {
  int x,y,z,i,j,k;
  for(z=0;z<HEIGHT;++z) {
    digitalWrite(layer[z],HIGH);
    i=z*WIDTH*LENGTH;
    for(y=0;y<LENGTH;++y) {
      for(x=0;x<WIDTH;++x) {
        j = y*LENGTH+x;
        k=image[i+j]?LOW:HIGH;
        digitalWrite(grid[j],k);
        //delayMicroseconds(300);
        //digitalWrite(grid[j],HIGH);
      }
    }
    digitalWrite(layer[z],LOW);
//*
    for(y=0;y<LENGTH;++y) {
      for(x=0;x<WIDTH;++x) {
        j = y*LENGTH+x;
        digitalWrite(grid[j],HIGH);
      }
    }
//*/
  }  
}


// show the buffer for ms milliseconds
void showImage(int ms) {
  long t=millis()+ms;
  while(millis()<t) showImage();
}


void oneAtATime() {
  int x,y,z;
  
  allOff();
  for(z=0;z<HEIGHT;++z) {
    for(y=0;y<LENGTH;++y) {
      for(x=0;x<WIDTH;++x) {
        //light(x,y,z);
        setImage(x,y,z,1);
        showImage(200);
        setImage(x,y,z,0);
      }
    }
  }
}


void setImage(int x,int y,int z,int state) {
  image[z*WIDTH*LENGTH + y*WIDTH + x]=state;
}


void allOn() {
  int i;
  for(i=0;i<HEIGHT*WIDTH*LENGTH;++i) {
    image[i]=1;
  }
}


void allOff() {
  int i;
  for(i=0;i<HEIGHT*WIDTH*LENGTH;++i) {
    image[i]=0;
  }
}

void turnColumnsOff() {
  for(int i = 0; i<WIDTH*HEIGHT; i++) {
    digitalWrite(grid[i], HIGH);
  }
}


void flickerOn() {
  int i = 150;
  while(i != 0) {
    allOn();
    showImage(i);
    allOff();
    showImage(i);
    i-= 5;
  }
}


void flickerOff() {
  int i = 0;
  while(i != 150) {
    allOn();
    showImage(i);
    allOff();
    showImage(i);
    i+= 5;
  }
}


/**
 * Turn on one light without using the image buffer
 * @param x the x coordinate
 * @param y the y coordinate
 * @param z the z coordinate
 * @deprecated
 */
void light(int x,int y,int z) {
  int i;
  for(i=0;i<HEIGHT;++i) {
    digitalWrite(layer[i],i==z?HIGH:LOW);
  }

  int target=y*WIDTH+x;
  for(i=0;i<WIDTH*LENGTH;++i) {
    digitalWrite(grid[i],i==target?LOW:HIGH);
  }
}


/**
 * control one layer along the Z axis
 * @param z the axis
 * @param state on or off
 */
void lightZ(int z,int state) {
  z*=WIDTH*LENGTH;
  int i;
  for(i=0;i<WIDTH*LENGTH;++i) {
    image[z++]=state;
  }
}


void sweepZ() {
  allOff();
  
  int x = 75;  // animation delay (ms)
  for(int j = 4; j >=0; j--) {  // 4 repeats
    allOn();
    for(int i = HEIGHT-1; i>=0; i--) {
      lightZ(i, 0);
      showImage(x);
    }
    for(int i = 0; i<HEIGHT; i++) {
      lightZ(i, 1);
      showImage(x);
    }
    for(int i = 0; i<HEIGHT; i++) {
      lightZ(i, 0);
      showImage(x);
    }
    for(int i = HEIGHT-1; i>=0; i--) {
      lightZ(i, 1);
      showImage(x);
    }
  }
}


/**
 * control one layer along the Y axis
 * @param y the axis
 * @param state on or off
 */
void lightY(int y,int state) {
  int z,x;
  for(z=0;z<HEIGHT;z++) {
    for(x=0;x<WIDTH;x++) {
      setImage(x,y,z,state);
    }
  }
}

void sweepY() {
  allOff();

  int x = 75;  // animation delay (ms)
  for(int j = 4; j >=0; j--) {  // 4 repeats
    allOn();
    for(int i = HEIGHT-1; i>=0; i--) {
      lightY(i, 0);
      showImage(x);
    }
    for(int i = 0; i<HEIGHT; i++) {
      lightY(i, 1);
      showImage(x);
    }
    for(int i = 0; i<HEIGHT; i++) {
      lightY(i, 0);
      showImage(x);
    }
    for(int i = HEIGHT-1; i>=0; i--) {
      lightY(i, 1);
      showImage(x);
    }
  }
}


/**
 * control one layer along the X axis
 * @param x the axis
 * @param state on or off
 */
void lightX(int x,int state) {
  int z,y;
  for(z=0;z<HEIGHT;z++) {
    for(y=0;y<LENGTH;y++) {
      setImage(x,y,z,state);
    }
  }
}

void sweepX() {
  allOff();

  int x = 75;  // animation delay (ms)
  for(int j = 4; j >=0; j--) {  // 4 repeats
    allOn();
    for(int i = HEIGHT-1; i>=0; i--) {
      lightX(i, 0);
      showImage(x);
    }
    for(int i = 0; i<HEIGHT; i++) {
      lightX(i, 1);
      showImage(x);
    }
    for(int i = 0; i<HEIGHT; i++) {
      lightX(i, 0);
      showImage(x);
    }
    for(int i = HEIGHT-1; i>=0; i--) {
      lightX(i, 1);
      showImage(x);
    }
  }
}


// spiral downwards
void aroundEdgeDown() {
  allOff();
  for(int ms = 200; ms != 0; ms -=50) {  // 4 loops, faster each time.
    for(int z = HEIGHT-1; z>=0; z--) {      
      setImage(0,0,z,1);  showImage(ms);  setImage(0,0,z,0);
      setImage(0,1,z,1);  showImage(ms);  setImage(0,1,z,0);
      setImage(0,2,z,1);  showImage(ms);  setImage(0,2,z,0);
      setImage(0,3,z,1);  showImage(ms);  setImage(0,3,z,0);
      setImage(1,3,z,1);  showImage(ms);  setImage(1,3,z,0);
      setImage(2,3,z,1);  showImage(ms);  setImage(2,3,z,0);
      setImage(3,3,z,1);  showImage(ms);  setImage(3,3,z,0);
      setImage(3,2,z,1);  showImage(ms);  setImage(3,2,z,0);
      setImage(3,1,z,1);  showImage(ms);  setImage(3,1,z,0);
      setImage(3,0,z,1);  showImage(ms);  setImage(3,0,z,0);
      setImage(2,0,z,1);  showImage(ms);  setImage(2,0,z,0);
      setImage(1,0,z,1);  showImage(ms);  setImage(1,0,z,0);
    }
  }
}


void randomFlicker() {
  allOff();
  for(int i = 0; i<250; ++i) {
    int rz = random(0,HEIGHT);
    int ry = random(0,LENGTH);
    int rx = random(0,WIDTH);
    setImage(rx,ry,rz,1);
    showImage(50);
    setImage(rx,ry,rz,0);
  }
}


void randomRain() {
  allOff();
  for(int i = 0; i<50; ++i) {
    int ry = random(0,LENGTH);
    int rx = random(0,WIDTH);
    for(int z=0;z<HEIGHT;z++) {
      setImage(rx,ry,z,1);
      showImage(100);
      setImage(rx,ry,z,0);
    }
  }
}

/**
 * Spin a plane around the center of the cube
 * anything within a short distance of the plane is lit.
 * See also https://www.siggraph.org/education/materials/HyperGraph/modeling/mod_tran/3drota.htm
 */
void intersectingArbitraryPlane() {
  allOff();

  float rx,ry,rz, px,py,pz, nx,ny,nz, len;
  long tEnd = millis() + 10000;  // 10 seconds
  while(millis()<tEnd) {
    float t=millis();
    rx = t*0.002;  // adjust x rotate speed
    ry = t*0.001;  // adjust y rotate speed
    rz = t*0.003;  // adjust z rotate speed

    px = 1;
    py = 0;
    pz = 0;

    // rotate Z
    nx = px * cos(rz) - py * sin(rz);
    ny = px * sin(rz) + py * cos(rz);
    nz = pz;
    px = nx;
    py = ny;
    pz = nz;

    // rotate Y
    nx = px * cos(ry) - pz * sin(ry);
    ny = py;
    nz = px * sin(ry) + pz * cos(ry);
    px = nx;
    py = ny;
    pz = nz;

    // rotate X
    nx = px;
    ny = py * cos(rx) - pz * sin(rx);
    nz = py * sin(rx) + pz * cos(rx);

    // normalize length
    len = 1.0/sqrt(nx*nx + ny*ny + nz*nz);
    nx*=len;
    ny*=len;
    nz*=len;

    float x,y,z;
    for(z=0;z<HEIGHT;++z) {
      for(y=0;y<LENGTH;++y) {
        for(x=0;x<WIDTH;++x) {
          float dot = (x-CUBE_CENTER)*nx
                    + (y-CUBE_CENTER)*ny
                    + (z-CUBE_CENTER)*nz;
          setImage(x,y,z,dot>=-0.5 && dot<=0.5);
        }
      }
    }
    showImage(5);
  }
}

void spiralInAndOut() {
  allOff();
  int x = 60;
  for(int z = 0; z<HEIGHT; z++) {
    //spiral outside to inside
    setImage(0,0,z,1);  showImage(x);
    setImage(1,0,z,1);  showImage(x);
    setImage(2,0,z,1);  showImage(x);
    setImage(3,0,z,1);  showImage(x);
    setImage(3,1,z,1);  showImage(x);
    setImage(3,2,z,1);  showImage(x);
    setImage(3,3,z,1);  showImage(x);
    setImage(2,3,z,1);  showImage(x);
    setImage(1,3,z,1);  showImage(x);
    setImage(0,3,z,1);  showImage(x);
    setImage(0,2,z,1);  showImage(x);
    setImage(0,1,z,1);  showImage(x);
    setImage(1,1,z,1);  showImage(x);
    setImage(2,1,z,1);  showImage(x);
    setImage(2,2,z,1);  showImage(x);
    setImage(1,2,z,1);  showImage(x);

    // spiral out in reverse
    setImage(1,2,z,0);  showImage(x);
    setImage(2,2,z,0);  showImage(x);
    setImage(2,1,z,0);  showImage(x);
    setImage(1,1,z,0);  showImage(x);
    setImage(0,1,z,0);  showImage(x);
    setImage(0,2,z,0);  showImage(x);
    setImage(0,3,z,0);  showImage(x);
    setImage(1,3,z,0);  showImage(x);
    setImage(2,3,z,0);  showImage(x);
    setImage(3,3,z,0);  showImage(x);
    setImage(3,2,z,0);  showImage(x);
    setImage(3,1,z,0);  showImage(x);
    setImage(3,0,z,0);  showImage(x);
    setImage(2,0,z,0);  showImage(x);
    setImage(1,0,z,0);  showImage(x);
    setImage(0,0,z,0);  showImage(x);

    
    //spiral outside to inside - counter clockwise
    setImage(0,0,z,1);  showImage(x);
    setImage(0,1,z,1);  showImage(x);
    setImage(0,2,z,1);  showImage(x);
    setImage(0,3,z,1);  showImage(x);
    setImage(1,3,z,1);  showImage(x);
    setImage(2,3,z,1);  showImage(x);
    setImage(3,3,z,1);  showImage(x);
    setImage(3,2,z,1);  showImage(x);
    setImage(3,1,z,1);  showImage(x);
    setImage(3,0,z,1);  showImage(x);
    setImage(2,0,z,1);  showImage(x);
    setImage(1,0,z,1);  showImage(x);
    setImage(1,1,z,1);  showImage(x);
    setImage(1,2,z,1);  showImage(x);
    setImage(2,2,z,1);  showImage(x);
    setImage(2,1,z,1);  showImage(x);

    // spiral out in reverse
    setImage(2,1,z,0);  showImage(x);
    setImage(2,2,z,0);  showImage(x);
    setImage(1,2,z,0);  showImage(x);
    setImage(1,1,z,0);  showImage(x);
    setImage(1,0,z,0);  showImage(x);
    setImage(2,0,z,0);  showImage(x);
    setImage(3,0,z,0);  showImage(x);
    setImage(3,1,z,0);  showImage(x);
    setImage(3,2,z,0);  showImage(x);
    setImage(3,3,z,0);  showImage(x);
    setImage(2,3,z,0);  showImage(x);
    setImage(1,3,z,0);  showImage(x);
    setImage(0,3,z,0);  showImage(x);
    setImage(0,2,z,0);  showImage(x);
    setImage(0,1,z,0);  showImage(x);
    setImage(0,0,z,0);  showImage(x);
  }
}

