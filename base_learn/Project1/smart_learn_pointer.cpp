#include<iostream>
#include<vld.h>
#include<memory>

int main() {
	//������new����ʼ������ָ��
	std::shared_ptr	<int> shared_ptr(new int(100));
	//��������ֻ����New,������Ĵ�������ָ�����Ĳ�����
	std::shared_ptr	<int> shared_ptr(new int[100]());
	//make_shared��Ϊ�淶�Ժ��������ʼ��
	std::shared_ptr	<int> shared_LD = std::make_shared<int>(100);
	std::shared_ptr	<int> shared_LD = std::make_shared<int>(100);
	//Ҳ�����ø��ƹ��ܺ�������ʼ��
	std::shared_ptr	<int> shared_LD2(shared_LD);
	//shared_LD.use_count()���Բ������ָ������ü�����reset()����ʹ�øö����ڴ��ָ������ΪNull��
	//shared_LD.swap(shared_LD2)����������ָ��ָ����ڴ��ַ������

	//weak_ptrֻ���õ�ַ�����������ü���,���������ڴ滥��ָ������ҪWeak�����ѭ������
	std::weak_ptr<int> weakid(shared_LD);

	//unque_ptr�ǶԶ�ռ�ڴ������ָ�룬���ʼ����Share_ptr��ȫ��ͬ
	std::unique_ptr<int>uniqueid(new int(100));
	std::unique_ptr<int>uniqueid = std::make_unique<int>(100);
	


	//����ָ���ڴ��������ʱ����Ĳ����ã��ܶ������������ʵ������ֻ������ָͨ�롣
	return 0;
}
