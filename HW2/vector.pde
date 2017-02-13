class Vec3 {
  float x, y, z;
  public Vec3() {
    x = y = z = 0;
  }
  public Vec3(float x, float y, float z) { 
    this.x = x; 
    this.y = y; 
    this.z = z;
  }
  public float mag2() {
    return this.dot(this);
  }
  public float mag() {
    return sqrt(mag2());
  }
  public Vec3 norm() {
    return this.div(this.mag());
  }
  public Vec3 div(float s) {
    return new Vec3(this.x/s, this.y/s, this.z/s);
  }
  public Vec3 mul(float s) {
    return new Vec3(this.x*s, this.y*s, this.z*s);
  }
  public Vec3 add(Vec3 b) {
    return new Vec3(this.x + b.x, this.y + b.y, this.z + b.z);
  }
  public Vec3 sub(Vec3 b) {
    return new Vec3(this.x - b.x, this.y - b.y, this.z - b.z);
  }
  public float dot(Vec3 b) {
    return this.x * b.x + this.y * b.y + this.z * b.z;
  }
  public Vec3 cross(Vec3 b) {
    return new Vec3(
      this.y * b.z - this.z * b.y,
      this.z * b.x - this.x * b.z,
      this.x * b.y - this.y * b.x);
  }
}


//really only used it for color...
class Vec4 {
  float x, y, z, w;
  public Vec4() {
    x = y = z = w = 0;
  }
  public Vec4(float x, float y, float z, float w) { 
    this.x = x; 
    this.y = y; 
    this.z = z;
    this.w = w;
  }
  public float dot(Vec4 b) {
    return this.x * b.x + this.y * b.y + this.z * b.z + this.w * b.w;
  }
}

Vec4 lerp(Vec4 a, Vec4 b, float t) {
  return new Vec4(a.x*(1-t) + (t)*b.x, a.y*(1-t) + (t)*b.y, a.z*(1-t) + (t)*b.z, a.w*(1-t) + b.w*(t));
}