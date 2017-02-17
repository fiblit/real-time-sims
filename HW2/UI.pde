Vec3 forward = new Vec3(0, 0, -1);
Vec3 right = new Vec3(1, 0, 0);

boolean isCam(int k) {
  return 
      k == KEYpitchUp     || k == KEYpitchDown ||
      k == KEYyawLeft     || k == KEYyawRight;
}
void userInput(float dt) {
  //cam angle
  if (isCam(key)) {
    if (key == KEYpitchUp) {
      if (pitch > -90 + pitchspeed*dt)
        pitch -= pitchspeed*dt;
    }
    if (key == KEYyawLeft) {
      yaw -= yawspeed*dt;
      if (yaw < 0)
        yaw += 360;
    }
    if (key == KEYpitchDown) {
      if (pitch < 90-pitchspeed*dt)
        pitch += pitchspeed*dt;
    }
    if (key == KEYyawRight) {
      yaw += pitchspeed*dt;
      if (yaw > 360)
        yaw -= 360;
    }
    
    cameraCalc();
  }
   
  //cam strafe
  if (key == KEYstrafeFront) {
    eye.x += forward.x * camspeed * dt;
    eye.y += forward.y * camspeed * dt;
    eye.z += forward.z * camspeed * dt;
  }
  if (key == KEYstrafeBack) {
    eye.x -= forward.x * camspeed * dt;
    eye.y -= forward.y * camspeed * dt;
    eye.z -= forward.z * camspeed * dt;
  }
  if (key == KEYstrafeLeft) {
    float mag = sqrt(right.x * right.x + right.z * right.z);
    eye.x -= right.x / mag * camspeed * dt;
    eye.z -= right.z / mag * camspeed * dt;
  }
  if (key == KEYstrafeRight) {
    float mag = sqrt(right.x * right.x + right.z * right.z);
    eye.x += right.x / mag * camspeed * dt;
    eye.z += right.z / mag * camspeed * dt;
  }
  if (key == KEYstrafeUp) {
    eye.y -= camspeed * dt;
  }
  if (key == KEYstrafeDown) {
    eye.y += camspeed * dt;
  }
  
  //paddle strafe
  
  if (key == KEYpaddleYP) {
    padY -= padSpeed * dt;
  }
  if (key == KEYpaddleYN) {
    padY += padSpeed * dt;
  }
  if (key == KEYpaddleXP) {
    padX += padSpeed * dt;
  }
  if (key == KEYpaddleXN) {
    padX -= padSpeed * dt;
  }
  if (key == KEYpaddleZP) {
    padZ -= padSpeed * dt;
  }
  if (key == KEYpaddleZN) {
    padZ += padSpeed * dt;
  }
}

void cameraCalc() {
  if (pFaceCam) {
    faceCam();
  }
  else {
    noFace();
  }

  forward = new Vec3(
    cos(yaw*PI/180)*cos(pitch*PI/180),
    sin(pitch*PI/180),
    sin(yaw*PI/180)*cos(pitch*PI/180));
  right = new Vec3(
    cos((yaw+90)*PI/180)*cos(pitch*PI/180),
    sin(pitch*PI/180),
    sin((yaw+90)*PI/180)*cos(pitch*PI/180));
}

void keyPressed() {
  if (key == KEYspawnEmitter) {
    if (emitterMode != 8)
      if (emitterMode == 3)
        //for (int i = 0; i < random(8,12); i++)
          emitters.add(new Emitter(emitterMode, new Vec3(eye.x+forward.x*spawnDist, eye.y+forward.y*spawnDist, eye.z+forward.z*spawnDist)));
      else      
        emitters.add(new Emitter(emitterMode, new Vec3(eye.x+forward.x*spawnDist, eye.y+forward.y*spawnDist, eye.z+forward.z*spawnDist)));
  }
  if (key == KEYmodeUP) {
    emitterMode++;
    if (emitterMode > 7)
      emitterMode = 0;
  }
  if (key == KEYmodeDOWN) {
    emitterMode--;
    if (emitterMode < 0)
      emitterMode = 7;
  }
  if (key == KEYregen) {
    regen();
  }
}


String getModeName() {
  String s = new Integer(emitterMode).toString() + ": ";
  switch(emitterMode) {
    case 0: s += "Bouncy Ball (might need to wait for one to spawn)"; break;
    case 1: s += "Spout of WATER!"; break;
    case 2: s += "Heh. Fire."; break;
    case 3: s += "Magic Missle!"; break;
    case 4: s += "Fireball. 100k@30FPS bench attempt"; break;
    case 5: s += "Fireball. 90K@30+FPS bench"; break;
    case 6: s += "Snow!"; break;
    case 7: s += "Blank Canvas"; break;
  }
  return s;
}