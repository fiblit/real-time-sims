vector3 forward = new vector3(0, 0, -1);
vector3 right = new vector3(1, 0, 0);

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
    forward = new vector3(
      cos(yaw*PI/180)*cos(pitch*PI/180),
      sin(pitch*PI/180),
      sin(yaw*PI/180)*cos(pitch*PI/180));
    right = new vector3(
      cos((yaw+90)*PI/180)*cos(pitch*PI/180),
      sin(pitch*PI/180),
      sin((yaw+90)*PI/180)*cos(pitch*PI/180));
    
    if (pFaceCam) {
      faceCam();
    }
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
  if (key == KEYmodeUP) {
    emitterMode++;
    if (emitterMode >= 7)
      emitterMode = 0;
  }
  if (key == KEYmodeDOWN) {
    emitterMode--;
    if (emitterMode < 0)
      emitterMode = 0;
  }
}

void keyPressed() {
  if (key == KEYspawnEmitter)
    println("spawn!");
  if (key == KEYregen) { // broken
    regen();
  }
}