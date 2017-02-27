float mdt() {
  return frameNow - lastFrame;
}
float sdt() {
  return mdt()/1000;
}

float lastFrame;
float frameNow;
float sdt;
//float clock = 0;

void tick() {
  lastFrame = frameNow;
  frameNow = millis();
  sdt = sdt();
}