#include "Actor.h"

//--------------------------------------------------------------
Actor::Actor() : Actor(ofColor(39)) { }

//--------------------------------------------------------------
Actor::Actor(ofColor color) {

	this->location = glm::vec3(ofRandom(-300, 300), ofRandom(-300, 300), ofRandom(-300, 300));
	this->velocity = glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1));

	this->range = 40;
	this->max_force = 0.5;
	this->max_speed = 6;

	this->color = color;
	this->len = 30;
}

//--------------------------------------------------------------
Actor::~Actor() {

}

//--------------------------------------------------------------
void Actor::think(vector<unique_ptr<Actor>>& Actors) {

	// •ª—£
	glm::vec3 separate = this->separate(Actors);
	this->applyForce(separate * 1.05);

	// ®—ñ
	glm::vec3 align = this->align(Actors);
	this->applyForce(align);

	// Œ‹‡
	glm::vec3 cohesion = this->cohesion(Actors);
	this->applyForce(cohesion);

	// Ž©‰ä
	if (glm::length(this->velocity) > 0) {

		glm::vec3 feature = this->location + glm::normalize(velocity) * this->range;
		glm::vec3 target = feature + glm::normalize(glm::vec3(ofRandom(-1, 1), ofRandom(-1, 1), ofRandom(-1, 1))) * this->range * 0.5;
		glm::vec3 ego = this->seek(target);

		this->applyForce(ego);
	}

	// ‹«ŠE
	if (glm::length(this->location - glm::vec3()) > 600) {

		glm::vec3 area = this->seek(glm::vec3());
		this->applyForce(area * 5);
	}
}

//--------------------------------------------------------------
void Actor::update() {

	// ‘Oi
	this->velocity += this->acceleration;
	if (glm::length(this->velocity) > this->max_speed) {

		this->velocity = glm::normalize(this->velocity) * this->max_speed;
	}
	this->location += this->velocity;
	this->acceleration *= 0;
	this->velocity *= 0.9;

	this->log.push_front(this->location);
	while (this->log.size() > this->len) {

		this->log.pop_back();
	}
}

//--------------------------------------------------------------
void Actor::draw() {

	ofSetColor(color);
	ofDrawSphere(this->location, 3);
	for (int i = 0; i < this->log.size() - 1; i++) {

		ofSetColor(color, ofMap(i, 0, this->log.size() - 1, 255, 0));
		ofDrawLine(this->log[i], this->log[i + 1]);
	}
}

//--------------------------------------------------------------
glm::vec3 Actor::separate(vector<unique_ptr<Actor>>& Actors) {

	float tmp_range = this->range * 0.5;

	glm::vec3 result;
	glm::vec3 sum;
	int count = 0;
	for (auto& other : Actors) {

		glm::vec3 difference = this->location - other->location;
		float len = glm::length(difference);
		if (len > 0 && len < tmp_range) {

			sum += glm::normalize(difference);
			count++;
		}
	}

	if (count > 0) {

		glm::vec3 avg = sum / count;
		avg = avg * this->max_speed;
		if (glm::length(avg) > this->max_speed) {

			avg = glm::normalize(avg) * this->max_speed;
		}
		glm::vec3 steer = avg - this->velocity;
		if (glm::length(steer) > this->max_force) {

			steer = glm::normalize(steer) * this->max_force;
		}
		result = steer;
	}

	return result;
}

//--------------------------------------------------------------
glm::vec3 Actor::align(vector<unique_ptr<Actor>>& Actors) {

	float tmp_range = this->range;

	glm::vec3 result;
	glm::vec3 sum;
	int count = 0;
	for (auto& other : Actors) {

		glm::vec3 difference = this->location - other->location;
		float len = glm::length(difference);
		if (len > 0 && len < tmp_range) {

			sum += other->velocity;
			count++;
		}
	}

	if (count > 0) {

		glm::vec3 avg = sum / count;
		avg = avg * this->max_speed;
		if (glm::length(avg) > this->max_speed) {

			avg = glm::normalize(avg) * this->max_speed;
		}
		glm::vec3 steer = avg - this->velocity;
		if (glm::length(steer) > this->max_force) {

			steer = glm::normalize(steer) * this->max_force;
		}
		result = steer;
	}

	return result;
}

//--------------------------------------------------------------
glm::vec3 Actor::cohesion(vector<unique_ptr<Actor>>& Actors) {

	float tmp_range = this->range * 0.5;

	glm::vec3 result;
	glm::vec3 sum;
	int count = 0;
	for (auto& other : Actors) {

		glm::vec3 difference = this->location - other->location;
		float len = glm::length(difference);
		if (len > 0 && len < tmp_range) {

			sum += other->location;
			count++;
		}
	}

	if (count > 0) {

		result = this->seek(sum / count);
	}

	return result;
}

//--------------------------------------------------------------
glm::vec3 Actor::seek(glm::vec3 target) {

	glm::vec3 desired = target - this->location;
	float distance = glm::length(desired);
	desired = glm::normalize(desired);
	desired *= distance < this->range ? ofMap(distance, 0, this->range, 0, this->max_speed) : max_speed;
	glm::vec3 steer = desired - this->velocity;
	if (glm::length(steer) > this->max_force) {

		steer = glm::normalize(steer) * this->max_force;
	}
	return steer;
}

//--------------------------------------------------------------
void Actor::applyForce(glm::vec3 force) {

	this->acceleration += force;
}

//--------------------------------------------------------------
glm::vec3 Actor::get_location() {

	return this->location;
}

//--------------------------------------------------------------
glm::vec3 Actor::get_last_log() {

	return this->log.back();
}