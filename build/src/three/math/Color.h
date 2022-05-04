#ifndef THREE_COLOR_H
#define THREE_COLOR_H
#include <three/utils/osdecl.h>
#include <three/math/MathUtils.h>
#include <iostream>
#include <sstream>
#include <iomanip>
namespace three {
	namespace math {
		enum class ColorKeywords {
			aliceblue = 0xf0f8ff,
			antiquewhite = 0xfaebd7,
			aqua = 0x00ffff,
			aquamarine = 0x7fffd4,
			azure = 0xf0ffff,
			beige = 0xf5f5dc,
			bisque = 0xffe4c4,
			black = 0x000000,
			blanchedalmond = 0xffebcd,
			blue = 0x0000ff,
			blueviolet = 0x8a2be2,
			brown = 0xa52a2a,
			burlywood = 0xdeb887,
			cadetblue = 0x5f9ea0,
			chartreuse = 0x7fff00,
			chocolate = 0xd2691e,
			coral = 0xff7f50,
			cornflowerblue = 0x6495ed,
			cornsilk = 0xfff8dc,
			crimson = 0xdc143c,
			cyan = 0x00ffff,
			darkblue = 0x00008b,
			darkcyan = 0x008b8b,
			darkgoldenrod = 0xb8860b,
			darkgray = 0xa9a9a9,
			darkgreen = 0x006400,
			darkgrey = 0xa9a9a9,
			darkkhaki = 0xbdb76b,
			darkmagenta = 0x8b008b,
			darkolivegreen = 0x556b2f,
			darkorange = 0xff8c00,
			darkorchid = 0x9932cc,
			darkred = 0x8b0000,
			darksalmon = 0xe9967a,
			darkseagreen = 0x8fbc8f,
			darkslateblue = 0x483d8b,
			darkslategray = 0x2f4f4f,
			darkslategrey = 0x2f4f4f,
			darkturquoise = 0x00ced1,
			darkviolet = 0x9400d3,
			deeppink = 0xff1493,
			deepskyblue = 0x00bfff,
			dimgray = 0x696969,
			dimgrey = 0x696969,
			dodgerblue = 0x1e90ff,
			firebrick = 0xb22222,
			floralwhite = 0xfffaf0,
			forestgreen = 0x228b22,
			fuchsia = 0xff00ff,
			gainsboro = 0xdcdcdc,
			ghostwhite = 0xf8f8ff,
			gold = 0xffd700,
			goldenrod = 0xdaa520,
			gray = 0x808080,
			green = 0x008000,
			greenyellow = 0xadff2f,
			grey = 0x808080,
			honeydew = 0xf0fff0,
			hotpink = 0xff69b4,
			indianred = 0xcd5c5c,
			indigo = 0x4b0082,
			ivory = 0xfffff0,
			khaki = 0xf0e68c,
			lavender = 0xe6e6fa,
			lavenderblush = 0xfff0f5,
			lawngreen = 0x7cfc00,
			lemonchiffon = 0xfffacd,
			lightblue = 0xadd8e6,
			lightcoral = 0xf08080,
			lightcyan = 0xe0ffff,
			lightgoldenrodyellow = 0xfafad2,
			lightgray = 0xd3d3d3,
			lightgreen = 0x90ee90,
			lightgrey = 0xd3d3d3,
			lightpink = 0xffb6c1,
			lightsalmon = 0xffa07a,
			lightseagreen = 0x20b2aa,
			lightskyblue = 0x87cefa,
			lightslategray = 0x778899,
			lightslategrey = 0x778899,
			lightsteelblue = 0xb0c4de,
			lightyellow = 0xffffe0,
			lime = 0x00ff00,
			limegreen = 0x32cd32,
			linen = 0xfaf0e6,
			magenta = 0xff00ff,
			maroon = 0x800000,
			mediumaquamarine = 0x66cdaa,
			mediumblue = 0x0000cd,
			mediumorchid = 0xba55d3,
			mediumpurple = 0x9370db,
			mediumseagreen = 0x3cb371,
			mediumslateblue = 0x7b68ee,
			mediumspringgreen = 0x00fa9a,
			mediumturquoise = 0x48d1cc,
			mediumvioletred = 0xc71585,
			midnightblue = 0x191970,
			mintcream = 0xf5fffa,
			mistyrose = 0xffe4e1,
			moccasin = 0xffe4b5,
			navajowhite = 0xffdead,
			navy = 0x000080,
			oldlace = 0xfdf5e6,
			olive = 0x808000,
			olivedrab = 0x6b8e23,
			orange = 0xffa500,
			orangered = 0xff4500,
			orchid = 0xda70d6,
			palegoldenrod = 0xeee8aa,
			palegreen = 0x98fb98,
			paleturquoise = 0xafeeee,
			palevioletred = 0xdb7093,
			papayawhip = 0xffefd5,
			peachpuff = 0xffdab9,
			peru = 0xcd853f,
			pink = 0xffc0cb,
			plum = 0xdda0dd,
			powderblue = 0xb0e0e6,
			purple = 0x800080,
			rebeccapurple = 0x663399,
			red = 0xff0000,
			rosybrown = 0xbc8f8f,
			royalblue = 0x4169e1,
			saddlebrown = 0x8b4513,
			salmon = 0xfa8072,
			sandybrown = 0xf4a460,
			seagreen = 0x2e8b57,
			seashell = 0xfff5ee,
			sienna = 0xa0522d,
			silver = 0xc0c0c0,
			skyblue = 0x87ceeb,
			slateblue = 0x6a5acd,
			slategray = 0x708090,
			slategrey = 0x708090,
			snow = 0xfffafa,
			springgreen = 0x00ff7f,
			steelblue = 0x4682b4,
			tan = 0xd2b48c,
			teal = 0x008080,
			thistle = 0xd8bfd8,
			tomato = 0xff6347,
			turquoise = 0x40e0d0,
			violet = 0xee82ee,
			wheat = 0xf5deb3,
			white = 0xffffff,
			whitesmoke = 0xf5f5f5,
			yellow = 0xffff00,
			yellowgreen = 0x9acd32,
		};
		inline float hue2rgb(float p, float q, float t) {
			if (t < 0) t += 1;
			if (t > 1) t -= 1;
			if (t < 1.0f / 6.0f) return p + (q - p) * 6 * t;
			if (t < 1.0f / 2.0f) return q;
			if (t < 2.0f / 3.0f) return p + (q - p) * 6 * (2.0f / 3.0f - t);

			return p;
		}

