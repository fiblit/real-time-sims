class Spring {
  float ks;//stiffness
  float kd;//damping
  float l0;//rest length
  
  public S_Particle a, b;
  
  boolean isVert;
  
  boolean torn;
  float threshold;
  
  public Spring(S_Particle a, S_Particle b, float ks, float kd, float l0) {
    this.ks = ks;
    this.kd = kd;
    this.l0 = l0;
    this.a = a;
    this.b = b;
    threshold = 100000;
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
      torn = torn || (f*dt/a.mass > threshold || f*dt/b.mass > threshold);
      if (torn) {
        Emitter emit = new Emitter();
        emit.setAll(
        true,
        new range(0.2, 1), new range(1,1), new rangeVec4(new Vec4(255,60,0,255), new Vec4(200,120,0,127)),
        new range(0, 2*PI), new range(l,l), 
        new rangeVec3(new Vec3(a.pos.x,a.pos.y,a.pos.z),new Vec3(b.pos.x,b.pos.y,b.pos.z)), new rangeVec3(new Vec3(),new Vec3()), new rangeVec3(new Vec3(0,-98,0),new Vec3(0,-98,0)),
        100,
        0.3,
        false);
        emitters.add(emit);
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