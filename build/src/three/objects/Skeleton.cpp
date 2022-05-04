#include "Skeleton.h"
namespace three {
	Skeleton::Skeleton(const std::vector<Bone>& bones, const std::vector<Matrix4>& boneInverses)
	{
		//std::copy(bones.begin(), bones.end(), this->bones.begin());
		this->bones=bones;
		this->boneMatrices = std::vector<unsigned char>(bones.size() * 16);

		if (boneInverses.size() == 0) {
			calculateInverses();
		}
		else {
			if (this->bones.size() == boneInverses.size()) {
				this->boneInverses = boneInverses;
			}
			else {
				for (int i = 0;i < this->bones.size();i++)
					this->boneInverses.push_back(Matrix4());
			}
		}
	}
	void Skeleton::calculateInverses()
	{
		if (this->boneInverses.size() > 0) {
			boneInverses.clear();
			//std::vector<Matrix4>().swap(this->boneInverses);
		}

		for (int i = 0;this->bones.size(); i++) {

			auto inverse = Matrix4();					

			inverse.getInverse(this->bones[i].matrixWorld);
					

			boneInverses.push_back(inverse);

		}
	}
	void Skeleton::pose()
	{
	}
	void Skeleton::update()
	{
	}
	Bone* Skeleton::getBoneByName(const std::string& name)
	{
		return nullptr;
	}
	void Skeleton::dispose()
	{
	}
}