		inline float SRGBToLinear(float c)
		{
			return (c < 0.04045f) ? c * 0.0773993808f : std::pow(c * 0.9478672986f + 0.0521327014f, 2.4f);
		}

		inline float LinearToSRGB(float c) {
			return (c < 0.0031308f) ? c * 12.92f : 1.055f * std::pow(c, 0.41666f) - 0.055f;
		}


		typedef struct _HSL {
			union {
				struct {
					float h, s, l;
				};
				float elements[3];
			};
		} HSL;

		class Color {
		public:
			union {
				struct {
					float r, g, b;
				};
				float elements[3];
			};			
				
			using ptr = std::shared_ptr<Color>;
			
			Color() {
				r = std::numeric_limits<float>::infinity();
				g = std::numeric_limits<float>::infinity();
				b = std::numeric_limits<float>::infinity();
			}

			Color(float r, float g, float b) : r(r), g(g), b(b) {}
			Color(const Color& color) : r(color.r), g(color.g), b(color.b) {}
			Color(unsigned hex) : r((hex >> 16 & 255) / 255.0f), g((hex >> 8 & 255) / 255.0f), b((hex & 255) / 255.0f) {}

			Color(ColorKeywords name) : Color((unsigned)name) {}

			Color& operator = (const Color& source) {
				return copy(source);
			}
			bool operator == (const Color& source) const {
				return r == source.r && g == source.g && b == source.b;
			}
			bool isNull() const {
				return r == std::numeric_limits<float>::infinity() ||
					g == std::numeric_limits<float>::infinity() ||
					b == std::numeric_limits<float>::infinity();
			}
			Color& set(Color& value) {
				copy(value);
				return *this;
			}
			Color& set(unsigned value) {
				setHex(value);
				return *this;
			}

