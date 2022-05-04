
#ifndef THREE_TYPES_H
#define THREE_TYPES_H
#include <limits>
#include <unordered_map>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cctype>
#include <locale>
#include <limits>
#include <ctime> 
#include <string>

namespace three {
	using byte = unsigned char;


	class Timer
	{	

	private:

		clock_t start_time;

	public:

		Timer() :
			start_time(clock()) { }

		float get_elapsed_time() const { return (float)(clock()- start_time); }

		void reset() { start_time = clock(); }
	};
	

	struct DrawRange {
		unsigned start =0;
		unsigned count = std::numeric_limits<unsigned>::infinity();
		int materialIndex=-1;
		DrawRange() :start(0), count(std::numeric_limits<unsigned>::infinity()), materialIndex(-1) {}
		DrawRange(unsigned start, unsigned count, int materialIndex) : start(start), count(count), materialIndex(materialIndex) {}
		bool equals(const DrawRange& target) {
			return start == target.start && count == target.count && materialIndex == target.materialIndex;
		}
	};

	struct UpdateRange {

		size_t start;

		size_t count;

		UpdateRange(size_t offset = 0, size_t count = std::numeric_limits<size_t>::max()) :start(offset), count(count) {}
	};
	typedef struct _Mipmap {
		std::vector<unsigned char> data;
		int width, height;
	} MipMap;


	template <class T>
	inline void hash_combine(std::size_t& seed, const T& v)
	{
		std::hash<T> hasher;
		seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}

	struct EnumHash {
		template <typename T>
		typename std::underlying_type<T>::type operator()(T t) const {
			return static_cast<typename std::underlying_type<T>::type>(t);
		}
	};
	
	template<typename EnumT,typename ValueT>
	using enum_map = std::unordered_map<EnumT, ValueT, EnumHash>;

	
	struct StringHash {
		size_t operator()(const std::string& key) const {
			const std::string_view sv{ key.c_str(),key.length() };
			return std::hash<std::string_view>()(sv);
		}
	};

	enum class Extension : size_t
	{
		ARB_depth_texture = 1,
		EXT_frag_depth = 1 << 1,
		EXT_texture_filter_anisotropic = 1 << 2,
		EXT_blend_minmax = 1 << 3,
		EXT_texture_compression_s3tc = 1 << 4,
		EXT_shader_texture_lod = 1 << 5,
		OES_texture_float = 1 << 6,
		OES_texture_float_linear = 1 << 7,
		OES_texture_half_float = 1 << 8,
		OES_texture_half_float_linear = 1 << 9,
		OES_standard_derivatives = 1 << 10,
		ANGLE_instanced_arrays = 1 << 11,
		OES_element_index_uint = 1 << 12,
		EXT_draw_buffers = 1 << 13,
		EXT_draw_buffers2 = 1 << 14,	
		ARB_instanced_arrays = 1<<15,
		ARB_ES3_compatibility=1<<16
	};

	static inline std::vector<std::string> split(std::string str, char delimiter) {
		std::vector<std::string> internal;		
		size_t size = str.find(delimiter);
		internal.reserve(size + 2);
		std::stringstream ss(str);
		std::string temp;
		while (std::getline(ss, temp, delimiter)) {
			internal.push_back(temp);
		}
		return internal;
	}
	// trim from start (in place)
	static inline void ltrim(std::string& s) {
		s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](int ch) {
			return !std::isspace(ch);
			}));
	}

	// trim from end (in place)
	static inline void rtrim(std::string& s) {
		s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
			return !std::isspace(ch);
			}).base(), s.end());
	}

	// trim from both ends (in place)
	static inline std::string& trim(std::string& s) {
		ltrim(s);
		rtrim(s);

		return s;
	}
	static inline std::string replace_all(
		__in std::string& message,
		__in const std::string& pattern,
		__in const std::string& replace
		) {
		std::string result = message;
		std::string::size_type pos = 0;
		std::string::size_type offset = 0;

		while ((pos = result.find(pattern, offset)) != std::string::npos)
		{

			result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);

			offset = pos + replace.size();

		}
		return result;	
	}
	size_t getStringHashcode(const std::string& shaderChunk);

	void checkError();

	void toUpperCase(std::string& str);

	void toLowerCase(std::string& str);

#ifdef _WIN32
#include<io.h>

#define access _access_s
#else
#include <unistd.h>
#endif
	bool FileExists(const std::string& fileName);	
	std::string getProgramPath();
}
#endif

