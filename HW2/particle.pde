//basically a struct
class Particle {
  Emitter eParent;
  vector3 pos, vel, acc; 
  float life, maxlife;
  float mass;
  vector4 clr;
  rangeVec4 clrRange;
  
  //should be enum renderMode;
  boolean isBall;
  
  public Particle() {
  }
}