#include "cShaderManager.h"
#include "stdafx.h"
#include "global.h"



cShaderManager *cShaderManager::instance() {
	if (!s_cShaderManager)
		s_cShaderManager = new cShaderManager();
	return s_cShaderManager;
}

GLuint cShaderManager::loadShaders(const char *vertex_file_path,
	const char *fragment_file_path) {
	// Create the shader
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		std::cout << "Impossible to open " << vertex_file_path
			<< ". Are you in the right directory?" << std::endl;
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (std::getline(FragmentShaderStream, Line)) {
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << vertex_file_path << std::endl;
	;
	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
			&VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader : " << fragment_file_path;
	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
			&FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}

	// Link the program
	std::cout << "\nLinking program ^ " << std::endl;
	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
			&ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

GLuint cShaderManager::loadShaders(const char *vertex_file_path,
	const char *fragment_file_path,
	const char *geometry_file_path) {
	// Create the shader
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint GeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if (VertexShaderStream.is_open()) {
		std::string Line = "";
		while (getline(VertexShaderStream, Line))
			VertexShaderCode += "\n" + Line;
		VertexShaderStream.close();
	}
	else {
		std::cout << "Impossible to open " << vertex_file_path
			<< ". Are you in the right directory?" << std::endl;
		return 0;
	}

	// Read the Geometry Shader code from the file
	std::string GeometryShaderCode;
	std::ifstream GeometryShaderStream(geometry_file_path, std::ios::in);
	if (GeometryShaderStream.is_open()) {
		std::string Line = "";
		while (getline(GeometryShaderStream, Line))
			GeometryShaderCode += "\n" + Line;
		GeometryShaderStream.close();
	}
	else {
		std::cout << "Impossible to open " << geometry_file_path
			<< ". Are you in the right directory?" << std::endl;
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if (FragmentShaderStream.is_open()) {
		std::string Line = "";
		while (std::getline(FragmentShaderStream, Line)) {
			FragmentShaderCode += "\n" + Line;
		}
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;

	// Compile Vertex Shader
	std::cout << "Compiling shader : " << vertex_file_path << std::endl;
	;
	char const *VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL,
			&VertexShaderErrorMessage[0]);
		std::cout << &VertexShaderErrorMessage[0] << std::endl;
	}
	// Compile Geometry Shader
	std::cout << "Compiling shader : " << geometry_file_path << std::endl;
	;
	char const *GeometrySourcePointer = GeometryShaderCode.c_str();
	glShaderSource(GeometryShaderID, 1, &GeometrySourcePointer, NULL);
	glCompileShader(GeometryShaderID);

	// Check Geometry Shader
	glGetShaderiv(GeometryShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(GeometryShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> GeometryShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(GeometryShaderID, InfoLogLength, NULL,
			&GeometryShaderErrorMessage[0]);
		std::cout << &GeometryShaderErrorMessage[0] << std::endl;
	}

	// Compile Fragment Shader
	std::cout << "Compiling shader : " << fragment_file_path;
	char const *FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL,
			&FragmentShaderErrorMessage[0]);
		std::cout << &FragmentShaderErrorMessage[0] << std::endl;
	}

	// Link the program
	std::cout << "\nLinking program ^ " << std::endl;
	GLuint ProgramID = glCreateProgram();

	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, GeometryShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if (InfoLogLength > 0) {
		std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL,
			&ProgramErrorMessage[0]);
		std::cout << &ProgramErrorMessage[0] << std::endl;
	}

	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, GeometryShaderID);
	glDetachShader(ProgramID, FragmentShaderID);

	glDeleteShader(VertexShaderID);
	glDeleteShader(GeometryShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}


bool cShaderManager::setupTheShaders() {
		//Create and compile our GLSL program from the shaders
		gProgramID = ::g_pShaderManager->loadShaders(
			"../GraphicsEngine/Simple.vs.glsl", "../GraphicsEngine/Simple.fs.glsl", "../GraphicsEngine/Simple.gs.glsl");

		if (!gProgramID) {
			std::cout << "Shaders failed to load!!" << std::endl;
			return false;
		}

		// per item
		// Uber shader
		gUniformId_ModelMatrix = glGetUniformLocation(gProgramID, "ModelMatrix");
		gUniformId_ModelMatrixOrientation =
			glGetUniformLocation(gProgramID, "ModelMatrixOrientation");
		gUniformId_Alpha = glGetUniformLocation(gProgramID, "Alpha");
		gUniformId_ModelColor = glGetUniformLocation(gProgramID, "ModelColor");
		gUniformId_NumLights = glGetUniformLocation(gProgramID, "NUM_LIGHTS");

		gUniformId_ViewMatrix = glGetUniformLocation(gProgramID, "ViewMatrix");
		gUniformId_PojectionMatrix =
			glGetUniformLocation(gProgramID, "ProjectionMatrix");
		gUniformId_EyePosition = glGetUniformLocation(gProgramID, "EyePosition");

		gUniformId_Toggle_Lights = glGetUniformLocation(gProgramID, "Toggle_Lights");

		gUniformId_Toggle_Textures =
			glGetUniformLocation(gProgramID, "Toggle_Textures");

		gUniformId_NumTextures = glGetUniformLocation(gProgramID, "NUM_TEXTURES");
		gUniformId_Texture0 = glGetUniformLocation(gProgramID, "Texture0");
		gUniformId_Texture1 = glGetUniformLocation(gProgramID, "Texture1");
		gUniformId_Texture2 = glGetUniformLocation(gProgramID, "Texture2");
		gUniformId_Texture3 = glGetUniformLocation(gProgramID, "Texture3");

		gUniformId_Toggle_NormalAndSpecularMaps = glGetUniformLocation(gProgramID, "Toggle_NormalAndSpecularMaps");

		// Skybox Shader
		::gSkyboxShaderID =
			::g_pShaderManager->loadShaders("../GraphicsEngine/Skybox.vs.glsl", "../GraphicsEngine/Skybox.fs.glsl");
		::gUniformId_ToggleSkyboxTextures =
			glGetUniformLocation(gSkyboxShaderID, "Toggle_Skybox_Textures");
		gSkyboxVMID = glGetUniformLocation(gSkyboxShaderID, "view_matrix");

		gUniformId_SamplerCube = glGetUniformLocation(gSkyboxShaderID, "skybox");
		return true;
	
}