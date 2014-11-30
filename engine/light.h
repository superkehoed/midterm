typedef struct Light_T
{
	Vec3f color;
	GLfloat brightness; 
	Vec3f offset;
	struct Light_T *next;
}Light_T;