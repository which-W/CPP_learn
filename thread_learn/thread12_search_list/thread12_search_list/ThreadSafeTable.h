#pragma once
#include <iostream>

#include <thread>
#include <vector>
#include <list>
#include <memory>
#include <mutex>
#include <shared_mutex>
#include <iterator>
#include <map>


template<typename Key, typename Value, typename Hash = std::hash<Key>>
class threadsafe_lookup_table
{
private:

	// Ͱ����
	class bucket_type
	{
		friend class threadsafe_lookup_table;
	private:
		//�洢Ԫ�ص�����Ϊpair����key��value����
		typedef std::pair<Key, Value> bucket_value;
		//������洢Ԫ�ع�
		typedef std::list<bucket_value> bucket_data;
		//����ĵ�����
		typedef typename bucket_data::iterator bucket_iterator;
		//��������
		bucket_data data;
		//���ù�����
		mutable std::shared_mutex mutex;
		//���Ҳ�������list���ҵ�ƥ���keyֵ��Ȼ�󷵻ص�����
		bucket_iterator find_entry_for(const Key& key)
		{
			return std::find_if(data.begin(), data.end(),
				[&](bucket_value const& item)
				{return item.first == key; });
		}
	public:
		//����keyֵ���ҵ����ض�Ӧ��value��δ�ҵ��򷵻�Ĭ��ֵ
		Value value_for(Key const& key, Value const& default_value)
		{
			std::shared_lock<std::shared_mutex> lock(mutex);
			bucket_iterator const found_entry = find_entry_for(key);
			return (found_entry == data.end()) ?
				default_value : found_entry->second;
		}
		//���key��value���ҵ�����£�û�ҵ������
		void add_or_update_mapping(Key const& key, Value const& value)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			bucket_iterator const found_entry = find_entry_for(key);
			if (found_entry == data.end())
			{
				data.push_back(bucket_value(key, value));
			}
			else
			{
				found_entry->second = value;
			}
		}
		//ɾ����Ӧ��key
		void remove_mapping(Key const& key)
		{
			std::unique_lock<std::shared_mutex> lock(mutex);
			bucket_iterator const found_entry = find_entry_for(key);
			if (found_entry != data.end())
			{
				data.erase(found_entry);
			}
		}
	};
	//��vector�洢Ͱ����
	std::vector<std::unique_ptr<bucket_type>> buckets;
	//hash<Key> ��ϣ�� ��������key���ɹ�ϣֵ
	Hash hasher;

	//����key�������֣�����Ͱ�Ĵ�Сȡ��õ��±꣬�����±귵�ض�Ӧ��Ͱ����ָ��
	bucket_type& get_bucket(Key const& key) const
	{
		std::size_t const bucket_index = hasher(key) % buckets.size();
		return *buckets[bucket_index];
	}


public:

	threadsafe_lookup_table(
		unsigned num_buckets = 19, Hash const& hasher_ = Hash()) :
		buckets(num_buckets), hasher(hasher_)
	{
		for (unsigned i = 0; i < num_buckets; ++i)
		{
			buckets[i].reset(new bucket_type);
		}
	}

	threadsafe_lookup_table(threadsafe_lookup_table const& other) = delete;
	threadsafe_lookup_table& operator=(
		threadsafe_lookup_table const& other) = delete;

	Value value_for(Key const& key,
		Value const& default_value = Value())
	{
		return get_bucket(key).value_for(key, default_value);
	}

	void add_or_update_mapping(Key const& key, Value const& value)
	{
		get_bucket(key).add_or_update_mapping(key, value);
	}

	void remove_mapping(Key const& key)
	{
		get_bucket(key).remove_mapping(key);
	}

	std::map<Key, Value> get_map()
	{
		std::vector<std::unique_lock<std::shared_mutex>> locks;
		for (unsigned i = 0; i < buckets.size(); ++i)
		{
			locks.push_back(std::unique_lock<std::shared_mutex>(buckets[i]->mutex));
		}
		std::map<Key, Value> res;
		for (unsigned i = 0; i < buckets.size(); ++i)
		{
			//����typename���߱�����bucket_type::bucket_iterator��һ�����ͣ��Ժ���ʵ����
			//��Ȼ�˴��ɼ�д��auto it = buckets[i]->data.begin();
			typename bucket_type::bucket_iterator it = buckets[i]->data.begin();
			for (; it != buckets[i]->data.end(); ++it)
			{
				res.insert(*it);
			}
		}
		return res;
	}
};