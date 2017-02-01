/***** CAMERA *****/
float camspeed   = 500;
float pitchspeed = 50;
float yawspeed   = 75;

float yaw = 315;
float pitch = -30;
vector3 eye = new vector3(-15*width, 1100, 15*width);
// TODO: add propersettings for camera init

/***** KEY BINDINGS *****/
char KEYpitchUp     = 'r'; char KEYpitchDown    = 'f';
char KEYyawLeft     = 'q'; char KEYyawRight     = 'e';
char KEYstrafeLeft  = 'a'; char KEYstrafeRight  = 'd';
char KEYstrafeFront = 'w'; char KEYstrafeBack   = 's';
char KEYstrafeUp    = 'z'; char KEYstrafeDown   = 'c';
char KEYmodeUP      = 'y'; char KEYmodeDOWN    = 't';
// TODO: add keys to interact
  // TODO: arrow keys (directional) to manipulate "paddle"
  // TODO: number key menu to spawn stuff (probably with default emitter modes)

char KEYspawnEmitter = 'x';
char KEYpaddleYP = 'u'; char KEYpaddleYN = 'o';
char KEYpaddleXP = 'l'; char KEYpaddleXN = 'j';
char KEYpaddleZP = 'i'; char KEYpaddleZN = 'k';
float padSpeed = 500;
float padX = width/2; float padY = 50; float padZ = -width/2; float padR = 300;

char KEYregen = 'g';
      
/***** REAL-TIME INTERACTION *****/
int emitterMode = 0;

/***** SIMULATION *****/
int   maxcount   = 200000;       //maximum number of particles

/***** EMITTERS *****/
int maxemitters  = 100;
emitter[] emitters = {  /* make larger than it actually is */
  new emitter(
    0, 2*PI,   0, width/2, //radius theta (around y-axis)
    width/2,   0, 100,   -width/2, //x y z
    -15, 15,   -10, -10,   -15, 15, //vx vy vz
    10, 20, //life
    255, 255,   195, 195,   0, 0,   255, 255, //r g b a
    0, 0, -25, -25, 0, 0, // acc x/y/z
    1, 1,                  //mass
    1000, -1),            //genrate/second
  new emitter(
    0, 2*PI,   0, width/2, //radius theta (around y-axis)
    width/2,   0, 100,   -width/2, //x y z
    -15, 15,   -10, -10,   -15, 15, //vx vy vz
    9, 11, //life
    255, 255,   195, 195,   0, 0,   255, 255, //r g b a
    0, 0, -25, -25, 0, 0,
    1, 1,
    5000, -1)
};

/***** PARTICLES *****/
float radius     = 10;           //particle size
boolean pFaceCam = true;