class Cloth {
  int resW; 
  int resH; //size = resH by resW
  float ks, kd, l0;
  Vec3 O, H, V;
  S_Particle[] parts;
  int[][] triIDs;
  Spring[] springs;
  
  public Cloth(int w, int h, Vec3 O, Vec3 H, Vec3 V, float ks, float kd, float l0) {
    resW = w;
    resH = h;
    int edges = h*(w-1) + w*(h-1);
    int boxes = (w-1)*(h-1);
    
    parts = new S_Particle[w*h];
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < h; j++) {
        parts[i+ w*j] = new S_Particle(
          O.add(H.mul( i / ((float)w) )).add(V.mul( j / ((float)h) )),
          new Vec3(0,0,0),
          new Vec3(0,0,0),
          1,
          (j!=0)//hacked in frozen top row
        );       
      }
    }
    
    //midpoint tesselation
    //triIDs = new int[4*boxes][3];
    //nevermind that will be slower, let's do the artifacty right-diag!
    triIDs = new int[2*boxes][3];
    //simple square (& diag) connections
    springs = new Spring[edges];//+2*boxes];
    
    for (int i = 0; i < w - 1; i++) {
      for (int j = 0; j < h - 1; j++) {
        int box = i+(w-1)*j;
        int tl = i   + j*w;
        int tr = i+1 + j*w;
        int bl = i   + (j+1)*w;
        int br = i+1 + (j+1)*w;
        triIDs[box][0] = tl;
        triIDs[box][1] = bl;
        triIDs[box][2] = tr;
        triIDs[box+boxes][0] = tr;
        triIDs[box+boxes][1] = bl;
        triIDs[box+boxes][2] = br;
      }
    }
    //vert edges
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < h -1; j++) {
        int edge = i+w*j;
        springs[edge] = new Spring(parts[edge], parts[edge+w], ks, kd, l0);
      }
    }
    //horiz edges
    for (int j = 0; j < h; j++) {
      for (int i = 0; i < w - 1; i++) {
        int edge = i+(w-1)*j;
        springs[w*(h-1) + edge] = new Spring(parts[edge], parts[edge+1], ks, kd, l0);
      }
    }
  }
  
  public void render() {
    beginShape(TRIANGLES);
    for (int i = 0; i < triIDs.length; i++) {
      Vec3 v0 = parts[triIDs[i][0]].pos;
      Vec3 v1 = parts[triIDs[i][1]].pos;
      Vec3 v2 = parts[triIDs[i][2]].pos;
      println(v0.x);
      vertex(v0.x, v0.y, v0.z);
      vertex(v1.x, v1.y, v1.z);
      vertex(v2.x, v2.y, v2.z);
    }
    endShape();
    pushMatrix();
    translate(0,20,0);
    sphere(10);
    popMatrix();
  }
  
  public void computePhysics(float dt) {
    this.ApplyForces(dt);
     
    for (int i = 0; i < parts.length; i++) {
      //eulerian integration
      parts[i].pos = parts[i].pos.add(parts[i].vel.mul(dt));//.add(parts[i].acc.mul(0.5*dt*dt);
      parts[i].vel = parts[i].vel.add(parts[i].acc.mul(dt));
    }
  }
  
  public void ApplyForces(float dt) {    
    for(int i = 0; i < parts.length; i++) {
      println(i);
      parts[i].acc = new Vec3(0,98,0);
    }
    for(int i = 0; i < springs.length; i++) {
      springs[i].ApplyForce(dt);
    }
  }
}