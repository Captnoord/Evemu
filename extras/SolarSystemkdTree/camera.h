/**
	
  Camera with OpenGL

  March, 13th, 2003

  This tutorial was written by Philipp Crocoll
  Contact: 
	philipp.crocoll@web.de
	www.codecolony.de

  Every comment would be appreciated.

  If you want to use parts of any code of mine:
	let me know and
	use it!

**/


#define PI 3.1415926535897932384626433832795
#define PIdiv180 (PI/180.0)

/////////////////////////////////
//Note: All angles in degrees  //
/////////////////////////////////

struct SF3dVector  //Float 3d-vect, normally used
{
	double x,y,z;
};
struct SF2dVector
{
	double x,y;
};
SF3dVector F3dVector ( double x, double y, double z );


class CCamera
{
private:
	
	SF3dVector ViewDir;
	SF3dVector RightVector;	
	SF3dVector UpVector;
	SF3dVector Position;

	double RotatedX, RotatedY, RotatedZ;	
	
public:
	CCamera();				//inits the values (Position: (0|0|0) Target: (0|0|-1) )
	void Render ( void );	//executes some glRotates and a glTranslate command
							//Note: You should call glLoadIdentity before using Render

	void Move ( SF3dVector Direction );
	void RotateX ( double Angle );
	void RotateY ( double Angle );
	void RotateZ ( double Angle );

	void MoveForward ( double Distance );
	void MoveUpward ( double Distance );
	void StrafeRight ( double Distance );

};


