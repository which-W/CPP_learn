#include<iostream>
#include<thread>
#include<mutex>
#include<atomic>
//ʹ������ʵ�����̺߳������߳�֮��ͬʱռ�������������������ݴ���
//��mymutex��lock�����׳�������unlock������Ϊ���������Unlockδ�����ж��߳̽����������³�������
//����һ��ʹ��lock_guard����Ϊ��
//Ҳ������ԭ�Ӳ���ʹ�����ݲ��ɷָ�������ﵽͬʱֻ��һ���߳�ʹ������
//�磺 std::atomic<unsigned>ans = 0;
//ע�����Ἣ��Ӱ������Ч�����Ա�����ڱ�Ҫ����������֮�ࡣ
unsigned  ans = 0;
std::mutex mymutex;
void test() {
	
	//��{}�е����ݽ��������lg������������������Mymutex��unlock����
	
	for (int i; i < 20000; i++) {
		std::lock_guard<std::mutex>lg(mymutex);
		ans++;
	}
}
int main() {
	std::thread thr(test);
	for (int i; i < 20000; i++) {
		std::lock_guard<std::mutex>lg(mymutex);
		ans++;
	}
	thr.join();
	std::cout << ans << std::endl;
	return 0;
}