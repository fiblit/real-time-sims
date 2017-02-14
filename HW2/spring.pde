class Spring {
  float ks;//stiffness
  float kd;//damping
  float l0;//rest length
  
  S_Particle a, b;
  
  public Spring(S_Particle a, S_Particle b, float ks, float kd, float l0) {
    this.ks = ks;
    this.kd = kd;
    this.l0 = l0;
    this.a = a;
    this.b = b;
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