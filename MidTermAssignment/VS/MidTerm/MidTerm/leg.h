#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

#include <model.h>
#include <maths/maths_funcs.h>
#include <shader.h>

class Leg {
public:
	Model upperLegModel, lowerLegModel;
	float loc = 0;
	bool invertMovement;
	float legMovOffset;
	//string path;
	glm::mat4 pos;
	glm::mat4 parentMat;
	glm::vec3 orientation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 front = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 right = glm::vec3(0.0f, 1.0f, -1.0f);

	Leg() {}

	Leg(string const& upperPath, string const& lowerPath, glm::mat4 startingPos, bool invert, float legOffset) {
		this->upperLegModel = Model(upperPath);
		this->lowerLegModel = Model(lowerPath);
		this->pos = startingPos;  
		this->invertMovement = invert;
		this->legMovOffset = legOffset;
	}

	void Init() {
		
	}

	void Draw(Shader& shader) {
		glm::mat4 upperModel = this->pos;

		//glm::mat4 upperModel = glm::mat4(1.0f);
		//std::cout << this->pos.x << " " << this->pos.y << " " << this->pos.z << "\n";
		
		upperModel =  this->parentMat * upperModel;
		
		upperModel = glm::rotate(upperModel, getSinRotation(), glm::vec3(0.0f, 0.0f, 1.0f));
		upperModel = glm::translate(upperModel, glm::vec3(0.0f, 0.3f, 0.0f));
	
			
		
		shader.setMat4("model", upperModel);
		upperLegModel.Draw(shader);

		glm::mat4 lowerModel = glm::mat4(1.0f);
		//lowerModel = glm::translate(lowerModel, glm::vec3(0.0f, 0.0f, 0.0f));
		lowerModel = upperModel * lowerModel;
		//lowerModel = lowerModel * upperModel;
		lowerModel = glm::translate(lowerModel, glm::vec3(0.0f, 0.6f, 0.0f));
		float lowerLegRotOffset = 20;
		float lowerSin = getLowerLegHeight();
		lowerModel = glm::rotate(lowerModel, lowerSin + lowerLegRotOffset, glm::vec3(1.0f, 0.0f, 0.0f));
		lowerModel = glm::translate(lowerModel, glm::vec3(0.0f, 0.2f, 0.2f));

		shader.setMat4("model", lowerModel);
		lowerLegModel.Draw(shader);


		//glm::normalize(this->pos);
	}

	float getSinRotation() {
		float rot = sin(this->loc + this->legMovOffset) / 6;
		//std::cout << "angle" << this->loc << " " << rot << "\n";
		return this->invertMovement ? -rot : rot;
	}

	float getLowerLegHeight() {
		//std::cout << "slope" << this->loc << " " << cos(this->loc + this->legMovOffset) << "\n";
		//float invertOffset = this->invertMovement && false ? atan(1) * 2 : 0;
		float angleVal = cos(this->loc + this->legMovOffset);
		if (angleVal > 0.0f)
			return 0;
		return angleVal / 4;
	}

	void updateLoc(float loc) {
		this->loc = loc;
		//std::cout << getSinRotation() << "  " << getLowerLegHeight() << "  " << loc << "\n";
		//std::cout << "slope " << this->loc << " " << cos(this->loc + this->legMovOffset) << "\n";
	}

	void updateMat(glm::mat4 parentMat) {
		this->parentMat = parentMat;
	}
};