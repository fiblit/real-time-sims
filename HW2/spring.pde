class Spring {
  float ks;//stiffness
  float kd;//damping
  float l0;//rest length
  
  public S_Particle a, b;
  
  boolean isVert;
  
  boolean torn;
  float threshold;
  int[] adj;
  
  public Spring(S_Particle a, S_Particle b, float ks, float kd, float l0) {
    this.ks = ks;
    this.kd = kd;
    this.l0 = l0;
    this.a = a;
    this.b = b;
    threshold = 3*l0;
  }
  
  public void ApplyForce(float dt) {
    if (a.isKinematic || b.isKinematic) {
      Vec3 e_ = b.pos.sub(a.pos);
      float l = e_.mag();
      Vec3 e = e_.div(l);
      float v1 = e.dot(a.vel);
      float v2 = e.dot(b.vel);
      
      float f = -ks*(l0 - l) - kd*(v1 - v2);
      //println(f," ",ks,l0,l,kd,v1,v2," ",dt," ",a.mass);
      //-10^6*(-.0417) -10^6*(0.09)
      //
      torn = torn || (l > threshold);//(f*dt/a.mass > threshold || f*dt/b.mass > threshold);
      if (torn) {
        Emitter emit;
        /* fiery */
        if (clothTearType == 0) {
          emit = new Emitter();
          Vec3 pos = a.pos.add(b.pos).div(2);
          emit.setAll(
            true,
            new range(1, 2), new range(1, 1),
            new rangeVec4(new Vec4(random(255,255), random(0,255), random(0,255), 255),
                          new Vec4(random(255,255), random(0,255), random(0,255), 0)),
            new range(0, 2*PI), new range(0, 5),
            new rangeVec3(new Vec3(pos.x+0,pos.y-5,pos.z+0),
                          new Vec3(pos.x+0,pos.y+5,pos.z+0)),
            new rangeVec3(new Vec3(-75,-100,-170),
                          new Vec3(75,-300,-200)),
            new rangeVec3(new Vec3(-200,-200,-200),
                          new Vec3(200,200,200)),
            250, -1,
            false);
          emitters.add(emit);
        }
        /* sparkles */
        else if (clothTearType == 1) {
          emit = new Emitter();
          Vec3 pos = a.pos.add(b.pos).div(2);
          emit.setAll(
            false,
            new range(0.3, 1), new range(1, 1),
            new rangeVec4(new Vec4(random(255,255), random(0,255), random(0,255), 255),
                          new Vec4(random(255,255), random(0,255), random(0,255), 0)),
            new range(0, 2*PI), new range(0, l),
            new rangeVec3(new Vec3(pos.x+0,pos.y-5,pos.z+0),
                          new Vec3(pos.x+0,pos.y+5,pos.z+0)),
            new rangeVec3(new Vec3(-50,-50,-50),
                          new Vec3(50,50,50)),
            new rangeVec3(new Vec3(-50,-50,-50),
                          new Vec3(50,50,50)),
            500, 0.2,
            false);
          emitters.add(emit);
        }
      }
      if (a.isKinematic)
        a.acc = a.acc.add(e.mul(f*dt/a.mass));
      if (b.isKinematic)
        b.acc = b.acc.add(e.mul(-f*dt/b.mass));
    }
  }
}

//a bit hacky :P
class S_Particle {  
  Vec3 pos, vel, acc; 
  float mass;
  
  boolean isKinematic;//do physics affect me?
  
  public S_Particle() {}
  public S_Particle(Vec3 pos, Vec3 vel, Vec3 acc, float mass, boolean isKinematic) {
    this.pos = pos;
    this.vel = vel;
    this.acc =acc;
    this.mass = mass;
    this.isKinematic = isKinematic;
  }
}