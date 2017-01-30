class vector3 {
  float x, y, z;
  public vector3() {
    x = y = z = 0;
  }
  public vector3(float x, float y, float z) { 
    this.x = x; 
    this.y = y; 
    this.z = z;
  }
}

class vector4 {
  float x, y, z, w;
  public vector4() {
    x = y = z = w =0;
  }
  public vector4(float x, float y, float z, float w) { 
    this.x = x; 
    this.y = y; 
    this.z = z;
    this.w = w;
  }
}

//attributes
vector3[] pos, vel; 
float[] life;
vector4[] clr;

int rgpcount = 0, pcount;
int maxcount = 200000;
float radius = 10; 
float floor; 

PShape s;

void setup() {
  size(1000, 1000, P3D);
  floor = height;
  regen();
  noStroke();
  //strokeWeight(radius);
  //stroke(255,255,255);
  fill(200, 100, 0);
  sphereDetail(1);
  frameNow = millis();
}  

void regen() {
  //gonna be slow AF at gen.
  pcount = rgpcount;
  pos = new vector3[maxcount];//I might want to be smarter about storage?
  vel = new vector3[maxcount];
  life = new float[maxcount];
  clr = new vector4[maxcount];
  for (int i = 0; i < rgpcount; i ++ ) {
    genRandAttrs(i);
  }
}

void genRandAttrs(int i) {
  float a = random(0, 2*PI);
  float r = random(0, width/2);
  float x = width/2+r*cos(a);//random(radius,width-radius); 
  float y = random(floor-radius, floor-100);
  float z = -(width/2+r*sin(a)); //random(-1000, -100);
  float vx = random(0, 0);
  float vy = random(-25, 0);
  float vz = random(0, 0);
  pos[i] = new vector3(x, y, z);
  vel[i] = new vector3(vx, vy, vz);
  life[i] = random(2, 6);
  clr[i] = new vector4(255, 195, 0, 255);
}

float acceleration = -98;  
/* currently causing little to no K-loss */
void computePhysics(float dt, int i) { 
  float tx = 500;
  float tz = -500;
  float dx = (pos[i].x - tx);
  float dz = (pos[i].z - tz);
  float distToPull2 = dx*dx + dz *dz;
  vel[i].y = vel[i].y - (width*width/4 - distToPull2)/2000 * dt;
  vel[i].x = vel[i].x - dx/3 * dt;
  vel[i].z = vel[i].z - dz/3 * dt;
  pos[i].y = pos[i].y + vel[i].y * dt;
  pos[i].x = pos[i].x + vel[i].x * dt;
  pos[i].z = pos[i].z + vel[i].z * dt;
  if (pos[i].y + radius >= floor) {
    pos[i].y = floor - radius - .95*(pos[i].y -floor);
    vel[i].y *= -.95;
  }
}  

float z = 0;

float mdt() {
  return frameNow - lastFrame;
}
float sdt() {
  return mdt()/1000;
}

float lastFrame;
float frameNow;
float clock = 0;

void tick() {
  lastFrame = frameNow;
  frameNow = millis();
}

void draw() {
  background(255, 255, 255);
  tick();
  
  camera(eye.x, eye.y, eye.z, 500, height/2, -500, 0, 1, 0.1);

  float sdt = sdt();

  p_gen(sdt);

  //gives a reasonable performance boost. (Roughly 5 to 10K, never got an exact count)
  beginShape(TRIANGLES);
  for (int i = 0; i < pcount; i++) {
    computePhysics(sdt, i);
    simuAttrs(sdt, i);
    render(i);
    if (p_kill(sdt, i))
      i--; // since we swapped from the end. We need to process what was at the end.
  }
  endShape();
}

vector4 lerp(vector4 a, vector4 b, float t) {
  return new vector4(a.x*(1-t) + (t)*b.x, a.y*(1-t) + (t)*b.y, a.z*(1-t) + (t)*b.z, a.w*(1-t) + b.w*(t));
}

vector4 a = new vector4(255, 195, 0, 255);
vector4 b = new vector4(199, 0, 57, 190);
void simuAttrs(float dt, int i) {
  /* causing surprising amount of K-loss. Roughly 8K */
  /* the issues is not a color instantiation... */
  /* the issue is the color primitive. switching to "vector4" (mine), 
     mitigates the loss to roughly 3K. Still significant for what lerp is */
  clr[i] = lerp(a, b, (pos[i].y)/(0.25*height));
}


