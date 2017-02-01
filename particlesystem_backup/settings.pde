/***** CAMERA *****/
float camspeed   = 10;
float pitchspeed = 2;
float yawspeed   = 3;
// TODO: add propersettings for camera init

/***** KEY BINDINGS *****/
char KEYpitchUp     = 'r'; char KEYpitchDown    = 'f';
char KEYyawLeft     = 'a'; char KEYyawRight     = 'd';
char KEYstrafeLeft  = 'q'; char KEYstrafeRight  = 'e';
char KEYstrafeFront = 'w'; char KEYstrafeBack   = 's';
char KEYgenRatePlus = 'y'; char KEYgenRateMinus = 't';

char KEYdebug = 'h'; //really more akin to "stats"
char KEYregen = 'g';
      
/***** REAL-TIME INTERACTION *****/
float deltaGenRate = 500;

/***** SIMULATION *****/
int   maxcount   = 200000;       //maximum number of particles
int   startcount = 0;            //# particles upon regen

/***** PARTICLES *****/
emitter[] emitters = {
  new emitter(
    0, 2*PI,   0, width/8, //radius theta (around y-axis)
    width/2,   -300, -500,   -width/2, //x y z
    100, 100,   200, 200,   100, 100, //vx vy vz
    6, 12, //life
    255, 255,   195, 195,   0, 0,   255, 255, //r g b a
    1500, -1), // life (TBI)time
  new emitter(
    0, 2*PI,   0, width/2, //radius theta (around y-axis)
    width/2,   0, 0,   -width/2, //x y z
    100, 100,   -200, -200,   100, 100, //vx vy vz
    5, 8, //life
    255, 255,   195, 195,   0, 0,   255, 255, //r g b a
    500, -1)
};
//deprecated
float genRate    = 4000;//17500; //particles per second
float radius     = 15;           //particle size