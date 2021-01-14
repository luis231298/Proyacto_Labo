/*---------------------------------------------------------*/
/* ----------------   Práctica 9 --------------------------*/
/*-----------------    2021-1   ---------------------------*/
/*------------- Alumno:                     ---------------*/
#include <Windows.h>

#include <glad/glad.h>
#include <glfw3.h>	//main
#include <stdlib.h>		
#include <glm/glm.hpp>	//camera y model
#include <glm/gtc/matrix_transform.hpp>	//camera y model
#include <glm/gtc/type_ptr.hpp>
#include<time.h>


#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>	//Texture

#define SDL_MAIN_HANDLED
#include <SDL/SDL.h>

#include <shader_m.h>
#include <camera.h>
#include <modelAnim.h>
#include <model.h>
#include <Skybox.h>
#include <iostream>

//#pragma comment(lib, "winmm.lib")

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void my_input(GLFWwindow *window);
void animate(void);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// camera
Camera camera(glm::vec3(0.0f, 35.0f, 10.0f));
float MovementSpeed = 1.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

// timing
const int FPS = 60;
const int LOOP_TIME = 1000 / FPS; // = 16 milisec // 1000 millisec == 1 sec
double	deltaTime = 0.0f,
lastFrame = 0.0f;

// Light
//glm::vec3 lightDirection = glm::vec3(-0.2f, -1.0f, -0.3f);
//Lighting
glm::vec3 lightPosition(0.0f, 4.0f, -10.0f);
glm::vec3 lightDirection(-1.0f, -1.0f, -1.0f);//modi 0,-1,0

// posiciones
float x = 0.0f;
float y = 0.0f;
bool animacion = false;

//variables para animacion de Nave
float	movNave_x = 0.0f,
		movNave_y = 0.0f,
		movNave_z = 0.0f,
		orientaNave = 0.0f;
bool	animacionNave = false,
		recorridoNave1 = true,
		recorridoNave2 = false,
		recorridoNave3 = false,
		recorridoNave4 = false,
		recorridoNave5 = false;
//despertador
float movD_y = 0.0f;
bool aniD = false;
bool recorridoD1 = true;
bool recorridoD2 = false;

void animate(void)
{
	/*std::cout << "posicion camara= " << camera.Position.x << " en X" << std::endl;
	std::cout << "posicion camara= " << camera.Position.y << " en Y" << std::endl;
	std::cout << "posicion camara= " << camera.Position.z << " en Z" << std::endl;*/
	if (animacion)
	{
		std::cout << "posicion camara= " << camera.Position.x << " en X" << std::endl;
		std::cout << "posicion camara= " << camera.Position.y << " en Y" << std::endl;
		std::cout << "posicion camara= " << camera.Position.z << " en Z" << std::endl;
	}

	//Animacion de la Nave
	if (animacionNave)
	{
		if (recorridoNave1)
		{
			movNave_z += 2;
			orientaNave = 0.0f;
			if (movNave_z > 100)
			{
				recorridoNave1 = false;
				recorridoNave2 = true;
			}
		}
		if (recorridoNave2)
		{
			movNave_y += 0.5;
			orientaNave = 0.0f;
			if (movNave_y > 50)
			{
				recorridoNave2 = false;
				recorridoNave3 = true;
			}
		}
		if (recorridoNave3)
		{
			movNave_x += 1.5;
			orientaNave = 90.0f;
			if (movNave_x > 70)
			{
				recorridoNave3 = false;
				recorridoNave4 = true;
			}
		}
		if (recorridoNave4)
		{
			movNave_z -= 2;
			orientaNave = 180.0f;
			if (movNave_z <= -100)
			{
				recorridoNave4 = false;
				recorridoNave5 = true;
			}
		}
		if (recorridoNave5)
		{
			movNave_y -= 0.3;
			orientaNave = 180.0f;
			if (movNave_y < 5)
			{
				recorridoNave5 = false;
			}
		}




	}
	//despertador
	if (aniD) {
		if (recorridoD1) {
			movD_y += 0.4f;
			if (movD_y >= 5.0f) {
				recorridoD1 = false;
				recorridoD2 = true;
			}
		}
		if (recorridoD2){
			movD_y -= 0.1f;
			if (movD_y <= 0.4){
				recorridoD2 = false;
				recorridoD1 = true;
				aniD = false;
			}
		}
	}



































}

