class vector3 {
  float x, y, z;
  public vector3() {
    x = y = z = 0;
  }
  public vector3(float x, float y, float z) { 
    this.x = x; 
    this.y = y; 
    this.z = z;
  }
  public float dot(vector3 b) {
    return this.x * b.x + this.y * b.y + this.z * b.z;
  }
  public vector3 cross(vector3 b) {
    return new vector3(
      this.y * b.z - this.z * b.y,
      this.z * b.x - this.x * b.z,
      this.x * b.y - this.y * b.x);
  }
}

class vector4 {
  float x, y, z, w;
  public vector4() {
    x = y = z = w = 0;
  }
  public vector4(float x, float y, float z, float w) { 
    this.x = x; 
    this.y = y; 
    this.z = z;
    this.w = w;
  }
  public float dot(vector4 b) {
    return this.x * b.x + this.y * b.y + this.z * b.z + this.w * b.w;
  }
}

vector4 lerp(vector4 a, vector4 b, float t) {
  return new vector4(a.x*(1-t) + (t)*b.x, a.y*(1-t) + (t)*b.y, a.z*(1-t) + (t)*b.z, a.w*(1-t) + b.w*(t));
}