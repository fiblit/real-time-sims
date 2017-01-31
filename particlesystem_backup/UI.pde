float yaw = 270;
float pitch = 0;
vector3 forward = new vector3(0, 0, -1);
vector3 right = new vector3(1, 0, 0);
vector3 eye = new vector3(500, -300, width);

void userInput() {
  /* this appears to cause a significant amount of lag when changing */
  if (key == KEYpitchUp ||
      key == KEYpitchDown || 
      key == KEYyawLeft || 
      key == KEYyawRight) {
    if (key == KEYpitchUp) {
      if (pitch > -90 + pitchspeed)
        pitch -= pitchspeed;
    }
    if (key == KEYyawLeft) {
      yaw -= yawspeed;
      if (yaw < 0)
        yaw += 360;
    }
    if (key == KEYpitchDown) {
      if (pitch < 90-pitchspeed)
        pitch += pitchspeed;
    }
    if (key == KEYyawRight) {
      yaw += pitchspeed;
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
  }
  
  /* no apparent lag */
  if (key == KEYstrafeFront) {
    eye.x += forward.x * camspeed;
    eye.y += forward.y * camspeed;
    eye.z += forward.z * camspeed;
  }
  if (key == KEYstrafeBack) {
    eye.x -= forward.x * camspeed;
    eye.y -= forward.y * camspeed;
    eye.z -= forward.z * camspeed;
  }
  if (key == KEYstrafeLeft) {
    eye.x -= right.x * camspeed;
    //eye.y -= right.y * camspeed; //feels weird AF
    eye.z -= right.z * camspeed;
  }
  if (key == KEYstrafeRight) {
    eye.x += right.x * camspeed;
    //eye.y += right.y * camspeed; //feels weird AF
    eye.z += right.z * camspeed;
  }
}

void keyPressed() {
  if (key == KEYgenRatePlus) {
    genRate += deltaGenRate;
  }
  if (key == KEYgenRateMinus) {
    genRate -= deltaGenRate;
  }
  if (key == KEYdebug) {
    println("fps: " + frameRate);  
    println("rate: " + genRate);
    println("particles: " + pcount);
  }
  if (key == KEYregen) {
    rgpcount = startcount;
    regen();
  }
}