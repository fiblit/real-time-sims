class emitter {
  float[] angle;
  float[] radius;
  float px, pz;
  float[] py;
  float[] vx, vy, vz;
  float[] l; //add maxlife?
  float[] r, g, b, a;
  
  float genRate;
  float time;
  
  public emitter(
      float angle, float angle2,   float radius, float radius2,
      float px,   float pz,   float py, float py2,
      float vx, float vx2,   float vy, float vy2,   float vz, float vz2,
      float l, float l2,
      float r, float r2,   float g, float g2,   float b, float b2,   float a, float a2,
      float genRate,
      float time) {
    this.angle = new float[2]; this.radius = new float[2];
    this.angle[0] = angle; this.angle[1] = angle2; this.radius[0] = radius; this.radius[1] = radius2;
    this.px = px;
    this.pz = pz;
    this.py = new float[2];
    this.py[0] = py; this.py[1] = py2;
    this.vx = new float[2]; this.vy = new float[2]; this.vz = new float[2];
    this.vx[0] = vx; this.vx[1] = vx2; this.vy[0] = vy; this.vy[1] = vy2; this.vz[0] = vz; this.vz[1] = vz2;
    this.l = new float[2];
    this.l[0] = l; this.l[1] = l2;
    this.r = new float[2]; this.g = new float[2]; this.b = new float[2]; this.a = new float[2];
    this.r[0] = r; this.r[1] = r2; this.g[0] = g; this.g[1] = g2; this.b[0] = b; this.a[1] = a;
    
    this.genRate = genRate;
    this.time = time;
  }
  
  public void genRandParticle(int i) {//at index i
    float ang = random(angle[0], angle[1]);
    float rad = radius[0] + sqrt(random(0,1))*(radius[1]-radius[0]);
    pos[i] = new vector3(
      px + rad*cos(ang),//I think I forgot to correct the bias
      random(py[0],py[1]),
      pz + rad*sin(ang));
    vel[i] = new vector3(
      random(vx[0],vx[1]),
      random(vy[0],vy[1]),
      random(vz[0],vz[1]));
    life[i] = random(l[0],l[1]);
    clr[i] = new vector4(
      random(r[0],r[1]),
      random(g[0],g[1]),
      random(b[0],b[1]),
      random(a[0],a[1]));
  }
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
  float r = random(0, width/8);
  float x = width/2+r*cos(a);//random(radius,width-radius); 
  float y = random(-300, -500);
  float z = -(width/2+r*sin(a)); //random(-1000, -100);
  float vx = random(100, 100);
  float vy = random(100, 100);
  float vz = random(100, 100);
  pos[i] = new vector3(x, y, z);
  vel[i] = new vector3(vx, vy, vz);
  life[i] = random(2, 6);
  clr[i] = new vector4(255, 195, 0, 255);
}

void p_gen(float dt) {
  for (int e = 0; e < emitters.length; e++) {
    if (emitters[e].genRate * dt > 1) {
      for (int i = 0; i < emitters[e].genRate * dt; i++) {
        if (pcount < maxcount) {
          emitters[e].genRandParticle(pcount);
          pcount++;
        }
      }
    } else if (random(1) <= emitters[e].genRate * dt) {
      if (pcount < maxcount) {
        emitters[e].genRandParticle(pcount);
        pcount++;
      }
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