			Color& set(ColorKeywords name) {
				setHex((unsigned)name);
				return *this;
			}
			Color& setScalar(float scalar) {

				r = scalar;
				g = scalar;
				b = scalar;

				return *this;

			}

			Color& setHex(unsigned hex) {

				hex = (unsigned)std::floor(hex);

				r = (hex >> 16 & 255) / 255.0f;
				g = (hex >> 8 & 255) / 255.0f;
				b = (hex & 255) / 255.0f;

				return *this;

			}

			Color& setRGB(float r, float g, float b) {

				this->r = r;
				this->g = g;
				this->b = b;

				return *this;

			}

			Color& setHSL(float h, float s, float l) {

				// h,s,l ranges are in 0.0 - 1.0
				h = euclideanModulo(h, 1);
				s = math::clamp<float>(s, 0.0f, 1.0f);
				l = math::clamp<float>(l, 0.0f, 1.0f);

				if (s ==  0) {

					r = g = b = l;

				}
				else {

					auto p = l <= 0.5f ? l * (1 + s) : l + s - (l * s);
					auto q = (2 * l) - p;

					r = hue2rgb(q, p, h + 1 / 3.0f);
					g = hue2rgb(q, p, h);
					b = hue2rgb(q, p, h - 1 / 3.0f);

				}

				return *this;

			}
					
			Color& setColorName(ColorKeywords style) {
				setHex((unsigned)style);
				return *this;
			}

			Color& clone(){

				return *this;
			}

			Color& copy(const Color& color) {

				r = color.r;
				g = color.g;
				b = color.b;

				return *this;

			}

			Color& copyGammaToLinear(const Color& color, float gammaFactor=0) {

				if (gammaFactor == 0) gammaFactor = 2.0f;

				r = (float)std::pow(color.r, gammaFactor);
				g = (float)std::pow(color.g, gammaFactor);
				b = (float)std::pow(color.b, gammaFactor);

				return *this;
			}

			Color& copyLinearToGamma(const Color& color, float gammaFactor=0) {

				if (gammaFactor == 0) gammaFactor = 2.0f;

				float safeInverse = (gammaFactor > 0) ? (1.0f / gammaFactor) : 1.0f;

				r = std::pow(color.r, safeInverse);
				g = std::pow(color.g, safeInverse);
				b = std::pow(color.b, safeInverse);

				return *this;

			}

			Color& convertGammaToLinear(float gammaFactor=0) {

				copyGammaToLinear(*this, gammaFactor);

				return *this;

			}

			Color& convertLinearToGamma(float gammaFactor=0) {

				copyLinearToGamma(*this, gammaFactor);

				return *this;

			}

			Color& copySRGBToLinear(const Color& color) {

				r = SRGBToLinear(color.r);
				g = SRGBToLinear(color.g);
				b = SRGBToLinear(color.b);

				return *this;

			}

			Color& copyLinearToSRGB(const Color& color) {

				r = LinearToSRGB(color.r);
				g = LinearToSRGB(color.g);
				b = LinearToSRGB(color.b);

				return *this;

			}

			Color& convertSRGBToLinear() {

				copySRGBToLinear(*this);

				return *this;

			}

			Color& convertLinearToSRGB() {

				copyLinearToSRGB(*this);

				return *this;

			}

			unsigned getHex() {

				return (unsigned)(r * 255.0f) << 16 ^ (unsigned)(g * 255.0f) << 8 ^ (unsigned)(b * 255.0f) << 0;

			}

