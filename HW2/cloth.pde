class Cloth {
  int resW; 
  int resH; //size = resH by resW
  float ks, kd, l0;
  Vec3 O, H, V;
  S_Particle[] parts;
  int[][] triIDs;
  float[][] tex;
  Spring[] springs;
  int numSprings;
  
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
          0.04,
          (j!=0)//hacked in frozen top row
        );       
      }
    }
    
    //midpoint tesselation
    triIDs = new int[2*boxes][3];
    tex = new float[w*h][2];
    //simple square (& diag) connections
    numSprings = edges;
    springs = new Spring[edges];//+2*boxes];
    
    for (int i = 0; i < w - 1; i++) {
      for (int j = 0; j < h - 1; j++) {
        int box = i+(w-1)*j;
        int tl = i   + j*w;
        int tr = i+1 + j*w;
        int bl = i   + (j+1)*w;
        int br = i+1 + (j+1)*w;
        tex[tl][0] = i/((float)w);
        tex[tl][1] = j/((float)h);
        tex[tr][0] = (i+1)/((float)w);
        tex[tr][1] = j/((float)h);
        tex[bl][0] = i/((float)w);
        tex[bl][1] = (j+1)/((float)h);
        tex[br][0] = (i+1)/((float)w);
        tex[br][1] = (j+1)/((float)h);
        if (j%2 == 0) {
          triIDs[box][0] = tl;
          triIDs[box][1] = bl;
          triIDs[box][2] = tr;
          triIDs[box+boxes][0] = tr;
          triIDs[box+boxes][1] = bl;
          triIDs[box+boxes][2] = br;
        }
        else {
          triIDs[box][0] = tl;
          triIDs[box][1] = br;
          triIDs[box][2] = tr;
          triIDs[box+boxes][0] = tl;
          triIDs[box+boxes][1] = bl;
          triIDs[box+boxes][2] = br;
        }
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
        int edge = w*(h-1) + i+j*(w-1);
        springs[edge] = new Spring(parts[i+j*w], parts[i+1+j*w], ks, kd, l0);
      }
    }
    /*
    //diags
    for (int j = 0; j < h - 1; j++) {
      for (int i = 0; i < w - 1; i++) {
        int b = i + j *(w-1);
        springs[edges + b] = new Spring(parts[i+j*w], parts[i+1+(j+1)*w], ks, kd, l0/sqrt(2));
        springs[edges + boxes+ b] = new Spring(parts[i+(j+1)*w], parts[i+1+j*w], ks, kd, l0/sqrt(2));
      }
    }
    */
  }
  
  public void render() {
    beginShape(TRIANGLES);
    texture(img);
    for (int i = 0; i < triIDs.length; i++) {
      Vec3 v0 = parts[triIDs[i][0]].pos;
      float u = tex[triIDs[i][0]][0];
      float v = tex[triIDs[i][0]][1];
      vertex(v0.x, v0.y, v0.z, u, v);
      Vec3 v1 = parts[triIDs[i][1]].pos;
      u = tex[triIDs[i][1]][0];
      v = tex[triIDs[i][1]][1];
      vertex(v1.x, v1.y, v1.z, u, v);
      Vec3 v2 = parts[triIDs[i][2]].pos;
      u = tex[triIDs[i][2]][0];
      v = tex[triIDs[i][2]][1];
      vertex(v2.x, v2.y, v2.z, u, v);
    }
    endShape();
    /*
    stroke(0,0,255);
    for (int i = 0; i < triIDs.length; i++) {
      Vec3 v0 = parts[triIDs[i][0]].pos;
      Vec3 v1 = parts[triIDs[i][1]].pos;
      Vec3 v2 = parts[triIDs[i][2]].pos;
      line(v0.x+6,v0.y-3,v0.z+6,v1.x+6,v1.y-3,v1.z+6);
      line(v0.x+6,v0.y-3,v0.z+6,v2.x+6,v2.y-3,v2.z+6);
      line(v1.x+6,v1.y-3,v1.z+6,v2.x+6,v2.y-3,v2.z+6);
    }
    */
    
    /*
    fill(0,255,0);
    sphereDetail(3);
    for (int i = 0; i < parts.length; i++) {
      pushMatrix();
      translate(parts[i].pos.x,parts[i].pos.y,parts[i].pos.z);
      sphere(3);
      popMatrix();
    }
    sphereDetail(50);
    */
    /*
    stroke(0,255,0);
    for (int i = 0; i < springs.length; i++) {
      line(
        springs[i].a.pos.x + 5, springs[i].a.pos.y - 3, springs[i].a.pos.z + 5, 
        springs[i].b.pos.x + 5, springs[i].b.pos.y - 3, springs[i].b.pos.z + 5);
    }
    noStroke();
    */
    
  }
  
  public void computePhysics(float dt) {
    this.ApplyForces(dt);
     
    for (int i = 0; i < parts.length; i++) {
      if (parts[i].isKinematic) {
        //midpoint integration
        //works by magic
        parts[i].pos = parts[i].pos.add(parts[i].vel.mul(dt)).add(parts[i].acc.mul(0.5*dt*dt));
        parts[i].vel = parts[i].vel.add(parts[i].acc.mul(dt));
      }
    }
  }
  
  public void checkCollisions() {
    float buffer = 10;//helps avoid face-clipping
    for (int i = 0; i < parts.length; i++) {
      /*
      for (int j = 0; j < parts.length; j++) {
        if (i == j)
          continue;
        Vec3 toS = parts[i].pos.sub(parts[j].pos);
        float mag2 = toS.mag2();
        if (mag2 < 4*buffer*buffer) {
          float mag = sqrt(mag2);
          Vec3 toSNorm = toS.div(mag);
          float alpha = 0.1;
          float co = (1+alpha)*parts[i].vel.dot(toSNorm);
          parts[i].vel = (parts[i].vel.sub(toSNorm.mul(co)));
          parts[i].pos = parts[j].pos.add(toSNorm.mul(2*buffer));
        }
      }
      */     
      
      //paddle
      Vec3 toS = new Vec3(parts[i].pos.x - padX, parts[i].pos.y - padY, parts[i].pos.z - padZ);
      float mag2 = toS.mag2();
      if (mag2 < (padR+buffer)*(padR+buffer)) {
        float mag  = sqrt(mag2);
        Vec3 toSNorm = toS.div(mag);
        float alpha = 0.1;// (0,1] 
        float co = (1+alpha)*parts[i].vel.dot(toSNorm);
        parts[i].vel = (parts[i].vel.sub(toSNorm.mul(co)));
        Vec3 t = toSNorm.mul(padR+buffer);
        parts[i].pos = new Vec3(
          (padX + t.x),
          (padY + t.y),
          (padZ + t.z));
      }
      
      //floor
      if (parts[i].pos.y + buffer >= floor) {
        parts[i].pos.y = floor - buffer - .1*(parts[i].pos.y -floor);
        parts[i].vel.y *= -.1;
      }
    }
  }

  public void ApplyForces(float dt) {    
    for(int i = 0; i < parts.length; i++) {
      parts[i].acc = new Vec3(0,980,0);
    }
    for(int i = 0; i < springs.length; i++) {
      springs[i].ApplyForce(dt);
    }
    
    for(int i = 0; i < triIDs.length; i++) {
      S_Particle s0 = parts[triIDs[i][0]];
      S_Particle s1 = parts[triIDs[i][1]];
      S_Particle s2 = parts[triIDs[i][2]];
     
      //f_aero = -1/2 (density) |v^2| c_d * a * norm
      //1/2 ||(s1 - s0)x( s2-s0 )||
      float a_0 = 0.5 *(s1.pos.sub(s0.pos).cross(s2.pos.sub(s0.pos))).mag();
      //(s1-s0)x(s2-s0))/||(s1 - s0)x( s2-s0 )||
      Vec3 norm = s1.pos.sub(s0.pos).cross(s2.pos.sub(s0.pos)).div(2*a_0);
      //( s0 + s1 + s2 ) / 3
      Vec3 v = s0.vel.add(s1.vel).add(s2.vel).div(3); 
      float vmag = v.mag();
      float a = a_0 * v.dot(norm);
      float density = 0.001;
      float c_d = 0.001;
      Vec3 f_aero = norm.mul(-0.5 * density * vmag*vmag * c_d * a/ 3);

      s0.acc = s0.acc.add(f_aero.mul(dt/s0.mass));
      s1.acc = s1.acc.add(f_aero.mul(dt/s1.mass));
      s2.acc = s2.acc.add(f_aero.mul(dt/s2.mass));
    }
  }
}