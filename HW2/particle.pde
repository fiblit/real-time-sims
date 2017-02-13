//basically a struct
class Particle {
  Emitter eParent;
  float life, maxlife;
  Vec3 pos, vel, acc; 
  float mass;
  Vec4 clr;
  rangeVec4 clrRange;
  
  //should be enum renderMode;
  boolean isBall;
  
  public Particle() {
  }
}