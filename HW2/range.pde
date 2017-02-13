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
  Vec3 l, h;
  public rangeVec3(Vec3 low, Vec3 high) {
    l = low; h = high;
  }
  public Vec3 sample() {
    return new Vec3(random(l.x,h.x), random(l.y,h.y), random(l.z,h.z));
  }
}
class rangeVec4 {
  Vec4 l, h;
  public rangeVec4(Vec4 low, Vec4 high) {
    l = low; h = high;
  }
  public Vec4 sample() {
    return new Vec4(random(l.x,h.x), random(l.y,h.y), random(l.z,h.z), random(l.w,h.w));
  }
}