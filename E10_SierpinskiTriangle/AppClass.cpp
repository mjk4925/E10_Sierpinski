#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	//Using Base InitWindow method
	super::InitWindow("E10 Kurtz Instance Rendering");
}

void AppClass::InitVariables(void)
{
	//Reserve Memory for a MyMeshClass object
	m_pMesh = new MyMesh();

	float recTriHeight = triangleHeight / (2.0f*recursions);
	float recTriWidth = triangleHeight / (tanf(PI / 3) *(2.0f*recursions));


	m_pMesh->AddVertexPosition(vector3(recTriWidth, 0.0f, 0.0f));
	m_pMesh->AddVertexPosition(vector3(0.0f, recTriHeight, 0.0f));
	m_pMesh->AddVertexPosition(vector3(-recTriWidth, 0.0f, 0.0f));

	m_pMesh->AddVertexColor(REGREEN);
	m_pMesh->AddVertexColor(RERED);
	m_pMesh->AddVertexColor(REBLUE);

	m_pMesh->CompileOpenGL3X();

	m_fMatrixArray = new float[m_nObjects * 16];

	sierpinski(recTriHeight, recTriWidth, recursions - 1, 
		vector3(0.0f,recTriHeight,0.0f), 0, 3);

/*	for (int nObject = 0; nObject < m_nObjects; nObject++)
	{
		const float* m4MVP = glm::value_ptr(
			glm::translate(vector3(0.01f * -nObject, 0.0f, 1.0f * -nObject)) *
			glm::rotate(REIDENTITY, nObject * 5.0f, REAXISZ)
			);
		memcpy(&m_fMatrixArray[nObject * 16], m4MVP, 16 * sizeof(float));
	}
	*/
}

void AppClass::sierpinski(float triHeight, float triWidth, int rec, vector3 origin, int triN, int tCase)
{
	//draw origin plus each child on side
	const float* m4MVP = glm::value_ptr(glm::translate(origin));
	memcpy(&m_fMatrixArray[triN * 16], m4MVP, 16 * sizeof(float));
	triN++;
	//right
	m4MVP = glm::value_ptr(glm::translate(origin + vector3(triWidth,-triHeight,0.0f)));
	memcpy(&m_fMatrixArray[triN * 16], m4MVP, 16 * sizeof(float));
	triN++;
	//left
	m4MVP = glm::value_ptr(glm::translate(origin + vector3(-triWidth, -triHeight,0.0f)));
	memcpy(&m_fMatrixArray[triN * 16], m4MVP, 16 * sizeof(float));
	triN++;

	rec--;
	if (rec == 0)
		return;
	if (rec % 2 == 0)
	{
		sierpinski(triHeight, triWidth, rec, 
			(origin + vector3(2 * triWidth, -2 * triHeight, 0.0f)), triN, 1);
		sierpinski(triHeight, triWidth, rec, 
			(origin + vector3(-2 * triWidth, -2 * triHeight, 0.0f)), triN + (3 * rec), 2);
	}
	else
	{
		//recurs right/left
		if (tCase == 1)
		{
			sierpinski(triHeight, triWidth, rec, 
				(origin + vector3(2 * triWidth, -2 * triHeight, 0.0f)), triN, 3);
		}
		else if (tCase == 2)
		{
			sierpinski(triHeight, triWidth, rec, 
				(origin + vector3(-2 * triWidth, -2 * triHeight, 0.0f)), triN + (3 * rec), 3);
		}
		else
		{
			sierpinski(triHeight, triWidth, rec, 
				(origin + vector3(2 * triWidth, -2 * triHeight, 0.0f)), triN, 3);
			sierpinski(triHeight, triWidth, rec, 
				(origin + vector3(-2 * triWidth, -2 * triHeight, 0.0f)), triN + (3 * rec), 3);
		}
	}
}

void AppClass::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->UpdateTime();

	//Is the arcball active?
	if (m_bArcBall == true)
		m_qArcBall = ArcBall();

	//Is the first person camera active?
	if (m_bFPC == true)
		CameraRotation();

	//Calculate Camera
	m_pCamera->CalculateView();

	//print info into the console
	printf("FPS: %d            \r", m_pSystem->GetFPS());//print the Frames per Second
}

void AppClass::Display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear the window

	m_pGrid->Render(1.0f, REAXIS::XZ); //renders the grid with a 100 scale

	m_pMesh->RenderList(m_fMatrixArray, m_nObjects);//Rendering nObjects

	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	SafeDelete(m_fMatrixArray);
	if (m_pMesh != nullptr)
	{
		delete m_pMesh;
		m_pMesh = nullptr;
	}
	super::Release();
}