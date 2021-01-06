#include "Camera.h"

// Constructor with vectors
Camera::Camera(glm::vec3 position, glm::vec3 direction, glm::vec3 up)
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	this->Position = position;
	this->Jump = Camera_Jump::NONE;
	this->WorldUp = glm::normalize(up);
	this->Direction = glm::normalize(direction);

	//Set pitch, yaw value 
	this->Pitch = glm::degrees(glm::asin((this->Direction.y) / (glm::length(this->Direction))));
	this->Yaw = glm::degrees(glm::asin((this->Direction.z) / (glm::length(this->Direction - glm::vec3(0.0f, this->Direction.y, 0.0f)))));
	this->updateCameraVectors();
}

// Constructor with scalar values
Camera::Camera(float posX, float posY, float posZ, float directionX, float directionY, float directionZ, float upX, float upY, float upZ)
	: MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
{
	this->Position = glm::vec3(posX, posY, posZ);
	this->Jump = Camera_Jump::NONE;
	this->WorldUp = glm::vec3(upX, upY, upZ);
	this->Direction = glm::vec3(directionX, directionY, directionZ);

	//Set pitch, yaw value 
	this->Pitch = glm::degrees(glm::asin((this->Direction.y) / (glm::length(this->Direction))));
	this->Yaw = glm::degrees(glm::asin((this->Direction.z) / (glm::length(this->Direction - glm::vec3(0.0f, this->Direction.y, 0.0f)))));
	this->updateCameraVectors();
}

// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
glm::mat4 Camera::GetViewMatrix()
{
	return glm::lookAt(this->Position, this->Position + this->Direction, this->Up);
}

// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime)
{
	float velocity = this->MovementSpeed * deltaTime;
	if (direction == FORWARD)
		this->Position += this->Direction * velocity;
	if (direction == BACKWARD)
		this->Position -= this->Direction * velocity;
	if (direction == LEFT)
		this->Position -= this->Right * velocity;
	if (direction == RIGHT)
		this->Position += this->Right * velocity;

	/*if(this->Jump == Camera_Jump::NONE && this->Position.y <= 3.2f &&  this->Position.y >= 1.5f)
	{
		this->Position.y = 1.5f;
	}*/
	//this->Position.y = 0.0f;
}

// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch)
{
	xoffset *= this->MouseSensitivity;
	yoffset *= this->MouseSensitivity;

	this->Yaw += xoffset;
	this->Pitch += yoffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (this->Pitch > 89.0f)
			this->Pitch = 89.0f;
		if (this->Pitch < -89.0f)
			this->Pitch = -89.0f;
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	this->updateCameraVectors();
}

// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
void Camera::ProcessMouseScroll(float yoffset)
{
	if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
		this->Zoom -= yoffset;
	if (this->Zoom <= 1.0f)
		this->Zoom = 1.0f;
	if (this->Zoom >= 45.0f)
		this->Zoom = 45.0f;
}

// Calculates the front vector from the Camera's (updated) Eular Angles
void Camera::updateCameraVectors()
{
	// Calculate the new Front vector
	glm::vec3 direction;
	direction.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	direction.y = sin(glm::radians(this->Pitch));
	direction.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
	this->Direction = glm::normalize(direction);
	// Also re-calculate the Right and Up vector
	this->Right = glm::normalize(glm::cross(this->Direction, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
	this->Up = glm::normalize(glm::cross(this->Right, this->Direction));
}


