/***** CAMERA *****/
float camspeed   = 500;
float pitchspeed = 50;
float yawspeed   = 75;

float yaw = 315;
float pitch = -25;
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
int emitterMode = 2;

/***** SIMULATION *****/
int   maxcount   = 200000;       //maximum number of particles

/***** STATIC COLLIDERS ****/

/***** EMITTERS *****/
int maxemitters  = 100;
emitter[] initEmitters = {  /* make larger than it actually is */
  new emitter(2, new vector3(width/2, 50, -width/2))
};

/***** PARTICLES *****/
float radius     = 10;           //particle size
boolean pFaceCam = true;
//float hry = radius ;//* pos[i].y / (0.75 * height);
//triangle(0, -hry*hry*0.4330127, 
//radius/2, hry*hry*0.4330127,
//-radius/2, hry*hry*0.4330127);
float[][] pMesh = { //init "mesh"
  {0,-radius/2,0},
  {-radius/2,radius/2,0},
  {radius/2,radius/2,0}};