int main()
{
	// glfw: initialize and configure
	// ------------------------------
	glfwInit();
	/*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// glfw window creation
	// --------------------
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Casa", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	// ---------------------------------------
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_DEPTH_TEST);

	// build and compile shaders
	// -------------------------
	//Shader staticShader("Shaders/lightVertex.vs", "Shaders/lightFragment.fs");
	Shader staticShader("Shaders/shader_Lights.vs", "Shaders/shader_Lights.fs");
	Shader skyboxShader("Shaders/skybox.vs", "Shaders/skybox.fs");

	vector<std::string> faces
	{
		"resources/skybox/derecha1.jpg",
		"resources/skybox/left1.jpg",
		"resources/skybox/top1.jpg",
		"resources/skybox/bottom1.jpg",
		"resources/skybox/front1.jpg",
		"resources/skybox/derecha2.jpg"
	};

	Skybox skybox = Skybox(faces);

	// Shader configuration
	// --------------------
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);

	// load models
	// -----------
	Model pasto("resources/objects/pisof/pasto.obj");
	Model pasillo("resources/objects/pasillo/pasillo.obj");
	Model cuartosYak("resources/objects/CuartosYak/CuartosYak.obj");
	Model cuartosLuis("resources/objects/CuartoLuis/cuarto.obj");
	Model cuartoIrma("resources/objects/Cuarto5Irma/cuartoIrma.obj");
	Model cuartoLavado("resources/objects/CuartoLav/cuartoLavado.obj");
	Model mesita("resources/objects/mesita/mesita.obj");
	Model asador("resources/objects/asador/asador.obj");
	Model nave("resources/objects/Nave/Nave.obj");
	Model cama1("resources/objects/cama/cama.obj");
	Model buro("resources/objects/buro/buro.obj");
	Model despertador("resources/objects/despertador/despertador.obj");
	Model estante("resources/objects/estante/estante.obj");
	Model lavadora1("resources/objects/lavadora/lavadora.obj");
	Model lavadora2("resources/objects/lavadora/lavadora.obj");
	Model burro("resources/objects/burro/burro.obj");
	//yak
	Model alacena("resources/objects/Alacena/Alacena.obj");
	Model asientoMesa("resources/objects/AsientoMesa/Asiento.obj");
	Model bocina("resources/objects/Bocina/Bocina.obj");
	Model cortinas("resources/objects/Cortinas/Cortinas.obj");
	Model estufa("resources/objects/Estufa/Estufa.obj");
	Model maquinita("resources/objects/Maquinita/Maquinita.obj");
	Model mesa("resources/objects/Mesa/Mesa.obj");
	Model muebleCom("resources/objects/MuebleComedor/Mueble.obj");
	Model sillonG("resources/objects/SillonG/SillonGrande.obj");
	Model sillonP("resources/objects/SillonP/SillonPequeño.obj");
	Model tele("resources/objects/TV/Tele.obj");
	Model paredes("resources/objects/Paredes/Paredes.obj");






	/////////
	//Irma
	Model regaderaP1("resources/objects/regadera/regaderaP1.obj");
	Model regaderaP2("resources/objects/regadera/regaderaP2.obj");
	Model banio("resources/objects/banio/ban.obj");
	Model tapabanio("resources/objects/banio/banTapa.obj");
	Model alberca("resources/objects/alberca/alberca.obj");
	Model banio2("resources/objects/banio/ban.obj");
	Model puertaPrinc1("resources/objects/PuertaPrincipal/puertaPrincipal.obj");
	Model puertaPrinc2("resources/objects/PuertaPrincipal/puertaPrincipal.obj");
	Model puertaPrinc3("resources/objects/PuertaPrincipal/puertaPrincipal.obj");
	Model puertaPrinc4("resources/objects/PuertaPrincipal/puertaPrincipal.obj");




	//////////
	//YO
	Model cama2("resources/objects/cama/cama.obj");
	Model estante2("resources/objects/estante/estante.obj");
	Model despertador2("resources/objects/despertador/despertador.obj");
	Model buro2("resources/objects/buro/buro.obj");
	Model lavabo1("resources/objects/lavabo/lavabo.obj");
	Model toalla1("resources/objects/toalla/toalla.obj");
	Model p1("resources/objects/puerta/puerta.obj");
	Model p2("resources/objects/puerta/puerta.obj");
	Model p3("resources/objects/puerta/puerta.obj");
	Model p4("resources/objects/puerta/puerta.obj");
	Model planta("resources/objects/planta/planta.obj");









	//////////////
	// draw in wireframe
	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// render loop
	// -----------
	while (!glfwWindowShouldClose(window))
	{
		skyboxShader.setInt("skybox", 0);

		// per-frame time logic
		// --------------------
		lastFrame = SDL_GetTicks();

		// input
		// -----
		my_input(window);
		animate();

		// render
		// ------
		glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// -------------------------------------------------------------------------------------------------------------------------
		// Personaje 1
		// -------------------------------------------------------------------------------------------------------------------------
		// don't forget to enable shader before setting uniforms
		staticShader.use();
		//Setup Advanced Lights
		staticShader.setVec3("viewPos", camera.Position);
		staticShader.setVec3("dirLight.direction", lightDirection);
		staticShader.setVec3("dirLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("dirLight.specular", glm::vec3(0.0f, 0.0f, 0.0f));

		staticShader.setVec3("pointLight[0].position", lightPosition);
		staticShader.setVec3("pointLight[0].ambient", glm::vec3(0.3f, 0.3f, 0.3f));
		staticShader.setVec3("pointLight[0].diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		staticShader.setVec3("pointLight[0].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[0].constant", 0.8f);//intensidad de la luz
		staticShader.setFloat("pointLight[0].linear", 0.009f);//cuanto viaja la luz
		staticShader.setFloat("pointLight[0].quadratic", 0.032f);//cuanto viaja la luz

		staticShader.setVec3("pointLight[1].position", glm::vec3(0.0, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].diffuse", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setVec3("pointLight[1].specular", glm::vec3(0.0f, 0.0f, 0.0f));
		staticShader.setFloat("pointLight[1].constant", 1.0f);
		staticShader.setFloat("pointLight[1].linear", 0.009f);
		staticShader.setFloat("pointLight[1].quadratic", 0.032f);

		staticShader.setFloat("material_shininess", 32.0f);



		glm::mat4 model = glm::mat4(1.0f);
		// view/projection transformations
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
		glm::mat4 view = camera.GetViewMatrix();
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		//// Light
		glm::vec3 lightColor = glm::vec3(1.0f);
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.75f);

		// -------------------------------------------------------------------------------------------------------------------------
		// Escenario
		// -------------------------------------------------------------------------------------------------------------------------
		staticShader.setMat4("projection", projection);
		staticShader.setMat4("view", view);

		/*staticShader.setVec3("material.specular", glm::vec3(0.5f));
		staticShader.setFloat("material.shininess", 32.0f);
		staticShader.setVec3("light.ambient", ambientColor);
		staticShader.setVec3("light.diffuse", diffuseColor);
		staticShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		staticShader.setVec3("light.direction", lightDirection);
		staticShader.setVec3("viewPos", camera.Position);*/

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.5f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		pasto.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-10.0f, -1.5f, 30.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		pasillo.Draw(staticShader);

		//-----------Espacio Irma--------------------------------
		//Objetos del cuarto de lavado 
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(38, -1.5f, -10.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		lavadora1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45, -1.5f, -10.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		lavadora2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(41, -1.5f, 8.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		burro.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-55, -1.0f, 45.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		regaderaP1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-55, -1.0f, 45.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		regaderaP2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-35, -1.5f, 42.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		banio.Draw(staticShader);

		/*model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-35, -1.5f, 42.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		tapabanio.Draw(staticShader);*/

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(45, -1.5f, -50.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		alberca.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(15, -1.5f, 0.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		banio2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-15, -1.5f, 64.5f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		puertaPrinc1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-7.5, -1.5f, 64.5f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		puertaPrinc2.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-13.4 , -1.5f, 2.9f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		puertaPrinc3.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-5.8, -1.5f, 2.9f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		puertaPrinc4.Draw(staticShader);




























		//-------Aqui termina espacio Irma------------------------


		//------Espacio Balam-----------------------------------------------
		//Cuartos Yak
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(35.45f, -1.5f, 53.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cuartosYak.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(65.0f+movNave_x, 5.0f+movNave_y, 56.0f+movNave_z));
		model = glm::rotate(model, glm::radians(orientaNave), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		nave.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(37.0f, -1.5f, 58.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		sillonG.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(25.0f, -1.5f, 68.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		sillonP.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(37.0f, 5.0f, 76.5f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		tele.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(30.0f, -1.5f, 77.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		bocina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(44.0f, -1.5f, 77.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		bocina.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(4.5f, -1.5f, 65.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		maquinita.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, 4.5f, 77.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cortinas.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, -1.5f, 33.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		mesa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, -1.5f, 27.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		asientoMesa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, -1.5f, 39.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		asientoMesa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(16.0f, -1.5f, 16.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		muebleCom.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(47.0f, -1.5f, 34.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		estufa.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(47.0f, 11.5f, 34.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		alacena.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(8.5f, -1.5f, -17.5f));
		//model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		paredes.Draw(staticShader);
		







		//------Aqui termina espacio Balam-------------------------------------------------------


		//------Espacio Luis-----------------------------------------------
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-39.0f, -1.5f, 63.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cuartosLuis.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-42.3f, -1.5f, 10.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cuartoIrma.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(20.3f, -1.5f, 6.2f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cuartoLavado.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-25.0f, -1.5f, -50.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		mesita.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -50.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		asador.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.5870f, 1.5f, 66.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cama1.Draw(staticShader);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.5870f, -1.5f, 68.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		buro.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-52.5f, 6.4f + movD_y, 74.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		despertador.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-23.0f, -1.5f, 72.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		estante.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-23.0f, -1.5f, 26.0f));
		model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		estante2.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.5870f, 1.5f, 30.0f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		cama2.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.5870f, 7.4f, 36.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		despertador2.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.5870f, 5.4f, 53.5f));
		model = glm::rotate(model, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		toalla1.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-53.5870f, 4.5f, 47.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		lavabo1.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.0f, -1.0f, 44.0f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		p1.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.0f, -1.0f, 35.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		p2.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.0f, -1.0f, 15.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		p3.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(-21.0f, -1.0f, 58.5f));
		model = glm::rotate(model, glm::radians(90.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		p4.Draw(staticShader);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(10.0f, -0.5f, -50.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		staticShader.setMat4("model", model);
		planta.Draw(staticShader);
		////////////
		// -------------------------------------------------------------------------------------------------------------------------
		// Termina Escenario
		// -------------------------------------------------------------------------------------------------------------------------

		//-------------------------------------------------------------------------------------
		// draw skybox as last
		// -------------------
		skyboxShader.use();
		skybox.Draw(skyboxShader, view, projection, camera);

		// Creo que limitan el framerate a 60
		deltaTime = SDL_GetTicks() - lastFrame; // time for full 1 loop
		//std::cout <<"frame time = " << frameTime << " milli sec"<< std::endl;
		if (deltaTime < LOOP_TIME)
		{
			SDL_Delay((int)(LOOP_TIME - deltaTime));
		}

		// glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		// -------------------------------------------------------------------------------
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	skybox.Terminate();

	glfwTerminate();
	return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
		if (animacion)
			animacion = false;
		else
			animacion = true;
	}

	if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		camera.MovementSpeed = MovementSpeed * 2.5f;
	else
		camera.MovementSpeed = MovementSpeed;

	//animacion de la Nave
	if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
		animacionNave ^= true;
	
	
	
	
	
	
	
	
	
	//pos camara
	if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {//frente
		camera.Position.x = -18.7272f;
		camera.Position.y = 11.7076f;
		camera.Position.z = 156.769f;
	}
	if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {//cuarto
		camera.Position.x = 30.0306f;
		camera.Position.y = 13.3445f;
		camera.Position.z = 65.7507f;
	}
	if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {//alberca
		camera.Position.x = 10.6137f;
		camera.Position.y = 34.9446f;
		camera.Position.z = -104.339f;
	}
	if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {//isometrico
		camera.Position.x = 126.49f;
		camera.Position.y = 73.326f;
		camera.Position.z = 116.869f;
	}
	if (glfwGetKey(window, GLFW_KEY_9) == GLFW_PRESS)
		aniD ^= true;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}
// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}