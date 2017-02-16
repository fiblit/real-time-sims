class Emitter {
  boolean diskSampleXZ;
  range angR, radR, pLifeR, massR;
  rangeVec3 posR, velR, accR;
  rangeVec4 clrR;
  float genRate;
  float eLife;
  float eMaxLife;
  boolean eBall;
  
  //there HAS to be a better way to do this... :/ Looks ugly AF.
  public void setAll(
      //disk sample XZ (true) / box sample XYZ (false)
       //uses low value range on pos.x/z and ang/rad to create the disk
      boolean diskSampleXZ,
      range pLifeR, range massR, rangeVec4 clrR,
      range angR, range radR, 
      rangeVec3 posR, rangeVec3 velR, rangeVec3 accR,
      float genRate,
      float eMaxLife,
      boolean eBall) {
    this.diskSampleXZ = diskSampleXZ;
    this.pLifeR = pLifeR; this.massR = massR; this.clrR = clrR;
    this.angR = angR; this.radR = radR;
    this.posR = posR; this.velR = velR; this.accR = accR;
    this.genRate = genRate;
    this.eLife = 0;
    this.eMaxLife = eMaxLife;
    this.eBall = eBall;
  }
  
  public Emitter() {
  }
  
  public Emitter(int mode, Vec3 pos) {
    //println(pos.x,pos.y,pos.z);    
    pos.y = min(pos.y, floor-15);
    //println(pos.x,pos.y,pos.z);
    switch (mode) {
    case 0:
      println("ball bouncing at pos.");
      //change PMesh?
      setAll(
        true,
        new range(60, 60), new range(1, 1),
        new rangeVec4(new Vec4(200,0,255,255),
                      new Vec4(200,0,255,255)),
        new range(0, 2*PI), new range(0, 1),
        new rangeVec3(new Vec3(pos.x+0,pos.y+0,pos.z+0),
                      new Vec3(pos.x+0,pos.y+0,pos.z+0)),
        new rangeVec3(new Vec3(20,-300,0),
                      new Vec3(20,-300,0)),
        new rangeVec3(new Vec3(0,200,0),
                      new Vec3(0,200,0)),
        0.1, -1,
        true);
      break;
    case 1:
      println("water fountain.");
      setAll(
        true,
        new range(3, 5), new range(1, 1),
        new rangeVec4(new Vec4(0, 0, 100, 255),
                      new Vec4(50, 175, 255, 50)),
        new range(0, 2*PI), new range(0, 15),
        new rangeVec3(new Vec3(pos.x+0,pos.y-10,pos.z+0),
                      new Vec3(pos.x+0,pos.y+10,pos.z+0)),
        new rangeVec3(new Vec3(-75,-100,170),
                      new Vec3(75,-300,200)),
        new rangeVec3(new Vec3(0,98,0),
                      new Vec3(0,98,0)),
        1000, -1,
        false);
      break;
    case 2:
      println("fire!!!");
      setAll(
        true,
        new range(1, 6), new range(1, 1),
        new rangeVec4(new Vec4(255, 63, 0, 255),
                      new Vec4(255, 233, 0, 190)),
        new range(0, 2*PI), new range(0, 200),
        new rangeVec3(new Vec3(pos.x+0,pos.y-5,pos.z+0),
                      new Vec3(pos.x+0,pos.y+5,pos.z+0)),
        new rangeVec3(new Vec3(-25,-30,-25),
                      new Vec3(25,-300,25)),
        new rangeVec3(new Vec3(0,-10,0),
                      new Vec3(0,-10,0)),
        1000, -1,
        false);
      break;
    case 3:
      println("spawn balls magic and/or games.");
      setAll(
        true,
        new range(1, 2), new range(1, 1),
        new rangeVec4(new Vec4(random(0,255), random(0,255), random(0,255), 255),
                      new Vec4(random(0,255), random(0,255), random(0,255), 0)),
        new range(0, 2*PI), new range(0, 5),
        new rangeVec3(new Vec3(pos.x+0,pos.y-5,pos.z+0),
                      new Vec3(pos.x+0,pos.y+5,pos.z+0)),
        new rangeVec3(new Vec3(-75,-100,-170),
                      new Vec3(75,-300,-200)),
        new rangeVec3(new Vec3(-200,-200,-200),
                      new Vec3(200,200,200)),
        250, -1,
        false);
      break;
    case 4:
      println("Fireball & 100K/30FPS bench attempt");
      setAll(
        true,
        new range(9, 11), new range(1, 1),
        new rangeVec4(new Vec4(255, 195, 0, 255),
                      new Vec4(199, 0, 57, 190)),
        new range(0, 2*PI), new range(0, 50),
        new rangeVec3(new Vec3(pos.x+0,pos.y-50,pos.z+0),
                      new Vec3(pos.x+0,pos.y+50,pos.z+0)),
        new rangeVec3(new Vec3(-25,-10,-25),
                      new Vec3(25,-10,25)),
        new rangeVec3(new Vec3(20,-40,20),
                      new Vec3(20,-40,20)),
        10000, -1,
        false);
      break;
    case 5:
      println("Fireball & 90K/30+FPS bench");
      setAll(
        true,
        new range(9, 11), new range(1, 1),
        new rangeVec4(new Vec4(255, 195, 0, 255),
                      new Vec4(199, 0, 57, 190)),
        new range(0, 2*PI), new range(0, 50),
        new rangeVec3(new Vec3(pos.x+0,pos.y-50,pos.z+0),
                      new Vec3(pos.x+0,pos.y+50,pos.z+0)),
        new rangeVec3(new Vec3(-25,-10,-25),
                      new Vec3(25,-10,25)),
        new rangeVec3(new Vec3(20,-40,20),
                      new Vec3(20,-40,20)),
        9000, -1,
        false);
      break;
    case 6:
      break;
    }
  }
  
  public Emitter (
      //disk sample XZ (true) / box sample XYZ (false)
      //uses low value range on pos.x/z and ang/rad to create the disk
      boolean diskSampleXZ,
      range angR, range radR, range pLifeR, range massR,
      rangeVec4 clrR,
      rangeVec3 posR, rangeVec3 velR, rangeVec3 accR,
      float genRate,
      float eMaxLife,
      boolean eBall) {
    setAll(diskSampleXZ, pLifeR, massR, clrR, angR, radR, posR, velR, accR, genRate, eMaxLife, eBall);
  }
  
  public void genRandParticle(int i) {//at global index i
    float a = angR.sample();
    float r = radR.l + sqrt(random(0,1))*(radR.h-radR.l);
    if (diskSampleXZ)
      ps[i].pos = new Vec3(
        posR.l.x + r*cos(a),
        random(posR.l.y,posR.h.y),
        posR.l.z + r*sin(a));
    ps[i].vel = velR.sample();
    ps[i].acc = accR.sample();
    ps[i].life = 0;
    ps[i].maxlife = pLifeR.sample();
    ps[i].clr = clrR.l;//.sample();
    ps[i].clrRange = clrR;
    ps[i].mass = massR.sample();
    ps[i].isBall = eBall;
    ps[i].eParent = this;
  }
}


void buildMode() {
  Vec3 pos = new Vec3(0,0,0);
  int howmany = 1;
  eye = new Vec3(-1300, 1100, 1300);
  yaw = 315; pitch = -25;
  padX = 50; padY = 50; padZ = -50; padR = 300;
  
  switch (emitterMode) {
    case 0://ball bounce
      pos = new Vec3(-500,500,500);
      break;
    case 1://water fountain
      pos = new Vec3(0,800,0);
      padX = 50; padY = 1000; padZ = 300; padR = 100;
      howmany = 5;
      break;
    case 2://fire
      howmany = 5;
      pos = new Vec3(0,floor-15,500);
      break;
    case 3://magic
      pos = new Vec3(500,500,500);
      break;
    default:
    case 4://meteor 100k
      pos = new Vec3(50, 50, -50);
      break;
    case 5://meteor 90k
      pos = new Vec3(50, 50, -50);
      break;
    case 6://snow
      howmany = 0;
      break;
    case 7://canvas
      howmany = 0;
      break;
  }
  for (int i = 0; i < howmany; i ++)
    emitters.add(new Emitter(emitterMode, pos));
  cameraCalc();
}