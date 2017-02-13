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