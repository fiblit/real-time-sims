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