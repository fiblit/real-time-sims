class range {
  float l, h;
  public range(float low, float high) {
    l = low; h = high;
  }
  public float sample() {
    return random(l,h);
  }
}
class rangeVec3 {
  vector3 l, h;
  public rangeVec3(vector3 low, vector3 high) {
    l = low; h = high;
  }
  public vector3 sample() {
    return new vector3(random(l.x,h.x), random(l.y,h.y), random(l.z,h.z));
  }
}
class rangeVec4 {
  vector4 l, h;
  public rangeVec4(vector4 low, vector4 high) {
    l = low; h = high;
  }
  public vector4 sample() {
    return new vector4(random(l.x,h.x), random(l.y,h.y), random(l.z,h.z), random(l.w,h.w));
  }
}

class emitter {
  boolean diskSampleXZ;
  range angR, radR, pLifeR, massR;
  rangeVec3 posR, velR, accR;
  rangeVec4 clrR;
  float genRate;
  float eLife;
  
  //there HAS to be a better way to do this... :/ Looks ugly AF.
  public void setAll(
      //disk sample XZ (true) / box sample XYZ (false)
       //uses low value range on pos.x/z and ang/rad to create the disk
      boolean diskSampleXZ,
      range pLifeR, range massR, rangeVec4 clrR,
      range angR, range radR, 
      rangeVec3 posR, rangeVec3 velR, rangeVec3 accR,
      float genRate,
      float eLife) {
    this.diskSampleXZ = diskSampleXZ;
    this.pLifeR = pLifeR; this.massR = massR; this.clrR = clrR;
    this.angR = angR; this.radR = radR;
    this.posR = posR; this.velR = velR; this.accR = accR;
    this.genRate = genRate;
    this.eLife = eLife;
  }
  
  public emitter(int mode, vector3 pos) {
    switch (mode) {
    case 0:
      println("spawn balls for ball bouncing at pos.");      
      break;
    case 1:
      println("spawn balls for water fountain.");
      break;
    case 2:
      println("fire!!!");
      setAll(
        true,
        new range(1, 3), new range(1, 1),
        new rangeVec4(new vector4(255,195,0,255),
                      new vector4(255,195,0,255)),
        new range(0, 2*PI), new range(0, width/2),
        new rangeVec3(new vector3(pos.x+0,pos.y-50,pos.z+0),
                      new vector3(pos.x+0,pos.y+50,pos.z+0)),
        new rangeVec3(new vector3(-15,-10,-15),
                      new vector3(15,-10,15)),
        new rangeVec3(new vector3(80,-160,80),
                      new vector3(80,-160,80)),
        9000, -1);
      break;
    case 3:
      println("spawn balls magic and/or games.");
      break;
    case 4:
      println("benchmark @ 100 K");
      break;
    case 6:
      println("spawn tree of life");
      break;
    case 7:
      println("spawn snow");
      break;
    case 8:
      println("spawn smoke");
      break;
    }
  }
  
  public emitter(
      //disk sample XZ (true) / box sample XYZ (false)
      //uses low value range on pos.x/z and ang/rad to create the disk
      boolean diskSampleXZ,
      range angR, range radR, range pLifeR, range massR,
      rangeVec4 clrR,
      rangeVec3 posR, rangeVec3 velR, rangeVec3 accR,
      float genRate,
      float eLife) {
    setAll(diskSampleXZ, pLifeR, massR, clrR, angR, radR, posR, velR, accR, genRate, eLife);
  }
  
  public void genRandParticle(int i) {//at global index i
    float a = angR.sample();
    float r = radR.l + sqrt(random(0,1))*(radR.h-radR.l);
    if (diskSampleXZ)
      pos[i] = new vector3(
        posR.l.x + r*cos(a),
        random(posR.l.y,posR.h.y),
        posR.l.z + r*sin(a));
    vel[i] = velR.sample();
    acc[i] = accR.sample();
    maxlife[i] = pLifeR.sample(); life[i] = 0;
    clr[i] = clrR.sample();
    mass[i] = massR.sample();
  }
}

void gen() {
  pos = new vector3[maxcount];//I might want to be smarter about storage?
  vel = new vector3[maxcount];
  life = new float[maxcount];
  maxlife = new float[maxcount];
  clr = new vector4[maxcount];
  acc = new vector3[maxcount];
  mass = new float[maxcount];
  emitters = new ArrayList<emitter>();
  pcount = 0;
}

void regen() {
  println("start gen");
  gen();
  println("finish gen");
  buildMode();
  println("finish build");
}

void buildMode() {
  vector3 pos;
  switch (emitterMode) {
    case 0://ball bounce
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    case 1://water fountain
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    case 3://magic
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    case 4://bench
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    case 5://tree
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    case 6://snow
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    case 7://smoke
      pos = new vector3(0,0,0);
      eye = new vector3(0,0,100);
      yaw = 270; pitch = 0;
      break;
    default:
    case 2://fire
      pos = new vector3(0, 0, 0);
      //eye = new vector3(-15*width, 1100, 15*width);
      yaw = 315; pitch = -25;
      break;
  }
  emitters.add(new emitter(2, new vector3(width/2, 50, -width/2)));
  cameraCalc();
}

ArrayList<emitter> emitters;

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
  maxlife[i] = random(2, 6); life[i] = 0;
  clr[i] = new vector4(255, 195, 0, 255);
}

void p_gen(float dt) {
  for (int e = 0; e < emitters.size(); e++) {
    if (emitters.get(e).genRate * dt > 1) {
      for (int i = 0; i < emitters.get(e).genRate * dt; i++) {
        if (pcount < maxcount) {
          emitters.get(e).genRandParticle(pcount);
          pcount++;
        }
      }
    } else if (random(1) <= emitters.get(e).genRate * dt) {
      if (pcount < maxcount) {
        emitters.get(e).genRandParticle(pcount);
        pcount++;
      }
    }
  }
}

void moveBtoA(int a, int b) {
  float t = life[b];
  life[b] = life[a];
  life[a] = t;
  
  t = maxlife[b];

  vector3 tv = vel[b];
  vel[b] = vel[a];
  vel[a] = tv;

  tv = pos[b];
  pos[b] = pos[a];
  pos[a] = tv;
  
  vector4 tc = clr[b];
  clr[b] = clr[a];
  clr[a] = tc;
}

boolean p_kill(float dt, int i) {
  if (life[i] >= maxlife[i]) {
    moveBtoA(i, pcount - 1);
    pcount -= 1;
    return true;
  } else {
    life[i] += dt;
    return false;
  }
}