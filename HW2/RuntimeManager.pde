Particle[] ps;
int pcount;

ArrayList<Emitter> emitters;

void gen() {
  ps = new Particle[maxcount];
  for (int i = 0; i < maxcount; i++) {
    ps[i] = new Particle();
  }
  emitters = new ArrayList<Emitter>();
  pcount = 0;
}

void regen() {
  println("start gen");
  gen();
  println("finish gen");
  buildMode();
  println("finish build");
}

void p_gen(float dt) {
  for (int e = 0; e < emitters.size(); e++) {
    Emitter emit = emitters.get(e);
    if (emit.genRate * dt > 1) {
      for (int i = 0; i < emit.genRate * dt; i++) {
        if (pcount < maxcount) {
          emit.genRandParticle(pcount);
          pcount++;
        }
      }
    } else if (random(1) <= emit.genRate * dt) {
      if (pcount < maxcount) {
        emit.genRandParticle(pcount);
        pcount++;
      }
    }
  }
}

void moveBtoA(int a, int b) {
  Particle t = ps[a];
  ps[a] = ps[b];
  ps[b] = t;
}

void e_kill(float dt) {
  for (int e = 0; e < emitters.size(); e++) {
    Emitter emit = emitters.get(e);
    if(emit.eMaxLife > 0) { 
      if (emit.eLife > emit.eMaxLife) {
        emitters.remove(e);
        e--;
      }
      else {
        emit.eLife += dt;
      }
    }
  }
}

boolean p_kill(float dt, int i) {
  if (ps[i].life >= ps[i].maxlife) {
    moveBtoA(i, pcount - 1);
    pcount -= 1;
    return true;
  } else {
    ps[i].life += dt;
    return false;
  }
}