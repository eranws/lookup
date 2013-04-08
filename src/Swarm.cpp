#include "Swarm.h"

// This 'swarm' object demonstrates a simple particle system
//  with 'simple harmonic motion'

swarm::swarm(){
	light.setAmbientColor(ofColor(0, 0, 0));
}

void swarm::init(float positionDispersion, float velocityDispersion){

	ofDirectory dir;
	//int nFiles = dir.listDir("plops");
	int nFiles = dir.listDir("PNG_yellow_new");

	if(nFiles) {        
		for(int i=0; i<dir.numFiles(); i++) { 
			// add the image to the vector
			string filePath = dir.getPath(i);
			images.push_back(ofImage());
			images.back().loadImage(filePath);
		}

		for(int i = 0; i < dir.numFiles(); i++) {
			images[i].setAnchorPercent(0.5f, 0.5f);
			images[i].rotate90(2);
		}
	}

	// Check if we've already initialised
	if(particles.size() != 0){
		// clear out old data
		ofLogWarning("swarm") <<  "Swarm: Already initialised";

		particles.clear();
	}

	_velocityDispersion = velocityDispersion;
	_positionDispersion = positionDispersion;
}

void swarm::addParticle(int nParticles)
{
	for (int i = 0; i < nParticles; i++){
		particles.push_back(Bird(_positionDispersion, _velocityDispersion));
	}
}

void swarm::customDraw(){

	// We run the update ourselves manually. ofNode does
	//  not do this for us.
	update();

	//--
	// Draw particles

	// We use the position of the first
	//  particle as the position of the
	//  light.
	ofPushStyle();
	light.enable();
	light.setPosition(particles[0].getGlobalPosition());

	for(int i = 0; i < particles.size(); i++){
		ofPushStyle();
		ofSetColor(particles[i].color);
		ofSphere(particles[i].getGlobalPosition(), 1.0);
		ofDrawArrow(particles[i].getGlobalPosition(), particles[i].getGlobalPosition() + particles[i].velocity);

		ofPushMatrix();
		ofTranslate(particles[i].getGlobalPosition());
		
		ofScale(0.1, 0.1);

		int frameIndex = ofGetFrameNum() - particles[i].animationFrameStart; //fix animation not starting in 0;
		int animationCounter = (frameIndex + particles[i].birth) / images.size();
		ofDrawBitmapString(ofToString(animationCounter), 0, 0, 0);
		int glidingFactor = 10; //flap wings each X times
		if ((animationCounter + particles[i].glideFrameStart) % glidingFactor > 0) frameIndex = particles[i].animationFrameStart;

		// draw the image sequence at the new frame count
		ofSetColor(255);

		ofRotateZ(atan2f(particles[i].velocity.y, particles[i].velocity.x) * RAD_TO_DEG);
		images[frameIndex % images.size()].draw(0, 0);
		ofPopMatrix();


		ofPopStyle();
	}

	light.disable();
	ofDisableLighting();

	//
	//--



	// Render light as white sphere
	ofSetColor(255, 255, 255);
	ofSphere(light.getPosition(), 2.0);
	ofSetDrawBitmapMode(OF_BITMAPMODE_MODEL);
	ofDrawBitmapString(" light", particles[0].getGlobalPosition());
	ofPopStyle();
}

void swarm::update(){

	// Calculate time past per frame
	float dt = ofGetLastFrameTime();

	// Update positions, velocities
	for(int i = 0; i < particles.size(); i++){

		// -----------
		//
		//	MOTION MATHS
		//
		//		'Simple Harmonic Motion' + a little extra
		// ----
		//

		// [1] apply velocity to postion
		//  (i.e. integrate velocity)
		//
		//  v = dx / dt (*)
		//  x = x + dx [every frame]
		//
		// therefore
		//  x = x + v * dt (*)
		//

		// (velcotity is taken from previous frame)
		particles[i].move(particles[i].velocity * dt);



		// [2] apply spring force to velocity
		//  (i.e. integrate acceleration)
		//
		//  a = -k * x (this is the shm restoring force, aka spring force)
		//  a = dv / dt
		//
		// therefore from (*)s above
		//  (v = v + dv)
		//
		//  v = v + (dt * a)
		//  v = v + (dt * -k * x)
		//
		particles[i].velocity += -SPRING_CONSTANT * particles[i].getGlobalPosition() * dt;


		// [3] to get a super simple kind of 'flocking' behaviour
		//  we add a second spring force to velocity relative
		//  to the position of the light
		// NOTICE: THIS ISN'T REAL FLOCKING!
		particles[i].velocity += -SPRING_CONSTANT * (particles[i].getGlobalPosition() - light.getPosition()) * dt;


		// [4] Force a maximum velocity
		if(particles[i].velocity.length() > MAX_VELOCITY){
			particles[i].velocity /= particles[i].velocity.length() * MAX_VELOCITY;
		}

		//
		// -----------

	}
}

int swarm::size()
{
	return particles.size();
}
