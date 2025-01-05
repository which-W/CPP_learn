#include <variant>
#include <vector>
#include <unordered_map>
#include <string>
#include <string_view>
#include <optional>
#include <regex>
#include <charconv>
#include "print.h"

struct Myjson_Object
{
	std::variant <
		std::nullptr_t //null
		,bool //true
		,int  //10
		,double // 1.12
		,std::string // "hello"
		,std::vector<Myjson_Object> // ["hello" , "10"]
		,std::unordered_map<std::string, Myjson_Object> //{"hello" : 985 , "name" : 211}
	>inner;

	void do_print() const {
		print(inner);
	};

	template <class T>
	bool is() const {
		return std::holds_alternative<T>(inner);
	}

	template <class T>
	T const& get() const {
		return std::get<T>(inner);
	}

	template <class T>
	T& get() {
		return std::get<T>(inner);
	}
};

char unescaped_char(char c) {
	switch (c) {
	case 'n': return '\n';
	case 'r': return '\r';
	case '0': return '\0';
	case 't': return '\t';
	case 'v': return '\v';
	case 'f': return '\f';
	case 'b': return '\b';
	case 'a': return '\a';
	default: return c;
	}
}


template <class T>
std::optional<T> try_parse_num(std::string_view str) {
	T value;
	auto res = std::from_chars(str.data(), str.data() + str.size(), value);//转为数字类型
	if (res.ec == std::errc() && res.ptr == str.data() + str.size()) { // 判断是否出错
		return value;
	}
	return std::nullopt;
}


std::pair<Myjson_Object, size_t> parse(std::string_view json) {
    if (json.empty()) {
        return { Myjson_Object{std::nullptr_t{}}, 0 };
    }
    else if (size_t off = json.find_first_not_of(" \n\r\t\v\f\0"); off != 0 && off != json.npos) {
        auto [obj, eaten] = parse(json.substr(off));
        return { std::move(obj), eaten + off };
    }
    else if ('0' <= json[0] && json[0] <= '9' || json[0] == '+' || json[0] == '-') {
        std::regex num_re{ "[+-]?[0-9]+(\\.[0-9]*)?([eE][+-]?[0-9]+)?" };
        std::cmatch match;
        if (std::regex_search(json.data(), json.data() + json.size(), match, num_re)) {
            std::string str = match.str();
            if (auto num = try_parse_num<int>(str)) {
                return { Myjson_Object{*num}, str.size() };
            }
            if (auto num = try_parse_num<double>(str)) {
                return { Myjson_Object{*num}, str.size() };
            }
        }
    }
    else if (json[0] == '"') {
        std::string str;
        enum {
            Raw,
            Escaped,
        } phase = Raw;
        size_t i;
        for (i = 1; i < json.size(); i++) {
            char ch = json[i];
            if (phase == Raw) {
                if (ch == '\\') {
                    phase = Escaped;
                }
                else if (ch == '"') {
                    i += 1;
                    break;
                }
                else {
                    str += ch;
                }
            }
            else if (phase == Escaped) {
                str += unescaped_char(ch);
                phase = Raw;
            }
        }
        return { Myjson_Object{std::move(str)}, i };
    }
    else if (json[0] == '[') {
        std::vector<Myjson_Object> res;
        size_t i;
        for (i = 1; i < json.size();) {
            if (json[i] == ']') {
                i += 1;
                break;
            }
            auto [obj, eaten] = parse(json.substr(i));
            if (eaten == 0) {
                i = 0;
                break;
            }
            res.push_back(std::move(obj));
            i += eaten;
            if (json[i] == ',') {
                i += 1;
            }
        }
        return { Myjson_Object{std::move(res)}, i };
    }
    else if (json[0] == '{') {
        std::unordered_map<std::string, Myjson_Object> res;
        size_t i;
        for (i = 1; i < json.size();) {
            if (json[i] == '}') {
                i += 1;
                break;
            }
            auto [keyobj, keyeaten] = parse(json.substr(i));
            if (keyeaten == 0) {
                i = 0;
                break;
            }
            i += keyeaten;
            if (!std::holds_alternative<std::string>(keyobj.inner)) {
                i = 0;
                break;
            }
            if (json[i] == ':') {
                i += 1;
            }
            std::string key = std::move(std::get<std::string>(keyobj.inner));
            auto [valobj, valeaten] = parse(json.substr(i));
            if (valeaten == 0) {
                i = 0;
                break;
            }
            i += valeaten;
            res.try_emplace(std::move(key), std::move(valobj));
            if (json[i] == ',') {
                i += 1;
            }
        }
        return { Myjson_Object{std::move(res)}, i };
    }
    return { Myjson_Object{std::nullptr_t{}}, 0 };
}

template <class ...Fs>
struct overloaded : Fs... {
    using Fs::operator()...;
};

template <class ...Fs>
overloaded(Fs...) -> overloaded<Fs...>;


int main() {
	const char* str = "hello";
	print(str);
	return 0;
}