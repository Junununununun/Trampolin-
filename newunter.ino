const int trigPin = 9; // 超声波传感器 Trig 引脚
const int echoPin = 10; // 超声波传感器 Echo 引脚

void setup() {
  Serial.begin(115200); // 初始化串口通信速率
  pinMode(trigPin, OUTPUT); // 设置 Trig 引脚为输出
  pinMode(echoPin, INPUT);  // 设置 Echo 引脚为输入
}

long getDistanceCM() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); // 设置超时为 30ms
  if (duration == 0) {
    return 9999; // 超时返回一个大值
  }

  long distanceCM = duration * 0.034 / 2; // 计算距离
  return distanceCM;
}

void loop() {
  long distance = getDistanceCM(); // 获取距离
  Serial.println(distance); // 打印距离值到串口
  delay(50); // 延时 50ms，避免过于频繁的测量
}