void render(int i) {
  //pushMatrix();
  //translate(pos[i].x, pos[i].y, pos[i].z);

  // fill marginally hurts FPS. Maybe a few K less @ 30FPS
  fill(clr[i].x, clr[i].y, clr[i].z, clr[i].w);

  //obviously the biggest FPS culprit
  

  
  float hry = radius ;//* pos[i].y / (0.75 * height);
  // can only handle roughly 57k particles
  //triangle(0, -hry*hry*0.4330127, radius/2, hry*hry*0.4330127, -radius/2, hry*hry*0.4330127);
  //float[][] t = new float[3][3];  
  float[][] v = new float[3][3];
  v[0][0] = 0         +pos[i].x; v[0][1] = -hry*hry*0.4330127 +pos[i].y; v[0][2] = pos[i].z;
  v[1][0] = -radius/2 +pos[i].x; v[1][1] = hry*hry*0.4330127  +pos[i].y; v[1][2] = pos[i].z;
  v[2][0] = radius/2  +pos[i].x; v[2][1] = hry*hry*0.4330127  +pos[i].y; v[2][2] = pos[i].z;
  
  //outofShape: ~22K (outdated)
  //insideShape: ~18K (outdated)
  //FOLLOW CAMERA : Does not work for new draw method...
  // I'll have to manually rotate the vertices just like how I'm manually transforming them.
  
  /* kinda but not quite
  vector3 eyeToTri = new vector3(pos[i].x - eye.x, pos[i].y - eye.y, pos[i].z - eye.z);
  float eyeToTriMag = sqrt(eyeToTri.x*eyeToTri.x + eyeToTri.z*eyeToTri.z);
  float th = (eye.x - pos[i].x > 0 ? 1 : -1) * acos((eye.z - pos[i].z)/eyeToTriMag);
  float ct = cos(th);
  float st = sin(th);
  
  
  v[0][0] = t[0][0] * ct + t[0][2] * st; v[0][1] = t[0][1]; v[0][2] = t[0][0] * -st + t[0][2] * ct;
  v[1][0] = t[1][0] * ct + t[1][2] * st; v[1][1] = t[1][1]; v[1][2] = t[1][0] * -st + t[1][2] * ct;
  v[2][0] = t[2][0] * ct + t[2][2] * st; v[2][1] = t[2][1]; v[2][2] = t[2][0] * -st + t[2][2] * ct;
  */

  //I don't trust it to unroll for me
  vertex(v[0][0],v[0][1], v[0][2]);
  vertex(v[1][0],v[1][1], v[1][2]);
  vertex(v[2][0],v[2][1], v[2][2]);
  //can only handle roughly 40k particles, that wasn't even with the textures applied yet.
  //beginShape();
  //texture(img);
  //triangle(0, -hry*hry*0.4330127, radius/2, hry*hry*0.4330127, -radius/2, hry*hry*0.4330127);
  //endShape();
  //popMatrix();
}

float genRate = 17500; //pps
void p_gen(float dt) {
  if (genRate * dt > 1) {
    for (int i = 0; i < genRate * dt; i++) {

      if (pcount < maxcount) {
        genRandAttrs(pcount);
        pcount++;
      }
    }
  } else if (random(1) <= genRate * dt) {
    if (pcount < maxcount) {
      genRandAttrs(pcount);
      pcount++;
    }
  }
}

void swapParticles(int a, int b) {
  float t = life[b];
  life[b] = life[a];
  life[a] = t;

  vector3 tv = vel[b];
  vel[b] = vel[a];
  vel[a] = tv;

  tv = pos[b];
  pos[b] = pos[a];
  pos[a] = tv;
}

boolean p_kill(float dt, int i) {
  if (life[i] <= 0) {
    swapParticles(i, pcount - 1);
    pcount -= 1;
    return true;
  } else {
    life[i] -= dt;
    return false;
  }
}

vector3 forward = new vector3(0, 0, -100);
vector3 eye = new vector3(500, 0, width);
void keyPressed() {
  if (key == 'w') {
    eye.z -= 100;
    //eye.x += forward.x;
    //eye.y += forward.y;
    //eye.z += forward.z;
  }
  if (key == 'a') {
    
    eye.x -= 100;
//forward.x += 10;
    //forward.z += 10;
  }
  if (key == 's') {
    
    
    eye.z += 100;
   // eye.x -= forward.x;
    //eye.y -= forward.y;
   // eye.z -= forward.z;
  }
  if (key == 'd') {
    
    
    eye.x += 100;
    //forward.x -= 10;
    //forward.z -= 10;
  }
  if (key == 'q') {
    genRate += 500;
  }
  if (key == 'e') {
    genRate -= 500;
  }
  if (key == 't' || key == 'g') {
    println("fps: " + frameRate);  
    println("rate: " + genRate);
    println("particles: " + pcount);
  }
  if (key == 'g') {
    rgpcount += 10000;
    regen();
  }
}