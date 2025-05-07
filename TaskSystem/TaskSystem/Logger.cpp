#include "Logger.h"
namespace fs = std::filesystem;
void Logger::open_log_file() {
	if (_log_file.is_open()) {
		_log_file.close();
	}
	_log_file.open(_base_filename, std::ios::out | std::ios::app);
}

void Logger::check_and_rotate_log_file() {
	if (get_file_size() >= _max_file_size) {
		rotate_log_file();
	}
}

size_t Logger::get_file_size() {
	if (!fs::exists(_base_filename)) {
		return 0;
	}
	return (size_t)fs::file_size(_base_filename);
}


void Logger::rotate_log_file() {
	_log_file.close();

	std::string new_log_name = _base_filename + "." + std::to_string(++_current_log_index);

	if (fs::exists(_base_filename)) {
		fs::rename(_base_filename, new_log_name);
	}


	open_log_file(); // 打开新的日志文件
}

//时间戳
std::string Logger::get_time() {
	auto now = std::chrono::system_clock::now();
	std::time_t time = std::chrono::system_clock::to_time_t(now);
	char buffer[100];
	std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time));
	return std::string(buffer);
}

