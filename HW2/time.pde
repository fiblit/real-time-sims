private float mdt() {
  return frameNow - lastFrame;
}
private float sdt() {
  return mdt/1000;
}

private float lastFrame;
private float frameNow;
float sdt;
float mdt;

float time() {
  return frameNow;
}

void tick() {
  lastFrame = frameNow;
  frameNow = millis();
  mdt = mdt();
  sdt = sdt();
}