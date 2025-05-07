#pragma once
#include "global.h"
#include "LogQueue.h"

class Logger 
{
public:
	static  Logger& GetInstance()
	{
		static  Logger single;
		return single;
	}
	// 新增：默认构造函数用于单例
	Logger() : _exit_flag(false),
		_console_output(false),
		_max_file_size(100 * 1024),
		_base_filename("Task.log") {
		open_log_file();
		if (!_log_file) {
			throw std::runtime_error("log file open is error");
		}

		_work_threads = std::thread([this]() {
			std::string msg;
			while (_log_queue.pop(msg)) {
				check_and_rotate_log_file();
				_log_file << msg << std::endl;
				if (_console_output) {
					std::cout << msg << std::endl;
				}
			}
			});
	}
	Logger(const Logger&& other) = delete;
	Logger& operator = (const Logger&& other) = delete;
	Logger(const Logger& other) = delete;
	Logger& operator = (const Logger& other) = delete;

	Logger(const std::string& filename, bool console_output = false, size_t max_file_size_kb = 100) :_exit_flag(false)
		, _console_output(console_output), _max_file_size(max_file_size_kb * 1024), _base_filename(filename) {
		open_log_file();
		if (!_log_file) {
			throw std::runtime_error("log file open is error");
		}

		_work_threads = std::thread([this]() {
			std::string msg;
			while (_log_queue.pop(msg)) {
				check_and_rotate_log_file();
				_log_file << msg << std::endl;
				if (_console_output) {
					std::cout << msg << std::endl;
				}
			}

			});
	}
	~Logger() {
		_exit_flag = true;
		_log_queue.shutdown();
		if (_work_threads.joinable()) {
			_work_threads.join();
		}
		if (_log_file.is_open()) {
			_log_file.close();
		}
	}


	template<typename ...Args>
	void log(LogLevel Loglevel, const std::string& format, Args&& ...args) {
		std::lock_guard<std::mutex> lock(mtx);
		std::string loglevel_str;
		switch (Loglevel) {
		case LogLevel::INFO: loglevel_str = "[INFO] "; break;
		case LogLevel::DEBUG: loglevel_str = "[DEBUG] "; break;
		case LogLevel::ERROR: loglevel_str = "[ERROR] "; break;
		}
		_log_queue.push(loglevel_str + formatMessage(format, std::forward<Args>(args)...));
	}
	template<typename... Args>
	void console_log(LogLevel level, const std::string& format, Args&&... args) {
		std::lock_guard<std::mutex> lock(mtx);
		std::string loglevel_str;
		switch (level) {
		case INFO: loglevel_str = "[INFO] "; break;
		case DEBUG: loglevel_str = "[DEBUG] "; break;
		case WARN: loglevel_str = "[WARN] "; break;
		case ERROR: loglevel_str = "[ERROR] "; break;
		}
		std::cout << (loglevel_str + formatMessage(format, std::forward<Args>(args)...)) << std::endl;
	}



private:
	void open_log_file();

	void check_and_rotate_log_file();

	size_t get_file_size();

	void rotate_log_file();


	//时间戳
	std::string get_time();
	// 使用模板折叠格式化日志消息，支持 "{}" 占位符
	template<typename... Args>
	std::string formatMessage(const std::string& format, Args&&... args) {
		std::vector<std::string> arg_strings = { to_string_helper(std::forward<Args>(args))... };
		std::ostringstream oss;
		size_t arg_index = 0;
		size_t pos = 0;
		size_t placeholder = format.find("{}", pos);

		while (placeholder != std::string::npos) {
			oss << format.substr(pos, placeholder - pos);
			if (arg_index < arg_strings.size()) {
				oss << arg_strings[arg_index++];
			}
			else {
				// 没有足够的参数，保留 "{}"
				oss << "{}";
			}
			pos = placeholder + 2; // 跳过 "{}"
			placeholder = format.find("{}", pos);
		}

		// 添加剩余的字符串
		oss << format.substr(pos);

		// 如果还有剩余的参数，按原方式拼接
		while (arg_index < arg_strings.size()) {
			oss << arg_strings[arg_index++];
		}

		return "[" + get_time() + "] " + oss.str();
	}



	std::thread _work_threads;
	std::ofstream _log_file;
	std::atomic<bool> _exit_flag;
	LogQueue _log_queue;
	bool _console_output; // 控制是否输出到控制台
	size_t _max_file_size;
	std::string _base_filename;
	int _current_log_index = 0;
	std::mutex mtx; // 互斥锁，保护日志输出
};

