# thread_singleton_test

单例模式并发读取测试 / 多线程同步读取加速

## 结果

```
// 线程数：2
// 任务数：8
// 本地延时：+200ms
// 在线延时：1000ms

Just from file Serial time: 1786ms
Just from Web Serial time: 8140ms
Serial time: 4973ms
Just from file Parallel time: 1753ms
Just from Web Parallel time: 5001ms
Parallel time: 3000ms
```

## 依赖

- openMP
- openCV
