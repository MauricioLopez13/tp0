#include "client.h"

int main(void)
{
	/*---------------------------------------------------PARTE 2-------------------------------------------------------------*/

	int conexion;
	char* ip;
	char* puerto;
	char* valor;

	t_log* logger;
	t_config* config;

	/* ---------------- LOGGING ---------------- */

	logger = iniciar_logger();

	// Usando el logger creado previamente
	// Escribi: "Hola! Soy un log"
	log_info(logger,"Hola! Soy un log");

	/* ---------------- ARCHIVOS DE CONFIGURACION ---------------- */

	config = iniciar_config();

	// Usando el config creado previamente, leemos los valores del config y los 
	// dejamos en las variables 'ip', 'puerto' y 'valor'
	ip = config_get_string_value(config,"IP");
	puerto= config_get_string_value(config,"PUERTO");
	valor= config_get_string_value(config,"CLAVE");

	// Loggeamos el valor de config
	log_info(logger,"VALOR leido de la config: %s",valor);


	/* ---------------- LEER DE CONSOLA ---------------- */

	leer_consola(logger);

	/*---------------------------------------------------PARTE 3-------------------------------------------------------------*/

	// ADVERTENCIA: Antes de continuar, tenemos que asegurarnos que el servidor esté corriendo para poder conectarnos a él

	// Creamos una conexión hacia el servidor
	conexion = crear_conexion(ip, puerto);

	// Enviamos al servidor el valor de CLAVE como mensaje
	enviar_mensaje(valor,conexion);
	// Armamos y enviamos el paquete
	paquete(conexion);

	terminar_programa(conexion, logger, config);

	/*---------------------------------------------------PARTE 5-------------------------------------------------------------*/
	// Proximamente

	printf("\nCLIENTE CERRADO !!!!!");
}

t_log* iniciar_logger(void)
{
	t_log* nuevo_logger = log_create ("tp0.log","LOGGER_TP0",1,LOG_LEVEL_INFO);

	if(nuevo_logger== NULL){

		perror ("Algo raro paso con el log. No se pudo crear o encontrar el archivo.");
		
		exit(EXIT_FAILURE);	
		//abort()
	}

	return nuevo_logger;
}

t_config* iniciar_config(void)
{
	t_config* nuevo_config = config_create("/home/utnso/tp0/client/cliente.config");
	if(nuevo_config == NULL){
		perror("Error al intentar cargar el config.");

		exit(EXIT_FAILURE);
		//abort()
	}
		
	return nuevo_config;
}

void leer_consola(t_log* logger)
{
	char* leido;

	// La primera te la dejo de yapa
	leido = readline("> ");
	log_info(logger,">> %s", leido);

	// El resto, las vamos leyendo y logueando hasta recibir un string vacío
	while(strcmp(leido, "")!=0){
		free(leido);
		leido = readline("> ");
		log_info(logger,">> %s", leido);
		
		//free(leido); MAL!! Yo lo leeia, lo loggeaba y lo liberaba. Al volver al while nunca iba a ser = 0.
		//Esto es debido a que siempre lo terminaba liberando, por mas que fuera una linea vacia al liberar ya no tiene información.
		//Una linea vacia (char */string vacio) es un \0 = "" (caracter nulo).
		//Estaba comparando algo que no tenia información con la linea vacia "" y por lo tanto nunca iba a ser igual a 0.
		//De ahi surgia el Bucle.
		//El free te borra todo y el centinela \0 incluido.
	}

	// ¡No te olvides de liberar las lineas antes de regresar!
	free(leido);
}

void paquete(int conexion)
{
	// Ahora toca lo divertido!
	char* leido;
	t_paquete* paquete = crear_paquete();

	// Leemos y esta vez agregamos las lineas al paquete
	leido = readline("> ");
	while(strcmp(leido, "")!=0){
		agregar_a_paquete(paquete,leido,strlen(leido) + 1); // "+ 1"  del centinela \0
		free(leido);
		leido = readline("> ");
	}
	// ¡No te olvides de liberar las líneas y el paquete antes de regresar!
	free (leido);

	//ENVIAR PAQUETE:
	enviar_paquete(paquete,conexion);

	//ELIMINAR PAQUETE:
	eliminar_paquete(paquete);
}

void terminar_programa(int conexion, t_log* logger, t_config* config)
{
	/* Y por ultimo, hay que liberar lo que utilizamos (conexion, log y config) 
	  con las funciones de las commons y del TP mencionadas en el enunciado */
	  log_destroy(logger);
	  config_destroy(config);
	  liberar_conexion(conexion);
}
