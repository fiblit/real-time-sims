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

/***** UNIVERSAL PER PARTICLE *****/
float genRate    = 4000;//17500; //particles per second
float radius     = 5;           //particle size