			std::string getHexString() {

				std::stringstream stream;
				stream << "0x" << std::setfill('0') << std::setw(6) << std::hex << getHex();

				return stream.str();
			}

			void getHSL(HSL* target) const {

				// h,s,l ranges are in 0.0 - 1.0

				

				const float r = this->r, g = this->g, b = this->b;

				float max = std::max(r, std::max(g, b));
				float min = std::min(r, std::min(g, b));

				float hue = 0, saturation=0;
				float lightness = (min + max) / 2.0f;

				if (min ==  max) {

					hue = 0;
					saturation = 0;

				}
				else {

					float delta = max - min;

					saturation = lightness <= 0.5f ? delta / (max + min) : delta / (2 - max - min);

					if(max==r)
						hue = (g - b) / delta + (g < b ? 6 : 0); 
					else if(max==g)
						hue = (b - r) / delta + 2;
					else if(max==b)
						hue = (r - g) / delta + 4; 									

					hue /= 6;
				}

				target->h = hue;
				target->s = saturation;
				target->l = lightness;

				

			}

			Color& offsetHSL(float h, float s, float l) {
				HSL _hslA;
				getHSL(&_hslA);

				_hslA.h += h; _hslA.s += s; _hslA.l += l;

				setHSL(_hslA.h, _hslA.s, _hslA.l);

				return *this;

			}

			Color& add(Color& color) {

				r += color.r;
				g += color.g;
				b += color.b;

				return *this;

			}

			Color& addColors(Color& color1, Color& color2) {

				r = color1.r + color2.r;
				g = color1.g + color2.g;
				b = color1.b + color2.b;

				return *this;

			}

			Color& addScalar(float s) {

				r += s;
				g += s;
				b += s;

				return *this;

			}

			Color& sub(const Color& color) {

				r = std::max(0.0f, r - color.r);
				g = std::max(0.0f, g - color.g);
				b = std::max(0.0f, b - color.b);

				return *this;

			}

			Color& multiply(const Color&color) {

				r *= color.r;
				g *= color.g;
				b *= color.b;

				return *this;

			}

			Color& multiplyScalar(float s) {

				r *= s;
				g *= s;
				b *= s;

				return *this;

			}

			Color& lerp(const Color& color, float alpha) {

				r += (color.r - r) * alpha;
				g += (color.g - g) * alpha;
				b += (color.b - b) * alpha;

				return *this;

			}

			Color& lerpHSL(const Color& color, float alpha) {
				HSL _hslA,_hslB;
				getHSL(&_hslA);
				color.getHSL(&_hslB);

				float h = math::lerp(_hslA.h, _hslB.h, alpha);
				float s = math::lerp(_hslA.s, _hslB.s, alpha);
				float l = math::lerp(_hslA.l, _hslB.l, alpha);

				setHSL(h, s, l);

				return *this;

			}

			bool equals(const Color& c) {

				return (c.r ==  r) && (c.g ==  g) && (c.b ==  b);

			}

			Color& fromArray(const std::vector<float>& array, unsigned offset = 0) {

				unsigned arrayLength = array.size();
				r = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
				g = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
				b = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();

				return *this;

			}
			Color& fromArray(const float* array, unsigned arrayLength,unsigned offset=0) {
							

				r = offset < arrayLength ? array[offset] : std::numeric_limits<float>::quiet_NaN();
				g = offset < arrayLength ? array[offset + 1] : std::numeric_limits<float>::quiet_NaN();
				b = offset < arrayLength ? array[offset + 2] : std::numeric_limits<float>::quiet_NaN();

				return *this;

			}
			std::string to_string() {
				return getHexString();
			}
			void toArray(float* array, unsigned offset=0) {
						

				array[offset] = r;
				array[offset + 1] = g;
				array[offset + 2] = b;
			}
		};

		
	}

}
#endif
