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
    triIDs = new int[2*boxes][5];
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
        
        //exact order matters for midpoint tesselation in the rendering step
        if (true) {
          triIDs[box][0] = bl;
          triIDs[box][1] = tr;
          triIDs[box][2] = tl;
          triIDs[box][3] = 0; //MAGIC ID VALUE
          triIDs[box][4] = 3; //MAGIC VALUE -> means both middlepoint splits are there
          triIDs[box+boxes][0] = tr;
          triIDs[box+boxes][1] = bl;
          triIDs[box+boxes][2] = br;
          triIDs[box+boxes][3] = 1; //MAGIC ID VALUE
          triIDs[box+boxes][4] = 3; //MAGIC VALUE -> means both middlepoint splits are there
        }
        else {
          triIDs[box][0] = tl;
          triIDs[box][1] = br;
          triIDs[box][2] = tr;
          triIDs[box][3] = 2; //MAGIC ID VALUE
          triIDs[box][4] = 3; //MAGIC VALUE -> means both middlepoint splits are there
          triIDs[box+boxes][0] = br;
          triIDs[box+boxes][1] = tl;
          triIDs[box+boxes][2] = bl;
          triIDs[box+boxes][3] = 3; //MAGIC ID VALUE
          triIDs[box+boxes][4] = 3; //MAGIC VALUE -> means both middlepoint splits are there
        }
      }
    }
    
    
    //vert edges
    for (int i = 0; i < w; i++) {
      for (int j = 0; j < h -1; j++) {
        int edge = i+w*j;
        springs[edge] = new Spring(parts[edge], parts[edge+w], ks, kd, l0);
        springs[edge].isVert = true;
        springs[edge].adj = spring2AdjTris(springs[edge]);
      }
    }
    //horiz edges
    for (int j = 0; j < h; j++) {
      for (int i = 0; i < w - 1; i++) {
        int edge = w*(h-1) + i+j*(w-1);
        springs[edge] = new Spring(parts[i+j*w], parts[i+1+j*w], ks, kd, l0);
        springs[edge].isVert = false;
        springs[edge].adj = spring2AdjTris(springs[edge]);
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
    //stroke(0,0,255);
    beginShape(TRIANGLES);
    texture(img);
    for (int i = 0; i < triIDs.length; i++) {
      Vec3[] p = new Vec3[4];
      float[] u = new float[4];
      float[] v = new float[4];
      p[0] = parts[triIDs[i][0]].pos;
      u[0] = tex[triIDs[i][0]][0];
      v[0] = tex[triIDs[i][0]][1];
      p[1] = parts[triIDs[i][1]].pos;
      u[1] = tex[triIDs[i][1]][0];
      v[1] = tex[triIDs[i][1]][1];
      p[2] = parts[triIDs[i][2]].pos;
      u[2] = tex[triIDs[i][2]][0];
      v[2] = tex[triIDs[i][2]][1];

      //midpoint between 0 & 1
      p[3] = p[1].add(p[0]).div(2.0);
      u[3] = (u[0] + u[1])/2.0;
      v[3] = (v[0] + v[1])/2.0;
      
      
      //println(triIDs[i][4]);
      if (triIDs[i][4] == 2 || triIDs[i][4] == 3) {//magic #'s 032: 2 / even
        vertex(p[0].x, p[0].y, p[0].z, u[0], v[0]);
        vertex(p[3].x, p[3].y, p[3].z, u[3], v[3]);
        vertex(p[2].x, p[2].y, p[2].z, u[2], v[2]);
      }

      if (triIDs[i][4] == 1 || triIDs[i][4] == 3) {//magic #'s 312: 1 / odd
        vertex(p[3].x, p[3].y, p[3].z, u[3], v[3]);
        vertex(p[1].x, p[1].y, p[1].z, u[1], v[1]);
        vertex(p[2].x, p[2].y, p[2].z, u[2], v[2]);
      }
    }
    endShape();
    //noStroke();
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

    for (int i = 0; i < numSprings; i++) {      
      stroke(64,64,64,64);
      if (springs[i].torn)
        stroke(255,0,0);
      line(
        springs[i].a.pos.x, springs[i].a.pos.y, springs[i].a.pos.z, 
        springs[i].b.pos.x, springs[i].b.pos.y, springs[i].b.pos.z);
    }
    noStroke();
    
    
    
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
    float buffer = 7;//helps avoid face-clipping
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

  public int[] spring2AdjTris(Spring s) {
    S_Particle a = s.a;
    S_Particle b = s.b;
    int[] ret = new int[2];
    int c = 0;
    
    boolean f = false;
    for (int t = 0; t < triIDs.length; t++) {
      int foundA = -1;
      int foundB = -1;
      for (int i = 0; i < 3; i++) {
        S_Particle v = parts[triIDs[t][i]];
        
        if (foundA == -1 && v == a)
          foundA = i;
        if (foundB == -1 && v == b)
          foundB = i;
      }
      f = true;
      if (foundA != -1 && foundB != -1) {
        ret[c] = t;
        c++;
      }
    }

    return ret;
  }

  public int triSpringTear(int before, int tear) {
    //println(before, tear);
    if (before == 3)
      return before - tear;
    else if(before == 2 && tear == 2)
      return 0;
    else if(before == 1 && tear == 1)
      return 0;
    else
      return before;
  }

  public void ApplyForces(float dt) {    
    for(int i = 0; i < parts.length; i++) {
      parts[i].acc = new Vec3(0,980,0);
    }
    
    /* spring force */
    for(int i = 0; i < springs.length; i++) {
      if (i < numSprings)
        springs[i].ApplyForce(dt);
    
      if (springs[i].torn) {
        int[] adj = springs[i].adj;//spring2AdjTris(springs[i]);
        
        int[] id = new int[2];
        id[0] = triIDs[adj[0]][3];
        id[1] = triIDs[adj[1]][3];
        
        triIDs[adj[0]][4] = triSpringTear(triIDs[adj[0]][4], 3);
        triIDs[adj[1]][4] = triSpringTear(triIDs[adj[1]][4], 3);

        if (i < numSprings) {
          Spring t = springs[i];
          springs[i] = springs[numSprings - 1];
          springs[numSprings - 1] = t;
          numSprings--;
          i--;
        }
      }
    }
    
    /* drag force */
    for(int i = 0; i < triIDs.length; i++) {
      if (triIDs[i][4] > 0) {
        S_Particle s0 = parts[triIDs[i][0]];
        S_Particle s1 = parts[triIDs[i][1]];
        S_Particle s2 = parts[triIDs[i][2]];
       
        //f_aero = -1/2 (density) |v^2| c_d * a * norm
        //1/2 ||(s1 - s0)x( s2-s0 )||
        Vec3 cross = s1.pos.sub(s0.pos).cross(s2.pos.sub(s0.pos));
        float a_0 = 0.5 * (cross).mag();
        //(s1-s0)x(s2-s0))/||(s1 - s0)x( s2-s0 )||
        Vec3 norm = cross.div(2*a_0);
        //( s0 + s1 + s2 ) / 3
        Vec3 v = s0.vel.add(s1.vel).add(s2.vel).div(3); 
        float a = a_0 * v.dot(norm);
        float density = 0.01;
        float c_d = 0.01;
        Vec3 f_aero = norm.mul(-0.5 * density * v.dot(v) * c_d * a/ 3);
        
        s0.acc = s0.acc.add(f_aero.mul(dt/s0.mass).mul(triIDs[i][4] == 1 ? 0 : 1));
        s1.acc = s1.acc.add(f_aero.mul(dt/s1.mass).mul(triIDs[i][4] == 2 ? 0 : 1));
        s2.acc = s2.acc.add(f_aero.mul(dt/s2.mass));
      }
    }
    /**/
  }
}