#ifndef FOG_THREE_H
#define FOG_THREE_H
#include <string>
#include <memory>
#include <limits>
#include <three/math/Color.h>
using namespace three::math;
namespace three {
	class Fog {
	public:
		using ptr = std::shared_ptr<Fog>;

		std::string name;
		std::string type;
		Color color;
		float _near = 1.0f;
		float _far = 1000.0f;

		Fog() : type("Fog") {}

		Fog(const Fog& source) : Fog() {
			name = source.name;
			color = source.color;
			_near = source._near;
			_far = source._far;
		}

		Fog(const Color& color, float _near = std::numeric_limits<float>::quiet_NaN(), float _far = std::numeric_limits<float>::quiet_NaN()){
			name = "";
			type = "Fog";
			this->color = color;
			this->_near = !std::isnan(_near)?_near : 1.0f;
			this->_far = !std::isnan(_far) ? _far : 1000.0f;
		}

		virtual Fog* clone() {
			return new Fog(*this);
		}

		virtual Fog& copy(const Fog& source) {
			name = source.name;
			color = source.color;
			_near = source._near;
			_far = source._far;

			return *this;
		}

		bool operator == (const Fog& source) const {
			return name == source.name && color == source.color && _near == source._near && _far && source._far;
		}

		static ptr create() {
			return std::make_shared<Fog>();
		}

		static ptr create(const Color& color, float _near = std::numeric_limits<float>::quiet_NaN(), float _far = std::numeric_limits<float>::quiet_NaN()) {
			return std::make_shared<Fog>(color, _near, _far);
		}
	};

	class FogExp2 : public Fog {
	public:
		using ptr = std::shared_ptr<FogExp2>;
		float density = 0.00025f;

		FogExp2() : Fog() {
			type = "FogExp2";
		}

		FogExp2(const Color& color, float density = std::numeric_limits<float>::quiet_NaN()) : Fog(color), density(density) {
			type = "FogExp2";
		}

		FogExp2(const FogExp2& source):Fog(source) {
			type = "FogExp2";
			density = source.density;
		}

		virtual FogExp2* clone() override {
			return new FogExp2(*this);
		}
		FogExp2& copy(const FogExp2& source) {
			Fog::Fog::copy(source);
			density = source.density;

			return *this;
		}

		static ptr create() {
			return std::make_shared<FogExp2>();
		}
		static ptr create(Color color, float density = std::numeric_limits<float>::quiet_NaN()) {
			return std::make_shared<FogExp2>(color, density);
		}
	};
}
#endif
