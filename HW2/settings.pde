/***** CAMERA *****/
float camspeed   = 500;
float pitchspeed = 50;
float yawspeed   = 75;

float yaw = 270;
float pitch = 0;
Vec3 eye;

/***** KEY BINDINGS *****/
char KEYpitchUp     = 'r'; char KEYpitchDown    = 'f';
char KEYyawLeft     = 'q'; char KEYyawRight     = 'e';
char KEYstrafeLeft  = 'a'; char KEYstrafeRight  = 'd';
char KEYstrafeFront = 'w'; char KEYstrafeBack   = 's';
char KEYstrafeUp    = 'z'; char KEYstrafeDown   = 'c';
char KEYmodeUP      = 't'; char KEYmodeDOWN     = 'g';
  // TODO: number key menu to spawn stuff (probably with default emitter modes)

char KEYspawnEmitter = 'x';
char KEYpaddleYP = 'u'; char KEYpaddleYN = 'o';
char KEYpaddleXP = 'l'; char KEYpaddleXN = 'j';
char KEYpaddleZP = 'i'; char KEYpaddleZN = 'k';
float padSpeed = 500;
float padX = 200; float padY = 250; float padZ = 200; float padR = 200;

char KEYregen = 'b';// "rebuild"

/***** SCENES / MODES *****/
//TODO: Extract from emitter

/***** REAL-TIME INTERACTION *****/
int emitterMode = 7;

/***** SIMULATION *****/
int   maxcount   = 200000;       //maximum number of particles

/***** STATIC COLLIDERS ****/
float floor = 1200;

/***** EMITTERS *****/
Emitter[] initEmitters = {
  //new Emitter(4, new Vec3(50, 50, -50))
};
float spawnDist = 500;

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
  
/**** TEXTURES *****/
//PImage smoke;
PImage img;